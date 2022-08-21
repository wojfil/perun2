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
#include <vector>


_boo isPossibleListElement(const Tokens& tks);
Generator<_num>* parseListElementIndex(const Tokens& tks, Uroboros* uro);
_boo isPossibleBinary(const Tokens& tks);
_boo isPossibleTernary(const Tokens& tks);
void checkLimitBySize(const Tokens& tks);
_boo isPossibleListElementMember(const Tokens& tks, Uroboros* uro);


template <typename T>
static Generator<T>* parseTernary(const Tokens& tks, Uroboros* uro)
{
   if (!isPossibleTernary(tks)) {
      return nullptr;
   }

   Tokens tks1(tks);
   Tokens tks2(tks);
   Tokens tks3(tks);
   tks.divideForTernary(tks1, tks2, tks3);

   Generator<_boo>* condition;
   if (!parse(uro, tks1, condition)) {
      return nullptr;
   }

   Generator<T>* left;
   if (!parse(uro, tks2, left)) {
      delete condition;
      return nullptr;
   }

   Generator<T>* right;
   if (!parse(uro, tks3, right)) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new Ternary<T>(condition, left, right);
}


template <typename T>
static Generator<T>* parseBinary(const Tokens& tks, Uroboros* uro)
{
   if (!isPossibleBinary(tks)) {
      return nullptr;
   }

   Tokens tks1(tks);
   Tokens tks2(tks);
   tks.divideBySymbol(L'?', tks1, tks2);

   Generator<_boo>* condition;
   if (!parse(uro, tks1, condition)) {
      return nullptr;
   }

   Generator<T>* value;
   if (!parse(uro, tks2, value)) {
      delete condition;
      return nullptr;
   }

   return new Binary<T>(condition, value);
}


template <typename T>
static Generator<std::vector<T>>* parseListedValues(const std::vector<Tokens>& elements, Uroboros* uro)
{
   const _size len = elements.size();
   std::vector<Generator<T>*>* result = new std::vector<Generator<T>*>();
   _boo isConstant = true;

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
         deleteVectorPtr(result);
         return nullptr;
      }
   }

   Generator<std::vector<T>>* v = new Listed<T>(result);
   if (isConstant) {
      Generator<std::vector<T>>* cnst = new Constant<std::vector<T>>(v->getValue());
      delete v;
      return cnst;
   }
   else {
      return v;
   }
}


template <typename T>
static Generator<std::vector<T>>* parseListedLists(const std::vector<Tokens>& elements, Uroboros* uro)
{
   const _size len = elements.size();
   std::vector<Generator<std::vector<T>>*>* result
      = new std::vector<Generator<std::vector<T>>*>();
   _boo isConstant = true;

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
         deleteVectorPtr(result);
         return nullptr;
      }
   }

   Generator<std::vector<T>>* v = new ListedLists<T>(result);
   if (isConstant) {
      Generator<std::vector<T>>* cnst = new Constant<std::vector<T>>(v->getValue());
      delete v;
      return cnst;
   }
   else {
      return v;
   }
}


template <typename T>
static Generator<std::vector<T>>* parseListed(const Tokens& tks, Uroboros* uro)
{
   if (!tks.containsSymbol(PG_CHAR_COMMA)) {
      return nullptr;
   }

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<std::vector<T>>* units = parseListedValues<T>(elements, uro);
   if (units != nullptr) {
      return units;
   }

   Generator<std::vector<T>>* lists = parseListedLists<T>(elements, uro);
   return lists;
}


template <typename T>
static Generator<T>* parseCollectionElement(const Tokens& tks, Uroboros* uro)
{
   if (!isPossibleListElement(tks)) {
      return nullptr;
   }

   Generator<_num>* num = parseListElementIndex(tks, uro);
   const Token& f = tks.first();
   Generator<std::vector<T>>* collection;
   if (uro->vars.getVarValue(f, collection)) {
      return new ListElement<T>(collection, num);
   }
   else {
      delete num;
      return nullptr;
   }
}


template <typename T>
void setOrderFilter(Attribute* attr, const _boo& hasMemory, OrderBy<T>*& order,
   Generator<std::vector<T>>*& result, Uroboros* uro)
{
   order = new OrderBy<T>(result, attr, uro);
   result = order;
};


void setOrderFilter(Attribute* attr, const _boo& hasMemory, OrderBy<_str>*& order, _def*& result, Uroboros* uro);


static _boo parseFilterBase(const Tokens& tks, Uroboros* uro, _def*& result, _fdata*& data)
{
   if (parseOneToken(uro, tks, result)) {
      data = result->getDataPtr();
      return true;
   }

   return false;
};


template <typename T>
static _boo parseFilterBase(const Tokens& tks, Uroboros* uro, Generator<T>*& result, _fdata*& data)
{
   data = nullptr;
   return parseOneToken(uro, tks, result);
};


template <typename T>
static void buildFilterPrototypes(std::vector<FilterPrototype<T>*>& prototypes, Attribute*& attr,
   const _boo& hasAttr, const _boo& isFinal, const _boo& hasMemory, Uroboros* uro, T& base)
{
   const _size fplen = prototypes.size();
   if (fplen != 0) {
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

      deleteVector(prototypes);
      prototypes.clear();

      if (isFinal) {
         if (hasAttr && lastWhereId == -1) {
            delete attr;
         }
      }
      else {
         if (hasAttr && lastWhereId != -1) {
            attr = new Attribute(uro);
         }
      }
   }
}


template <typename T, typename T2>
void addOrderByFilter(T& result, const ThisState& state, const Token& orderKeyword,
   Tokens& ts2, Uroboros* uro)
{
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = state;
   const _boo hasMemory = uro->vc.anyAttribute();
   Attribute* attr = state == ThisState::ts_String
      ? new Attribute(uro)
      : nullptr;

   if (state == ThisState::ts_String) {
      uro->vc.addAttribute(attr);
   }

   const Token& first = ts2.first();

   if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
      const Keyword& kw = first.value.keyword.k;
      if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
         const _boo desc = kw == Keyword::kw_Desc;
         OrderBy<T2>* order;
         setOrderFilter(attr, hasMemory, order, result, uro);

         switch (state) {
            case ThisState::ts_String: {
               Generator<_str>* vr;
               uro->vars.inner.createThisRef(vr);
               order->addUnit(vr, OrderUnitType::out_String, desc);
               break;
            }
            case ThisState::ts_Number: {
               Generator<_num>* vr;
               uro->vars.inner.createThisRef(vr);
               order->addUnit(vr, OrderUnitType::out_Number, desc);
               break;
            }
            case ThisState::ts_Time: {
               Generator<_tim>* vr;
               uro->vars.inner.createThisRef(vr);
               order->addUnit(vr, OrderUnitType::out_Time, desc);
               break;
            }
         }

         if (state == ThisState::ts_String) {
            uro->vc.retreatAttribute();
         }

         return;
      }
   }

   if (!first.isKeyword(Keyword::kw_By)) {
      delete result;
      throw SyntaxException(str(L"keyword '", *orderKeyword.value.keyword.os,
         L"' should to be followed by a keyword 'by'"), first.line);
   }

   ts2.trimLeft();
   if (ts2.isEmpty()) {
      delete result;
      throw SyntaxException(str(L"declaration of '", *orderKeyword.value.keyword.os,
         L" ", *first.value.keyword.os, L"' filter is empty"), first.line);
   }

   Tokens ts3 = prepareForGen(ts2, uro);
   if (ts3.isEmpty()) {
      delete result;
      throw SyntaxException(str(L"declaration of '", *orderKeyword.value.keyword.os,
         L" ", *first.value.keyword.os, L"' filter is empty"), first.line);
   }

   const _int commas = ts3.countSymbols(L',');
   std::vector<Tokens> units;
   ts3.splitBySymbol(L',', units);

   OrderBy<T2>* order;
   setOrderFilter(attr, hasMemory, order, result, uro);

   const _size len = units.size();
   for (_size i = 0; i < len; i++) {
      Tokens& un = units[i];
      const Token& last = un.last();
      _boo desc = false;

      if (last.type == Token::t_Keyword) {
         switch (last.value.keyword.k) {
            case Keyword::kw_Asc: {
               un.trimRight();
               if (un.isEmpty()) {
                  delete order;
                  throw SyntaxException(str(L"keyword '", *last.value.keyword.os,
                     L"' is not preceded by a value declaration"), last.line);
               }
               break;
            }
            case Keyword::kw_Desc: {
               un.trimRight();
               desc = true;
               if (un.isEmpty()) {
                  delete order;
                  throw SyntaxException(str(L"keyword '", *last.value.keyword.os,
                     L"' is not preceded by a value declaration"), last.line);
               }
               break;
            }
         }
      }

      Generator<_boo>* uboo;
      if (parse(uro, un, uboo)) {
         order->addUnit(uboo, OrderUnitType::out_Bool, desc);
         continue;
      }

      Generator<_num>* unum;
      if (parse(uro, un, unum)) {
         order->addUnit(unum, OrderUnitType::out_Number, desc);
         continue;
      }

      Generator<_per>* uper;
      if (parse(uro, un, uper)) {
         order->addUnit(uper, OrderUnitType::out_Period, desc);
         continue;
      }

      Generator<_tim>* utim;
      if (parse(uro, un, utim)) {
         order->addUnit(utim, OrderUnitType::out_Time, desc);
         continue;
      }

      Generator<_str>* ustr;
      if (parse(uro, un, ustr)) {
         order->addUnit(ustr, OrderUnitType::out_String, desc);
      }
      else {
         delete order;
         throw SyntaxException(str(L"value of '", *orderKeyword.value.keyword.os, L" by' unit "
            L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas"),
            un.first().line);
      }
   }

   if (state == ThisState::ts_String) {
      uro->vc.retreatAttribute();
   }

   uro->vars.inner.thisState = prevThisState;
}


template <typename T, typename T2>
static T parseFilter(const Tokens& tks, const ThisState& state, Uroboros* uro)
{
   if (tks.getLength() < 3 || !tks.second().isFiltherKeyword()) {
      return nullptr;
   }

   const _int start = tks.getStart() + 2;
   const _int length = tks.getLength() - 2;

   _fdata* fdata;
   T base;

   if (!parseFilterBase(tks, uro, base, fdata)) {
      return nullptr;
   }

   // core
   const _boo hasMemory = uro->vc.anyAttribute();
   const Tokens tks2(tks.list, start, length);
   std::vector<Tokens> filterTokens;
   tks2.splitByFiltherKeywords(filterTokens);
   const _size flength = filterTokens.size();
   std::vector<FilterPrototype<T>*> prototypes;

   // attribute
   const _boo hasAttr = (state == ThisState::ts_String);
   Attribute* attr;
   if (hasAttr) {
      if (fdata == nullptr) {
         attr = new Attribute(uro);
      }
      else {
         attr = new BridgeAttribute(ATTR_NULL, uro, fdata);
      }
   }
   else {
      attr = nullptr;
   }

   for (_size i = 0; i < flength; i++) {
      const Tokens& ts = filterTokens[i];
      const Keyword& kw = ts.first().value.keyword.k;
      Tokens ts2(ts.list, ts.getStart() + 1, ts.getLength() - 1);

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts2);
            }

            Generator<_num>* num;
            if (!parse(uro, ts2, num)) {
               deleteVector(prototypes);
               throw SyntaxException(str(L"tokens after keyword '", *ts.first().value.keyword.os,
                  L"' cannot be resolved to a number"), tks.first().line);
            }

            prototypes.push_back(new FP_Numeric<T>(num, kw));
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = uro->vars.inner.thisState;
            uro->vars.inner.thisState = state;

            if (hasAttr) {
               uro->vc.addAttribute(attr);
            }

            Generator<_boo>* boo;
            if (!parse(uro, ts2, boo)) {
               deleteVector(prototypes);
               uro->vars.inner.thisState = prevThisState;
               throw SyntaxException(str(L"tokens after keyword '", *ts.first().value.keyword.os,
                  L"' cannot be resolved to a logic condition"), tks.first().line);
            }

            prototypes.push_back(new FP_Where<T>(boo));
            uro->vars.inner.thisState = prevThisState;

            if (hasAttr) {
               uro->vc.retreatAttribute();
            }

            break;
         }
         case Keyword::kw_Order: {
            buildFilterPrototypes(prototypes, attr, hasAttr, false, hasMemory, uro, base);
            addOrderByFilter<T, T2>(base, state, ts.first(), ts2, uro);
            break;
         }
      }
   }

   buildFilterPrototypes(prototypes, attr, hasAttr, true, hasMemory, uro, base);
   return base;
}


#endif /* PARSE_GENERIC_H */
