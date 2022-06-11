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

#ifndef GEN_NUMLIST_H
#define GEN_NUMLIST_H

#include "../generator.h"
#include "../order.h"
#include "../../var/var-runtime.h"


struct Filter_WhereNumber : Generator<_nlist>
{
public:
   Filter_WhereNumber(Generator<_nlist>* li, Generator<_boo>* cond)
      : list(li), condition(cond) {};

   ~Filter_WhereNumber() {
      delete list;
      delete condition;
   }

   _nlist getValue() override;

private:
   Generator<_nlist>* list;
   Generator<_boo>* condition;
};


struct Filter_OrderByNumber : Generator<_nlist>, OrderBy<_num>
{
public:
   Filter_OrderByNumber(Generator<_nlist>* val)
      : OrderBy(val) {};


   _nlist getValue() override;
   _boo isSmallerOrEquals(const _num& leftId, const _num& rightId,
      const _num& left, const _num& right) const override;


   template <typename T>
   _boo finalComparison(const OrderUnit<T>* ou, const _num& leftId,
      const _num& rightId, const _num& left, const _num& right) const {

      g_index.value = leftId;
      g_this_n.value = left;
      const T leftValue = ou->value->getValue();

      g_index.value = rightId;
      g_this_n.value = right;
      const T rightValue = ou->value->getValue();

      return ou->descending
         ? leftValue >= rightValue
         : leftValue <= rightValue;
   }

   template <typename T>
   _boo usualComparison(const OrderUnit<T>* ou, const _num& leftId,
      const _num& rightId, const _num& left, const _num& right,
      _boo& success) const {

      g_index.value = leftId;
      g_this_n.value = left;
      const T leftValue = ou->value->getValue();

      g_index.value = rightId;
      g_this_n.value = right;
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




#endif /* GEN_NUMLIST_H */

