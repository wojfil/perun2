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

#include "gen-timlist.h"
#include "../../uroboros.h"


_tlist Filter_WhereTime::getValue()
{
   const _tlist values = list->getValue();
   _tlist result;
   const _size length = values.size();

   const _num prevIndex = g_index.value;
   const _tim prevThis = g_this_t.value;

   g_index.value = Number(0LL);

   _size index = 0;
   while (g_running && index != length) {
      const _tim& time = values[index];
      g_this_t.value = time;

      if (condition->getValue()) {
         result.push_back(time);
      }

      g_index.value++;
      index++;
   }

   g_index.value = prevIndex;
   g_this_t.value = prevThis;

   return result;
}

_tlist Filter_OrderByTime::getValue()
{
   _tlist list = baseValue->getValue();
   const _size size = list.size();

   if (size <= 1) {
      return list;
   }

   const _num prevIndex = g_index.value;
   const _tim prevThis = g_this_t.value;

   this->quicksort(list, 0, size - 1);

   g_index.value = prevIndex;
   g_this_t.value = prevThis;

   return list;
}

_boo Filter_OrderByTime::isSmallerOrEquals(const _num& leftId,
   const _num& rightId, const _tim& left, const _tim& right) const
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

_tlist UniqueTime::getValue() {
   _tlist val = value->getValue();
   const _size len = val.size();

   switch (len) {
      case 0:
      case 1: {
         return val;
      }
      case 2: {
         return val[0].equalsExactly(val[1])
            ? _tlist(1, val[0])
            : val;
      }
   }

   std::vector<_tim> newVal;
   _size newLen = 0;
   _boo found;

   for (_size i = 0; i < len; i++) {
      found = false;
      const _tim& t = val[i];

      for (_size j = 0; j < newLen; j++) {
         if (t.equalsExactly(newVal[j])) {
            found = true;
            break;
         }
      }

      if (!found) {
         newVal.push_back(t);
         newLen++;
      }
   }

   return newVal;
}

