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

#include "parse-numlist.h"
#include "../generator/gen-numlist.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../function.h"
#include "../parse-gen.h"


Generator<_nlist>* parseNumList(const Tokens& tks)
{
   const _size len = tks.getLength();
   const Token& first = tks.first();

   if (len == 1) {
      if (first.type == Token::t_Word) {
         return getVarValueNlist(first);
      }
   }

   if (len >= 3) {
      if (tks.second().isFiltherKeyword()) {
         if (first.type != Token::t_Word) {
            throw SyntaxException(L"filter keyword '" + tks.second().originString +
               L"' has to be preceded by a variable name" , first.line);
         }

         Generator<_nlist>* filter = parseNumListFilter(tks);
         if (filter != nullptr) {
            return filter;
         }
      }

      if (tks.containsSymbol(L',')) {
         Generator<_nlist>* nlisted = parseNumListed(tks);
         if (nlisted != nullptr) {
            return nlisted;
         }
      }

      if (isPossibleBinary(tks)) {
         Generator<_nlist>* bin = parseNlistBinary(tks);
         if (bin != nullptr) {
            return bin;
         }
      }

      if (isPossibleTernary(tks)) {
         Generator<_nlist>* tern = parseNlistTernary(tks);
         if (tern != nullptr) {
            return tern;
         }
      }
   }

   if (isPossibleFunction(tks)) {
      Generator<_nlist>* func = numListFunction(tks);
      if (func != nullptr) {
         return func;
      }
   }

   return nullptr;
}

static Generator<_nlist>* parseNlistBinary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   tks.divideBySymbol(L'?', tks1, tks2);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_nlist>* value = numListGenerator(tks2);
   if (value == nullptr) {
      delete condition;
      return nullptr;
   }

   return new Binary<_nlist>(condition, value);
}

static Generator<_nlist>* parseNlistTernary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   Tokens tks3(tks);
   tks.divideForTernary(tks1, tks2, tks3);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_nlist>* left = numListGenerator(tks2);
   if (left == nullptr) {
      delete condition;
      return nullptr;
   }

   Generator<_nlist>* right = numListGenerator(tks3);
   if (right == nullptr) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new Ternary<_nlist>(condition, left, right);
}

static Generator<_nlist>* parseNumListed(const Tokens& tks)
{
   Generator<_nlist>* cnst = parseNumListConst(tks);
   if (cnst != nullptr) {
      return cnst;
   }

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<_nlist>* nums = parseListedNums(elements);
   if (nums != nullptr) {
      return nums;
   }

   Generator<_nlist>* nlists = parseListedNumLists(elements);
   if (nlists != nullptr) {
      return nlists;
   }

   return nullptr;
}

Generator<_nlist>* parseNumListConst(const Tokens& tks)
{
   _nlist nlist;
   const _int start = tks.getStart();
   const _int end = tks.getEnd();
   const _boo even = (start % 2 == 0);

   // throw special errors if nlist starts or ends with a comma
   if (tks.first().isSymbol(L',')) {
      throw SyntaxException(L"list cannot start with a comma", tks.first().line);
   }
   else if (tks.last().isSymbol(L',')) {
      return nullptr;
   }

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (even ^ (i % 2 == 0)) {
         if (!t.isSymbol(L',')) {
            return nullptr;
         }
      }
      else {
         switch(t.type) {
            case Token::t_Number: {
               nlist.push_back(t.value.n);
               break;
            }
            default: {
               return nullptr;
            }
         }
      }
   }

   return new Constant<_nlist>(nlist);
}

static Generator<_nlist>* parseListedNums(const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   std::vector<Generator<_num>*>* result =
      new std::vector<Generator<_num>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];

      Generator<_num>* num = numberGenerator(tks);
      if (num == nullptr) {
         deleteVectorPtr(result);
         return nullptr;
      }
      else {
         result->push_back(num);
      }
   }

   return new Listed<_num>(result);
}

static Generator<_nlist>* parseListedNumLists
   (const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   std::vector<Generator<_nlist>*>* result =
      new std::vector<Generator<_nlist>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];

      Generator<_nlist>* nlist = numListGenerator(tks);
      if (nlist == nullptr) {
         deleteVectorPtr(result);
         return nullptr;
      }
      else {
         result->push_back(nlist);
      }
   }

   return new ListedLists<_num>(result);
}

static Generator<_nlist>* parseNumListFilter(const Tokens& tks)
{
   const _int start = tks.getStart() + 2;
   const _int length = tks.getLength() - 2;
   const Tokens tks2(tks.list, start, length);

   std::vector<Tokens> filters;
   tks2.splitByFiltherKeywords(filters);

   const Token& f = tks.first();
   Generator<_nlist>* nlist = getVarValueNlist(f);
   if (nlist == nullptr) {
      return nullptr;
   }

   const _size flength = filters.size();
   for (_size i = 0; i < flength; i++) {
      const Tokens& ts = filters[i];
      const Keyword& kw = ts.first().value.k;
      Tokens ts2(ts.list, ts.getStart() + 1, ts.getLength() - 1);
      Generator<_nlist>* prev = nlist;

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            Generator<_num>* num = numberGenerator(ts2);
            if (num == nullptr) {
               delete nlist;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString +
                  L"' cannot be resolved to a number", tks.first().line);
            }

            switch (kw) {
               case Keyword::kw_Every: {
                  nlist = new Filter_Every<_num>(prev, num);
                  break;
               }
               case Keyword::kw_Limit: {
                  nlist = new Filter_Limit<_num>(prev, num);
                  break;
               }
               case Keyword::kw_Skip: {
                  nlist = new Filter_Skip<_num>(prev, num);
                  break;
               }
            }
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_Number;

            Generator<_boo>* boo = boolGenerator(ts2);
            if (boo == nullptr) {
               delete nlist;
               g_thisstate = prevThisState;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString + L"' cannot "
                  L"be resolved to a logic condition", tks.first().line);
            }

            nlist = new Filter_WhereNumber(prev, boo);
            g_thisstate = prevThisState;
            break;
         }
         case Keyword::kw_Order: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_Number;
            const Token& first = ts2.first();

            if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
               const Keyword& kw = first.value.k;
               if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
                  const _boo desc = kw == Keyword::kw_Desc;
                  Filter_OrderByNumber* order = new Filter_OrderByNumber(nlist);
                  order->addNumber(new GeneratorRef<_num>(&g_this_n), desc);
                  nlist = order;
                  break;
               }
            }

            if (!first.isKeyword(Keyword::kw_By)) {
               delete nlist;
               throw SyntaxException(L"keyword '" + ts.first().originString + L"' has to be followed by a keyword 'by'", first.line);
            }

            ts2.trimLeft();
            if (ts2.isEmpty()) {
               delete nlist;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            Tokens ts3 = prepareForGen(ts2);
            if (ts3.isEmpty()) {
               delete nlist;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            std::vector<Tokens> units;
            ts3.splitBySymbol(',', units);
            Filter_OrderByNumber* order = new Filter_OrderByNumber(nlist);

            const _size len = units.size();
            for (_size i = 0; i < len; i++) {
               Tokens& un = units[i];
               const Token& last = un.last();
               _boo desc = false;

               if (last.type == Token::t_Keyword) {
                  switch (last.value.k) {
                     case Keyword::kw_Asc: {
                        un.trimRight();
                        if (un.isEmpty()) {
                           delete order;
                           throw SyntaxException(L"keyword '" + last.originString + L"' is not preceded by a value declaration", last.line);
                        }
                        break;
                     }
                     case Keyword::kw_Desc: {
                        un.trimRight();
                        desc = true;
                        if (un.isEmpty()) {
                           delete order;
                           throw SyntaxException(L"keyword '" + last.originString + L"' is not preceded by a value declaration", last.line);
                        }
                        break;
                     }
                  }
               }

               Generator<_boo>* uboo = boolGenerator(un);
               if (uboo != nullptr) {
                  order->addBool(uboo, desc);
                  continue;
               }

               Generator<_num>* unum = numberGenerator(un);
               if (unum != nullptr) {
                  order->addNumber(unum, desc);
                  continue;
               }

               Generator<_per>* uper = periodGenerator(un);
               if (uper != nullptr) {
                  order->addPeriod(uper, desc);
                  continue;
               }

               Generator<_tim>* utim = timeGenerator(un);
               if (utim != nullptr) {
                  order->addTime(utim, desc);
                  continue;
               }

               Generator<_str>* ustr = stringGenerator(un);
               if (ustr == nullptr) {
                  delete order;
                  throw SyntaxException(L"value of '" + ts.first().originString + L" by' unit cannot be resolved to any valid data type", un.first().line);
               }
               else {
                  order->addString(ustr, desc);
               }
            }

            nlist = order;
            g_thisstate = prevThisState;
            break;
         }
         default:
            return nullptr;
      }
   }

   return nlist;
}
