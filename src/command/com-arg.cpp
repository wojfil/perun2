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
   if (this->uroboros->running) {
      if (hasMemory) {
         attrMemory.load();
      }

      prevThis = this->inner->this_s.value;
      prevIndex = this->inner->index.value;

      const _str val = string->getValue();
      this->inner->index.value = 0LL;
      this->inner->this_s.value = val;

      if (hasAttribute) {
         os_loadAttributes(*attribute);
      }
      command->run();

      this->inner->this_s.value = prevThis;
      this->inner->index.value = prevIndex;

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

   prevThis = this->inner->this_s.value;
   prevIndex = this->inner->index.value;

   _size index = 0;
   this->inner->index.value = 0LL;

   while (this->uroboros->running && index != length) {
      this->inner->this_s.value = values[index];

      if (hasAttribute) {
         os_loadAttributes(*attribute);
      }

      command->run();
      index++;
      this->inner->index.value = _num((_nint)index);
   }

   this->inner->this_s.value = prevThis;
   this->inner->index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}


void CS_DefinitionComArg::run()
{
   if (hasMemory) {
      attrMemory.load();
   }

   prevThis = this->inner->this_s.value;
   prevIndex = this->inner->index.value;

   _nint index = 0LL;
   this->inner->index.value = 0LL;

   while (definition->hasNext()) {
      if (!this->uroboros->running) {
         definition->reset();
         break;
      }

      this->inner->this_s.value = definition->getValue();

      if (hasAttribute) {
         os_loadAttributes(*attribute);
      }

      command->run();
      index++;
      this->inner->index.value = _num(index);
   }

   this->inner->this_s.value = prevThis;
   this->inner->index.value = prevIndex;

   if (hasMemory) {
      attrMemory.restore();
   }
}
