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
   if (this->uroboros->running) {
      this->first->run();
   }
   if (this->uroboros->running) {
      this->second->run();
   }
}


void CS_Block::run()
{
   for (_size i = 0; this->uroboros->running && i < this->length; i++) {
      (this->commands[i])->run();
   }
}


void CS_Times::run()
{
   _nint repeats = this->times->getValue().toInt();
   if (repeats <= 0LL) {
      return;
   }

   const _num prevIndex = this->inner->index.value;
   this->inner->index.value = _num(0LL);

   while (this->uroboros->running && repeats != 0LL) {
      this->command->run();
      this->inner->index.value++;
      repeats--;

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
         break;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
         continue;
      }
   }

   this->aggregate->run();
   this->inner->index.value = prevIndex;
}


void CS_While::run()
{
   const _num prevIndex = this->inner->index.value;
   this->inner->index.value = _num(0LL);

   while (this->uroboros->running && this->condition->getValue()) {
      this->command->run();
      this->inner->index.value++;

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
         break;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
         continue;
      }
   }

   this->aggregate->run();
   this->inner->index.value = prevIndex;
}


void CS_TimeLoop::run()
{
   const _tlist values = this->timeList->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   const _num prevIndex = this->inner->index.value;
   const _tim prevThis = this->inner->this_t.value;

   this->inner->index.value = _num(0LL);
   _size index = 0;

   while (this->uroboros->running && index != length) {
      this->inner->this_t.value = values[index];
      this->command->run();
      this->inner->index.value++;
      index++;

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
         break;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
         continue;
      }
   }

   this->aggregate->run();
   this->inner->index.value = prevIndex;
   this->inner->this_t.value = prevThis;
}


void CS_NumberLoop::run()
{
   const _nlist values = this->numberList->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   const _num prevIndex = this->inner->index.value;
   const _num prevThis = this->inner->this_n.value;

   this->inner->index.value = _num(0LL);

   _size index = 0;
   while (this->uroboros->running && index != length) {
      this->inner->this_n.value = values[index];
      this->command->run();
      this->inner->index.value++;
      index++;

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
         break;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
         continue;
      }
   }

   this->aggregate->run();
   this->inner->index.value = prevIndex;
   this->inner->this_n.value = prevThis;
}


void CS_StringLoop::run()
{
   if (this->uroboros->running) {
      if (this->hasMemory) {
         this->attrMemory.load();
      }

      this->prevThis = this->inner->this_s.value;
      this->prevIndex = this->inner->index.value;

      const _str val = this->string->getValue();
      this->inner->index.value = 0LL;
      this->inner->this_s.value = val;

      if (this->hasAttribute) {
         this->attribute->run();
      }
      this->command->run();

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
      }

      this->aggregate->run();

      this->inner->this_s.value = this->prevThis;
      this->inner->index.value = this->prevIndex;

      if (this->hasMemory) {
         this->attrMemory.restore();
      }
   }
}


void CS_DefinitionLoop::run()
{
   if (this->hasMemory) {
      this->attrMemory.load();
   }

   this->prevThis = this->inner->this_s.value;
   this->prevIndex = this->inner->index.value;

   _nint index = 0LL;
   this->inner->index.value = 0LL;

   while (this->definition->hasNext()) {
      if (!this->uroboros->running) {
         this->definition->reset();
         break;
      }

      this->inner->this_s.value = this->definition->getValue();

      if (this->hasAttribute) {
         this->attribute->run();
      }

      this->command->run();

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
         this->definition->reset();
         break;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
      }

      index++;
      this->inner->index.value = _num(index);
   }

   this->aggregate->run();

   this->inner->this_s.value = this->prevThis;
   this->inner->index.value = this->prevIndex;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}


void CS_ListLoop::run()
{
   const _list values = this->list->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   if (this->hasMemory) {
      this->attrMemory.load();
   }

   this->prevThis = this->inner->this_s.value;
   this->prevIndex = this->inner->index.value;

   _size index = 0;
   this->inner->index.value = 0LL;

   while (this->uroboros->running && index != length) {
      this->inner->this_s.value = values[index];

      if (this->hasAttribute) {
         this->attribute->run();
      }

      this->command->run();
      index++;
      this->inner->index.value = _num((_nint)index);

      if (this->uroboros->break_) {
         this->uroboros->running = true;
         this->uroboros->break_ = false;
         break;
      }
      else if (this->uroboros->continue_) {
         this->uroboros->running = true;
         this->uroboros->continue_ = false;
         continue;
      }
   }

   this->aggregate->run();

   this->inner->this_s.value = this->prevThis;
   this->inner->index.value =this-> prevIndex;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}


void CS_InsideString::run()
{
   if (this->uroboros->running) {
      if (this->hasMemory) {
         this->attrMemory.load();
      }

      const _str val = os_trim(this->string->getValue());
      const _str newLocation = os_join(this->inner->location.value, val);

      if (!val.empty() && os_directoryExists(newLocation)) {
         this->prevThis = this->inner->this_s.value;
         this->prevIndex = this->inner->index.value;
         this->prevLocation = this->inner->location.value;

         this->inner->this_s.value = newLocation;
         this->inner->index.value = _num(0LL);
         this->inner->location.value = newLocation;

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->command->run();

         if (this->uroboros->break_) {
            this->uroboros->running = true;
            this->uroboros->break_ = false;
         }
         else if (this->uroboros->continue_) {
            this->uroboros->running = true;
            this->uroboros->continue_ = false;
         }

         this->aggregate->run();

         this->inner->location.value = this->prevLocation;
         this->inner->this_s.value = this->prevThis;
         this->inner->index.value = this->prevIndex;
      }

      if (this->hasMemory) {
         this->attrMemory.restore();
      }
   }
}


void CS_InsideDefinition::run()
{
   if (this->hasMemory) {
      this->attrMemory.load();
   }

   this->prevThis = this->inner->this_s.value;
   this->prevIndex = this->inner->index.value;
   this->prevLocation = this->inner->location.value;

   _nint index = 0LL;
   this->inner->index.value = 0LL;

   while (definition->hasNext()) {
      if (!this->uroboros->running) {
         this->definition->reset();
         break;
      }

      const _str newLocation = 
         os_join(this->prevLocation, this->definition->getValue());

      if (os_directoryExists(newLocation)) {
         this->inner->location.value = newLocation;
         this->inner->this_s.value = newLocation;

         if (this->hasAttribute) {
            this->attribute->run();
         }
         
         this->command->run();

         if (this->uroboros->break_) {
            this->uroboros->running = true;
            this->uroboros->break_ = false;
            this->definition->reset();
            break;
         }
         else if (this->uroboros->continue_) {
            this->uroboros->running = true;
            this->uroboros->continue_ = false;
         }

         index++;
         this->inner->index.value = _num(index);
      }

      this->inner->location.value = this->prevLocation;
   }

   this->aggregate->run();

   this->inner->location.value = this->prevLocation;
   this->inner->this_s.value = this->prevThis;
   this->inner->index.value = this->prevIndex;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}


void CS_InsideList::run()
{
   const _list values = this->list->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   if (this->hasMemory) {
      this->attrMemory.load();
   }

   this->prevThis = this->inner->this_s.value;
   this->prevIndex = this->inner->index.value;
   this->prevLocation = this->inner->location.value;

   _size index = 0;
   _nint outIndex = 0LL;
   this->inner->index.value = 0LL;

   while (this->uroboros->running && index != length) {
      const _str v = os_trim(values[index]);
      const _str newLocation = os_join(this->prevLocation, v);
      if (!v.empty() && os_directoryExists(newLocation)) {
         this->inner->this_s.value = newLocation;
         this->inner->location.value = newLocation;

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->command->run();

         if (this->uroboros->break_) {
            this->uroboros->running = true;
            this->uroboros->break_ = false;
            break;
         }
         else if (this->uroboros->continue_) {
            this->uroboros->running = true;
            this->uroboros->continue_ = false;
         }

         outIndex++;
         this->inner->index.value = _num(outIndex);
      }

      index++;
   }

   this->aggregate->run();

   this->inner->this_s.value = this->prevThis;
   this->inner->index.value = this->prevIndex;
   this->inner->location.value = this->prevLocation;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}
