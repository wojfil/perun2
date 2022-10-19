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


namespace uro::comm
{

void CS_StringComArg::run()
{
   if (this->uroboros.state == State::s_Running) {
      if (hasMemory) {
         attrMemory.load();
      }

      P_MEMORY_LOAD;

      const _str val = string->getValue();
      this->inner.this_s.value = val;
      this->inner.index.value.setToZero();
      this->inner.depth.value.setToZero();

      if (hasAttribute) {
         this->attribute->run();
      }

      command->run();

      P_MEMORY_RESTORE;

      if (hasMemory) {
         attrMemory.restore();
      }
   }
}


void CS_ListComArg::run()
{
   const _list values = list->getValue();
   const _numi length = _numi(static_cast<_nint>(values.size()));

   if (length.value.i == 0LL) {
      return;
   }

   if (hasMemory) {
      attrMemory.load();
   }

   P_MEMORY_LOAD;

   _numi index(0LL);
   this->inner.index.value.setToZero();
   this->inner.depth.value.setToZero();

   while (this->uroboros.state == State::s_Running && index != length) {
      this->inner.this_s.value = values[index.value.i];

      if (hasAttribute) {
         this->attribute->run();
      }

      command->run();
      index++;
      this->inner.index.value = index;
   }

   P_MEMORY_RESTORE;

   if (hasMemory) {
      attrMemory.restore();
   }
}


void CS_DefinitionComArg::run()
{
   if (hasMemory) {
      attrMemory.load();
   }

   P_MEMORY_LOAD;

   _numi index;
   this->inner.index.value.setToZero();

   while (definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         definition->reset();
         break;
      }

      this->inner.this_s.value = definition->getValue();

      if (hasAttribute) {
         this->attribute->run();
      }

      command->run();
      index++;
      this->inner.index.value = index;
   }

   P_MEMORY_RESTORE;

   if (hasMemory) {
      attrMemory.restore();
   }
}

}
