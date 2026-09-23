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

#pragma once

#include "../primitives.h"


namespace perun2
{

void char_toLower(p_char& ch);
void char_toUpper(p_char& ch);
p_bool char_isAlpha(const p_char ch);
p_bool char_isSpace(const p_char ch);
p_bool char_isDigit(const p_char ch);
p_bool char_isUpper(const p_char ch);
p_bool char_isLower(const p_char ch);

p_bool charsEqualInsensitive(p_char ch1, p_char ch2);

p_constexpr p_char CHAR_COMMA =                  U',';
p_constexpr p_char CHAR_EXCLAMATION_MARK =       U'!';
p_constexpr p_char CHAR_EQUAL_SIGN =             U'=';
p_constexpr p_char CHAR_OPENING_ROUND_BRACKET =  U'(';
p_constexpr p_char CHAR_CLOSING_ROUND_BRACKET =  U')';
p_constexpr p_char CHAR_OPENING_CURLY_BRACKET =  U'{';
p_constexpr p_char CHAR_CLOSING_CURLY_BRACKET =  U'}';
p_constexpr p_char CHAR_OPENING_SQUARE_BRACKET = U'[';
p_constexpr p_char CHAR_CLOSING_SQUARE_BRACKET = U']';
p_constexpr p_char CHAR_COLON =                  U':';
p_constexpr p_char CHAR_SEMICOLON =              U';';
p_constexpr p_char CHAR_MINUS =                  U'-';
p_constexpr p_char CHAR_PLUS =                   U'+';
p_constexpr p_char CHAR_ASTERISK =               U'*';
p_constexpr p_char CHAR_PERCENT =                U'%';
p_constexpr p_char CHAR_SLASH =                  U'/';
p_constexpr p_char CHAR_BACKSLASH =              U'\\';
p_constexpr p_char CHAR_SMALLER =                U'<';
p_constexpr p_char CHAR_GREATER =                U'>';
p_constexpr p_char CHAR_QUESTION_MARK =          U'?';
p_constexpr p_char CHAR_NEW_LINE =               U'\n';
p_constexpr p_char CHAR_TAB =                    U'\t';
p_constexpr p_char CHAR_CARRIAGE_RETURN =        U'\r';
p_constexpr p_char CHAR_DOT =                    U'.';
p_constexpr p_char CHAR_UNDERSCORE =             U'_';
p_constexpr p_char CHAR_CARET =                  U'^';
p_constexpr p_char CHAR_AMPERSAND =              U'&';
p_constexpr p_char CHAR_VERTICAL_BAR =           U'|';
p_constexpr p_char CHAR_BACKTICK =               U'`';
p_constexpr p_char CHAR_SPACE =                  U' ';
p_constexpr p_char CHAR_QUOTATION_MARK =         U'"';
p_constexpr p_char CHAR_APOSTROPHE =             U'\'';
p_constexpr p_char CHAR_INTERPUNCT =             0x00B7;
p_constexpr p_char CHAR_HASH =                   U'#';
p_constexpr p_char CHAR_TILDE =                  U'~';

p_constexpr p_char CHAR_NULL =                   U'\0';
p_constexpr p_char CHAR_NULL_2 =                 U'\1';

p_constexpr p_char CHAR_b =                      U'b';
p_constexpr p_char CHAR_B =                      U'B';
p_constexpr p_char CHAR_k =                      U'k';
p_constexpr p_char CHAR_K =                      U'K';
p_constexpr p_char CHAR_m =                      U'm';
p_constexpr p_char CHAR_M =                      U'M';
p_constexpr p_char CHAR_g =                      U'g';
p_constexpr p_char CHAR_G =                      U'G';
p_constexpr p_char CHAR_t =                      U't';
p_constexpr p_char CHAR_T =                      U'T';
p_constexpr p_char CHAR_p =                      U'p';
p_constexpr p_char CHAR_P =                      U'P';
p_constexpr p_char CHAR_n =                      U'n';
p_constexpr p_char CHAR_N =                      U'N';
p_constexpr p_char CHAR_s =                      U's';
p_constexpr p_char CHAR_S =                      U'S';
p_constexpr p_char CHAR_d =                      U'd';
p_constexpr p_char CHAR_D =                      U'D';
p_constexpr p_char CHAR_h =                      U'h';
p_constexpr p_char CHAR_H =                      U'H';
p_constexpr p_char CHAR_c =                      U'c';
p_constexpr p_char CHAR_C =                      U'C';
p_constexpr p_char CHAR_a =                      U'a';
p_constexpr p_char CHAR_A =                      U'A';
p_constexpr p_char CHAR_e =                      U'e';
p_constexpr p_char CHAR_E =                      U'E';
p_constexpr p_char CHAR_f =                      U'f';
p_constexpr p_char CHAR_F =                      U'F';
p_constexpr p_char CHAR_z =                      U'z';
p_constexpr p_char CHAR_Z =                      U'Z';
p_constexpr p_char CHAR_o =                      U'o';
p_constexpr p_char CHAR_O =                      U'O';

p_constexpr p_char CHAR_0 =                      U'0';
p_constexpr p_char CHAR_1 =                      U'1';
p_constexpr p_char CHAR_2 =                      U'2';
p_constexpr p_char CHAR_3 =                      U'3';
p_constexpr p_char CHAR_4 =                      U'4';
p_constexpr p_char CHAR_5 =                      U'5';
p_constexpr p_char CHAR_6 =                      U'6';
p_constexpr p_char CHAR_7 =                      U'7';
p_constexpr p_char CHAR_8 =                      U'8';
p_constexpr p_char CHAR_9 =                      U'9';

}
