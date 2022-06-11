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

#include "parse-list.h"
#include "../generator/gen-list.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../function.h"
#include "parse-generic.h"
#include "../parse-gen.h"


Generator<_list>* parseList (const Tokens& tks)
{
   const _size len = tks.getLength();
   const Token& first = tks.first();

   if (len == 1) {
      if (first.type == Token::t_Word) {
         return getVarValueList(first);
      }
   }

   if (len >= 3) {
      if (tks.second().isFiltherKeyword()) {
         if (first.type != Token::t_Word) {
            throw SyntaxException(L"filter keyword '" + tks.second().originString +
               L"' has to be preceded by a variable name" , first.line);
         }

         Generator<_list>* filter = parseListFilter(tks);
         if (filter != nullptr) {
            return filter;
         }
      }

      if (tks.containsSymbol(L',')) {
         Generator<_list>* listed = parseListed(tks);
         if (listed != nullptr) {
            return listed;
         }
      }

      if (isPossibleBinary(tks)) {
         Generator<_list>* bin = parseListBinary(tks);
         if (bin != nullptr) {
            return bin;
         }
      }

      if (isPossibleTernary(tks)) {
         Generator<_list>* tern = parseListTernary(tks);
         if (tern != nullptr) {
            return tern;
         }
      }
   }

   if (isPossibleFunction(tks)) {
      Generator<_list>* func = listFunction(tks);
      if (func != nullptr) {
         return func;
      }
   }

   return nullptr;
}


static Generator<_list>* parseListBinary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   tks.divideBySymbol(L'?', tks1, tks2);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_list>* value = listGenerator(tks2);
   if (value == nullptr) {
      delete condition;
      return nullptr;
   }

   return new Binary<_list>(condition, value);
}

static Generator<_list>* parseListTernary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   Tokens tks3(tks);
   tks.divideForTernary(tks1, tks2, tks3);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_list>* left = listGenerator(tks2);
   if (left == nullptr) {
      delete condition;
      return nullptr;
   }

   Generator<_list>* right = listGenerator(tks3);
   if (right == nullptr) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new Ternary<_list>(condition, left, right);
}

static Generator<_list>* parseListed(const Tokens& tks)
{
   Generator<_list>* cnst = parseListConst(tks);
   if (cnst != nullptr) {
      return cnst;
   }

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<_list>* strs = parseListedStrings(elements);
   if (strs != nullptr) {
      return strs;
   }

   Generator<_list>* lists = parseListedLists(elements);
   if (lists != nullptr) {
      return lists;
   }

   return nullptr;
}

Generator<_list>* parseListConst(const Tokens& tks)
{
   _list list;
   const _int start = tks.getStart();
   const _int end = tks.getEnd();
   const _boo even = (start % 2 == 0);

   // throw special errors if list starts or ends with a comma
   if (tks.first().isSymbol(L',')) {
      throw SyntaxException(L"list cannot start with a comma",
         tks.first().line);
   }
   if (tks.last().isSymbol(L',')) {
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
            case Token::t_Quotation: {
               list.push_back(t.value.sl);
               break;
            }
            case Token::t_Number: {
               list.push_back(t.value.n.toString());
               break;
            }
            default: {
               return nullptr;
            }
         }
      }
   }

   return new Constant<_list>(list);
}

static Generator<_list>* parseListedStrings(const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   std::vector<Generator<_str>*>* result =
      new std::vector<Generator<_str>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];

      Generator<_str>* str = stringGenerator(tks);
      if (str == nullptr) {
         deleteVectorPtr(result);
         return nullptr;
      }
      else {
         result->push_back(str);
      }
   }

   return new Listed<_str>(result);
}

static Generator<_list>* parseListedLists(const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   std::vector<Generator<_list>*>* result =
      new std::vector<Generator<_list>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];

      Generator<_list>* list = listGenerator(tks);
      if (list == nullptr) {
         deleteVectorPtr(result);
         return nullptr;
      }
      else {
         result->push_back(list);
      }
   }

   return new ListedLists<_str>(result);
}

static Generator<_list>* parseListFilter(const Tokens& tks)
{
   const _int start = tks.getStart() + 2;
   const _int length = tks.getLength() - 2;
   const Tokens tks2(tks.list, start, length);

   std::vector<Tokens> filters;
   tks2.splitByFiltherKeywords(filters);

   const Token& f = tks.first();
   Generator<_list>* list = getVarValueList(f);
   if (list == nullptr) {
      return nullptr;
   }

   const _size flength = filters.size();
   for (_size i = 0; i < flength; i++) {
      const Tokens& ts = filters[i];
      const Keyword& kw = ts.first().value.k;
      Tokens ts2(ts.list, ts.getStart() + 1, ts.getLength() - 1);
      Generator<_list>* prev = list;

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts2);
            }

            Generator<_num>* num = numberGenerator(ts2);
            if (num == nullptr) {
               delete list;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString +
                  L"' cannot be resolved to a number", tks.first().line);
            }

            switch (kw) {
               case Keyword::kw_Every: {
                  list = new Filter_Every<_str>(prev, num);
                  break;
               }
               case Keyword::kw_Limit: {
                  list = new Filter_Limit<_str>(prev, num);
                  break;
               }
               case Keyword::kw_Skip: {
                  list = new Filter_Skip<_str>(prev, num);
                  break;
               }
            }
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_String;
            Attribute attr;
            addAttribute(&attr);

            Generator<_boo>* boo = boolGenerator(ts2);
            if (boo == nullptr) {
               delete list;
               g_thisstate = prevThisState;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString + L"' cannot "
                  L"be resolved to a logic condition", tks.first().line);
            }

            list = new Filter_WhereString(prev, boo, attr);
            g_thisstate = prevThisState;
            retreatAttribute();
            break;
         }
         case Keyword::kw_Order: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_String;
            Attribute* attr = new Attribute();
            addAttribute(attr);
            const Token& first = ts2.first();

            if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
               const Keyword& kw = first.value.k;
               if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
                  const _boo desc = kw == Keyword::kw_Desc;
                  Filter_OrderByString* order =
                     new Filter_OrderByString(list, attr);
                  order->addString(new GeneratorRef<_str>(&g_this_s), desc);
                  list = order;
                  retreatAttribute();
                  break;
               }
            }

            if (!first.isKeyword(Keyword::kw_By)) {
               delete list;
               throw SyntaxException(L"keyword '" + ts.first().originString + L"' has to be followed by a keyword 'by'", first.line);
            }

            ts2.trimLeft();
            if (ts2.isEmpty()) {
               delete list;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            Tokens ts3 = prepareForGen(ts2);
            if (ts3.isEmpty()) {
               delete list;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            std::vector<Tokens> units;
            ts3.splitBySymbol(L',', units);
            Filter_OrderByString* order = new Filter_OrderByString(list, attr);

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
                  throw SyntaxException(L"value of '" + ts.first().originString + L" by' unit "
                    L"cannot be resolved to any valid data type", un.first().line);
               }
               else {
                  order->addString(ustr, desc);
               }
            }

            list = order;
            g_thisstate = prevThisState;
            retreatAttribute();
            break;
         }
         default:
            return nullptr;
      }
   }

   return list;
}

