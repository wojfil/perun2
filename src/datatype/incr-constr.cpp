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

#include "incr-constr.h"


namespace perun2
{


IncrementalConstraint::IncrementalConstraint(p_genptr<p_num>& lg, const CompType ct)
   : limitGen(std::move(lg)), comparisonType(ct) { };

void IncrementalConstraint::reset()
{
   this->value.setToZero();
   this->limit = this->limitGen->getValue();
}

void IncrementalConstraint::setValue(const p_num& val)
{
   this->value = val;
   this->limit = this->limitGen->getValue();
}

void IncrementalConstraint::increment(const p_num& val)
{
   this->value += val;
}

void IncrementalConstraint::incrementByOne()
{
   this->value++;
}


IC_State IncrementalConstraint::getState() const
{
   switch (this->comparisonType) {
      case CompType::ct_Equals: {
         return (this->value > this->limit)
            ? IC_State::False
            : IC_State::Unknown;
      }
      case CompType::ct_NotEquals: {
         return (this->value > this->limit)
            ? IC_State::True
            : IC_State::Unknown;
      }
      case CompType::ct_Smaller: {
         return (this->value >= this->limit)
            ? IC_State::False
            : IC_State::Unknown;
      }
      case CompType::ct_SmallerEquals: {
         return (this->value > this->limit)
            ? IC_State::False
            : IC_State::Unknown;
      }
      case CompType::ct_Bigger: {
         return (this->value > this->limit)
            ? IC_State::True
            : IC_State::Unknown;
      }
      case CompType::ct_BiggerEquals: {
         return (this->value >= this->limit)
            ? IC_State::True
            : IC_State::Unknown;
      }
      default: {
         return IC_State::Unknown;
      }
   }
}


p_bool IncrementalConstraint::getFailureResult() const
{
   switch (this->comparisonType) {
      case CompType::ct_Equals: {
         return this->limit == NINT_MINUS_ONE;
      }
      case CompType::ct_NotEquals: {
         return this->limit != NINT_MINUS_ONE;
      }
      case CompType::ct_Smaller: {
         return this->limit > NINT_MINUS_ONE;
      }
      case CompType::ct_SmallerEquals: {
         return this->limit >= NINT_MINUS_ONE;
      }
      case CompType::ct_Bigger: {
         return this->limit < NINT_MINUS_ONE;
      }
      case CompType::ct_BiggerEquals: {
         return this->limit <= NINT_MINUS_ONE;
      }
      default: {
         return false;
      }
   }
}


p_bool IncrementalConstraint::getFinalResult() const
{
   switch (this->comparisonType) {
      case CompType::ct_Equals: {
         return this->value == this->limit;
      }
      case CompType::ct_NotEquals: {
         return this->value != this->limit;
      }
      case CompType::ct_Smaller: {
         return this->value < this->limit;
      }
      case CompType::ct_SmallerEquals: {
         return this->value <= this->limit;
      }
      case CompType::ct_Bigger: {
         return this->value > this->limit;
      }
      case CompType::ct_BiggerEquals: {
         return this->value >= this->limit;
      }
      default: {
         return false;
      }
   }
}


}
