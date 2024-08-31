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

#include "primitives.h"


namespace perun2
{

// p_tnum is a data structure used for every element of Time and Period
// let it be "int" by default
// it can be anything, but necessarily signed
typedef p_int p_tnum;

p_constexpr p_tnum TNUM_DAYS_IN_NORMAL_MONTH =          30;
p_constexpr p_tnum TNUM_DAYS_IN_NORMAL_YEAR =          365;
p_constexpr p_tnum TNUM_DAYS_IN_LEAP_YEAR =            366;
p_constexpr p_tnum TNUM_MONTHS_IN_YEAR =                12;
p_constexpr p_tnum TNUM_HOURS_IN_DAY =                  24;
p_constexpr p_tnum TNUM_MINUTES_IN_HOUR =               60;
p_constexpr p_tnum TNUM_SECONDS_IN_MINUTE =             60;
p_constexpr p_tnum TNUM_DAYS_IN_WEEK =                   7;
p_constexpr p_tnum TNUM_DAYS_IN_JANUARY =               31;
p_constexpr p_tnum TNUM_DAYS_IN_FEBRUARY =              28;
p_constexpr p_tnum TNUM_DAYS_IN_APRIL =                 30;
p_constexpr p_tnum TNUM_DAYS_IN_LEAP_FEBRUARY =         29;

p_constexpr p_tnum TNUM_SECONDS_IN_HOUR =              TNUM_SECONDS_IN_MINUTE * TNUM_MINUTES_IN_HOUR;
p_constexpr p_tnum TNUM_SECONDS_IN_DAY =               TNUM_SECONDS_IN_HOUR * TNUM_HOURS_IN_DAY;
p_constexpr p_tnum TNUM_SECONDS_IN_WEEK =              TNUM_SECONDS_IN_DAY * TNUM_DAYS_IN_WEEK;
p_constexpr p_tnum TNUM_SECONDS_IN_MONTH =             TNUM_SECONDS_IN_DAY * TNUM_DAYS_IN_NORMAL_MONTH;
p_constexpr p_tnum TNUM_SECONDS_IN_YEAR =              TNUM_SECONDS_IN_DAY * TNUM_DAYS_IN_NORMAL_YEAR;
p_constexpr p_tnum TNUM_MINUTES_IN_DAY =               TNUM_MINUTES_IN_HOUR * TNUM_HOURS_IN_DAY;
p_constexpr p_tnum TNUM_MINUTES_IN_WEEK =              TNUM_MINUTES_IN_DAY * TNUM_DAYS_IN_WEEK;
p_constexpr p_tnum TNUM_MINUTES_IN_MONTH =             TNUM_MINUTES_IN_DAY * TNUM_DAYS_IN_NORMAL_MONTH;
p_constexpr p_tnum TNUM_MINUTES_IN_YEAR =              TNUM_MINUTES_IN_DAY * TNUM_DAYS_IN_NORMAL_YEAR;
p_constexpr p_tnum TNUM_HOURS_IN_WEEK =                TNUM_HOURS_IN_DAY * TNUM_DAYS_IN_WEEK;
p_constexpr p_tnum TNUM_HOURS_IN_MONTH =               TNUM_HOURS_IN_DAY * TNUM_DAYS_IN_NORMAL_MONTH;
p_constexpr p_tnum TNUM_HOURS_IN_YEAR =                TNUM_HOURS_IN_DAY * TNUM_DAYS_IN_NORMAL_YEAR;

p_constexpr p_tnum TNUM_ZERO =                           0;
p_constexpr p_tnum TNUM_ONE =                            1;
p_constexpr p_tnum TNUM_MINUS_ONE =                     -1;
p_constexpr p_tnum TNUM_THREE =                          3;
p_constexpr p_tnum TNUM_FOUR =                           4;
p_constexpr p_tnum TNUM_NINE =                           9;
p_constexpr p_tnum TNUM_13 =                            13;
p_constexpr p_tnum TNUM_100 =                          100;
p_constexpr p_tnum TNUM_400 =                          400;

p_constexpr p_tnum TNUM_JANUARY =                        1;
p_constexpr p_tnum TNUM_FEBRUARY =                       2;
p_constexpr p_tnum TNUM_MARCH =                          3;
p_constexpr p_tnum TNUM_APRIL =                          4;
p_constexpr p_tnum TNUM_MAY =                            5;
p_constexpr p_tnum TNUM_JUNE =                           6;
p_constexpr p_tnum TNUM_JULY =                           7;
p_constexpr p_tnum TNUM_AUGUST =                         8;
p_constexpr p_tnum TNUM_SEPTEMBER =                      9;
p_constexpr p_tnum TNUM_OCTOBER =                       10;
p_constexpr p_tnum TNUM_NOVEMBER =                      11;
p_constexpr p_tnum TNUM_DECEMBER =                      12;

p_constexpr p_tnum TNUM_MONDAY =                         1;
p_constexpr p_tnum TNUM_TUESDAY =                        2;
p_constexpr p_tnum TNUM_WEDNESDAY =                      3;
p_constexpr p_tnum TNUM_THURSDAY =                       4;
p_constexpr p_tnum TNUM_FRIDAY =                         5;
p_constexpr p_tnum TNUM_SATURDAY =                       6;
p_constexpr p_tnum TNUM_SUNDAY =                         7;

// commulative days in months
p_constexpr p_tnum TNUM_CUMUL_DAYS_NORMAL[12] =
   { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
p_constexpr p_tnum TNUM_CUMUL_DAYS_LEAP[12] =
   { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

p_constexpr p_tnum TNUM_WEEKDAY_DATA[] = 
   { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

}
