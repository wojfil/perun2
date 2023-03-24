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

#include "cache.h"
#include "perun2.h"
#include "os.h"


namespace perun2
{
   Cache::Cache(_p2& p2)
      : hashes(p2.hashes), context(p2.contexts.globalVars) { };

   void Cache::actualize(const _hash nameHash)
   {
      if (nameHash == this->hashes.HASH_VAR_DESKTOP && this->isNotLoaded(CACHE_DESKTOP_PATH)) {
         this->context.strings[nameHash]->value = os_desktopPath();
      }

      if (nameHash == this->hashes.HASH_VAR_PERUN2 && this->isNotLoaded(CACHE_EXE_PATH)) {
         this->context.strings[nameHash]->value = os_executablePath();
      }
   }

   void Cache::loadExePath()
   {
      if (this->isNotLoaded(CACHE_EXE_PATH)) {
         this->context.strings[this->hashes.HASH_VAR_PERUN2]->value = os_executablePath();
      }
   }

   _bool Cache::isNotLoaded(const _cunit v)
   {
      const _bool notLoaded = !(this->value & v); 
      if (notLoaded) {
         this->value |= v;
      }

      return notLoaded;
   }
}
