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

#ifndef GEN_TIMLIST_H
#define GEN_TIMLIST_H

#include "../generator.h"
#include "../order.h"
#include "../../var/var-runtime.h"
#include "gen-generic.h"



struct Filter_OrderByTime : Generator<_tlist>, OrderBy<_tim>
{
public:
   Filter_OrderByTime(Generator<_tlist>* val, Uroboros* uro)
      : OrderBy(val, uro) {};


   _tlist getValue() override;
   _boo isSmallerOrEquals(const _numi& leftId, const _numi& rightId,
      const _tim& left, const _tim& right) const override;


   template <typename T>
   _boo finalComparison(const OrderUnit<T>* ou, const _numi& leftId,
      const _numi& rightId, const _tim& left, const _tim& right) const {

      this->inner->index.value = leftId;
      this->inner->this_t.value = left;
      const T leftValue = ou->value->getValue();

      this->inner->index.value = rightId;
      this->inner->this_t.value = right;
      const T rightValue = ou->value->getValue();

      return ou->descending
         ? leftValue >= rightValue
         : leftValue <= rightValue;
   }

   template <typename T>
   _boo usualComparison(const OrderUnit<T>* ou, const _numi& leftId,
      const _numi& rightId, const _tim& left, const _tim& right,
      _boo& success) const {

      this->inner->index.value = leftId;
      this->inner->this_t.value = left;
      const T leftValue = ou->value->getValue();

      this->inner->index.value = rightId;
      this->inner->this_t.value = right;
      const T rightValue = ou->value->getValue();

      if (leftValue == rightValue) {
         return false;
      }

      success = true;

      return ou->descending
         ? leftValue > rightValue
         : leftValue < rightValue;
   }
};


struct UniqueTime : UnaryOperation<_tlist>
{
   UniqueTime(Generator<_tlist>* val) : UnaryOperation<_tlist>(val) { };

   _tlist getValue() override;
};



#endif /* GEN_TIMLIST_H */
