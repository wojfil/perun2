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

#ifndef CHARS_H_INCLUDED
#define CHARS_H_INCLUDED

#include "primitives.h"


namespace perun2
{

void toLower(_char& ch);
void toUpper(_char& ch);
_bool charsEqualInsensitive(_char ch1, _char ch2);

_constexpr _char CHAR_COMMA =                  L',';
_constexpr _char CHAR_EXCLAMATION_MARK =       L'!';
_constexpr _char CHAR_EQUAL_SIGN =             L'=';
_constexpr _char CHAR_OPENING_ROUND_BRACKET =  L'(';
_constexpr _char CHAR_CLOSING_ROUND_BRACKET =  L')';
_constexpr _char CHAR_OPENING_CURLY_BRACKET =  L'{';
_constexpr _char CHAR_CLOSING_CURLY_BRACKET =  L'}';
_constexpr _char CHAR_OPENING_SQUARE_BRACKET = L'[';
_constexpr _char CHAR_CLOSING_SQUARE_BRACKET = L']';
_constexpr _char CHAR_COLON =                  L':';
_constexpr _char CHAR_SEMICOLON =              L';';
_constexpr _char CHAR_MINUS =                  L'-';
_constexpr _char CHAR_PLUS =                   L'+';
_constexpr _char CHAR_ASTERISK =               L'*';
_constexpr _char CHAR_PERCENT =                L'%';
_constexpr _char CHAR_SLASH =                  L'/';
_constexpr _char CHAR_BACKSLASH =              L'\\';
_constexpr _char CHAR_SMALLER =                L'<';
_constexpr _char CHAR_GREATER =                L'>';
_constexpr _char CHAR_QUESTION_MARK =          L'?';
_constexpr _char CHAR_NEW_LINE =               L'\n';
_constexpr _char CHAR_DOT =                    L'.';
_constexpr _char CHAR_UNDERSCORE =             L'_';
_constexpr _char CHAR_CARET =                  L'^';
_constexpr _char CHAR_AMPERSAND =              L'&';
_constexpr _char CHAR_VERTICAL_BAR =           L'|';
_constexpr _char CHAR_BACKTICK =               L'`';
_constexpr _char CHAR_SPACE =                  L' ';
_constexpr _char CHAR_QUOTATION_MARK =         L'"';
_constexpr _char CHAR_APOSTROPHE =             L'\'';
_constexpr _char CHAR_INTERPUNCT =             L'·';
_constexpr _char CHAR_HASH =                   L'#';
_constexpr _char CHAR_TILDE =                  L'~';

_constexpr _char CHAR_NULL =                   L'\0';
_constexpr _char CHAR_NULL_2 =                 L'\1';

_constexpr _char CHAR_b =                      L'b';
_constexpr _char CHAR_B =                      L'B';
_constexpr _char CHAR_k =                      L'k';
_constexpr _char CHAR_K =                      L'K';
_constexpr _char CHAR_m =                      L'm';
_constexpr _char CHAR_M =                      L'M';
_constexpr _char CHAR_g =                      L'g';
_constexpr _char CHAR_G =                      L'G';
_constexpr _char CHAR_t =                      L't';
_constexpr _char CHAR_T =                      L'T';
_constexpr _char CHAR_p =                      L'p';
_constexpr _char CHAR_P =                      L'P';
_constexpr _char CHAR_n =                      L'n';
_constexpr _char CHAR_N =                      L'N';
_constexpr _char CHAR_s =                      L's';
_constexpr _char CHAR_S =                      L'S';
_constexpr _char CHAR_d =                      L'd';
_constexpr _char CHAR_D =                      L'D';
_constexpr _char CHAR_h =                      L'h';
_constexpr _char CHAR_H =                      L'H';
_constexpr _char CHAR_c =                      L'c';
_constexpr _char CHAR_C =                      L'C';
_constexpr _char CHAR_a =                      L'a';
_constexpr _char CHAR_A =                      L'A';
_constexpr _char CHAR_e =                      L'e';
_constexpr _char CHAR_E =                      L'E';
_constexpr _char CHAR_f =                      L'f';
_constexpr _char CHAR_F =                      L'F';
_constexpr _char CHAR_z =                      L'z';
_constexpr _char CHAR_Z =                      L'Z';

_constexpr _char CHAR_0 =                      L'0';
_constexpr _char CHAR_1 =                      L'1';
_constexpr _char CHAR_2 =                      L'2';
_constexpr _char CHAR_3 =                      L'3';
_constexpr _char CHAR_4 =                      L'4';
_constexpr _char CHAR_5 =                      L'5';
_constexpr _char CHAR_6 =                      L'6';
_constexpr _char CHAR_7 =                      L'7';
_constexpr _char CHAR_8 =                      L'8';
_constexpr _char CHAR_9 =                      L'9';

}

#endif // CHARS_H_INCLUDED
