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


namespace uro::parse
{

_def* parseDefinition(const Tokens& tks, uro::Uroboros* uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      _def* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return parseFilter<_def*, _str>(tks, ThisState::ts_String, uro);
   }

   if (isDefinitionChain(tks, uro)) {
      _def* chain = parseDefinitionChain(tks, uro);
      if (chain != nullptr) {
         return chain;
      }
   }

   _def* bin = parseDefBinary(tks, uro);
   if (bin != nullptr) {
      return bin;
   }

   _def* tern = parseDefTernary(tks, uro);
   if (tern != nullptr) {
      return tern;
   }

   return nullptr;
}


static _boo isDefinitionChain(const Tokens& tks, uro::Uroboros* uro)
{
   if (!tks.check(TI_HAS_CHAR_COMMA)) {
      return false;
   }

   const std::vector<Tokens> elements = tks.splitBySymbol(L',');
   const _size len = elements.size();

   for (_size i = 0; i < len; i++) {
      const Tokens& tk = elements[i];
      _def* def;
      if (parse(uro, tk, def)) {
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
static _def* parseDefinitionChain(const Tokens& tks, uro::Uroboros* uro)
{
   enum ChainLink {
      cl_Definition = 0,
      cl_String,
      cl_List
   };

   const std::vector<Tokens> elements = tks.splitBySymbol(L',');
   const _size len = elements.size();

   ChainLink cl;
   _def* prevDef;
   Generator<_str>* prevStr;
   Generator<_list>* prevList;

   if (parse(uro, elements[0], prevDef)) {
      cl = ChainLink::cl_Definition;
   }
   else {
      if (parse(uro, elements[0], prevStr)) {
         cl = ChainLink::cl_String;
      }
      else {
         if (parse(uro, elements[0], prevList)) {
            cl = ChainLink::cl_List;
         }
         else {
            return nullptr;
         }
      }
   }

   for (_size i = 1; i < len; i++) {
      const Tokens& tk = elements[i];

      switch (cl) {
         case cl_Definition: {
            _def* def;
            if (parse(uro, tk, def)) {
               _def* pdef = prevDef;
               prevDef = new gen::Join_DefDef(pdef, def, uro);
            }
            else {
               Generator<_str>* str;
               if (parse(uro, tk, str)) {
                  _def* pdef = prevDef;
                  prevDef = new gen::Join_DefStr(pdef, str, uro);
               }
               else {
                  Generator<_list>* list;
                  if (parse(uro, tk, list)) {
                     _def* pdef = prevDef;
                     prevDef = new gen::Join_DefList(pdef, list, uro);
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
            if (parse(uro, tk, def)) {
               prevDef = new gen::Join_StrDef(prevStr, def, uro);
               prevStr = nullptr;
               cl = ChainLink::cl_Definition;
            }
            else {
               Generator<_str>* str;
               if (parse(uro, tk, str)) {
                  prevList = new gen::Join_StrStr(prevStr, str);
                  prevStr = nullptr;
                  cl = ChainLink::cl_List;
               }
               else {
                  Generator<_list>* list;
                  if (parse(uro, tk, list)) {
                     prevList = new gen::Join_StrList(prevStr, list);
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
            if (parse(uro, tk, def)) {
               prevDef = new gen::Join_ListDef(prevList, def, uro);
               prevList = nullptr;
               cl = ChainLink::cl_Definition;
            }
            else {
               Generator<_str>* str;
               if (parse(uro, tk, str)) {
                  Generator<_list>* plist = prevList;
                  prevList = new gen::Join_ListStr(plist, str);
               }
               else {
                  Generator<_list>* list;
                  if (parse(uro, tk, list)) {
                     Generator<_list>* plist = prevList;
                     prevList = new gen::Join_ListList(plist, list);
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
         return new gen::DefinitionChain(prevDef, uro);
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


_def* parseDefTernary(const Tokens& tks, uro::Uroboros* uro)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return nullptr;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   Generator<_boo>* condition;
   if (!parse(uro, std::get<0>(trio), condition)) {
      return nullptr;
   }

   _def* left;
   if (!parse(uro, std::get<1>(trio), left)) {
      delete condition;
      return nullptr;
   }

   _def* right;
   if (!parse(uro, std::get<2>(trio), right)) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new gen::DefTernary(condition, left, right);
}


static _def* parseDefBinary(const Tokens& tks, uro::Uroboros* uro)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return nullptr;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(L'?');

   Generator<_boo>* condition;
   if (!parse(uro, pair.first, condition)) {
      return nullptr;
   }

   _def* value;
   if (!parse(uro, pair.second, value)) {
      delete condition;
      return nullptr;
   }

   return new gen::DefBinary(condition, value);
}

}
