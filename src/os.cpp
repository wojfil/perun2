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
#include "var/var-runtime.h"
#include "uroboros.h"
#include <shlobj.h>
#include <cwctype>
#include <shellapi.h>
#include <shlwapi.h>
#include <algorithm>


static const _nint OS_SLEEP_UNIT = 300LL;
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

void os_sleepForMs(const _nint& ms, Uroboros* uro)
{
   if (ms <= 0) {
      return;
   }

   _nint loops = ms / OS_SLEEP_UNIT;
   const _nint remainder = ms % OS_SLEEP_UNIT;

   Sleep(remainder);

   while (uro->running && loops != 0LL) {
      Sleep(OS_SLEEP_UNIT);
      loops--;
   }
}

// explanation of attributes is in file 'attribute.h'
void os_loadAttributes(const Attribute* attr)
{
   attr->inner->trimmed = os_trim(attr->inner->this_s.value);

   if (os_isInvaild(attr->inner->trimmed)) {
      os_loadAttributes_empty(attr);
      return;
   }

   _str path;

   // "drive", "path", "parent" and "fullname" do not require access to the file system
   if (attr->has(ATTR_PATH)) {
      path = os_join(attr->inner->location.value, attr->inner->trimmed);
      attr->inner->path.value = path;
   }

   if (attr->has(ATTR_FULLNAME)) {
      attr->inner->fullname.value = os_fullname(attr->inner->trimmed);
   }

   if (attr->has(ATTR_PARENT)) {
      attr->inner->parent.value = os_parent(path);
   }

   if (attr->has(ATTR_DRIVE)) {
      attr->inner->drive.value = os_drive(path);
   }

   if (!attr->has(ATTR_EXISTS)) {
      return;
   }

   // below are "real" attributes of files and directories
   WIN32_FILE_ATTRIBUTE_DATA data;
   const _boo gotAttrs = GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data);
   const DWORD& dwAttrib = data.dwFileAttributes;
   const _boo exists = gotAttrs && dwAttrib != INVALID_FILE_ATTRIBUTES;
   attr->inner->exists.value = exists;

   _boo isDir;
   _boo isFile;

   if (exists) {
      isDir = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
      isFile = !isDir;
   }
   else {
      isFile = os_hasExtension(attr->inner->trimmed);
      isDir = !isFile;
   }

   attr->inner->isfile.value = isFile;
   attr->inner->isdirectory.value = isDir;


   if (attr->has(ATTR_ACCESS)) {
      attr->inner->access.value = exists
         ? convertToUroTime(&data.ftLastAccessTime)
         : _tim();
   }

   if (attr->has(ATTR_ARCHIVE)) {
      attr->inner->archive.value = exists ? (dwAttrib & FILE_ATTRIBUTE_ARCHIVE) : false;
   }

   if (attr->has(ATTR_COMPRESSED)) {
      attr->inner->compressed.value = exists ? (dwAttrib & FILE_ATTRIBUTE_COMPRESSED) : false;
   }

   if (attr->has(ATTR_CREATION)) {
      attr->inner->creation.value = exists
         ? convertToUroTime(&data.ftCreationTime)
         : _tim();
   }

   const _boo hasMod = attr->has(ATTR_MODIFICATION);
   const _boo hasChange = attr->has(ATTR_CHANGE);
   if (hasMod || hasChange) {
      const _tim time = exists
         ? convertToUroTime(&data.ftLastWriteTime)
         : _tim();

      if (hasChange) {
         attr->inner->change.value = time;
      }
      if (hasMod) {
         attr->inner->modification.value = time;
      }
   }

   if (attr->has(ATTR_LIFETIME)) {
      if (exists) {
         attr->inner->lifetime.value = attr->inner->creation.value < attr->inner->modification.value
            ? (os_now() - attr->inner->creation.value)
            : (os_now() - attr->inner->modification.value);
      }
      else {
         attr->inner->lifetime.value = _per();
      }
   }

   if (attr->has(ATTR_EMPTY)) {
      if (exists) {
         attr->inner->empty.value = isFile
            ? os_emptyFile(data)
            : os_emptyDirectory(path);
      }
      else {
         attr->inner->empty.value = false;
      }
   }

   if (attr->has(ATTR_ENCRYPTED)) {
      attr->inner->encrypted.value = exists ? (dwAttrib & FILE_ATTRIBUTE_ENCRYPTED) : false;
   }

   if (attr->has(ATTR_EXTENSION)) {
      attr->inner->extension.value = isFile ? os_extension(attr->inner->trimmed) : L"";
   }

   if (attr->has(ATTR_HIDDEN)) {
      attr->inner->hidden.value = exists ? (dwAttrib & FILE_ATTRIBUTE_HIDDEN) : false;
   }

   if (attr->has(ATTR_NAME)) {
      if (isDir) {
         attr->inner->name.value = os_fullname(attr->inner->trimmed);
      }
      else {
         attr->inner->name.value = os_hasExtension(attr->inner->trimmed)
            ? os_name(attr->inner->trimmed)
            : os_fullname(attr->inner->trimmed);
      }
   }

   if (attr->has(ATTR_READONLY)) {
      attr->inner->readonly.value = exists ? (dwAttrib & FILE_ATTRIBUTE_READONLY) : false;
   }

   if (attr->has(ATTR_SIZE)) {
      if (exists) {
         attr->inner->size.value = isFile
            ? _num(os_sizeFile(data))
            : _num(os_sizeDirectory(path, attr->uroboros));
      }
      else {
         attr->inner->size.value = _num(-1LL);
      }
   }
}

// attributes of something that does not exist
void os_loadAttributes_empty(const Attribute* attr)
{
   if (attr->has(ATTR_PATH)) {
      attr->inner->path.value = L"";
   }

   if (attr->has(ATTR_FULLNAME)) {
      attr->inner->fullname.value = L"";
   }

   if (attr->has(ATTR_DRIVE)) {
      attr->inner->drive.value = L"";
   }

   if (!attr->has(ATTR_EXISTS)) {
      return;
   }

   attr->inner->exists.value = false;
   attr->inner->isfile.value = false;
   attr->inner->isdirectory.value = false;

   if (attr->has(ATTR_ACCESS)) {
      attr->inner->access.value = _tim();
   }

   if (attr->has(ATTR_ARCHIVE)) {
      attr->inner->archive.value = false;
   }

   if (attr->has(ATTR_COMPRESSED)) {
      attr->inner->compressed.value = false;
   }

   if (attr->has(ATTR_CHANGE)) {
      attr->inner->change.value = _tim();
   }

   if (attr->has(ATTR_CREATION)) {
      attr->inner->creation.value = _tim();
   }

   if (attr->has(ATTR_EMPTY)) {
      attr->inner->empty.value = false;
   }

   if (attr->has(ATTR_ENCRYPTED)) {
      attr->inner->encrypted.value = false;
   }

   if (attr->has(ATTR_EXTENSION)) {
      attr->inner->extension.value = L"";
   }

   if (attr->has(ATTR_HIDDEN)) {
      attr->inner->hidden.value = false;
   }

   if (attr->has(ATTR_LIFETIME)) {
      attr->inner->lifetime.value = _per();
   }

   if (attr->has(ATTR_MODIFICATION)) {
      attr->inner->modification.value = _tim();
   }

   if (attr->has(ATTR_NAME)) {
      attr->inner->name.value = L"";
   }

   if (attr->has(ATTR_PARENT)) {
      attr->inner->parent.value = L"";
   }

   if (attr->has(ATTR_READONLY)) {
      attr->inner->readonly.value = false;
   }

   if (attr->has(ATTR_SIZE)) {
      attr->inner->size.value = _num(-1LL);
   }
}

_tim os_access(const _str& path)
{
   WIN32_FILE_ATTRIBUTE_DATA data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return convertToUroTime(&data.ftLastAccessTime);
}

_boo os_archive(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_ARCHIVE);
}

_tim os_change(const _str& path)
{
   WIN32_FILE_ATTRIBUTE_DATA data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return convertToUroTime(&data.ftLastWriteTime);
}

_boo os_compressed(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_COMPRESSED);
}

_tim os_creation(const _str& path)
{
   WIN32_FILE_ATTRIBUTE_DATA data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return convertToUroTime(&data.ftCreationTime);
}

_str os_drive(const _str& path)
{
   return os_isAbsolute(path)
      ? str(charStr(std::towupper(path[0])), L":")
      : L"";
}

_boo os_empty(const _str& path)
{
   WIN32_FILE_ATTRIBUTE_DATA data;
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

_boo os_encrypted(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_ENCRYPTED);
}

_boo os_emptyFile(const WIN32_FILE_ATTRIBUTE_DATA& data)
{
   return data.nFileSizeLow == 0
       && data.nFileSizeHigh == 0;
}

_boo os_emptyDirectory(const _str& path)
{
   WIN32_FIND_DATA data;
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
            return L"";
         }
         else {
            _str result = value.substr(i + 1);
            toLower(result);
            return result;
         }
      }
      else if (ch == OS_SEPARATOR) {
         return L"";
      }
   }

   return L"";
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

_boo os_hasAttribute(const _str& path, const ULONG& attribute)
{
   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES) &&
          (dwAttrib & attribute);
}

_boo os_hidden(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_HIDDEN);
}

_boo os_isFile(const _str& path)
{
   DWORD dwAttrib = GetFileAttributesW(path.c_str());
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return os_hasExtension(path);
   }

   return !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

_boo os_isDirectory(const _str& path)
{
   DWORD dwAttrib = GetFileAttributesW(path.c_str());
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return !os_hasExtension(path);
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
}

_per os_lifetime(const _str& path)
{
   WIN32_FILE_ATTRIBUTE_DATA data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _per();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _per();
   }

   const _tim modification = convertToUroTime(&data.ftLastWriteTime);
   const _tim creation = convertToUroTime(&data.ftCreationTime);

   return creation < modification
      ? (os_now() - creation)
      : (os_now() - modification);
}

_tim os_modification(const _str& path)
{
   WIN32_FILE_ATTRIBUTE_DATA data;

   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return _tim();
   }

   if (data.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
      return _tim();
   }

   return convertToUroTime(&data.ftLastWriteTime);
}

_str os_name(const _str& value)
{
   const _int len = value.length();
   _int i = len - 1;
   _int dot = -1;
   _boo sep = false;

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

   return L"";
}

_boo os_readonly(const _str& path)
{
   return os_hasAttribute(path, FILE_ATTRIBUTE_READONLY);
}

_nint os_size(const _str& path, Uroboros* uro)
{
   WIN32_FILE_ATTRIBUTE_DATA data;
   if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) {
      return -1LL;
   }

   const DWORD& dwAttrib = data.dwFileAttributes;
   if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
      return -1LL;
   }

   return dwAttrib & FILE_ATTRIBUTE_DIRECTORY
      ? os_sizeDirectory(path, uro)
      : os_sizeFile(data);
}

_nint os_sizeFile(const WIN32_FILE_ATTRIBUTE_DATA& data)
{
   return bigInteger(data.nFileSizeLow, data.nFileSizeHigh);
}

_nint os_sizeDirectory(const _str& path, Uroboros* uro)
{
   _nint totalSize = 0LL;
   WIN32_FIND_DATA data;
   HANDLE sh = NULL;
   sh = FindFirstFile((str(path, OS_SEPARATOR_ASTERISK)).c_str(), &data);

   if (sh == INVALID_HANDLE_VALUE) {
      return totalSize;
   }

   do {
      if (!uro->running) {
         FindClose(sh);
         return -1LL;
      }
      if (!os_isBrowsePath(data.cFileName)) {
         if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            totalSize += os_sizeDirectory(str(path, OS_SEPARATOR_ASTERISK, data.cFileName), uro);
         }
         else {
            totalSize += (_nint)(data.nFileSizeHigh * (MAXDWORD) + data.nFileSizeLow);
         }
      }
   } while (FindNextFile(sh, &data));

   FindClose(sh);
   return totalSize;
}

_boo os_exists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   return GetFileAttributesW(path.c_str())
      != INVALID_FILE_ATTRIBUTES;
}

_boo os_fileExists(const _str& path)
{
   if (!os_isAbsolute(path)) {
      return false;
   }

   DWORD dwAttrib = GetFileAttributesW(path.c_str());

   return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
          (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

_boo os_directoryExists(const _str& path)
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

_boo os_delete(const _str& path)
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

_boo os_drop(const _str& path, Uroboros* uro)
{
   return os_isFile(path)
      ? os_dropFile(path)
      : os_dropDirectory(path, uro);
}

_boo os_drop(const _str& path, const _boo& isFile, Uroboros* uro)
{
   return isFile
      ? os_dropFile(path)
      : os_dropDirectory(path, uro);
}

_boo os_dropFile(const _str& path)
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

_boo os_dropDirectory(const _str& path, Uroboros* uro)
{
   HANDLE hFind;
   WIN32_FIND_DATA FindFileData;

   _char DirPath[MAX_PATH];
   _char FileName[MAX_PATH];

   wcscpy(DirPath,(_char*)path.c_str());
   wcscat(DirPath, OS_SEPARATOR_ASTERISK.c_str());
   wcscpy(FileName,(_char*)path.c_str());
   wcscat(FileName, OS_SEPARATOR_STRING.c_str());

   hFind = FindFirstFile(DirPath,&FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) return false;
      wcscpy(DirPath,FileName);

   bool bSearch = true;
   while (bSearch) {
      if (FindNextFile(hFind,&FindFileData)) {
         if (!uro->running) {
            FindClose(hFind);
            return false;
         }

         if (!(wcscmp(FindFileData.cFileName, L".") && wcscmp(FindFileData.cFileName, L"..")))
            continue;

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

   return RemoveDirectoryW((_char*)path.c_str()) != 0;
}

_boo os_hide(const _str& path)
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

_boo os_lock(const _str& path)
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

_boo os_open(const _str& path)
{
   return (INT_PTR)ShellExecuteW(0, 0, path.c_str(), 0, 0 , SW_SHOW) > 32;
}

_boo os_openWith(const _str& program, const _str& path)
{
   return (INT_PTR)ShellExecuteW(NULL, L"open", program.c_str(), path.c_str(), L"", SW_SHOW) > 32;
}

_boo os_openAsCommand(const _str& command)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    const _size len = command.size() + 1;
    _char cmd[len];
    wcscpy(cmd, command.c_str());

    return CreateProcessW
    (
        NULL,
        cmd,
        NULL,NULL,FALSE,
        CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
        NULL,NULL,
        &si, &pi
    ) != 0;
}

_boo os_unhide(const _str& path)
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

_boo os_unlock(const _str& path)
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


_boo os_setTime(const _str& path, const _tim& creation,
   const _tim& access, const _tim& modification)
{
   FILETIME time_c;
   FILETIME time_a;
   FILETIME time_m;

   if (!(convertToFileTime(creation, time_c)
    && convertToFileTime(access, time_a)
    && convertToFileTime(modification, time_m)))
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

   const _boo result = SetFileTime(handle, &time_c, &time_a, &time_m);
   CloseHandle(handle);
   return result;
}

_boo os_createFile(const _str& path)
{
   if (os_hasParentDirectory(path)) {
      const _str p = os_parent(path);
      if (!os_exists(p)) {
         const _boo b = os_createDirectory(p);
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
      FILETIME ftime;

      if (convertToFileTime(os_now(), ftime)) {
         SetFileTime(h, &ftime, &ftime, &ftime);
      }

      CloseHandle(h);
      return true;
   }
   else {
      return false;
   }
}

_boo os_createDirectory(const _str& path)
{
   if (os_hasParentDirectory(path)) {
      const _str p = os_parent(path);
      if (!os_exists(p)) {
         const _boo b = os_createDirectory(p);
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

_boo os_moveTo(const _str& oldPath, const _str& newPath)
{
   return MoveFileExW(oldPath.c_str(), newPath.c_str(), MOVEFILE_COPY_ALLOWED) != 0;
}

_boo os_copyTo(const _str& oldPath, const _str& newPath, const _boo& isFile, Uroboros* uro)
{
   return isFile
      ? os_copyToFile(oldPath, newPath)
      : os_copyToDirectory(oldPath, newPath, uro);
}

_boo os_copyToFile(const _str& oldPath, const _str& newPath)
{
   return CopyFileW(oldPath.c_str(), newPath.c_str(), true) != 0;
}

_boo os_copyToDirectory(const _str& oldPath, const _str& newPath, Uroboros* uro)
{
   if (!os_createDirectory(newPath)) {
      return false;
   }

   const _size length = oldPath.size() + 1;

   HANDLE hFind;
   WIN32_FIND_DATA FindFileData;

   _char DirPath[MAX_PATH];
   _char FileName[MAX_PATH];

   wcscpy(DirPath,(_char*)oldPath.c_str());
   wcscat(DirPath, OS_SEPARATOR_ASTERISK.c_str());
   wcscpy(FileName,(_char*)oldPath.c_str());
   wcscat(FileName, &OS_SEPARATOR);

   hFind = FindFirstFile(DirPath,&FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) return false;
      wcscpy(DirPath,FileName);

   bool bSearch = true;
   while (bSearch) {
      if (FindNextFile(hFind,&FindFileData)) {
         if (!uro->running) {
            FindClose(hFind);
            return false;
         }

         if (!(wcscmp(FindFileData.cFileName, L".") && wcscmp(FindFileData.cFileName, L"..")))
            continue;

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

_boo os_copy(const std::set<_str>& paths)
{
    _size totalSize = sizeof(DROPFILES) + sizeof(_char);

   std::set<_str>::iterator it1;
   for (it1 = paths.begin(); it1 != paths.end(); ++it1) {
      totalSize += sizeof(_char) * (it1->size() + 1);
   }

   HDROP hdrop   = (HDROP)GlobalAlloc(GHND, totalSize);
   DROPFILES* df = (DROPFILES*)GlobalLock(hdrop);
   df->pFiles    = sizeof(DROPFILES);
   df->fWide     = true;

   _char* dstStart = (_char*)&df[1];
   std::set<_str>::iterator it2;
   for (it2 = paths.begin(); it2 != paths.end(); ++it2) {
      wcscpy(dstStart, it2->c_str());
      dstStart = &dstStart[it2->size() + 1];
   }

   GlobalUnlock(hdrop);
   OpenClipboard(NULL);
   EmptyClipboard();
   SetClipboardData(CF_HDROP, hdrop);
   CloseClipboard();

   return true;
}

_boo os_select(const _str& parent, const std::set<_str>& paths)
{
   ITEMIDLIST* folder = ILCreateFromPathW(parent.c_str());
   std::vector<ITEMIDLIST*> v;

   std::set<_str>::iterator it;
   for (it = paths.begin(); it != paths.end(); ++it) {
      v.push_back(ILCreateFromPathW(it->c_str()));
   }

   HRESULT hr = SHOpenFolderAndSelectItems(folder, v.size(),
      (LPCITEMIDLIST*)v.data(), 0);

   for (auto idl : v) {
      ILFree(idl);
   }

   ILFree(folder);
   return hr == S_OK;
}

_boo os_run(const _str& comm, Uroboros* uro)
{
   uro->sideProcess.running = true;
   STARTUPINFO si;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&uro->sideProcess.info, sizeof(uro->sideProcess.info));

   const _size len = comm.size() + 1;
   _char cmd[len];
   wcscpy(cmd, comm.c_str());

   const _size lenloc = uro->vars.inner.location.value.size() + 1;
   _char loc[lenloc];
   wcscpy(loc, uro->vars.inner.location.value.c_str());

   CreateProcessW
   (
      NULL,
      cmd,
      NULL,NULL,FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL,
      loc,
      &si, &uro->sideProcess.info
   );

   WaitForSingleObject(uro->sideProcess.info.hProcess, INFINITE);
   DWORD dwExitCode = 0;
   ::GetExitCodeProcess(uro->sideProcess.info.hProcess, &dwExitCode);

   uro->sideProcess.running = false;
   return uro->running && dwExitCode == 0;
}

_boo os_process(const _str& proc, Uroboros* uro)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));

   const _size len = proc.size() + 1;
   _char cmd[len];
   wcscpy(cmd, proc.c_str());

   const _size lenloc = uro->vars.inner.location.value.size() + 1;
   _char loc[lenloc];
   wcscpy(loc, uro->vars.inner.location.value.c_str());

   return CreateProcessW
   (
      NULL,
      cmd,
      NULL,NULL,FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL,
      loc,
      &si, &pi
   ) != 0;
}

_boo os_isInvaild(const _str& path)
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

_str os_trim(_str path)
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
         return L"";
      }
      case 1: {
         path = path.substr(len - 1, 1);
         os_escapeQuote(path);
         return path;
      }
   }

   _str result;
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

   if (start == 0) {
      if (end != len - 1) {
         path = path.substr(0, end + 1);
      }
   }
   else {
      if (end == len - 1) {
         path = path.substr(start);
      }
      else {
         path = path.substr(start, end - start + 1);
      }
   }

   std::replace(path.begin(),path.end(), OS_WRONG_SEPARATOR, OS_SEPARATOR);
   os_escapeQuote(path);
   return path;
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

_boo os_isAbsolute(const _str& path)
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

_boo os_hasExtension(const _str& value)
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

_boo os_isDriveLetter(const _char& ch)
{
   // or just use normal comparisons ch > 'a'  etc
   switch (ch) {
      case L'a': case L'b': case L'c': case L'd': case L'e':
      case L'f': case L'g': case L'h': case L'i': case L'j':
      case L'k': case L'l': case L'm': case L'n': case L'o':
      case L'p': case L'q': case L'r': case L's': case L't':
      case L'u': case L'v': case L'w': case L'x': case L'y':
      case L'z': case L'A': case L'B': case L'C': case L'D':
      case L'E': case L'F': case L'G': case L'H': case L'I':
      case L'J': case L'K': case L'L': case L'M': case L'N':
      case L'O': case L'P': case L'Q': case L'R': case L'S':
      case L'T': case L'U': case L'V': case L'W': case L'X':
      case L'Y': case L'Z':
         return true;
      default:
         return false;
   }
}

_boo os_isPath(const _str value)
{
   return value.find(OS_SEPARATOR) != _str::npos;
}

_boo os_hasParentDirectory(const _str& path)
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

_boo os_pathWasStacked(const _str& basePath)
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

_str os_readFile(const _str& path, _boo& result)
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

inline _nint bigInteger(const _uint32& low, const _uint32& high)
{
   _nint n = high;
   n <<= sizeof(high) * 8;
   n |= low;
   return n;
}

_boo os_isBrowsePath(const _str& path)
{
   return path == L"." || path == L"..";
}

inline _tim convertToUroTime(const FILETIME* time)
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

inline _boo convertToFileTime(const _tim& uroTime, FILETIME& result)
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
   _boo anySpace = false;
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
         value = L"";
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

