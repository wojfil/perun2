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
#include "../../print.h"


namespace uro::gen
{

DefFilter::DefFilter(_defptr& def, Uroboros& uro)
   : first(true), definition(std::move(def)), uroboros(uro) { };


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


Filter_WhereDef::Filter_WhereDef(_defptr& def, _genptr<_bool>& cond, _attrptr& attr, const _bool& hasMem, Uroboros& uro)
   : DefFilter(def, uro), condition(std::move(cond)), attribute(std::move(attr)), finished(true), inner(uro.vars.inner),
      hasMemory(hasMem), attrMemory(AttributeMemory(attr, uro.vars.inner)), hasAttribute(true) { };

Filter_WhereDef::Filter_WhereDef(_defptr& def, _genptr<_bool>& cond, Uroboros& uro)
   : DefFilter(def, uro), condition(std::move(cond)), finished(true), inner(uro.vars.inner),
      hasMemory(false), attrMemory(AttributeMemory(uro.vars.inner)), hasAttribute(false) { };


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


_bool Filter_WhereDef::hasNext()
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
      if (this->uroboros.state != State::s_Running) {
         break;
      }

      value = definition->getValue();

      if (this->hasAttribute) {
         this->attribute->run();
      }

      const _bool con = this->condition->getValue();

      if (con) {
         this->inner.index.value = index;
         index++;
         return true;
      }
   }

   finished = true;
   reset();
   return false;
}


DefinitionChain::DefinitionChain(_defptr& def, Uroboros& uro)
   : definition(std::move(def)), inner(uro.vars.inner) { };


void DefinitionChain::reset()
{
   if (!this->finished) {
      this->finished = true;
      this->index = NINT_ZERO;
      this->definition->reset();
   }
};


_bool DefinitionChain::hasNext()
{
   if (this->definition->hasNext()) {
      this->finished = false;
      this->value = this->definition->getValue();
      this->inner.index.value.value.i = this->index;
      this->index++;
      return true;
   }

   this->finished = true;
   this->index = NINT_ZERO;
   return false;
};


LocationVessel::LocationVessel(const _bool& abs, Uroboros& uro)
   : isAbsolute(abs), inner(uro.vars.inner) { };


_str LocationVessel::getValue()
{
   return this->isAbsolute
      ? this->value
      : str(this->inner.location.value, OS_SEPARATOR_STRING, this->value);
};

const _str& LocationVessel::getRawValue() const
{
   return this->value;
};


void LocationVessel::setValue(const _str& val)
{
   this->value = val;
};


NestedDefiniton::NestedDefiniton(LocationVessel& ves, _defptr& def, _defptr& locs, Uroboros& uro, const _bool& abs, const _bool& fin)
   : vessel(ves), definition(std::move(def)), locations(std::move(locs)), inner(uro.vars.inner), isAbsolute(abs), isFinal(fin) { };


void NestedDefiniton::reset()
{
   if (this->defOpened) {
      this->defOpened = false;
      this->definition->reset();
   }
   if (this->locsOpened) {
      this->locsOpened = false;
      this->locations->reset();
   }
};


_bool NestedDefiniton::hasNext()
{
   if (!this->locsOpened) {
      if (this->locations->hasNext()) {
         this->locDepth = this->inner.depth.value;
         this->locsOpened = true;
         this->vessel.setValue(this->locations->getValue());
         if (this->isFinal) {
            this->index.setToZero();
         }
      }
      else {
         return false;
      }
   }

   while (true) {
      if (this->definition->hasNext()) {
         this->defOpened = true;

         if (this->isAbsolute) {
            this->value = this->definition->getValue();
         }
         else {
            this->value = this->vessel.getRawValue() == STRING_DOT
               ? this->definition->getValue()
               : str(this->vessel.getRawValue(), OS_SEPARATOR_STRING, this->definition->getValue());
         }
         //this->value = this->isAbsolute
          //  ? this->definition->getValue()
         //   : str(this->vessel.getRawValue(), OS_SEPARATOR_STRING, this->definition->getValue());

         if (this->isFinal) {
            this->inner.index.value = index;
            index++;
         }

         this->inner.depth.value = this->locDepth;
         return true;
      }
      else {
         this->defOpened = false;

         if (this->locations->hasNext()) {
            this->locDepth = this->inner.depth.value;
            this->locsOpened = true;
            this->vessel.setValue(this->locations->getValue());
         }
         else {
            this->locsOpened = false;
            return false;
         }
      }
   }
   
   return false;
};

_fdata* NestedDefiniton::getDataPtr()
{
   return this->definition->getDataPtr();
}

_bool Filter_LimitDef::hasNext()
{
   if (first) {
      limit = this->number->getValue().toInt();
      if (limit <= NINT_ZERO) {
         return false;
      }

      counter = NINT_ZERO;
      first = false;
   }

   while (definition->hasNext()) {
      if (this->uroboros.state != State::s_Running || counter >= limit) {
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


_bool Filter_SkipDef::hasNext()
{
   if (first) {
      limit = this->number->getValue().toInt();
      if (limit < NINT_ZERO) {
         limit = NINT_ZERO;
      }

      counter = NINT_ZERO;
      first = false;
   }

   while (definition->hasNext()) {
      if (this->uroboros.state != State::s_Running) {
         definition->reset();
      }

      if (counter == limit) {
         this->inner.index.value -= limit;
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


_bool Filter_EveryDef::hasNext()
{
   if (first) {
      limit = this->number->getValue().toInt();
      if (limit < NINT_ONE) {
         limit = NINT_ONE;
      }

      counter = limit;
      first = false;
      index.setToZero();
   }

   while (definition->hasNext()) {
      if (this->uroboros.state != State::s_Running) {
         definition->reset();
      }

      if (counter == limit) {
         counter = NINT_ONE;
         value = definition->getValue();
         this->inner.index.value = index;
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


_bool Filter_FinalDef::hasNext()
{
   if (first) {
      const _nint limit = this->number->getValue().toInt();
      if (limit < NINT_ONE) {
         return false;
      }

      values.clear();
      length = NINT_ZERO;
      
      while (definition->hasNext()) {
         if (this->uroboros.state != State::s_Running) {
            definition->reset();
            return false;
         }

         if (length == limit) {
            values.pop_front();
         }
         else {
            length++;
         }
         values.emplace_back(definition->getValue());
      }

      index = NINT_ZERO;
      first = false;
   }

   if (this->uroboros.state != State::s_Running) {
      first = true;
      return false;
   }

   if (index < length) {
      value = values[static_cast<_size>(index)];
      this->inner.index.value.value.i = index;
      index++;
      return true;
   }

   first = true;
   return false;
};

void Join_DefStr::reset()
{
   if (taken) {
      taken = false;
   }
   else {
      this->left->reset();
   }
}

_bool Join_DefStr::hasNext()
{
   if (this->uroboros.state != State::s_Running) {
      reset();
      return false;
   }

   if (taken) {
      taken = false;
      return false;
   }

   if (this->left->hasNext()) {
      value = this->left->getValue();
      return true;
   }

   value = this->right->getValue();
   taken = true;
   return true;
}


void Join_StrDef::reset()
{
   if (!first) {
      first = true;
      this->right->reset();
   }
}


_bool Join_StrDef::hasNext()
{
   if (this->uroboros.state != State::s_Running) {
      reset();
      return false;
   }

   if (first) {
      value = this->left->getValue();
      first = false;
      return true;
   }

   if (this->right->hasNext()) {
      value = this->right->getValue();
      return true;
   }

   first = true;
   return false;
}


void Join_DefList::reset()
{
   if (taken) {
      taken = false;
   }
   else {
      this->left->reset();
   }
}


_bool Join_DefList::hasNext()
{
   if (this->uroboros.state != State::s_Running) {
      reset();
      return false;
   }

   if (!taken) {
      if (this->left->hasNext()) {
         value = this->left->getValue();
         return true;
      }
      else {
         values = this->right->getValue();
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


void Join_ListDef::reset()
{
   if (taken) {
      this->right->reset();
      taken = false;
   }
   first = true;
}


_bool Join_ListDef::hasNext()
{
   if (this->uroboros.state != State::s_Running) {
      reset();
      return false;
   }

   if (first) {
      values = this->left->getValue();
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

   if (this->right->hasNext()) {
      value = this->right->getValue();
      return true;
   }
   else {
      first = true;
      taken = false;
      return false;
   }
}


void Join_DefDef::reset()
{
   if (!first) {
      if (taken) {
         this->right->reset();
         taken = false;
      }
      else {
         this->left->reset();
      }
      first = true;
   }
}


_bool Join_DefDef::hasNext()
{
   if (this->uroboros.state != State::s_Running) {
      reset();
      return false;
   }

   if (first) {
      first = false;
   }

   if (!taken) {
      if (this->left->hasNext()) {
         value = this->left->getValue();
         return true;
      }
      else {
         taken = true;
      }
   }

   if (taken) {
      if (this->right->hasNext()) {
         value = this->right->getValue();
         return true;
      }
      else {
         first = true;
         taken = false;
         return false;
      }
   }
}


DefinitionSuffix::DefinitionSuffix(_defptr& def, Uroboros& uro, const _str& suf, const _bool& abs, const _bool& fin)
   : uroboros(uro), inner(uro.vars.inner), definition(std::move(def)), suffix(suf), absoluteBase(abs), isFinal(fin) { };


void DefinitionSuffix::reset()
{
   if (!this->first) {
      this->definition->reset();
      this->first = true;
   }
}


_bool DefinitionSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->isFinal) {
         this->index.setToZero();
      }
   }

   while (definition->hasNext()) {
      if (this->uroboros.state != State::s_Running) {
         this->definition->reset();
         this->first = true;
         break;
      }

      this->value = str(this->definition->getValue(), this->suffix);
      const _str path = this->absoluteBase 
         ? this->value 
         : str(this->inner.location.value, OS_SEPARATOR_STRING, this->value);

      if (this->isFinal ? os_exists(path) : os_directoryExists(path)) {
         if (this->isFinal) {
            this->inner.index.value = index;
            index++;
         }
         return true;
      }
   }

   this->first = true;
   return false;
}


void DefTernary::reset()
{
   if (!first) {
      first = true;
      if (isLeft) {
         this->left->reset();
      }
      else {
         this->right->reset();
      }
   }
}


_bool DefTernary::hasNext()
{
   if (first) {
      isLeft = this->condition->getValue();
   }

   if (isLeft) {
      if (this->left->hasNext()) {
         value = this->left->getValue();
         return true;
      }
   }
   else {
      if (this->right->hasNext()) {
         value = this->right->getValue();
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
      this->left->reset();
   }
}


_bool DefBinary::hasNext()
{
   if (first) {
      if (!this->condition->getValue()) {
         return false;
      }
      first = false;
   }

   if (this->left->hasNext()) {
      value = this->left->getValue();
      return true;
   }

   first = true;
   return false;
}

}
