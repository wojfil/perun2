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

#ifndef RAW_H_INCLUDED
#define RAW_H_INCLUDED

#include "../primitives.h"

namespace perun2
{


p_constexpr uint32_t RAW_DOUBLECHAR_NONE               = 0;
p_constexpr uint32_t RAW_DOUBLECHAR_LOWER_OE           = 1 << 0;
p_constexpr uint32_t RAW_DOUBLECHAR_LOWER_AE           = 1 << 1;
p_constexpr uint32_t RAW_DOUBLECHAR_LOWER_AE_ACCENT    = 1 << 2;
p_constexpr uint32_t RAW_DOUBLECHAR_LOWER_AE_MACRON    = 1 << 3;
p_constexpr uint32_t RAW_DOUBLECHAR_UPPER_OE           = 1 << 4;
p_constexpr uint32_t RAW_DOUBLECHAR_UPPER_AE           = 1 << 5;
p_constexpr uint32_t RAW_DOUBLECHAR_UPPER_AE_ACCENT    = 1 << 6;
p_constexpr uint32_t RAW_DOUBLECHAR_UPPER_AE_MACRON    = 1 << 7;


// remove all diacritical marks from Latin letters of this text
// case size stays the same
// in addition, turn every Æ to AE and every Œ to OE
void str_toRaw(p_str& value);

// replace certain char with a pair of chars
// string is expanded every replacement
inline static void replaceCharWithTwoChars(p_str& value, 
   const p_char old, const p_char new1, const p_char new2);

}

#endif // RAW_H_INCLUDED
