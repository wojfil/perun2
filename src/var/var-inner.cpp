/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "var-inner.h"
#include "../datatype/gen-ref.h"
#include "../datatype/generator/gen-generic.h"


namespace uro
{

inline constexpr _size LETTERS_IN_ENGLISH_ALPHABET = 26;


InnerVariables::InnerVariables(const _str& loc, const _str& ucom)
   : thisState(ThisState::ts_None), urocom(ucom),
     depth(_numi(NINT_ZERO)), location(loc) { }

_list InnerVariables::getAlphabet()
{
   _list a(LETTERS_IN_ENGLISH_ALPHABET);

   for (_size i = 0; i < LETTERS_IN_ENGLISH_ALPHABET; i++) {
      a[i] = LETTER_a + i;
   }

   return a;
}

void InnerVariables::createThisRef(_genptr<_str>& result)
{
   result = std::make_unique<gen::GeneratorRef<_str>>(this_s);
}

void InnerVariables::createThisRef(_genptr<_num>& result)
{
   result = std::make_unique<gen::GeneratorRef<_num>>(this_n);
}

void InnerVariables::createThisRef(_genptr<_tim>& result)
{
   result = std::make_unique<gen::GeneratorRef<_tim>>(this_t);
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
