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

#ifndef NUMBERS_H_INCLUDED
#define NUMBERS_H_INCLUDED

#include "primitives.h"
#include "time-const.h"


namespace perun2
{

// here all number literals, that are of type _nint or _ndouble
// their literals should not be hardcoded in the source code

_constexpr _nint NINT_ZERO =                 0LL;
_constexpr _nint NINT_ONE =                  1LL;
_constexpr _nint NINT_MINUS_ONE =           -1LL;
_constexpr _nint NINT_TWO =                  2LL;
_constexpr _nint NINT_THREE =                3LL;
_constexpr _nint NINT_FOUR =                 4LL;
_constexpr _nint NINT_TEN =                 10LL;
_constexpr _nint NINT_300 =                300LL;
_constexpr _nint NINT_THOUSAND =          1000LL;
_constexpr _nint NINT_1024 =              1024LL;
_constexpr _nint NINT_1950 =              1950LL;
_constexpr _nint NINT_2100 =              2100LL;
_constexpr _nint NINT_MILLION =        1000000LL;

_constexpr _nint SIZE_UNIT_KB =      NINT_1024;
_constexpr _nint SIZE_UNIT_MB =      SIZE_UNIT_KB * SIZE_UNIT_KB;
_constexpr _nint SIZE_UNIT_GB =      SIZE_UNIT_MB * SIZE_UNIT_KB;
_constexpr _nint SIZE_UNIT_TB =      SIZE_UNIT_GB * SIZE_UNIT_KB;
_constexpr _nint SIZE_UNIT_PB =      SIZE_UNIT_TB * SIZE_UNIT_KB;

_constexpr _nint NINT_SECONDS_IN_MINUTE =      static_cast<_nint>(TNUM_SECONDS_IN_MINUTE);
_constexpr _nint NINT_SECONDS_IN_HOUR =        static_cast<_nint>(TNUM_SECONDS_IN_HOUR);
_constexpr _nint NINT_SECONDS_IN_DAY =         static_cast<_nint>(TNUM_SECONDS_IN_DAY);
_constexpr _nint NINT_SECONDS_IN_WEEK =        static_cast<_nint>(TNUM_SECONDS_IN_WEEK);
_constexpr _nint NINT_SECONDS_IN_MONTH =       static_cast<_nint>(TNUM_SECONDS_IN_MONTH);
_constexpr _nint NINT_SECONDS_IN_YEAR =        static_cast<_nint>(TNUM_SECONDS_IN_YEAR);
_constexpr _nint NINT_DAYS_IN_WEEK =           static_cast<_nint>(TNUM_DAYS_IN_WEEK);
_constexpr _nint NINT_MONTHS_IN_YEAR =         static_cast<_nint>(TNUM_MONTHS_IN_YEAR);

_constexpr _ndouble NDOUBLE_ZERO =            0L;
_constexpr _ndouble NDOUBLE_HALF =          0.5L;
_constexpr _ndouble NDOUBLE_ONE =             1L;
_constexpr _ndouble NDOUBLE_MINUS_ONE =      -1L;
_constexpr _ndouble NDOUBLE_TWELVE =         12L;


_constexpr _nint ROMAN_VINCULUM_THRESHOLD = 4000LL;
_constexpr _nint ROMAN_MAXIMUM = 5000000LL;
_constexpr _nint ROMAN_NUMBER_LITERALS[] = { 1LL, 4LL, 5LL, 9LL, 10LL, 40LL, 50LL, 90LL, 100LL, 400LL, 500LL, 900LL, 1000LL,
    4000LL, 5000LL, 9000LL, 10000LL, 40000LL, 50000LL, 90000LL, 100000LL, 400000LL, 500000LL, 900000LL, 1000000LL
};


}

#endif // NUMBERS_H_INCLUDED
