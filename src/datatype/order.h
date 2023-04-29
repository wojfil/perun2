/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ORDER_H
#define ORDER_H

#include "generator.h"
#include "datatype.h"
#include "../util.h"
#include "../perun2.h"


namespace perun2::gen
{

struct OrderIndices
{
public:
   void prepare(const _size length);

   std::unique_ptr<_size[]> values;
};

typedef std::unique_ptr<OrderIndices> _indptr;


struct Order
{
public:
   virtual void clearValues(const _size length) = 0;
   virtual void clearValues() = 0;
   virtual void addValues() = 0;
   virtual _bool matchesSwap(const _int start, const _int end) const = 0;
};

typedef std::unique_ptr<Order> _ordptr;


template <typename T>
struct OrderUnit : Order
{
public:
   OrderUnit() = delete;
   OrderUnit(_genptr<T>& val, const _bool desc, _indptr& inds)
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
   OrderUnit_Middle() = delete;
   OrderUnit_Middle(_genptr<T>& val, const _bool desc, _ordptr& next, _indptr& inds)
      : OrderUnit<T>(val, desc, inds), nextUnit(std::move(next)) { };

   void clearValues(const _size length) override
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

   _bool matchesSwap(const _int start, const _int end) const override
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
   OrderUnit_Final() = delete;
   OrderUnit_Final(_genptr<T>& val, const _bool desc, _indptr& inds)
      : OrderUnit<T>(val, desc, inds) { };

   void clearValues(const _size length) override
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

   _bool matchesSwap(const _int start, const _int end) const override
   {
      return this->descending
         ? this->values[this->indices->values[start]] >= this->values[this->indices->values[end]]
         : this->values[this->indices->values[start]] <= this->values[this->indices->values[end]];
   }
};


struct OrderBy
{
public:
   OrderBy() = delete;
   OrderBy(_indptr& inds, _ordptr& ord);

   void quicksort(_int start, _int end);
   _int partition(const _int start, const _int end);

protected:
   _indptr indices;
   _ordptr order;
   _list* resultPtr = nullptr;
};


struct OrderBy_List : OrderBy, Generator<_list>
{
public:
   OrderBy_List() = delete;
   OrderBy_List(_genptr<_list>& bas, _fcptr& ctx, _indptr& inds, _ordptr& ord, _p2& p2);

   _list getValue() override;

private:
   _fcptr context;
   _genptr<_list> base;
};


struct OrderBy_Definition : OrderBy, _def
{
public:
   OrderBy_Definition() = delete;
   OrderBy_Definition(_defptr& bas, FileContext* ctx, _fcptr& nextCtx, _indptr& inds, _ordptr& ord, _p2& p2);
   FileContext* getFileContext() override;

   void reset() override;
   _bool hasNext() override;

private:
   FileContext* fileContext;
   _fcptr nextContext;
   _defptr base;
   _bool first = true;
   _p2& perun2;

   _size length;
   _size index;
   _list result;
};

}

#endif /* ORDER_H */
