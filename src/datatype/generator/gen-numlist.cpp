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

#include "gen-numlist.h"
#include "../../uroboros.h"


_nlist Filter_OrderByNumber::getValue()
{
   _nlist list = baseValue->getValue();
   const _size size = list.size();

   if (size <= 1) {
      return list;
   }

   const _num prevIndex = this->inner->index.value;
   const _num prevThis = this->inner->this_n.value;

   this->quicksort(list, 0, size - 1);

   this->inner->index.value = prevIndex;
   this->inner->this_n.value = prevThis;

   return list;
}

_boo Filter_OrderByNumber::isSmallerOrEquals(const _num& leftId,
   const _num& rightId, const _num& left, const _num& right) const
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
