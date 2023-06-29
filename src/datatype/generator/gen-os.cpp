/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
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

#include "gen-os.h"
#include "gen-string.h"
#include "../../perun2.h"
#include "../../metadata.h"
#include "../../os.h"

namespace perun2::gen
{

#define P_OS_GEN_VALUE_ALTERATION if (this->isAbsolute) { \
      this->value = this->hasPrefix \
         ? str(this->prefix, this->baseLocation, OS_SEPARATOR, this->value) \
         : str(this->baseLocation, OS_SEPARATOR, this->value); \
      } \
   else if (this->hasPrefix) { \
      this->value = str(this->prefix, this->value); \
   } \


namespace os
{
   _str getDefaultPattern()
   {
      return str(OS_SEPARATOR, CHAR_ASTERISK);
   }
}


OsDefinition::OsDefinition(P_GEN_OS_ARGS)
   : location(std::move(loc)), perun2(p2), context(p2),
     flags(p2.flags), isAbsolute(abs),
     hasPrefix(!pref.empty()), prefix(pref) { };


FileContext* OsDefinition::getFileContext()
{
   return &this->context;
}

void OsDefinitionPlain::reset()
{
   if (!first) {
      first = true;
      os_closeEntry(handle);
   }
}

_bool OsDefinitionPlain::isExceptional(const _str& patt)
{
   const _size len = patt.size();

   for (_size i = 1; i < len; i++) {
      switch (patt[i]) {
         case CHAR_DOT:
         case CHAR_ASTERISK: {
            break;
         }
         default: {
            return false;
         }
      }
   }

   return true;
}

void OsDefinitionRecursive::reset()
{
   if (!first) {
      first = true;
      paths.clear();
      bases.clear();
      const _size len = handles.size();
      if (len != 0) {
         for (_size i = 0; i < len; i++) {
            os_closeEntry(handles[i]);
         }
         handles.clear();
      }
   }
}

_bool All::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      if (os_directoryExists(this->baseLocation)) {
         const _str path = str(this->baseLocation, pattern);

         if (!os_hasFirstFile(path, handle, data)) {
            return false;
         }

         first = false;
         value = data.cFileName;
         index.setToZero();
         this->context.index->value = index;

         if (!os_isBrowsePath(value)) {
            const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            if (((this->flags & FLAG_NOOMIT) || (isDir && os_isExplorableDirectory(value))
               || (!isDir && os_extension(value) != metadata::EXTENSION))
               && (!this->exceptional || this->comparer.matches(this->value)))
            {
               this->context.index->value = index;
               index++;

               P_OS_GEN_VALUE_ALTERATION;

               if (retreats > 0) {
                  this->value = str(os_retreats(retreats), this->value);
               }

               this->context.loadData(value, data);
               return true;
            }
         }
      }
      else {
         return false;
      }
   }

   while (os_hasNextFile(handle, data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         if (((this->flags & FLAG_NOOMIT) || (isDir && os_isExplorableDirectory(value))
            || (!isDir && os_extension(value) != metadata::EXTENSION))
            && (!this->exceptional || this->comparer.matches(this->value)))
         {
            this->context.index->value = index;
            index++;

            P_OS_GEN_VALUE_ALTERATION;

            if (retreats > 0) {
               this->value = str(os_retreats(retreats), this->value);
            }

            this->context.loadData(value, data);
            return true;
         }
      }
   }

   first = true;
   os_closeEntry(handle);
   return false;
}

_bool Files::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      if (os_directoryExists(this->baseLocation)) {
         const _str path = str(this->baseLocation, pattern);

         if (!os_hasFirstFile(path, handle, data)) {
            return false;
         }

         first = false;
         value = data.cFileName;
         index.setToZero();
         this->context.index->value = index;

         if (!os_isBrowsePath(value)) {
            const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            if ((!isDir && ((this->flags & FLAG_NOOMIT) || os_extension(value) != metadata::EXTENSION))
               && (!this->exceptional || this->comparer.matches(this->value)))
            {
               this->context.index->value = index;
               index++;

               P_OS_GEN_VALUE_ALTERATION;

               this->context.loadData(value, data);
               return true;
            }
         }
      }
      else {
         return false;
      }
   }

   while (os_hasNextFile(handle, data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         if ((!isDir && ((this->flags & FLAG_NOOMIT) || os_extension(value) != metadata::EXTENSION))
            && (!this->exceptional || this->comparer.matches(this->value)))
         {
            this->context.index->value = index;
            index++;

            P_OS_GEN_VALUE_ALTERATION;

            this->context.loadData(value, data);
            return true;
         }
      }
   }

   first = true;
   os_closeEntry(handle);
   return false;
}

_bool Directories::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());

      if (os_directoryExists(this->baseLocation)) {
         const _str path = str(this->baseLocation, pattern);

         if (!os_hasFirstFile(path, handle, data)) {
            return false;
         }

         first = false;
         value = data.cFileName;
         index.setToZero();
         this->context.index->value = index;

         if (!os_isBrowsePath(value)) {
            const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            if ((isDir && ((this->flags & FLAG_NOOMIT) || os_isExplorableDirectory(value)))
               && (!this->exceptional || this->comparer.matches(this->value)))
            {
               this->context.index->value = index;
               index++;

               P_OS_GEN_VALUE_ALTERATION;

               this->context.loadData(value, data);
               return true;
            }
         }
      }
      else {
         return false;
      }
   }

   while (os_hasNextFile(handle, data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         if ((isDir && ((this->flags & FLAG_NOOMIT) || os_isExplorableDirectory(value)))
            && (!this->exceptional || this->comparer.matches(this->value)))
         {
            this->context.index->value = index;
            index++;

            P_OS_GEN_VALUE_ALTERATION;

            this->context.loadData(value, data);
            return true;
         }
      }
   }

   first = true;
   os_closeEntry(handle);
   return false;
}


_bool RecursiveFiles::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      goDeeper = true;
      first = false;
      index.setToZero();
      this->context.index->value = index;
   }

   while (this->perun2.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str path = str(paths.back(), gen::os::getDefaultPattern());
            handles.emplace_back();
            
            if (!os_hasFirstFile(path, handles.back(), data)) {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
               }
            }
            else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
               const _str& v = data.cFileName;

               if ((this->flags & FLAG_NOOMIT) || os_extension(v) != metadata::EXTENSION) {
                  value = v;
                  this->context.index->value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->context.loadData(value, data);
                  return true;
               }
            }
         }
         else {
            paths.pop_back();
            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
            }
         }
      }
      else {
         if (os_hasNextFile(handles.back(), data)) {
            const _str v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  if ((this->flags & FLAG_NOOMIT) || os_isExplorableDirectory(v)) {
                     paths.emplace_back(str(paths.back(), OS_SEPARATOR, v));

                     if (this->bases.empty()) {
                        bases.emplace_back(str(v, OS_SEPARATOR));
                     }
                     else {
                        bases.emplace_back(str(bases.back(), v, OS_SEPARATOR));
                     }

                     goDeeper = true;
                  }
               }
               else if ((this->flags & FLAG_NOOMIT) || os_extension(v) != metadata::EXTENSION) {
                  value = this->bases.empty() ? v : str(bases.back(), v);
                  this->context.index->value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->context.loadData(value, data);
                  return true;
               }
            }
         }
         else {
            os_closeEntry(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
            }
         }
      }
   }

   reset();
   return false;
}

_bool RecursiveDirectories::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      goDeeper = true;
      first = false;
      index.setToZero();
      this->context.index->value = index;
   }

   while (this->perun2.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str path = str(paths.back(), gen::os::getDefaultPattern());
            handles.emplace_back();
            
            if (!os_hasFirstFile(path, handles.back(), data)) {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
               }
            }
         }
         else {
            paths.pop_back();
            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
            }
         }
      }
      else {
         if (os_hasNextFile(handles.back(), data)) {
            const _str v = data.cFileName;

            if (!os_isBrowsePath(v) && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                && ((this->flags & FLAG_NOOMIT) || os_isExplorableDirectory(v)))
            {
               const _bool isBase = this->bases.empty();
               value = isBase ? v : str(bases.back(), v);
               paths.emplace_back(str(paths.back(), OS_SEPARATOR, v));

               if (isBase) {
                  bases.emplace_back(str(v, OS_SEPARATOR));
               }
               else {
                  bases.emplace_back(str(bases.back(), v, OS_SEPARATOR));
               }

               goDeeper = true;
               this->context.index->value = index;
               index++;

               P_OS_GEN_VALUE_ALTERATION;

               this->context.loadData(value, data);
               return true;
            }
         }
         else {
            os_closeEntry(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
            }
         }
      }
   }

   reset();
   return false;
}

_bool RecursiveAll::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      goDeeper = true;
      first = false;
      index.setToZero();
      this->context.index->value = index;
   }

   while (this->perun2.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str path = str(paths.back(), gen::os::getDefaultPattern());
            handles.emplace_back();
            
            if (!os_hasFirstFile(path, handles.back(), data)) {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
               }
            }
         }
         else {
            paths.pop_back();
            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
            }
         }
      }
      else {
         if (os_hasNextFile(handles.back(), data)) {
            const _str v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                  && ((this->flags & FLAG_NOOMIT) || os_isExplorableDirectory(v)))
               {
                  if (this->prevFile) {
                     this->prevFile = false;
                  }

                  const _bool isBase = this->bases.empty();

                  value = isBase ? v : str(bases.back(), v);
                  paths.emplace_back(str(paths.back(), OS_SEPARATOR, v));

                  if (isBase) {
                     bases.emplace_back(str(v, OS_SEPARATOR));
                  }
                  else {
                     bases.emplace_back(str(bases.back(), v, OS_SEPARATOR));
                  }

                  goDeeper = true;
                  this->context.index->value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->context.loadData(value, data);
                  return true;
               }
               else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                  && ((this->flags & FLAG_NOOMIT) || os_extension(v) != metadata::EXTENSION))
               {
                  if (!this->prevFile) {
                     this->prevFile = true;
                  }

                  const _bool isBase = this->bases.empty();
                  value = isBase ? v : str(bases.back(), v);
                  this->context.index->value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->context.loadData(value, data);
                  return true;
               }
            }
         }
         else {
            os_closeEntry(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
            }
         }
      }
   }

   reset();
   return false;
}

}
