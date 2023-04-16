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

#include "strings.h"

namespace perun2
{

const _list ROMAN_STRING_LITERALS = { L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M",
   (L"I" L"̅" L"V" L"̅"), (L"V" L"̅"), (L"I" L"̅" L"X" L"̅"), (L"X" L"̅"),
   (L"X" L"̅" L"L" L"̅"), (L"L" L"̅"), (L"X" L"̅" L"C" L"̅"),
   (L"C" L"̅"), (L"C" L"̅" L"D" L"̅"), (L"D" L"̅"),
   (L"C" L"̅" L"M" L"̅"), (L"M" L"̅")
};

const _list STRINGS_ASCII = {
   L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
   L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
   L" ", L"!", L"\"", L"#", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/",
   L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?",
   L"@", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O",
   L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"[", L"\\", L"]", L"^", L"_",
   L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o",
   L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~", L" "
};

_str toStr(const _char ch) 
{
   return _str(1, ch);
}

_ndouble stringToDouble(const _str& value)
{
   _stream ss(value);
   _ndouble n;
   ss >> n;
   return n;
}

void toLower(_str& value)
{
   for (_char& ch : value) {
      ch = std::tolower(ch, std::locale(""));
   }
}

void toUpper(_str& value)
{
   for (_char& ch : value) {
      ch = std::toupper(ch, std::locale(""));
   }
}

}
