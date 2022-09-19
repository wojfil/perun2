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

#ifndef OS_H
#define OS_H

#include "datatype\datatype.h"
#include "attribute.h"
#include <set>
#include <map>
#include "windows.h"


namespace uro
{

inline constexpr _char OS_SEPARATOR = L'\\';
const _str OS_SEPARATOR_STRING = L"\\";
const _str OS_SEPARATOR_ASTERISK = L"\\*";
const _str OS_SEPARATOR_APOSTROPHE = L"\\'";
inline constexpr _char OS_WRONG_SEPARATOR = L'/';
const _str OS_UROEXT = L"uro";
const _str OS_DOT_UROEXT = L".uro";
const _str OS_GIT_DIRECTORY = L".git";

struct InnerVariables;

_tim os_now();
_tim os_today();
_tim os_yesterday();
_tim os_tomorrow();

void os_sleepForMs(const _nint& ms, Uroboros* uro);

void os_loadAttributes(const Attribute* attr, Uroboros* uro);
void os_loadEmptyAttributes(const Attribute* attr, InnerVariables& inner);
void os_loadDataAttributes(const Attribute* attr, Uroboros* uro, _fdata* data);

// file system variables:
_tim os_access(const _str& path);
_boo os_archive(const _str& path);
_tim os_change(const _str& path);
_boo os_compressed(const _str& path);
_tim os_creation(const _str& path);
_str os_drive(const _str& path);
_boo os_empty(const _str& path);
_boo os_emptyFile(const _adata& data);
_boo os_emptyDirectory(const _str& path);
_boo os_encrypted(const _str& path);
_str os_extension(const _str& value);
_str os_fullname(const _str& value);
_boo os_hasAttribute(const _str& path, const ULONG& attribute);
_boo os_hidden(const _str& path);
_boo os_isFile(const _str& path);
_boo os_isDirectory(const _str& path);
_per os_lifetime(const _str& path);
_tim os_modification(const _str& path);
_str os_name(const _str& value);
_str os_parent(const _str& path);
_boo os_readonly(const _str& path);
_nint os_size(const _str& path, Uroboros* uro);
_nint os_sizeFile(const _adata& data);
_nint os_sizeDirectory(const _str& path, Uroboros* uro);

_boo os_exists(const _str& path);
_boo os_fileExists(const _str& path);
_boo os_directoryExists(const _str& path);

// file system operations:
// some of them require pointer to the running Uroboros instance
// they can be stopped safely by an interruption call during operation
_boo os_delete(const _str& path);
_boo os_drop(const _str& path, Uroboros* uro);
_boo os_drop(const _str& path, const _boo& isFile, Uroboros* uro);
_boo os_dropFile(const _str& path);
_boo os_dropDirectory(const _str& path, Uroboros* uro);
_boo os_hide(const _str& path);
_boo os_lock(const _str& path);
_boo os_open(const _str& path);
_boo os_openWith(const _str& program, const _str& path);
_boo os_openAsCommand(const _str& command);
_boo os_unhide(const _str& path);
_boo os_unlock(const _str& path);

_boo os_setTime(const _str& path, const _tim& creation,
   const _tim& access, const _tim& modification);

_boo os_createFile(const _str& path);
_boo os_createDirectory(const _str& path);

_boo os_moveTo(const _str& oldPath, const _str& newPath);
_boo os_copyTo(const _str& oldPath, const _str& newPath, const _boo& isFile, Uroboros* uro);
_boo os_copyToFile(const _str& oldPath, const _str& newPath);
_boo os_copyToDirectory(const _str& oldPath, const _str& newPath, Uroboros* uro);

_boo os_copy(const std::set<_str>& paths);
_boo os_select(const _str& parent, const std::set<_str>& paths);

_boo os_run(const _str& comm, Uroboros* uro);
_boo os_process(const _str& command, const _str& location);

_boo os_isInvaild(const _str& path);
_str os_trim(const _str& path);
inline void os_escapeQuote(_str& path);
_str os_join(const _str& path1, const _str& path2);
_boo os_isAbsolute(const _str& path);
_boo os_hasExtension(const _str& value);
_boo os_isDriveLetter(const _char& ch);
_boo os_isPath(const _str value);

_boo os_hasParentDirectory(const _str& path);
_str os_stackPath(const _str& path);
_str os_stackPathExt(const _str& basePath, const _str& extension);
_str os_stackPathBase(const _str& path);

_boo os_pathWasStacked(const _str& basePath);
_str os_stackPathStacked(const _str& path);
_str os_stackPathExtStacked(const _str& path, const _str& extension);
void os_getStackedData(const _str& path, _nint& index, _str& basePath);

_str os_uroborosPath();
_str os_desktopPath();
_str os_currentPath();

_str os_readFile(const _str& path, _boo& result);
void os_showWebsite(const _str& url);

inline _nint bigInteger(const _uint32& low, const _uint32& high);
_boo os_isBrowsePath(const _str& path);
inline _tim convertToUroTime(const _ftim* time);
inline _boo convertToFileTime(const _tim& uroTime, _ftim& result);

_str os_makeArg(const _str& value);
void os_rawTrim(_str& value);
_str os_quoteEmbraced(const _str& value);

}

#endif /* OS_H */
