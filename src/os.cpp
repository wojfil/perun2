/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "os.h"
#include <time.h>
#include "uroboros.h"
#include <shlobj.h>
#include <cwctype>
#include <shellapi.h>
#include <shlwapi.h>
#include <algorithm>
#include <fstream>


namespace uro
{

inline constexpr _nint OS_SLEEP_UNIT = 300LL;
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

void os_sleepForMs(const _nint& ms, Uroboros& uro)
{
   if (ms <= 0LL) {
      return;
   }

   _nint loops = ms / OS_SLEEP_UNIT;
   const _nint remainder = ms % OS_SLEEP_UNIT;

   Sleep(remainder);

   while (uro.state == State::s_Running && loops != 0LL) {
      Sleep(OS_SLEEP_UNIT);
      loops--;
   }
}

// explanation of attributes is in file 'attribute.h'
void os_loadAttributes(const Attribute* attr, Uroboros& uro)
{
   InnerVariables& inner = uro.vars.inner;
   inner.trimmed = os_trim(inner.this_s.value);

   if (os_isInvaild(inner.trimmed)) {
      os_loadEmptyAttributes(attr, inner);
      return;
   }

   _str path;

   // "drive", "path", "parent" and "fullname" do not require access to the file system
   if (attr->has(ATTR_PATH)) {
      path = os_join(inner.location.value, inner.trimmed);
      inner.path.value = path;
   }

   if (attr->has(ATTR_FULLNAME)) {
      inner.fullname.value = os_fullname(inner.trimmed);
   }

   if (attr->has(ATTR_PARENT)) {
      inner.parent.value = os_parent(path);
   }

   if (attr->has(ATTR_DRIVE)) {
      inner.drive.value = os_drive(path);
   }

   if (!attr->has(ATTR_EXISTS)) {
      return;
   }

   // below are "real" attributes of files and directories
   _adata data;
   const _bool gotAttrs = GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data);
   const DWORD& dwAttrib = data.dwFileAttributes;
   const _bool exists = gotAttrs && dwAttrib != INVALID_FILE_ATTRIBUTES;
   inner.exists.value = exists;

   _bool isDir;
   _bool isFile;

   if (exists) {
      isDir = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
      isFile = !isDir;
   }
   else {
      isFile = os_hasExtension(inner.trimmed);
      isDir = !isFile;
   }

   inner.isfile.value = isFile;
   inner.isdirectory.value = isDir;

   if (attr->has(ATTR_ACCESS)) {
      inner.access.value = exists
         ? os_convertToUroTime(&data.ftLastAccessTime)
         : _tim();
   }

   if (attr->has(ATTR_ARCHIVE)) {
      inner.archive.value = exists ? (dwAttrib & FILE_ATTRIBUTE_ARCHIVE) : false;
   }

   if (attr->has(ATTR_COMPRESSED)) {
      inner.compressed.value = exists ? (dwAttrib & FILE_ATTRIBUTE_COMPRESSED) : false;
   }

   if (attr->has(ATTR_CREATION)) {
      inner.creation.value = exists
         ? os_convertToUroTime(&data.ftCreationTime)
         : _tim();
   }

   const _bool hasMod = attr->has(ATTR_MODIFICATION);
   const _bool hasChange = attr->has(ATTR_CHANGE);
   if (hasMod || hasChange) {
      const _tim time = exists
         ? os_convertToUroTime(&data.ftLastWriteTime)
         : _tim();

      if (hasChange) {
         inner.change.value = time;
      }
      if (hasMod) {
         inner.modification.value = time;
      }
   }

   if (attr->has(ATTR_LIFETIME)) {
      if (exists) {
         inner.lifetime.value = inner.creation.value < inner.modification.value
            ? (os_now() - inner.creation.value)
            : (os_now() - inner.modification.value);
      }
      else {
         inner.lifetime.value = _per();
      }
   }

   if (attr->has(ATTR_EMPTY)) {
      if (exists) {
         inner.empty.value = isFile
            ? os_emptyFile(data)
            : os_emptyDirectory(path);
      }
      else {
         inner.empty.value = false;
      }
   }

   if (attr->has(ATTR_ENCRYPTED)) {
      inner.encrypted.value = exists ? (dwAttrib & FILE_ATTRIBUTE_ENCRYPTED) : false;
   }

   if (attr->has(ATTR_EXTENSION)) {
      inner.extension.value = isFile ? os_extension(inner.trimmed) : EMPTY_STRING;
   }

   if (attr->has(ATTR_HIDDEN)) {
      inner.hidden.value = exists ? (dwAttrib & FILE_ATTRIBUTE_HIDDEN) : false;
   }

   if (attr->has(ATTR_NAME)) {
      if (isDir) {
         inner.name.value = os_fullname(inner.trimmed);
      }
      else {
         inner.name.value = os_hasExtension(inner.trimmed)
            ? os_name(inner.trimmed)
            : os_fullname(inner.trimmed);
      }
   }

   if (attr->has(ATTR_READONLY)) {
      inner.readonly.value = exists ? (dwAttrib & FILE_ATTRIBUTE_READONLY) : false;
   }

   if (attr->has(ATTR_SIZE)) {
      if (exists) {
         inner.size.value = isFile
            ? _num(os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh))
            : _num(os_sizeDirectory(path, uro));
      }
      else {
         inner.size.value = _num(-1LL);
      }
   }
}

// attributes of something, that cannot exist (empty string, contains not allowed chars, ...)
void os_loadEmptyAttributes(const Attribute* attr, InnerVariables& inner)
{
   if (attr->has(ATTR_PATH)) {
      inner.path.value = EMPTY_STRING;
   }

   if (attr->has(ATTR_FULLNAME)) {
      inner.fullname.value = EMPTY_STRING;
   }

   if (attr->has(ATTR_DRIVE)) {
      inner.drive.value = EMPTY_STRING;
   }

   if (!attr->has(ATTR_EXISTS)) {
      return;
   }

   inner.exists.value = false;
   inner.isfile.value = false;
   inner.isdirectory.value = false;

   if (attr->has(ATTR_ACCESS)) {
      inner.access.value = _tim();
   }

   if (attr->has(ATTR_ARCHIVE)) {
      inner.archive.value = false;
   }

   if (attr->has(ATTR_COMPRESSED)) {
      inner.compressed.value = false;
   }

   if (attr->has(ATTR_CHANGE)) {
      inner.change.value = _tim();
   }

   if (attr->has(ATTR_CREATION)) {
      inner.creation.value = _tim();
   }

   if (attr->has(ATTR_EMPTY)) {
      inner.empty.value = false;
   }

   if (attr->has(ATTR_ENCRYPTED)) {
      inner.encrypted.value = false;
   }

   if (attr->has(ATTR_EXTENSION)) {
      inner.extension.value = EMPTY_STRING;
   }

   if (attr->has(ATTR_HIDDEN)) {
      inner.hidden.value = false;
   }

   if (attr->has(ATTR_LIFETIME)) {
      inner.lifetime.value = _per();
   }

   if (attr->has(ATTR_MODIFICATION)) {
      inner.modification.value = _tim();
   }

   if (attr->has(ATTR_NAME)) {
      inner.name.value = EMPTY_STRING;
   }

   if (attr->has(ATTR_PARENT)) {
      inner.parent.value = EMPTY_STRING;
   }

   if (attr->has(ATTR_READONLY)) {
      inner.readonly.value = false;
   }

   if (attr->has(ATTR_SIZE)) {
      inner.size.value = _num(-1LL);
   }
}

void os_loadDataAttributes(const Attribute* attr, Uroboros& uro, _fdata* data)
{
   const DWORD& dwAttrib = data->dwFileAttributes;
   InnerVariables& inner = uro.vars.inner;
   inner.trimmed = inner.this_s.value;
   inner.path.value = os_join(inner.location.value, inner.trimmed);

   const _str& path = inner.path.value;

   if (attr->has(ATTR_FULLNAME)) {
      inner.fullname.value = os_fullname(inner.trimmed);
   }

   if (attr->has(ATTR_PARENT)) {
      inner.parent.value = os_parent(path);
   }

   if (attr->has(ATTR_DRIVE)) {
      inner.drive.value = os_drive(path);
   }

   inner.exists.value = true;
   inner.isdirectory.value = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
   inner.isfile.value = !inner.isdirectory.value;

   if (attr->has(ATTR_ACCESS)) {
      inner.access.value = os_convertToUroTime(&data->ftLastAccessTime);
   }

   if (attr->has(ATTR_ARCHIVE)) {
      inner.archive.value = dwAttrib & FILE_ATTRIBUTE_ARCHIVE;
   }

   if (attr->has(ATTR_COMPRESSED)) {
      inner.compressed.value = dwAttrib & FILE_ATTRIBUTE_COMPRESSED;
   }

   if (attr->has(ATTR_CREATION)) {
      inner.creation.value = os_convertToUroTime(&data->ftCreationTime);
   }

   const _bool hasMod = attr->has(ATTR_MODIFICATION);
   const _bool hasChange = attr->has(ATTR_CHANGE);
   if (hasMod || hasChange) {
      const _tim time = os_convertToUroTime(&data->ftLastWriteTime);

      if (hasChange) {
         inner.change.value = time;
      }
      if (hasMod) {
         inner.modification.value = time;
      }
   }

   if (attr->has(ATTR_LIFETIME)) {
      inner.lifetime.value = inner.creation.value < inner.modification.value
         ? (os_now() - inner.creation.value)
         : (os_now() - inner.modification.value);
   }

   if (attr->has(ATTR_EMPTY)) {
      inner.empty.value = inner.isfile.value
         ? (data->nFileSizeLow == 0 && data->nFileSizeHigh == 0)
         : os_emptyDirectory(path);
   }

   if (attr->has(ATTR_ENCRYPTED)) {
      inner.encrypted.value = dwAttrib & FILE_ATTRIBUTE_ENCRYPTED;
   }

   if (attr->has(ATTR_EXTENSION)) {
      inner.extension.value = inner.isfile.value ? os_extension(inner.trimmed) : EMPTY_STRING;
   }

   if (attr->has(ATTR_HIDDEN)) {
      inner.hidden.value = dwAttrib & FILE_ATTRIBUTE_HIDDEN;
   }

   if (attr->has(ATTR_NAME)) {
      if (inner.isfile.value) {
         inner.name.value = os_hasExtension(inner.trimmed)
            ? os_name(inner.trimmed)
            : os_fullname(inner.trimmed);
      }
      else {
         inner.name.value = os_fullname(inner.trimmed);
      }
   }

   if (attr->has(ATTR_READONLY)) {
      inner.readonly.value = dwAttrib & FILE_ATTRIBUTE_READONLY;
   }

   if (attr->has(ATTR_SIZE)) {
      inner.size.value = inner.isfile.value
         ? _num(os_bigInteger(data->nFileSizeLow, data->nFileSizeHigh))
         : _num(os_sizeDirectory(path, uro));
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

   return os_convertToUroTime(&data.ftLastAccessTime);
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

   return os_convertToUroTime(&data.ftLastWriteTime);
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

   return os_convertToUroTime(&data.ftCreationTime);
}

_str os_drive(const _str& path)
{
   return os_isAbsolute(path)
      ? str(toStr(std::towupper(path[0])), L":")
      : EMPTY_STRING;
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
      else {
         FindClose(handle);
         return true;
      }
   }
}

_str os_extension(const _str& value)
{
   const _int len = value.length();

   for (_int i = len - 1; i >= 0; i--) {
      const _char& ch = value[i];
      if (ch == L'.') {
         if (i == 0 || value[i - 1] == OS_SEPARATOR) {
            return EMPTY_STRING;
         }
         else {
            _str result = value.substr(i + 1);
            toLower(result);
            return result;
         }
      }
      else if (ch == OS_SEPARATOR) {
         return EMPTY_STRING;
      }
   }

   return EMPTY_STRING;
}

_str os_fullname(const _str& value)
{
   const _int len = value.length();

   for (_int i = len - 1; i >= 0; i--) {
      const _char& ch = value[i];
      if (ch == OS_SEPARATOR) {
         return value.substr(i + 1);
      }
   }

   return value;
}

_bool os_hasAttribute(const _str& path, const ULONG& attribute)
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

   const _tim modification = os_convertToUroTime(&data.ftLastWriteTime);
   const _tim creation = os_convertToUroTime(&data.ftCreationTime);

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

   return os_convertToUroTime(&data.ftLastWriteTime);
}

_str os_name(const _str& value)
{
   const _int len = value.length();
   _int i = len - 1;
   _int dot = -1;
   _bool sep = false;

   for (; i >= 0; i--) {
      const _char& ch = value[i];
      if (dot == -1 && ch == L'.') {
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
   else {
      return dot == -1
         ? value
         : value.substr(0, dot);
   }
}

_str os_parent(const _str& path)
{
   const _int len = path.size();

   for (_int i = len - 1; i >= 0; i--) {
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

_nint os_size(const _str& path, Uroboros& uro)
{
   _adata data;
   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return -1LL;
   }

   const DWORD& dwAttrib = data.dwFileAttributes;
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return -1LL;
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY
      ? os_sizeDirectory(path, uro)
      : os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh);
}

_nint os_sizeDirectory(const _str& path, Uroboros& uro)
{
   _nint totalSize = 0LL;
   _fdata data;
   HANDLE handle = FindFirstFile((str(path, OS_SEPARATOR_ASTERISK)).c_str(), &data);

   if (handle == INVALID_HANDLE_VALUE) {
      return totalSize;
   }

   do {
      if (!uro.state == State::s_Running) {
         FindClose(handle);
         return -1LL;
      }
      if (!os_isBrowsePath(data.cFileName)) {
         if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            totalSize += os_sizeDirectory(str(path, OS_SEPARATOR_STRING, data.cFileName), uro);
         }
         else {
            totalSize += os_bigInteger(data.nFileSizeLow, data.nFileSizeHigh);
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

   return GetFileAttributesW(path.c_str())
      != INVALID_FILE_ATTRIBUTES;
}

_bool os_fileExists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
          (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

_bool os_directoryExists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
          (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
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

_bool os_drop(const _str& path, Uroboros& uro)
{
   return os_isFile(path)
      ? os_dropFile(path)
      : os_dropDirectory(path, uro);
}

_bool os_drop(const _str& path, const _bool& isFile, Uroboros& uro)
{
   return isFile
      ? os_dropFile(path)
      : os_dropDirectory(path, uro);
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

_bool os_dropDirectory(const _str& path, Uroboros& uro)
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

   bool bSearch = true;
   while (bSearch) {
      if (FindNextFile(hFind,&FindFileData)) {
         if (!uro.state == State::s_Running) {
            FindClose(hFind);
            return false;
         }

         if (os_isBrowsePath(FindFileData.cFileName)) {
            continue;
         }

         wcscat(FileName,FindFileData.cFileName);
         if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!os_dropDirectory(FileName, uro)) {
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
   return (INT_PTR)ShellExecuteW(NULL, L"open", program.c_str(), path.c_str(), location.c_str(), SW_SHOW) > 32;
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

   /*HANDLE handle = CreateFileW(path.c_str(),
      FILE_WRITE_ATTRIBUTES,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL, OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS, NULL);*/


   /*HANDLE handle = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL,
      CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);*/

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

_bool os_copyTo(const _str& oldPath, const _str& newPath, const _bool& isFile, Uroboros& uro)
{
   if (isFile) {
      return os_copyToFile(oldPath, newPath);
   }
   else {
      const _bool success = os_copyToDirectory(oldPath, newPath, uro);
      if (!success && !uro.state == State::s_Running && os_directoryExists(newPath)) {
         // if directory copy operation
         // was stopped by the user
         // delete recent partially copied directory if it is there
         os_dropDirectory(newPath, uro);
      }

      return success;
   }
}

_bool os_copyToFile(const _str& oldPath, const _str& newPath)
{
   return CopyFileW(oldPath.c_str(), newPath.c_str(), true) != 0;
}

_bool os_copyToDirectory(const _str& oldPath, const _str& newPath, Uroboros& uro)
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

   bool bSearch = true;
   while (bSearch) {
      if (FindNextFile(hFind,&FindFileData)) {
         if (!uro.state == State::s_Running) {
            FindClose(hFind);
            return false;
         }

         if (!os_isBrowsePath(FindFileData.cFileName)) {
            continue;
         }

         wcscat(FileName, FindFileData.cFileName);
         const _str np = str(newPath, OS_SEPARATOR_STRING, _str(FileName).substr(length));

         if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!os_copyToDirectory(FileName, np, uro)) {
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

_bool os_copy(const std::unordered_set<_str>& paths)
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

_bool os_select(const _str& parent, const std::unordered_set<_str>& paths)
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

_bool os_run(const _str& comm, Uroboros& uro)
{
   uro.sideProcess.running = true;
   STARTUPINFO si;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&uro.sideProcess.info, sizeof(uro.sideProcess.info));

   const _size len = comm.size() + 1;
   _char cmd[len];
   wcscpy(cmd, comm.c_str());

   const _size lenloc = uro.vars.inner.location.value.size() + 1;
   _char loc[lenloc];
   wcscpy(loc, uro.vars.inner.location.value.c_str());

   CreateProcessW
   (
      NULL,
      cmd,
      NULL,NULL,FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL,
      loc,
      &si, &uro.sideProcess.info
   );

   WaitForSingleObject(uro.sideProcess.info.hProcess, INFINITE);
   DWORD dwExitCode = 0;
   ::GetExitCodeProcess(uro.sideProcess.info.hProcess, &dwExitCode);

   uro.sideProcess.running = false;
   return uro.state == State::s_Running && dwExitCode == 0;
}

_bool os_process(const _str& command, const _str& location)
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

_bool os_isInvaild(const _str& path)
{
   const _size length = path.size();

   if (length == 0 || path[length - 1] == L'.') {
      return true;
   }

   for (_size i = 0; i < length; i++) {
      switch (path[i]) {
         case L'<':
         case L'>':
         case L'|':
         case L'?':
         case L'*':
         case L'"': {
            return true;
         }
         case L':': {
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
   _uint32 result = gen::PATTERN_INFO_NULL;
   const _size length = pattern.size();

   if (length == 0 || pattern[length - 1] == L'.') {
      return result;
   }

   if (os_isAbsolute(pattern)) {
      result |= gen::PATTERN_INFO_IS_ABSOLUTE;
      if (length < 4 || pattern[2] != OS_SEPARATOR) {
         return result;
      }
   }

   _char prev;
   _size countAstrisks = 0;

   for (_size i = 0; i < length; i++) {
      const _char& ch = pattern[i];

      switch (ch) {
         case L'*': {
            countAstrisks++;

            if (prev == L'*') {
               result |= gen::PATTERN_INFO_DOUBLE_ASTERISK;
            }
            break;
         }
         case L'<':
         case L'>':
         case L'|':
         case L'?':
         case L'"': {
            return result;
         }
         case L':': {
            if (i != 1 || !(result & gen::PATTERN_INFO_IS_ABSOLUTE)) {
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
      result |= gen::PATTERN_INFO_ONE_ASTERISK;
   }

   result |= gen::PATTERN_INFO_VALID;
   return result;
}

_bool os_bothAreSeparators(const _char& left, const _char& right)
{ 
   return (left == right) 
      && (left == OS_SEPARATOR); 
}

_str os_trim(const _str& path)
{
   const _int len = path.length();
   _int start = 0;

   while (start < len) {
      switch (path[start]) {
         case OS_WRONG_SEPARATOR:
         case OS_SEPARATOR:
         case L' ': {
            break;
         }
         default: {
            goto exitStart;
         }
      }
      start++;
   }

exitStart:

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
         case L' ': {
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

inline void os_escapeQuote(_str& path)
{
   const _size length = path.size();
   if (path[0] == L'"' && path[length - 1] == L'"') {
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

   return path[1] == L':' && os_isDriveLetter(path[0]);
}

_bool os_hasExtension(const _str& value)
{
   const _int len = value.length();

   for (_int i = len - 1; i >= 0; i--) {
      const _char& ch = value[i];

      if (ch == L'.') {
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

inline _bool os_isDriveLetter(const _char& ch)
{
   return (ch >= L'a' && ch <= L'z') 
       || (ch >= L'A' && ch <= L'Z');
}

_bool os_isPath(const _str value)
{
   return value.find(OS_SEPARATOR) != _str::npos;
}

_bool os_hasParentDirectory(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   const _int len = path.size();

   for (_int i = len - 1; i >= 1; i--) {
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

   _nint index = 2LL;
   _str newPath = path;

   while (os_exists(newPath))
   {
      newPath = str(path, L"(", toStr(index),  L")");
      index++;
   }

   return newPath;
}

_str os_stackPathExt(const _str& basePath, const _str& extension)
{
   if (os_pathWasStacked(basePath)) {
      return os_stackPathExtStacked(basePath, extension);
   }

   _nint index = 2LL;
   _str newPath = str(basePath, L".", extension);

   while (os_exists(newPath))
   {
      newPath = str(basePath, L"(", toStr(index), L").", extension);
      index++;
   }

   return newPath;
}

_str os_stackPathBase(const _str& path)
{
   const _int len = path.length();

   for (_int i = len - 1; i >= 0; i--) {
      if (path[i] == L'.') {
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

   _str newPath = str(basePath, L"(", toStr(index), L")");

   while (os_exists(newPath))
   {
      index++;
      newPath = str(basePath, L"(", toStr(index), L")");
   }

   return newPath;
}

_str os_stackPathExtStacked(const _str& path, const _str& extension)
{
   _nint index;
   _str basePath;
   os_getStackedData(path, index, basePath);

   _str newPath = str(basePath, L"(", toStr(index), L").", extension);

   while (os_exists(newPath))
   {
      index++;
      newPath = str(basePath, L"(", toStr(index), L").", extension);
   }

   return newPath;
}

_bool os_pathWasStacked(const _str& basePath)
{
   const _size len = basePath.length();
   if (len < 4 || basePath[len - 1] != L')') {
      return false;
   }

   for (_int i = len - 2; i >= 0; i--) {
      switch (basePath[i]) {
         case L'0': case L'1': case L'2': case L'3': case L'4':
         case L'5': case L'6': case L'7': case L'8': case L'9': {
            break;
         }
         case L'(': {
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
         default: {
            return false;
         }
      }
   }

   return false;
}

void os_getStackedData(const _str& path, _nint& index, _str& basePath)
{
   const _size len = path.length();

   for (_int i = len - 2; i >= 0; i--) {
      switch (path[i]) {
         case L'0': case L'1': case L'2': case L'3': case L'4':
         case L'5': case L'6': case L'7': case L'8': case L'9': {
            break;
         }
         default: {
            basePath = path.substr(0, i);
            const _str numStr = path.substr(i + 1, len - i - 2);

            try {
               index = std::stoll(numStr);
            }
            catch (...) {
               index = 2LL;
            }

            return;
         }
      }
   }
}

_str os_uroborosPath()
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
      : _str();
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

_str os_readFile(const _str& path, _bool& result)
{
   _str buffer;
   FILE* f = _wfopen(path.c_str(), L"rtS, ccs=UTF-8");

   if (f == NULL) {
      result = false;
      return buffer;
   }

   _size filesize = os_readFile_size(path);

   if (filesize > 0) {
      buffer.resize(filesize);
      _size wchars_read = fread(&(buffer.front()), sizeof(_char), filesize, f);
      buffer.resize(wchars_read);
      buffer.shrink_to_fit();
   }

   fclose(f);
   result = true;
   return buffer;
}

void os_showWebsite(const _str& url)
{
   ShellExecuteW(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

_bool os_find(const _str& path, const _str& value)
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

inline _nint os_bigInteger(const _uint32& low, const _uint32& high)
{
   _nint n = high;
   n <<= sizeof(high) * 8;
   n |= low;
   return n;
}

inline _bool os_isBrowsePath(const _str& path)
{
   // this is an equivalent to
   // return path == L"." || path == L"..";
   switch (path.size()) {
      case 1: {
         return path[0] == L'.';
      }
      case 2: {
         return path[0] == L'.' && path[1] == L'.';
      }
      default: {
         return false;
      }
   }
}

inline _tim os_convertToUroTime(const _ftim* time)
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

inline _bool os_convertToFileTime(const _tim& uroTime, _ftim& result)
{
   _SYSTEMTIME stime;
   stime.wYear = uroTime.year;
   stime.wMonth = uroTime.month;
   stime.wDay = uroTime.day;
   stime.wHour = uroTime.hour;
   stime.wMinute = uroTime.minute;
   stime.wSecond = uroTime.second;

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
         case L' ': {
            anySpace = true;
            break;
         }
         case L'"': {
            quotes++;
            break;
         }
      }
   }

   if (quotes == 0) {
      return anySpace
         ? str(L"\"", value, L"\"")
         : value;
   }
   else {
      _str result;
      result.reserve(len + quotes);

      for (_size i = 0; i < len; i++) {
         const _char& ch = value[i];
         if (ch == L'"') {
            result.push_back(L'\\');
         }
         result.push_back(ch);
      }

      return anySpace
         ? str(L"\"", result, L"\"")
         : result;
   }
}


void os_rawTrim(_str& value)
{
   const _int len = value.length();
   _int start = 0;

   while (start < len) {
      switch (value[start]) {
         case OS_WRONG_SEPARATOR:
         case OS_SEPARATOR:
         case L' ': {
            break;
         }
         default: {
            goto r_exitStart;
         }
      }
      start++;
   }

r_exitStart:

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
         case L' ': {
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

_str os_quoteEmbraced(const _str& value)
{
   return value.find(L' ') == _str::npos
      ? value
      : str(L"\"", value, L"\"");
}

}
