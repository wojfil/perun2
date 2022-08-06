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

#ifndef ORDER_H
#define ORDER_H

#include "generator.h"
#include "datatype.h"
#include "../util.h"
#include "../uroboros.h"


template <typename T>
struct OrderUnit {

public:
   OrderUnit(Generator<T>* val, const _boo& desc)
      : value(val), descending(desc) { };

   ~OrderUnit() {
      delete value;
   }

   Generator<T>* value;
   const _boo descending;
};

enum OrderUnitType {
   out_Bool = 0,
   out_Number,
   out_Time,
   out_Period,
   out_String
};

template <typename T>
struct TempOrderBy : Generator<std::vector<T>>
{
public:
   TempOrderBy(Generator<std::vector<T>>* val, Attribute* attr, Uroboros* uro)
      : baseValue(val), unitsCount(0), uroboros(uro), inner(&uro->vars.inner),
        attribute(attr), hasAttribute(attr != nullptr)
   {
      this->inner->createThisVarRef(thisReference);
   }

   ~TempOrderBy()
   {
      delete baseValue;
      deleteVector(boolUnits);
      deleteVector(numberUnits);
      deleteVector(timeUnits);
      deleteVector(periodUnits);
      deleteVector(stringUnits);
      if (hasAttribute) {
         delete attribute;
      }
   }

   template <typename T2>
   void addUnit(Generator<T2>* value, const OrderUnitType& type, const _boo& descending)
   {
      unitsIndexes.push_back(boolUnits.size());
      unitsTypes.push_back(type);
      std::vector<OrderUnit<T2>*>* units;
      getUnitsPointer(units);
      units->push_back(new OrderUnit<T2>(value, descending));
      unitsCount++;
   }

   std::vector<T> getValue()
   {
      std::vector<T> list = baseValue->getValue();
      const _size size = list.size();

      if (size <= 1) {
         return list;
      }

      const _numi prevIndex = this->inner->index.value;
      const T prevThis = this->thisReference->value;

      this->quicksort(list, 0, size - 1);

      this->inner->index.value = prevIndex;
      this->thisReference->value = prevThis;

      return list;
   }

private:

   void getUnitsPointer(std::vector<OrderUnit<_boo>*>*& result) { result = &boolUnits; }
   void getUnitsPointer(std::vector<OrderUnit<_num>*>*& result) { result = &numberUnits; }
   void getUnitsPointer(std::vector<OrderUnit<_tim>*>*& result) { result = &timeUnits; }
   void getUnitsPointer(std::vector<OrderUnit<_per>*>*& result) { result = &periodUnits; }
   void getUnitsPointer(std::vector<OrderUnit<_str>*>*& result) { result = &stringUnits; }

   void quicksort(std::vector<T>& list, const _int& start,
      const _int& end) const
   {
      if (start < end) {
         const _int i = partition(list, start, end);
         const _int ip = i + 1;
         const _int im = i - 1;
         quicksort(list, start, im);
         quicksort(list, ip, end);
      }
   }

   _int partition(std::vector<T>& list, const _int& start, const _int& end) const
   {
      const T& p = list[end];
      const _numi endId = _numi(static_cast<_nint>(end));
      _int i = start - 1;

      for (_int j = start; j <= end - 1; j++){
         if (isSmallerOrEquals(_numi(static_cast<_nint>(j)), endId, list[j], p)) {
            i++;
            std::iter_swap(list.begin() + i, list.begin() + j);
         }
      }

      const _int ip = i + 1;
      std::iter_swap(list.begin() + ip, list.begin() + end);
      return ip;
   }

   _boo isSmallerOrEquals(const _numi& leftId, const _numi& rightId,
      const T& left, const T& right) const
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

   template <typename T2>
   _boo finalComparison(const OrderUnit<T2>* ou, const _numi& leftId,
      const _numi& rightId, const T& left, const T& right) const
   {
      this->inner->index.value = leftId;
      this->thisReference->value = left;
      if (this->hasAttribute) {
         this->attribute->run();
      }
      const T2 leftValue = ou->value->getValue();

      this->inner->index.value = rightId;
      this->thisReference->value = right;
      if (this->hasAttribute) {
         this->attribute->run();
      }
      const T2 rightValue = ou->value->getValue();

      return ou->descending
         ? leftValue >= rightValue
         : leftValue <= rightValue;
   }

   template <typename T2>
   _boo usualComparison(const OrderUnit<T2>* ou, const _numi& leftId,
      const _numi& rightId, const T& left, const T& right, _boo& success) const
   {
      this->inner->index.value = leftId;
      this->thisReference->value = left;
      if (this->hasAttribute) {
         this->attribute->run();
      }
      const T2 leftValue = ou->value->getValue();

      this->inner->index.value = rightId;
      this->thisReference->value = right;
      if (this->hasAttribute) {
         this->attribute->run();
      }
      const T2 rightValue = ou->value->getValue();

      if (leftValue == rightValue) {
         return false;
      }

      success = true;

      return ou->descending
         ? leftValue > rightValue
         : leftValue < rightValue;
   }

   // main members:
   Attribute* attribute;
   const _boo hasAttribute;
   Uroboros* uroboros;
   InnerVariables* inner;
   Generator<std::vector<T>>* baseValue;
   Variable<T>* thisReference;

   // order by units can be of 5 data types:
   std::vector<OrderUnit<_boo>*> boolUnits;
   std::vector<OrderUnit<_num>*> numberUnits;
   std::vector<OrderUnit<_tim>*> timeUnits;
   std::vector<OrderUnit<_per>*> periodUnits;
   std::vector<OrderUnit<_str>*> stringUnits;

   // information about order units
   std::vector<_size> unitsIndexes;
   std::vector<OrderUnitType> unitsTypes;
   _size unitsCount;
};


#endif /* ORDER_H */
