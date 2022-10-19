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
#include "../generator/gen-list.h"
#include "parse-prototype.h"
#include "parse-order.h"
#include <vector>


namespace uro::parse
{

Generator<_num>* parseListElementIndex(const Tokens& tks, Uroboros& uro);
void checkLimitBySize(const Tokens& tks, Uroboros& uro);

template <typename T>
static Generator<T>* parseTernary(const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return nullptr;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   Generator<_bool>* condition;
   if (!parse(uro, std::get<0>(trio), condition)) {
      return nullptr;
   }

   Generator<T>* left;
   if (!parse(uro, std::get<1>(trio), left)) {
      delete condition;
      return nullptr;
   }

   Generator<T>* right;
   if (!parse(uro, std::get<2>(trio), right)) {
      delete condition;
      delete left;
      return nullptr;
   }

   if (condition->isConstant()) {
      if (condition->getValue()) {
         delete condition;
         delete right;

         if (left->isConstant()) {
            const T v = left->getValue();
            delete left;
            return new gen::Constant<T>(v);
         }
         else {
            return left;
         }
      }
      else {
         delete condition;
         delete left;

         if (right->isConstant()) {
            const T v = right->getValue();
            delete right;
            return new gen::Constant<T>(v);
         }
         else {
            return right;
         }
      }
   }

   return new gen::Ternary<T>(condition, left, right);
}


template <typename T>
static Generator<T>* parseBinary(const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return nullptr;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(L'?');

   Generator<_bool>* condition;
   if (!parse(uro, pair.first, condition)) {
      return nullptr;
   }

   Generator<T>* value;
   if (!parse(uro, pair.second, value)) {
      delete condition;
      return nullptr;
   }

   if (condition->isConstant()) {
      if (condition->getValue()) {
         if (value->isConstant()) {
            const T v = value->getValue();
            delete condition;
            delete value;
            return new gen::Constant<T>(v);
         }
         else {
            delete condition;
            return value;
         }
      }
      else {
         delete condition;
         delete value;
         return new gen::Constant<T>(T());
      }
   }

   return new gen::Binary<T>(condition, value);
}


template <typename T>
static Generator<std::vector<T>>* parseListedValues(const std::vector<Tokens>& elements, Uroboros& uro)
{
   const _size len = elements.size();
   std::vector<Generator<T>*>* result = new std::vector<Generator<T>*>();
   _bool isConstant = true;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<T>* value;
      if (parse(uro, tks, value)) {
         result->push_back(value);
         if (isConstant && !value->isConstant()) {
            isConstant = false;
         }
      }
      else {
         langutil::deleteVectorPtr(result);
         return nullptr;
      }
   }

   Generator<std::vector<T>>* v = new gen::Listed<T>(result);
   if (isConstant) {
      Generator<std::vector<T>>* cnst = new gen::Constant<std::vector<T>>(v->getValue());
      delete v;
      return cnst;
   }
   else {
      return v;
   }
}


template <typename T>
static Generator<std::vector<T>>* parseListedLists(const std::vector<Tokens>& elements, Uroboros& uro)
{
   const _size len = elements.size();
   std::vector<Generator<std::vector<T>>*>* result
      = new std::vector<Generator<std::vector<T>>*>();
   _bool isConstant = true;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<std::vector<T>>* value;
      if (parse(uro, tks, value)) {
         result->push_back(value);
         if (isConstant && !value->isConstant()) {
            isConstant = false;
         }
      }
      else {
         langutil::deleteVectorPtr(result);
         return nullptr;
      }
   }

   Generator<std::vector<T>>* v = new gen::ListedLists<T>(result);
   if (isConstant) {
      Generator<std::vector<T>>* cnst = new gen::Constant<std::vector<T>>(v->getValue());
      delete v;
      return cnst;
   }
   else {
      return v;
   }
}


template <typename T>
static Generator<std::vector<T>>* parseListed(const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_HAS_CHAR_COMMA)) {
      return nullptr;
   }

   const std::vector<Tokens> elements = tks.splitBySymbol(L',');

   Generator<std::vector<T>>* units = parseListedValues<T>(elements, uro);
   if (units != nullptr) {
      return units;
   }

   return parseListedLists<T>(elements, uro);
}


template <typename T>
static Generator<T>* parseCollectionElement(const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_LIST_ELEM)) {
      return nullptr;
   }

   Generator<_num>* num = parseListElementIndex(tks, uro);
   const Token& f = tks.first();
   Generator<std::vector<T>>* collection;
   if (uro.vars.getVarValue(f, collection)) {
      return new gen::ListElement<T>(collection, num);
   }
   else {
      delete num;
      return nullptr;
   }
}


static _bool parseFilterBase(const Tokens& tks, Uroboros& uro, _def*& result, _fdata*& data)
{
   if (parse::parse(uro, tks, result)) {
      data = result->getDataPtr();
      return true;
   }

   return false;
};


template <typename T>
static _bool parseFilterBase(const Tokens& tks, Uroboros& uro, Generator<T>*& result, _fdata*& data)
{
   data = nullptr;
   return parse::parse(uro, tks, result);
};


template <typename T>
static void buildFilterPrototypes(std::vector<FilterPrototype<T>*>& prototypes, Attribute*& attr,
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
      FilterPrototype<T>* fp = prototypes[i];
      if (i == lastWhereId) {
         base = fp->build(base, attr, hasMemory, uro);
      }
      else {
         base = fp->build(base, nullptr, hasMemory, uro);
      }
   }

   langutil::deleteVector(prototypes);
   prototypes.clear();

   if (isFinal) {
      if (hasAttr && lastWhereId == -1) {
         delete attr;
      }
   }
   else {
      if (hasAttr) {
         if (lastWhereId == -1) {
            delete attr;
         }
         attr = new Attribute(uro);
      }
   }
};


template <typename T, typename T2>
static T parseFilter(const Tokens& tks, const ThisState& state, Uroboros& uro)
{
   const _size firstKeywordId = tks.getFilterKeywordId();

   if (firstKeywordId == tks.getStart()) {
      throw SyntaxException(str(L"filter keyword '", tks.first().getOriginString(uro),
         L"' is not preceded by a collection of values"), tks.first().line);
   }
   else if (firstKeywordId == tks.getStart() + tks.getLength() - 1) {
      const Token& t = tks.listAt(firstKeywordId);
      throw SyntaxException(str(L"filter keyword '", t.getOriginString(uro),
         L"' cannot stand at the end of an expression"), t.line);
   }

   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   _fdata* fdata;
   T base;

   if (!parseFilterBase(tks2, uro, base, fdata)) {
      return nullptr;
   }

   // core
   const _int kw = firstKeywordId - tks.getStart() + 1;
   const _int start = tks.getStart() + kw;
   const _int length = tks.getLength() - kw;
   const _bool hasMemory = uro.vc.anyAttribute();
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(uro);
   const _size flength = filterTokens.size();
   std::vector<FilterPrototype<T>*> prototypes;

   // attribute
   const _bool hasAttr = (state == ThisState::ts_String);
   Attribute* attr;
   _bool hasBridgeAttr = (fdata != nullptr);

   if (hasAttr) {
      if (hasBridgeAttr) {
         attr = new BridgeAttribute(uro, fdata);
      }
      else {
         attr = new Attribute(uro);
      }
   }
   else {
      attr = nullptr;
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

            Generator<_num>* num;
            if (!parse(uro, ts, num)) {
               langutil::deleteVector(prototypes);
               throw SyntaxException(str(L"tokens after keyword '", tsf.getOriginString(uro),
                  L"' cannot be resolved to a number"), tsf.line);
            }

            prototypes.push_back(new FP_Numeric<T>(num, kw));
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = uro.vars.inner.thisState;
            uro.vars.inner.thisState = state;

            if (hasAttr) {
               uro.vc.addAttribute(attr);
            }

            Generator<_bool>* boo;
            if (!parse(uro, ts, boo)) {
               langutil::deleteVector(prototypes);
               uro.vars.inner.thisState = prevThisState;
               throw SyntaxException(str(L"tokens after keyword '", tsf.getOriginString(uro),
                  L"' cannot be resolved to a logic condition"), tsf.line);
            }

            prototypes.push_back(new FP_Where<T>(boo));
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
   return base;
}

}

#endif /* PARSE_GENERIC_H */
