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

#include "gen-list.h"
#include "../../uroboros.h"
#include "../../util.h"


Filter_WhereString::~Filter_WhereString() {
   delete list;
   delete condition;
}

_list Filter_WhereString::getValue()
{
   const _list values = list->getValue();
   _list result;
   const _size length = values.size();

   const _num prevIndex = this->inner->index.value;
   const _str prevThis = this->inner->this_s.value;

   this->inner->index.value = Number(0LL);

   _size index = 0;
   while (this->uroboros->running && index != length) {
      const _str& string = values[index];
      this->inner->this_s.value = string;
      this->attribute.run();

      if (condition->getValue()) {
         result.push_back(string);
      }

      this->inner->index.value++;
      index++;
   }

   this->inner->index.value = prevIndex;
   this->inner->this_s.value = prevThis;

   return result;
}

_list Filter_OrderByString::getValue()
{
   _list list = baseValue->getValue();
   const _size size = list.size();

   if (size <= 1) {
      return list;
   }

   const _num prevIndex = this->inner->index.value;
   const _str prevThis = this->inner->this_s.value;

   this->quicksort(list, 0, size - 1);

   this->inner->index.value = prevIndex;
   this->inner->this_s.value = prevThis;

   return list;
}

_boo Filter_OrderByString::isSmallerOrEquals(const _num& leftId,
   const _num& rightId, const _str& left, const _str& right) const
{
   for (_size i = 0; i < unitsCount; i++) {
      const OrderUnitType& unitType = unitsTypes[i];
      const _size& unitId = unitsIndexes[i];

      if (i == unitsCount - 1) {
         switch (unitType) {
            case out_Bool: {
               return finalComparison<_boo>(boolUnits[unitId],
                  leftId, rightId, left, right);
            }
            case out_Number: {
               return finalComparison<_num>(numberUnits[unitId],
                  leftId, rightId, left, right);
            }
            case out_Time: {
               return finalComparison<_tim>(timeUnits[unitId],
                  leftId, rightId, left, right);
            }
            case out_Period: {
               return finalComparison<_per>(periodUnits[unitId],
                  leftId, rightId, left, right);
            }
            case out_String: {
               return finalComparison<_str>(stringUnits[unitId],
                  leftId, rightId, left, right);
            }
         }
      }
      else {
         _boo success = false;
         _boo result;

         switch (unitType) {
            case out_Bool: {
               result = usualComparison<_boo>(boolUnits[unitId],
                  leftId, rightId, left, right, success);
               break;
            }
            case out_Number: {
               result = usualComparison<_num>(numberUnits[unitId],
                  leftId, rightId, left, right, success);
               break;
            }
            case out_Time: {
               result = usualComparison<_tim>(timeUnits[unitId],
                  leftId, rightId, left, right, success);
               break;
            }
            case out_Period: {
               result = usualComparison<_per>(periodUnits[unitId],
                  leftId, rightId, left, right, success);
               break;
            }
            case out_String: {
               result = usualComparison<_str>(stringUnits[unitId],
                  leftId, rightId, left, right, success);
               break;
            }
         }

         if (success) {
            return result;
         }
      }
   }

   return false;
}

Join_StrStr::~Join_StrStr() {
   delete left;
   delete right;
}

Join_StrList::~Join_StrList() {
   delete left;
   delete right;
}

Join_ListStr::~Join_ListStr() {
   delete left;
   delete right;
}

Join_ListList::~Join_ListList() {
   delete left;
   delete right;
}

_list Join_StrStr::getValue()
{
   _list v;
   v.reserve(2);
   v.push_back(left->getValue());
   v.push_back(right->getValue());
   return v;
}

_list Join_StrList::getValue()
{
   _list v = right->getValue();
   v.insert(v.begin(), left->getValue());
   return v;
}

_list Join_ListStr::getValue()
{
   _list v = left->getValue();
   v.push_back(right->getValue());
   return v;
}

_list Join_ListList::getValue()
{
   _list v = left->getValue();
   appendVector(v, right->getValue());
   return v;
}
