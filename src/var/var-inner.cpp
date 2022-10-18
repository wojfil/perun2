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

#include "var-inner.h"
#include "../datatype/gen-ref.h"
#include "../datatype/generator/gen-generic.h"


namespace uro
{

InnerVariables::InnerVariables(const _str& loc, const _str& ucom)
   : thisState(ThisState::ts_None), urocom(ucom),
     depth(_numi(0LL)), location(loc) { }

_list InnerVariables::getAlphabet()
{
   _list a(26);

   for (_size i = 0; i < 26; i++) {
      a[i] = L'a' + i;
   }

   return a;
}

_list InnerVariables::getAscii()
{
   return {
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L"!", L"\"", L"#", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/",
      L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?",
      L"@", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O",
      L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"[", L"\\", L"]", L"^", L"_",
      L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o",
      L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~", L" "
   };
}

void InnerVariables::createThisRef(Generator<_str>*& result)
{
   result = new gen::GeneratorRef<_str>(&this_s);
}

void InnerVariables::createThisRef(Generator<_num>*& result)
{
   result = new gen::GeneratorRef<_num>(&this_n);
}

void InnerVariables::createThisRef(Generator<_tim>*& result)
{
   result = new gen::GeneratorRef<_tim>(&this_t);
}

void InnerVariables::createThisVarRef(vars::Variable<_str>*& result)
{
   result = &this_s;
}

void InnerVariables::createThisVarRef(vars::Variable<_num>*& result)
{
   result = &this_n;
}

void InnerVariables::createThisVarRef(vars::Variable<_tim>*& result)
{
   result = &this_t;
}

}
