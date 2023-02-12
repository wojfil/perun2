/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSE_PROTOTYPE_H_INCLUDED
#define PARSE_PROTOTYPE_H_INCLUDED

#include "../../uroboros.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-generic-filter.h"
#include <memory>


namespace uro::parse
{
/*
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
   virtual void build(T& result, _attrptr& attr, const _bool& hasMem, _uro& uro) = 0;
};

template <typename T>
using _fpptr = std::unique_ptr<FilterPrototype<T>>;

template <typename T>
void makeWhereFilter(_genptr<_bool>& boo, _attrptr& attr,
   const _bool& hasMemory, _genptr<std::vector<T>>& result, _uro& uro)
{
   _genptr<std::vector<T>> prev = std::move(result);
   result = std::make_unique<gen::Filter_Where<T>>(prev, boo, attr, uro);
}

template <typename T>
void makeWhereFilter(_genptr<_bool>& boo, _genptr<std::vector<T>>& result, _uro& uro)
{
   _genptr<std::vector<T>> prev = std::move(result);
   result = std::make_unique<gen::Filter_Where<T>>(prev, boo, uro);
}

void makeWhereFilter(_genptr<_bool>& boo, _attrptr& attr, const _bool& hasMemory, _defptr& result, _uro& uro);
void makeWhereFilter(_genptr<_bool>& boo, _defptr& result, _uro& uro);

template <typename T>
void makeNumericFilter(const Keyword& kw, _genptr<_num>& num, _genptr<std::vector<T>>& result, _uro& uro)
{
   _genptr<std::vector<T>> prev = std::move(result);

   switch (kw) {
      case Keyword::kw_Every: {
         result = std::make_unique<gen::Filter_Every<T>>(prev, num);
         break;
      }
      case Keyword::kw_Final: {
         result = std::make_unique<gen::Filter_Final<T>>(prev, num);
         break;
      }
      case Keyword::kw_Limit: {
         result = std::make_unique<gen::Filter_Limit<T>>(prev, num);
         break;
      }
      case Keyword::kw_Skip: {
         result = std::make_unique<gen::Filter_Skip<T>>(prev, num);
         break;
      }
   }
}

void makeNumericFilter(const Keyword& kw, _genptr<_num>& num, _defptr& result, _uro& uro);

template <typename T>
struct FP_Where : FilterPrototype<T>
{
public:
   FP_Where(_genptr<_bool>& cond)
      : condition(std::move(cond)) { };

   FilterType getFilterType() override
   {
      return FilterType::ft_Where;
   };

   void build(T& result, _attrptr& attr, const _bool& hasMem, _uro& uro) override
   {
      if (attr) {
         makeWhereFilter(condition, attr, hasMem, result, uro);
      }
      else {
         makeWhereFilter(condition, result, uro);
      }
   };

private:
   _genptr<_bool> condition;

};

template <typename T>
struct FP_Numeric : FilterPrototype<T>
{
public:
   FP_Numeric(_genptr<_num>& val, const Keyword& kw)
      : value(std::move(val)), keyword(kw) { };

   FilterType getFilterType() override
   {
      return FilterType::ft_Numeric;
   };

   void build(T& result, _attrptr& attr, const _bool& hasMem, _uro& uro) override
   {
      makeNumericFilter(keyword, value, result, uro);
   };

private:
   _genptr<_num> value;
   const Keyword keyword;
};*/

}

#endif // PARSE_PROTOTYPE_H_INCLUDED
