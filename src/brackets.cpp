/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "brackets.h"
#include "exception.h"


namespace perun2
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
   _int roundLvl = 0;
   _int squareLvl = 0;
   _int curlyLvl = 0;
   _int roundId = 0;
   _int squareId = 0;
   _int curlyId = 0;
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type != Token::t_Symbol) { continue; }

      switch (t.value.ch)  {
         case CHAR_OPENING_ROUND_BRACKET: {
            roundLvl++;
            roundId = t.line;
            break;
         }
         case CHAR_CLOSING_ROUND_BRACKET: {
            roundLvl--;
            if (roundLvl < 0) {
               throw SyntaxError::unopenedBracketIsClosed(CHAR_OPENING_ROUND_BRACKET, t.line);
            }
            break;
         }
         case CHAR_OPENING_SQUARE_BRACKET: {
            squareLvl++;
            squareId = t.line;
            break;
         }
         case CHAR_CLOSING_SQUARE_BRACKET: {
            squareLvl--;
            if (squareLvl < 0) {
               throw SyntaxError::unopenedBracketIsClosed(CHAR_OPENING_SQUARE_BRACKET, t.line);
            }
            break;
         }
         case CHAR_OPENING_CURLY_BRACKET: {
            curlyLvl++;
            curlyId = t.line;
            break;
         }
         case CHAR_CLOSING_CURLY_BRACKET: {
            curlyLvl--;
            if (curlyLvl < 0) {
               throw SyntaxError::unopenedBracketIsClosed(CHAR_OPENING_CURLY_BRACKET, t.line);
            }
            if (roundLvl != 0) {
               throw SyntaxError::bracketShouldBeClosedBeforeCurlyBracket(CHAR_OPENING_ROUND_BRACKET, roundId);
            }
            if (squareLvl != 0) {
               throw SyntaxError::bracketShouldBeClosedBeforeCurlyBracket(CHAR_OPENING_SQUARE_BRACKET, squareId);
            }
            break;
         }
      }
   }

   if (roundLvl != 0) {
      throw SyntaxError::bracketIsNotClosed(CHAR_OPENING_ROUND_BRACKET, roundId);
   }
   else if (squareLvl != 0) {
      throw SyntaxError::bracketIsNotClosed(CHAR_OPENING_SQUARE_BRACKET, squareId);
   }
   else if (curlyLvl != 0) {
      throw SyntaxError::bracketIsNotClosed(CHAR_OPENING_CURLY_BRACKET, curlyId);
   }
}

}
