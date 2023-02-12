/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parse-numlist.h"
#include "parse-generic.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../parse/parse-function.h"
#include "../parse-gen.h"


namespace uro::parse
{

_bool parseNumList(_genptr<_nlist>& result, const Tokens& tks, _uro& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }

   /*if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return parseFilter<_genptr<_nlist>, _num>(result, tks, ThisState::ts_Number, uro);
   }*/

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseListed<_num>(result, tks, uro);
      }

      if (parseBinary<_nlist>(result, tks, uro) || parseTernary<_nlist>(result, tks, uro)) {
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::numListFunction(result, tks, uro);
   }

   return false;
}

}
