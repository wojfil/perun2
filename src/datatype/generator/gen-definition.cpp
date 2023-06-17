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

#include "gen-definition.h"
#include "../../os.h"
#include "../../perun2.h"


namespace perun2::gen
{


DefWithContext::DefWithContext(_defptr& def, _p2& p2)
   : definition(std::move(def)), context(std::make_unique<FileContext>(p2)) { };

DefWithContext::DefWithContext(_defptr& def, _fcptr& ctx)
   : definition(std::move(def)), context(std::move(ctx)) { };

void DefWithContext::reset()
{
   this->definition->reset();
}

_bool DefWithContext::hasNext()
{
   if (this->definition->hasNext()) {
      this->value = this->definition->getValue();
      this->context->loadData(this->value);
      return true;
   }

   return false;
}

FileContext* DefWithContext::getFileContext()
{
   return this->context.get();
}


DefFilter::DefFilter(_defptr& def, FileContext* ctx, _p2& p2)
   : first(true), definition(std::move(def)), perun2(p2), context(ctx) { };

FileContext* DefFilter::getFileContext()
{
   return this->definition->getFileContext();
}

void DefFilter::reset() {
   if (!first) {
      first = true;
      definition->reset();
   }
}


DefFilter_Where::DefFilter_Where(_genptr<_bool>& cond, _defptr& def, FileContext* ctx, _p2& p2)
   : DefFilter(def, ctx, p2), condition(std::move(cond)) { };


void DefFilter_Where::reset() {
   if (!first) {
      first = true;
      if (!finished) {
         definition->reset();
      }
   }
}


_bool DefFilter_Where::hasNext()
{
   if (first) {
      finished = false;
      first = false;
      index.setToZero();
   }

   while (definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         break;
      }

      value = definition->getValue();
      const _bool con = this->condition->getValue();

      if (con) {
         this->context->index->value = index;
         index++;
         return true;
      }
   }

   finished = true;
   reset();
   return false;
}


LocationVessel::LocationVessel(const _bool abs, _genptr<_str>& loc)
   : isAbsolute(abs), location(std::move(loc)) { };


_str LocationVessel::getValue()
{
   if (this->isAbsolute) {
      return this->value;
   }

   return str(this->location->getValue(), OS_SEPARATOR, this->value);
};

const _str& LocationVessel::getRawValue() const
{
   return this->value;
};


void LocationVessel::setValue(const _str& val)
{
   this->value = val;
};


NestedDefiniton::NestedDefiniton(LocationVessel& ves, _defptr& def, _defptr& locs, const _bool abs, const _bool fin, const _int retr)
   : vessel(ves), definition(std::move(def)), locations(std::move(locs)),
     context(definition->getFileContext()), isAbsolute(abs), isFinal(fin), retreats(retr) { };


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
            const _bool isDot = this->vessel.getRawValue().size() == 1 && this->vessel.getRawValue()[0] == CHAR_DOT;

            this->value = isDot
               ? this->definition->getValue()
               : str(this->vessel.getRawValue(), OS_SEPARATOR, this->definition->getValue());
         }

         if (this->isFinal) {
            if (this->retreats > 0) {
               this->value = str(os_retreats(this->retreats), this->value);
            }

            this->context->index->value = index;
            this->context->loadData(this->value);
            index++;
         }

         return true;
      }
      else {
         this->defOpened = false;

         if (this->locations->hasNext()) {
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


FileContext* NestedDefiniton::getFileContext()
{
   return this->definition->getFileContext();
}


_bool DefFilter_Limit::hasNext()
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
      if (this->perun2.isNotRunning() || counter >= limit) {
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


_bool DefFilter_Skip::hasNext()
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
      if (this->perun2.isNotRunning()) {
         definition->reset();
      }

      if (counter == limit) {
         this->context->index->value -= limit;
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


_bool DefFilter_Every::hasNext()
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
      if (this->perun2.isNotRunning()) {
         definition->reset();
      }

      if (counter == limit) {
         counter = NINT_ONE;
         value = definition->getValue();
         this->context->index->value = index;
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


FileContext* DefFilter_Final::getFileContext()
{
   return this->nextContext.get();
}


_bool DefFilter_Final::hasNext()
{
   if (first) {
      const _nint limit = this->number->getValue().toInt();
      if (limit < NINT_ONE) {
         return false;
      }

      values.clear();
      length = NINT_ZERO;

      while (definition->hasNext()) {
         if (this->perun2.isNotRunning()) {
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

   if (this->perun2.isNotRunning()) {
      first = true;
      return false;
   }

   if (index < length) {
      value = values[static_cast<_size>(index)];
      nextContext->loadData(value);
      this->context->index->value.value.i = index;
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
   if (this->perun2.isNotRunning()) {
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
   if (this->perun2.isNotRunning()) {
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
   if (this->perun2.isNotRunning()) {
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
   if (this->perun2.isNotRunning()) {
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
   if (this->perun2.isNotRunning()) {
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

   this->first = true;
   return false;
}



DefinitionSuffix::DefinitionSuffix(_defptr& def, const _str& suf, const _bool fin)
   : definition(std::move(def)), fileContext(definition->getFileContext()), suffix(suf), isFinal(fin) { };


void DefinitionSuffix::reset()
{
   if (!this->first) {
      this->definition->reset();
      this->first = true;
   }
}


AbsoluteDefSuffix::AbsoluteDefSuffix(_defptr& def, const _str& suf, const _bool fin)
   : DefinitionSuffix(def, suf, fin) { };


_bool AbsoluteDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->isFinal) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      this->value = str(this->definition->getValue(), this->suffix);

      if (this->isFinal ? os_exists(this->value) : os_directoryExists(this->value)) {
         if (this->isFinal) {
            this->fileContext->index->value = index;
            index++;
         }

         return true;
      }
   }

   this->first = true;
   return false;
}


RelativeDefSuffix::RelativeDefSuffix(_defptr& def, _p2& p2, const _str& suf, const _bool fin)
   : DefinitionSuffix(def, suf, fin), locContext(p2.contexts.getLocationContext()) { };


_bool RelativeDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->isFinal) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      this->value = str(this->definition->getValue(), this->suffix);
      const _str path = str(this->locContext->location->value, OS_SEPARATOR, this->value);

      if (this->isFinal ? os_exists(path) : os_directoryExists(path)) {
         if (this->isFinal) {
            this->fileContext->index->value = index;
            index++;
         }
         return true;
      }
   }

   this->first = true;
   return false;
}


RetreatedDefSuffix::RetreatedDefSuffix(_defptr& def, _p2& p2, const _str& suf, const _bool fin, const _int retr)
   : DefinitionSuffix(def, suf, fin), locContext(p2.contexts.getLocationContext()), retreats(retr) { };


_bool RetreatedDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->isFinal) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      this->value = str(this->definition->getValue(), this->suffix);
      
      _str path = this->locContext->location->value;
      os_retreatPath(path, this->retreats);
      if (path.empty()) {
         continue;
      }

      path += OS_SEPARATOR;
      path += this->value;

      if (this->isFinal ? os_exists(path) : os_directoryExists(path)) {
         if (this->isFinal) {
            this->fileContext->index->value = index;
            index++;
            this->value = str(os_retreats(this->retreats), this->value);
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
