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
#include "../attr-memory.h"


namespace uro::gen
{

struct OrderIndices
{
public:
   OrderIndices();
   ~OrderIndices();
   void prepare(const _size& length);

   _size* values;
};


struct Order
{
public:
   virtual void clearValues(const _size& length) = 0;
   virtual void clearValues() = 0;
   virtual void addValues() = 0;
   virtual _boo matchesSwap(const _int& start, const _int& end) const = 0;
};


template <typename T>
struct OrderUnit : Order
{
public:
   OrderUnit(Generator<T>* val, const _boo desc, OrderIndices* indie)
      : valueGenerator(val), descending(desc), indices(indie) { };

   ~OrderUnit()
   {
      delete valueGenerator;
   }

protected:
   Generator<T>* valueGenerator;
   OrderIndices* indices;
   std::vector<T> values;
   const _boo descending;
};


template <typename T>
struct OrderUnit_Middle : OrderUnit<T>
{
public:
   OrderUnit_Middle(Generator<T>* val, const _boo desc, Order* next, OrderIndices* indie)
      : OrderUnit<T>(val, desc, indie), nextUnit(next) { };

   ~OrderUnit_Middle()
   {
      delete nextUnit;
   }

   void clearValues(const _size& length) override
   {
      langutil::clearAndReserve(this->values, length);
      this->nextUnit->clearValues(length);
   }

   void clearValues() override
   {
      this->values.clear();
      this->nextUnit->clearValues();
   }

   void addValues() override
   {
      this->values.push_back(this->valueGenerator->getValue());
      this->nextUnit->addValues();
   }

   _boo matchesSwap(const _int& start, const _int& end) const override
   {
      const T& left = this->values[this->indices->values[start]];
      const T& right = this->values[this->indices->values[end]];

      if (left == right) {
         return this->nextUnit->matchesSwap(start, end);
      }
      else {
         return this->descending
            ? left > right
            : left < right;
      }
   }

private:
   Order* nextUnit;
};


template <typename T>
struct OrderUnit_Final : OrderUnit<T>
{
public:
   OrderUnit_Final(Generator<T>* val, const _boo desc, OrderIndices* indie)
      : OrderUnit<T>(val, desc, indie) { };

   void clearValues(const _size& length) override
   {
      langutil::clearAndReserve(this->values, length);
   }

   void clearValues() override
   {
      this->values.clear();
   }

   void addValues() override
   {
      this->values.push_back(this->valueGenerator->getValue());
   }

   _boo matchesSwap(const _int& start, const _int& end) const override
   {
      return this->descending
         ? this->values[this->indices->values[start]] >= this->values[this->indices->values[end]]
         : this->values[this->indices->values[start]] <= this->values[this->indices->values[end]];
   }
};


template <typename T>
struct OrderBy
{
public:
   OrderBy(Attribute* attr, OrderIndices* indices, Order* ord, uro::Uroboros* uro)
      : attribute(attr), inner(&uro->vars.inner),
        hasAttribute(attr != nullptr), orderIndices(indices), order(ord)
   {
      this->inner->createThisVarRef(thisReference);
   }

   ~OrderBy()
   {
      delete attribute;
      delete orderIndices;
      delete order;
   }


   void quicksort(const _int& start, const _int& end)
   {
      if (start < end) {
         const _int i = partition(start, end);
         quicksort(start, i - 1);
         quicksort(i + 1, end);
      }
   }

   _int partition(const _int& start, const _int& end)
   {
      _int i = start - 1;

      for (_int j = start; j <= end - 1; j++) {
         if (this->order->matchesSwap(j, end)) {
            i++;
            std::swap(this->orderIndices->values[i], this->orderIndices->values[j]);
            std::iter_swap(this->resultPtr->begin() + i, this->resultPtr->begin() + j);
         }
      }

      const _int ip = i + 1;
      std::swap(this->orderIndices->values[ip], this->orderIndices->values[end]);
      std::iter_swap(this->resultPtr->begin() + ip, this->resultPtr->begin() + end);
      return ip;
   }

protected:
   OrderIndices* orderIndices;
   Order* order;
   Attribute* attribute;
   const _boo hasAttribute;
   uro::InnerVariables* inner;
   vars::Variable<T>* thisReference;
   std::vector<T>* resultPtr;
};


template <typename T>
struct OrderBy_List : OrderBy<T>, Generator<std::vector<T>>
{
public:
   OrderBy_List(Generator<std::vector<T>>* bas, Attribute* attr, OrderIndices* indices, Order* ord, uro::Uroboros* uro)
      : OrderBy<T>(attr, indices, ord, uro), base(bas) { }

   ~OrderBy_List()
   {
      delete this->base;
   }

   std::vector<T> getValue() override
   {
      std::vector<T> result = this->base->getValue();
      const _size length = result.size();

      if (length == 0) {
         return result;
      }

      this->resultPtr = &result;
      this->orderIndices->prepare(length);
      this->order->clearValues(length);

      const _numi prevIndex = this->inner->index.value;
      const _numi prevDepth = this->inner->depth.value;
      const T prevThis = this->thisReference->value;
      this->inner->depth.value.setToZero();

      for (_size i = 0; i < length; i++) {
         this->thisReference->value = result[i];
         this->inner->index.value = _numi(static_cast<_nint>(i));

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->orderIndices->values[i] = i;
         this->order->addValues();
      }

      this->quicksort(0, length - 1);

      this->inner->index.value = prevIndex;
      this->inner->depth.value = prevDepth;
      this->thisReference->value = prevThis;

      return result;
   }

private:
   Generator<std::vector<T>>* base;
};


struct OrderBy_Definition : OrderBy<_str>, _def
{
public:
   OrderBy_Definition(_def* bas, Attribute* attr, const _boo& hasMem,
      OrderIndices* indices, Order* ord, uro::Uroboros* uro);
   ~OrderBy_Definition();

   void reset() override;
   _boo hasNext() override;

private:
   _def* base;
   _boo first = true;
   uro::Uroboros* uroboros;
   uro::InnerVariables* inner;
   const _boo hasMemory;
   AttributeMemory attrMemory;

   _size length;
   _size index;

   _numi prevIndex;
   _numi prevDepth;
   _str prevThis;

   _list result;

   _boo hasVolatileDepth;
   std::vector<_nint> depths;
};

}

#endif /* ORDER_H */
