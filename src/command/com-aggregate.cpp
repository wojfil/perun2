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

#include "com-aggregate.h"
#include "com-core-aggr.h"
#include "../os.h"
#include "../perun2.h"


namespace perun2::comm
{

Aggregate::Aggregate(_p2& p2)
   : contexts(p2.contexts), perun2(p2) { };

void Aggregate::set(const _uint32 v)
{
   if (!has(v)) {
      value |= v;
   }
}

void Aggregate::run()
{
   if (value == AGGR_NULL) {
      return;
   }

   _bool selectFailure = false;

   if (has(AGGR_SELECT)) {
      if (!invalidSelect.empty()) {
         for (auto it = invalidSelect.begin(); it != invalidSelect.end(); it++) {
            logSelectError(this->perun2, *it);
         }
         invalidSelect.clear();
      }

      if (failedSelect > 0) {
         for (_uint32 i = 0; i < failedSelect; i++) {
            logSelectError(this->perun2, EMPTY_STRING);
         }
         failedSelect = 0;
         selectFailure = true;
      }

      if (!selectPaths.empty()) {
         _bool anyGoodPath = false;
         _set goodPaths;

         for (const std::pair<_str, _set>& pair : selectPaths) {
            const _str& parent = pair.first;
            const _set& paths = pair.second;

            if (!os_directoryExists(parent)) {
               for (auto it = paths.begin(); it != paths.end(); it++) {
                  logSelectError(this->perun2, *it);
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
                  logSelectError(this->perun2, path);
                  selectFailure = true;
               }
            }

            if (anyGoodPath) {
               if (os_select(parent, goodPaths)) {
                  for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                     logSelectSuccess(this->perun2, *it);
                  }
               }
               else {
                  for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                     logSelectError(this->perun2, *it);
                  }
                  selectFailure = true;
               }

               goodPaths.clear();
               anyGoodPath = false;
            }
         }

         selectPaths.clear();
      }
   }

   if (has(AGGR_COPY)) {
      if (!invalidCopy.empty()) {
         for (auto it = invalidCopy.begin(); it != invalidCopy.end(); it++) {
            logCopyError(this->perun2, *it);
         }
         invalidCopy.clear();
      }

      if (failedCopy > 0) {
         for (_uint32 i = 0; i < failedCopy; i++) {
            logCopyError(this->perun2, EMPTY_STRING);
         }
         failedCopy = 0;
      }

      if (!copyPaths.empty()) {
         _set goodPaths;

         for (auto it = copyPaths.begin(); it != copyPaths.end(); it++) {
            const _str& path = *it;
            if (os_exists(path)) {
               goodPaths.insert(path);
            }
            else {
               logCopyError(this->perun2, path);
            }
         }

         if (!goodPaths.empty()) {
            if (os_copy(goodPaths)) {
               for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                  logCopySuccess(this->perun2, *it);
               }
               this->contexts.success->value = !selectFailure;
            }
            else {
               for (auto it = goodPaths.begin(); it != goodPaths.end(); it++) {
                  logCopyError(this->perun2, *it);
               }
               this->contexts.success->value = false;
            }
         }

         copyPaths.clear();
      }
   }
   else {
      this->contexts.success->value = !selectFailure;
   }
}

inline _bool Aggregate::has(const _uint32 v) const
{
   return value & v;
}

}
