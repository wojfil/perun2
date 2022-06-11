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
#include "../generator/gen-generic.h"
#include "../generator/gen-timlist.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../../util.h"
#include "../parse-gen.h"


Generator<_tlist>* parseTimList(const Tokens& tks)
{
   const _size len = tks.getLength();
   const Token& first = tks.first();

   if (len == 1) {
      if (first.type == Token::t_Word) {
         return getVarValueTlist(first);
      }
   }

   if (len >= 3) {
      if (tks.second().isFiltherKeyword()) {
         if (first.type != Token::t_Word) {
            throw SyntaxException(L"filter keyword '" + tks.second().originString +
               L"' should be preceded by a variable name" , first.line);
         }

         Generator<_tlist>* filter = parseTimListFilter(tks);
         if (filter != nullptr) {
            return filter;
         }
      }

      if (tks.containsSymbol(L',')) {
         Generator<_tlist>* listed = parseTimListed(tks);
         if (listed != nullptr) {
            return listed;
         }
      }


      if (isPossibleBinary(tks)) {
         Generator<_tlist>* bin = parseTimListBinary(tks);
         if (bin != nullptr) {
            return bin;
         }
      }

      if (isPossibleTernary(tks)) {
         Generator<_tlist>* tern = parseTimListTernary(tks);
         if (tern != nullptr) {
            return tern;
         }
      }
   }

   return nullptr;
}

static Generator<_tlist>* parseTimListBinary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   tks.divideBySymbol(L'?', tks1, tks2);
   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_tlist>* value = timListGenerator(tks2);
   if (value == nullptr) {
      delete condition;
      return nullptr;
   }

   return new Binary<_tlist>(condition, value);
}

static Generator<_tlist>* parseTimListTernary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   Tokens tks3(tks);
   tks.divideForTernary(tks1, tks2, tks3);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_tlist>* left = timListGenerator(tks2);
   if (left == nullptr) {
      delete condition;
      return nullptr;
   }

   Generator<_tlist>* right = timListGenerator(tks3);
   if (right == nullptr) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new Ternary<_tlist>(condition, left, right);
}

static Generator<_tlist>* parseTimListed(const Tokens& tks)
{
   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<_tlist>* cnst = parseTimListConst(elements);
   if (cnst != nullptr) {
      return cnst;
   }

   Generator<_tlist>* times = parseListedTimes(elements);
   if (times != nullptr) {
      return times;
   }

   Generator<_tlist>* lists = parseListedTimLists(elements);
   if (lists != nullptr) {
      return lists;
   }

   return nullptr;
}

Generator<_tlist>* parseTimListConst(const std::vector<Tokens>& elements)
{
   const _size length = elements.size();
   std::vector<Generator<_tim>*> gens;

   for (_size i = 0; i < length; i++) {
      Generator<_tim>* time = parseTimeConst(elements[i]);
      if (time == nullptr) {
         if (i != 0) {
            deleteVector(gens);
         }
         return nullptr;
      }
      else {
         gens.push_back(time);
      }
   }

   _tlist times(length);
   for (_size i = 0; i < length; i++) {
      times[i] = (gens[i])->getValue();
   }

   return new Constant<_tlist>(times);
}

static Generator<_tlist>* parseListedTimes(const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   _boo isPrev = false;
   std::vector<Generator<_tim>*>* result =
      new std::vector<Generator<_tim>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<_tim>* time = timeGenerator(tks);

      if (time == nullptr) {
         if (isPrev) {
            Generator<_tim>*& last = result->back();
            result->pop_back();
            delete last;
            Generator<_tim>* time2 = timeFromTwoSeqs(elements[i - 1], tks);

            if (time2 == nullptr) {
               deleteVectorPtr(result);
               return nullptr;
            }
            else {
               result->push_back(time2);
               isPrev = false;
            }
         }
         else {
            deleteVectorPtr(result);
            return nullptr;
         }
      }
      else {
         isPrev = true;
         result->push_back(time);
      }
   }

   return new Listed<_tim>(result);
}

static Generator<_tim>* timeFromTwoSeqs(const Tokens& prev, const Tokens& curr)
{
   const _int start = prev.getStart();
   const _int length = prev.getLength() + curr.getLength() + 1;
   const Tokens tks2(curr.list, start, length);
   return timeGenerator(tks2);
}

static Generator<_tlist>* parseListedTimLists
   (const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   _boo isPrev = false;
   std::vector<Generator<_tlist>*>* result =
      new std::vector<Generator<_tlist>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<_tim>* time = timeGenerator(tks);

      if (time == nullptr) {
         if (isPrev) {
            Generator<_tim>* time2 = timeFromTwoSeqs(elements[i - 1], tks);

            if (time2 != nullptr) {
               Generator<_tlist>*& last = result->back();
               result->pop_back();
               delete last;
               result->push_back(new Cast_T_TL(time2));
               isPrev = false;
               continue;
            }
         }

         Generator<_tlist>* tlist = timListGenerator(tks);
         if (tlist == nullptr) {
            deleteVectorPtr(result);
            return nullptr;
         }
         else {
            result->push_back(tlist);
            isPrev = false;
         }
      }
      else {
         result->push_back(new Cast_T_TL(time));
         isPrev = true;
      }
   }

   return new ListedLists<_tim>(result);
}

static Generator<_tlist>* parseTimListFilter(const Tokens& tks)
{
   const _int start = tks.getStart() + 2;
   const _int length = tks.getLength() - 2;
   const Tokens tks2(tks.list, start, length);

   std::vector<Tokens> filters;
   tks2.splitByFiltherKeywords(filters);

   const Token& f = tks.first();
   Generator<_tlist>* tlist = getVarValueTlist(f);
   if (tlist == nullptr) {
      return nullptr;
   }

   const _size flength = filters.size();
   for (_size i = 0; i < flength; i++) {
      const Tokens& ts = filters[i];
      const Keyword& kw = ts.first().value.k;
      Tokens ts2(ts.list, ts.getStart() + 1, ts.getLength() - 1);
      Generator<_tlist>* prev = tlist;

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            Generator<_num>* num = numberGenerator(ts2);
            if (num == nullptr) {
               delete tlist;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString +
                  L"' cannot be resolved to a number", tks.first().line);
            }

            switch (kw) {
               case Keyword::kw_Every: {
                  tlist = new Filter_Every<_tim>(prev, num);
                  break;
               }
               case Keyword::kw_Limit: {
                  tlist = new Filter_Limit<_tim>(prev, num);
                  break;
               }
               case Keyword::kw_Skip: {
                  tlist = new Filter_Skip<_tim>(prev, num);
                  break;
               }
            }
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_Time;

            Generator<_boo>* boo = boolGenerator(ts2);
            if (boo == nullptr) {
               delete tlist;
               g_thisstate = prevThisState;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString
                  + L"' cannot be resolved to a logic condition", tks.first().line);
            }

            tlist = new Filter_WhereTime(prev, boo);
            g_thisstate = prevThisState;
            break;
         }
         case Keyword::kw_Order: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_Time;
            const Token& first = ts2.first();

            if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
               const Keyword& kw = first.value.k;
               if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
                  const _boo desc = kw == Keyword::kw_Desc;
                  Filter_OrderByTime* order = new Filter_OrderByTime(tlist);
                  order->addTime(new GeneratorRef<_tim>(&g_this_t), desc);
                  tlist = order;
                  break;
               }
            }

            if (!first.isKeyword(Keyword::kw_By)) {
               delete tlist;
               throw SyntaxException(L"keyword '" + ts.first().originString + L"' has to be followed by a keyword 'by'", first.line);
            }

            ts2.trimLeft();
            if (ts2.isEmpty()) {
               delete tlist;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            Tokens ts3 = prepareForGen(ts2);
            if (ts3.isEmpty()) {
               delete tlist;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            std::vector<Tokens> units;
            ts3.splitBySymbol(L',', units);
            Filter_OrderByTime* order = new Filter_OrderByTime(tlist);

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
                  throw SyntaxException(L"value of the '" + ts.first().originString + L" by' unit cannot be resolved to any valid data type", un.first().line);
               }
               else {
                  order->addString(ustr, desc);
               }
            }

            tlist = order;
            g_thisstate = prevThisState;
            break;
         }
         default: {
            return nullptr;
         }
      }
   }

   return tlist;
}

