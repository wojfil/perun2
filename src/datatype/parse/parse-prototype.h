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

#ifndef PARSE_PROTOTYPE_H_INCLUDED
#define PARSE_PROTOTYPE_H_INCLUDED

#include "../../uroboros.h"
#include "../generator/gen-generic.h"


namespace uro::parse
{

enum FilterType
{
   ft_Where,
   ft_Numeric
};

template <typename T>
struct FilterPrototype
{
public:
   virtual ~FilterPrototype() { };
   virtual FilterType getFilterType() = 0;
   virtual T build(T base, Attribute* attr, const _bool& hasMem, uro::Uroboros* uro) = 0;
};

template <typename T>
void makeWhereFilter(Generator<_bool>* boo, Attribute* attr,
   const _bool& hasMemory, Generator<std::vector<T>>*& result, uro::Uroboros* uro)
{
   result = new gen::Filter_Where<T>(result, boo, attr, uro);
}

template <typename T>
void makeWhereFilter(Generator<_bool>* boo, Generator<std::vector<T>>*& result, uro::Uroboros* uro)
{
   result = new gen::Filter_Where<T>(result, boo, nullptr, uro);
}

void makeWhereFilter(Generator<_bool>* boo, Attribute* attr, const _bool& hasMemory, _def*& result, uro::Uroboros* uro);
void makeWhereFilter(Generator<_bool>* boo, _def*& result, uro::Uroboros* uro);

template <typename T>
void makeNumericFilter(const Keyword& kw, Generator<_num>* num, Generator<std::vector<T>>*& result, uro::Uroboros* uro)
{
   switch (kw) {
      case Keyword::kw_Every: {
         result = new gen::Filter_Every<T>(result, num);
         break;
      }
      case Keyword::kw_Final: {
         result = new gen::Filter_Final<T>(result, num);
         break;
      }
      case Keyword::kw_Limit: {
         result = new gen::Filter_Limit<T>(result, num);
         break;
      }
      case Keyword::kw_Skip: {
         result = new gen::Filter_Skip<T>(result, num);
         break;
      }
   }
}

void makeNumericFilter(const Keyword& kw, Generator<_num>* num, _def*& result, uro::Uroboros* uro);

template <typename T>
struct FP_Where : FilterPrototype<T>
{
public:
   FP_Where(Generator<_bool>* cond)
      : condition(cond) { };

   ~FP_Where()
   {
      if (condition != nullptr) {
         delete condition;
      }
   };

   FilterType getFilterType() override
   {
      return FilterType::ft_Where;
   };

   T build(T base, Attribute* attr, const _bool& hasMem, uro::Uroboros* uro) override
   {
      T result = base;

      if (attr == nullptr) {
         makeWhereFilter(condition, result, uro);
      }
      else {
         makeWhereFilter(condition, attr, hasMem, result, uro);
      }

      condition = nullptr;
      return result;
   };

private:
   Generator<_bool>* condition;

};

template <typename T>
struct FP_Numeric : FilterPrototype<T>
{
public:
   FP_Numeric(Generator<_num>* val, const Keyword& kw)
      : value(val), keyword(kw) { };

   ~FP_Numeric()
   {
      if (value != nullptr) {
         delete value;
      }
   };

   FilterType getFilterType() override
   {
      return FilterType::ft_Numeric;
   };

   T build(T base, Attribute* attr, const _bool& hasMem, uro::Uroboros* uro) override
   {
      T result = base;
      makeNumericFilter(keyword, value, result, uro);
      value = nullptr;
      return result;
   };

private:
   Generator<_num>* value;
   const Keyword keyword;
};

}

#endif // PARSE_PROTOTYPE_H_INCLUDED
