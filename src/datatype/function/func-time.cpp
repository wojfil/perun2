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

#include "func-time.h"
#include "../../exception.h"


namespace perun2::func
{

p_tim F_Christmas::getValue()
{
   const p_tnum year = static_cast<p_tnum>(arg1->getValue().toInt());
   return p_tim(25, 12, year);
}

p_tim F_Easter::getValue()
{
   const p_tnum year = static_cast<p_tnum>(arg1->getValue().toInt());

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
   const p_tnum year = static_cast<p_tnum>(arg1->getValue().toInt());
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
   const p_tnum v1 = static_cast<p_tnum>(arg1->getValue().toInt());
   if (wrongMonthRuntime(v1)) {
      return p_tim();
   }

   const p_tnum v2 = static_cast<p_tnum>(arg2->getValue().toInt());
   return p_tim(v1, v2);
}


p_tim F_Time_3::getValue()
{
   const p_tnum v2 = static_cast<p_tnum>(arg2->getValue().toInt());
   if (wrongMonthRuntime(v2)) {
      return p_tim();
   }

   const p_tnum v1 = static_cast<p_tnum>(arg1->getValue().toInt());
   const p_tnum v3 = static_cast<p_tnum>(arg3->getValue().toInt());
   if (wrongDayRuntime(v1, v2, v3)) {
      return p_tim();
   }

   return p_tim(v1, v2, v3);
}


p_tim F_Time_5::getValue()
{
   const p_tnum v2 = static_cast<p_tnum>(arg2->getValue().toInt());
   if (wrongMonthRuntime(v2)) {
      return p_tim();
   }

   const p_tnum v1 = static_cast<p_tnum>(arg1->getValue().toInt());
   const p_tnum v3 = static_cast<p_tnum>(arg3->getValue().toInt());
   if (wrongDayRuntime(v1, v2, v3)) {
      return p_tim();
   }

   const p_tnum v4 = static_cast<p_tnum>(arg4->getValue().toInt());
   const p_tnum v5 = static_cast<p_tnum>(arg5->getValue().toInt());
   if (wrongSmallClockRuntime(v4, v5)) {
      return p_tim();
   }

   return p_tim(v1, v2, v3, v4, v5);
}


p_tim F_Time_6::getValue()
{
   const p_tnum v2 = static_cast<p_tnum>(arg2->getValue().toInt());
   if (wrongMonthRuntime(v2)) {
      return p_tim();
   }

   const p_tnum v1 = static_cast<p_tnum>(arg1->getValue().toInt());
   const p_tnum v3 = static_cast<p_tnum>(arg3->getValue().toInt());
   if (wrongDayRuntime(v1, v2, v3)) {
      return p_tim();
   }

   const p_tnum v4 = static_cast<p_tnum>(arg4->getValue().toInt());
   const p_tnum v5 = static_cast<p_tnum>(arg5->getValue().toInt());
   if (wrongSmallClockRuntime(v4, v5)) {
      return p_tim();
   }

   const p_tnum v6 = static_cast<p_tnum>(arg6->getValue().toInt());
   if (wrongSecondsRuntime(v6)) {
      return p_tim();
   }

   return p_tim(v1, v2, v3, v4, v5, v6);
}

}
