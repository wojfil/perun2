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

#include "aggregate.h"
#include "com-core-aggr.h"
#include "../os.h"
#include "../var/var-runtime.h"
#include "../uroboros.h"


Aggregate::Aggregate()
{
   failedCopy = 0;
   failedSelect = 0;
   value = 0;
}

void Aggregate::set(const _uint32& v)
{
   if (!has(v)) {
      value |= v;
   }
}

void Aggregate::run()
{
   if (value == 0) {
      return;
   }

   _boo selectFailure = false;

   if (has(AGGR_SELECT)) {
      if (!invalidSelect.empty()) {
         for (auto it = invalidSelect.begin(); it != invalidSelect.end(); it++) {
            logSelectError(*it);
         }
         invalidSelect.clear();
      }

      if (failedSelect > 0) {
         for (_uint32 i = 0; i < failedSelect; i++) {
            logSelectError(L"");
         }
         failedSelect = 0;
      }

      if (!selectPaths.empty()) {
         _boo anyGoodPath = false;
         std::set<_str> goodPaths;

         for (const std::pair<_str, std::set<_str>>& pair : selectPaths) {
            if (!g_running) {
               break;
            }

            const _str& parent = pair.first;
            const std::set<_str>& paths = pair.second;

            if (!os_directoryExists(parent)) {
               for (auto it = paths.begin(); it != paths.end(); it++) {
                  logSelectError(*it);
               }
               continue;
            }

            for (auto it = paths.begin(); it != paths.end(); it++) {
               const _str& path = *it;
               if (os_exists(path)) {
                  goodPaths.insert(path);
                  anyGoodPath = true;
               }
               else {
                  logSelectError(path);
               }
            }

            if (anyGoodPath) {
               if (os_select(parent, goodPaths)) {
                  for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                     logSelectSuccess(*it);
                  }
                  selectFailure = false;
               }
               else {
                  for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                     logSelectError(*it);
                  }
                  selectFailure = true;
               }

               goodPaths.clear();
               anyGoodPath = false;
            }
            else {
               selectFailure = true;
            }
         }

         selectPaths.clear();
      }
   }

   if (has(AGGR_COPY)) {
      if (!invalidCopy.empty()) {
         for (auto it = invalidCopy.begin(); it != invalidCopy.end(); it++) {
            logCopyError(*it);
         }
         invalidCopy.clear();
      }

      if (failedCopy > 0) {
         for (_uint32 i = 0; i < failedCopy; i++) {
            logCopyError(L"");
         }
         failedCopy = 0;
      }

      if (!copyPaths.empty()) {
         std::set<_str> goodPaths;

         for (auto it = copyPaths.begin(); it != copyPaths.end(); it++) {
            const _str& path = *it;
            if (os_exists(path)) {
               goodPaths.insert(path);
            }
            else {
               logCopyError(path);
            }
         }

         if (!goodPaths.empty()) {
            if (os_copy(goodPaths)) {
               for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                  logCopySuccess(*it);
               }
               g_success.value = !selectFailure;
            }
            else {
               for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                  logCopyError(*it);
               }
               g_success.value = false;
            }
         }

         copyPaths.clear();
      }
   }
}

inline _boo Aggregate::has(const _uint32& v) const
{
   return value & v;
}
