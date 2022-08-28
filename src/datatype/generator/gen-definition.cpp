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

#include "gen-definition.h"
#include "../../os.h"
#include "../../uroboros.h"


DefFilter::DefFilter(_def* def, Uroboros* uro)
   : first(true), definition(def), uroboros(uro) { };

DefFilter::~DefFilter()
{
   delete definition;
}

_fdata* DefFilter::getDataPtr()
{
   return this->definition->getDataPtr();
}

void DefFilter::reset() {
   if (!first) {
      first = true;
      definition->reset();
   }
}


void Filter_WhereDef::reset() {
   if (!first) {
      first = true;
      if (!finished) {
         definition->reset();
      }

      if (hasMemory) {
         attrMemory.restore();
      }
   }
}


_boo Filter_WhereDef::hasNext()
{
   if (first) {
      finished = false;
      first = false;
      if (hasMemory) {
         attrMemory.load();
      }
      index.setToZero();
   }

   while (definition->hasNext()) {
      if (!this->uroboros->running) {
         definition->reset();
         break;
      }

      value = definition->getValue();

      if (this->hasAttribute) {
         this->attribute->run();
      }

      const _boo con = condition->getValue();

      if (con) {
         this->inner->index.value = index;
         index++;
         return true;
      }
   }

   finished = true;
   reset();
   return false;
}


_boo Filter_LimitDef::hasNext()
{
   if (first) {
      limit = number->getValue().toInt();
      if (limit <= 0) {
         return false;
      }

      counter = 0;
      first = false;
   }

   while (definition->hasNext()) {
      if (!this->uroboros->running || counter >= limit) {
         definition->reset();
         break;
      }
      value = definition->getValue();
      counter++;
      return true;
   }

   first = true;
   return false;
};


_boo Filter_SkipDef::hasNext()
{
   if (first) {
      limit = number->getValue().toInt();
      if (limit < 0) {
         limit = 0;
      }

      counter = 0;
      first = false;
   }

   while (definition->hasNext()) {
      if (!this->uroboros->running) {
         definition->reset();
      }

      if (counter == limit) {
         this->inner->index.value -= limit;
         value = definition->getValue();
         return true;
      }
      else {
         counter++;
      }
   }

   first = true;
   return false;
};


_boo Filter_EveryDef::hasNext()
{
   if (first) {
      limit = number->getValue().toInt();
      if (limit < 1) {
         limit = 1;
      }

      counter = limit;
      first = false;
      index.setToZero();
   }

   while (definition->hasNext()) {
      if (!this->uroboros->running) {
         definition->reset();
      }

      if (counter == limit) {
         counter = 1;
         value = definition->getValue();
         this->inner->index.value = index;
         index++;
         return true;
      }
      else {
         counter++;
      }
   }

   first = true;
   return false;
};


Join_DefStr::~Join_DefStr()
{
   delete left;
   delete right;
}

void Join_DefStr::reset()
{
   if (taken) {
      taken = false;
   }
   else {
      left->reset();
   }
}

_boo Join_DefStr::hasNext()
{
   if (!this->uroboros->running) {
      reset();
      return false;
   }

   if (taken) {
      taken = false;
      return false;
   }

   if (left->hasNext()) {
      value = left->getValue();
      return true;
   }

   value = right->getValue();
   taken = true;
   return true;
}


Join_StrDef::~Join_StrDef()
{
   delete left;
   delete right;
}


void Join_StrDef::reset()
{
   if (!first) {
      first = true;
      right->reset();
   }
}


_boo Join_StrDef::hasNext()
{
   if (!this->uroboros->running) {
      reset();
      return false;
   }

   if (first) {
      value = left->getValue();
      first = false;
      return true;
   }

   if (right->hasNext()) {
      value = right->getValue();
      return true;
   }

   first = true;
   return false;
}


Join_DefList::~Join_DefList()
{
   delete left;
   delete right;
}


void Join_DefList::reset()
{
   if (taken) {
      taken = false;
   }
   else {
      left->reset();
   }
}


_boo Join_DefList::hasNext()
{
   if (!this->uroboros->running) {
      reset();
      return false;
   }

   if (!taken) {
      if (left->hasNext()) {
         value = left->getValue();
         return true;
      }
      else {
         values = right->getValue();
         length = values.size();
         index = 0;
         taken = true;
      }
   }

   if (index != length) {
      value = values[index];
      index++;
      return true;
   }
   else {
      taken = false;
      return false;
   }
}


Join_ListDef::~Join_ListDef()
{
   delete left;
   delete right;
}


void Join_ListDef::reset()
{
   if (taken) {
      right->reset();
      taken = false;
   }
   first = true;
}


_boo Join_ListDef::hasNext()
{
   if (!this->uroboros->running) {
      reset();
      return false;
   }

   if (first) {
      values = left->getValue();
      length = values.size();
      index = 0;
      first = false;
   }

   if (!taken) {
      if (index == length) {
         taken = true;
      }
      else {
         value = values[index];
         index++;
         return true;
      }
   }

   if (right->hasNext()) {
      value = right->getValue();
      return true;
   }
   else {
      first = true;
      taken = false;
      return false;
   }
}


Join_DefDef::~Join_DefDef()
{
   delete left;
   delete right;
}


void Join_DefDef::reset()
{
   if (!first) {
      if (taken) {
         right->reset();
         taken = false;
      }
      else {
         left->reset();
      }
      first = true;
   }
}


_boo Join_DefDef::hasNext()
{
   if (!this->uroboros->running) {
      reset();
      return false;
   }

   if (first) {
      first = false;
   }

   if (!taken) {
      if (left->hasNext()) {
         value = left->getValue();
         return true;
      }
      else {
         taken = true;
      }
   }

   if (taken) {
      if (right->hasNext()) {
         value = right->getValue();
         return true;
      }
      else {
         first = true;
         taken = false;
         return false;
      }
   }
}


void OrderByCast::reset()
{
   if (!this->first) {
      this->first = true;
   }
}


_boo OrderByCast::hasNext()
{
   if (this->first) {
      if (this->hasMemory) {
         this->attrMemory.load();
      }

      this->prevIndex = this->inner->index.value;
      this->prevThis = this->inner->this_s.value;

      this->first = false;
      this->values = this->base->getValue();
      this->index = 0;
      this->length = this->values.size();
      this->hasVolatileDepth = this->obase->hasVolatileDepth();
   }

   if (this->uroboros->running && this->index != this->length) {
      this->value = values[index];
      this->inner->this_s.value = this->value;
      this->inner->index.value.value.i = static_cast<_nint>(this->index);
      if (hasVolatileDepth) {
         this->inner->depth.value = this->obase->getDepth(index);
      }
      this->index++;
      return true;
   }

   this->first = true;
   this->inner->index.value = this->prevIndex;
   this->inner->this_s.value = this->prevThis;

   if (this->hasMemory) {
      this->attrMemory.restore();
   }

   return false;
}


void DefTernary::reset()
{
   if (!first) {
      first = true;
      if (isLeft) {
         left->reset();
      }
      else {
         right->reset();
      }
   }
}


_boo DefTernary::hasNext()
{
   if (first) {
      isLeft = condition->getValue();
   }

   if (isLeft) {
      if (left->hasNext()) {
         value = left->getValue();
         return true;
      }
   }
   else {
      if (right->hasNext()) {
         value = right->getValue();
         return true;
      }
   }

   first = true;
   return false;
}


void DefBinary::reset()
{
   if (!first) {
      first = true;
      left->reset();
   }
}


_boo DefBinary::hasNext()
{
   if (first) {
      if (!condition->getValue()) {
         return false;
      }
      first = false;
   }

   if (left->hasNext()) {
      value = left->getValue();
      return true;
   }

   first = true;
   return false;
}

