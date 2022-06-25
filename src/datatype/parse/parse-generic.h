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
#include "../generator/gen-list.h"
#include "../generator/gen-timlist.h"
#include "../generator/gen-numlist.h"
#include <vector>

_boo isPossibleListElement(const Tokens& tks);
Generator<_num>* parseListElementIndex(const Tokens& tks);
_boo isPossibleBinary(const Tokens& tks);
_boo isPossibleTernary(const Tokens& tks);
void checkLimitBySize(const Tokens& tks);
_boo isPossibleListElementMember(const Tokens& tks);


template <typename T>
Generator<T>* parseTernary(const Tokens& tks)
{
   if (!isPossibleTernary(tks)) {
      return nullptr;
   }

   Tokens tks1(tks);
   Tokens tks2(tks);
   Tokens tks3(tks);
   tks.divideForTernary(tks1, tks2, tks3);

   Generator<_boo>* condition;
   if (!parse(tks1, condition)) {
      return nullptr;
   }

   Generator<T>* left;
   if (!parse(tks2, left)) {
      delete condition;
      return nullptr;
   }

   Generator<T>* right;
   if (!parse(tks3, right)) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new Ternary<T>(condition, left, right);
}


template <typename T>
static Generator<T>* parseBinary(const Tokens& tks)
{
   if (!isPossibleBinary(tks)) {
      return nullptr;
   }

   Tokens tks1(tks);
   Tokens tks2(tks);
   tks.divideBySymbol(L'?', tks1, tks2);

   Generator<_boo>* condition;
   if (!parse(tks1, condition)) {
      return nullptr;
   }

   Generator<T>* value;
   if (!parse(tks2, value)) {
      delete condition;
      return nullptr;
   }

   return new Binary<T>(condition, value);
}


template <typename T>
static Generator<std::vector<T>>* parseListedValues(const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   std::vector<Generator<T>*>* result = new std::vector<Generator<T>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<T>* value;
      if (parse(tks, value)) {
         result->push_back(value);
      }
      else {
         deleteVectorPtr(result);
         return nullptr;
      }
   }

   return new Listed<T>(result);
}


template <typename T>
static Generator<std::vector<T>>* parseListedLists(const std::vector<Tokens>& elements)
{
   const _size len = elements.size();
   std::vector<Generator<std::vector<T>>*>* result = new std::vector<Generator<std::vector<T>>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<std::vector<T>>* value;
      if (parse(tks, value)) {
         result->push_back(value);
      }
      else {
         deleteVectorPtr(result);
         return nullptr;
      }
   }

   return new ListedLists<T>(result);
}


template <typename T>
static Generator<T>* parseCollectionElement(const Tokens& tks)
{
   if (!isPossibleListElement(tks)) {
      return nullptr;
   }

   Generator<_num>* num = parseListElementIndex(tks);
   const Token& f = tks.first();
   Generator<std::vector<T>>* collection;
   if (getVarValue(f, collection)) {
      return new ListElement<T>(collection, num);
   }
   else {
      delete num;
      return nullptr;
   }
}

void setNumericFilter(const Keyword& kw, Generator<_num>*& num, Generator<_nlist>*& result);
void setNumericFilter(const Keyword& kw, Generator<_num>*& num, Generator<_tlist>*& result);
void setNumericFilter(const Keyword& kw, Generator<_num>*& num, Generator<_list>*& result);
void setNumericFilter(const Keyword& kw, Generator<_num>*& num, _def*& result);

void setWhereFilter(Generator<_boo>*& boo, Attribute*& attr, const _boo& hasMemory, Generator<_nlist>*& result);
void setWhereFilter(Generator<_boo>*& boo, Attribute*& attr, const _boo& hasMemory, Generator<_tlist>*& result);
void setWhereFilter(Generator<_boo>*& boo, Attribute*& attr, const _boo& hasMemory,  Generator<_list>*& result);
void setWhereFilter(Generator<_boo>*& boo, Attribute*& attr, const _boo& hasMemory, _def*& result);

void setOrderFilter(Attribute*& attr, const _boo& hasMemory, OrderBy<_num>*& order, Generator<_nlist>*& result);
void setOrderFilter(Attribute*& attr, const _boo& hasMemory, OrderBy<_tim>*& order, Generator<_tlist>*& result);
void setOrderFilter(Attribute*& attr, const _boo& hasMemory, OrderBy<_str>*& order, Generator<_list>*& result);
void setOrderFilter(Attribute*& attr, const _boo& hasMemory, OrderBy<_str>*& order, _def*& result);


template <typename T, typename T2>
static T parseFilter(const Tokens& tks, const ThisState& state)
{
   const _int totalLen = tks.getLength();
   if (totalLen < 3 || !tks.second().isFiltherKeyword()) {
      return nullptr;
   }

   const Token& first = tks.first();
   if (first.type != Token::t_Word) {
      throw SyntaxException(str(L"filter keyword '", tks.second().originString,
         L"' should be preceded by a variable name") , first.line);
   }

   const _int start = tks.getStart() + 2;
   const _int length = tks.getLength() - 2;
   const Tokens tks2(tks.list, start, length);

   std::vector<Tokens> filters;
   tks2.splitByFiltherKeywords(filters);

   const Token& f = tks.first();
   T result;
   if (!getVarValue(f, result)) {
      return nullptr;
   }

   const _size flength = filters.size();
   for (_size i = 0; i < flength; i++) {
      const Tokens& ts = filters[i];
      const Keyword& kw = ts.first().value.k;
      Tokens ts2(ts.list, ts.getStart() + 1, ts.getLength() - 1);

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts2);
            }

            Generator<_num>* num;
            if (!parse(ts2, num)) {
               delete result;
               throw SyntaxException(str(L"tokens after keyword '", ts.first().originString,
                  L"' cannot be resolved to a number"), tks.first().line);
            }

            setNumericFilter(kw, num, result);
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = state;
            const _boo hasMemory = anyAttribute();
            Attribute* attr = new Attribute();

            if (state == ThisState::ts_String) {
               addAttribute(attr);
            }

            Generator<_boo>* boo;
            if (!parse(ts2, boo)) {
               delete result;
               g_thisstate = prevThisState;
               throw SyntaxException(str(L"tokens after keyword '", ts.first().originString,
                  L"' cannot be resolved to a logic condition"), tks.first().line);
            }

            setWhereFilter(boo, attr, hasMemory, result);
            g_thisstate = prevThisState;

            if (state == ThisState::ts_String) {
               retreatAttribute();
            }
            else {
               delete attr;
            }
            break;
         }
         case Keyword::kw_Order: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = state;
            const _boo hasMemory = anyAttribute();
            Attribute* attr = new Attribute();

            if (state == ThisState::ts_String) {
               addAttribute(attr);
            }

            const Token& first = ts2.first();

            if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
               const Keyword& kw = first.value.k;
               if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
                  const _boo desc = kw == Keyword::kw_Desc;
                  OrderBy<T2>* order;
                  setOrderFilter(attr, hasMemory, order, result);

                  switch (state) {
                     case ThisState::ts_String: {
                        order->addString(new GeneratorRef<_str>(&g_this_s), desc);
                        break;
                     }
                     case ThisState::ts_Number: {
                        order->addNumber(new GeneratorRef<_num>(&g_this_n), desc);
                        break;
                     }
                     case ThisState::ts_Time: {
                        order->addTime(new GeneratorRef<_tim>(&g_this_t), desc);
                        break;
                     }
                  }

                  if (state == ThisState::ts_String) {
                     retreatAttribute();
                  }
                  else {
                     delete attr;
                  }
                  break;
               }
            }


            if (!first.isKeyword(Keyword::kw_By)) {
               delete result;
               throw SyntaxException(str(L"keyword '", ts.first().originString,
                  L"' should to be followed by a keyword 'by'"), first.line);
            }

            ts2.trimLeft();
            if (ts2.isEmpty()) {
               delete result;
               throw SyntaxException(str(L"declaration of '", ts.first().originString,
                  L" ", first.originString, L"' filter is empty"), first.line);
            }

            Tokens ts3 = prepareForGen(ts2);
            if (ts3.isEmpty()) {
               delete result;
               throw SyntaxException(str(L"declaration of '", ts.first().originString,
                  L" ", first.originString, L"' filter is empty"), first.line);
            }

            std::vector<Tokens> units;
            ts3.splitBySymbol(L',', units);

            OrderBy<T2>* order;
            setOrderFilter(attr, hasMemory, order, result);

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
                           throw SyntaxException(str(L"keyword '", last.originString,
                              L"' is not preceded by a value declaration"), last.line);
                        }
                        break;
                     }
                     case Keyword::kw_Desc: {
                        un.trimRight();
                        desc = true;
                        if (un.isEmpty()) {
                           delete order;
                           throw SyntaxException(str(L"keyword '", last.originString,
                              L"' is not preceded by a value declaration"), last.line);
                        }
                        break;
                     }
                  }
               }

               Generator<_boo>* uboo;
               if (parse(un, uboo)) {
                  order->addBool(uboo, desc);
                  continue;
               }

               Generator<_num>* unum;
               if (parse(un, unum)) {
                  order->addNumber(unum, desc);
                  continue;
               }

               Generator<_per>* uper;
               if (parse(un, uper)) {
                  order->addPeriod(uper, desc);
                  continue;
               }

               Generator<_tim>* utim;
               if (parse(un, utim)) {
                  order->addTime(utim, desc);
                  continue;
               }

               Generator<_str>* ustr;
               if (parse(un, ustr)) {
                  order->addString(ustr, desc);
               }
               else {
                  delete order;
                  throw SyntaxException(str(L"value of '", ts.first().originString, L" by' unit "
                    L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas"), 
                    un.first().line);
               }
            }

            g_thisstate = prevThisState;
            if (state == ThisState::ts_String) {
               retreatAttribute();
            }
            else {
               delete attr;
            }

            break;
         }
         default:
            return nullptr;
      }
   }

   return result;
}


#endif /* PARSE_GENERIC_H */
