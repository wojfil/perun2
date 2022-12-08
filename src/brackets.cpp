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

#include "brackets.h"
#include "exception.h"


namespace uro
{

void BracketsInfo::refresh(const Token& tk)
{
   if (tk.type == Token::t_Symbol) {
      switch (tk.value.ch) {
         case CHAR_OPENING_ROUND_BRACKET:  {
            round++;
            break;
         }
         case CHAR_CLOSING_ROUND_BRACKET:  {
            round--;
            break;
         }
         case CHAR_OPENING_SQUARE_BRACKET:  {
            square++;
            break;
         }
         case CHAR_CLOSING_SQUARE_BRACKET:  {
            square--;
            break;
         }
      }
   }
}

_bool BracketsInfo::isBracketFree() const
{
   return round == 0 && square == 0;
}

void checkBracketsThoroughly(const Tokens& tks)
{
   // 1 = ()
   // 2 = []
   // 3 = {}
   _int lv1 = 0, lv2 = 0, lv3 = 0, i1 = 0, i2 = 0, i3 = 0;
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type != Token::t_Symbol) { continue; }

      switch(t.value.ch)  {
         case CHAR_OPENING_ROUND_BRACKET: {
            lv1++;
            i1 = t.line;
            break;
         }
         case CHAR_CLOSING_ROUND_BRACKET: {
            lv1--;
            if (lv1 < 0) {
               throw SyntaxException(L"unopened bracket ( is closed", t.line);
            }
            break;
         }
         case CHAR_OPENING_SQUARE_BRACKET: {
            lv2++;
            i2 = t.line;
            break;
         }
         case CHAR_CLOSING_SQUARE_BRACKET: {
            lv2--;
            if (lv2 < 0) {
               throw SyntaxException(L"unopened bracket [ is closed", t.line);
            }
            break;
         }
         case CHAR_OPENING_CURLY_BRACKET: {
            lv3++;
            i3 = t.line;
            break;
         }
         case CHAR_CLOSING_CURLY_BRACKET: {
            lv3--;
            if (lv3 < 0) {
               throw SyntaxException(L"unopened bracket { is closed", t.line);
            }
            if (lv1 != 0) {
               throw SyntaxException(
               L"bracket ( has to be closed before opening curly bracket {", i1);
            }
            if (lv2 != 0) {
               throw SyntaxException(
               L"bracket [ has to be closed before opening curly bracket {", i2);
            }
            break;
         }
      }
   }

   if (lv1 != 0) {
      throw SyntaxException(L"bracket ( is not closed", i1);
   }
   else if (lv2 != 0) {
      throw SyntaxException(L"bracket [ is not closed", i2);
   }
   else if (lv3 != 0) {
      throw SyntaxException(L"bracket { is not closed", i3);
   }
}

}
