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

void toLower(p_char& ch);
void toUpper(p_char& ch);
p_bool charsEqualInsensitive(p_char ch1, p_char ch2);

p_constexpr p_char CHAR_COMMA =                  L',';
p_constexpr p_char CHAR_EXCLAMATION_MARK =       L'!';
p_constexpr p_char CHAR_EQUAL_SIGN =             L'=';
p_constexpr p_char CHAR_OPENING_ROUND_BRACKET =  L'(';
p_constexpr p_char CHAR_CLOSING_ROUND_BRACKET =  L')';
p_constexpr p_char CHAR_OPENING_CURLY_BRACKET =  L'{';
p_constexpr p_char CHAR_CLOSING_CURLY_BRACKET =  L'}';
p_constexpr p_char CHAR_OPENING_SQUARE_BRACKET = L'[';
p_constexpr p_char CHAR_CLOSING_SQUARE_BRACKET = L']';
p_constexpr p_char CHAR_COLON =                  L':';
p_constexpr p_char CHAR_SEMICOLON =              L';';
p_constexpr p_char CHAR_MINUS =                  L'-';
p_constexpr p_char CHAR_PLUS =                   L'+';
p_constexpr p_char CHAR_ASTERISK =               L'*';
p_constexpr p_char CHAR_PERCENT =                L'%';
p_constexpr p_char CHAR_SLASH =                  L'/';
p_constexpr p_char CHAR_BACKSLASH =              L'\\';
p_constexpr p_char CHAR_SMALLER =                L'<';
p_constexpr p_char CHAR_GREATER =                L'>';
p_constexpr p_char CHAR_QUESTION_MARK =          L'?';
p_constexpr p_char CHAR_NEW_LINE =               L'\n';
p_constexpr p_char CHAR_TAB =                    L'\t';
p_constexpr p_char CHAR_CARRIAGE_RETURN =        L'\r';
p_constexpr p_char CHAR_DOT =                    L'.';
p_constexpr p_char CHAR_UNDERSCORE =             L'_';
p_constexpr p_char CHAR_CARET =                  L'^';
p_constexpr p_char CHAR_AMPERSAND =              L'&';
p_constexpr p_char CHAR_VERTICAL_BAR =           L'|';
p_constexpr p_char CHAR_BACKTICK =               L'`';
p_constexpr p_char CHAR_SPACE =                  L' ';
p_constexpr p_char CHAR_QUOTATION_MARK =         L'"';
p_constexpr p_char CHAR_APOSTROPHE =             L'\'';
p_constexpr p_char CHAR_INTERPUNCT =             L'·';
p_constexpr p_char CHAR_HASH =                   L'#';
p_constexpr p_char CHAR_TILDE =                  L'~';

p_constexpr p_char CHAR_NULL =                   L'\0';
p_constexpr p_char CHAR_NULL_2 =                 L'\1';

p_constexpr p_char CHAR_b =                      L'b';
p_constexpr p_char CHAR_B =                      L'B';
p_constexpr p_char CHAR_k =                      L'k';
p_constexpr p_char CHAR_K =                      L'K';
p_constexpr p_char CHAR_m =                      L'm';
p_constexpr p_char CHAR_M =                      L'M';
p_constexpr p_char CHAR_g =                      L'g';
p_constexpr p_char CHAR_G =                      L'G';
p_constexpr p_char CHAR_t =                      L't';
p_constexpr p_char CHAR_T =                      L'T';
p_constexpr p_char CHAR_p =                      L'p';
p_constexpr p_char CHAR_P =                      L'P';
p_constexpr p_char CHAR_n =                      L'n';
p_constexpr p_char CHAR_N =                      L'N';
p_constexpr p_char CHAR_s =                      L's';
p_constexpr p_char CHAR_S =                      L'S';
p_constexpr p_char CHAR_d =                      L'd';
p_constexpr p_char CHAR_D =                      L'D';
p_constexpr p_char CHAR_h =                      L'h';
p_constexpr p_char CHAR_H =                      L'H';
p_constexpr p_char CHAR_c =                      L'c';
p_constexpr p_char CHAR_C =                      L'C';
p_constexpr p_char CHAR_a =                      L'a';
p_constexpr p_char CHAR_A =                      L'A';
p_constexpr p_char CHAR_e =                      L'e';
p_constexpr p_char CHAR_E =                      L'E';
p_constexpr p_char CHAR_f =                      L'f';
p_constexpr p_char CHAR_F =                      L'F';
p_constexpr p_char CHAR_z =                      L'z';
p_constexpr p_char CHAR_Z =                      L'Z';

p_constexpr p_char CHAR_0 =                      L'0';
p_constexpr p_char CHAR_1 =                      L'1';
p_constexpr p_char CHAR_2 =                      L'2';
p_constexpr p_char CHAR_3 =                      L'3';
p_constexpr p_char CHAR_4 =                      L'4';
p_constexpr p_char CHAR_5 =                      L'5';
p_constexpr p_char CHAR_6 =                      L'6';
p_constexpr p_char CHAR_7 =                      L'7';
p_constexpr p_char CHAR_8 =                      L'8';
p_constexpr p_char CHAR_9 =                      L'9';

}

#endif // CHARS_H_INCLUDED
