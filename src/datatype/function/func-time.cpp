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

#include "func-time.h"
#include "../../exception.h"


_tim F_Christmas::getValue()
{
   const _tnum year = static_cast<_tnum>(arg1->getValue().toInt());
   return _tim(25, 12, year);
}

_tim F_Easter::getValue()
{
   const _tnum year = static_cast<_tnum>(arg1->getValue().toInt());

   const _tnum a = year % 19;
   const _tnum b = year / 100;
   const _tnum c = (b - (b / 4) - ((8 * b + 13) / 25) + (19 * a) + 15) % 30;
   const _tnum d = c - (c / 28) * (1 - (c / 28) * (29 / (c + 1)) * ((21 - a) / 11));
   const _tnum e = d - ((year + (year / 4) + d + 2 - b + (b / 4)) % 7);
   const _tnum month = 3 + ((e + 40) / 44);
   const _tnum day = e + 28 - (31 * (month / 4));

   return _tim(day, month, year);
}

_tim F_NewYear::getValue()
{
   const _tnum year = static_cast<_tnum>(arg1->getValue().toInt());
   return _tim(1, 1, year, 0, 0, 0);
}


inline void checkMonthRuntime(const _tnum& month)
{
   if (month < 1) {
      throw UroRuntimeException(str(L"months cannot be smaller than 1. Received value: ",
         toStr(month)));
   }
   else if (month > 12) {
      throw UroRuntimeException(str(L"there are only 12 months in a year. Received value: ",
         toStr(month)));
   }
}

inline void checkDayRuntime(const _tnum& day, const _tnum& month,
   const _tnum& year)
{
   if (day < 1) {
      throw UroRuntimeException(str(L"day cannot be smaller than 1. Received value: ",
         toStr(day)));
   }

   const _tnum expected = daysInMonth(month, year);
   if (day > expected) {
      throw UroRuntimeException(str(L"month ", monthToString(month), L" has only ",
         toStr(expected), L" days. Received value: ", toStr(day)));
   }
}

inline void checkSmallClockRuntime(const _tnum& hour, const _tnum& minute)
{
   if (hour < 0) {
      throw UroRuntimeException(str(L"value of hours cannot be smaller than 0. Received value: ",
         toStr(hour)));
   }
   else if (hour >= 24) {
      throw UroRuntimeException(str(L"value of hours cannot be greater than 23. Received value: ",
         toStr(hour)));
   }
   else if (minute < 0) {
      throw UroRuntimeException(str(L"value of minutes cannot be smaller than 0. Received value: ",
         toStr(minute)));
   }
   else if (minute >= 60) {
      throw UroRuntimeException(str(L"value of minutes cannot be greater than 59. Received value: ",
         toStr(minute)));
   }
}

inline void checkSecondsRuntime(const _tnum& second)
{
   if (second < 0) {
      throw UroRuntimeException(str(L"value of seconds cannot be smaller than 0. Received value: ",
         toStr(second)));
   }
   else if (second >= 60) {
      throw UroRuntimeException(str(L"value of seconds cannot be greater than 59. Received value: ",
         toStr(second)));
   }
}


_tim F_Time_2::getValue()
{
   const _tnum v1 = static_cast<_tnum>(arg1->getValue().toInt());
   checkMonthRuntime(v1);

   const _tnum v2 = static_cast<_tnum>(arg2->getValue().toInt());
   return _tim(v1, v2);
}


_tim F_Time_3::getValue()
{
   const _tnum v2 = static_cast<_tnum>(arg2->getValue().toInt());
   checkMonthRuntime(v2);

   const _tnum v1 = static_cast<_tnum>(arg1->getValue().toInt());
   const _tnum v3 = static_cast<_tnum>(arg3->getValue().toInt());
   checkDayRuntime(v1, v2, v3);

   return _tim(v1, v2, v3);
}


_tim F_Time_5::getValue()
{
   const _tnum v2 = static_cast<_tnum>(arg2->getValue().toInt());
   checkMonthRuntime(v2);

   const _tnum v1 = static_cast<_tnum>(arg1->getValue().toInt());
   const _tnum v3 = static_cast<_tnum>(arg3->getValue().toInt());
   checkDayRuntime(v1, v2, v3);

   const _tnum v4 = static_cast<_tnum>(arg4->getValue().toInt());
   const _tnum v5 = static_cast<_tnum>(arg5->getValue().toInt());
   checkSmallClockRuntime(v4, v5);

   return _tim(v1, v2, v3, v4, v5);
}


_tim F_Time_6::getValue()
{
   const _tnum v2 = static_cast<_tnum>(arg2->getValue().toInt());
   checkMonthRuntime(v2);

   const _tnum v1 = static_cast<_tnum>(arg1->getValue().toInt());
   const _tnum v3 = static_cast<_tnum>(arg3->getValue().toInt());
   checkDayRuntime(v1, v2, v3);

   const _tnum v4 = static_cast<_tnum>(arg4->getValue().toInt());
   const _tnum v5 = static_cast<_tnum>(arg5->getValue().toInt());
   checkSmallClockRuntime(v4, v5);

   const _tnum v6 = static_cast<_tnum>(arg6->getValue().toInt());
   checkSecondsRuntime(v6);

   return _tim(v1, v2, v3, v4, v5, v6);
}

