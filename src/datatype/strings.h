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

#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include "chars.h"


namespace uro
{

static const _str EMPTY_STRING =              _str();
static const _str STRING_0 =                  toStr(DIGIT_0);
static const _str STRING_1 =                  toStr(DIGIT_1);
static const _str STRING_MINUS =              toStr(CHAR_MINUS);
static const _str STRING_ASTERISK =           toStr(CHAR_ASTERISK);
static const _str STRING_DOUBLE_ASTERISK =    str(STRING_ASTERISK, STRING_ASTERISK);




}


#endif // STRINGS_H_INCLUDED