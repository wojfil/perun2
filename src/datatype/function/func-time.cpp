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

#include "func-time.h"
#include "../../exception.h"


namespace perun2::func
{

#define P_NUM_ARG_IN_TIME_FUNC(a, n, v) \
   const p_num n = a->getValue(); \
   if (n.isNaN()) { \
      return p_tim();\
   } \
   const p_tnum v = static_cast<p_tnum>(n.toInt());


p_tim F_Christmas::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg1, n, v);
   return p_tim(25, 12, v);
}

p_tim F_Easter::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg1, n, year);

   const p_tnum a = year % 19;
   const p_tnum b = year / 100;
   const p_tnum c = (b - (b / 4) - ((8 * b + 13) / 25) + (19 * a) + 15) % 30;
   const p_tnum d = c - (c / 28) * (1 - (c / 28) * (29 / (c + 1)) * ((21 - a) / 11));
   const p_tnum e = d - ((year + (year / 4) + d + 2 - b + (b / 4)) % 7);
   const p_tnum month = 3 + ((e + 40) / 44);
   const p_tnum day = e + 28 - (31 * (month / 4));

   return p_tim(day, month, year);
}

p_tim F_NewYear::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg1, n, year);
   return p_tim(1, 1, year, 0, 0, 0);
}


inline p_bool wrongMonthRuntime(const p_tnum month)
{
   return month < 1 || month > 12;
}


inline p_bool wrongDayRuntime(const p_tnum day, const p_tnum month, const p_tnum year)
{
   if (day < 1) {
      return true;
   }

   return day > daysInMonth(month, year);
}


inline p_bool wrongSmallClockRuntime(const p_tnum hour, const p_tnum minute)
{
   return hour < 0 || hour >= 24
      || minute < 0 || minute >= 60;
}


inline p_bool wrongSecondsRuntime(const p_tnum second)
{
   return second < 0 || second >= 60;
}


p_tim F_Time_2::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg1, n1, v1);
   if (wrongMonthRuntime(v1)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg2, n2, v2);
   return p_tim(v1, v2);
}


p_tim F_Time_3::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg2, n2, v2);
   if (wrongMonthRuntime(v2)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg1, n1, v1);
   P_NUM_ARG_IN_TIME_FUNC(arg3, n3, v3);
   if (wrongDayRuntime(v1, v2, v3)) {
      return p_tim();
   }

   return p_tim(v1, v2, v3);
}


p_tim F_Time_5::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg2, n2, v2);
   if (wrongMonthRuntime(v2)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg1, n1, v1);
   P_NUM_ARG_IN_TIME_FUNC(arg3, n3, v3);
   if (wrongDayRuntime(v1, v2, v3)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg4, n4, v4);
   P_NUM_ARG_IN_TIME_FUNC(arg5, n5, v5);
   if (wrongSmallClockRuntime(v4, v5)) {
      return p_tim();
   }

   return p_tim(v1, v2, v3, v4, v5);
}


p_tim F_Time_6::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg2, n2, v2);
   if (wrongMonthRuntime(v2)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg1, n1, v1);
   P_NUM_ARG_IN_TIME_FUNC(arg3, n3, v3);
   if (wrongDayRuntime(v1, v2, v3)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg4, n4, v4);
   P_NUM_ARG_IN_TIME_FUNC(arg5, n5, v5);
   if (wrongSmallClockRuntime(v4, v5)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg6, n6, v6);
   if (wrongSecondsRuntime(v6)) {
      return p_tim();
   }

   return p_tim(v1, v2, v3, v4, v5, v6);
}


p_tim F_Clock_2::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg1, n1, v1);
   P_NUM_ARG_IN_TIME_FUNC(arg2, n2, v2);
   if (wrongSmallClockRuntime(v1, v2)) {
      return p_tim();
   }

   return p_tim::clock(v1, v2);
}


p_tim F_Clock_3::getValue()
{
   P_NUM_ARG_IN_TIME_FUNC(arg1, n1, v1);
   P_NUM_ARG_IN_TIME_FUNC(arg2, n2, v2);
   if (wrongSmallClockRuntime(v1, v2)) {
      return p_tim();
   }

   P_NUM_ARG_IN_TIME_FUNC(arg3, n3, v3);
   if (wrongSecondsRuntime(v3)) {
      return p_tim();
   }

   return p_tim::clock(v1, v2, v3);
}


}
