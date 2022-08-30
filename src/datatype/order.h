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
struct OrderBase : Generator<std::vector<T>>
{
public:
   virtual _boo hasVolatileDepth() const = 0;
   virtual _numi getDepth(const _size& index) const = 0;
   virtual void shuffleDepth(const std::vector<_size>& indices, const _size& length) { };
};


template <typename T>
struct OrderBase_Depthless : OrderBase<T>
{
public:
   OrderBase_Depthless(Generator<std::vector<T>>* bas)
      : base(bas) { }

   ~OrderBase_Depthless()
   {
      delete base;
   }

   std::vector<T> getValue() override
   {
      return this->base->getValue();
   }

   _boo hasVolatileDepth() const override
   {
      return false;
   }

   _numi getDepth(const _size& index) const override
   {
      return _numi(0LL);
   }

private:
   Generator<std::vector<T>>* base;
};


struct OrderBase_Definition : OrderBase<_str>
{
public:
   OrderBase_Definition(_def* bas, Uroboros* uro)
      : base(bas), uroboros(uro), inner(&uro->vars.inner) { }

   ~OrderBase_Definition();

   std::vector<_str> getValue() override;
   _boo hasVolatileDepth() const override;
   _numi getDepth(const _size& index) const override;
   void shuffleDepth(const std::vector<_size>& indices, const _size& length) override;

private:
   _def* base;
   Uroboros* uroboros;
   InnerVariables* inner;
   _boo volatileDepth;
   std::vector<_numi> depths;
};


struct OrderIndices
{
public:
   void clearValues(const _size& length);
   std::vector<_size> values;
};


struct Order
{
public:
   virtual void clearValues(const _size& length) = 0;
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
      clearAndReserve(this->values, length);
      this->nextUnit->clearValues(length);
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
      clearAndReserve(this->values, length);
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
struct OrderBy : Generator<std::vector<T>>
{
public:
   OrderBy(OrderBase<T>* bas, Attribute* attr, OrderIndices* indices, Order* ord, Uroboros* uro)
      : base(bas), attribute(attr), inner(&uro->vars.inner),
        hasAttribute(attr != nullptr), orderIndices(indices), order(ord)
   {
      this->inner->createThisVarRef(thisReference);
   }

   ~OrderBy()
   {
      delete base;
      delete attribute;
      delete orderIndices;
      delete order;
   }

   std::vector<T> getValue() override
   {
      this->result = this->base->getValue();
      const _size length = this->result.size();

      if (length == 0) {
         return this->result;
      }

      this->orderIndices->clearValues(length);
      this->order->clearValues(length);

      const _numi prevIndex = this->inner->index.value;
      const T prevThis = this->thisReference->value;
      const _boo hasVolatileDepth = this->base->hasVolatileDepth();

      for (_size i = 0; i < length; i++) {
         this->thisReference->value = this->result[i];
         this->inner->index.value = _numi(static_cast<_nint>(i));

         if (this->hasAttribute) {
            this->attribute->run();
         }
         this->orderIndices->values[i] = i;
         if (hasVolatileDepth) {
            this->inner->depth.value = this->base->getDepth(i);
         }

         this->order->addValues();
      }

      this->quicksort(0, length - 1);

      this->inner->index.value = prevIndex;
      this->thisReference->value = prevThis;

      shuffleByIndices(this->result, this->orderIndices->values, length);

      if (hasVolatileDepth) {
         this->base->shuffleDepth(this->orderIndices->values, length);
      }

      return result;
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
            std::iter_swap(this->orderIndices->values.begin() + i, this->orderIndices->values.begin() + j);
         }
      }

      const _int ip = i + 1;
      std::iter_swap(this->orderIndices->values.begin() + ip, this->orderIndices->values.begin() + end);
      return ip;
   }

protected:
   std::vector<T> result;
   OrderIndices* orderIndices;
   OrderBase<T>* base;
   Order* order;
   Attribute* attribute;
   const _boo hasAttribute;
   InnerVariables* inner;
   Variable<T>* thisReference;
};


#endif /* ORDER_H */
