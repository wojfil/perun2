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
p_constexpr p_char OS_SEPARATOR = CHAR_BACKSLASH;
p_constexpr p_char OS_WRONG_SEPARATOR = CHAR_SLASH;


// for Windows OS only
// traditionally, it did not allow file paths to be longer than 260 characters
// but newer APIs make it possible to avoid this restriction as below
#define P_WINDOWS_PATH(path) (path.size() < MAX_PATH - 1) \
   ? path.c_str() \
   : str(STRING_WINDOWS_PATH_PREFIX, path).c_str() \


void os_init();

p_tim os_now();
p_tim os_today();

void os_rawSleepForMs(const p_nint ms);

void os_loadAttributes(FileContext& context);
void os_loadDataAttributes(FileContext& context, const p_fdata& data);

// get values of filesystem variables:
p_tim os_access(const p_str& path);
p_bool os_archive(const p_str& path);
p_tim os_change(const p_str& path);
p_bool os_compressed(const p_str& path);
p_tim os_creation(const p_str& path);
p_num os_depth(const p_str& value);
p_str os_drive(const p_str& path);
p_bool os_empty(const p_str& path);
p_bool os_emptyFile(const p_adata& data);
p_bool os_emptyDirectory(const p_str& path);
p_bool os_encrypted(const p_str& path);
p_bool os_hasAttribute(const p_str& path, const DWORD attribute);
p_bool os_hidden(const p_str& path);
p_bool os_isFile(const p_str& path);
p_bool os_isDirectory(const p_str& path);
p_bool os_isDirectory(const p_fdata& data);
p_per os_lifetime(const p_str& path);
p_tim os_modification(const p_str& path);
p_bool os_readonly(const p_str& path);
p_nint os_size(const p_str& path, p_perun2& p2);
p_nint os_sizeDirectory(const p_str& path, p_perun2& p2);
p_bool os_constr_sizeDirectory(const p_str& path, IncrementalConstraint& constr, p_perun2& p2);

p_bool os_exists(const p_str& path);
p_bool os_fileExists(const p_str& path);
p_bool os_directoryExists(const p_str& path);

p_bool os_hasFirstFile(const p_str& path, p_entry& entry, p_fdata& output);
p_bool os_hasNextFile(p_entry& entry, p_fdata& output);
void os_closeEntry(p_entry& entry);

// filesystem operations:
// some of them take a reference to the running Perun2 instance
// they can be stopped safely by an interruption signal during operation
p_bool os_delete(const p_str& path);
p_bool os_drop(const p_str& path, p_perun2& p2);
p_bool os_drop(const p_str& path, const p_bool isFile, p_perun2& p2);
p_bool os_dropFile(const p_str& path);
p_bool os_dropDirectory(const p_str& path, p_perun2& p2);
p_bool os_hide(const p_str& path);
p_bool os_lock(const p_str& path);
p_bool os_open(const p_str& path);
p_bool os_openWith(const p_str& program, const p_str& path);
p_bool os_openAsCommand(const p_str& command, const p_str& location);
p_bool os_unhide(const p_str& path);
p_bool os_unlock(const p_str& path);

p_bool os_setTime(const p_str& path, const p_tim& creation,
   const p_tim& access, const p_tim& modification);

p_bool os_createFile(const p_str& path);
p_bool os_createDirectory(const p_str& path);

p_bool os_moveTo(const p_str& oldPath, const p_str& newPath);
p_bool os_copyTo(const p_str& oldPath, const p_str& newPath, const p_bool isFile, p_perun2& p2);
p_bool os_copyToFile(const p_str& oldPath, const p_str& newPath);
p_bool os_copyToDirectory(const p_str& oldPath, const p_str& newPath, p_perun2& p2);

p_bool os_copy(const p_set& paths);
p_bool os_select(const p_str& parent, const p_set& paths);

p_bool os_run(const p_str& comm, const p_str& location, p_perun2& p2);

p_str os_trim(const p_str& path);
p_bool os_isInvalid(const p_str& path);
uint32_t os_patternInfo(const p_str& pattern);

p_str os_doubleDotsPrefix(p_int amount);
p_bool os_hasDotSegments(const p_str& path);
p_str os_trimRetreats(const p_str& path, p_size& retreats);
p_str os_segmentWithName(const p_str& path);
p_str os_retreats(p_int times);
p_bool os_retreatPath(p_str& path);
void os_retreatPath(p_str& path, p_int times);

// return true if the result is a 'true path' (has no .. nor .)
p_bool os_extendPath(p_str& result, const p_str& path);

// it is guaranteed, that both paths do not contain . nor ..
p_str os_softJoin(const p_str& path1, const p_str& path2);

// the left path does not contain . nor .., the right path may
// can return empty string if .. pushes us too far to the left
p_str os_leftJoin(const p_str& path1, const p_str& path2);

// both paths may contain multiple . and .. anywhere
p_str os_join(const p_str& path1, const p_str& path2);

p_bool os_endsWithDoubleDot(const p_str& path);
p_bool os_isAbsolute(const p_str& path);
p_bool os_hasExtension(const p_str& value);
p_bool os_isDriveLetter(const p_char ch);
p_bool os_isPath(const p_str& value);

p_bool os_isAncestor(const p_str& path, const p_str& supposedChildPath);
p_bool os_hasParentDirectory(const p_str& path);
p_str os_stackPath(const p_str& path);
p_str os_stackPathExt(const p_str& basePath, const p_str& extension);
p_str os_stackPathBase(const p_str& path);

p_bool os_pathWasStacked(const p_str& basePath);
p_str os_stackPathStacked(const p_str& path);
p_str os_stackPathExtStacked(const p_str& path, const p_str& extension);
void os_getStackedData(const p_str& path, p_nint& index, p_str& basePath);

p_str os_executablePath();
p_str os_desktopPath();
p_list os_pendrives();
p_str os_currentPath();
p_str os_system32Path();

p_bool os_readFile(p_str& result, const p_str& path);
void os_showWebsite(const p_str& url);
p_bool os_findText(const p_str& path, const p_str& value);

inline uint64_t os_bigInteger(const uint32_t low, const uint32_t high);
inline p_bool os_isBrowsePath(const p_str& path);
inline p_tim os_convertToPerun2Time(const p_ftim* time);
inline p_bool os_convertToFileTime(const p_tim& perunTime, p_ftim& result);

p_str os_makeArg(const p_str& value);
p_str os_quoteEmbraced(const p_str& value);

}

#endif /* OS_H */
