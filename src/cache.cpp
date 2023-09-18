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

Cache::Cache(p_perun2& p2)
   : perun2(p2), context(p2.contexts.globalVars), programs(p2) 
{ 
   p2.cache.programs.insertVars(p2.contexts);
};

void Cache::actualize(const Token& tk)
{
   if (tk.isWord(STRING_DESKTOP, this->perun2)) {
      if (this->isNotLoaded(CACHE_DESKTOP_PATH)) {
         this->context.strings[STRING_DESKTOP]->value = os_desktopPath();
      }
   }
   else if (tk.isWord(STRING_PERUN2, this->perun2)) {
      if (this->isNotLoaded(CACHE_EXE_PATH)) {
         this->context.strings[STRING_PERUN2]->value = os_executablePath();
      }
   }
   else if (tk.isWord(STRING_ALPHABET, this->perun2)) {
      if (this->isNotLoaded(CACHE_ALPHABET)) {
         this->context.lists[STRING_ALPHABET]->value = this->getAlphabet();
      }
   }
   else if (tk.isWord(STRING_ASCII, this->perun2)) {
      if (this->isNotLoaded(CACHE_ASCII)) {
         this->context.lists[STRING_ASCII]->value = STRINGS_ASCII;
      }
   }
   else if (tk.isWord(STRING_ORIGIN, this->perun2)) {
      if (this->isNotLoaded(CACHE_ORIGIN)) {
         this->context.strings[STRING_ORIGIN]->value = this->perun2.arguments.getLocation();
      }
   }
   else if (tk.isWord(STRING_ARGUMENTS, this->perun2)) {
      if (this->isNotLoaded(CACHE_ARGUMENTS)) {
         this->context.lists[STRING_ARGUMENTS]->value = this->perun2.arguments.getArgs();
      }
   }
   else if (tk.isWord(STRING_PENDRIVE, this->perun2)
         || tk.isWord(STRING_PENDRIVES, this->perun2)) 
   {
      if (this->isNotLoaded(CACHE_PENDRIVES)) {
         const p_list pendrives = os_pendrives();
         if (!pendrives.empty()) {
            this->context.strings[STRING_PENDRIVE]->value = pendrives[0];
            this->context.lists[STRING_PENDRIVES]->value = std::move(pendrives);
         }
      }
   }
   else {
      this->programs.actualize(tk);
   }
}

void Cache::loadCmdPath()
{
   if (this->isNotLoaded(CACHE_EXE_PATH)) {
      this->context.strings[STRING_PERUN2]->value = os_executablePath();
   }

   if (this->isNotLoaded(CACHE_CMD_PROCESS)) {
      this->cmdProcessStartingArgs = this->getCmdProcessStartingArgs();
   }
}

p_bool Cache::isNotLoaded(const p_cunit v)
{
   const p_bool notLoaded = !(this->value & v);
   if (notLoaded) {
      this->value |= v;
   }

   return notLoaded;
}

p_str Cache::getCmdProcessStartingArgs() const
{
   return str(os_quoteEmbraced(this->context.strings[STRING_PERUN2]->value),
      CHAR_SPACE, CHAR_MINUS, CHAR_FLAG_SILENT, CHAR_SPACE);
}

p_list Cache::getAlphabet() const
{
   p_list a(LETTERS_IN_ENGLISH_ALPHABET);
   for (p_int i = 0; i < LETTERS_IN_ENGLISH_ALPHABET; i++) {
      a[i] = CHAR_a + i;
   }
   return a;
}

}
