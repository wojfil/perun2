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

#ifndef CHARS_H_INCLUDED
#define CHARS_H_INCLUDED

#include "primitives.h"


namespace uro
{

void toLower(_char& ch);
void toUpper(_char& ch);

inline constexpr _char CHAR_COMMA =                  L',';
inline constexpr _char CHAR_EXCLAMATION_MARK =       L'!';
inline constexpr _char CHAR_EQUAL_SIGN =             L'=';
inline constexpr _char CHAR_OPENING_ROUND_BRACKET =  L'(';
inline constexpr _char CHAR_CLOSING_ROUND_BRACKET =  L')';
inline constexpr _char CHAR_OPENING_CURLY_BRACKET =  L'{';
inline constexpr _char CHAR_CLOSING_CURLY_BRACKET =  L'}';
inline constexpr _char CHAR_OPENING_SQUARE_BRACKET = L'[';
inline constexpr _char CHAR_CLOSING_SQUARE_BRACKET = L']';
inline constexpr _char CHAR_COLON =                  L':';
inline constexpr _char CHAR_SEMICOLON =              L';';
inline constexpr _char CHAR_MINUS =                  L'-';
inline constexpr _char CHAR_PLUS =                   L'+';
inline constexpr _char CHAR_ASTERISK =               L'*';
inline constexpr _char CHAR_PERCENT =                L'%';
inline constexpr _char CHAR_SLASH =                  L'/';
inline constexpr _char CHAR_BACKSLASH =              L'\\';
inline constexpr _char CHAR_SMALLER =                L'<';
inline constexpr _char CHAR_GREATER =                L'>';
inline constexpr _char CHAR_QUESTION_MARK =          L'?';
inline constexpr _char CHAR_NEW_LINE =               L'\n';
inline constexpr _char CHAR_DOT =                    L'.';
inline constexpr _char CHAR_UNDERSCORE =             L'_';
inline constexpr _char CHAR_CARET =                  L'^';
inline constexpr _char CHAR_AMPERSAND =              L'&';
inline constexpr _char CHAR_VERTICAL_BAR =           L'|';
inline constexpr _char CHAR_BACKTICK =               L'`';
inline constexpr _char CHAR_SPACE =                  L' ';
inline constexpr _char CHAR_QUOTATION_MARK =         L'"';
inline constexpr _char CHAR_APOSTROPHE =             L'\'';
inline constexpr _char CHAR_INTERPUNCT =             L'·';

inline constexpr _char LETTER_b =                    L'b';
inline constexpr _char LETTER_B =                    L'B';
inline constexpr _char LETTER_k =                    L'k';
inline constexpr _char LETTER_K =                    L'K';
inline constexpr _char LETTER_m =                    L'm';
inline constexpr _char LETTER_M =                    L'M';
inline constexpr _char LETTER_g =                    L'g';
inline constexpr _char LETTER_G =                    L'G';
inline constexpr _char LETTER_t =                    L't';
inline constexpr _char LETTER_T =                    L'T';
inline constexpr _char LETTER_p =                    L'p';
inline constexpr _char LETTER_P =                    L'P';
inline constexpr _char LETTER_n =                    L'n';
inline constexpr _char LETTER_N =                    L'N';
inline constexpr _char LETTER_s =                    L's';
inline constexpr _char LETTER_S =                    L'S';

inline constexpr _char DIGIT_0 =                     L'0';
inline constexpr _char DIGIT_1 =                     L'1';


inline constexpr _char CHAR_UNARY_MINUS =            L'~';
// within a numerical expression, the minus sign means either subtraction operation (x-y) or unary negation (-x)
// the sign above is used by the interpreter to distinguish them
// is used safely internally and wlll not cause collisions if the ~ character is added as a regular language operator


}

#endif // CHARS_H_INCLUDED
