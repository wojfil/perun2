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


DefWithContext::DefWithContext(p_defptr& def, p_perun2& p2)
   : definition(std::move(def)), context(std::make_unique<FileContext>(p2)) { };

DefWithContext::DefWithContext(p_defptr& def, p_fcptr& ctx)
   : definition(std::move(def)), context(std::move(ctx)) { };

void DefWithContext::reset()
{
   this->definition->reset();
}

p_bool DefWithContext::hasNext()
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


DefFilter::DefFilter(p_defptr& def, FileContext* ctx, p_perun2& p2)
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

p_bool DefFilter::setAction(p_daptr& act)
{
   return this->definition->setAction(act);
}


DefFilter_Where::DefFilter_Where(p_genptr<p_bool>& cond, p_defptr& def, FileContext* ctx, p_perun2& p2)
   : DefFilter(def, ctx, p2), condition(std::move(cond)) { };


void DefFilter_Where::reset() {
   if (!first) {
      first = true;
      if (!finished) {
         definition->reset();
      }
   }
}


p_bool DefFilter_Where::hasNext()
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
      const p_bool con = this->condition->getValue();

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


LocationVessel::LocationVessel(const PathType pt, p_genptr<p_str>& loc)
   : pathType(pt), location(std::move(loc)) { };


p_str LocationVessel::getValue()
{
   if (this->pathType == PathType::Absolute) {
      return this->value;
   }

   return str(this->location->getValue(), OS_SEPARATOR, this->value);
};

const p_str& LocationVessel::getRawValue() const
{
   return this->value;
};


void LocationVessel::setValue(const p_str& val)
{
   this->value = val;
};


NestedDefiniton::NestedDefiniton(LocationVessel& ves, p_defptr& def, p_defptr& locs, const PathType pt, const SegmentType segmType, const p_int retr)
   : vessel(ves), definition(std::move(def)), locations(std::move(locs)),
     context(definition->getFileContext()), pathType(pt), segmentType(segmType), retreats(retr) { };


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


p_bool NestedDefiniton::hasNext()
{
   if (!this->locsOpened) {
      if (this->locations->hasNext()) {
         this->locsOpened = true;
         this->vessel.setValue(this->locations->getValue());
         if (this->segmentType == SegmentType::Final) {
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

         if (this->pathType == PathType::Absolute) {
            this->value = this->definition->getValue();
         }
         else {
            const p_bool isDot = this->vessel.getRawValue().size() == 1 && this->vessel.getRawValue()[0] == CHAR_DOT;

            this->value = isDot
               ? this->definition->getValue()
               : str(this->vessel.getRawValue(), OS_SEPARATOR, this->definition->getValue());
            
            if (this->context == nullptr) {
               this->value = this->definition->getValue();
            }
         }

         if (this->segmentType == SegmentType::Final) {
            if (this->retreats > 0) {
               this->value = str(os_retreats(this->retreats), this->value);
            }

            if (this->context != nullptr) {
               this->context->index->value = index;
               this->context->loadData(this->value);
               index++;
            }
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


p_bool DefFilter_Limit::hasNext()
{
   if (first) {
      const p_num n = this->number->getValue();
      if (n.state == NumberState::NaN) {
         return false;
      }

      limit = n.toInt();
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


p_bool DefFilter_Skip::hasNext()
{
   if (first) {
      const p_num n = this->number->getValue();
      if (n.state == NumberState::NaN) {
         return false;
      }

      limit = n.toInt();
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


p_bool DefFilter_Every::hasNext()
{
   if (first) {
      const p_num n = this->number->getValue();
      if (n.state == NumberState::NaN) {
         return false;
      }

      limit = n.toInt();
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


p_bool DefFilter_Final::hasNext()
{
   if (first) {
      const p_num n = this->number->getValue();
      if (n.state == NumberState::NaN) {
         return false;
      }

      const p_nint limit = n.toInt();
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
      value = values[static_cast<p_size>(index)];
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

p_bool Join_DefStr::hasNext()
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


p_bool Join_StrDef::hasNext()
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


p_bool Join_DefList::hasNext()
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


p_bool Join_ListDef::hasNext()
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


p_bool Join_DefDef::hasNext()
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



DefinitionSuffix::DefinitionSuffix(p_defptr& def, const p_str& suf, const SegmentType segmType)
   : definition(std::move(def)), fileContext(definition->getFileContext()), suffix(suf), segmentType(segmType) { };


void DefinitionSuffix::reset()
{
   if (!this->first) {
      this->definition->reset();
      this->first = true;
   }
}


AbsoluteDefSuffix::AbsoluteDefSuffix(p_defptr& def, const p_str& suf, const SegmentType segmType)
   : DefinitionSuffix(def, suf, segmType) { };


p_bool AbsoluteDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->segmentType == SegmentType::Final) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      this->value = str(this->definition->getValue(), this->suffix);

      if ((this->segmentType == SegmentType::Final) 
         ? os_exists(this->value) 
         : os_directoryExists(this->value)) 
      {
         if (this->segmentType == SegmentType::Final) {
            this->fileContext->index->value = index;
            index++;
         }

         return true;
      }
   }

   this->first = true;
   return false;
}


RelativeDefSuffix::RelativeDefSuffix(p_defptr& def, p_perun2& p2, const p_str& suf, const SegmentType segmType, p_def* const prev)
   : DefinitionSuffix(def, suf, segmType), locContext(p2.contexts.getLocationContext()), previous(prev) { };


p_bool RelativeDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->segmentType == SegmentType::Final) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      this->value = this->previous == nullptr
         ? str(this->definition->getValue(), this->suffix)
         : str(this->previous->getValue(), OS_SEPARATOR, this->definition->getValue(), this->suffix);

      const p_str path = str(this->locContext->location->value, OS_SEPARATOR, this->value);

      if ((this->segmentType == SegmentType::Final)
         ? os_exists(path) 
         : os_directoryExists(path)) 
      {
         if (this->segmentType == SegmentType::Final) {
            this->fileContext->index->value = index;
            index++;
         }
         return true;
      }
   }

   this->first = true;
   return false;
}


RetreatedDefSuffix::RetreatedDefSuffix(p_defptr& def, p_perun2& p2, const p_str& suf, const SegmentType segmType, const p_int retr, p_def* const prev)
   : DefinitionSuffix(def, suf, segmType), locContext(p2.contexts.getLocationContext()), retreats(retr), previous(prev) { };


p_bool RetreatedDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->segmentType == SegmentType::Final) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      const p_str rawValue = str(this->definition->getValue(), this->suffix);

      this->value = this->previous == nullptr
         ? rawValue
         : str(this->previous->getValue(), OS_SEPARATOR, rawValue);


      p_str path = this->locContext->location->value;
      os_retreatPath(path, this->retreats);

      if (path.empty()) {
         continue;
      }

      path += OS_SEPARATOR;
      path += this->value;

      if ((this->segmentType == SegmentType::Final)
         ? os_exists(path)
         : os_directoryExists(path))
      {
         if (this->segmentType == SegmentType::Final) {
            this->fileContext->index->value = index;
            index++;

            if (this->previous == nullptr) {
               this->value = str(os_retreats(this->retreats), this->value);
            }
         }
         else {
            this->value = rawValue;
         }

         return true;
      }
   }

   this->first = true;
   return false;
}


FarRetreatedDefSuffix::FarRetreatedDefSuffix(p_defptr& def, p_perun2& p2, const p_str& suf, const SegmentType segmType, const p_int retr, p_def* const prev)
   : DefinitionSuffix(def, suf, segmType), locContext(p2.contexts.getLocationContext()), retreats(retr), previous(prev) { };


p_bool FarRetreatedDefSuffix::hasNext()
{
   if (this->first) {
      this->first = false;
      if (this->segmentType == SegmentType::Final) {
         this->index.setToZero();
      }
   }

   while (this->definition->hasNext()) {
      this->value = this->previous == nullptr
         ? str(this->definition->getValue(), this->suffix)
         : str(this->previous->getValue(), OS_SEPARATOR, this->definition->getValue(), this->suffix);

      p_str path = this->locContext->location->value;
      os_retreatPath(path, this->retreats);
      path = str(path, OS_SEPARATOR, this->value);

      if ((this->segmentType == SegmentType::Final)
         ? os_exists(path) 
         : os_directoryExists(path)) 
      {
         if (this->segmentType == SegmentType::Final) {
            this->fileContext->index->value = index;
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


p_bool DefTernary::hasNext()
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


p_bool DefBinary::hasNext()
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
