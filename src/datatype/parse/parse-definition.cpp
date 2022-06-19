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
#include "parse-generic.h"
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
         _def* var;
         return getVarValue(f, var) ? var : nullptr;
      }
      return nullptr;
   }

   _def* filter = parseFilter<_def*, _str>(tks, ThisState::ts_String);
   if (filter != nullptr) {
      return filter;
   }

   if (isDefinitionChain(tks)) {
      _def* chain = parseDefinitionChain(tks);
      if (chain != nullptr) {
         return chain;
      }
   }

   return nullptr;
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
      _def* def;
      if (parse(tk, def)) {
         delete def;
         return true;
      }
   }

   return false;
}

// definition chain is a collection of strings, lists and definitions
// separated by commas, that contain at least one definition

// for the sake of optimization
// definition chains are lazy evaluated
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

   if (parse(elements[0], prevDef)) {
      cl = ChainLink::cl_Definition;
   }
   else {
      if (parse(elements[0], prevStr)) {
         cl = ChainLink::cl_String;
      }
      else {
         if (parse(elements[0], prevList)) {
            cl = ChainLink::cl_List;
         }
         else {
            return nullptr;
         }
      }
   }

   for (_size i = 1; i < len; i++) {
      Tokens& tk = elements[i];

      switch (cl) {
         case cl_Definition: {
            _def* def;
            if (parse(tk, def)) {
               _def* pdef = prevDef;
               prevDef = new Join_DefDef(pdef, def);
            }
            else {
               Generator<_str>* str;
               if (parse(tk, str)) {
                  _def* pdef = prevDef;
                  prevDef = new Join_DefStr(pdef, str);
               }
               else {
                  Generator<_list>* list;
                  if (parse(tk, list)) {
                     _def* pdef = prevDef;
                     prevDef = new Join_DefList(pdef, list);
                  }
                  else {
                     delete prevDef;
                     return nullptr;
                  }
               }
            }

            break;
         }
         case cl_String: {
            _def* def;
            if (parse(tk, def)) {
               prevDef = new Join_StrDef(prevStr, def);
               prevStr = nullptr;
               cl = ChainLink::cl_Definition;
            }
            else {
               Generator<_str>* str;
               if (parse(tk, str)) {
                  prevList =  new Join_StrStr(prevStr, str);
                  prevStr = nullptr;
                  cl = ChainLink::cl_List;
               }
               else {
                  Generator<_list>* list;
                  if (parse(tk, list)) {
                     prevList =  new Join_StrList(prevStr, list);
                     prevStr = nullptr;
                     cl = ChainLink::cl_List;
                  }
                  else {
                     delete prevStr;
                     return nullptr;
                  }
               }
            }

            break;
         }
         case cl_List: {
            _def* def;
            if (parse(tk, def)) {
               prevDef = new Join_ListDef(prevList, def);
               prevList = nullptr;
               cl = ChainLink::cl_Definition;
            }
            else {
               Generator<_str>* str;
               if (parse(tk, str)) {
                  Generator<_list>* plist = prevList;
                  prevList = new Join_ListStr(plist, str);
               }
               else {
                  Generator<_list>* list;
                  if (parse(tk, list)) {
                     Generator<_list>* plist = prevList;
                     prevList = new Join_ListList(plist, list);
                  }
                  else {
                     delete prevList;
                     return nullptr;
                  }
               }
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
