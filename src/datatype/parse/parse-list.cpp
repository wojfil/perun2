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


namespace uro::parse
{

_bool parseList(_genptr<_list>& result, const Tokens& tks, Uroboros& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      if (parseFilter<_genptr<_list>, _str>(result, tks, ThisState::ts_String, uro)) {
         return true;
      }
      else {
         throw SyntaxException(L"this syntax structure cannot be resolved to any collection of values", tks.first().line);
      }
   }

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseListed<_str>(result, tks, uro);
      }

      if (parseBinary<_list>(result, tks, uro) || parseTernary<_list>(result, tks, uro)) {
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::listFunction(result, tks, uro);
   }

   return false;
}

}
