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

#ifndef GEN_BOOL_H
#define GEN_BOOL_H

#include <algorithm>
#include "../generator.h"
#include "../datatype.h"
#include "gen-generic.h"
#include "gen-bool-compare.h"


// logic negation:
struct Not : UnaryOperation<_boo>
{
   Not(Generator<_boo>* val) : UnaryOperation<_boo>(val) { };

   _boo getValue() override;
};


// three basic gates:
struct And : BinaryOperation<_boo>
{
   And(Generator<_boo>* val1, Generator<_boo>* val2)
      : BinaryOperation<_boo>(val1, val2) { };

   _boo getValue() override;
};


struct Or : BinaryOperation<_boo>
{
   Or(Generator<_boo>* val1, Generator<_boo>* val2)
      : BinaryOperation<_boo>(val1, val2) { };

   _boo getValue() override;
};


struct Xor : BinaryOperation<_boo>
{
   Xor(Generator<_boo>* val1, Generator<_boo>* val2)
      : BinaryOperation<_boo>(val1, val2) { };

   _boo getValue() override;
};


// IN operator straight from SQL
template <typename T>
struct InList : Generator<_boo>
{
public:
   InList<T> (Generator<T>* val, Generator<std::vector<T>>* li)
      : value(val), list(li), hasPrev(false) { };

   ~InList<T>() {
      delete value;
      delete list;
   };

   _boo getValue() override {

      if (hasPrev && prevLength == 0) {
         return false;
      }

      std::vector<T> lst = list->getValue();

      if (!hasPrev || lst != prevRaw) {
         prevRaw = lst;
         prevSorted = lst;

         std::sort(prevSorted.begin(), prevSorted.end());
         lst.erase(std::unique(prevSorted.begin(), prevSorted.end()),
            prevSorted.end());

         if (!hasPrev) {
            hasPrev = true;
         }

         prevLength = prevSorted.size();
      }

      return prevLength == 1
         ? prevSorted[0] == value->getValue()
         : std::binary_search(prevSorted.begin(), prevSorted.end(),
               value->getValue());
   };

private:
   _boo hasPrev;
   std::vector<T> prevRaw;
   std::vector<T> prevSorted;
   _size prevLength;

   Generator<T>* value;
   Generator<std::vector<T>>* list;
};


// an optimized variant of the IN operator
// is parsed if second argument is constant
template <typename T>
struct InConstList : Generator<_boo>
{
public:
   InConstList<T> (Generator<T>* val, std::vector<T> li)
      : value(val)
   {
      std::sort(li.begin(), li.end());
      li.erase(std::unique(li.begin(), li.end()), li.end());
      list = li;
   };

   ~InConstList<T>() {
      delete value;
   };

   _boo getValue() override  {
      return std::binary_search(list.begin(), list.end(), value->getValue());
   };

private:
   Generator<T>* value;
   std::vector<T> list;
};


struct InTimeList : Generator<_boo>
{
public:
   InTimeList(Generator<_tim>* val, Generator<_tlist>* li)
      : value(val), list(li) { };

   ~InTimeList() {
      delete value;
      delete list;
   };

   _boo getValue() override;

private:
   Generator<_tim>* value;
   Generator<_tlist>* list;
};


struct InConstTimeList : Generator<_boo>
{
public:
   InConstTimeList(Generator<_tim>* val, _tlist li)
      : value(val), list(li), length(li.size()) { };

   ~InConstTimeList() {
      delete value;
   };

   _boo getValue() override;

private:
   Generator<_tim>* value;
   const _tlist list;
   const _size length;
};


#endif /* GEN_BOOL_H */

