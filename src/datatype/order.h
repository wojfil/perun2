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
   void prepare(const p_size length);

   std::unique_ptr<p_size[]> values;
};

typedef std::unique_ptr<OrderIndices> _indptr;


struct Order
{
public:
   virtual void clearValues(const p_size length) = 0;
   virtual void clearValues() = 0;
   virtual void addValues() = 0;
   virtual p_bool matchesSwap(const p_int start, const p_int end) const = 0;
};

typedef std::unique_ptr<Order> _ordptr;


template <typename T>
struct OrderUnit : Order
{
public:
   OrderUnit() = delete;
   OrderUnit(_genptr<T>& val, const p_bool desc, OrderIndices* inds)
      : valueGenerator(std::move(val)), descending(desc), indices(inds) { };

protected:
   _genptr<T> valueGenerator;
   OrderIndices* indices;
   std::vector<T> values;
   const p_bool descending;
};


template <typename T>
struct OrderUnit_Middle : OrderUnit<T>
{
public:
   OrderUnit_Middle() = delete;
   OrderUnit_Middle(_genptr<T>& val, const p_bool desc, _ordptr& next, OrderIndices* inds)
      : OrderUnit<T>(val, desc, inds), nextUnit(std::move(next)) { };

   void clearValues(const p_size length) override
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

   p_bool matchesSwap(const p_int start, const p_int end) const override
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
   OrderUnit_Final(_genptr<T>& val, const p_bool desc, OrderIndices* inds)
      : OrderUnit<T>(val, desc, inds) { };

   void clearValues(const p_size length) override
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

   p_bool matchesSwap(const p_int start, const p_int end) const override
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

   void quicksort(p_int start, p_int end);
   p_int partition(const p_int start, const p_int end);

protected:
   _indptr indices;
   _ordptr order;
   p_list* resultPtr = nullptr;
};


struct OrderBy_List : OrderBy, Generator<p_list>
{
public:
   OrderBy_List() = delete;
   OrderBy_List(_genptr<p_list>& bas, _fcptr& ctx, _indptr& inds, _ordptr& ord, pp_perun2& p2);

   p_list getValue() override;

private:
   _fcptr context;
   _genptr<p_list> base;
};


struct OrderBy_Definition : OrderBy, p_def
{
public:
   OrderBy_Definition() = delete;
   OrderBy_Definition(p_defptr& bas, FileContext* ctx, _fcptr& nextCtx, _indptr& inds, _ordptr& ord, pp_perun2& p2);
   FileContext* getFileContext() override;

   void reset() override;
   p_bool hasNext() override;

private:
   FileContext* fileContext;
   _fcptr nextContext;
   p_defptr base;
   p_bool first = true;
   pp_perun2& perun2;

   p_size length;
   p_size index;
   p_list result;
};

}

#endif /* ORDER_H */
