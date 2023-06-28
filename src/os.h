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

#ifndef OS_H
#define OS_H

#include "os-common.h"


namespace perun2
{

// default file path separator
// in Windows OS, this separator is \ and the 'wrong separator' is /
_constexpr _char OS_SEPARATOR = CHAR_BACKSLASH;
_constexpr _char OS_WRONG_SEPARATOR = CHAR_SLASH;


// for Windows OS only
// traditionally, it did not allow file paths to be longer than 260 characters
// but newer APIs make it possible to avoid this restriction as below
#define P_WINDOWS_PATH(path) (path.size() < MAX_PATH - 1) \
   ? path.c_str() \
   : str(STRING_LONG_PATH_PREFIX, path).c_str() \


void os_init();

_tim os_now();
_tim os_today();

void os_rawSleepForMs(const _nint ms);

void os_loadAttributes(FileContext& context);
void os_loadDataAttributes(FileContext& context, const _fdata& data);

// get values of filesystem variables:
_tim os_access(const _str& path);
_bool os_archive(const _str& path);
_tim os_change(const _str& path);
_bool os_compressed(const _str& path);
_tim os_creation(const _str& path);
_num os_depth(const _str& value);
_str os_drive(const _str& path);
_bool os_empty(const _str& path);
_bool os_emptyFile(const _adata& data);
_bool os_emptyDirectory(const _str& path);
_bool os_encrypted(const _str& path);
_bool os_hasAttribute(const _str& path, const DWORD attribute);
_bool os_hidden(const _str& path);
_bool os_isFile(const _str& path);
_bool os_isDirectory(const _str& path);
_per os_lifetime(const _str& path);
_tim os_modification(const _str& path);
_bool os_readonly(const _str& path);
_nint os_size(const _str& path, _p2& p2);
_nint os_sizeDirectory(const _str& path, _p2& p2);

_bool os_exists(const _str& path);
_bool os_fileExists(const _str& path);
_bool os_directoryExists(const _str& path);

_bool os_hasFirstFile(const _str& path, _entry& entry, _fdata& output);
_bool os_hasNextFile(_entry& entry, _fdata& output);

// filesystem operations:
// some of them take a reference to the running Perun2 instance
// they can be stopped safely by an interruption signal during operation
_bool os_delete(const _str& path);
_bool os_drop(const _str& path, _p2& p2);
_bool os_drop(const _str& path, const _bool isFile, _p2& p2);
_bool os_dropFile(const _str& path);
_bool os_dropDirectory(const _str& path, _p2& p2);
_bool os_hide(const _str& path);
_bool os_lock(const _str& path);
_bool os_open(const _str& path);
_bool os_openWith(const _str& program, const _str& path);
_bool os_openAsCommand(const _str& command, const _str& location);
_bool os_unhide(const _str& path);
_bool os_unlock(const _str& path);

_bool os_setTime(const _str& path, const _tim& creation,
   const _tim& access, const _tim& modification);

_bool os_createFile(const _str& path);
_bool os_createDirectory(const _str& path);

_bool os_moveTo(const _str& oldPath, const _str& newPath);
_bool os_copyTo(const _str& oldPath, const _str& newPath, const _bool isFile, _p2& p2);
_bool os_copyToFile(const _str& oldPath, const _str& newPath);
_bool os_copyToDirectory(const _str& oldPath, const _str& newPath, _p2& p2);

_bool os_copy(const _set& paths);
_bool os_select(const _str& parent, const _set& paths);

_bool os_run(const _str& comm, const _str& location, _p2& p2);

_bool os_isInvaild(const _str& path);
uint32_t os_patternInfo(const _str& pattern);

_str os_doubleDotsPrefix(_int amount);
_bool os_hasDotSegments(const _str& path);
_str os_trimRetreats(const _str& path, _size& retreats);
_str os_segmentWithName(const _str& path);
_str os_retreats(_int times);
_bool os_retreatPath(_str& path);
void os_retreatPath(_str& path, _int times);

// return true if the result is a 'true path' (has no .. nor .)
_bool os_extendPath(_str& result, const _str& path);

// it is guaranteed, that both paths do not contain . nor ..
_str os_softJoin(const _str& path1, const _str& path2);

// the left path does not contain . nor .., the right path may
// can return empty string if .. pushes us too far to the left
_str os_leftJoin(const _str& path1, const _str& path2);

// both paths may contain multiple . and .. anywhere
_str os_join(const _str& path1, const _str& path2);

_bool os_endsWithDoubleDot(const _str& path);
_bool os_isAbsolute(const _str& path);
_bool os_hasExtension(const _str& value);
_bool os_isDriveLetter(const _char ch);
_bool os_isPath(const _str& value);
_bool os_isExplorableDirectory(const _str& name);

_bool os_isAncestor(const _str& path, const _str& supposedChildPath);
_bool os_hasParentDirectory(const _str& path);
_str os_stackPath(const _str& path);
_str os_stackPathExt(const _str& basePath, const _str& extension);
_str os_stackPathBase(const _str& path);

_bool os_pathWasStacked(const _str& basePath);
_str os_stackPathStacked(const _str& path);
_str os_stackPathExtStacked(const _str& path, const _str& extension);
void os_getStackedData(const _str& path, _nint& index, _str& basePath);

_str os_executablePath();
_str os_desktopPath();
_str os_currentPath();
_str os_system32Path();

_bool os_readFile(_str& result, const _str& path);
void os_showWebsite(const _str& url);
_bool os_findText(const _str& path, const _str& value);

inline uint64_t os_bigInteger(const uint32_t low, const uint32_t high);
inline _bool os_isBrowsePath(const _str& path);
inline _tim os_convertToPerun2Time(const _ftim* time);
inline _bool os_convertToFileTime(const _tim& perunTime, _ftim& result);

_str os_makeArg(const _str& value);
_str os_quoteEmbraced(const _str& value);

}

#endif /* OS_H */
