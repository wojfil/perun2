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

#ifndef PARSE_GENERIC_H
#define PARSE_GENERIC_H

#include "../parse-gen.h"
#include "../../util.h"
#include "../order.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-list.h"
#include "parse-order.h"
#include <vector>


namespace uro::parse
{

_bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, _uro& uro);
void checkLimitBySize(const Tokens& tks, _uro& uro);

template <typename T>
static _bool parseTernary(_genptr<T>& result, const Tokens& tks, _uro& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return false;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   _genptr<_bool> condition;
   if (!parse(uro, std::get<0>(trio), condition)) {
      return false;
   }

   _genptr<T> left;
   if (!parse(uro, std::get<1>(trio), left)) {
      return false;
   }

   _genptr<T> right;
   if (!parse(uro, std::get<2>(trio), right)) {
      return false;
   }

   if (condition->isConstant()) {
      if (condition->getValue()) {
         if (left->isConstant()) {
            result = std::make_unique<gen::Constant<T>>(left->getValue());
         }
         else {
            result = std::move(left);
         }
      }
      else {
         if (right->isConstant()) {
            result = std::make_unique<gen::Constant<T>>(right->getValue());
         }
         else {
            result = std::move(right);
         }
      }
      return true;
   }

   result = std::make_unique<gen::Ternary<T>>(condition, left, right);
   return true;
}


template <typename T>
static _bool parseBinary(_genptr<T>& result, const Tokens& tks, _uro& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return false;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_QUESTION_MARK);

   _genptr<_bool> condition;
   if (!parse(uro, pair.first, condition)) {
      return false;
   }

   _genptr<T> value;
   if (!parse(uro, pair.second, value)) {
      return false;
   }

   if (condition->isConstant()) {
      if (condition->getValue()) {
         if (value->isConstant()) {
            result = std::make_unique<gen::Constant<T>>(value->getValue());
         }
         else {
            result = std::move(value);
         }
      }
      else {
         result = std::make_unique<gen::Constant<T>>(T());
      }

      return true;
   }

   result = std::make_unique<gen::Binary<T>>(condition, value);
   return true;
}


template <typename T>
static _bool parseListedValues(_genptr<std::vector<T>>& res,
   const std::vector<Tokens>& elements, _uro& uro)
{
   const _size len = elements.size();
   std::vector<_genptr<T>> result;
   _bool isConstant = true;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _genptr<T> value;
      if (parse(uro, tks, value)) {
         if (isConstant && !value->isConstant()) {
            isConstant = false;
         }
         result.push_back(std::move(value));
      }
      else {
         return false;
      }
   }

   _genptr<std::vector<T>> v = std::make_unique<gen::Listed<T>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<std::vector<T>>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}


template <typename T>
static _bool parseListedLists(_genptr<std::vector<T>>& res,
   const std::vector<Tokens>& elements, _uro& uro)
{
   const _size len = elements.size();
   std::vector<_genptr<std::vector<T>>> result;
   _bool isConstant = true;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _genptr<std::vector<T>> value;
      if (parse(uro, tks, value)) {
         if (isConstant && !value->isConstant()) {
            isConstant = false;
         }
         result.push_back(std::move(value));
      }
      else {
         return false;
      }
   }

   _genptr<std::vector<T>> v = std::make_unique<gen::ListedLists<T>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<std::vector<T>>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}


template <typename T>
static _bool parseListed(_genptr<std::vector<T>>& result, const Tokens& tks, _uro& uro)
{
   if (!tks.check(TI_HAS_CHAR_COMMA)) {
      return false;
   }

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_COMMA);

   if (parseListedValues<T>(result, elements, uro)) {
      return true;
   }

   return parseListedLists<T>(result, elements, uro);
}


template <typename T>
static _bool parseCollectionElement(_genptr<T>& result, const Tokens& tks, _uro& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_LIST_ELEM)) {
      return false;
   }

   _genptr<_num> num;
   parseListElementIndex(num, tks, uro);
   const Token& f = tks.first();
   _genptr<std::vector<T>> collection;
   if (uro.contexts.makeVarRef(f, collection, uro)) {
      result = std::make_unique<gen::ListElement<T>>(collection, num);
      return true;
   }
   else {
      return false;
   }
}

}

#endif /* PARSE_GENERIC_H */
