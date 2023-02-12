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

#include "parse-order.h"


namespace uro::parse
{
/*
void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, _uro& uro)
{
   if (state == ThisState::ts_String) {
      uro.vc.retreatAttribute();
   }

   uro.vars.inner.thisState = prevState;
}

void orderUnitFailure(const Token& tk, _uro& uro)
{
   throw SyntaxError(str(L"keyword '", tk.getOriginString(uro),
      L"' is not preceded by a value used for order"), tk.line);
}

void prepareOrderUnit(Tokens& tks, _bool& desc, gen::_ordptr& order, gen::_indptr& indices, _uro& uro)
{
   desc = false;
   const Token& last = tks.last();

   if (last.type == Token::t_Keyword) {
      const Keyword& kw = last.value.keyword.k;
      if (kw == Keyword::kw_Asc) {
         tks.trimRight();
         if (tks.isEmpty()) {
            orderUnitFailure(last, uro);
         }
      }
      else if (kw == Keyword::kw_Desc) {
         desc = true;
         tks.trimRight();
         if (tks.isEmpty()) {
            orderUnitFailure(last, uro);
         }
      }
   }
}

void setSingleOrderFilter(_attrptr& attr, const _bool& hasMemory, _defptr& result,
   gen::_indptr& indices, gen::_ordptr& order, _uro& uro)
{
   _defptr prev = std::move(result);
   result = std::make_unique<gen::OrderBy_Definition>(prev, attr, hasMemory, indices, order, uro);
}*/

}
