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
#include "parse-generic.h"
#include "../generator/gen-numlist.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../function.h"
#include "../parse-gen.h"


Generator<_nlist>* parseNumList(const Tokens& tks, Uroboros* uro)
{
   const _size len = tks.getLength();
   const Token& first = tks.first();

   if (len == 1) {
      if (first.type == Token::t_Word) {
         Generator<_nlist>* var;
         return uro->vars.getVarValue(first, var) ? var : nullptr;
      }
   }

   Generator<_nlist>* filter = parseFilter<Generator<_nlist>*, _num>(tks, ThisState::ts_Number, uro);
   if (filter != nullptr) {
      return filter;
   }

   if (len >= 3) {
      if (tks.containsSymbol(L',')) {
         Generator<_nlist>* nlisted = parseNumListed(tks, uro);
         if (nlisted != nullptr) {
            return nlisted;
         }
      }

      Generator<_nlist>* bin = parseBinary<_nlist>(tks, uro);
      if (bin != nullptr) {
         return bin;
      }

      Generator<_nlist>* tern = parseTernary<_nlist>(tks, uro);
      if (tern != nullptr) {
         return tern;
      }
   }

   if (isPossibleFunction(tks)) {
      Generator<_nlist>* func = numListFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }

   return nullptr;
}

static Generator<_nlist>* parseNumListed(const Tokens& tks, Uroboros* uro)
{
   Generator<_nlist>* cnst = parseNumListConst(tks);
   if (cnst != nullptr) {
      return cnst;
   }

   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<_nlist>* nums = parseListedValues<_num>(elements, uro);
   if (nums != nullptr) {
      return nums;
   }

   Generator<_nlist>* nlists = parseListedLists<_num>(elements, uro);
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
               nlist.push_back(t.value.num.n);
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
