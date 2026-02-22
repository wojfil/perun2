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
#include "../../../include/perun2/datatype/text/chars.hpp"


namespace perun2
{

void char_toLower(p_char& ch)
{
   ch = std::tolower(ch, std::locale(""));
}


void char_toUpper(p_char& ch)
{
   ch = std::toupper(ch, std::locale(""));
}


p_bool char_isAlpha(const p_char ch)
{
   return std::iswalpha(ch);
}


p_bool char_isSpace(const p_char ch)
{
   return std::iswspace(ch);
}


p_bool char_isDigit(const p_char ch)
{
   return std::iswdigit(ch);
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
   ch1 = std::tolower(ch1, std::locale(""));
   ch2 = std::tolower(ch2, std::locale(""));
   return ch1 == ch2;
}

}
