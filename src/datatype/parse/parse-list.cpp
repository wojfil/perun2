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

   if (len == 1) {
      Generator<_list>* unit;
      if (parseOneToken(uro, tks, unit)) {
         return unit;
      }
   }

   Generator<_list>* filter = parseFilter<Generator<_list>*, _str>(tks, ThisState::ts_String, uro);
   if (filter != nullptr) {
      return filter;
   }

   if (len >= 3) {
      Generator<_list>* listed = parseListed<_str>(tks, uro);
      if (listed != nullptr) {
         return listed;
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

   if (tks.isPossibleFunction()) {
      Generator<_list>* func = listFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }

   return nullptr;
}
