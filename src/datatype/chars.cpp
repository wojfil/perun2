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

#include <cwctype>
#include <clocale>
#include <locale>
#include "chars.h"


namespace perun2
{

void toLower(_char& ch)
{
   ch = std::tolower(ch, std::locale(""));
}

void toUpper(_char& ch)
{
   ch = std::toupper(ch, std::locale(""));
}

_bool charsEqualInsensitive(const _char* ch1, const _char* ch2)
{
   return _wcsicmp(ch1, ch2) == 0;
}

}
