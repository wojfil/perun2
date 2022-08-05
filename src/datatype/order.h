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

//protected:
};


enum OrderUnitType {
   out_Bool = 0,
   out_Number,
   out_Time,
   out_Period,
   out_String
};


template <typename T>
struct OrderBy : Generator<std::vector<T>>
{
public:
   OrderBy(Generator<std::vector<T>>* val, Uroboros* uro)
      : baseValue(val), unitsCount(0), uroboros(uro), inner(&uro->vars.inner)  { }

   ~OrderBy() {
      delete baseValue;
      deleteVector(boolUnits);
      deleteVector(numberUnits);
      deleteVector(timeUnits);
      deleteVector(periodUnits);
      deleteVector(stringUnits);
   }

   void addBool(Generator<_boo>* value, const _boo& descending) {
      unitsIndexes.push_back(boolUnits.size());
      unitsTypes.push_back(OrderUnitType::out_Bool);
      boolUnits.push_back(new OrderUnit<_boo>(value, descending));
      unitsCount++;
   }

   void addNumber(Generator<_num>* value, const _boo& descending) {
      unitsIndexes.push_back(numberUnits.size());
      unitsTypes.push_back(OrderUnitType::out_Number);
      numberUnits.push_back(new OrderUnit<_num>(value, descending));
      unitsCount++;
   }

   void addTime(Generator<_tim>* value, const _boo& descending) {
      unitsIndexes.push_back(timeUnits.size());
      unitsTypes.push_back(OrderUnitType::out_Time);
      timeUnits.push_back(new OrderUnit<_tim>(value, descending));
      unitsCount++;
   }

   void addPeriod(Generator<_per>* value, const _boo& descending) {
      unitsIndexes.push_back(periodUnits.size());
      unitsTypes.push_back(OrderUnitType::out_Period);
      periodUnits.push_back(new OrderUnit<_per>(value, descending));
      unitsCount++;
   }

   void addString(Generator<_str>* value, const _boo& descending) {
      unitsIndexes.push_back(stringUnits.size());
      unitsTypes.push_back(OrderUnitType::out_String);
      stringUnits.push_back(new OrderUnit<_str>(value, descending));
      unitsCount++;
   }

protected:

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

   _int partition(std::vector<T>& list, const _int& start,
      const _int& end) const
   {
      const T& p = list[end];
      const _numi endId = _numi(static_cast<_nint>(end));
      _int i = start - 1;

      for (_int j = start; j <= end - 1; j++){
         if (/*list[j] <= p*/isSmallerOrEquals(_numi(static_cast<_nint>(j)), endId, list[j], p)) {
            i++;
            std::iter_swap(list.begin() + i, list.begin() + j);
         }
      }

      const _int ip = i + 1;
      std::iter_swap(list.begin() + ip, list.begin() + end);
      return ip;
   }

   virtual _boo isSmallerOrEquals(const _numi& leftId, const _numi& rightId,
      const T& left, const T& right) const = 0;


   Uroboros* uroboros;
   InnerVariables* inner;

   Generator<std::vector<T>>* baseValue;
   std::vector<OrderUnit<_boo>*> boolUnits;
   std::vector<OrderUnit<_num>*> numberUnits;
   std::vector<OrderUnit<_tim>*> timeUnits;
   std::vector<OrderUnit<_per>*> periodUnits;
   std::vector<OrderUnit<_str>*> stringUnits;

   std::vector<_size> unitsIndexes;
   std::vector<OrderUnitType> unitsTypes;
   _size unitsCount;
};


#endif /* ORDER_H */
