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

#include "gen-os.h"
#include "../../uroboros.h"
#include "../../os.h"



_def* Gen_ElementsAtLocation::generate(Generator<_str>* location)
{
   return new ElementsAtLocation(location, element);
}

void ElementsAtLocation::reset()
{
   if (!first) {
      first = true;
      FindClose(handle);
      g_this_s.value = prevThis;
      g_index.value = prevIndex;
      g_depth.value = prevDepth;
   }
}

_boo ElementsAtLocation::hasNext()
{
   if (first) {
      _str path = os_trim(location->getValue());
      if (os_directoryExists(path)) {
         path = str(path, OS_SEPARATOR_ASTERISK);
         handle = FindFirstFile(path.c_str(), &data);
         if (handle == INVALID_HANDLE_VALUE) {
            return false;
         }

         first = false;
         value = data.cFileName;
         prevThis = g_this_s.value;
         prevIndex = g_index.value;
         prevDepth = g_depth.value;
         g_depth.value = _num(0LL);
         index = _num(0LL);
         g_index.value = index;

         if (!os_isBrowsePath(value)) {
            const _boo isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            switch (element) {
               case ELEM_ALL: {
                  if ((g_flags & FLAG_NOOMIT) || (isDir && value != OS_GIT_DIRECTORY)
                     || (!isDir && os_extension(value) != OS_UROEXT))
                  {
                     g_index.value = index;
                     index++;
                     g_this_s.value = value;
                     return true;
                  }
                  break;
               }
               case ELEM_DIRECTORIES: {
                  if (isDir && ((g_flags & FLAG_NOOMIT) || value != OS_GIT_DIRECTORY)) {
                     g_index.value = index;
                     index++;
                     g_this_s.value = value;
                     return true;
                  }
                  break;
               }
               case ELEM_FILES: {
                  if (!isDir && ((g_flags & FLAG_NOOMIT) || os_extension(value) != OS_UROEXT)) {
                     g_index.value = index;
                     index++;
                     g_this_s.value = value;
                     return true;
                  }
                  break;
               }
            }
         }
      }
      else {
         return false;
      }
   }

   while (FindNextFile(handle, &data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _boo isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         switch (element) {
            case ELEM_ALL: {
               if ((g_flags & FLAG_NOOMIT) || (isDir && value != OS_GIT_DIRECTORY)
                  || (!isDir && os_extension(value) != OS_UROEXT))
               {
                  g_index.value = index;
                  index++;
                  g_this_s.value = value;
                  return true;
               }
               break;
            }
            case ELEM_DIRECTORIES: {
               if (isDir && ((g_flags & FLAG_NOOMIT) || value != OS_GIT_DIRECTORY)) {
                  g_index.value = index;
                  index++;
                  g_this_s.value = value;
                  return true;
               }
               break;
            }
            case ELEM_FILES: {
               if (!isDir && ((g_flags & FLAG_NOOMIT) || os_extension(value) != OS_UROEXT)) {
                  g_index.value = index;
                  index++;
                  g_this_s.value = value;
                  return true;
               }
               break;
            }
         }
      }
   }

   first = true;
   FindClose(handle);
   g_this_s.value = prevThis;
   g_index.value = prevIndex;
   g_depth.value = prevDepth;
   return false;
}

_def* Gen_RecursiveFiles::generate(Generator<_str>* location)
{
   return new RecursiveFiles(location);
}

RecursiveFiles::~RecursiveFiles() {
   delete location;
}

void RecursiveFiles::reset()
{
   if (!first) {
      first = true;
      paths.clear();
      bases.clear();
      const _size len = handles.size();
      if (len != 0) {
         for (_size i = 0; i < len; i++) {
            FindClose(handles[i]);
         }
         handles.clear();
      }
      g_depth.value = prevDepth;
      g_this_s.value = prevThis;
      g_index.value = prevIndex;
   }
}

_boo RecursiveFiles::hasNext()
{
   if (first) {
      this->paths.push_back(os_trim(location->getValue()));
      prevDepth = g_depth.value;
      g_depth.value = _num(0LL);
      goDeeper = true;
      first = false;
      prevThis = g_this_s.value;
      prevIndex = g_index.value;
      index = _num(0LL);
      g_index.value = index;
   }

   while (g_running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), OS_SEPARATOR_ASTERISK);
            handles.push_back(FindFirstFile(p.c_str(), &data));

            if (handles.back() == INVALID_HANDLE_VALUE)
            {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
                  g_depth.value--;
               }
            }
            else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
               const _str& v = data.cFileName;

               if ((g_flags & FLAG_NOOMIT) || os_extension(v) != OS_UROEXT) {
                  value = v;
                  g_index.value = index;
                  index++;
                  g_this_s.value = value;
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
               g_depth.value--;
            }
         }
      }
      else {
         if (FindNextFile(handles.back(), &data)) {
            const _str& v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  if ((g_flags & FLAG_NOOMIT) || v != OS_GIT_DIRECTORY) {
                     paths.push_back(str(paths.back(), OS_SEPARATOR_STRING, v));

                     if (g_depth.value.isZero()) {
                        bases.push_back(str(v, OS_SEPARATOR_STRING));
                     }
                     else {
                        bases.push_back(str(bases.back(), v, OS_SEPARATOR_STRING));
                     }

                     goDeeper = true;
                     g_depth.value++;
                  }
               }
               else {
                  if ((g_flags & FLAG_NOOMIT) || os_extension(v) != OS_UROEXT) {
                     value = g_depth.value.isZero() ? v : str(bases.back(), v);
                     g_index.value = index;
                     index++;
                     g_this_s.value = value;
                     return true;
                  }
               }
            }
         }
         else {
            FindClose(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               g_depth.value--;
            }
         }
      }
   }

   reset();
   return false;
}

_def* Gen_RecursiveDirectories::generate(Generator<_str>* location)
{
   return new RecursiveDirectories(location);
}

RecursiveDirectories::~RecursiveDirectories() {
   delete location;
}

void RecursiveDirectories::reset()
{
   if (!first) {
      first = true;
      paths.clear();
      bases.clear();
      const _size len = handles.size();
      if (len != 0) {
         for (_size i = 0; i < len; i++) {
            FindClose(handles[i]);
         }
         handles.clear();
      }
      g_depth.value = prevDepth;
      g_this_s.value = prevThis;
      g_index.value = prevIndex;
   }
}

_boo RecursiveDirectories::hasNext()
{
   if (first) {
      paths.push_back(os_trim(location->getValue()));
      prevDepth = g_depth.value;
      g_depth.value = _num(-1LL);
      goDeeper = true;
      first = false;
      prevThis = g_this_s.value;
      prevIndex = g_index.value;
      index = _num(0LL);
      g_index.value = index;
   }

   while (g_running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), OS_SEPARATOR_ASTERISK);

            handles.push_back(FindFirstFile(p.c_str(), &data));
            if (handles.back() == INVALID_HANDLE_VALUE)
            {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
                  g_depth.value--;
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
               g_depth.value--;
            }
         }
      }
      else {
         if (FindNextFile(handles.back(), &data)) {
            const _str& v = data.cFileName;

            if (!os_isBrowsePath(v) && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                && ((g_flags & FLAG_NOOMIT) || v != OS_GIT_DIRECTORY))
            {
               value = g_depth.value.isMinusOne() ? v : str(bases.back(), v);
               paths.push_back(str(paths.back(), OS_SEPARATOR_STRING, v));

               if (g_depth.value.isMinusOne()) {
                  bases.push_back(str(v, OS_SEPARATOR_STRING));
               }
               else {
                  bases.push_back(str(bases.back(), v, OS_SEPARATOR_STRING));
               }

               goDeeper = true;
               g_depth.value++;
               g_index.value = index;
               index++;
               g_this_s.value = value;
               return true;
            }
         }
         else {
            FindClose(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               g_depth.value--;
            }
         }
      }
   }

   reset();
   return false;
}
