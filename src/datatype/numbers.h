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

#ifndef NUMBERS_H_INCLUDED
#define NUMBERS_H_INCLUDED

#include "primitives.h"
#include "time-const.h"


namespace uro
{

// here all number literals, that are of type _nint or _ndouble
// their literals should not be hardcoded in the source code

inline constexpr _nint NINT_ZERO =                 0LL;
inline constexpr _nint NINT_ONE =                  1LL;
inline constexpr _nint NINT_MINUS_ONE =           -1LL;
inline constexpr _nint NINT_TWO =                  2LL;
inline constexpr _nint NINT_THREE =                3LL;
inline constexpr _nint NINT_FOUR =                 4LL;
inline constexpr _nint NINT_TEN =                 10LL;
inline constexpr _nint NINT_300 =                300LL;
inline constexpr _nint NINT_THOUSAND =          1000LL;
inline constexpr _nint NINT_1024 =              1024LL;

inline constexpr _nint SIZE_UNIT_KB =      NINT_1024;
inline constexpr _nint SIZE_UNIT_MB =      SIZE_UNIT_KB * SIZE_UNIT_KB;
inline constexpr _nint SIZE_UNIT_GB =      SIZE_UNIT_MB * SIZE_UNIT_KB;
inline constexpr _nint SIZE_UNIT_TB =      SIZE_UNIT_GB * SIZE_UNIT_KB;
inline constexpr _nint SIZE_UNIT_PB =      SIZE_UNIT_TB * SIZE_UNIT_KB;

inline constexpr _nint NINT_SECONDS_IN_MINUTE =      static_cast<_nint>(TNUM_SECONDS_IN_MINUTE);
inline constexpr _nint NINT_SECONDS_IN_HOUR =        static_cast<_nint>(TNUM_SECONDS_IN_HOUR);
inline constexpr _nint NINT_SECONDS_IN_DAY =         static_cast<_nint>(TNUM_SECONDS_IN_DAY);
inline constexpr _nint NINT_SECONDS_IN_WEEK =        static_cast<_nint>(TNUM_SECONDS_IN_WEEK);
inline constexpr _nint NINT_SECONDS_IN_MONTH =       static_cast<_nint>(TNUM_SECONDS_IN_MONTH);
inline constexpr _nint NINT_SECONDS_IN_YEAR =        static_cast<_nint>(TNUM_SECONDS_IN_YEAR);


inline constexpr _ndouble NDOUBLE_ZERO =            0L;
inline constexpr _ndouble NDOUBLE_HALF =          0.5L;
inline constexpr _ndouble NDOUBLE_ONE =             1L;
inline constexpr _ndouble NDOUBLE_MINUS_ONE =      -1L;
inline constexpr _ndouble NDOUBLE_TWELVE =         12L;



inline constexpr _nint ROMAN_VINCULUM_THRESHOLD = 4000LL;
inline constexpr _nint ROMAN_MAXIMUM = 5000000LL;
static const _nint ROMAN_NUMBER_LITERALS[] = { 1LL, 4LL, 5LL, 9LL, 10LL, 40LL, 50LL, 90LL, 100LL, 400LL, 500LL, 900LL, 1000LL,
    4000LL, 5000LL, 9000LL, 10000LL, 40000LL, 50000LL, 90000LL, 100000LL, 400000LL, 500000LL, 900000LL, 1000000LL
};



}

#endif // NUMBERS_H_INCLUDED
