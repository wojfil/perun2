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

#include "time.h"
#include "strings.h"
#include <vector>
#include <sstream>
#include <math.h>
#include <algorithm>


namespace uro
{

Time::Time() 
   : day(TNUM_ONE), month(TNUM_ONE), year(TNUM_FIRST_YEAR), type(TimeType::tt_Clock) { };

Time::Time(const _tnum& mo, const _tnum& ye) 
   : day(TNUM_ZERO), month(mo), year(ye), type(TimeType::tt_YearMonth) { };

Time::Time(const _tnum& da, const _tnum& mo, const _tnum& ye) 
   : day(da), month(mo), year(ye), type(TimeType::tt_Date) { };

Time::Time(const _tnum& da, const _tnum& mo, const _tnum& ye, const _tnum& ho, const _tnum& mi) 
   : day(da), month(mo), year(ye), hour(ho), minute(mi), type(TimeType::tt_ShortClock) { };

Time::Time(const _tnum& da, const _tnum& mo, const _tnum& ye, const _tnum& ho, const _tnum& mi, const _tnum& sec) 
   : day(da), month(mo), year(ye), hour(ho), minute(mi), second(sec), type(TimeType::tt_Clock) { };


_str Time::toString() const
{
   if (this->isEmpty()) {
      return STRING_NO_TIME;
   }

   _stream ss;

   if (type != tt_YearMonth) {
      ss << day;
      ss << CHAR_SPACE;
   }

   ss << monthToString(month);
   ss << CHAR_SPACE;
   ss << year;

   if (type == tt_ShortClock || type == tt_Clock) {
      ss << STRING_COMMA_SPACE;
      ss << fillTimeUnit(hour);
      ss << CHAR_COLON;
      ss << fillTimeUnit(minute);
      if (type == tt_Clock) {
         ss << CHAR_COLON;
         ss << fillTimeUnit(second);
      }
   }

   return ss.str();
}

void Time::addYears(const _tnum& y)
{
   year += y;
   if (type != tt_YearMonth && month == TNUM_FEBRUARY
       && day == TNUM_DAYS_IN_LEAP_FEBRUARY && !isLeapYear(year))
   {
      day = TNUM_DAYS_IN_LEAP_FEBRUARY;
   }
}

void Time::addMonths(const _tnum& m)
{
   const _tnum m2 = m % TNUM_MONTHS_IN_YEAR;
   const _tnum y = m / TNUM_MONTHS_IN_YEAR;
   month += m2;
   year += y;

   if (month > TNUM_MONTHS_IN_YEAR) {
      month -= TNUM_MONTHS_IN_YEAR;
      year++;
   }

   if (type != tt_YearMonth && day >= TNUM_DAYS_IN_LEAP_FEBRUARY) {
      const _tnum max = daysInMonth(month, year);
      if (day > max) {
         day = max;
      }
   }
}

void Time::addWeeks(const _tnum& w)
{
   addDays(w * TNUM_DAYS_IN_WEEK);
}

// todo:  possible optimization
// todo:  to make faster calculations with big numbers
// todo:  at first, compute how many whole years these days contain
// todo:  do not forget taking leap years into account
void Time::addDays(const _tnum& d)
{
   if (type == tt_YearMonth) {
      day = d < TNUM_ZERO ? TNUM_ONE : daysInMonth(month, year);
      type = tt_Date;
   }

   if (d > TNUM_ZERO) {
      _tnum d2 = d;

      while (d2 != TNUM_ZERO) {
         const _tnum dif = daysInMonth(month, year) - day;
         if (d2 <= dif) {
            day += d2;
            d2 = TNUM_ZERO;
         }
         else {
            d2 -= dif + TNUM_ONE;
            month++;
            if (month == TNUM_13) {
               month = TNUM_JANUARY;
               year++;
            }
            day = TNUM_ONE;
         }
      }
   }
   else if (d < TNUM_ZERO) {
      _tnum d2 = -d;

      while (d2 != TNUM_ZERO) {
         if (d2 < day) {
            day -= d2;
            d2 = TNUM_ZERO;
         }
         else {
            d2 -= day;
            month--;
            if (month == TNUM_ZERO) {
               month = TNUM_DECEMBER;
               year--;
            }
            day = daysInMonth(month, year);
         }
      }
   }
}

void Time::addHours(const _tnum& h)
{
   initClock(false, h);

   const _tnum h2 = h % TNUM_HOURS_IN_DAY;
   _tnum d = h / TNUM_HOURS_IN_DAY;
   hour += h2;
   if (hour >= TNUM_HOURS_IN_DAY) {
      hour -= TNUM_HOURS_IN_DAY;
      d++;
   }
   else if (hour < TNUM_ZERO) {
      hour += TNUM_HOURS_IN_DAY;
      d--;
   }

   if (d != TNUM_ZERO) {
      addDays(d);
   }
}

void Time::addMinutes(const _tnum& m)
{
   initClock(false, m);

   const _tnum m2 = m % TNUM_MINUTES_IN_HOUR;
   _tnum h = m / TNUM_MINUTES_IN_HOUR;
   minute += m2;
   if (minute >= TNUM_MINUTES_IN_HOUR) {
      minute -= TNUM_MINUTES_IN_HOUR;
      h++;
   }
   else if (minute < TNUM_ZERO) {
      minute += TNUM_MINUTES_IN_HOUR;
      h--;
   }

   if (h != TNUM_ZERO) {
      addHours(h);
   }
}

void Time::addSeconds(const _tnum& s)
{
   initClock(true, s);

   const _tnum s2 = s % TNUM_SECONDS_IN_MINUTE;
   _tnum m = s / TNUM_SECONDS_IN_MINUTE;
   second += s2;
   if (second >= TNUM_SECONDS_IN_MINUTE) {
      second -= TNUM_SECONDS_IN_MINUTE;
      m++;
   }
   else if (second < TNUM_ZERO) {
      second += TNUM_SECONDS_IN_MINUTE;
      m--;
   }

   if (m != TNUM_ZERO) {
      addMinutes(m);
   }
}

void Time::setYear(const _tnum& y)
{
   year = y;
}

void Time::setMonth(const _tnum& m)
{
   if (m < TNUM_JANUARY) {
      throw RuntimeError(str(L"value of month cannot be smaller than 1 (received: ",
         toStr(m), L")"));
   }
   else if (m > TNUM_DECEMBER) {
      throw RuntimeError(str(L"value of month cannot be greater than 12 (received: ",
         toStr(m), L")"));
   }

   month = m;
}

void Time::setDay(const _tnum& d)
{
   if (d < TNUM_JANUARY) {
      throw RuntimeError(str(L"value of day cannot be smaller than 1 (received: ",
         toStr(d), L")"));
   }

   const _tnum inMonth = daysInMonth(month, year);
   if (d > inMonth) {
      const _str name = monthToString(month);
      throw RuntimeError(str(name, L" ", toStr(year),
         L"contains only ", toStr(inMonth), L" days (received day: ",
         toStr(d), L")"));
   }

   day = d;
}

void Time::setHour(const _tnum& h)
{
   if (h < TNUM_ZERO) {
      throw RuntimeError(str(L"value of hours cannot be smaller than 0 (received: ",
         toStr(h), L")"));
   }
   else if (h >= TNUM_HOURS_IN_DAY) {
      throw RuntimeError(str(L"value of hours cannot be greater than 23 (received: ",
         toStr(h), L")"));
   }

   hour = h;
}

void Time::setMinute(const _tnum& m)
{
   if (m < TNUM_ZERO) {
      throw RuntimeError(str(L"value of minutes cannot be smaller than 0 (received: ",
         toStr(m), L")"));
   }
   else if (m >= TNUM_MINUTES_IN_HOUR) {
      throw RuntimeError(str(L"value of minutes cannot be greater than 59 (received: ",
         toStr(m), L")"));
   }

   minute = m;
}

void Time::setSecond(const _tnum& s)
{
   if (s < TNUM_ZERO) {
      throw RuntimeError(str(L"value of seconds cannot be smaller than 0 (received: ",
         toStr(s), L")"));
   }
   else if (s >= TNUM_SECONDS_IN_MINUTE) {
      throw RuntimeError(str(L"value of seconds cannot be greater than 59 (received: ",
         toStr(s), L")"));
   }

   second = s;
}

Time Time::toDate() const
{
   switch (type) {
      case tt_YearMonth: {
         return Time(month, year);
      }
      default: {
         return Time(day, month, year);
      }
   }
}

_tnum Time::getWeekDay() const
{
   const _tnum y = year - (month < TNUM_THREE);
   const _tnum wd = (y + y / TNUM_FOUR - y / TNUM_100 + y / TNUM_400 
      + TNUM_WEEKDAY_DATA[month - TNUM_ONE] + day) % TNUM_DAYS_IN_WEEK;
   return wd == TNUM_ZERO ? TNUM_DAYS_IN_WEEK : wd;
}

_bool Time::equalsExactly(const Time& tim) const
{
   return type == tim.type && *this == tim;
}

_bool Time::isEmpty() const
{
   return year == TNUM_FIRST_YEAR
       && type == TimeType::tt_Clock
       && month == TNUM_ONE
       && day == TNUM_ONE
       && hour == TNUM_ZERO
       && minute == TNUM_ZERO
       && second == TNUM_ZERO;
}

void Time::setValue(const Time& tim)
{
   year = tim.year >= TNUM_FIRST_YEAR
      ? tim.year
      : TNUM_FIRST_YEAR;

   if (year > TNUM_LAST_YEAR) {
      year = TNUM_LAST_YEAR;
   }

   month = tim.month;

   if (tim.type == TimeType::tt_YearMonth) {
      const _tnum d = daysInMonth(month, year);
      if (day > d) {
         day = d;
      }
      return;
   }

   day = tim.day;

   if (tim.type == TimeType::tt_Date) {
      return;
   }

   hour = tim.hour;
   minute = tim.minute;

   if (tim.type == TimeType::tt_ShortClock) {
      return;
   }

   second = tim.second;
}

Time& Time::operator += (const Period& per)
{
   const _tnum d = per.days + per.years_ad + per.months_ad;

   if (per.seconds != TNUM_ZERO)
      addSeconds(per.seconds);

   if (per.minutes != TNUM_ZERO)
      addMinutes(per.minutes);

   if (per.hours != TNUM_ZERO)
      addHours(per.hours);

   if (d != TNUM_ZERO)
      addDays(d);

   if (per.weeks != TNUM_ZERO)
      addDays(per.weeks * TNUM_DAYS_IN_WEEK);

   if (per.months != TNUM_ZERO)
      addMonths(per.months);

   if (per.years != TNUM_ZERO)
      addYears(per.years);

   return *this;
}

Time& Time::operator -= (const Period& per)
{
   const _tnum d = -per.days - per.years_ad - per.months_ad;

   if (per.seconds != TNUM_ZERO)
      addSeconds(-per.seconds);

   if (per.minutes != TNUM_ZERO)
      addMinutes(-per.minutes);

   if (per.hours != TNUM_ZERO)
      addHours(-per.hours);

   if (d != TNUM_ZERO)
      addDays(d);

   if (per.weeks != TNUM_ZERO)
      addDays(-per.weeks * TNUM_DAYS_IN_WEEK);

   if (per.months != TNUM_ZERO)
      addMonths(-per.months);

   if (per.years != TNUM_ZERO)
      addYears(-per.years);

   return *this;
}

// add clock to an already existing time
void Time::initClock(const _bool& withSeconds, const _tnum& recentChange)
{
   if (type == tt_YearMonth || type == tt_Date) {
      hour = TNUM_ZERO;
      minute = TNUM_ZERO;

      if (withSeconds) {
         second = TNUM_ZERO;
      }

      if (type == tt_YearMonth) {
         day = TNUM_ONE;
         if (recentChange > TNUM_ZERO) {
            month++;
            if (month == TNUM_13) {
               month = TNUM_ONE;
               year++;
            }
         }
      }
      else if (recentChange > TNUM_ZERO) {
         day++;
         if (day > daysInMonth(month, year)) {
            day = TNUM_ONE;
            month++;
            if (month == TNUM_13) {
               month = TNUM_ONE;
               year++;
            }
         }
      }

      type = withSeconds ? tt_Clock : tt_ShortClock;
   }
}

_bool Time::operator == (const Time& tim) const
{
   if (type == tt_YearMonth || tim.type == tt_YearMonth) {
      return month == tim.month
          && year == tim.year;
   }

   if (type == tt_Date || tim.type == tt_Date) {
      return day == tim.day
          && month == tim.month
          && year == tim.year;
   }

   if (type == tt_ShortClock || tim.type == tt_ShortClock) {
      return day == tim.day
          && month == tim.month
          && year == tim.year
          && hour == tim.hour
          && minute == tim.minute;
   }

   return day == tim.day
       && month == tim.month
       && year == tim.year
       && hour == tim.hour
       && minute == tim.minute
       && second == tim.second;
}

_bool Time::operator != (const Time& tim) const
{
   if (type == tt_YearMonth || tim.type == tt_YearMonth) {
      return month != tim.month
          || year != tim.year;
   }

   if (type == tt_Date || tim.type == tt_Date) {
      return day != tim.day
          || month != tim.month
          || year != tim.year;
   }

   if (type == tt_ShortClock || tim.type == tt_ShortClock) {
      return day != tim.day
          || month != tim.month
          || year != tim.year
          || hour != tim.hour
          || minute != tim.minute;
   }

   return day != tim.day
       || month != tim.month
       || year != tim.year
       || hour != tim.hour
       || minute != tim.minute
       || second != tim.second;
}

_bool Time::operator < (const Time& tim) const
{
   if (year < tim.year) { return true; }
   else if (year > tim.year) { return false; }

   if (month < tim.month) { return true; }
   else if (month > tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return false; }

   if (day < tim.day) { return true; }
   else if (day > tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return false; }

   if (hour < tim.hour) { return true; }
   else if (hour > tim.hour) { return false; }

   if (minute < tim.minute) { return true; }
   else if (minute > tim.minute) { return false; }

   if (type == tt_ShortClock || tim.type == tt_ShortClock) { return false; }

   return second < tim.second;
}

_bool Time::operator > (const Time& tim) const
{
   if (year > tim.year) { return true; }
   else if (year < tim.year) { return false; }

   if (month > tim.month) { return true; }
   else if (month < tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return false; }

   if (day > tim.day) { return true; }
   else if (day < tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return false; }

   if (hour > tim.hour) { return true; }
   else if (hour < tim.hour) { return false; }

   if (minute > tim.minute) { return true; }
   else if (minute < tim.minute) { return false; }

   if (type == tt_ShortClock || tim.type == tt_ShortClock) { return false; }

   return second > tim.second;
}

_bool Time::operator <= (const Time& tim) const
{
   if (year < tim.year) { return true; }
   else if (year > tim.year) { return false; }

   if (month < tim.month) { return true; }
   else if (month > tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return true; }

   if (day < tim.day) { return true; }
   else if (day > tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return true; }

   if (hour < tim.hour) { return true; }
   else if (hour > tim.hour) { return false; }

   if (minute < tim.minute) { return true; }
   else if (minute > tim.minute) { return false; }

   if (type == tt_ShortClock || tim.type == tt_ShortClock) { return true; }

   return second <= tim.second;
}

_bool Time::operator >= (const Time& tim) const
{
   if (year > tim.year) { return true; }
   else if (year < tim.year) { return false; }

   if (month > tim.month) { return true; }
   else if (month < tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return true; }

   if (day > tim.day) { return true; }
   else if (day < tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return true; }

   if (hour > tim.hour) { return true; }
   else if (hour < tim.hour) { return false; }

   if (minute > tim.minute) { return true; }
   else if (minute < tim.minute) { return false; }

   if (type == tt_ShortClock || tim.type == tt_ShortClock) { return true; }

   return second >= tim.second;
}

Period Time::operator - (const Time& tim) const
{
   if (tim == *this) {
      return Period();
   }
   else if (*this < tim) {
      Period per = timeDifference(*this, tim);
      per.reverse();
      return per;
   }
   else {
      return timeDifference(tim, *this);
   }
}

_tnum toTimeNumber(const Number& num)
{
   return num.isDouble
      ? round(num.value.d)
      : static_cast<_tnum>(num.value.i);
}

_str monthToString(const _tnum& month)
{
   switch (month) {
      case TNUM_JANUARY:
         return STRING_MONTH_JANUARY;
      case TNUM_FEBRUARY:
         return STRING_MONTH_FEBRUARY;
      case TNUM_MARCH:
         return STRING_MONTH_MARCH;
      case TNUM_APRIL:
         return STRING_MONTH_APRIL;
      case TNUM_MAY:
         return STRING_MONTH_MAY;
      case TNUM_JUNE:
         return STRING_MONTH_JUNE;
      case TNUM_JULY:
         return STRING_MONTH_JULY;
      case TNUM_AUGUST:
         return STRING_MONTH_AUGUST;
      case TNUM_SEPTEMBER:
         return STRING_MONTH_SEPTEMBER;
      case TNUM_OCTOBER:
         return STRING_MONTH_OCTOBER;
      case TNUM_NOVEMBER:
         return STRING_MONTH_NOVEMBER;
      case TNUM_DECEMBER:
         return STRING_MONTH_DECEMBER;
      default:
         return EMPTY_STRING;
   }
}

_str weekdayToString(const _tnum& wday)
{
   switch (wday) {
      case TNUM_MONDAY:
         return STRING_WEEKDAY_MONDAY;
      case TNUM_TUESDAY:
         return STRING_WEEKDAY_TUESDAY;
      case TNUM_WEDNESDAY:
         return STRING_WEEKDAY_WEDNESDAY;
      case TNUM_THURSDAY:
         return STRING_WEEKDAY_THURSDAY;
      case TNUM_FRIDAY:
         return STRING_WEEKDAY_FRIDAY;
      case TNUM_SATURDAY:
         return STRING_WEEKDAY_SATURDAY;
      case TNUM_SUNDAY:
         return STRING_WEEKDAY_SUNDAY;
      default:
         return EMPTY_STRING;
   }
}

inline _str fillTimeUnit(const _tnum& val)
{
   return val <= TNUM_NINE
      ? str(STRING_0, toStr(val))
      : toStr(val);
}

inline _bool isLeapYear(const _tnum& year)
{
   if (year % 4 == TNUM_ZERO) {
      if (year % TNUM_100 == TNUM_ZERO) {
         return year % TNUM_400 == TNUM_ZERO;
      }
      return true;
   }
   return false;
}

_tnum daysInMonth(const _tnum& month, const _tnum& year)
{
   switch (month) {
      case TNUM_ZERO: // 0th month = December
      case TNUM_JANUARY:
      case TNUM_MARCH:
      case TNUM_MAY:
      case TNUM_JULY:
      case TNUM_AUGUST:
      case TNUM_OCTOBER:
      case TNUM_DECEMBER:
      case TNUM_13: // 13th month = January
         return TNUM_DAYS_IN_JANUARY;
      case TNUM_FEBRUARY:
         return isLeapYear(year) ? TNUM_DAYS_IN_LEAP_FEBRUARY : TNUM_DAYS_IN_FEBRUARY;
      default:
         return TNUM_DAYS_IN_APRIL;
   }
}


inline Period timeDifference(const Time& min, const Time& max)
{
   Period p;
   p.periodType = Period::pt_Difference;
   p.years_sec = max.year - min.year;
   p.months_sec = max.month - min.month;

   if (p.months_sec < TNUM_ZERO) {
      p.months_sec += TNUM_MONTHS_IN_YEAR;
      p.years_sec--;

      p.months_ad = (isLeapYear(max.year)
            ? TNUM_CUMUL_DAYS_LEAP[max.month - TNUM_ONE]
            : TNUM_CUMUL_DAYS_NORMAL[max.month - TNUM_ONE])
         + (isLeapYear(min.year)
            ? (TNUM_DAYS_IN_LEAP_YEAR - TNUM_CUMUL_DAYS_LEAP[min.month - TNUM_ONE])
            : (TNUM_DAYS_IN_NORMAL_YEAR - TNUM_CUMUL_DAYS_NORMAL[min.month - TNUM_ONE]));

      if (p.years_sec != TNUM_ZERO) {
         p.years_ad = daysInYears(min.year + TNUM_ONE, max.year - TNUM_ONE);
      }
   }
   else {
      p.months_ad = isLeapYear(min.year)
         ? TNUM_CUMUL_DAYS_LEAP[max.month - TNUM_ONE] - TNUM_CUMUL_DAYS_LEAP[min.month - TNUM_ONE]
         : TNUM_CUMUL_DAYS_NORMAL[max.month - TNUM_ONE] - TNUM_CUMUL_DAYS_NORMAL[min.month - TNUM_ONE];

      if (p.years_sec != TNUM_ZERO) {
         p.years_ad = (min.month <= TNUM_FEBRUARY)
            ? daysInYears(min.year, max.year - TNUM_ONE)
            : daysInYears(min.year + TNUM_ONE, max.year);
      }
   }

   switch (min.type) {
      case Time::tt_YearMonth: {
         if (max.type == Time::tt_YearMonth) {
            return p;
         }
         p.days = max.day;
         decrementMonth(p, min, false);

         if (max.type == Time::tt_Date) {
            return p;
         }

         p.hours = max.hour;
         p.minutes = max.minute;
         if (max.type == Time::tt_ShortClock) {
            return p;
         }

         p.seconds = max.second;
         return p;
      }
      case Time::tt_Date: {
         if (max.type == Time::tt_YearMonth) {
            decrementMonth(p, min, true);
            p.days++;
            p.days -= min.day;
            return p;
         }
         p.days = max.day - min.day;
         if (p.days < TNUM_ZERO) {
            decrementMonth(p, min, true);
         }
         if (max.type == Time::tt_Date) {
            return p;
         }

         p.hours = max.hour;
         p.minutes = max.minute;
         if (max.type == Time::tt_ShortClock) {
            return p;
         }

         p.seconds = max.second;
         return p;
      }
      case Time::tt_ShortClock: {
         if (max.type == Time::tt_YearMonth) {
            decrementMonth(p, min, true);
            p.days -= min.day;
            shortClockTillMidnight(p, min);
            return p;
         }
         p.days = max.day - min.day;
         if (p.days < TNUM_ZERO) {
            decrementMonth(p, min, true);
         }

         if (max.type == Time::tt_Date) {
            decrementDay(p, min);
            shortClockTillMidnight(p, min);
            return p;
         }

         p.hours = max.hour - min.hour;
         p.minutes = max.minute - min.minute;
         if (p.minutes < TNUM_ZERO) {
            p.minutes += TNUM_MINUTES_IN_HOUR;
            p.hours--;
         }
         if (p.hours < TNUM_ZERO) {
            p.hours += TNUM_HOURS_IN_DAY;
            decrementDay(p, min);
         }

         if (max.type == Time::tt_ShortClock) {
            return p;
         }

         p.seconds = max.second;
         return p;
      }
      case Time::tt_Clock: {
         if (max.type == Time::tt_YearMonth) {
            decrementMonth(p, min, true);
            p.days -= min.day;
            clockTillMidnight(p, min);
            return p;
         }
         p.days = max.day - min.day;
         if (p.days < TNUM_ZERO) {
            decrementMonth(p, min, true);
         }

         if (max.type == Time::tt_Date) {
            decrementDay(p, min);
            clockTillMidnight(p, min);
            return p;
         }

         p.hours = max.hour - min.hour;
         p.minutes = max.minute - min.minute;
         p.seconds = max.second - min.second;

         if (p.seconds < TNUM_ZERO) {
            p.seconds += TNUM_SECONDS_IN_MINUTE;
            p.minutes--;
         }
         if (p.minutes < TNUM_ZERO) {
            p.minutes += TNUM_MINUTES_IN_HOUR;
            p.hours--;
         }
         if (p.hours < TNUM_ZERO) {
            p.hours += TNUM_HOURS_IN_DAY;
            decrementDay(p, min);
         }

         return p;
      }
   }

   return Period();
}

// number of days between two years (including bounds)
inline _tnum daysInYears(const _tnum& min, const _tnum& max)
{
   if (min == max) {
      return isLeapYear(min) 
         ? TNUM_DAYS_IN_LEAP_YEAR 
         : TNUM_DAYS_IN_NORMAL_YEAR;
   }
   else {
      return TNUM_DAYS_IN_NORMAL_YEAR * (max - min + TNUM_ONE)
         + (max / TNUM_FOUR) - (min / TNUM_FOUR)
         - (max / TNUM_100) + (min / TNUM_100)
         + (max / TNUM_400) - (min / TNUM_400)
         + (isLeapYear(min) ? TNUM_ONE : TNUM_ZERO);
   }
}

inline void decrementMonth(Period& p, const Time& t, const _bool& addDays)
{
   if (p.months_sec == TNUM_ZERO) {
      p.years_sec--;
      p.years_ad -= isLeapYear(t.year)
         ? TNUM_DAYS_IN_LEAP_YEAR
         : TNUM_DAYS_IN_NORMAL_YEAR;
      p.months_sec = TNUM_MONTHS_IN_YEAR - TNUM_ONE;
   }
   else {
      p.months_sec--;
   }
   const _tnum d = daysInMonth(t.month, t.year);
   p.months_ad -= d;

   if (addDays)
      p.days += d;
}

inline void decrementDay(Period& p, const Time& t)
{
   if (p.days == TNUM_ZERO) {
      decrementMonth(p, t, true);
   }
   p.days--;
}

inline void shortClockTillMidnight(Period& p, const Time& t)
{
   if (t.minute == TNUM_ZERO) {
      if (t.hour == TNUM_ZERO) {
         p.days++;
      }
      else {
         p.hours = TNUM_HOURS_IN_DAY - t.hour;
      }
   }
   else {
      p.minutes = TNUM_MINUTES_IN_HOUR - t.minute;
      p.hours = TNUM_HOURS_IN_DAY - t.hour - TNUM_ONE;
   }
}

inline void clockTillMidnight(Period& p, const Time& t)
{
   if (t.second == TNUM_ZERO && t.minute == TNUM_ZERO && t.hour == TNUM_ZERO) {
      p.days++;
   }
   else if (t.second == TNUM_ZERO) {
      shortClockTillMidnight(p, t);
   }
   else {
      p.seconds = TNUM_SECONDS_IN_MINUTE - t.second;
      p.minutes = TNUM_MINUTES_IN_HOUR - t.minute - TNUM_ONE;
      p.hours = TNUM_HOURS_IN_DAY - t.hour - TNUM_ONE;
   }
}

std::vector<Time> sortedAndUniqueTimeList(const std::vector<Time>& base)
{
   std::vector<Time> result = base;
   std::sort(result.begin(), result.end());
   _size len = result.size();

   for (_size i = 1; i < len; i++) {
      if (result[i - 1] == result[i]) {
         if (result[i - 1].type <= result[i].type) {
            result.erase(result.begin() + i);
         }
         else {
            result.erase(result.begin() + i - 1);
         }
         len--;
         i--;
      }
   }

   return result;
}

}
