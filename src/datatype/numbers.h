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

// here all number literals, that are of type p_nint or p_ndouble
// their literals should not be hardcoded in the source code

p_constexpr p_nint NINT_ZERO =                 0LL;
p_constexpr p_nint NINT_ONE =                  1LL;
p_constexpr p_nint NINT_MINUS_ONE =           -1LL;
p_constexpr p_nint NINT_TWO =                  2LL;
p_constexpr p_nint NINT_THREE =                3LL;
p_constexpr p_nint NINT_FOUR =                 4LL;
p_constexpr p_nint NINT_TEN =                 10LL;
p_constexpr p_nint NINT_300 =                300LL;
p_constexpr p_nint NINT_THOUSAND =          1000LL;
p_constexpr p_nint NINT_1024 =              1024LL;
p_constexpr p_nint NINT_1950 =              1950LL;
p_constexpr p_nint NINT_2100 =              2100LL;
p_constexpr p_nint NINT_MILLION =        1000000LL;

p_constexpr p_nint SIZE_UNIT_KB =      NINT_1024;
p_constexpr p_nint SIZE_UNIT_MB =      SIZE_UNIT_KB * SIZE_UNIT_KB;
p_constexpr p_nint SIZE_UNIT_GB =      SIZE_UNIT_MB * SIZE_UNIT_KB;
p_constexpr p_nint SIZE_UNIT_TB =      SIZE_UNIT_GB * SIZE_UNIT_KB;
p_constexpr p_nint SIZE_UNIT_PB =      SIZE_UNIT_TB * SIZE_UNIT_KB;

p_constexpr p_nint NINT_SECONDS_IN_MINUTE =      static_cast<p_nint>(TNUM_SECONDS_IN_MINUTE);
p_constexpr p_nint NINT_SECONDS_IN_HOUR =        static_cast<p_nint>(TNUM_SECONDS_IN_HOUR);
p_constexpr p_nint NINT_SECONDS_IN_DAY =         static_cast<p_nint>(TNUM_SECONDS_IN_DAY);
p_constexpr p_nint NINT_SECONDS_IN_WEEK =        static_cast<p_nint>(TNUM_SECONDS_IN_WEEK);
p_constexpr p_nint NINT_SECONDS_IN_MONTH =       static_cast<p_nint>(TNUM_SECONDS_IN_MONTH);
p_constexpr p_nint NINT_SECONDS_IN_YEAR =        static_cast<p_nint>(TNUM_SECONDS_IN_YEAR);
p_constexpr p_nint NINT_DAYS_IN_WEEK =           static_cast<p_nint>(TNUM_DAYS_IN_WEEK);
p_constexpr p_nint NINT_MONTHS_IN_YEAR =         static_cast<p_nint>(TNUM_MONTHS_IN_YEAR);

p_constexpr p_ndouble NDOUBLE_ZERO =            0L;
p_constexpr p_ndouble NDOUBLE_HALF =          0.5L;
p_constexpr p_ndouble NDOUBLE_ONE =             1L;
p_constexpr p_ndouble NDOUBLE_MINUS_ONE =      -1L;
p_constexpr p_ndouble NDOUBLE_TWELVE =         12L;


p_constexpr p_nint ROMAN_VINCULUM_THRESHOLD = 4000LL;
p_constexpr p_nint ROMAN_MAXIMUM = 5000000LL;
p_constexpr p_nint ROMAN_NUMBER_LITERALS[] = { 1LL, 4LL, 5LL, 9LL, 10LL, 40LL, 50LL, 90LL, 100LL, 400LL, 500LL, 900LL, 1000LL,
    4000LL, 5000LL, 9000LL, 10000LL, 40000LL, 50000LL, 90000LL, 100000LL, 400000LL, 500000LL, 900000LL, 1000000LL
};


}

#endif // NUMBERS_H_INCLUDED
