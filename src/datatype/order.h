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
   OrderIndices() noexcept;
   ~OrderIndices() noexcept;
   void prepare(const _size& length);

   _size* values;
};

typedef std::unique_ptr<OrderIndices> _indptr;


struct Order
{
public:
   virtual void clearValues(const _size& length) = 0;
   virtual void clearValues() = 0;
   virtual void addValues() = 0;
   virtual _bool matchesSwap(const _int& start, const _int& end) const = 0;
};

typedef std::unique_ptr<Order> _ordptr;


template <typename T>
struct OrderUnit : Order
{
public:
   OrderUnit(_genptr<T>& val, const _bool& desc, _indptr& inds)
      : valueGenerator(std::move(val)), descending(desc), indices(inds.get()) { };

protected:
   _genptr<T> valueGenerator;
   OrderIndices* indices;
   std::vector<T> values;
   const _bool descending;
};


template <typename T>
struct OrderUnit_Middle : OrderUnit<T>
{
public:
   OrderUnit_Middle(_genptr<T>& val, const _bool& desc, _ordptr& next, _indptr& inds)
      : OrderUnit<T>(val, desc, inds), nextUnit(std::move(next)) { };

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
      this->values.emplace_back(this->valueGenerator->getValue());
      this->nextUnit->addValues();
   }

   _bool matchesSwap(const _int& start, const _int& end) const override
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
   _ordptr nextUnit;
};


template <typename T>
struct OrderUnit_Final : OrderUnit<T>
{
public:
   OrderUnit_Final(_genptr<T>& val, const _bool& desc, _indptr& inds)
      : OrderUnit<T>(val, desc, inds) { };

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
      this->values.emplace_back(this->valueGenerator->getValue());
   }

   _bool matchesSwap(const _int& start, const _int& end) const override
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
   OrderBy(_attrptr& attr, _indptr& inds, _ordptr& ord, Uroboros& uro)
      : attribute(std::move(attr)), inner(uro.vars.inner),
        hasAttribute(attribute), indices(std::move(inds)), order(std::move(ord))
   {
      this->inner.createThisVarRef(thisReference);
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
            std::swap(this->indices->values[i], this->indices->values[j]);
            std::iter_swap(this->resultPtr->begin() + i, this->resultPtr->begin() + j);
         }
      }

      const _int ip = i + 1;
      std::swap(this->indices->values[ip], this->indices->values[end]);
      std::iter_swap(this->resultPtr->begin() + ip, this->resultPtr->begin() + end);
      return ip;
   }

protected:
   _indptr indices;
   _ordptr order;
   _attrptr attribute;
   const _bool hasAttribute;
   InnerVariables& inner;
   vars::Variable<T>* thisReference;
   std::vector<T>* resultPtr;
};


template <typename T>
struct OrderBy_List : OrderBy<T>, Generator<std::vector<T>>
{
public:
   OrderBy_List(_genptr<std::vector<T>>& bas, _attrptr& attr, _indptr& inds, _ordptr& ord, Uroboros& uro)
      : OrderBy<T>(attr, inds, ord, uro), base(std::move(bas)) { }

   std::vector<T> getValue() override
   {
      std::vector<T> result = this->base->getValue();
      const _size length = result.size();

      if (length == 0) {
         return result;
      }

      this->resultPtr = &result;
      this->indices->prepare(length);
      this->order->clearValues(length);

      const _numi prevIndex = this->inner.index.value;
      const _numi prevDepth = this->inner.depth.value;
      const T prevThis = this->thisReference->value;
      this->inner.depth.value.setToZero();

      for (_size i = 0; i < length; i++) {
         this->thisReference->value = result[i];
         this->inner.index.value = _numi(static_cast<_nint>(i));

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->indices->values[i] = i;
         this->order->addValues();
      }

      this->quicksort(0, length - 1);

      this->inner.index.value = prevIndex;
      this->inner.depth.value = prevDepth;
      this->thisReference->value = prevThis;

      return result;
   }

private:
   _genptr<std::vector<T>> base;
};


struct OrderBy_Definition : OrderBy<_str>, _def
{
public:
   OrderBy_Definition(_defptr& bas, _attrptr& attr, const _bool& hasMem, _indptr& inds, _ordptr& ord, Uroboros& uro);

   void reset() override;
   _bool hasNext() override;

private:
   _defptr base;
   _bool first = true;
   Uroboros& uroboros;
   InnerVariables& inner;
   const _bool hasMemory;
   AttributeMemory attrMemory;

   _size length;
   _size index;
   _list result;

   P_MEMORY_MEMBER;

   _bool hasVolatileDepth;
   std::vector<_nint> depths;
};

}

#endif /* ORDER_H */
