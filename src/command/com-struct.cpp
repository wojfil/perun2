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

#include "com-struct.h"
#include "..\var\var-runtime.h"
#include "..\os.h"
#include "..\uroboros.h"


void CS_Pair::run()
{
   if (g_running) {
      first->run();
   }
   if (g_running) {
      second->run();
   }
}


void CS_Block::run()
{
   for (_size i = 0; g_running && i < length; i++) {
      (commands[i])->run();
   }
}


void CS_Times::run()
{
   _nint repeats = times->getValue().toInt();
   if (repeats <= 0LL) {
      return;
   }

   const _num prevIndex = g_index.value;
   g_index.value = Number(0LL);

   while (g_running && repeats != 0LL) {
      command->run();
      g_index.value++;
      repeats--;

      if (g_break) {
         g_running = true;
         g_break = false;
         break;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
         continue;
      }
   }

   aggregate->run();
   g_index.value = prevIndex;
}


void CS_While::run()
{
   const _num prevIndex = g_index.value;
   g_index.value = Number(0LL);

   while (g_running && condition->getValue()) {
      command->run();
      g_index.value++;

      if (g_break) {
         g_running = true;
         g_break = false;
         break;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
         continue;
      }
   }

   aggregate->run();
   g_index.value = prevIndex;
}


void CS_TimeLoop::run()
{
   const _tlist values = timeList->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   const _num prevIndex = g_index.value;
   const _tim prevThis = g_this_t.value;

   g_index.value = Number(0LL);
   _size index = 0;

   while (g_running && index != length) {
      g_this_t.value = values[index];
      command->run();
      g_index.value++;
      index++;

      if (g_break) {
         g_running = true;
         g_break = false;
         break;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
         continue;
      }
   }

   aggregate->run();
   g_index.value = prevIndex;
   g_this_t.value = prevThis;
}


void CS_NumberLoop::run()
{
   const _nlist values = numberList->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   const _num prevIndex = g_index.value;
   const _num prevThis = g_this_n.value;

   g_index.value = Number(0LL);

   _size index = 0;
   while (g_running && index != length) {
      g_this_n.value = values[index];
      command->run();
      g_index.value++;
      index++;

      if (g_break) {
         g_running = true;
         g_break = false;
         break;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
         continue;
      }
   }

   aggregate->run();
   g_index.value = prevIndex;
   g_this_n.value = prevThis;
}


void CS_StringLoop::run()
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

      if (g_break) {
         g_running = true;
         g_break = false;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
      }

      aggregate->run();

      g_this_s.value = prevThis;
      g_index.value = prevIndex;

      if (hasMemory) {
         attrMemory.restore();
      }
   }
}


void CS_DefinitionLoop::run()
{
   if (hasMemory) {
      attrMemory.load();
   }

   prevThis = g_this_s.value;
   prevIndex = g_index.value;

   _nint index = 0LL;
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

      if (g_break) {
         g_running = true;
         g_break = false;
         definition->reset();
         break;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
      }

      index++;
      g_index.value = _num(index);
   }

   aggregate->run();

   g_this_s.value = prevThis;
   g_index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}


void CS_ListLoop::run()
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

      if (g_break) {
         g_running = true;
         g_break = false;
         break;
      }
      else if (g_continue) {
         g_running = true;
         g_continue = false;
         continue;
      }
   }

   aggregate->run();

   g_this_s.value = prevThis;
   g_index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}


void CS_InsideString::run()
{
   if (g_running) {
      if (hasMemory) {
         attrMemory.load();
      }

      const _str val = os_trim(string->getValue());
      const _str newLocation = os_join(g_location.value, val);

      if (!val.empty() && os_directoryExists(newLocation)) {
         prevThis = g_this_s.value;
         prevIndex = g_index.value;
         prevLocation = g_location.value;

         g_this_s.value = newLocation;
         g_index.value = _num(0LL);
         g_location.value = newLocation;

         if (hasAttribute) {
            os_loadAttributes(*attribute);
         }
         command->run();

         if (g_break) {
            g_running = true;
            g_break = false;
         }
         else if (g_continue) {
            g_running = true;
            g_continue = false;
         }

         aggregate->run();

         g_location.value = prevLocation;
         g_this_s.value = prevThis;
         g_index.value = prevIndex;
      }

      if (hasMemory) {
         attrMemory.restore();
      }
   }
}


void CS_InsideDefinition::run()
{
   if (hasMemory) {
      attrMemory.load();
   }

   prevThis = g_this_s.value;
   prevIndex = g_index.value;
   prevLocation = g_location.value;

   _nint index = 0LL;
   g_index.value = 0LL;

   while (definition->hasNext()) {
      if (!g_running) {
         definition->reset();
         break;
      }

      const _str newLocation = os_join(prevLocation, definition->getValue());

      if (os_directoryExists(newLocation)) {
         g_location.value = newLocation;
         g_this_s.value = newLocation;

         if (hasAttribute) {
            os_loadAttributes(*attribute);
         }
         command->run();

         if (g_break) {
            g_running = true;
            g_break = false;
            definition->reset();
            break;
         }
         else if (g_continue) {
            g_running = true;
            g_continue = false;
         }

         index++;
         g_index.value = _num(index);
      }

      g_location.value = prevLocation;
   }

   aggregate->run();

   g_location.value = prevLocation;
   g_this_s.value = prevThis;
   g_index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}


void CS_InsideList::run()
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
   prevLocation = g_location.value;

   _size index = 0;
   _nint outIndex = 0LL;
   g_index.value = 0LL;

   while (g_running && index != length) {
      const _str v = os_trim(values[index]);
      const _str newLocation = os_join(prevLocation, v);
      if (!v.empty() && os_directoryExists(newLocation)) {
         g_this_s.value = newLocation;
         g_location.value = newLocation;

         if (hasAttribute) {
            os_loadAttributes(*attribute);
         }

         command->run();

         if (g_break) {
            g_running = true;
            g_break = false;
            break;
         }
         else if (g_continue) {
            g_running = true;
            g_continue = false;
         }

         outIndex++;
         g_index.value = _num(outIndex);
      }

      index++;
   }

   aggregate->run();

   g_this_s.value = prevThis;
   g_index.value = prevIndex;
   g_location.value = prevLocation;

   if (hasMemory) {
      attrMemory.restore();
   }
}
