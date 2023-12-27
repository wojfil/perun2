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

#include "parse-generic.h"
#include "../../brackets.h"
#include "../../lexer.h"
#include "../generator/gen-definition.h"
#include "../generator/gen-time.h"
#include "../generator/gen-list.h"


namespace perun2::parse
{

p_bool parseListElementIndex(p_genptr<p_num>& result, const Tokens& tks, p_perun2& p2)
{
   const p_size start = tks.getStart() + 2;
   const p_size length = tks.getLength() - 3;
   const Tokens tks2(tks, start, length);

   if (parse(p2, tks2, result))  {
      return true;
   }
   else {
      throw SyntaxError(L"content of square brackets [] cannot be resolved to a number",
         tks.first().line);
   }

}

void checkLimitBySize(const Tokens& tks, p_perun2& p2)
{
   if (tks.getLength() == 1) {
      const Token& tk = tks.first();
      if (tk.type == Token::t_Number && tk.value.num.nm == NumberMode::nm_Size) {
         throw SyntaxError(str(L"collection cannot be limited by file size '", tk.getOriginString(p2),
            L"' in this way. You have to iterate over files in a loop, add their size to a helper variable and provide a loop break condition"),
            tk.line);
      }
   }
}

}
