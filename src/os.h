/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OS_H
#define OS_H

#include "datatype\datatype.h"
#include "attribute.h"


namespace uro
{

// theses constants refer to the default file path separator
// in Windows OS, this separator is \ and the 'wrong separator' is /
// example: C:\some\correct\path
inline constexpr _char OS_SEPARATOR = CHAR_BACKSLASH;
inline constexpr _char OS_WRONG_SEPARATOR = CHAR_SLASH;

static const _str OS_SEPARATOR_STRING = toStr(OS_SEPARATOR);
static const _str OS_SEPARATOR_ASTERISK = str(OS_SEPARATOR_STRING, STRING_ASTERISK);
static const _str OS_SEPARATOR_APOSTROPHE = str(OS_SEPARATOR_STRING, STRING_ASTERISK);

static const _str OS_UROEXT = STRING_URO;
static const _str OS_DOT_UROEXT = str(STRING_DOT, STRING_URO);
static const _str OS_GIT_DIRECTORY = str(STRING_DOT, STRING_GIT);
static const _str OS_SVN_DIRECTORY = str(STRING_DOT, STRING_SVN);

_tim os_now();
_tim os_today();
_tim os_yesterday();
_tim os_tomorrow();

void os_sleepForMs(const _nint& ms, _uro& uro);
void os_rawSleepForMs(const _nint& ms);

void os_loadAttributes(FileContext& context);
void os_loadEmptyAttributes(FileContext& context);

// file system variables:
_tim os_access(const _str& path);
_bool os_archive(const _str& path);
_tim os_change(const _str& path);
_bool os_compressed(const _str& path);
_tim os_creation(const _str& path);
_str os_drive(const _str& path);
_bool os_empty(const _str& path);
_bool os_emptyFile(const _adata& data);
_bool os_emptyDirectory(const _str& path);
_bool os_encrypted(const _str& path);
_str os_extension(const _str& value);
_str os_fullname(const _str& value);
_bool os_hasAttribute(const _str& path, const _ulong& attribute);
_bool os_hidden(const _str& path);
_bool os_isFile(const _str& path);
_bool os_isDirectory(const _str& path);
_per os_lifetime(const _str& path);
_tim os_modification(const _str& path);
_str os_name(const _str& value);
_str os_parent(const _str& path);
_bool os_readonly(const _str& path);
_nint os_size(const _str& path, _uro& uro);
_nint os_sizeDirectory(const _str& path, _uro& uro);

_bool os_exists(const _str& path);
_bool os_fileExists(const _str& path);
_bool os_directoryExists(const _str& path);

// file system operations:
// some of them require pointer to the running Uroboros2 instance
// they can be stopped safely by an interruption call during operation
_bool os_delete(const _str& path);
_bool os_drop(const _str& path, _uro& uro);
_bool os_drop(const _str& path, const _bool& isFile, _uro& uro);
_bool os_dropFile(const _str& path);
_bool os_dropDirectory(const _str& path, _uro& uro);
_bool os_hide(const _str& path);
_bool os_lock(const _str& path);
_bool os_open(const _str& path);
_bool os_openWith(const _str& program, const _str& path);
_bool os_openAsCommand(const _str& command);
_bool os_unhide(const _str& path);
_bool os_unlock(const _str& path);

_bool os_setTime(const _str& path, const _tim& creation,
   const _tim& access, const _tim& modification);

_bool os_createFile(const _str& path);
_bool os_createDirectory(const _str& path);

_bool os_moveTo(const _str& oldPath, const _str& newPath);
_bool os_copyTo(const _str& oldPath, const _str& newPath, const _bool& isFile, _uro& uro);
_bool os_copyToFile(const _str& oldPath, const _str& newPath);
_bool os_copyToDirectory(const _str& oldPath, const _str& newPath, _uro& uro);

_bool os_copy(const _set& paths);
_bool os_select(const _str& parent, const _set& paths);

_bool os_run(const _str& comm, const _str& location, _uro& uro);
_bool os_process(const _str& command, const _str& location);

_bool os_isInvaild(const _str& path);
_uint32 os_patternInfo(const _str& pattern);
_bool os_bothAreSeparators(const _char& left, const _char& right);
_str os_trim(const _str& path);
inline void os_escapeQuote(_str& path);
_str os_join(const _str& path1, const _str& path2);
_bool os_isAbsolute(const _str& path);
_bool os_hasExtension(const _str& value);
inline _bool os_isDriveLetter(const _char& ch);
_bool os_isPath(const _str value);
_bool os_isExplorableDirectory(const _str& name);

_bool os_hasParentDirectory(const _str& path);
_str os_stackPath(const _str& path);
_str os_stackPathExt(const _str& basePath, const _str& extension);
_str os_stackPathBase(const _str& path);

_bool os_pathWasStacked(const _str& basePath);
_str os_stackPathStacked(const _str& path);
_str os_stackPathExtStacked(const _str& path, const _str& extension);
void os_getStackedData(const _str& path, _nint& index, _str& basePath);

_str os_uroborosPath();
_str os_desktopPath();
_str os_currentPath();

_bool os_readFile(_str& result, const _str& path);
void os_showWebsite(const _str& url);
_bool os_find(const _str& path, const _str& value);

inline _uint64 os_bigInteger(const _uint32& low, const _uint32& high);
inline _bool os_isBrowsePath(const _str& path);
inline _tim os_convertToUroTime(const _ftim* time);
inline _bool os_convertToFileTime(const _tim& uroTime, _ftim& result);

_str os_makeArg(const _str& value);
void os_rawTrim(_str& value);
_str os_quoteEmbraced(const _str& value);

}

#endif /* OS_H */
