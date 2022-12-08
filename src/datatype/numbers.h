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

#ifndef NUMBERS_H_INCLUDED
#define NUMBERS_H_INCLUDED

#include "primitives.h"


namespace uro
{

inline constexpr _nint NINT_ZERO =                 0LL;
inline constexpr _nint NINT_ONE =                  1LL;
inline constexpr _nint NINT_MINUS_ONE =           -1LL;
inline constexpr _nint NINT_300 =                300LL;
inline constexpr _nint NINT_THOUSAND =          1000LL;
inline constexpr _nint NINT_1024 =              1024LL;

inline constexpr _nint SIZE_UNIT_KB =      NINT_1024;
inline constexpr _nint SIZE_UNIT_MB =      NINT_1024 * NINT_1024;
inline constexpr _nint SIZE_UNIT_GB =      NINT_1024 * NINT_1024 * NINT_1024;
inline constexpr _nint SIZE_UNIT_TB =      NINT_1024 * NINT_1024 * NINT_1024 * NINT_1024;
inline constexpr _nint SIZE_UNIT_PB =      NINT_1024 * NINT_1024 * NINT_1024 * NINT_1024 * NINT_1024;

}

#endif // NUMBERS_H_INCLUDED
