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


Time::Time() : day(1), month(1), year(TNUM_FIRST_YEAR), hour(0), minute(0), second(0),
   type(TimeType::tt_Clock) { };

Time::Time(_tnum mo, _tnum ye) : day(0), month(mo), year(ye), hour(0), minute(0), second(0),
   type(TimeType::tt_YearMonth) { };

Time::Time(_tnum da, _tnum mo, _tnum ye) : day(da), month(mo), year(ye), hour(0), minute(0), second(0),
   type(TimeType::tt_Date) { };

Time::Time(_tnum da, _tnum mo, _tnum ye, _tnum ho, _tnum mi) :
   day(da), month(mo), year(ye), hour(ho), minute(mi), second(0),
   type(TimeType::tt_ShortClock) { };

Time::Time(_tnum da, _tnum mo, _tnum ye, _tnum ho, _tnum mi, _tnum sec) :
   day(da), month(mo), year(ye), hour(ho), minute(mi), second(sec),
   type(TimeType::tt_Clock) { };


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
      day = d < 0 ? 1 : daysInMonth(month, year);
      type = tt_Date;
   }

   if (d > 0) {
      _tnum d2 = d;

      while (d2 != 0) {
         const _tnum dif = daysInMonth(month, year) - day;
         if (d2 <= dif) {
            day += d2;
            d2 = 0;
         }
         else {
            d2 -= dif + 1;
            month++;
            if (month == 13) {
               month = TNUM_JANUARY;
               year++;
            }
            day = 1;
         }
      }
   }
   else if (d < 0) {
      _tnum d2 = -d;

      while (d2 != 0) {
         if (d2 < day) {
            day -= d2;
            d2 = 0;
         }
         else {
            d2 -= day;
            month--;
            if (month == 0) {
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
   else if (hour < 0) {
      hour += TNUM_HOURS_IN_DAY;
      d--;
   }

   if (d != 0) {
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
   else if (minute < 0) {
      minute += TNUM_MINUTES_IN_HOUR;
      h--;
   }

   if (h != 0) {
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
   else if (second < 0) {
      second += TNUM_SECONDS_IN_MINUTE;
      m--;
   }

   if (m != 0) {
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
   if (h < 0) {
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
   if (m < 0) {
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
   if (s < 0) {
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
   static const _tnum t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
   const _tnum y = year - (month < 3);
   const _tnum wd = (y + y/4 - y/100 + y/400 + t[month - 1] + day) % 7;
   return wd == 0 ? 7 : wd;
}

_bool Time::equalsExactly(const Time& tim) const
{
   return type == tim.type && *this == tim;
}

_bool Time::isEmpty() const
{
   return year == TNUM_FIRST_YEAR
       && type == TimeType::tt_Clock
       && month == 1
       && day == 1
       && hour == 0
       && minute == 0
       && second == 0;
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

   if (per.seconds != 0)
      addSeconds(per.seconds);

   if (per.minutes != 0)
      addMinutes(per.minutes);

   if (per.hours != 0)
      addHours(per.hours);

   if (d != 0)
      addDays(d);

   if (per.weeks != 0)
      addDays(per.weeks * TNUM_DAYS_IN_WEEK);

   if (per.months != 0)
      addMonths(per.months);

   if (per.years != 0)
      addYears(per.years);

   return *this;
}

// the same, but reversed
Time& Time::operator -= (const Period& per)
{
   const _tnum d = -per.days - per.years_ad - per.months_ad;

   if (per.seconds != 0)
      addSeconds(-per.seconds);

   if (per.minutes != 0)
      addMinutes(-per.minutes);

   if (per.hours != 0)
      addHours(-per.hours);

   if (d != 0)
      addDays(d);

   if (per.weeks != 0)
      addDays(-per.weeks * 7);

   if (per.months != 0)
      addMonths(-per.months);

   if (per.years != 0)
      addYears(-per.years);

   return *this;
}

// add clock to an already existing time
void Time::initClock(const _bool& withSeconds, const _tnum& recentChange)
{
   if (type == tt_YearMonth || type == tt_Date) {
      hour = 0;
      minute = 0;

      if (withSeconds) {
         second = 0;
      }

      if (type == tt_YearMonth) {
         day = 1;
         if (recentChange > 0) {
            month++;
            if (month == 13) {
               month = 1;
               year++;
            }
         }
      }
      else if (recentChange > 0) {
         day++;
         if (day > daysInMonth(month, year)) {
            day = 1;
            month++;
            if (month == 13) {
               month = 1;
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
         return L"January";
      case TNUM_FEBRUARY:
         return L"February";
      case TNUM_MARCH:
         return L"March";
      case TNUM_APRIL:
         return L"April";
      case TNUM_MAY:
         return L"May";
      case TNUM_JUNE:
         return L"June";
      case TNUM_JULY:
         return L"July";
      case TNUM_AUGUST:
         return L"August";
      case TNUM_SEPTEMBER:
         return L"September";
      case TNUM_OCTOBER:
         return L"October";
      case TNUM_NOVEMBER:
         return L"November";
      case TNUM_DECEMBER:
         return L"December";
      default:
         return EMPTY_STRING;
   }
}

_str weekdayToString(const _tnum& wday)
{
   switch (wday) {
      case TNUM_MONDAY:
         return L"Monday";
      case TNUM_TUESDAY:
         return L"Tuesday";
      case TNUM_WEDNESDAY:
         return L"Wednesday";
      case TNUM_THURSDAY:
         return L"Thursday";
      case TNUM_FRIDAY:
         return L"Friday";
      case TNUM_SATURDAY:
         return L"Saturday";
      case TNUM_SUNDAY:
         return L"Sunday";
      default:
         return EMPTY_STRING;
   }
}

inline _str fillTimeUnit(const _tnum& val)
{
   return val <= 9
      ? str(STRING_0, toStr(val))
      : toStr(val);
}

inline _bool isLeapYear(const _tnum& year)
{
   if (year % 4 == 0) {
      if (year % 100 == 0) {
         return year % 400 == 0;
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

// commulative days in months
const _tnum MDAYS_NORMAL[12] =
   { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
const _tnum MDAYS_LEAP[12] =
   { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };


inline Period timeDifference(const Time& min, const Time& max)
{
   Period p;
   p.periodType = Period::pt_Difference;
   p.years_sec = max.year - min.year;
   p.months_sec = max.month - min.month;

   if (p.months_sec < 0) {
      p.months_sec += TNUM_MONTHS_IN_YEAR;
      p.years_sec--;

      p.months_ad = (isLeapYear(max.year)
            ? MDAYS_LEAP[max.month - 1]
            : MDAYS_NORMAL[max.month - 1])
         + (isLeapYear(min.year)
            ? (TNUM_DAYS_IN_LEAP_YEAR - MDAYS_LEAP[min.month - 1])
            : (TNUM_DAYS_IN_NORMAL_YEAR - MDAYS_NORMAL[min.month - 1]));

      if (p.years_sec != 0) {
         p.years_ad = daysInYears(min.year + 1, max.year - 1);
      }
   }
   else {
      p.months_ad = isLeapYear(min.year)
         ? MDAYS_LEAP[max.month - 1] - MDAYS_LEAP[min.month - 1]
         : MDAYS_NORMAL[max.month - 1] - MDAYS_NORMAL[min.month - 1];

      if (p.years_sec != 0) {
         p.years_ad = (min.month <= TNUM_FEBRUARY)
            ? daysInYears(min.year, max.year - 1)
            : daysInYears(min.year + 1, max.year);
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
         if (p.days < 0) {
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
         if (p.days < 0) {
            decrementMonth(p, min, true);
         }

         if (max.type == Time::tt_Date) {
            decrementDay(p, min);
            shortClockTillMidnight(p, min);
            return p;
         }

         p.hours = max.hour - min.hour;
         p.minutes = max.minute - min.minute;
         if (p.minutes < 0) {
            p.minutes += TNUM_MINUTES_IN_HOUR;
            p.hours--;
         }
         if (p.hours < 0) {
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
         if (p.days < 0) {
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

         if (p.seconds < 0) {
            p.seconds += TNUM_SECONDS_IN_MINUTE;
            p.minutes--;
         }
         if (p.minutes < 0) {
            p.minutes += TNUM_MINUTES_IN_HOUR;
            p.hours--;
         }
         if (p.hours < 0) {
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
      return TNUM_DAYS_IN_NORMAL_YEAR * (max - min + 1)
         + (max / 4) - (min / 4)
         - (max / 100) + (min / 100)
         + (max / 400) - (min / 400)
         + (isLeapYear(min) ? 1 : 0);
   }
}

inline void decrementMonth(Period& p, const Time& t, const _bool& addDays)
{
   if (p.months_sec == 0) {
      p.years_sec--;
      p.years_ad -= isLeapYear(t.year)
         ? TNUM_DAYS_IN_LEAP_YEAR
         : TNUM_DAYS_IN_NORMAL_YEAR;
      p.months_sec = TNUM_MONTHS_IN_YEAR - 1;
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
   if (p.days == 0) {
      decrementMonth(p, t, true);
   }
   p.days--;
}

inline void shortClockTillMidnight(Period& p, const Time& t)
{
   if (t.minute == 0) {
      if (t.hour == 0) {
         p.days++;
      }
      else {
         p.hours = TNUM_HOURS_IN_DAY - t.hour;
      }
   }
   else {
      p.minutes = TNUM_MINUTES_IN_HOUR - t.minute;
      p.hours = TNUM_HOURS_IN_DAY - t.hour - 1;
   }
}

inline void clockTillMidnight(Period& p, const Time& t)
{
   if (t.second == 0 && t.minute == 0 && t.hour == 0) {
      p.days++;
   }
   else if (t.second == 0) {
      shortClockTillMidnight(p, t);
   }
   else {
      p.seconds = TNUM_SECONDS_IN_MINUTE - t.second;
      p.minutes = TNUM_MINUTES_IN_HOUR - t.minute - 1;
      p.hours = TNUM_HOURS_IN_DAY - t.hour - 1;
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
