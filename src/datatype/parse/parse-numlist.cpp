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
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../function.h"
#include "../parse-gen.h"


namespace uro::parse
{

Generator<_nlist>* parseNumList(const Tokens& tks, uro::Uroboros* uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      Generator<_nlist>* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }

   if (tks.containsFilterKeyword()) {
      return parseFilter<Generator<_nlist>*, _num>(tks, ThisState::ts_Number, uro);
   }

   if (len >= 3) {
      Generator<_nlist>* nlisted = parseListed<_num>(tks, uro);
      if (nlisted != nullptr) {
         return nlisted;
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

   if (tks.isPossibleFunction()) {
      Generator<_nlist>* func = func::numListFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }

   return nullptr;
}

}
