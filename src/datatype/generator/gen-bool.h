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

struct Not : UnaryOperation<p_bool>
{
public:
   Not(p_genptr<p_bool>& val);
   p_bool getValue() override;
};


struct And : BinaryOperation<p_bool>
{
public:
   And(p_genptr<p_bool>& val1, p_genptr<p_bool>& val2);
   p_bool getValue() override;
};


struct Or : BinaryOperation<p_bool>
{
public:
   Or(p_genptr<p_bool>& val1, p_genptr<p_bool>& val2);
   p_bool getValue() override;
};


struct Xor : BinaryOperation<p_bool>
{
public:
   Xor(p_genptr<p_bool>& val1, p_genptr<p_bool>& val2);
   p_bool getValue() override;
};


// IN operator straight outta SQL
// right side is variant, so is generated for every call
template <typename T>
struct InList : Generator<p_bool>
{
public:
   InList<T>(p_genptr<T>& val, p_genptr<std::vector<T>>& li)
      : value(std::move(val)), list(std::move(li)) { };

   p_bool getValue() override 
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
   p_genptr<T> value;
   p_genptr<std::vector<T>> list;
};


// an optimized variant of the IN operator
// is parsed if second argument is constant
template <typename T>
struct InConstList : Generator<p_bool>
{
public:
   InConstList<T>(p_genptr<T>& val, const std::vector<T>& li)
      : value(std::move(val)), list(li)
   {
      std::sort(list.begin(), list.end());
      list.erase(std::unique(list.begin(), list.end()), list.end());
   };

   p_bool getValue() override
   {
      return std::binary_search(list.begin(), list.end(), value->getValue());
   };

private:
   p_genptr<T> value;
   std::vector<T> list;
};


// Time works quite differently than other data types
// for example '3 June 2005' equals 'June 2005'
// so let there be a special case struct
struct InConstTimeList : Generator<p_bool>
{
public:
   InConstTimeList(p_genptr<p_tim>& val, const p_tlist& li);
   p_bool getValue() override;

private:
   p_genptr<p_tim> value;
   const p_tlist list;
};

}

#endif /* GEN_BOOL_H */
