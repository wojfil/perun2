/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parse-numlist.h"
#include "parse-generic.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../parse/parse-function.h"
#include "../parse-gen.h"


namespace perun2::parse
{

p_bool parseNumList(p_genptr<p_nlist>& result, const Tokens& tks, p_perun2& p2)
{
   const p_size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseListed<p_num>(result, tks, p2);
      }

      if (parseBinary<p_nlist>(result, tks, p2) || parseTernary<p_nlist>(result, tks, p2)) {
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::numListFunction(result, tks, p2);
   }

   return false;
}

}
