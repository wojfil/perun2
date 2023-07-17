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

#ifndef ORDER_LIMIT_ONE_H_INCLUDED
#define ORDER_LIMIT_ONE_H_INCLUDED

#include "generator.h"
#include "datatype.h"
#include "../util.h"
#include "../perun2.h"


namespace perun2::gen
{

// when Perun2 code look like this:
//
//      select '*.pdf'
//         order by creation desc
//         limit 1
//
// there is no point in sorting everything
// we only want one element from the collection
// so instead of sorting, let us iterate through elements and find min/max


struct LimitOne
{
public:
   virtual void loadData() = 0;
   virtual p_bool nextElementIsBetter() = 0;
};

typedef std::unique_ptr<LimitOne> p_loptr;


template <typename T>
struct LimitOneUnit : LimitOne
{
public:
   LimitOneUnit() = delete;
   LimitOneUnit(p_genptr<T>& val, const p_bool desc)
      : valueGenerator(std::move(val)), descending(desc) { };

   void loadData() override
   {
      this->value = this->valueGenerator->getValue();
   };

protected:
   T value;
   T nextValue;
   p_genptr<T> valueGenerator;
   const p_bool descending;
};


template <typename T>
struct LimitOneUnit_Middle : LimitOneUnit<T>
{
public:
   LimitOneUnit_Middle() = delete;
   LimitOneUnit_Middle(p_genptr<T>& val, const p_bool desc, p_loptr& next)
      : LimitOneUnit<T>(val, desc), nextUnit(std::move(next)) { };

   void loadData() override
   {
      this->value = this->valueGenerator->getValue();
      this->nextUnit->loadData();
   };

   p_bool nextElementIsBetter() override
   {
      this->nextValue = this->valueGenerator->getValue();

      if (this->descending ? (this->nextValue > this->value) : (this->nextValue < this->value)) {
         this->nextUnit->loadData();
         return true;
      }

      return this->nextUnit->nextElementIsBetter();
   };

private:
   p_loptr nextUnit;
};


template <typename T>
struct LimitOneUnit_Final : LimitOneUnit<T>
{
public:
   LimitOneUnit_Final() = delete;
   LimitOneUnit_Final(p_genptr<T>& val, const p_bool desc)
      : LimitOneUnit<T>(val, desc) { };

   p_bool nextElementIsBetter() override
   {
      this->nextValue = this->valueGenerator->getValue();
      if (this->descending ? (this->nextValue > this->value) : (this->nextValue < this->value)) {
         this->value = this->nextValue;
         return true;
      }

      return false;
   };
};


struct OrderByLimitOne : p_def
{
public:
   OrderByLimitOne() = delete;
   OrderByLimitOne(p_defptr& bas, FileContext* ctx, p_fcptr& nextCtx, p_loptr& lo, p_perun2& p2);
   FileContext* getFileContext() override;

   void reset() override;
   p_bool hasNext() override;

private:
   p_defptr base;
   FileContext* fileContext;
   p_fcptr nextContext;
   p_loptr limitOne;
   p_perun2& perun2;
   p_bool valueTaken = false;
};


}

#endif // ORDER_LIMIT_ONE_H_INCLUDED
