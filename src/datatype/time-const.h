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

#ifndef TIME_CONST_H_INCLUDED
#define TIME_CONST_H_INCLUDED

#include "primitives.h"


namespace uro
{

// _tnum is a data structure used for every element of Time and Period
// let it be "int" by default
// it can be anything, but necessarily non-unsigned
typedef _int _tnum;

inline constexpr _tnum TNUM_FIRST_YEAR =                  1601;
inline constexpr _tnum TNUM_LAST_YEAR =                  30000;

inline constexpr _tnum TNUM_DAYS_IN_NORMAL_MONTH =          30;
inline constexpr _tnum TNUM_DAYS_IN_NORMAL_YEAR =          365;
inline constexpr _tnum TNUM_DAYS_IN_LEAP_YEAR =            366;
inline constexpr _tnum TNUM_MONTHS_IN_YEAR =                12;
inline constexpr _tnum TNUM_HOURS_IN_DAY =                  24;
inline constexpr _tnum TNUM_MINUTES_IN_HOUR =               60;
inline constexpr _tnum TNUM_SECONDS_IN_MINUTE =             60;
inline constexpr _tnum TNUM_DAYS_IN_WEEK =                   7;
inline constexpr _tnum TNUM_DAYS_IN_JANUARY =               31;
inline constexpr _tnum TNUM_DAYS_IN_FEBRUARY =              28;
inline constexpr _tnum TNUM_DAYS_IN_APRIL =                 30;
inline constexpr _tnum TNUM_DAYS_IN_LEAP_FEBRUARY =         29;

inline constexpr _tnum TNUM_SECONDS_IN_HOUR =              TNUM_SECONDS_IN_MINUTE * TNUM_MINUTES_IN_HOUR;
inline constexpr _tnum TNUM_SECONDS_IN_DAY =               TNUM_SECONDS_IN_HOUR * TNUM_HOURS_IN_DAY;
inline constexpr _tnum TNUM_SECONDS_IN_WEEK =              TNUM_SECONDS_IN_DAY * TNUM_DAYS_IN_WEEK;
inline constexpr _tnum TNUM_SECONDS_IN_MONTH =             TNUM_SECONDS_IN_DAY * TNUM_DAYS_IN_NORMAL_MONTH;
inline constexpr _tnum TNUM_SECONDS_IN_YEAR =              TNUM_SECONDS_IN_DAY * TNUM_DAYS_IN_NORMAL_YEAR;
inline constexpr _tnum TNUM_HOURS_IN_MONTH =               TNUM_HOURS_IN_DAY * TNUM_DAYS_IN_NORMAL_MONTH;


inline constexpr _tnum TNUM_ZERO =                           0;
inline constexpr _tnum TNUM_ONE =                            1;
inline constexpr _tnum TNUM_MINUS_ONE =                     -1;
inline constexpr _tnum TNUM_THREE =                          3;
inline constexpr _tnum TNUM_FOUR =                           4;
inline constexpr _tnum TNUM_NINE =                           9;
inline constexpr _tnum TNUM_13 =                            13;
inline constexpr _tnum TNUM_100 =                          100;
inline constexpr _tnum TNUM_400 =                          400;

inline constexpr _tnum TNUM_JANUARY =                        1;
inline constexpr _tnum TNUM_FEBRUARY =                       2;
inline constexpr _tnum TNUM_MARCH =                          3;
inline constexpr _tnum TNUM_APRIL =                          4;
inline constexpr _tnum TNUM_MAY =                            5;
inline constexpr _tnum TNUM_JUNE =                           6;
inline constexpr _tnum TNUM_JULY =                           7;
inline constexpr _tnum TNUM_AUGUST =                         8;
inline constexpr _tnum TNUM_SEPTEMBER =                      9;
inline constexpr _tnum TNUM_OCTOBER =                       10;
inline constexpr _tnum TNUM_NOVEMBER =                      11;
inline constexpr _tnum TNUM_DECEMBER =                      12;

inline constexpr _tnum TNUM_MONDAY =                         1;
inline constexpr _tnum TNUM_TUESDAY =                        2;
inline constexpr _tnum TNUM_WEDNESDAY =                      3;
inline constexpr _tnum TNUM_THURSDAY =                       4;
inline constexpr _tnum TNUM_FRIDAY =                         5;
inline constexpr _tnum TNUM_SATURDAY =                       6;
inline constexpr _tnum TNUM_SUNDAY =                         7;


// commulative days in months
inline constexpr _tnum TNUM_CUMUL_DAYS_NORMAL[12] =
   { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
inline constexpr _tnum TNUM_CUMUL_DAYS_LEAP[12] =
   { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
inline constexpr _tnum TNUM_WEEKDAY_DATA[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

}

#endif // TIME_CONST_H_INCLUDED
