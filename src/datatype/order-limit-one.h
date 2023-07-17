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
   virtual _bool nextElementIsBetter() = 0;
};

typedef std::unique_ptr<LimitOne> _loptr;


template <typename T>
struct LimitOneUnit : LimitOne
{
public:
   LimitOneUnit() = delete;
   LimitOneUnit(_genptr<T>& val, const _bool desc)
      : valueGenerator(std::move(val)), descending(desc) { };

   void loadData() override
   {
      this->value = this->valueGenerator->getValue();
   };

protected:
   T value;
   T nextValue;
   _genptr<T> valueGenerator;
   const _bool descending;
};


template <typename T>
struct LimitOneUnit_Middle : LimitOneUnit<T>
{
public:
   LimitOneUnit_Middle() = delete;
   LimitOneUnit_Middle(_genptr<T>& val, const _bool desc, _loptr& next)
      : LimitOneUnit<T>(val, desc), nextUnit(std::move(next)) { };

   void loadData() override
   {
      this->value = this->valueGenerator->getValue();
      this->nextUnit->loadData();
   };

   _bool nextElementIsBetter() override
   {
      this->nextValue = this->valueGenerator->getValue();

      if (this->descending ? (this->nextValue > this->value) : (this->nextValue < this->value)) {
         this->nextUnit->loadData();
         return true;
      }

      return this->nextUnit->nextElementIsBetter();
   };

private:
   _loptr nextUnit;
};


template <typename T>
struct LimitOneUnit_Final : LimitOneUnit<T>
{
public:
   LimitOneUnit_Final() = delete;
   LimitOneUnit_Final(_genptr<T>& val, const _bool desc)
      : LimitOneUnit<T>(val, desc) { };

   _bool nextElementIsBetter() override
   {
      this->nextValue = this->valueGenerator->getValue();
      if (this->descending ? (this->nextValue > this->value) : (this->nextValue < this->value)) {
         this->value = this->nextValue;
         return true;
      }

      return false;
   };
};


struct OrderByLimitOne : _def
{
public:
   OrderByLimitOne() = delete;
   OrderByLimitOne(_defptr& bas, FileContext* ctx, _fcptr& nextCtx, _loptr& lo, p_perun2& p2);
   FileContext* getFileContext() override;

   void reset() override;
   _bool hasNext() override;

private:
   _defptr base;
   FileContext* fileContext;
   _fcptr nextContext;
   _loptr limitOne;
   p_perun2& perun2;
   _bool valueTaken = false;
};


}

#endif // ORDER_LIMIT_ONE_H_INCLUDED
