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

#include "parse-generic.h"
#include "../../brackets.h"
#include "../../lexer.h"
#include "../../hash.h"
#include "../generator/gen-definition.h"
#include "../generator/gen-time.h"
#include "../generator/gen-list.h"
#include <cwctype>


namespace uro::parse
{

Generator<_num>* parseListElementIndex(const Tokens& tks, Uroboros& uro)
{
   const _size start = tks.getStart() + 2;
   const _size length = tks.getLength() - 3;
   const Tokens tks2(tks, start, length);

   Generator<_num>* num = parseNumber(tks2, uro);
   if (num == nullptr) {
      throw SyntaxException(
         L"content of square brackets [] cannot be resolved to a number",
         tks.first().line);
   }

   return num;
}

void checkLimitBySize(const Tokens& tks, Uroboros& uro)
{
   if (tks.getLength() == 1) {
      const Token& tk = tks.first();
      if (tk.type == Token::t_Number && tk.value.num.nm == NumberMode::nm_Size) {
         throw SyntaxException(str(L"collection cannot be limited by file size '", tk.getOriginString(uro),
            L"' in this way. You have to iterate over files in a loop, add their size to a helper variable and provide a loop break condition"),
            tk.line);
      }
   }
}

}
