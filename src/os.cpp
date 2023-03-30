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

#include "os.h"
#include <time.h>
#include "perun2.h"
#include "datatype/parse/parse-asterisk.h"
#include <shlobj.h>
#include <cwctype>
#include <shellapi.h>
#include <shlwapi.h>
#include <algorithm>
#include <fstream>

namespace perun2
{

_constexpr _nint OS_SLEEP_UNIT = NINT_300;
// while sleeping
// check every 300 ms if the program received an interruption signal


_tim os_now()
{
   time_t raw;
   time(&raw);
   const struct tm* info = localtime(&raw);

   return _tim(info->tm_mday, info->tm_mon + 1, 1900 + info->tm_year,
      info->tm_hour, info->tm_min, info->tm_sec);
}

_tim os_today()
{
   time_t raw;
   time(&raw);
   const struct tm* info = localtime(&raw);

   return _tim(info->tm_mday, info->tm_mon + 1, 1900 + info->tm_year);
}

_tim os_tomorrow()
{
   _tim t = os_today();
   t.addDays(1);
   return t;
}

_tim os_yesterday()
{
   _tim t = os_today();
   t.addDays(-1);
   return t;
}

void os_sleepForMs(const _nint ms, _p2& p2)
{
   if (ms <= NINT_ZERO) {
      return;
   }

   _nint loops = ms / OS_SLEEP_UNIT;
   const _nint remainder = ms % OS_SLEEP_UNIT;

   os_rawSleepForMs(remainder);

   while (p2.state == State::s_Running && loops != NINT_ZERO) {
      os_rawSleepForMs(OS_SLEEP_UNIT);
      loops--;
   }
}

void os_rawSleepForMs(const _nint ms)
{
   Sleep(ms);
}

// explanation of attributes is in file 'attribute.h'
void os_loadAttributes(FileContext& context)
{
   const _attrptr& attribute = context.attribute;
   context.trimmed = os_trim(context.this_->value);
   context.invalid = os_isInvaild(context.trimmed);

   if (context.invalid) {
      os_loadEmptyAttributes(context);
      return;
   }

   _str path;

   // "drive", "depth", "path", "parent" and "fullname" do not require access to the file system
   if (attribute->has(ATTR_PATH)) {
      path = os_join(context.locContext->location->value, context.trimmed);
      context.v_path->value = path;
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value = os_fullname(context.trimmed);
   }

   if (attribute->has(ATTR_PARENT)) {
      context.v_parent->value = os_parent(path);
   }

   if (attribute->has(ATTR_DRIVE)) {
      context.v_drive->value = os_drive(path);
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = os_depth(context.trimmed);
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   // below are "real" attributes of files and directories
   _adata data;
   const _bool gotAttrs = GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data);
   const DWORD& dwAttrib = data.dwFileAttributes;
   const _bool exists = gotAttrs && dwAttrib != INVALID_FILE_ATTRIBUTES;
   context.v_exists->value = exists;

   _bool isDir;
   _bool isFile;

   if (exists) {
      isDir = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
      isFile = !isDir;
   }
   else {
      isFile = os_hasExtension(context.trimmed);
      isDir = !isFile;
   }

   context.v_isfile->value = isFile;
   context.v_isdirectory->value = isDir;

   if (attribute->has(ATTR_ACCESS)) {
      context.v_access->value = exists
         ? os_convertToPerun2Time(&data.ftLastAccessTime)
         : _tim();
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      context.v_archive->value = exists ? (dwAttrib & FILE_ATTRIBUTE_ARCHIVE) : false;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      context.v_compressed->value = exists ? (dwAttrib & FILE_ATTRIBUTE_COMPRESSED) : false;
   }

   if (attribute->has(ATTR_CREATION)) {
      context.v_creation->value = exists
         ? os_convertToPerun2Time(&data.ftCreationTime)
         : _tim();
   }

   const _bool hasMod = attribute->has(ATTR_MODIFICATION);
   const _bool hasChange = attribute->has(ATTR_CHANGE);
   if (hasMod || hasChange) {
      const _tim time = exists
         ? os_convertToPerun2Time(&data.ftLastWriteTime)
         : _tim();

      if (hasChange) {
         context.v_change->value = time;
      }
      if (hasMod) {
         context.v_modification->value = time;
      }
   }

   if (attribute->has(ATTR_LIFETIME)) {
      if (exists) {
         context.v_lifetime->value = context.v_creation->value < context.v_modification->value
            ? (os_now() - context.v_creation->value)
            : (os_now() - context.v_modification->value);
      }
      else {
         context.v_lifetime->value = _per();
      }
   }

   if (attribute->has(ATTR_EMPTY)) {
      if (exists) {
         context.v_empty->value = isFile
            ? os_emptyFile(data)
            : os_emptyDirectory(path);
      }
      else {
         context.v_empty->value = false;
      }
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      context.v_encrypted->value = exists ? (dwAttrib & FILE_ATTRIBUTE_ENCRYPTED) : false;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      context.v_extension->value = isFile ? os_extension(context.trimmed) : EMPTY_STRING;
   }

   if (attribute->has(ATTR_HIDDEN)) {
      context.v_hidden->value = exists ? (dwAttrib & FILE_ATTRIBUTE_HIDDEN) : false;
   }

   if (attribute->has(ATTR_NAME)) {
      if (isDir) {
         context.v_name->value = os_fullname(context.trimmed);
      }
      else {
         context.v_name->value = os_hasExtension(context.trimmed)
            ? os_name(context.trimmed)
            : os_fullname(context.trimmed);
      }
   }

   if (attribute->has(ATTR_READONLY)) {
      context.v_readonly->value = exists ? (dwAttrib & FILE_ATTRIBUTE_READONLY) : false;
   }

   if (attribute->has(ATTR_SIZE)) {
      if (exists) {
         context.v_size->value = isFile
            ? _num(static_cast<_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh)))
            : _num(os_sizeDirectory(path, context.attribute->perun2));
      }
      else {
         context.v_size->value = _num(NINT_MINUS_ONE);
      }
   }
}

// attributes of something, that cannot exist (empty string, contains not allowed chars, ...)
void os_loadEmptyAttributes(FileContext& context)
{
   const _attrptr& attribute = context.attribute;

   if (attribute->has(ATTR_PATH)) {
      context.v_path->value.clear();
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value.clear();
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = _num(NINT_MINUS_ONE);
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
      context.v_access->value = _tim();
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      context.v_archive->value = false;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      context.v_compressed->value = false;
   }

   if (attribute->has(ATTR_CHANGE)) {
      context.v_change->value = _tim();
   }

   if (attribute->has(ATTR_CREATION)) {
      context.v_creation->value = _tim();
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
      context.v_lifetime->value = _per();
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      context.v_modification->value = _tim();
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

   if (attribute->has(ATTR_SIZE)) {
      context.v_size->value = _num(NINT_MINUS_ONE);
   }
}

void os_loadDataAttributes(FileContext& context, const _fdata& data)
{
   const _attrptr& attribute = context.attribute;
   context.trimmed = os_trim(context.this_->value);
   context.invalid = false;
   _str path;

   // "drive", "depth", "path", "parent" and "fullname" do not require access to the file system
   if (attribute->has(ATTR_PATH)) {
      path = os_join(context.locContext->location->value, context.trimmed);
      context.v_path->value = path;
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value = os_fullname(context.trimmed);
   }

   if (attribute->has(ATTR_PARENT)) {
      context.v_parent->value = os_parent(path);
   }

   if (attribute->has(ATTR_DRIVE)) {
      context.v_drive->value = os_drive(path);
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = os_depth(context.trimmed);
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   // below are "real" attributes of files and directories
   const DWORD& dwAttrib = data.dwFileAttributes;
   context.v_exists->value = true;

   const _bool isDir = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
   const _bool isFile = !isDir;

   context.v_isfile->value = isFile;
   context.v_isdirectory->value = isDir;

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

   const _bool hasMod = attribute->has(ATTR_MODIFICATION);
   const _bool hasChange = attribute->has(ATTR_CHANGE);
   if (hasMod || hasChange) {
      const _tim time = os_convertToPerun2Time(&data.ftLastWriteTime);

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
      context.v_empty->value = isFile
         ? (data.nFileSizeLow == 0 && data.nFileSizeHigh == 0)
         : os_emptyDirectory(path);
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      context.v_encrypted->value = (dwAttrib & FILE_ATTRIBUTE_ENCRYPTED);
   }

   if (attribute->has(ATTR_EXTENSION)) {
      context.v_extension->value = isFile ? os_extension(context.trimmed) : EMPTY_STRING;
   }

   if (attribute->has(ATTR_HIDDEN)) {
      context.v_hidden->value = (dwAttrib & FILE_ATTRIBUTE_HIDDEN);
   }

   if (attribute->has(ATTR_NAME)) {
      if (isDir) {
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
      context.v_size->value = isFile
         ? _num(static_cast<_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh)))
         : _num(os_sizeDirectory(path, context.attribute->perun2));
   }
}

_tim os_access(const _str& path)
{
   _adata data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return os_convertToPerun2Time(&data.ftLastAccessTime);
}

_bool os_archive(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_ARCHIVE);
}

_tim os_change(const _str& path)
{
   _adata data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return os_convertToPerun2Time(&data.ftLastWriteTime);
}

_bool os_compressed(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_COMPRESSED);
}

_tim os_creation(const _str& path)
{
   _adata data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return os_convertToPerun2Time(&data.ftCreationTime);
}

_num os_depth(const _str& value)
{
   _nint result = NINT_ZERO;

   for (const _char ch : value) {
      if (ch == OS_SEPARATOR) {
         result++;
      }
   }

   return result;
}

_str os_drive(const _str& path)
{
   if (os_isAbsolute(path)) {
      _char letter = path[0];
      toUpper(letter);
      return str(toStr(letter), STRING_CHAR_COLON);
   }
   else {
      return EMPTY_STRING;
   }
}

_bool os_empty(const _str& path)
{
   _adata data;
   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
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

_bool os_encrypted(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_ENCRYPTED);
}

_bool os_emptyFile(const _adata& data)
{
   return data.nFileSizeLow == 0
       && data.nFileSizeHigh == 0;
}

_bool os_emptyDirectory(const _str& path)
{
   _fdata data;
   HANDLE handle = FindFirstFile((str(path, OS_SEPARATOR_ASTERISK)).c_str(), &data);
   if (handle == INVALID_HANDLE_VALUE) {
      return true;
   }
   else {
      FindNextFile(handle, &data);
      if (FindNextFile(handle, &data)) {
         FindClose(handle);
         return false;
      }

      FindClose(handle);
      return true;
   }
}

_str os_extension(const _str& value)
{
   for (_int i = value.size() - 1; i >= 0; i--) {
      const _char& ch = value[i];
      if (ch == CHAR_DOT) {
         if (i == 0 || value[i - 1] == OS_SEPARATOR) {
            return EMPTY_STRING;
         }

         _str result = value.substr(i + 1);
         toLower(result);
         return result;
      }
      else if (ch == OS_SEPARATOR) {
         return EMPTY_STRING;
      }
   }

   return EMPTY_STRING;
}

_str os_fullname(const _str& value)
{
   for (_int i = value.size() - 1; i >= 0; i--) {
      const _char& ch = value[i];
      if (ch == OS_SEPARATOR) {
         return value.substr(i + 1);
      }
   }

   return value;
}

_bool os_hasAttribute(const _str& path, const DWORD attribute)
{
   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES) &&
          (dwAttrib & attribute);
}

_bool os_hidden(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_HIDDEN);
}

_bool os_isFile(const _str& path)
{
   DWORD dwAttrib = GetFileAttributesW(path.c_str());
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return os_hasExtension(path);
   }

   return !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

_bool os_isDirectory(const _str& path)
{
   DWORD dwAttrib = GetFileAttributesW(path.c_str());
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return !os_hasExtension(path);
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
}

_per os_lifetime(const _str& path)
{
   _adata data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _per();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _per();
   }

   const _tim modification = os_convertToPerun2Time(&data.ftLastWriteTime);
   const _tim creation = os_convertToPerun2Time(&data.ftCreationTime);

   return creation < modification
      ? (os_now() - creation)
      : (os_now() - modification);
}

_tim os_modification(const _str& path)
{
   _adata data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return os_convertToPerun2Time(&data.ftLastWriteTime);
}

_str os_name(const _str& value)
{
   _int i = value.size() - 1;
   _int dot = -1;
   _bool sep = false;

   for (; i >= 0; i--) {
      const _char& ch = value[i];
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

_str os_parent(const _str& path)
{
   for (_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == OS_SEPARATOR) {
         return path.substr(0, i);
      }
   }

   return EMPTY_STRING;
}

_bool os_readonly(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_READONLY);
}

_nint os_size(const _str& path, _p2& p2)
{
   _adata data;
   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return NINT_MINUS_ONE;
   }

   const DWORD& dwAttrib = data.dwFileAttributes;
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return NINT_MINUS_ONE;
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY
      ? os_sizeDirectory(path, p2)
      : static_cast<_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh));
}

_nint os_sizeDirectory(const _str& path, _p2& p2)
{
   _nint totalSize = NINT_ZERO;
   _fdata data;
   HANDLE handle = FindFirstFile((str(path, OS_SEPARATOR_ASTERISK)).c_str(), &data);

   if (handle == INVALID_HANDLE_VALUE) {
      return totalSize;
   }

   do {
      if (!p2.state == State::s_Running) {
         FindClose(handle);
         return NINT_MINUS_ONE;
      }
      if (!os_isBrowsePath(data.cFileName)) {
         if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            totalSize += os_sizeDirectory(str(path, OS_SEPARATOR_STRING, data.cFileName), p2);
         }
         else {
            totalSize += static_cast<_nint>(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh));
         }
      }
   } while (FindNextFile(handle, &data));

   FindClose(handle);
   return totalSize;
}

_bool os_exists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   return GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

_bool os_fileExists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES && (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

_bool os_directoryExists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


//////
///
//  file system:
///
/////

_bool os_delete(const _str& path)
{
   _char wszFrom[MAX_PATH] = { 0 };
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

_bool os_drop(const _str& path, _p2& p2)
{
   return os_isFile(path)
      ? os_dropFile(path)
      : os_dropDirectory(path, p2);
}

_bool os_drop(const _str& path, const _bool isFile, _p2& p2)
{
   return isFile
      ? os_dropFile(path)
      : os_dropDirectory(path, p2);
}

_bool os_dropFile(const _str& path)
{
   WINBOOL w = DeleteFileW(path.c_str());

   if (w != 0) {
      return true;
   }
   else if (w == ERROR_ACCESS_DENIED) {
      if (os_unlock(path)) {
         return DeleteFileW(path.c_str()) != 0;
      }
   }
   return false;
}

_bool os_dropDirectory(const _str& path, _p2& p2)
{
   HANDLE hFind;
   _fdata FindFileData;

   _char DirPath[MAX_PATH];
   _char FileName[MAX_PATH];

   wcscpy(DirPath, const_cast<_char*>(path.c_str()));
   wcscat(DirPath, OS_SEPARATOR_ASTERISK.c_str());
   wcscpy(FileName, const_cast<_char*>(path.c_str()));
   wcscat(FileName, OS_SEPARATOR_STRING.c_str());

   hFind = FindFirstFile(DirPath,&FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) return false;
      wcscpy(DirPath,FileName);

   _bool bSearch = true;
   while (bSearch) {
      if (FindNextFile(hFind,&FindFileData)) {
         if (!p2.state == State::s_Running) {
            FindClose(hFind);
            return false;
         }

         if (os_isBrowsePath(FindFileData.cFileName)) {
            continue;
         }

         wcscat(FileName,FindFileData.cFileName);
         if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!os_dropDirectory(FileName, p2)) {
               FindClose(hFind);
               return false;
            }

            RemoveDirectoryW(FileName);
            wcscpy(FileName,DirPath);
         }
         else {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
               os_unlock(FileName);

            if (!DeleteFileW(FileName)) {
               FindClose(hFind);
               return false;
            }

            wcscpy(FileName,DirPath);
         }
      }
      else {
         if (GetLastError() == ERROR_NO_MORE_FILES)
            bSearch = false;
         else {
            FindClose(hFind);
            return false;
         }
      }
   }
   FindClose(hFind);

   return RemoveDirectoryW(const_cast<_char*>(path.c_str())) != 0;
}

_bool os_hide(const _str& path)
{
   const _char* p = path.c_str();
   const DWORD attr = GetFileAttributesW(p);
   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
      return SetFileAttributes(p, attr | FILE_ATTRIBUTE_HIDDEN) != 0;
   }

   return true;
}

_bool os_lock(const _str& path)
{
   const _char* p = path.c_str();
   const DWORD attr = GetFileAttributesW(p);
   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_READONLY) == 0) {
      return SetFileAttributes(p, attr | FILE_ATTRIBUTE_READONLY) != 0;
   }

   return true;
}

_bool os_open(const _str& path)
{
   const _str location = os_parent(path);
   return (INT_PTR)ShellExecuteW(0, 0, path.c_str(), 0, location.c_str() , SW_SHOW) > 32;
}

_bool os_openWith(const _str& program, const _str& path)
{
   const _str location = os_parent(path);
   return (INT_PTR)ShellExecuteW(NULL, STRING_OPEN.c_str(), program.c_str(), path.c_str(), location.c_str(), SW_SHOW) > 32;
}

_bool os_openAsCommand(const _str& command, const _str& location)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));

   const _size len = command.size() + 1;
   _char cmd[len];
   wcscpy(cmd, command.c_str());

   const _size lenloc = location.size() + 1;
   _char loc[lenloc];
   wcscpy(loc, location.c_str());

   return CreateProcessW
   (
      NULL,
      cmd,
      NULL, NULL, FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL, loc,
      &si, &pi
   ) != 0;
}

_bool os_unhide(const _str& path)
{
   const _char* p = path.c_str();
   const DWORD attr = GetFileAttributesW(p);
   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) {
      return SetFileAttributes(p, attr & ~FILE_ATTRIBUTE_HIDDEN) != 0;
   }

   return true;
}

_bool os_unlock(const _str& path)
{
   const _char* p = path.c_str();
   const DWORD attr = GetFileAttributesW(p);
   if (attr == INVALID_FILE_ATTRIBUTES) {
      return false;
   }

   if ((attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY) {
      return SetFileAttributes(p, attr & ~FILE_ATTRIBUTE_READONLY) != 0;
   }

   return true;
}


_bool os_setTime(const _str& path, const _tim& creation,
   const _tim& access, const _tim& modification)
{
   _ftim time_c;
   _ftim time_a;
   _ftim time_m;

   if (!(os_convertToFileTime(creation, time_c)
    && os_convertToFileTime(access, time_a)
    && os_convertToFileTime(modification, time_m)))
   {
      return false;
   }

   HANDLE handle = CreateFile(path.c_str(),
      FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE,
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (handle == INVALID_HANDLE_VALUE) {
      return false;
   }

   const _bool result = SetFileTime(handle, &time_c, &time_a, &time_m);
   CloseHandle(handle);
   return result;
}

_bool os_createFile(const _str& path)
{
   if (os_hasParentDirectory(path)) {
      const _str p = os_parent(path);
      if (!os_exists(p)) {
         const _bool b = os_createDirectory(p);
         if (!b) {
            return false;
         }
      }
   }
   else {
      return false;
   }

   HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL,
      CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

   if (h) {
      _ftim ftime;

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

_bool os_createDirectory(const _str& path)
{
   if (os_hasParentDirectory(path)) {
      const _str p = os_parent(path);
      if (!os_exists(p)) {
         const _bool b = os_createDirectory(p);
         if (!b) {
            return false;
         }
      }
   }
   else {
      return false;
   }

   return CreateDirectoryW(path.c_str(), NULL) != 0;
}

_bool os_moveTo(const _str& oldPath, const _str& newPath)
{
   return MoveFileExW(oldPath.c_str(), newPath.c_str(), MOVEFILE_COPY_ALLOWED) != 0;
}

_bool os_copyTo(const _str& oldPath, const _str& newPath, const _bool isFile, _p2& p2)
{
   if (isFile) {
      return os_copyToFile(oldPath, newPath);
   }

   if (os_isAncestor(oldPath, newPath)) {
      return false;
   }

   const _bool success = os_copyToDirectory(oldPath, newPath, p2);
   if (!success && !p2.state == State::s_Running && os_directoryExists(newPath)) {
      // if directory copy operation
      // was stopped by the user
      // delete recent partially copied directory if it is there
      os_dropDirectory(newPath, p2);
   }

   return success;
}

_bool os_copyToFile(const _str& oldPath, const _str& newPath)
{
   return CopyFileW(oldPath.c_str(), newPath.c_str(), true) != 0;
}

_bool os_copyToDirectory(const _str& oldPath, const _str& newPath, _p2& p2)
{
   if (!os_createDirectory(newPath)) {
      return false;
   }

   const _size length = oldPath.size() + 1;

   HANDLE hFind;
   _fdata FindFileData;

   _char DirPath[MAX_PATH];
   _char FileName[MAX_PATH];

   wcscpy(DirPath, const_cast<_char*>(oldPath.c_str()));
   wcscat(DirPath, OS_SEPARATOR_ASTERISK.c_str());
   wcscpy(FileName, const_cast<_char*>(oldPath.c_str()));
   wcscat(FileName, &OS_SEPARATOR);

   hFind = FindFirstFile(DirPath,&FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) return false;
      wcscpy(DirPath,FileName);

   _bool bSearch = true;
   while (bSearch) {
      if (FindNextFile(hFind,&FindFileData)) {
         if (!p2.state == State::s_Running) {
            FindClose(hFind);
            return false;
         }

         if (!os_isBrowsePath(FindFileData.cFileName)) {
            continue;
         }

         wcscat(FileName, FindFileData.cFileName);
         const _str np = str(newPath, OS_SEPARATOR_STRING, _str(FileName).substr(length));

         if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!os_copyToDirectory(FileName, np, p2)) {
               FindClose(hFind);
               return false;
            }

            wcscpy(FileName, DirPath);
         }
         else {
            if (!os_copyToFile(FileName, np)) {
               FindClose(hFind);
               return false;
            }

            wcscpy(FileName, DirPath);
         }
      }
      else {
         if (GetLastError() == ERROR_NO_MORE_FILES)
            bSearch = false;
         else {
            FindClose(hFind);
            return false;
         }
      }
   }

   FindClose(hFind);

   return true;
}

_bool os_copy(const _set& paths)
{
   _size totalSize = sizeof(DROPFILES) + sizeof(_char);

   for (const auto& p : paths) {
      totalSize += sizeof(_char) * (p.size() + 1);
   }

   HDROP hdrop   = static_cast<HDROP>(GlobalAlloc(GHND, totalSize));
   DROPFILES* df = static_cast<DROPFILES*>(GlobalLock(hdrop));
   df->pFiles    = sizeof(DROPFILES);
   df->fWide     = true;

   _char* dstStart = (_char*)(&df[1]);

   for (const auto& p : paths) {
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

_bool os_select(const _str& parent, const _set& paths)
{
   ITEMIDLIST* folder = ILCreateFromPathW(parent.c_str());
   std::vector<ITEMIDLIST*> v;

   for (const auto& p : paths) {
      v.emplace_back(ILCreateFromPathW(p.c_str()));
   }

   HRESULT hr = SHOpenFolderAndSelectItems(folder, v.size(), (LPCITEMIDLIST*)v.data(), 0);

   for (auto idl : v) {
      ILFree(idl);
   }

   ILFree(folder);
   return hr == S_OK;
}

_bool os_run(const _str& comm, const _str& location, _p2& p2)
{
   p2.sideProcess.running = true;
   STARTUPINFO si;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&p2.sideProcess.info, sizeof(p2.sideProcess.info));

   const _size len = comm.size() + 1;
   _char cmd[len];
   wcscpy(cmd, comm.c_str());

   const _size lenloc = location.size() + 1;
   _char loc[lenloc];
   wcscpy(loc, location.c_str());

   CreateProcessW
   (
      NULL,
      cmd,
      NULL,NULL,FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL,
      loc,
      &si, &p2.sideProcess.info
   );

   WaitForSingleObject(p2.sideProcess.info.hProcess, INFINITE);
   DWORD dwExitCode = 0;
   ::GetExitCodeProcess(p2.sideProcess.info.hProcess, &dwExitCode);

   p2.sideProcess.running = false;
   return p2.state == State::s_Running && dwExitCode == 0;
}

_bool os_isInvaild(const _str& path)
{
   const _size length = path.size();

   if (length == 0 || path[length - 1] == CHAR_DOT) {
      return true;
   }

   for (_size i = 0; i < length; i++) {
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

_uint32 os_patternInfo(const _str& pattern)
{
   _uint32 result = parse::ASTERISK_INFO_NULL;
   const _size length = pattern.size();

   if (length == 0 || pattern[length - 1] == CHAR_DOT) {
      return result;
   }

   if (os_isAbsolute(pattern)) {
      result |= parse::ASTERISK_INFO_IS_ABSOLUTE;
      if (length < 4 || pattern[2] != OS_SEPARATOR) {
         return result;
      }
   }

   _char prev;
   _size countAstrisks = 0;

   for (_size i = 0; i < length; i++) {
      const _char& ch = pattern[i];

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

_bool os_bothAreSeparators(const _char left, const _char right)
{
   return (left == right)
      && (left == OS_SEPARATOR);
}

_str os_trim(const _str& path)
{
   const _int len = path.size();
   _int start = 0;

   if (len >= 2 && path[0] == CHAR_DOT
    && (path[1] == OS_SEPARATOR || path[1] == OS_WRONG_SEPARATOR))
   {
      start = 2;
   }

   while (start < len) {
      switch (path[start]) {
         case OS_WRONG_SEPARATOR:
         case OS_SEPARATOR:
         case CHAR_SPACE: {
            break;
         }
         default: {
            goto exitStart;
         }
      }
      start++;
   }

exitStart:

   if (len >= (start + 2) && path[start] == CHAR_DOT
    && (path[start+ 1] == OS_SEPARATOR || path[start + 1] == OS_WRONG_SEPARATOR))
   {
      start += 2;
   }

   switch (len - start) {
      case 0: {
         return EMPTY_STRING;
      }
      case 1: {
         _str result = path.substr(len - 1, 1);
         os_escapeQuote(result);
         return result;
      }
   }

   _int end = len - 1;

   while (end >= 0) {
      switch (path[end]) {
         case OS_WRONG_SEPARATOR:
         case OS_SEPARATOR:
         case CHAR_SPACE: {
            break;
         }
         default: {
            goto exitEnd;
         }
      }
      end--;
   }

exitEnd:

   _str result;

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

void os_rawTrim(_str& value)
{
   const _int len = value.size();
   _int start = 0;

   if (len >= 2 && value[0] == CHAR_DOT
    && (value[1] == OS_SEPARATOR || value[1] == OS_WRONG_SEPARATOR))
   {
      start = 2;
   }

   while (start < len) {
      switch (value[start]) {
         case OS_WRONG_SEPARATOR:
         case OS_SEPARATOR:
         case CHAR_SPACE: {
            break;
         }
         default: {
            goto r_exitStart;
         }
      }
      start++;
   }

r_exitStart:

   if (len >= (start + 2) && value[start] == CHAR_DOT
    && (value[start+ 1] == OS_SEPARATOR || value[start + 1] == OS_WRONG_SEPARATOR))
   {
      start += 2;
   }

   switch (len - start) {
      case 0: {
         value = EMPTY_STRING;
         return;
      }
      case 1: {
         value = value.substr(len - 1, 1);
         return;
      }
   }

   _str result;
   _int end = len - 1;

   while (end >= 0) {
      switch (value[end]) {
         case OS_WRONG_SEPARATOR:
         case OS_SEPARATOR:
         case CHAR_SPACE: {
            break;
         }
         default: {
            goto r_exitEnd;
         }
      }
      end--;
   }

r_exitEnd:

   if (start == 0) {
      if (end != len - 1) {
         value = value.substr(0, end + 1);
      }
   }
   else {
      if (end == len - 1) {
         value = value.substr(start);
      }
      else {
         value = value.substr(start, end - start + 1);
      }
   }

   return;
}

inline void os_escapeQuote(_str& path)
{
   const _size length = path.size();
   if (path[0] == CHAR_QUOTATION_MARK && path[length - 1] == CHAR_QUOTATION_MARK) {
      path = path.substr(1, length - 2);
   }
}

_str os_join(const _str& path1, const _str& path2)
{
   return os_isAbsolute(path2)
      ? path2
      : str(path1, OS_SEPARATOR_STRING, path2);
}

_bool os_isAbsolute(const _str& path)
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
         if (path[2] != OS_SEPARATOR) {
            return false;
         }
         break;
      }
   }

   return path[1] == CHAR_COLON && os_isDriveLetter(path[0]);
}

_bool os_hasExtension(const _str& value)
{
   const _int len = value.size();

   for (_int i = len - 1; i >= 0; i--) {
      const _char& ch = value[i];

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

_bool os_isDriveLetter(const _char ch)
{
   return (ch >= CHAR_a && ch <= CHAR_z)
       || (ch >= CHAR_A && ch <= CHAR_Z);
}

_bool os_isPath(const _str& value)
{
   return value.find(OS_SEPARATOR) != _str::npos;
}

_bool os_isExplorableDirectory(const _str& name)
{
   // this is an equivalent to
   // return name != .git && name != .svn
   if (name.size() == 4) {
      return name != OS_GIT_DIRECTORY
          && name != OS_SVN_DIRECTORY;
   }
   else {
      return true;
   }
}

_bool os_isAncestor(const _str& path, const _str& supposedChildPath)
{
   if (supposedChildPath.size() <= path.size()) {
      return false;
   }

   for (_size i = 0; i < path.size(); i++) {
      if (!charsEqualInsensitive(&path[i], &supposedChildPath[i])) {
         return false;
      }
   }

   return supposedChildPath[path.size() + 1] == OS_SEPARATOR;
}

_bool os_hasParentDirectory(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   for (_int i = path.size() - 1; i >= 1; i--) {
      const _char& ch = path[i];
      if (ch == OS_SEPARATOR) {
         return true;
      }
   }

   return false;
}

// stack path to an element without extension
_str os_stackPath(const _str& path)
{
   if (os_pathWasStacked(path)) {
      return os_stackPathStacked(path);
   }

   _nint index = NINT_TWO;
   _str newPath = path;

   while (os_exists(newPath))
   {
      newPath = str(path, STRING_CHAR_OPENING_ROUND_BRACKET, toStr(index), STRING_CHAR_CLOSING_ROUND_BRACKET);
      index++;
   }

   return newPath;
}

_str os_stackPathExt(const _str& basePath, const _str& extension)
{
   if (os_pathWasStacked(basePath)) {
      return os_stackPathExtStacked(basePath, extension);
   }

   _nint index = NINT_TWO;
   _str newPath = str(basePath, STRING_CHAR_DOT, extension);

   while (os_exists(newPath))
   {
      newPath = str(basePath, STRING_CHAR_OPENING_ROUND_BRACKET, toStr(index),
         STRING_CHAR_CLOSING_ROUND_BRACKET, STRING_CHAR_DOT, extension);
      index++;
   }

   return newPath;
}

_str os_stackPathBase(const _str& path)
{
   for (_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == CHAR_DOT) {
         return path.substr(0, i);
      }
   }

   return path;
}

_str os_stackPathStacked(const _str& path)
{
   _nint index;
   _str basePath;
   os_getStackedData(path, index, basePath);

   _str newPath = str(basePath, STRING_CHAR_OPENING_ROUND_BRACKET,
      toStr(index), STRING_CHAR_CLOSING_ROUND_BRACKET);

   while (os_exists(newPath))
   {
      index++;
      newPath = str(basePath, STRING_CHAR_OPENING_ROUND_BRACKET,
         toStr(index), STRING_CHAR_CLOSING_ROUND_BRACKET);
   }

   return newPath;
}

_str os_stackPathExtStacked(const _str& path, const _str& extension)
{
   _nint index;
   _str basePath;
   os_getStackedData(path, index, basePath);

   _str newPath = str(basePath, STRING_CHAR_OPENING_ROUND_BRACKET,
      toStr(index), STRING_CHAR_CLOSING_ROUND_BRACKET, STRING_CHAR_DOT, extension);

   while (os_exists(newPath))
   {
      index++;
      newPath = str(basePath, STRING_CHAR_OPENING_ROUND_BRACKET,
         toStr(index), STRING_CHAR_CLOSING_ROUND_BRACKET, STRING_CHAR_DOT, extension);
   }

   return newPath;
}

_bool os_pathWasStacked(const _str& basePath)
{
   const _size len = basePath.size();
   if (len < 4 || basePath[len - 1] != CHAR_CLOSING_ROUND_BRACKET) {
      return false;
   }

   for (_int i = len - 2; i >= 0; i--) {
      if (basePath[i] == CHAR_OPENING_ROUND_BRACKET) {
         if (i == (len - 2) || i == 0) {
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
      else if (!std::iswdigit(basePath[i])) {
         return false;
      }
   }

   return false;
}

void os_getStackedData(const _str& path, _nint& index, _str& basePath)
{
   const _size len = path.size();

   for (_int i = len - 2; i >= 0; i--) {
      if (!std::iswdigit(path[i])) {
         basePath = path.substr(0, i);
         const _str numStr = path.substr(i + 1, len - i - 2);

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

_str os_executablePath()
{
   _char path[MAX_PATH];
   GetModuleFileNameW(NULL, path, MAX_PATH);
   return _str(path);
}

_str os_desktopPath()
{
   _char path[MAX_PATH];
   return SHGetSpecialFolderPathW(0, path, CSIDL_DESKTOP, FALSE)
      ? _str(path)
      : EMPTY_STRING;
}

_str os_currentPath()
{
   _char path[MAX_PATH];
   GetCurrentDirectory(MAX_PATH, path);
   return _str(path);
}

_size os_readFile_size(const _str& path)
{
    struct _stat fileinfo;
    _wstat(path.c_str(), &fileinfo);
    return fileinfo.st_size;
}

_bool os_readFile(_str& result, const _str& path)
{
   FILE* f = _wfopen(path.c_str(), STRING_FILE_OPEN_MODE.c_str());

   if (f == NULL) {
      return false;
   }

   _size filesize = os_readFile_size(path);

   if (filesize > 0) {
      result.resize(filesize);
      _size wchars_read = fread(&(result.front()), sizeof(_char), filesize, f);
      result.resize(wchars_read);
      result.shrink_to_fit();
   }

   fclose(f);
   return true;
}

void os_showWebsite(const _str& url)
{
   ShellExecuteW(NULL, STRING_OPEN.c_str(), url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

_bool os_findText(const _str& path, const _str& value)
{
   std::wifstream stream(path.c_str());
   if (!stream) {
      return false;
   }
   else if (value.empty()) {
      stream.close();
      return true;
   }

   _str line;
   _bool result = false;

   while (std::getline(stream, line)) {
      if (line.find(value) != _str::npos) {
         result = true;
         break;
      }
   }

   stream.close();
   return result;
}

inline _uint64 os_bigInteger(const _uint32 low, const _uint32 high)
{
   _uint64 n = high;
   n <<= sizeof(high) * 8;
   n |= low;
   return n;
}

inline _bool os_isBrowsePath(const _str& path)
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

inline _tim os_convertToPerun2Time(const _ftim* time)
{
   _FILETIME ftime;
   if (FileTimeToLocalFileTime(time, &ftime)) {
      _SYSTEMTIME stime;

      if (FileTimeToSystemTime(&ftime, &stime)) {
         return _tim(stime.wDay, stime.wMonth, stime.wYear,
            stime.wHour, stime.wMinute, stime.wSecond);
      }
      else {
         return _tim();
      }
   }
   else {
      return _tim();
   }
}

inline _bool os_convertToFileTime(const _tim& perunTime, _ftim& result)
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

_str os_makeArg(const _str& value)
{
   const _size len = value.size();
   _bool anySpace = false;
   _size quotes = 0;

   for (_size i = 0; i < len; i++) {
      switch (value[i]) {
         case CHAR_SPACE: {
            anySpace = true;
            break;
         }
         case CHAR_QUOTATION_MARK: {
            quotes++;
            break;
         }
      }
   }

   if (quotes == 0) {
      return anySpace
         ? str(STRING_CHAR_QUOTATION_MARK, value, STRING_CHAR_QUOTATION_MARK)
         : value;
   }
   else {
      _str result;
      result.reserve(len + quotes);

      for (_size i = 0; i < len; i++) {
         const _char& ch = value[i];
         if (ch == CHAR_QUOTATION_MARK) {
            result.push_back(CHAR_BACKSLASH);
         }
         result.push_back(ch);
      }

      return anySpace
         ? str(STRING_CHAR_QUOTATION_MARK, result, STRING_CHAR_QUOTATION_MARK)
         : result;
   }
}

_str os_quoteEmbraced(const _str& value)
{
   return value.find(CHAR_SPACE) == _str::npos
      ? value
      : str(STRING_CHAR_QUOTATION_MARK, value, STRING_CHAR_QUOTATION_MARK);
}

}
