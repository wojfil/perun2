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

#ifndef GEN_BOOL_H
#define GEN_BOOL_H

#include "../generator.h"
#include "../datatype.h"
#include "gen-generic.h"
#include <algorithm>


namespace perun2::gen
{

struct Not : UnaryOperation<_bool>
{
public:
   Not(_genptr<_bool>& val);
   _bool getValue() override;
};


struct And : BinaryOperation<_bool>
{
public:
   And(_genptr<_bool>& val1, _genptr<_bool>& val2);
   _bool getValue() override;
};


struct Or : BinaryOperation<_bool>
{
public:
   Or(_genptr<_bool>& val1, _genptr<_bool>& val2);
   _bool getValue() override;
};


struct Xor : BinaryOperation<_bool>
{
public:
   Xor(_genptr<_bool>& val1, _genptr<_bool>& val2);
   _bool getValue() override;
};


// IN operator straight outta SQL
// right side is variant, so is generated for every call
template <typename T>
struct InList : Generator<_bool>
{
public:
   InList<T>(_genptr<T>& val, _genptr<std::vector<T>>& li)
      : value(std::move(val)), list(std::move(li)) { };

   _bool getValue() override 
   {
      const std::vector<T> multipleValues = list->getValue();
      const T singleValue = value->getValue();

      for (const T& mv : multipleValues) {
         if (mv == singleValue) {
            return true;
         }
      }

      return false;
   };

private:
   _genptr<T> value;
   _genptr<std::vector<T>> list;
};


// an optimized variant of the IN operator
// is parsed if second argument is constant
template <typename T>
struct InConstList : Generator<_bool>
{
public:
   InConstList<T>(_genptr<T>& val, const std::vector<T>& li)
      : value(std::move(val)), list(li)
   {
      std::sort(list.begin(), list.end());
      list.erase(std::unique(list.begin(), list.end()), list.end());
   };

   _bool getValue() override
   {
      return std::binary_search(list.begin(), list.end(), value->getValue());
   };

private:
   _genptr<T> value;
   std::vector<T> list;
};


// Time works quite differently than other data types
// for example '3 June 2005' equals 'June 2005'
// so let there be a special case struct
struct InConstTimeList : Generator<_bool>
{
public:
   InConstTimeList(_genptr<_tim>& val, const _tlist& li);
   _bool getValue() override;

private:
   _genptr<_tim> value;
   const _tlist list;
};

}

#endif /* GEN_BOOL_H */
