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

#include "com-arg.h"
#include "..\var\var-runtime.h"
#include "..\os.h"
#include "..\uroboros.h"



void CS_StringComArg::run()
{
   if (g_running) {
      if (hasMemory) {
         attrMemory.load();
      }

      prevThis = g_this_s.value;
      prevIndex = g_index.value;

      const _str val = string->getValue();
      g_index.value = 0LL;
      g_this_s.value = val;

      if (hasAttribute) {
         os_loadAttributes(*attribute);
      }
      command->run();

      g_this_s.value = prevThis;
      g_index.value = prevIndex;

      if (hasMemory) {
         attrMemory.restore();
      }
   }
}


void CS_ListComArg::run()
{
   const _list values = list->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   if (hasMemory) {
      attrMemory.load();
   }

   prevThis = g_this_s.value;
   prevIndex = g_index.value;

   _size index = 0;
   g_index.value = 0LL;

   while (g_running && index != length) {
      g_this_s.value = values[index];

      if (hasAttribute) {
         os_loadAttributes(*attribute);
      }

      command->run();
      index++;
      g_index.value = _num((_nint)index);
   }

   g_this_s.value = prevThis;
   g_index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}


void CS_DefinitionComArg::run()
{
   if (hasMemory) {
      attrMemory.load();
   }

   prevThis = g_this_s.value;
   prevIndex = g_index.value;

   _size index = 0;
   g_index.value = 0LL;

   while (definition->hasNext()) {
      if (!g_running) {
         definition->reset();
         break;
      }

      g_this_s.value = definition->getValue();

      if (hasAttribute) {
         os_loadAttributes(*attribute);
      }

      command->run();
      index++;
      g_index.value = _num((_nint)index);
   }

   g_this_s.value = prevThis;
   g_index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}

