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

#ifndef PARSE_GENERIC_H
#define PARSE_GENERIC_H

#include "../parse-gen.h"
#include "../../util.h"
#include "../../var/var-runtime.h"
#include "../order.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-generic-filter.h"
#include "../generator/gen-list.h"
#include "parse-prototype.h"
#include "parse-order.h"
#include <vector>


namespace uro::parse
{

_bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, Uroboros& uro);
void checkLimitBySize(const Tokens& tks, Uroboros& uro);

template <typename T>
static _bool parseTernary(_genptr<T>& result, const Tokens& tks, Uroboros& uro)
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
static _bool parseBinary(_genptr<T>& result, const Tokens& tks, Uroboros& uro)
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
   const std::vector<Tokens>& elements, Uroboros& uro)
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

   _genptr<std::vector<T>> v(new gen::Listed<T>(result));

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
   const std::vector<Tokens>& elements, Uroboros& uro)
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

   _genptr<std::vector<T>> v(new gen::ListedLists<T>(result));

   if (isConstant) {
      res = std::make_unique<gen::Constant<std::vector<T>>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}


template <typename T>
static _bool parseListed(_genptr<std::vector<T>>& result, const Tokens& tks, Uroboros& uro)
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
static _bool parseCollectionElement(_genptr<T>& result, const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_LIST_ELEM)) {
      return false;
   }

   _genptr<_num> num;
   parseListElementIndex(num, tks, uro);
   const Token& f = tks.first();
   _genptr<std::vector<T>> collection;
   if (uro.vars.getVarValue(f, collection)) {
      result = std::make_unique<gen::ListElement<T>>(collection, num);
      return true;
   }
   else {
      return false;
   }
}


static _bool parseFilterBase(const Tokens& tks, Uroboros& uro, _defptr& result, _fdata*& data)
{
   if (parse::parse(uro, tks, result)) {
      data = result->getDataPtr();
      return true;
   }

   return false;
};


template <typename T>
static _bool parseFilterBase(const Tokens& tks, Uroboros& uro, _genptr<T>& result, _fdata*& data)
{
   data = nullptr;
   return parse::parse(uro, tks, result);
};


template <typename T>
static void buildFilterPrototypes(std::vector<_fpptr<T>>& prototypes, _attrptr& attr,
   const _bool& hasAttr, const _bool& isFinal, const _bool& hasMemory, Uroboros& uro, T& base)
{
   const _size fplen = prototypes.size();
   _int lastWhereId = -1;

   // look for index of the last Where filter in the list
   // only this one filter will run an Attribute
   // attributes of all Where filters are combined into this one
   for (_int i = fplen - 1; i >= 0; i--) {
      if (prototypes[i]->getFilterType() == FilterType::ft_Where) {
         lastWhereId = i;
         break;
      }
   }

   for (_size i = 0; i < fplen; i++) {
      _fpptr<T>& fp = prototypes[i];
      if (i == lastWhereId) {
         fp->build(base, attr, hasMemory, uro);
      }
      else {
         _attrptr a;
         fp->build(base, a, hasMemory, uro);
      }
   }

   prototypes.clear();

   if (hasAttr && !isFinal) {
      attr = std::make_unique<Attribute>(uro);
   }
};


template <typename T, typename T2>
static _bool parseFilter(T& result, const Tokens& tks, const ThisState& state, Uroboros& uro)
{
   const _size firstKeywordId = tks.getFilterKeywordId();

   if (firstKeywordId == tks.getStart()) {
      throw SyntaxError::filterKeywordAtStart(tks.first().getOriginString(uro), tks.first().line);
   }
   else if (firstKeywordId == tks.getStart() + tks.getLength() - 1) {
      const Token& t = tks.listAt(firstKeywordId);
      throw SyntaxError::filterKeywordAtEnd(t.getOriginString(uro), t.line);
   }

   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   _fdata* fdata;
   T base;

   if (!parseFilterBase(tks2, uro, base, fdata)) {
      return false;
   }

   // core
   const _int kw = firstKeywordId - tks.getStart() + 1;
   const _int start = tks.getStart() + kw;
   const _int length = tks.getLength() - kw;
   const _bool hasMemory = uro.vc.anyAttribute();
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(uro);
   const _size flength = filterTokens.size();
   std::vector<_fpptr<T>> prototypes;

   // attribute
   const _bool hasAttr = (state == ThisState::ts_String);
   _attrptr attr;
   _bool hasBridgeAttr = (fdata != nullptr);

   if (hasAttr) {
      if (hasBridgeAttr) {
         attr = std::make_unique<BridgeAttribute>(uro, fdata);
      }
      else {
         attr = std::make_unique<Attribute>(uro);
      }
   }

   for (_size i = 0; i < flength; i++) {
      Tokens& ts = filterTokens[i];
      const Token tsf = ts.first();
      const Keyword& kw = tsf.value.keyword.k;
      ts.trimLeft();

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Final:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts, uro);
            }

            _genptr<_num> num;
            if (!parse(uro, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(uro), tsf.line);
            }

            _fpptr<T> unit(new FP_Numeric<T>(num, kw));
            prototypes.push_back(std::move(unit));
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = uro.vars.inner.thisState;
            uro.vars.inner.thisState = state;

            if (hasAttr) {
               uro.vc.addAttribute(attr);
            }

            _genptr<_bool> boo;
            if (!parse(uro, ts, boo)) {
               throw SyntaxError::keywordNotFollowedByBool(tsf.getOriginString(uro), tsf.line);
            }

            _fpptr<T> unit(new FP_Where<T>(boo));
            prototypes.push_back(std::move(unit));
            uro.vars.inner.thisState = prevThisState;

            if (hasAttr) {
               uro.vc.retreatAttribute();
            }

            break;
         }
         case Keyword::kw_Order: {
            buildFilterPrototypes(prototypes, attr, hasAttr, false, hasMemory, uro, base);
            addOrderByFilter<T, T2>(base, state, tsf, ts, hasBridgeAttr ? fdata : nullptr, uro);
            hasBridgeAttr = false;
            break;
         }
      }
   }

   buildFilterPrototypes(prototypes, attr, hasAttr, true, hasMemory, uro, base);
   result = std::move(base);
   return true;
}

}

#endif /* PARSE_GENERIC_H */
