/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSE_GENERIC_H
#define PARSE_GENERIC_H

#include "../parse-gen.h"
#include "../../util.h"
#include "../order.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-list.h"
#include "parse-order.h"
#include "parse-var.h"


namespace perun2::parse
{

p_bool parseListElementIndex(p_genptr<p_num>& result, const Tokens& tks, p_perun2& p2);
void checkLimitBySize(const Tokens& tks, p_perun2& p2);

template <typename T>
static p_bool parseTernary(p_genptr<T>& result, const Tokens& tks, p_perun2& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return false;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   p_genptr<p_bool> condition;
   if (!parse(p2, std::get<0>(trio), condition)) {
      return false;
   }

   p_genptr<T> left;
   if (!parse(p2, std::get<1>(trio), left)) {
      return false;
   }

   p_genptr<T> right;
   if (!parse(p2, std::get<2>(trio), right)) {
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
static p_bool parseBinary(p_genptr<T>& result, const Tokens& tks, p_perun2& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return false;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_QUESTION_MARK);

   p_genptr<p_bool> condition;
   if (!parse(p2, pair.first, condition)) {
      return false;
   }

   p_genptr<T> value;
   if (!parse(p2, pair.second, value)) {
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
static p_bool parseListedValues(p_genptr<std::vector<T>>& res,
   const std::vector<Tokens>& elements, p_perun2& p2)
{
   const p_size len = elements.size();
   std::vector<p_genptr<T>> result;
   p_bool isConstant = true;

   for (p_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      p_genptr<T> value;
      if (parse(p2, tks, value)) {
         if (isConstant && !value->isConstant()) {
            isConstant = false;
         }
         result.push_back(std::move(value));
      }
      else {
         return false;
      }
   }

   p_genptr<std::vector<T>> v = std::make_unique<gen::Listed<T>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<std::vector<T>>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}


template <typename T>
static p_bool parseListedLists(p_genptr<std::vector<T>>& res,
   const std::vector<Tokens>& elements, p_perun2& p2)
{
   const p_size len = elements.size();
   std::vector<p_genptr<std::vector<T>>> result;
   p_bool isConstant = true;

   for (p_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      p_genptr<std::vector<T>> value;
      if (parse(p2, tks, value)) {
         if (isConstant && !value->isConstant()) {
            isConstant = false;
         }
         result.push_back(std::move(value));
      }
      else {
         return false;
      }
   }

   p_genptr<std::vector<T>> v = std::make_unique<gen::ListedLists<T>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<std::vector<T>>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}


template <typename T>
static p_bool parseListed(p_genptr<std::vector<T>>& result, const Tokens& tks, p_perun2& p2)
{
   if (!tks.check(TI_HAS_CHAR_COMMA)) {
      return false;
   }

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_COMMA);

   if (parseListedValues<T>(result, elements, p2)) {
      return true;
   }

   return parseListedLists<T>(result, elements, p2);
}


template <typename T>
static p_bool parseCollectionElement(p_genptr<T>& result, const Tokens& tks, p_perun2& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_LIST_ELEM)) {
      return false;
   }

   p_genptr<p_num> num;
   parseListElementIndex(num, tks, p2);
   const Token& f = tks.first();
   p_genptr<std::vector<T>> collection;
   if (makeVarRef(f, collection, p2)) {
      result = std::make_unique<gen::ListElement<T>>(collection, num);
      return true;
   }
   else {
      return false;
   }
}

}

#endif /* PARSE_GENERIC_H */
