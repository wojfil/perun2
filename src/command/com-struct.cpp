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


namespace uro::comm
{

void CS_Pair::run()
{
   if (this->uroboros.state == State::s_Running) {
      this->first->run();
   }
   if (this->uroboros.state == State::s_Running) {
      this->second->run();
   }
}


void CS_Block::run()
{
   for (_size i = 0; this->uroboros.state == State::s_Running && i < this->length; i++) {
      (this->commands[i])->run();
   }
}


void CS_Times::run()
{
   _nint repeats = this->times->getValue().toInt();
   if (repeats <= NINT_ZERO) {
      return;
   }

   const _numi prevIndex = this->inner.index.value;
   this->inner.index.value.setToZero();

   while (this->uroboros.state == State::s_Running && repeats != NINT_ZERO) {
      this->command->run();
      this->inner.index.value++;
      repeats--;

      P_CHECK_LOOP_BREAK;
   }

   this->aggregate->run();
   this->inner.index.value = prevIndex;
}


void CS_While::run()
{
   const _numi prevIndex = this->inner.index.value;
   this->inner.index.value.setToZero();

   while (this->uroboros.state == State::s_Running && this->condition->getValue()) {
      this->command->run();
      this->inner.index.value++;

      P_CHECK_LOOP_BREAK;
   }

   this->aggregate->run();
   this->inner.index.value = prevIndex;
}


void CS_TimeLoop::run()
{
   const _tlist values = this->timeList->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   const _numi prevIndex = this->inner.index.value;
   const _tim prevThis = this->inner.this_t.value;

   this->inner.index.value.setToZero();
   _size index = 0;

   while (this->uroboros.state == State::s_Running && index != length) {
      this->inner.this_t.value = values[index];
      this->command->run();
      this->inner.index.value++;
      index++;

      P_CHECK_LOOP_BREAK;
   }

   this->aggregate->run();
   this->inner.index.value = prevIndex;
   this->inner.this_t.value = prevThis;
}


void CS_NumberLoop::run()
{
   const _nlist values = this->numberList->getValue();
   const _size length = values.size();

   if (length == 0) {
      return;
   }

   const _numi prevIndex = this->inner.index.value;
   const _num prevThis = this->inner.this_n.value;

   this->inner.index.value.setToZero();

   _size index = 0;
   while (this->uroboros.state == State::s_Running && index != length) {
      this->inner.this_n.value = values[index];
      this->command->run();
      this->inner.index.value++;
      index++;

      P_CHECK_LOOP_BREAK;
   }

   this->aggregate->run();
   this->inner.index.value = prevIndex;
   this->inner.this_n.value = prevThis;
}


void CS_StringLoop::run()
{
   if (this->uroboros.state == State::s_Running) {
      if (this->hasMemory) {
         this->attrMemory.load();
      }

      P_MEMORY_LOAD;

      const _str val = this->string->getValue();
      this->inner.this_s.value = val;
      this->inner.index.value.setToZero();
      this->inner.depth.value.setToZero();

      if (this->hasAttribute) {
         this->attribute->run();
      }
      this->command->run();

      P_CHECK_SOFT_LOOP_BREAK;

      this->aggregate->run();

      P_MEMORY_RESTORE;

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

   P_MEMORY_LOAD;

   _numi index = NINT_ZERO;
   this->inner.index.value.setToZero();

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      this->inner.this_s.value = this->definition->getValue();

      if (this->hasAttribute) {
         this->attribute->run();
      }

      this->command->run();

      P_CHECK_LOOP_BREAK;

      index++;
      this->inner.index.value = index;
   }

   this->aggregate->run();

   P_MEMORY_RESTORE;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}


void CS_ListLoop::run()
{
   const _list values = this->list->getValue();
   const _numi length = _numi(static_cast<_nint>(values.size()));

   if (length == 0) {
      return;
   }

   if (this->hasMemory) {
      this->attrMemory.load();
   }

   P_MEMORY_LOAD;

   _numi index = NINT_ZERO;
   this->inner.index.value.setToZero();
   this->inner.depth.value.setToZero();

   while (this->uroboros.state == State::s_Running && index != length) {
      this->inner.this_s.value = values[index.value.i];

      if (this->hasAttribute) {
         this->attribute->run();
      }

      this->command->run();
      index++;
      this->inner.index.value = index;

      P_CHECK_LOOP_BREAK;
   }

   this->aggregate->run();

   P_MEMORY_RESTORE;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}


void CS_InsideString::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str val = os_trim(this->string->getValue());
      const _str newLocation = os_join(this->inner.location.value, val);

      if (!val.empty() && os_directoryExists(newLocation)) {
         if (this->hasMemory) {
            this->attrMemory.load();
         }

         P_MEMORY_LOAD;
         this->prevLocation = this->inner.location.value;

         this->inner.this_s.value = newLocation;
         this->inner.index.value.setToZero();
         this->inner.depth.value.setToZero();
         this->inner.location.value = newLocation;

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->command->run();
         this->aggregate->run();

         P_CHECK_SOFT_LOOP_BREAK;

         P_MEMORY_RESTORE;
         this->inner.location.value = this->prevLocation;

         if (this->hasMemory) {
            this->attrMemory.restore();
         }
      }

   }
}


void CS_InsideDefinition::run()
{
   if (this->hasMemory) {
      this->attrMemory.load();
   }

   P_MEMORY_LOAD;
   this->prevLocation = this->inner.location.value;

   _numi index = NINT_ZERO;
   this->inner.index.value.setToZero();

   while (definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      const _str newLocation = os_join(this->prevLocation, this->definition->getValue());

      if (os_directoryExists(newLocation)) {
         this->inner.location.value = newLocation;
         this->inner.this_s.value = newLocation;

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->command->run();

         P_CHECK_LOOP_BREAK;

         index++;
         this->inner.index.value = index;
      }

      this->inner.location.value = this->prevLocation;
   }

   this->aggregate->run();

   P_MEMORY_RESTORE;
   this->inner.location.value = this->prevLocation;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}


void CS_InsideList::run()
{
   const _list values = this->list->getValue();
   const _numi length = _numi(static_cast<_nint>(values.size()));

   if (length.value.i == 0) {
      return;
   }

   if (this->hasMemory) {
      this->attrMemory.load();
   }

   P_MEMORY_LOAD;
   this->prevLocation = this->inner.location.value;

   _numi index = NINT_ZERO;
   _nint outIndex = NINT_ZERO;
   this->inner.index.value.setToZero();
   this->inner.depth.value.setToZero();

   while (this->uroboros.state == State::s_Running && index != length) {
      const _str v = os_trim(values[index.value.i]);
      const _str newLocation = os_join(this->prevLocation, v);
      if (!v.empty() && os_directoryExists(newLocation)) {
         this->inner.this_s.value = newLocation;
         this->inner.location.value = newLocation;

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->command->run();

         P_CHECK_LOOP_BREAK;

         outIndex++;
         this->inner.index.value = outIndex;
      }

      index++;
   }

   this->aggregate->run();

   P_MEMORY_RESTORE;
   this->inner.location.value = this->prevLocation;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }
}

}
