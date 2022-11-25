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

#include "parse-order.h"


namespace uro::parse
{

void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, Uroboros& uro)
{
   if (state == ThisState::ts_String) {
      uro.vc.retreatAttribute();
   }

   uro.vars.inner.thisState = prevState;
}

void orderUnitFailure(const Token& tk, Uroboros& uro)
{
   throw SyntaxException(str(L"keyword '", tk.getOriginString(uro),
      L"' is not preceded by a value used for order"), tk.line);
}

void prepareOrderUnit(Tokens& tks, _bool& desc, gen::Order* order, gen::OrderIndices* indices, Uroboros& uro)
{
   desc = false;
   const Token& last = tks.last();

   if (last.type == Token::t_Keyword) {
      const Keyword& kw = last.value.keyword.k;
      if (kw == Keyword::kw_Asc) {
         tks.trimRight();
         if (tks.isEmpty()) {
            delete indices;
            if (order != nullptr) {
               delete order;
            }
            orderUnitFailure(last, uro);
         }
      }
      else if (kw == Keyword::kw_Desc) {
         desc = true;
         tks.trimRight();
         if (tks.isEmpty()) {
            delete indices;
            if (order != nullptr) {
               delete order;
            }
            orderUnitFailure(last, uro);
         }
      }
   }
}

void setSingleOrderFilter(_attrptr& attr, const _bool& hasMemory, _defptr& result,
   gen::OrderIndices* indices, gen::Order* order, Uroboros& uro)
{
   _defptr prev = std::move(result);
   result = std::make_unique<gen::OrderBy_Definition>(prev, attr, hasMemory, indices, order, uro);
}

}
