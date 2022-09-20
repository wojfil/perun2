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

#include "../generator.h"
#include "../datatype.h"
#include "gen-generic.h"
#include "gen-bool-compare.h"
#include <algorithm>


namespace uro::gen
{

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


// IN operator straight outta SQL
// right side is variant, so is generated for every call
template <typename T>
struct InList : Generator<_boo>
{
public:
   InList<T> (Generator<T>* val, Generator<std::vector<T>>* li)
      : value(val), list(li) { };

   ~InList<T>() {
      delete value;
      delete list;
   };

   _boo getValue() override {
      const std::vector<T> vs = list->getValue();
      const T v = value->getValue();
      const _size len = vs.size();

      for (_size i = 0; i < len; i++) {
         if (vs[i] == v) {
            return true;
         }
      }

      return false;
   };

private:
   Generator<T>* value;
   Generator<std::vector<T>>* list;
};


// an optimized variant of the IN operator
// is parsed if second argument is constant
template <typename T>
struct InConstList : Generator<_boo>
{
public:
   InConstList<T> (Generator<T>* val, const std::vector<T>& li)
      : value(val), list(li)
   {
      std::sort(list.begin(), list.end());
      list.erase(std::unique(list.begin(), list.end()), list.end());
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


// Time works quite differently than other data types
// for example '3 June 2005' equals 'June 2005'
// so let there be special a case struct
struct InConstTimeList : Generator<_boo>
{
public:
   InConstTimeList(Generator<_tim>* val, const _tlist& li);
   ~InConstTimeList();

   _boo getValue() override;

private:
   Generator<_tim>* value;
   const _tlist list;
};

}

#endif /* GEN_BOOL_H */
