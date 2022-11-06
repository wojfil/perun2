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
#include "../parse/parse-function.h"
#include "parse-generic.h"
#include "../parse-gen.h"


namespace uro::parse
{

_bool parseDefinition(_defptr& result, const Tokens& tks, Uroboros& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return parseFilter<_defptr, _str>(result, tks, ThisState::ts_String, uro);
   }

   if (isDefinitionChain(tks, uro)) {
      if (parseDefinitionChain(result, tks, uro)) {
         return true;
      }
   }

   if (parseDefBinary(result, tks, uro) || parseDefTernary(result, tks, uro)) {
      return true;
   }

   return false;
}


static _bool isDefinitionChain(const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_HAS_CHAR_COMMA)) {
      return false;
   }

   const std::vector<Tokens> elements = tks.splitBySymbol(L',');
   const _size len = elements.size();

   for (_size i = 0; i < len; i++) {
      const Tokens& tk = elements[i];
      _defptr def;
      if (parse(uro, tk, def)) {
         return true;
      }
   }

   return false;
}

// definition chain is a collection of strings, lists and definitions
// separated by commas, that contain at least one definition
// for the sake of Uroboros optimization
// definition chains are lazy evaluated runtime
static _bool parseDefinitionChain(_defptr& result, const Tokens& tks, Uroboros& uro)
{
   enum ChainLink {
      cl_Definition = 0,
      cl_String,
      cl_List
   };

   const std::vector<Tokens> elements = tks.splitBySymbol(L',');
   const _size len = elements.size();

   ChainLink cl;
   _defptr prevDef;
   _genptr<_str> prevStr;
   _genptr<_list> prevList;

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
            return false;
         }
      }
   }

   for (_size i = 1; i < len; i++) {
      const Tokens& tk = elements[i];

      switch (cl) {
         case cl_Definition: {
            _defptr def;
            if (parse(uro, tk, def)) {
               _defptr pdef = std::move(prevDef);
               prevDef = std::make_unique<gen::Join_DefDef>(pdef, def, uro);
            }
            else {
               _genptr<_str> str;
               if (parse(uro, tk, str)) {
                  _defptr pdef = std::move(prevDef);
                  prevDef = std::make_unique<gen::Join_DefStr>(pdef, str, uro);
               }
               else {
                  _genptr<_list> list;
                  if (parse(uro, tk, list)) {
                     _defptr pdef = std::move(prevDef);
                     prevDef = std::make_unique<gen::Join_DefList>(pdef, list, uro);
                  }
                  else {
                     return false;
                  }
               }
            }

            break;
         }
         case cl_String: {
            _defptr def;
            if (parse(uro, tk, def)) {
               prevDef = std::make_unique<gen::Join_StrDef>(prevStr, def, uro);
               prevStr.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               _genptr<_str> str;
               if (parse(uro, tk, str)) {
                  prevList = std::make_unique<gen::Join_StrStr>(prevStr, str);
                  prevStr.reset();
                  cl = ChainLink::cl_List;
               }
               else {
                  _genptr<_list> list;
                  if (parse(uro, tk, list)) {
                     prevList = std::make_unique<gen::Join_StrList>(prevStr, list);
                     prevStr.reset();
                     cl = ChainLink::cl_List;
                  }
                  else {
                     return false;
                  }
               }
            }

            break;
         }
         case cl_List: {
            _defptr def;
            if (parse(uro, tk, def)) {
               prevDef = std::make_unique<gen::Join_ListDef>(prevList, def, uro);
               prevList.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               _genptr<_str> str;
               if (parse(uro, tk, str)) {
                  _genptr<_list> plist(std::move(prevList));
                  prevList = std::make_unique<gen::Join_ListStr>(plist, str);
               }
               else {
                  _genptr<_list> list;
                  if (parse(uro, tk, list)) {
                     _genptr<_list> plist(std::move(prevList));
                     prevList = std::make_unique<gen::Join_ListList>(plist, list);
                  }
                  else {
                     return false;
                  }
               }
            }
            break;
         }
      }
   }

   if (cl == cl_Definition) {
      result = std::make_unique<gen::DefinitionChain>(prevDef, uro);
      return true;
   }

   return false;
}


static _bool parseDefTernary(_defptr& result, const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return false;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   _genptr<_bool> condition;
   if (!parse(uro, std::get<0>(trio), condition)) {
      return false;
   }

   _defptr left;
   if (!parse(uro, std::get<1>(trio), left)) {
      return false;
   }

   _defptr right;
   if (!parse(uro, std::get<2>(trio), right)) {
      return false;
   }

   result = std::make_unique<gen::DefTernary>(condition, left, right);
   return true;
}


static _bool parseDefBinary(_defptr& result, const Tokens& tks, Uroboros& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return false;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(L'?');

   _genptr<_bool> condition;
   if (!parse(uro, pair.first, condition)) {
      return false;
   }

   _defptr value;
   if (!parse(uro, pair.second, value)) {
      return false;
   }

   result = std::make_unique<gen::DefBinary>(condition, value);
   return true;
}

}
