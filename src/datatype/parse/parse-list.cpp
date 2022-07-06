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
#include "parse-generic.h"
#include "../generator/gen-list.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../function.h"
#include "parse-generic.h"
#include "../parse-gen.h"


Generator<_list>* parseList (const Tokens& tks, Uroboros* uro)
{
   const _size len = tks.getLength();
   const Token& first = tks.first();

   if (len == 1) {
      if (first.type == Token::t_Word) {
         Generator<_list>* var;
         return uro->vars.getVarValue(first, var) ? var : nullptr;
      }
   }

   Generator<_list>* filter = parseFilter<Generator<_list>*, _str>(tks, ThisState::ts_String, uro);
   if (filter != nullptr) {
      return filter;
   }

   if (len >= 3) {
      if (tks.containsSymbol(L',')) {
         Generator<_list>* listed = parseListed(tks, uro);
         if (listed != nullptr) {
            return listed;
         }
      }

      Generator<_list>* bin = parseBinary<_list>(tks, uro);
      if (bin != nullptr) {
         return bin;
      }

      Generator<_list>* tern = parseTernary<_list>(tks, uro);
      if (tern != nullptr) {
         return tern;
      }
   }

   if (isPossibleFunction(tks)) {
      Generator<_list>* func = listFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }

   return nullptr;
}


static Generator<_list>* parseListed(const Tokens& tks, Uroboros* uro)
{
   Generator<_list>* cnst = parseListConst(tks, uro);
   if (cnst != nullptr) {
      return cnst;
   }

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<_list>* strs = parseListedValues<_str>(elements, uro);
   if (strs != nullptr) {
      return strs;
   }

   Generator<_list>* lists = parseListedLists<_str>(elements, uro);
   if (lists != nullptr) {
      return lists;
   }

   return nullptr;
}

Generator<_list>* parseListConst(const Tokens& tks, Uroboros* uro)
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
               list.push_back(*t.value.str);
               break;
            }
            case Token::t_Number: {
               list.push_back(t.value.num.n.toString());
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
