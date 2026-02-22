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

#include "../../include/perun2/os/os-windows.hpp"
#include "../../include/perun2/perun2.hpp"
#include "../../include/perun2/datatype/parse/parse-asterisk.hpp"
#include "../../include/perun2/datatype/text/strings.hpp"
#include "../../include/perun2/metadata.hpp"
#include <time.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <algorithm>
#include <fstream>
#include <combaseapi.h>
#include <fcntl.h>
#include <setupapi.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <cstdlib>
#include <array>
#include <optional>


namespace perun2
{

void os_init()
{
   _setmode(_fileno(stdout), _O_U8TEXT);
   CoInitializeEx(0, COINIT_MULTITHREADED);
}

void os_deinit()
{
   // nothing as expected
}

p_tim os_now()
{
   time_t raw;
   time(&raw);
   const struct tm* info = localtime(&raw);

   return p_tim(info->tm_mday, info->tm_mon + 1, 1900 + info->tm_year,
      info->tm_hour, info->tm_min, info->tm_sec);
}

p_tim os_today()
{
   time_t raw;
   time(&raw);
   const struct tm* info = localtime(&raw);

   return p_tim(info->tm_mday, info->tm_mon + 1, 1900 + info->tm_year);
}

void os_rawSleepForMs(const p_nint ms)
{
   Sleep(ms);
}

// explanation of attributes is in file 'attribute.h'
void os_loadAttributes(FileContext& context)
{
   const p_attrptr& attribute = context.attribute;
   context.trimmed = os_trim(context.this_->value);
   context.invalid = os_isInvalid(context.trimmed);

   if (context.invalid) {
      os_loadEmptyAttributes(context);
      return;
   }

   // we do not need access to the file system to get these values below

   if (attribute->has(ATTR_PATH)) {
      context.v_path->value = os_leftJoin(context.locContext->location->value, context.trimmed);

      p_bool noPath = false;

      if (context.v_path->value.empty()) {
         noPath = true;
      }
      else if (!os_isAbsolute(context.v_path->value)) {
         noPath = true;
         context.v_path->value.clear();
      }

      if (noPath) {
         os_loadEmptyAttributes(context);

         if (attribute->has(ATTR_DEPTH)) {
            context.v_depth->value = os_depth(context.trimmed);
         }

         const p_str namePart = os_segmentWithName(context.trimmed);

         if (namePart.empty()) {
            return;
         }

         if (attribute->has(ATTR_EXTENSION)) {
            context.v_extension->value = os_extension(namePart);
         }

         if (attribute->has(ATTR_NAME)) {
            context.v_name->value = os_hasExtension(namePart)
               ? os_name(namePart)
               : os_fullname(namePart);
         }

         if (attribute->has(ATTR_FULLNAME)) {
            context.v_fullname->value = os_fullname(namePart);
         }

         return;
      }
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value = os_fullname(context.trimmed);
   }

   if (attribute->has(ATTR_PARENT)) {
      context.v_parent->value = os_parent(context.v_path->value);
   }

   if (attribute->has(ATTR_DRIVE)) {
      context.v_drive->value = os_drive(context.v_path->value);
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = os_depth(context.trimmed);
   }

   if (!attribute->has(ATTR_EXISTS) || context.v_path->value.empty()) {
      return;
   }

   // below are "real" attributes of files and directories
   p_adata data;
   const p_bool gotAttrs = GetFileAttributesExW(P_WINDOWS_PATH(context.v_path->value), GetFileExInfoStandard, &data);
   const DWORD dwAttrib = data.dwFileAttributes;
   context.v_exists->value = gotAttrs && dwAttrib != INVALID_FILE_ATTRIBUTES;

   if (context.v_exists->value) {
      context.v_isdirectory->value = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
      context.v_isfile->value = !context.v_isdirectory->value;
   }
   else {
      context.v_isfile->value = os_hasExtension(context.trimmed);
      context.v_isdirectory->value = !context.v_isfile->value;
   }

   if (attribute->has(ATTR_ACCESS)) {
      context.v_access->value = context.v_exists->value
         ? os_convertToPerun2Time(&data.ftLastAccessTime)
         : p_tim();
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      context.v_archive->value = context.v_exists->value ? (dwAttrib & FILE_ATTRIBUTE_ARCHIVE) : false;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      context.v_compressed->value = context.v_exists->value ? (dwAttrib & FILE_ATTRIBUTE_COMPRESSED) : false;
   }

   if (attribute->has(ATTR_CREATION)) {
      context.v_creation->value = context.v_exists->value
         ? os_convertToPerun2Time(&data.ftCreationTime)
         : p_tim();
   }

   if (attribute->has(ATTR_MODIFICATION) || attribute->has(ATTR_CHANGE)) {
      const p_tim time = context.v_exists->value
         ? os_convertToPerun2Time(&data.ftLastWriteTime)
         : p_tim();

      if (attribute->has(ATTR_CHANGE)) {
         context.v_change->value = time;
      }
      if (attribute->has(ATTR_MODIFICATION)) {
         context.v_modification->value = time;
      }
   }

   if (attribute->has(ATTR_LIFETIME)) {
      if (context.v_exists->value) {
         context.v_lifetime->value = context.v_creation->value < context.v_modification->value
            ? (os_now() - context.v_creation->value)
            : (os_now() - context.v_modification->value);
      }
      else {
         context.v_lifetime->value = p_per();
      }
   }

   if (attribute->has(ATTR_EMPTY)) {
      if (context.v_exists->value) {
         context.v_empty->value = context.v_isfile->value
            ? os_emptyFile(data)
            : os_emptyDirectory(context.v_path->value);
      }
      else {
         context.v_empty->value = false;
      }
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      context.v_encrypted->value = context.v_exists->value ? (dwAttrib & FILE_ATTRIBUTE_ENCRYPTED) : false;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      context.v_extension->value = context.v_isfile->value ? os_extension(context.trimmed) : p_str();
   }

   if (attribute->has(ATTR_HIDDEN)) {
      context.v_hidden->value = context.v_exists->value ? (dwAttrib & FILE_ATTRIBUTE_HIDDEN) : false;
   }

   if (attribute->has(ATTR_NAME)) {
      if (context.v_isdirectory->value) {
         context.v_name->value = os_fullname(context.v_path->value);
      }
      else {
         context.v_name->value = os_hasExtension(context.v_path->value)
            ? os_name(context.v_path->value)
            : os_fullname(context.v_path->value);
      }
   }

   if (attribute->has(ATTR_READONLY)) {
      context.v_readonly->value = context.v_exists->value ? (dwAttrib & FILE_ATTRIBUTE_READONLY) : false;
   }

   if (attribute->has(ATTR_SIZE)) {
      if (context.v_exists->value) {
         context.v_size->value = context.v_isfile->value
            ? static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh))
            : os_sizeDirectory(context.v_path->value, context.perun2);
      }
      else {
         context.v_size->value = P_NaN;
      }
   }
   else if (attribute->has(ATTR_SIZE_FILE_ONLY)) {
      if (context.v_exists->value) {
         if (context.v_isfile->value) {
            context.v_size->value = p_num(static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh)));
         }
      }
      else {
         context.v_size->value = P_NaN;
      }
   }
   
   if (attribute->has(ATTR_IMAGE_OR_VIDEO)) {
      const MediaAttributes media = os_ffmpegAttributes(context.v_path->value);
      context.v_isimage->value = media.isImage;
      context.v_isvideo->value = media.isVideo;
      context.v_width->value = media.width;
      context.v_height->value = media.height;
      context.v_duration->value = media.duration;
   }
}

// load attributes, but we already have some data
// we do not need to read it again from the file system
void os_loadDataAttributes(FileContext& context, const p_fdata& data)
{
   const p_attrptr& attribute = context.attribute;
   context.trimmed = os_trim(context.this_->value);
   context.invalid = false;

   // we do not need access to the file system to get these values
   if (attribute->has(ATTR_PATH)) {
      if (!context.trimmed.empty() && context.trimmed[0] == CHAR_DOT) {
         context.v_path->value = os_join(context.locContext->location->value, context.trimmed);
      }
      else {
         context.v_path->value = os_softJoin(context.locContext->location->value, context.trimmed);
      }
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value = os_fullname(context.trimmed);
   }

   if (attribute->has(ATTR_PARENT)) {
      context.v_parent->value = os_parent(context.v_path->value);
   }

   if (attribute->has(ATTR_DRIVE)) {
      context.v_drive->value = os_drive(context.v_path->value);
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = os_depth(context.trimmed);
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   // below are "real" attributes of files and directories
   const DWORD dwAttrib = data.dwFileAttributes;
   context.v_exists->value = true;
   context.v_isdirectory->value = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
   context.v_isfile->value = !context.v_isdirectory->value;

   if (attribute->has(ATTR_ACCESS)) {
      context.v_access->value = os_convertToPerun2Time(&data.ftLastAccessTime);
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      context.v_archive->value = dwAttrib & FILE_ATTRIBUTE_ARCHIVE;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      context.v_compressed->value = dwAttrib & FILE_ATTRIBUTE_COMPRESSED;
   }

   if (attribute->has(ATTR_CREATION)) {
      context.v_creation->value = os_convertToPerun2Time(&data.ftCreationTime);
   }

   const p_bool hasMod = attribute->has(ATTR_MODIFICATION);
   const p_bool hasChange = attribute->has(ATTR_CHANGE);
   if (hasMod || hasChange) {
      const p_tim time = os_convertToPerun2Time(&data.ftLastWriteTime);

      if (hasChange) {
         context.v_change->value = time;
      }
      if (hasMod) {
         context.v_modification->value = time;
      }
   }

   if (attribute->has(ATTR_LIFETIME)) {
      context.v_lifetime->value = context.v_creation->value < context.v_modification->value
         ? (os_now() - context.v_creation->value)
         : (os_now() - context.v_modification->value);
   }

   if (attribute->has(ATTR_EMPTY)) {
      context.v_empty->value = context.v_isfile->value
         ? (data.nFileSizeLow == 0 && data.nFileSizeHigh == 0)
         : os_emptyDirectory(context.v_path->value);
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      context.v_encrypted->value = (dwAttrib & FILE_ATTRIBUTE_ENCRYPTED);
   }

   if (attribute->has(ATTR_EXTENSION)) {
      context.v_extension->value = context.v_isfile->value ? os_extension(context.trimmed) : p_str();
   }

   if (attribute->has(ATTR_HIDDEN)) {
      context.v_hidden->value = (dwAttrib & FILE_ATTRIBUTE_HIDDEN);
   }

   if (attribute->has(ATTR_NAME)) {
      if (context.v_isdirectory->value) {
         context.v_name->value = os_fullname(context.trimmed);
      }
      else {
         context.v_name->value = os_hasExtension(context.trimmed)
            ? os_name(context.trimmed)
            : os_fullname(context.trimmed);
      }
   }

   if (attribute->has(ATTR_READONLY)) {
      context.v_readonly->value = (dwAttrib & FILE_ATTRIBUTE_READONLY);
   }

   if (attribute->has(ATTR_SIZE)) {
      context.v_size->value = context.v_isfile->value
         ? static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh))
         : os_sizeDirectory(context.v_path->value, context.perun2);
   }
   else if (attribute->has(ATTR_SIZE_FILE_ONLY)) {
      if (context.v_isfile->value) {
         context.v_size->value = p_num(static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh)));
      }
   }

   if (attribute->has(ATTR_IMAGE_OR_VIDEO)) {
      const MediaAttributes media = os_ffmpegAttributes(context.v_path->value);
      context.v_isimage->value = media.isImage;
      context.v_isvideo->value = media.isVideo;
      context.v_width->value = media.width;
      context.v_height->value = media.height;
      context.v_duration->value = media.duration;
   }
}

p_tim os_access(const p_str& path)
{
   p_adata data;

   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return p_tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return p_tim();
   }

   return os_convertToPerun2Time(&data.ftLastAccessTime);
}

p_bool os_archive(const p_str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_ARCHIVE);
}

p_tim os_change(const p_str& path)
{
   p_adata data;

   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return p_tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return p_tim();
   }

   return os_convertToPerun2Time(&data.ftLastWriteTime);
}

p_bool os_compressed(const p_str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_COMPRESSED);
}

p_tim os_creation(const p_str& path)
{
   p_adata data;

   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return p_tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return p_tim();
   }

   return os_convertToPerun2Time(&data.ftCreationTime);
}

p_num os_depth(const p_str& value)
{
   if (value.empty()) {
      return NINT_ZERO;
   }

   p_nint depth = NINT_ZERO;
   p_int prevId = 0;
   
   for (p_size i = 0; i < value.size(); i++) {
      if (value[i] == OS_SEPARATOR) {
         const p_size len = i - prevId;

         if (len == 1 && value[prevId] == CHAR_DOT) { }
         else if (len == 2 && value[prevId] == CHAR_DOT && value[prevId + 1] == CHAR_DOT) {
            depth--;
         }
         else {
            depth++;
         }

         prevId = i + 1;
      }
   }

   const p_size len = value.size() - prevId;

   if (len == 2 && value[prevId] == CHAR_DOT && value[prevId + 1] == CHAR_DOT) {
      depth--;
   }
   else if (!(len == 1 && value[prevId] == CHAR_DOT)) {
      depth++;
   }

   depth--;
   return depth;
}

p_str os_drive(const p_str& path)
{
   if (os_isAbsolute(path)) {
      p_char letter = path[0];
      char_toUpper(letter);
      return str(letter, CHAR_COLON);
   }
   else {
      return p_str();
   }
}

p_bool os_empty(const p_str& path)
{
   p_adata data;
   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return false;
   }

   const DWORD& dwAttrib = data.dwFileAttributes;
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY
      ? os_emptyDirectory(path)
      : os_emptyFile(data);
}

p_bool os_encrypted(const p_str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_ENCRYPTED);
}

p_bool os_emptyFile(const p_adata& data)
{
   return data.nFileSizeLow == 0
       && data.nFileSizeHigh == 0;
}

p_bool os_emptyDirectory(const p_str& path)
{
   p_fdata data;
   p_entry handle;
   const p_str pattern = str(path, OS_SEPARATOR, CHAR_ASTERISK);

   if (!os_hasFirstFile(pattern, handle, data)) {
      return true;
   }
 
   while (os_hasNextFile(handle, data)) {
      if (!os_isBrowsePath(data.cFileName)) {
         os_closeEntry(handle);
         return false;
      }
   }

   os_closeEntry(handle);
   return true;
}

p_bool os_hasAttribute(const p_str& path, const DWORD attribute)
{
   DWORD dwAttrib = GetFileAttributesW(P_WINDOWS_PATH(path));

   return (dwAttrib != INVALID_FILE_ATTRIBUTES) &&
          (dwAttrib & attribute);
}

p_bool os_hidden(const p_str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_HIDDEN);
}

p_bool os_isFile(const p_str& path)
{
   DWORD dwAttrib = GetFileAttributesW(P_WINDOWS_PATH(path));
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return os_hasExtension(path);
   }

   return !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

p_bool os_isDirectory(const p_str& path)
{
   DWORD dwAttrib = GetFileAttributesW(P_WINDOWS_PATH(path));
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return !os_hasExtension(path);
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
}

p_bool os_isDirectory(const p_fdata& data)
{
   return data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

p_per os_lifetime(const p_str& path)
{
   p_adata data;

   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return p_per();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return p_per();
   }

   const p_tim modification = os_convertToPerun2Time(&data.ftLastWriteTime);
   const p_tim creation = os_convertToPerun2Time(&data.ftCreationTime);

   return creation < modification
      ? (os_now() - creation)
      : (os_now() - modification);
}

p_tim os_modification(const p_str& path)
{
   p_adata data;

   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return p_tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return p_tim();
   }

   return os_convertToPerun2Time(&data.ftLastWriteTime);
}

p_bool os_readonly(const p_str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_READONLY);
}

p_num os_size(const p_str& path, Perun2Process& p2)
{
   p_adata data;
   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return P_NaN;
   }

   const DWORD& dwAttrib = data.dwFileAttributes;
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return P_NaN;
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY
      ? os_sizeDirectory(path, p2)
      : static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh));
}

p_num os_sizeFile(const p_str& path)
{
   p_adata data;
   if (!GetFileAttributesExW(P_WINDOWS_PATH(path), GetFileExInfoStandard, &data)) {
      return P_NaN;
   }

   const DWORD& dwAttrib = data.dwFileAttributes;
   if (dwAttrib == INVALID_FILE_ATTRIBUTES || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
      return P_NaN;
   }

   return static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh));
}

p_num os_sizeDirectory(const p_str& path, Perun2Process& p2)
{
   std::vector<p_entry> entries;
   p_list paths = { path };
   p_list bases;
   p_bool goDeeper = true;
   p_fdata data;
   p_nint totalSize = NINT_ZERO;

   while (true) {
      if (p2.isNotRunning()) {
         for (p_entry& entry : entries) {
            os_closeEntry(entry);
         }

         return P_NaN;
      }

      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const p_str newPath = str(paths.back(), gen::os::DEFAULT_PATTERN);
            entries.emplace_back();
            
            if (!os_hasFirstFile(newPath, entries.back(), data)) {
               entries.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
               }
            }
            else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
               totalSize += static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh));
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
         if (os_hasNextFile(entries.back(), data)) {
            const p_str v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  paths.emplace_back(str(paths.back(), OS_SEPARATOR, v));

                  if (bases.empty()) {
                     bases.emplace_back(str(v, OS_SEPARATOR));
                  }
                  else {
                     bases.emplace_back(str(bases.back(), v, OS_SEPARATOR));
                  }

                  goDeeper = true;
               }
               else {
                  totalSize += static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh));
               }
            }
         }
         else {
            os_closeEntry(entries.back());
            entries.pop_back();
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

   return totalSize;
}

p_bool os_sizeDirectorySatisfies(const p_str& path, IncrementalConstraint& constr, Perun2Process& p2)
{
   constr.loadLimit();
   if (constr.limitIsNaN()) {
      return false;
   }

   constr.setValueToZero();
   
   Logic state = constr.getState();
   if (state != Logic::Unknown) {
      return state == Logic::True;
   }

   std::vector<p_entry> entries;
   p_list paths = { path };
   p_list bases;
   p_bool goDeeper = true;
   p_fdata data;

   while (true) {
      if (p2.isNotRunning()) {
         for (p_entry& entry : entries) {
            os_closeEntry(entry);
         }

         return false;
      }

      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const p_str newPath = str(paths.back(), gen::os::DEFAULT_PATTERN);
            entries.emplace_back();
            
            if (!os_hasFirstFile(newPath, entries.back(), data)) {
               entries.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
               }
            }
            else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
               constr.increment(static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh)));
               state = constr.getState();
               if (state != Logic::Unknown) {
                  for (p_entry& entry : entries) {
                     os_closeEntry(entry);
                  }

                  return state == Logic::True;
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
         if (os_hasNextFile(entries.back(), data)) {
            const p_str v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  paths.emplace_back(str(paths.back(), OS_SEPARATOR, v));

                  if (bases.empty()) {
                     bases.emplace_back(str(v, OS_SEPARATOR));
                  }
                  else {
                     bases.emplace_back(str(bases.back(), v, OS_SEPARATOR));
                  }

                  goDeeper = true;
               }
               else {
                  constr.increment(static_cast<p_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh)));
                  state = constr.getState();
                  if (state != Logic::Unknown) {
                     for (p_entry& entry : entries) {
                        os_closeEntry(entry);
                     }

                     return state == Logic::True;
                  }
               }
            }
         }
         else {
            os_closeEntry(entries.back());
            entries.pop_back();
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

   return constr.getFinalResult();
}

p_bool os_exists(const p_str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   return GetFileAttributesW(P_WINDOWS_PATH(path)) != INVALID_FILE_ATTRIBUTES;
}

p_bool os_fileExists(const p_str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(P_WINDOWS_PATH(path));
   return (dwAttrib != INVALID_FILE_ATTRIBUTES && (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

p_bool os_directoryExists(const p_str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(P_WINDOWS_PATH(path));
   return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


p_bool os_hasFirstFile(const p_str& path, p_entry& entry, p_fdata& output)
{
   entry = FindFirstFileEx(P_WINDOWS_PATH(path), FindExInfoBasic, &output, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH); 
   return entry != INVALID_HANDLE_VALUE;
}

p_bool os_hasNextFile(p_entry& entry, p_fdata& output)
{
   return FindNextFile(entry, &output);
}

void os_closeEntry(p_entry& entry)
{
   CloseHandle(entry);
}

//////
///
// filesystem commands:
///
/////

p_bool os_delete(const p_str& path)
{
   p_char wszFrom[MAX_PATH] = { 0 };
   wcscpy(wszFrom, path.c_str());
   CopyMemory(wszFrom + lstrlenW(wszFrom), "\0\0", 2);

   SHFILEOPSTRUCTW sfo = {0};
   sfo.wFunc = FO_DELETE;
   sfo.pFrom = wszFrom;
   sfo.fFlags = FOF_ALLOWUNDO |
     FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR |
     FOF_WANTNUKEWARNING;

   return SHFileOperationW(&sfo) == 0 && !sfo.fAnyOperationsAborted;
}

p_bool os_drop(const p_str& path, Perun2Process& p2)
{
   return os_isFile(path)
      ? os_dropFile(path)
      : os_dropDirectory(path, p2);
}

p_bool os_drop(const p_str& path, const p_bool isFile, Perun2Process& p2)
{
   return isFile
      ? os_dropFile(path)
      : os_dropDirectory(path, p2);
}

p_bool os_dropFile(const p_str& path)
{
   return DeleteFileW(P_WINDOWS_PATH(path)) != 0;
}

p_bool os_dropDirectory(const p_str& path, Perun2Process& p2)
{
   p_entry hFind;
   p_fdata FindFileData;

   p_char DirPath[MAX_PATH];
   p_char FileName[MAX_PATH];

   wcscpy(DirPath, const_cast<p_char*>(path.c_str()));
   wcscat(DirPath, str(OS_SEPARATOR, CHAR_ASTERISK).c_str());
   wcscpy(FileName, const_cast<p_char*>(path.c_str()));
   wcscat(FileName, toStr(OS_SEPARATOR).c_str());

   if (!os_hasFirstFile(DirPath, hFind, FindFileData)) {
      return false;
   }

   wcscpy(DirPath, FileName);

   p_bool bSearch = true;
   while (bSearch) {
      if (os_hasNextFile(hFind, FindFileData)) {
         if (p2.isNotRunning()) {
            os_closeEntry(hFind);
            return false;
         }

         const p_str v = FindFileData.cFileName;

         if (os_isBrowsePath(v)) {
            continue;
         }

         wcscat(FileName,FindFileData.cFileName);
         if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!os_dropDirectory(FileName, p2)) {
               os_closeEntry(hFind);
               return false;
            }

            RemoveDirectoryW(FileName);
            wcscpy(FileName,DirPath);
         }
         else {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
               os_unlock(FileName);

            if (!DeleteFileW(FileName)) {
               os_closeEntry(hFind);
               return false;
            }

            wcscpy(FileName,DirPath);
         }
      }
      else {
         if (GetLastError() == ERROR_NO_MORE_FILES)
            bSearch = false;
         else {
            os_closeEntry(hFind);
            return false;
         }
      }
   }
   os_closeEntry(hFind);

   return RemoveDirectoryW(const_cast<p_char*>(P_WINDOWS_PATH(path))) != 0;
}

p_bool os_hide(const p_str& path)
{
   const DWORD attr = GetFileAttributesW(P_WINDOWS_PATH(path));

   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
      return SetFileAttributes(P_WINDOWS_PATH(path), attr | FILE_ATTRIBUTE_HIDDEN) != 0;
   }

   return true;
}

p_bool os_lock(const p_str& path)
{
   const DWORD attr = GetFileAttributesW(P_WINDOWS_PATH(path));

   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_READONLY) == 0) {
      return SetFileAttributes(P_WINDOWS_PATH(path), attr | FILE_ATTRIBUTE_READONLY) != 0;
   }

   return true;
}

p_bool os_open(const p_str& path)
{
   const p_str location = os_parent(path);
   return (INT_PTR)ShellExecuteW(0, 0, P_WINDOWS_PATH(path), 0, P_WINDOWS_PATH(location) , SW_SHOW) > 32;
}

p_bool os_openAsCommand(const p_str& command, const p_str& location)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));

   std::unique_ptr<p_char[]> cmd = std::make_unique<p_char[]>(command.size() + 1);
   wcscpy(cmd.get(), command.c_str());
   cmd[command.size()] = CHAR_NULL;

   const BOOL creation = CreateProcessW (
      NULL,
      cmd.get(),
      NULL, NULL, FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL, 
      location.c_str(),
      &si, &pi
   );

   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   return creation != 0;
}

p_bool os_unhide(const p_str& path)
{
   const DWORD attr = GetFileAttributesW(P_WINDOWS_PATH(path));

   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) {
      return SetFileAttributes(P_WINDOWS_PATH(path), attr & ~FILE_ATTRIBUTE_HIDDEN) != 0;
   }

   return true;
}

p_bool os_unlock(const p_str& path)
{
   const DWORD attr = GetFileAttributesW(P_WINDOWS_PATH(path));

   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY) {
      return SetFileAttributes(P_WINDOWS_PATH(path), attr & ~FILE_ATTRIBUTE_READONLY) != 0;
   }

   return true;
}


p_bool os_setTime(const p_str& path, const p_tim& creation,
   const p_tim& access, const p_tim& modification)
{
   p_ftim time_c;
   p_ftim time_a;
   p_ftim time_m;

   if (!(os_convertToFileTime(creation, time_c)
    && os_convertToFileTime(access, time_a)
    && os_convertToFileTime(modification, time_m)))
   {
      return false;
   }

   p_entry handle = CreateFile(P_WINDOWS_PATH(path),
      FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE,
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (handle == INVALID_HANDLE_VALUE) {
      return false;
   }

   const p_bool result = SetFileTime(handle, &time_c, &time_a, &time_m);
   CloseHandle(handle);
   return result;
}

p_bool os_createFile(const p_str& path)
{
   if (os_hasParentDirectory(path)) {
      const p_str p = os_parent(path);
      if (!os_exists(p)) {
         const p_bool b = os_createDirectory(p);
         if (!b) {
            return false;
         }
      }
   }
   else {
      return false;
   }
   
   p_entry h = CreateFileW(P_WINDOWS_PATH(path), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

   if (h != INVALID_HANDLE_VALUE) {
      p_ftim ftime;

      if (os_convertToFileTime(os_now(), ftime)) {
         SetFileTime(h, &ftime, &ftime, &ftime);
      }

      CloseHandle(h);
      return true;
   }
   else {
      return false;
   }
}

p_bool os_createDirectory(const p_str& path)
{
   if (os_hasParentDirectory(path)) {
      const p_str p = os_parent(path);
      if (!os_exists(p)) {
         const p_bool b = os_createDirectory(p);
         if (!b) {
            return false;
         }
      }
   }
   else {
      return false;
   }

   return CreateDirectoryW(P_WINDOWS_PATH(path), NULL) != 0;
}

p_bool os_moveTo(const p_str& oldPath, const p_str& newPath)
{
   return MoveFileExW(P_WINDOWS_PATH(oldPath), P_WINDOWS_PATH(newPath), MOVEFILE_COPY_ALLOWED) != 0;
}

p_bool os_copyTo(const p_str& oldPath, const p_str& newPath, const p_bool isFile, Perun2Process& p2)
{
   if (isFile) {
      return os_copyToFile(oldPath, newPath);
   }

   if (os_isAncestor(oldPath, newPath)) {
      return false;
   }

   const p_bool success = os_copyToDirectory(oldPath, newPath, p2);
   if (!success && p2.isNotRunning() && os_directoryExists(newPath)) {
      // if directory copy operation
      // was stopped by the user
      // delete recent partially copied directory if it is there
      os_dropDirectory(newPath, p2);
   }

   return success;
}

p_bool os_copyToFile(const p_str& oldPath, const p_str& newPath)
{
   return CopyFileW(P_WINDOWS_PATH(oldPath), P_WINDOWS_PATH(newPath), true) != 0;
}

p_bool os_copyToDirectory(const p_str& oldPath, const p_str& newPath, Perun2Process& p2)
{
   if (!os_createDirectory(newPath)) {
      return false;
   }

   const p_size length = oldPath.size() + 1;

   p_entry hFind;
   p_fdata FindFileData;

   p_char DirPath[MAX_PATH];
   p_char FileName[MAX_PATH];

   wcscpy(DirPath, const_cast<p_char*>(oldPath.c_str()));
   wcscat(DirPath, str(OS_SEPARATOR, CHAR_ASTERISK).c_str());
   wcscpy(FileName, const_cast<p_char*>(oldPath.c_str()));
   wcscat(FileName, toStr(OS_SEPARATOR).c_str());

   if (!os_hasFirstFile(DirPath, hFind, FindFileData)) {
      return false;
   }

   wcscpy(DirPath, FileName);

   p_bool bSearch = true;
   while (bSearch) {
      if (os_hasNextFile(hFind, FindFileData)) {
         if (p2.isNotRunning()) {
            os_closeEntry(hFind);
            return false;
         }

         const p_str v = FindFileData.cFileName;

         if (os_isBrowsePath(v)) {
            continue;
         }

         wcscat(FileName, FindFileData.cFileName);
         const p_str np = str(newPath, OS_SEPARATOR, p_str(FileName).substr(length));

         if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!os_copyToDirectory(FileName, np, p2)) {
               os_closeEntry(hFind);
               return false;
            }

            wcscpy(FileName, DirPath);
         }
         else {
            if (!os_copyToFile(FileName, np)) {
               os_closeEntry(hFind);
               return false;
            }

            wcscpy(FileName, DirPath);
         }
      }
      else {
         if (GetLastError() == ERROR_NO_MORE_FILES)
            bSearch = false;
         else {
            os_closeEntry(hFind);
            return false;
         }
      }
   }

   os_closeEntry(hFind);
   return true;
}

p_bool os_copy(const p_set& paths)
{
   p_size totalSize = sizeof(DROPFILES) + sizeof(p_char);

   for (const p_str& p : paths) {
      totalSize += sizeof(p_char) * (p.size() + 1);
   }

   HDROP hdrop   = static_cast<HDROP>(GlobalAlloc(GHND, totalSize));
   DROPFILES* df = static_cast<DROPFILES*>(GlobalLock(hdrop));
   df->pFiles    = sizeof(DROPFILES);
   df->fWide     = true;

   p_char* dstStart = (p_char*)(&df[1]);

   for (const p_str& p : paths) {
      wcscpy(dstStart, p.c_str());
      dstStart = &dstStart[p.size() + 1];
   }

   GlobalUnlock(hdrop);
   OpenClipboard(NULL);
   EmptyClipboard();
   SetClipboardData(CF_HDROP, hdrop);
   CloseClipboard();

   return true;
}

p_bool os_select(const p_str& parent, const p_set& paths)
{
   ITEMIDLIST* folder = ILCreateFromPathW(parent.c_str());
   std::vector<ITEMIDLIST*> v;

   for (const p_str& p : paths) {
      v.emplace_back(ILCreateFromPathW(p.c_str()));
   }

   HRESULT hr = SHOpenFolderAndSelectItems(folder, v.size(), (LPCITEMIDLIST*)v.data(), 0);

   for (ITEMIDLIST* idl : v) {
      ILFree(idl);
   }

   ILFree(folder);
   return hr == S_OK;
}

p_bool os_run(const p_str& command, const p_str& location, Perun2Process& p2)
{
   STARTUPINFO si;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&p2.sideProcess.info, sizeof(p2.sideProcess.info));

   std::unique_ptr<p_char[]> cmd = std::make_unique<p_char[]>(command.size() + 1);
   wcscpy(cmd.get(), command.c_str());
   cmd[command.size()] = CHAR_NULL;

   const BOOL creation = CreateProcessW(
      NULL,
      cmd.get(),
      NULL,NULL,FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL,
      location.empty() ? NULL : location.c_str(),
      &si, &p2.sideProcess.info
   );

   if (! creation) {
      return false;
   }

   p2.sideProcess.running = true;

   WaitForSingleObject(p2.sideProcess.info.hProcess, INFINITE);
   DWORD dwExitCode = 0;
   ::GetExitCodeProcess(p2.sideProcess.info.hProcess, &dwExitCode);
   
   CloseHandle(p2.sideProcess.info.hProcess);
   CloseHandle(p2.sideProcess.info.hThread);

   p2.sideProcess.running = false;
   return p2.state == State::s_Running && dwExitCode == 0;
}

p_bool os_terminate(SideProcess& process)
{
   return TerminateProcess(process.info.hProcess, 0) != 0;
}

p_bool os_popup(const p_str& text)
{
   return MessageBoxW(NULL, text.c_str(), STRING_POPUP_TITLE, MB_OK | MB_ICONINFORMATION) == IDOK;
}

p_bool os_isInvalid(const p_str& path)
{
   const p_size length = path.size();

   if (length == 0 || (length >= 2&& path[length - 1] == CHAR_DOT
      && !(path[length - 2] == CHAR_DOT || path[length - 2] == OS_SEPARATOR)))
   {
      return true;
   }

   for (p_size i = 0; i < length; i++) {
      switch (path[i]) {
         case CHAR_SMALLER:
         case CHAR_GREATER:
         case CHAR_VERTICAL_BAR:
         case CHAR_QUESTION_MARK:
         case CHAR_ASTERISK:
         case CHAR_QUOTATION_MARK: {
            return true;
         }
         case CHAR_COLON: {
            if (i != 1 || !os_isAbsolute(path)) {
               return true;
            }
            break;
         }
      }
   }

   return false;
}

p_str os_trim(const p_str& path)
{
   const p_int len = path.size();
   p_int start = 0;
   p_bool anyDot = false;

   while (true) {
      if (len >= (start + 2) && path[start] == CHAR_DOT
         && (path[start + 1] == OS_SEPARATOR || path[start + 1] == OS_WRONG_SEPARATOR))
      {
         anyDot = true;
         start += 2;
      }
      else if (start < len) {
         switch (path[start]) {
            case OS_WRONG_SEPARATOR:
            case OS_SEPARATOR:
            case CHAR_SPACE: {
               start++;
               break;
            }
            default: {
               goto exitStart;
            }
         }
      }
      else {
         break;
      }
   }

exitStart:

   if (start == len) {
      return anyDot ? toStr(CHAR_DOT) : p_str();
   }

   p_int end = len - 1;

   while (true) {
      if (end >= 1 && path[end] == CHAR_DOT
       && (path[end - 1] == OS_SEPARATOR || path[end - 1] == OS_WRONG_SEPARATOR))
      {
         end -= 2;
      }
      else if (end >= 0) {
         switch (path[end]) {
            case OS_WRONG_SEPARATOR:
            case OS_SEPARATOR:
            case CHAR_SPACE: {
               end--;
               break;
            }
            default: {
               goto exitEnd;
            }
         }
      }
      else {
         break;
      }

   }

exitEnd:

   p_str result;

   if (start == 0) {
      result = end == len - 1
         ? path
         : path.substr(0, end + 1);
   }
   else {
      result = end == len - 1
         ? path.substr(start)
         : path.substr(start, end - start + 1);
   }

   std::replace(result.begin(), result.end(), OS_WRONG_SEPARATOR, OS_SEPARATOR);
   result.erase(std::unique(result.begin(), result.end(), os_bothAreSeparators), result.end());
   os_escapeQuote(result);

   return result;
}

void os_rightTrim(p_str& path, const p_bool separatorEnding)
{
   if (path.empty()) {
      return;
   }

   p_int end = path.size() - 1;
   p_int retreats = 0;

   while (true) {
      if (end >= 2 && path[end] == CHAR_DOT && path[end - 1] == CHAR_DOT
       && (path[end - 2] == OS_SEPARATOR || path[end - 2] == OS_WRONG_SEPARATOR))
      {
         end -= 3;
         retreats++;

         while (end >= 0 && (path[end] == OS_SEPARATOR || path[end] == OS_WRONG_SEPARATOR)) {
            end--;
         }
      }
      else if (end >= 1 && path[end] == CHAR_DOT
       && (path[end - 1] == OS_SEPARATOR || path[end - 1] == OS_WRONG_SEPARATOR))
      {
         end -= 2;

         while (end >= 0 && (path[end] == OS_SEPARATOR || path[end] == OS_WRONG_SEPARATOR)) {
            end--;
         }
      }
      else if (end >= 0) {
         switch (path[end]) {
            case OS_WRONG_SEPARATOR:
            case OS_SEPARATOR:{
               end--;
               if (retreats > 0) {
                  retreats--;

                  while (end >= 0 && (path[end] == OS_SEPARATOR || path[end] == OS_WRONG_SEPARATOR)) {
                     end--;
                  }
               }

               break;
            }
            case CHAR_SPACE: {
               end--;
               break;
            }
            default: {
               if (retreats == 0) {
                  goto rtExitEnd;
               }
               end--;
               break;
            }
         }
      }
      else {
         if (retreats == 0) {
            break;
         }
      }

      if (end == 0) {
         switch (path.size()) {
            case 0:
            case 1: {
               break;
            }
            case 2: {
               if (path[0] == CHAR_DOT && path[1] == CHAR_DOT) {
                  retreats++;
               }
               break;
            }
            default: {
               if (path[0] == CHAR_DOT && path[1] == CHAR_DOT
                  && (path[2] == OS_SEPARATOR || path[2] == OS_WRONG_SEPARATOR))
               {
                  retreats++;
               }
               break;
            }
         }

         if (retreats == 0) {
            if (!separatorEnding) {
               path.resize(1);
            }

            return;
         }

         path = separatorEnding 
            ? os_doubleDotsPrefix(retreats)
            : os_doubleDots(retreats);
         return;
      }
      else if (end == -1) {
         if (retreats == 0) {
            path = separatorEnding
               ? str(CHAR_DOT, OS_SEPARATOR)
               : toStr(CHAR_DOT);
            return;
         }

         path = separatorEnding 
            ? os_doubleDotsPrefix(retreats)
            : os_doubleDots(retreats);
         return;
      }
   }

   rtExitEnd:

   const p_int limit = static_cast<p_int>(path.size() - 1);

   if (end != limit) {
      if (separatorEnding) {
         end++;

         while (end <= limit) {
            if (path[end] == OS_SEPARATOR || path[end] == OS_WRONG_SEPARATOR) {
               end++;
            }
            else {
               end--;
               break;
            }
         }

         path = path.substr(0, end + 1);
         if (!os_endsWithSeparator(path)) {
            path += OS_SEPARATOR;
         }

         return;
      }

      path = path.substr(0, end + 1);
      return;
   }

   if (separatorEnding && !os_endsWithSeparator(path)) {
      path += OS_SEPARATOR;
   }
}

uint32_t os_patternInfo(const p_str& pattern)
{
   uint32_t result = parse::ASTERISK_INFO_NULL;
   const p_size length = pattern.size();

   if (length == 0 || pattern[length - 1] == CHAR_DOT) {
      return result;
   }

   if (os_isAbsolute(pattern)) {
      result |= parse::ASTERISK_INFO_IS_ABSOLUTE;
      if (length < 4 || pattern[2] != OS_SEPARATOR) {
         return result;
      }
   }

   p_char prev;
   p_size countAstrisks = 0;

   for (p_size i = 0; i < length; i++) {
      const p_char ch = pattern[i];

      switch (ch) {
         case CHAR_ASTERISK: {
            countAstrisks++;

            if (prev == CHAR_ASTERISK) {
               result |= parse::ASTERISK_INFO_DOUBLE_ASTERISK;
            }
            break;
         }
         case CHAR_SMALLER:
         case CHAR_GREATER:
         case CHAR_VERTICAL_BAR:
         case CHAR_QUESTION_MARK:
         case CHAR_QUOTATION_MARK: {
            return result;
         }
         case CHAR_COLON: {
            if (i != 1 || !(result & parse::ASTERISK_INFO_IS_ABSOLUTE)) {
               return result;
            }
            break;
         }
         case OS_SEPARATOR:{
            if (prev == OS_SEPARATOR) {
               return result;
            }
         }
      }

      prev = ch;
   }

   if (countAstrisks == 1) {
      result |= parse::ASTERISK_INFO_ONE_ASTERISK;
   }

   result |= parse::ASTERISK_INFO_VALID;
   return result;
}

p_str os_doubleDotsPrefix(p_int amount)
{
   p_str result;
   result.reserve(amount * 3);

   while (amount > 0) {
      result += CHAR_DOT;
      result += CHAR_DOT;
      result += OS_SEPARATOR;
      amount--;
   }

   return result;
}

p_str os_doubleDots(p_int amount)
{
   p_str result;
   result.reserve(amount * 3 - 1);

   while (amount > 0) {
      result += CHAR_DOT;
      result += CHAR_DOT;
      if (amount > 1) {
         result += OS_SEPARATOR;
      }
      amount--;
   }

   return result;
}

p_bool os_hasDotSegments(const p_str& path)
{
   p_int prev = path.size() - 1;

   for (p_int i = path.size() - 1; i >= 0; i--) {
      const p_char ch = path[i];

      if (ch == OS_SEPARATOR) {
         const p_int len = prev - i;

         if (len == 2 && path[i + 1] == CHAR_DOT && path[i + 2] == CHAR_DOT) {
            return true;
         }
         else if (len == 1 && path[i + 1] == CHAR_DOT) { 
            return true;
         }

         prev = i - 1;
      }
   }

   const p_int len = prev + 1;

   return (len == 2 && path[0] == CHAR_DOT && path[1] == CHAR_DOT)
      || (len == 1 && path[0] == CHAR_DOT);
}

p_str os_trimRetreats(const p_str& path, p_size& retreats)
{
   if (path.empty()) {
      return p_str();
   }

   p_size prevId = 0;
   p_size i = 0;

   for (; i < path.size(); i++) {
      if (path[i] == OS_SEPARATOR) {
         const p_size len = i - prevId;
         if (len == 1 && path[prevId] == CHAR_DOT) {
            prevId = i + 1;
            continue;
         }
         else if (len == 2 && path[prevId] == CHAR_DOT && path[prevId + 1] == CHAR_DOT) {
            retreats++;
         }
         else {
            return path.substr(prevId);
         }

         prevId = i + 1;
      }
   }

   const p_int len = path.size() - prevId;

   if (len == 2 && path[prevId] == CHAR_DOT && path[prevId + 1] == CHAR_DOT) {
      retreats++;
      return p_str();
   }
   else if (!(len == 1 && path[prevId] == CHAR_DOT)) {
      return path.substr(prevId);
   }

   return toStr(CHAR_DOT);
}

p_str os_segmentWithName(const p_str& path)
{
   p_int prev = path.size() - 1;
   p_int level = 0;

   for (p_int i = path.size() - 1; i >= 0; i--) {
      const p_char ch = path[i];

      if (ch == OS_SEPARATOR) {
         const p_int len = prev - i;

         if (len == 2 && path[i + 1] == CHAR_DOT && path[i + 2] == CHAR_DOT) {
            level--;
         }
         else if (len == 1 && path[i + 1] == CHAR_DOT) { }
         else {
            if (level == 0) {
               return path.substr(0, prev + 1);
            }
            else {
               level++;
            }
         }

         prev = i - 1;
      }
   }

   const p_int len = prev + 1;


   if (len == 2 && path[0] == CHAR_DOT && path[1] == CHAR_DOT) {
      return p_str();
   }
   else if (len == 1 && path[0] == CHAR_DOT) {
      return p_str();
   }

   if (level < 0) {
      return p_str();
   }

   return path.substr(0, prev + 1);
}

p_str os_retreats(p_int times)
{
   p_str result;
   result.reserve(times * 3);

   while (times > 0) {
      times--;
      result += CHAR_DOT;
      result += CHAR_DOT;
      result += OS_SEPARATOR;
   }

   return result;
}

p_bool os_retreatPath(p_str& path)
{
   if (path.empty()) {
      return false;
   }

   for (p_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == OS_SEPARATOR) {
         path.resize(i);
         return true;
      }
   }

   path.clear();
   return true;
}


void os_retreatPath(p_str& path, p_int times)
{
   if (times <= 0 || path.empty()) {
      return;
   }

   for (p_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == OS_SEPARATOR) {
         times--;
         if (times <= 0) {
            path.resize(i);
            return;
         }
      }
   }

   path.clear();
}


void os_retreatDirtyPath(p_str& path)
{
   if (path.empty()) {
      return;
   }

   for (p_int i = path.size() - 1; i >= 0; i--) {
      switch (path[i]) {
         case OS_SEPARATOR:
         case OS_WRONG_SEPARATOR: {
            path.resize(i + 1);
            return;
         }
      }
   }

   path.clear();
}


p_bool os_pathHasHead(const p_str& path)
{
   switch (path.size()) {
      case 0:
         return false;
      case 1:
         return path[0] != CHAR_DOT;
      case 2:
         return ! (path[0] == CHAR_DOT && path[1] == CHAR_DOT);
      default: 
         return ! ((path[path.size() - 1] == CHAR_DOT) 
                && (path[path.size() - 2] == CHAR_DOT)
                && (path[path.size() - 3] == OS_SEPARATOR));
   }
}


p_bool os_extendPath(p_str& result, const p_str& path)
{
   if (path.empty()) {
      result.clear();
      return false;
   }

   p_int prevId = 0;
   p_int retreats = 0;
   p_size i = 0;
   p_bool thereWereRetreats = false;
   const p_bool wasAbsolute = os_isAbsolute(path);
   p_bool wasEmpty = false;

   if (os_isAbsolute(path)) {
      if (path.size() == 2) {
         result = path;
         return true;
      }

      result = path.substr(0, 2);
      i = 3;
      prevId = i;
   }

   for (; i < path.size(); i++) {
      if (path[i] == OS_SEPARATOR) {
         const p_int len = i - prevId;
         if (len == 1 && path[prevId] == CHAR_DOT) {
            prevId = i + 1;
            continue;
         }
         else if (len == 2 && path[prevId] == CHAR_DOT && path[prevId + 1] == CHAR_DOT) {
            if (!os_retreatPath(result)) {
               retreats++;
               thereWereRetreats = true;
            }

            if (result.empty()) {
               wasEmpty = true;
            }
         }
         else {
            if (!result.empty()) {
               result += OS_SEPARATOR;
            }
            result += path.substr(prevId, len);
         }

         prevId = i + 1;
      }
   }

   const p_int len = path.size() - prevId;

   if (len == 2 && path[prevId] == CHAR_DOT && path[prevId + 1] == CHAR_DOT) {
      if (!os_retreatPath(result)) {
         retreats++;
         thereWereRetreats = true;
      }

      if (result.empty()) {
         wasEmpty = true;
      }
   }
   else if (!(len == 1 && path[prevId] == CHAR_DOT)) {
      if (!result.empty()) {
         result += OS_SEPARATOR;
      }
      result += path.substr(prevId, len);
   }

   if (wasAbsolute && (wasEmpty || thereWereRetreats)) {
      result.clear();
      return false;
   }

   if (retreats > 0) {
      p_str beginning;
      beginning.reserve(retreats * 3 - 1);
      beginning.push_back(CHAR_DOT);
      beginning.push_back(CHAR_DOT);

      while (retreats > 1) {
         beginning.push_back(OS_SEPARATOR);
         beginning.push_back(CHAR_DOT);
         beginning.push_back(CHAR_DOT);
         retreats--;
      }

      result = result.empty()
         ? str(beginning, result)
         : str(beginning, OS_SEPARATOR, result);
      return false;
   }

   if (result.empty()) {
      result += CHAR_DOT;
      return false;
   }

   return !thereWereRetreats;
}

p_str os_softJoin(const p_str& path1, const p_str& path2)
{
   return os_isAbsolute(path2)
      ? path2
      : str(path1, OS_SEPARATOR, path2);
}

p_str os_leftJoin(const p_str& path1, const p_str& path2)
{
   p_str result = path1;
   if (os_extendPath(result, path2)) {
      return result;
   }

   return p_str();
}

p_str os_join(const p_str& path1, const p_str& path2)
{
   if (os_isInvalid(path1) || os_isInvalid(path2)) {
      return p_str();
   }

   p_str result;
   os_extendPath(result, os_isAbsolute(path2) ? path2 : str(path1, OS_SEPARATOR, path2));
   return result;
}

p_bool os_endsWithDoubleDot(const p_str& path)
{
   const p_size len = path.size();

   if (len == 2) {
      return path[0] == CHAR_DOT && path[1] == CHAR_DOT;
   }

   if (len < 2) {
      return false;
   }

   return path[len - 1] == CHAR_DOT
       && path[len - 2] == CHAR_DOT
       && path[len - 3] == OS_SEPARATOR;
}

p_bool os_endsWithSeparator(const p_str& path)
{
   if (path.empty()) {
      return false;
   }

   switch (path[path.size() - 1]) {
      case OS_SEPARATOR:
      case OS_WRONG_SEPARATOR: {
         return true;
      }
      default: {
         return false;
      }
   }
}

p_bool os_isAbsolute(const p_str& path)
{
   switch (path.size()) {
      case 0:
      case 1: {
         return false;
      }
      case 2: {
         break;
      }
      default: {
         if (path[2] != OS_SEPARATOR && path[2] != OS_WRONG_SEPARATOR) {
            return false;
         }
         break;
      }
   }

   return path[1] == CHAR_COLON && os_isDriveLetter(path[0]);
}

p_bool os_hasExtension(const p_str& value)
{
   const p_int len = value.size();

   for (p_int i = len - 1; i >= 0; i--) {
      const p_char ch = value[i];

      if (ch == CHAR_DOT) {
         return (i == len - 1 || i == 0)
            ? false
            : value[i - 1] != OS_SEPARATOR;
      }
      else if (ch == OS_SEPARATOR) {
         return false;
      }
   }

   return false;
}

p_bool os_isPerun2Extension(const p_str& value)
{
   const p_size length = value.length();

   if (length <= metadata::EXTENSION_LENGTH) {
      return false;
   }

   if (value[length - metadata::EXTENSION_LENGTH - 1] != CHAR_DOT) {
      return false;
   }

   for (p_size i = 0; i < metadata::EXTENSION_LENGTH; i++) {
      if (metadata::EXTENSION[i] != value[length + i - metadata::EXTENSION_LENGTH]) {
         return false;
      }
   }

   return true;
}

p_bool os_isDriveLetter(const p_char ch)
{
   return (ch >= CHAR_a && ch <= CHAR_z)
       || (ch >= CHAR_A && ch <= CHAR_Z);
}

p_bool os_isPath(const p_str& value)
{
   return value.find(OS_SEPARATOR) != p_str::npos;
}

p_bool os_isAncestor(const p_str& path, const p_str& supposedChildPath)
{
   if (supposedChildPath.size() <= path.size()) {
      return false;
   }

   for (p_size i = 0; i < path.size(); i++) {
      if (!os_areEqualInPath(path[i], supposedChildPath[i])) {
         return false;
      }
   }

   return supposedChildPath[path.size() + 1] == OS_SEPARATOR;
}

p_bool os_hasParentDirectory(const p_str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   for (p_int i = path.size() - 1; i >= 1; i--) {
      if (path[i] == OS_SEPARATOR) {
         return true;
      }
   }

   return false;
}

// stack path to an element without extension
p_str os_stackPath(const p_str& path)
{
   if (os_pathWasStacked(path)) {
      return os_stackPathStacked(path);
   }

   p_nint index = NINT_TWO;
   p_str newPath = path;

   while (os_exists(newPath))
   {
      newPath = str(path, CHAR_OPENING_ROUND_BRACKET, toStr(index), CHAR_CLOSING_ROUND_BRACKET);
      index++;
   }

   return newPath;
}

p_str os_stackPathExt(const p_str& basePath, const p_str& extension)
{
   if (os_pathWasStacked(basePath)) {
      return os_stackPathExtStacked(basePath, extension);
   }

   p_nint index = NINT_TWO;
   p_str newPath = str(basePath, CHAR_DOT, extension);

   while (os_exists(newPath))
   {
      newPath = str(basePath, CHAR_OPENING_ROUND_BRACKET, toStr(index),
         CHAR_CLOSING_ROUND_BRACKET, CHAR_DOT, extension);
      index++;
   }

   return newPath;
}

p_str os_stackPathBase(const p_str& path)
{
   for (p_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == CHAR_DOT) {
         return path.substr(0, i);
      }
   }

   return path;
}

p_str os_stackPathStacked(const p_str& path)
{
   p_nint index;
   p_str basePath;
   os_getStackedData(path, index, basePath);

   p_str newPath = str(basePath, CHAR_OPENING_ROUND_BRACKET,
      toStr(index), CHAR_CLOSING_ROUND_BRACKET);

   while (os_exists(newPath))
   {
      index++;
      newPath = str(basePath, CHAR_OPENING_ROUND_BRACKET,
         toStr(index), CHAR_CLOSING_ROUND_BRACKET);
   }

   return newPath;
}

p_str os_stackPathExtStacked(const p_str& path, const p_str& extension)
{
   p_nint index;
   p_str basePath;
   os_getStackedData(path, index, basePath);

   p_str newPath = str(basePath, CHAR_OPENING_ROUND_BRACKET,
      toStr(index), CHAR_CLOSING_ROUND_BRACKET, CHAR_DOT, extension);

   while (os_exists(newPath))
   {
      index++;
      newPath = str(basePath, CHAR_OPENING_ROUND_BRACKET,
         toStr(index), CHAR_CLOSING_ROUND_BRACKET, CHAR_DOT, extension);
   }

   return newPath;
}

p_bool os_pathWasStacked(const p_str& basePath)
{
   const p_size len = basePath.size();
   if (len < 4 || basePath[len - 1] != CHAR_CLOSING_ROUND_BRACKET) {
      return false;
   }

   const p_int beginning = static_cast<p_int>(len - 2);

   for (p_int i = beginning; i >= 0; i--) {
      if (basePath[i] == CHAR_OPENING_ROUND_BRACKET) {
         if (i == beginning || i == 0) {
            return false;
         }

         switch(basePath[i - 1]) {
            case OS_SEPARATOR:
            case OS_WRONG_SEPARATOR: {
               return false;
            }
            default: {
               return true;
            }
         }
      }
      else if (!char_isDigit(basePath[i])) {
         return false;
      }
   }

   return false;
}

void os_getStackedData(const p_str& path, p_nint& index, p_str& basePath)
{
   const p_size len = path.size();

   for (p_int i = static_cast<p_int>(len - 2); i >= 0; i--) {
      if (!char_isDigit(path[i])) {
         basePath = path.substr(0, i);
         const p_str numStr = path.substr(i + 1, static_cast<p_int>(len) - i - 2);

         try {
            index = std::stoll(numStr);
         }
         catch (...) {
            index = NINT_TWO;
         }

         return;
      }
   }
}

p_str os_executablePath()
{
   p_char path[MAX_PATH];
   GetModuleFileNameW(NULL, path, MAX_PATH);
   return p_str(path);
}

p_str os_desktopPath()
{
   p_char path[MAX_PATH];
   return SHGetSpecialFolderPathW(0, path, CSIDL_DESKTOP, FALSE)
      ? p_str(path)
      : p_str();
}

p_list os_pendrives()
{
   p_list result;
   DWORD drivesBitMask = GetLogicalDrives();

   for (p_char drive = CHAR_A; drive <= CHAR_Z; drive++) {
      if (drivesBitMask & 1) {
         const p_str rootPath = str(drive, CHAR_COLON, CHAR_BACKSLASH);
         const UINT driveType = GetDriveTypeW(rootPath.c_str());

         if (driveType == DRIVE_REMOVABLE) {
            result.push_back(rootPath);
         }
      }

      drivesBitMask >>= 1;
   }

   return result;
}

p_str os_currentPath()
{
   p_char path[MAX_PATH];
   GetCurrentDirectory(MAX_PATH, path);
   return p_str(path);
}

p_str os_system32Path()
{
   p_char path[MAX_PATH];
   return SHGetSpecialFolderPathW(0, path, CSIDL_SYSTEM, FALSE)
      ? p_str(path)
      : p_str();
}

p_str os_downloadsPath()
{
   HKEY hKey;
   LONG result = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 0, KEY_READ, &hKey);

   if (result != ERROR_SUCCESS) {
      RegCloseKey(hKey);
      return EMPTY_STRING;
   }

   p_char buffer[MAX_PATH];
   DWORD bufferSize = sizeof(buffer);
   result = RegQueryValueExW(hKey, L"{374DE290-123F-4565-9164-39C4925E467B}", nullptr, nullptr, reinterpret_cast<BYTE*>(buffer), &bufferSize);
   RegCloseKey(hKey);

   if (result == ERROR_SUCCESS) {
      return buffer;
   } 
   else {
      return EMPTY_STRING;
   }
}

std::optional<p_str> os_readStringFromCmd(const p_str& cmd)
{
   std::array<p_char, 256> buffer;
   p_str result;
   FILE* pipe = _wpopen(cmd.c_str(), L"r");
   
   if (! pipe) {
      return std::nullopt;
   }

   if (fgetws(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
      result += buffer.data();
   }

   _pclose(pipe);
   return result.empty()
      ? std::nullopt
      : std::make_optional<p_str>(result);
}

std::optional<p_str> os_registry_readPython3Path(const p_str& version, HKEY rootKey) 
{
   HKEY hKey;
   const p_str regPath = str(L"SOFTWARE\\Python\\PythonCore\\", version, L"\\InstallPath");
   
   LONG result = RegOpenKeyExW(rootKey, regPath.c_str(), 0, KEY_READ, &hKey);
   if (result != ERROR_SUCCESS) {
      return std::nullopt;
   }

   p_char value[MAX_PATH];
   DWORD valueSize = sizeof(value);
   DWORD type = 0;

   result = RegQueryValueExW(hKey, L"ExecutablePath", nullptr, &type, (LPBYTE)value, &valueSize);
   RegCloseKey(hKey);

   if (result == ERROR_SUCCESS && type == REG_SZ) {
      return std::make_optional<p_str>(value);
   } 
   else {
      return std::nullopt;
   }
}

std::vector<p_str> os_registry_getAllPython3s(HKEY rootKey)
{
   std::vector<p_str> output;

   const p_char* subKey = L"SOFTWARE\\Python\\PythonCore";
   HKEY hKey;
   
   LONG result = RegOpenKeyExW(rootKey, subKey, 0, KEY_READ, &hKey);
   if (result != ERROR_SUCCESS) {
      return output;
   }

   DWORD index = 0;
   DWORD nameSize;
   p_char name[MAX_PATH];
   
   while (true) {
      nameSize = sizeof(name) / sizeof(p_char);
      result = RegEnumKeyExW(hKey, index, name, &nameSize, nullptr, nullptr, nullptr, nullptr);
      
      if (result == ERROR_NO_MORE_ITEMS) {
         break;
      }
      else if (result == ERROR_SUCCESS) {
         const p_str nameAsString = name;
         const std::optional<p_str> path = os_registry_readPython3Path(nameAsString, rootKey);

         if (path.has_value() && os_fileExists(path.value())) {
            p_str realPath = path.value();
            str_toLower(realPath);
            output.emplace_back(realPath);
         }
      }

      ++index;
   }

   RegCloseKey(hKey);
   return output;
}

std::optional<p_str> os_readPython3PathFromCmd(const p_str& alias, std::optional<p_str>& versionString) 
{
   versionString = os_readStringFromCmd(str(alias, L" --version 2>nul"));

   if (versionString.has_value() && str_startsWith(versionString.value(), L"Python 3.")) {
      std::optional<p_str> path = os_readStringFromCmd(str(L"where ", alias, L" 2>nul"));

      if (path.has_value()) {
         p_str output = path.value();
         str_toLower(output);
         str_trimEndNewLines(output);
         return std::make_optional<p_str>(output);
      }
   }

   return std::nullopt;
}

Python3State os_getPython3(p_str& cmdPath)
{
   // The algorithm is quite complex.
   // 1. Read all paths to Python3 from the registry. Give up if nothing is found.
   // 2. Try to get the path by executing "python3 --version". 
   // If this path exists and is also found in the registry, return it.
   // 3. Try to get the path by executing "python --version". Then do the same as in 2.
   // 4. Try to return the first value from HKCU.
   // 5. Try to return the first value from HKLM.
   // 6. Give up and return nothing. Try to give a message about Python being not 3.*, if possible.

   // Step 1.
   const std::vector<p_str> hkcu = os_registry_getAllPython3s(HKEY_CURRENT_USER);
   const std::vector<p_str> hklm = os_registry_getAllPython3s(HKEY_LOCAL_MACHINE);

   if (hkcu.empty() && hklm.empty()) {
      return Python3State::P3_NotInstalled;
   }

   // Step 2.
   std::optional<p_str> versionStringPython3;
   std::optional<p_str> python3 = os_readPython3PathFromCmd(L"python3", versionStringPython3);
   if (python3.has_value()) {
      const p_str& path = python3.value();
         
      for (const p_str& registryPath : hkcu) {
         if (registryPath == path) {
            cmdPath = path;
            return Python3State::P3_Installed;
         }
      }

      for (const p_str& registryPath : hklm) {
         if (registryPath == path) {
            cmdPath = path;
            return Python3State::P3_Installed;
         }
      }
   }

   // Step 3.
   std::optional<p_str> versionStringPython;
   std::optional<p_str> python = os_readPython3PathFromCmd(L"python", versionStringPython);
   if (python.has_value()) {
      const p_str& path = python.value();
         
      for (const p_str& registryPath : hkcu) {
         if (registryPath == path) {
            cmdPath = path;
            return Python3State::P3_Installed;
         }
      }

      for (const p_str& registryPath : hklm) {
         if (registryPath == path) {
            cmdPath = path;
            return Python3State::P3_Installed;
         }
      }
   }

   // Step 4.
   if (! hkcu.empty()) {
      cmdPath = hkcu[0];
      return Python3State::P3_Installed;
   }

   // Step 5.
   if (! hklm.empty()) {
      cmdPath = hklm[0];
      return Python3State::P3_Installed;
   }
   
   // Step 6.
   if (versionStringPython.has_value()) {
      const p_str& version = versionStringPython.value();

      if (str_startsWith(version, L"Python ") && ! str_startsWith(version, L"Python 3")) {
         return Python3State::P3_DifferentVersionThan3;
      }
   }

   return Python3State::P3_NotInstalled;
}

p_size os_readFileSize(const p_str& path)
{
   struct _stat fileinfo;
   _wstat(path.c_str(), &fileinfo);
   return fileinfo.st_size;
}

p_bool os_readFile(p_str& result, const p_str& path)
{
   FILE* f = _wfopen(path.c_str(), STRING_FILE_OPEN_MODE);

   if (f == NULL) {
      return false;
   }

   const p_size filesize = os_readFileSize(path);

   if (filesize > 0) {
      result.resize(filesize);
      p_size wchars_read = fread(&(result.front()), sizeof(p_char), filesize, f);
      result.resize(wchars_read);
      result.shrink_to_fit();
   }

   fclose(f);
   return true;
}

void os_showWebsite(const p_str& url)
{
   ShellExecuteW(NULL, STRING_OPEN, url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

p_bool os_findText(const p_str& path, const p_str& value)
{
   std::wifstream stream(P_WINDOWS_PATH(path));
   if (!stream) {
      return false;
   }
   else if (value.empty()) {
      stream.close();
      return true;
   }

   p_str line;
   p_bool result = false;

   while (std::getline(stream, line)) {
      if (line.find(value) != p_str::npos) {
         result = true;
         break;
      }
   }

   stream.close();
   return result;
}

p_bool os_areEqualInPath(const p_char ch1, const p_char ch2)
{
   return std::tolower(ch1, std::locale("")) == std::tolower(ch2, std::locale(""));
}

p_str os_makeArg(const p_str& value)
{
   p_size specials = 0;

   for (p_size i = 0; i < value.size(); i++) {
      const p_char ch = value[i];

      if (ch == CHAR_QUOTATION_MARK) {
         specials++;
         continue;
      }

      if (ch == CHAR_BACKSLASH && i != (value.size() - 1) && value[i + 1] == CHAR_QUOTATION_MARK) {
         specials++;
      }
   }

   if (specials == 0) {
      return str(CHAR_QUOTATION_MARK, value, CHAR_QUOTATION_MARK);
   }

   p_str result = toStr(CHAR_QUOTATION_MARK);
   result.reserve(value.size() + specials + 2);

   for (p_size i = 0; i < value.size(); i++) {
      const p_char ch = value[i];

      if (ch == CHAR_QUOTATION_MARK) {
         result += CHAR_BACKSLASH;
      }
      else if (ch == CHAR_BACKSLASH && i != (value.size() - 1) && value[i + 1] == CHAR_QUOTATION_MARK) {
         result += CHAR_BACKSLASH;
      }

      result += ch;
   }

   result += CHAR_QUOTATION_MARK;
   return result;
}

inline uint64_t os_bigInteger(const uint32_t low, const uint32_t high)
{
   uint64_t n = high;
   n <<= sizeof(high) * 8;
   n |= low;
   return n;
}

inline p_bool os_isBrowsePath(const p_str& path)
{
   // this is an equivalent to
   // return path == . || path == ..
   switch (path.size()) {
      case 1: {
         return path[0] == CHAR_DOT;
      }
      case 2: {
         return path[0] == CHAR_DOT && path[1] == CHAR_DOT;
      }
      default: {
         return false;
      }
   }
}

inline p_tim os_convertToPerun2Time(const p_ftim* time)
{
   _FILETIME ftime;
   if (FileTimeToLocalFileTime(time, &ftime)) {
      _SYSTEMTIME stime;

      if (FileTimeToSystemTime(&ftime, &stime)) {
         return p_tim(stime.wDay, stime.wMonth, stime.wYear,
            stime.wHour, stime.wMinute, stime.wSecond);
      }
      else {
         return p_tim();
      }
   }
   else {
      return p_tim();
   }
}

inline p_bool os_convertToFileTime(const p_tim& perunTime, p_ftim& result)
{
   _SYSTEMTIME stime;
   stime.wYear = perunTime.year;
   stime.wMonth = perunTime.month;
   stime.wDay = perunTime.day;
   stime.wHour = perunTime.hour;
   stime.wMinute = perunTime.minute;
   stime.wSecond = perunTime.second;

   _FILETIME ftime;
   if (!SystemTimeToFileTime(&stime, &ftime)) {
      return false;
   }

   return LocalFileTimeToFileTime(&ftime, &result);
}


std::string os_toUtf8(const p_str& value)
{
   if (value.empty()) {
      return std::string();
   }
    
   const int length = WideCharToMultiByte(CP_UTF8, 0, &value[0], (int)value.size(), nullptr, 0, nullptr, nullptr);
   std::string result(length, 0);

   WideCharToMultiByte(CP_UTF8, 0, &value[0], (int)value.size(), &result[0], length, nullptr, nullptr);
   return result;
}

}
