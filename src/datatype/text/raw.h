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

// this function turns string into its "raw" format
// it does 3 things:
// 1) removes all diacritical marks from Latin letters of this text
// 2) removes all combining marks from all characters
// 3) turns every ß to ss, every æ to ae and every œ to oe (uppercase variants included)
// case size stays the same
void str_toRaw(p_str& value);

// replace certain char with a pair of chars
// string is expanded every replacement
inline static void replaceCharWithTwoChars(p_str& value, 
   const p_char old, const p_char new1, const p_char new2);

inline static p_bool isCombiningMark(const p_char ch);

}
