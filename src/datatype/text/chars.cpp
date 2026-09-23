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

#include <cwctype>
#include <clocale>
#include <locale>
#include <charconv>
#include <unicode/locid.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/uchar.h>
#include "chars.h"
#include "../../unicode/convert.h"


namespace perun2
{

void char_toLower(p_char& ch)
{
   p_str string;
   string += ch;

   const p_str lowerString = toLowercase(string);
   ch = lowerString[0];
}


void char_toUpper(p_char& ch)
{
   p_str string;
   string += ch;

   const p_str upperString = toUppercase(string);
   ch = upperString[0];
}


p_bool char_isAlpha(const p_char ch)
{
   return u_isalpha(ch);
}


p_bool char_isSpace(const p_char ch)
{
   return u_isUWhiteSpace(ch);
}


p_bool char_isDigit(const p_char ch)
{
   return ch >= U'0' && ch <= U'9';
}


p_bool char_isUpper(const p_char ch)
{
   return std::iswupper(ch);
}


p_bool char_isLower(const p_char ch)
{
   return std::iswlower(ch);
}


p_bool charsEqualInsensitive(p_char ch1, p_char ch2)
{
   char_toLower(ch1);
   char_toLower(ch2);
   return ch1 == ch2;
}

}
