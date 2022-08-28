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
#include "../print.h"


// the base
template <typename T>
struct OrderBase : Generator<std::vector<T>>
{
public:
   virtual _boo hasVolatileDepth() const = 0;
   virtual _numi getDepth(const _size& index) const = 0;
   virtual void shuffleDepth(const std::vector<_size>& indices, const _size& length) { }; // to be overriden
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


template <typename T>
struct OrderBy : Generator<std::vector<T>>
{
public:
   OrderBy(OrderBase<T>* bas, Attribute* attr, const _boo& desc, Uroboros* uro)
      : base(bas), attribute(attr), descending(desc), inner(&uro->vars.inner), hasAttribute(attr != nullptr)
   {
      this->inner->createThisVarRef(thisReference);
   }

   ~OrderBy()
   {
      delete base;
      delete attribute;
   }

   virtual void clearValues(const _size& length) = 0;
   virtual void addValues() = 0;
   virtual _boo matchesSwap(const _int& start, const _int& end) const = 0;

   std::vector<T> getValue() override
   {
      this->result = this->base->getValue();
      const _size length = this->result.size();

      if (length == 0) {
         return this->result;
      }

      clearAndReserve(this->indices, length);
      clearValues(length);

      const _numi prevIndex = this->inner->index.value;
      const T prevThis = this->thisReference->value;
      const _boo hasVolatileDepth = this->base->hasVolatileDepth();

      for (_size i = 0; i < length; i++) {
         this->thisReference->value = this->result[i];
         this->inner->index.value = _numi(static_cast<_nint>(i));

         if (this->hasAttribute) {
            this->attribute->run();
         }
         this->indices[i] = i;
         if (hasVolatileDepth) {
            this->inner->depth.value = this->base->getDepth(i);
         }

         addValues();
      }

      this->quicksort(0, length - 1);

      this->inner->index.value = prevIndex;
      this->thisReference->value = prevThis;

      shuffleByIndices(this->result, this->indices, length);

      if (hasVolatileDepth) {
         this->base->shuffleDepth(this->indices, length);
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
         if (this->matchesSwap(j, end)) {
            i++;
            std::iter_swap(this->indices.begin() + i, this->indices.begin() + j);
         }
      }

      const _int ip = i + 1;
      std::iter_swap(indices.begin() + ip, indices.begin() + end);
      return ip;
   }

protected:
   std::vector<T> result;
   std::vector<_size> indices;
   OrderBase<T>* base;
   Attribute* attribute;
   const _boo hasAttribute;
   const _boo descending;
   InnerVariables* inner;
   Variable<T>* thisReference;
};


template <typename T, typename U1>
struct OrderBy_OneValue : OrderBy<T>
{
public:
   OrderBy_OneValue(OrderBase<T>* bas, Generator<U1>* vg,
      Attribute* attr, const _boo& desc, Uroboros* uro)
      : OrderBy<T>(bas, attr, desc, uro), valueGenerator(vg) { }

   ~OrderBy_OneValue()
   {
      delete valueGenerator;
   }

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
        ? this->values[this->indices[start]] >= this->values[this->indices[end]]
        : this->values[this->indices[start]] <= this->values[this->indices[end]];
   }

private:
   std::vector<U1> values;
   Generator<U1>* valueGenerator;
};


template <typename T, typename U1, typename U2>
struct OrderBy_TwoValues : OrderBy<T>
{
public:
   OrderBy_TwoValues(OrderBase<T>* bas, Generator<U1>* vg_1, Generator<U2>* vg_2,
      Attribute* attr, const _boo& desc, Uroboros* uro)
      : OrderBy<T>(bas, attr, desc, uro), valueGenerator_1(vg_1), valueGenerator_2(vg_2) { }

   ~OrderBy_TwoValues()
   {
      delete valueGenerator_1;
      delete valueGenerator_2;
   }

   void clearValues(const _size& length) override
   {
      clearAndReserve(this->values_1, length);
      clearAndReserve(this->values_2, length);
   }

   void addValues() override
   {
      this->values_1.push_back(this->valueGenerator_1->getValue());
      this->values_2.push_back(this->valueGenerator_2->getValue());
   }

   _boo matchesSwap(const _int& start, const _int& end) const override
   {
      if (this->values_1[this->indices[start]] == this->values_1[this->indices[end]]) {
         return this->descending
            ? this->values_2[this->indices[start]] >= this->values_2[this->indices[end]]
            : this->values_2[this->indices[start]] <= this->values_2[this->indices[end]];
      }

      return this->descending
        ? this->values_1[this->indices[start]] > this->values_1[this->indices[end]]
        : this->values_1[this->indices[start]] < this->values_1[this->indices[end]];
   }

private:
   std::vector<U1> values_1;
   std::vector<U2> values_2;
   Generator<U1>* valueGenerator_1;
   Generator<U2>* valueGenerator_2;
};


template <typename T, typename U1, typename U2, typename U3>
struct OrderBy_ThreeValues : OrderBy<T>
{
public:
   OrderBy_ThreeValues(OrderBase<T>* bas, Generator<U1>* vg_1, Generator<U2>* vg_2, Generator<U3>* vg_3,
      Attribute* attr, const _boo& desc, Uroboros* uro)
      : OrderBy<T>(bas, attr, desc, uro), valueGenerator_1(vg_1), valueGenerator_2(vg_2), valueGenerator_3(vg_3) { }

   ~OrderBy_ThreeValues()
   {
      delete valueGenerator_1;
      delete valueGenerator_2;
      delete valueGenerator_3;
   }

   void clearValues(const _size& length) override
   {
      clearAndReserve(this->values_1, length);
      clearAndReserve(this->values_2, length);
      clearAndReserve(this->values_3, length);
   }

   void addValues() override
   {
      this->values_1.push_back(this->valueGenerator_1->getValue());
      this->values_2.push_back(this->valueGenerator_2->getValue());
      this->values_3.push_back(this->valueGenerator_3->getValue());
   }

   _boo matchesSwap(const _int& start, const _int& end) const override
   {
      if (this->values_1[this->indices[start]] == this->values_1[this->indices[end]]) {
         if (this->values_2[this->indices[start]] == this->values_2[this->indices[end]]) {
            return this->descending
               ? this->values_3[this->indices[start]] >= this->values_3[this->indices[end]]
               : this->values_3[this->indices[start]] <= this->values_3[this->indices[end]];
         }

         return this->descending
            ? this->values_2[this->indices[start]] > this->values_2[this->indices[end]]
            : this->values_2[this->indices[start]] < this->values_2[this->indices[end]];
      }

      return this->descending
        ? this->values_1[this->indices[start]] > this->values_1[this->indices[end]]
        : this->values_1[this->indices[start]] < this->values_1[this->indices[end]];
   }

private:
   std::vector<U1> values_1;
   std::vector<U2> values_2;
   std::vector<U3> values_3;
   Generator<U1>* valueGenerator_1;
   Generator<U2>* valueGenerator_2;
   Generator<U3>* valueGenerator_3;
};


#endif /* ORDER_H */
