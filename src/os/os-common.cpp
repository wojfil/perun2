/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "os.h"
#include <time.h>
#include "../perun2.h"
#include "../datatype/parse/parse-asterisk.h"
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <algorithm>
#include <fstream>
#include <combaseapi.h>
#include <fcntl.h>


namespace perun2
{

p_tim os_tomorrow()
{
   p_tim t = os_today();
   t.addDays(1);
   return t;
}

p_tim os_yesterday()
{
   p_tim t = os_today();
   t.addDays(-1);
   return t;
}

void os_sleepForMs(const p_nint ms, p_perun2& p2)
{
   if (ms <= NINT_ZERO) {
      return;
   }

   p_nint loops = ms / OS_SLEEP_UNIT;
   const p_nint remainder = ms % OS_SLEEP_UNIT;

   os_rawSleepForMs(remainder);

   while (p2.state == State::s_Running && loops != NINT_ZERO) {
      os_rawSleepForMs(OS_SLEEP_UNIT);
      loops--;
   }
}

// attributes of something, that cannot exist (file name is empty string, contains not allowed chars, ...)
void os_loadEmptyAttributes(FileContext& context)
{
   const p_attrptr& attribute = context.attribute;

   if (attribute->has(ATTR_PATH)) {
      context.v_path->value.clear();
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value.clear();
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = P_NaN;
   }

   if (attribute->has(ATTR_DRIVE)) {
      context.v_drive->value.clear();
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   context.v_exists->value = false;
   context.v_isfile->value = false;
   context.v_isdirectory->value = false;

   if (attribute->has(ATTR_ACCESS)) {
      context.v_access->value = p_tim();
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      context.v_archive->value = false;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      context.v_compressed->value = false;
   }

   if (attribute->has(ATTR_CHANGE)) {
      context.v_change->value = p_tim();
   }

   if (attribute->has(ATTR_CREATION)) {
      context.v_creation->value = p_tim();
   }

   if (attribute->has(ATTR_EMPTY)) {
      context.v_empty->value = false;
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      context.v_encrypted->value = false;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      context.v_extension->value.clear();
   }

   if (attribute->has(ATTR_HIDDEN)) {
      context.v_hidden->value = false;
   }

   if (attribute->has(ATTR_LIFETIME)) {
      context.v_lifetime->value = p_per();
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      context.v_modification->value = p_tim();
   }

   if (attribute->has(ATTR_NAME)) {
      context.v_name->value.clear();
   }

   if (attribute->has(ATTR_PARENT)) {
      context.v_parent->value.clear();
   }

   if (attribute->has(ATTR_READONLY)) {
      context.v_readonly->value = false;
   }

   if (attribute->has(ATTR_SIZE) || attribute->has(ATTR_SIZE_FILE_ONLY)) {
      context.v_size->value = P_NaN;
   }
}

p_str os_extension(const p_str& value)
{
   for (p_int i = value.size() - 1; i >= 0; i--) {
      const p_char ch = value[i];

      if (ch == CHAR_DOT) {
         if (i == 0 || value[i - 1] == OS_SEPARATOR) {
            return p_str();
         }

         p_str result = value.substr(i + 1);
         str_toLower(result);
         return result;
      }
      else if (ch == OS_SEPARATOR) {
         return p_str();
      }
   }

   return p_str();
}

p_str os_fullname(const p_str& value)
{
   for (p_int i = value.size() - 1; i >= 0; i--) {
      if (value[i] == OS_SEPARATOR) {
         return value.substr(i + 1);
      }
   }

   return value;
}

p_str os_name(const p_str& value)
{
   p_int i = value.size() - 1;
   p_int dot = -1;
   p_bool sep = false;

   for (; i >= 0; i--) {
      const p_char ch = value[i];

      if (dot == -1 && ch == CHAR_DOT) {
         dot = i;
      }
      else if (ch == OS_SEPARATOR) {
         sep = true;
         break;
      }
   }

   if (sep) {
      return dot == -1
         ? value.substr(i + 1)
         : value.substr(i + 1, dot - i - 1);
   }

   return dot == -1
      ? value
      : value.substr(0, dot);
}

p_str os_parent(const p_str& path)
{
   for (p_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == OS_SEPARATOR) {
         return path.substr(0, i);
      }
   }

   return p_str();
}

p_bool os_bothAreSeparators(const p_char left, const p_char right)
{
   return (left == right)
      && (left == OS_SEPARATOR);
}

p_str os_softTrim(const p_str& value)
{
   const p_int len = value.size();
   p_int start = 0;

   while (start < len && value[start] == CHAR_SPACE) {
      start++;
   }

   if (start == len) {
      return p_str();
   }

   p_int end = len - 1;

   while (end >= 0 && value[end] == CHAR_SPACE) {
      end--;
   }

   if (start == 0) {
      return end == len - 1
         ? value
         : value.substr(0, end + 1);
   }
   else {
      return end == len - 1
         ? value.substr(start)
         : value.substr(start, end - start + 1);
   }
}

void os_escapeQuote(p_str& path)
{
   const p_size length = path.size();
   if (path[0] == CHAR_QUOTATION_MARK && path[length - 1] == CHAR_QUOTATION_MARK) {
      path = path.substr(1, length - 2);
   }
}




}
