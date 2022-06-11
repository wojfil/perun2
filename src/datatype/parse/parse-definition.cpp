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

#include "parse-definition.h"
#include "../../lexer.h"
#include "../order.h"
#include "../generator/gen-definition.h"
#include "../cast.h"
#include "../function.h"
#include "parse-generic.h"
#include "../parse-gen.h"


_def* parseDefinition(const Tokens& tks)
{
   const _size len = tks.getLength();
   const Token& f = tks.first();

   if (len == 1) {
      if (f.type == Token::t_Word) {
         return getVarValueDef(f);
      }
      return nullptr;
   }

   if (len >= 3 && tks.second().isFiltherKeyword()) {
      if (f.type != Token::t_Word) {
         throw SyntaxException(L"filter keyword '" + tks.second().originString +
            L"' has to be preceded by a variable name" , f.line);
      }

      _def* filter = parseDefinitionFilter(tks);
      if (filter != nullptr) {
         return filter;
      }
   }
   if (isDefinitionChain(tks)) {
      _def* chain = parseDefinitionChain(tks);
      if (chain != nullptr) {
         return chain;
      }
   }

   return nullptr;
}


static _def* parseDefinitionFilter(const Tokens& tks)
{
   const _int start = tks.getStart() + 2;
   const _int length = tks.getLength() - 2;
   const Tokens tks2(tks.list, start, length);

   std::vector<Tokens> filters;
   tks2.splitByFiltherKeywords(filters);

   const Token& f = tks.first();
   _def* def = getVarValueDef(f);
   if (def == nullptr) {
      return nullptr;
   }

   const _size flength = filters.size();
   for (_size i = 0; i < flength; i++) {
      const Tokens& ts = filters[i];
      const Keyword& kw = ts.first().value.k;
      Tokens ts2(ts.list, ts.getStart() + 1, ts.getLength() - 1);
      _def* prev = def;

      switch (kw) {
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts2);
            }

            Generator<_num>* num = numberGenerator(ts2);
            if (num == nullptr) {
               delete def;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString +
                  L"' cannot be resolved to a number", tks.first().line);
            }

            switch (kw) {
               case Keyword::kw_Every: {
                  def = new Filter_EveryDef(prev, num);
                  break;
               }
               case Keyword::kw_Limit: {
                  def = new Filter_LimitDef(prev, num);
                  break;
               }
               case Keyword::kw_Skip: {
                  def = new Filter_SkipDef(prev, num);
                  break;
               }
            }
            break;
         }
         case Keyword::kw_Where: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_String;
            const _boo hasMemory = anyAttribute();
            Attribute* attr = new Attribute();
            addAttribute(attr);

            Generator<_boo>* boo = boolGenerator(ts2);
            if (boo == nullptr) {
               delete def;
               g_thisstate = prevThisState;
               throw SyntaxException(L"tokens after keyword '" + ts.first().originString + L"' cannot be resolved to a logic condition", tks.first().line);
            }

            def = new Filter_WhereDef(prev, boo, attr, hasMemory);
            g_thisstate = prevThisState;
            retreatAttribute();
            break;
         }
         case Keyword::kw_Order: {
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_String;
            const _boo hasMemory = anyAttribute();
            Attribute* attr = new Attribute();
            addAttribute(attr);
            const Token& first = ts2.first();

            if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
               const Keyword& kw = first.value.k;
               if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
                  const _boo desc = kw == Keyword::kw_Desc;
                  Filter_OrderByString* order =
                     new Filter_OrderByString(new Cast_D_L(def), attr);
                  order->addString(new GeneratorRef<_str>(&g_this_s), desc);
                  def = new OrderByCast(order, attr, hasMemory);
                  retreatAttribute();
                  break;
               }
            }

            if (!first.isKeyword(Keyword::kw_By)) {
               delete def;
               throw SyntaxException(L"keyword '" + ts.first().originString + L"' should to be followed by a keyword 'by'", first.line);
            }

            ts2.trimLeft();
            if (ts2.isEmpty()) {
               delete def;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            Tokens ts3 = prepareForGen(ts2);
            if (ts3.isEmpty()) {
               delete def;
               throw SyntaxException(L"declaration of '" + ts.first().originString + L" " + first.originString + L"' filter is empty", first.line);
            }

            std::vector<Tokens> units;
            ts3.splitBySymbol(L',', units);
            Filter_OrderByString* order = new Filter_OrderByString(new Cast_D_L(def), attr);

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
                    L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas", un.first().line);
               }
               else {
                  order->addString(ustr, desc);
               }
            }

            def = new OrderByCast(order, attr, hasMemory);
            g_thisstate = prevThisState;
            retreatAttribute();
            break;
         }
         default:
            return nullptr;
      }
   }

   return def;
}

static _boo isDefinitionChain(const Tokens& tks)
{
   if (!tks.containsSymbol(L',')) {
      return false;
   }

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);
   const _size len = elements.size();

   for (_size i = 0; i < len; i++) {
      Tokens& tk = elements[i];
      _def* def = definitionGenerator(tk);
      if (def != nullptr) {
         delete def;
         return true;
      }
   }

   return false;
}

static _def* parseDefinitionChain(const Tokens& tks)
{
   enum ChainLink {
      cl_Definition = 0,
      cl_String,
      cl_List
   };

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);
   const _size len = elements.size();

   ChainLink cl;
   _def* prevDef;
   Generator<_str>* prevStr;
   Generator<_list>* prevList;

   prevDef = definitionGenerator(elements[0]);
   if (prevDef == nullptr) {
      prevStr = stringGenerator(elements[0]);
      if (prevStr == nullptr) {
         prevList = listGenerator(elements[0]);
         if (prevList == nullptr) {
            return nullptr;
         }
         else {
            cl = ChainLink::cl_List;
         }
      }
      else {
         cl = ChainLink::cl_String;
      }
   }
   else {
      cl = ChainLink::cl_Definition;
   }

   for (_size i = 1; i < len; i++) {
      Tokens& tk = elements[i];

      switch (cl) {
         case cl_Definition: {
            _def* def = definitionGenerator(tk);
            if (def == nullptr) {
               Generator<_str>* str = stringGenerator(tk);
               if (str == nullptr) {
                  Generator<_list>* list = listGenerator(tk);
                  if (list == nullptr) {
                     delete prevDef;
                     return nullptr;
                  }
                  else {
                     _def* pdef = prevDef;
                     prevDef = new Join_DefList(pdef, list);
                  }
               }
               else {
                  _def* pdef = prevDef;
                  prevDef = new Join_DefStr(pdef, str);
               }
            }
            else {
               _def* pdef = prevDef;
               prevDef = new Join_DefDef(pdef, def);
            }
            break;
         }
         case cl_String: {
            _def* def = definitionGenerator(tk);
            if (def == nullptr) {
               Generator<_str>* str = stringGenerator(tk);
               if (str == nullptr) {
                  Generator<_list>* list = listGenerator(tk);
                  if (list == nullptr) {
                     delete prevStr;
                     return nullptr;
                  }
                  else {
                     prevList =  new Join_StrList(prevStr, list);
                     prevStr = nullptr;
                     cl = ChainLink::cl_List;
                  }
               }
               else {
                  prevList =  new Join_StrStr(prevStr, str);
                  prevStr = nullptr;
                  cl = ChainLink::cl_List;
               }
            }
            else {
               prevDef = new Join_StrDef(prevStr, def);
               prevStr = nullptr;
               cl = ChainLink::cl_Definition;
            }
            break;
         }
         case cl_List: {
            _def* def = definitionGenerator(tk);
            if (def == nullptr) {
               Generator<_str>* str = stringGenerator(tk);
               if (str == nullptr) {
                  Generator<_list>* list = listGenerator(tk);
                  if (list == nullptr) {
                     delete prevList;
                     return nullptr;
                  }
                  else {
                     Generator<_list>* plist = prevList;
                     prevList = new Join_ListList(plist, list);
                  }
               }
               else {
                  Generator<_list>* plist = prevList;
                  prevList = new Join_ListStr(plist, str);
               }
            }
            else {
               prevDef = new Join_ListDef(prevList, def);
               prevList = nullptr;
               cl = ChainLink::cl_Definition;
            }
            break;
         }
      }
   }

   switch (cl) {
      case cl_Definition: {
         return prevDef;
      }
      case cl_String: {
         delete prevStr;
         break;
      }
      case cl_List: {
         delete prevList;
         break;
      }
   }

   return nullptr;
}




