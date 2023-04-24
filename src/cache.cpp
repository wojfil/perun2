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
   : perun2(p2), context(p2.contexts.globalVars) { };

void Cache::actualize(const _hash nameHash)
{
   if (nameHash == this->perun2.hashes.HASH_VAR_DESKTOP) {
      if (this->isNotLoaded(CACHE_DESKTOP_PATH)) {
         this->context.strings[nameHash]->value = os_desktopPath();
      }
   }
   else if (nameHash == this->perun2.hashes.HASH_VAR_PERUN2) {
      if (this->isNotLoaded(CACHE_EXE_PATH)) {
         this->context.strings[nameHash]->value = os_executablePath();
      }
   }
   else if (nameHash == this->perun2.hashes.HASH_VAR_ALPHABET) {
      if (this->isNotLoaded(CACHE_ALPHABET)) {
         this->context.lists[nameHash]->value = this->getAlphabet();
      }
   }
   else if (nameHash == this->perun2.hashes.HASH_VAR_ASCII) {
      if (this->isNotLoaded(CACHE_ASCII)) {
         this->context.lists[nameHash]->value = STRINGS_ASCII;
      }
   }
   else if (nameHash == this->perun2.hashes.HASH_VAR_ORIGIN) {
      if (this->isNotLoaded(CACHE_ORIGIN)) {
         this->context.strings[nameHash]->value = this->perun2.arguments.getLocation();
      }
   }
   else if (nameHash == this->perun2.hashes.HASH_VAR_ARGUMENTS) {
      if (this->isNotLoaded(CACHE_ARGUMENTS)) {
         this->context.lists[nameHash]->value = this->perun2.arguments.getArgs();
      }
   }
}

void Cache::loadCmdPath()
{
   if (this->isNotLoaded(CACHE_EXE_PATH)) {
      this->context.strings[this->perun2.hashes.HASH_VAR_PERUN2]->value = os_executablePath();
   }

   if (this->isNotLoaded(CACHE_CMD_PROCESS)) {
      this->cmdProcessStartingArgs = this->getCmdProcessStartingArgs();
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

_str Cache::getCmdProcessStartingArgs() const
{
   return str(os_quoteEmbraced(this->context.strings[this->perun2.hashes.HASH_VAR_PERUN2]->value),
      CHAR_SPACE, CHAR_MINUS, CHAR_FLAG_SILENT, CHAR_SPACE);
}

_list Cache::getAlphabet() const
{
   _list a(LETTERS_IN_ENGLISH_ALPHABET);
   for (_int i = 0; i < LETTERS_IN_ENGLISH_ALPHABET; i++) {
      a[i] = CHAR_a + i;
   }
   return a;
}



}
