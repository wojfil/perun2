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

#include "time.h"
#include "datatype.h"
#include <vector>
#include <sstream>
#include <math.h>
#include <algorithm>


namespace perun2
{

Time::Time()
   : day(TNUM_MINUS_ONE), month(TNUM_MINUS_ONE), year(TNUM_MINUS_ONE), 
     hour(TNUM_MINUS_ONE), minute(TNUM_MINUS_ONE), second(TNUM_MINUS_ONE), type(TimeType::tt_Never) { };

Time::Time(const p_tnum mo, const p_tnum ye)
   : day(TNUM_ONE), month(mo), year(ye), type(TimeType::tt_YearMonth) { };

Time::Time(const p_tnum da, const p_tnum mo, const p_tnum ye)
   : day(da), month(mo), year(ye), type(TimeType::tt_Date) { };

Time::Time(const p_tnum da, const p_tnum mo, const p_tnum ye, const p_tnum ho, const p_tnum mi)
   : day(da), month(mo), year(ye), hour(ho), minute(mi), type(TimeType::tt_DateShortClock) { };

Time::Time(const p_tnum da, const p_tnum mo, const p_tnum ye, const p_tnum ho, const p_tnum mi, const p_tnum sec)
   : day(da), month(mo), year(ye), hour(ho), minute(mi), second(sec), type(TimeType::tt_DateClock) { };

Time Time::clock(const p_tnum ho, const p_tnum mi)
{
   Time t;
   t.type = tt_ShortClock;
   t.hour = ho;
   t.minute = mi;
   return t;
}

Time Time::clock(const p_tnum ho, const p_tnum mi, const p_tnum sec)
{
   Time t;
   t.type = tt_Clock;
   t.hour = ho;
   t.minute = mi;
   t.second = sec;
   return t;
}

p_str Time::toString() const
{
   switch (type) {
      case TimeType::tt_Never: {
         return STRING_NEVER;
      }
      case TimeType::tt_ShortClock: {
         p_stream ss;

         addTimeUnit(ss, hour);
         ss << CHAR_COLON;
         addTimeUnit(ss, minute);

         return ss.str();
      }
      case TimeType::tt_Clock: {
         p_stream ss;
         
         addTimeUnit(ss, hour);
         ss << CHAR_COLON;
         addTimeUnit(ss, minute);
         ss << CHAR_COLON;
         addTimeUnit(ss, second);

         return ss.str();
      }
   }

   p_stream ss;

   if (type != tt_YearMonth) {
      ss << day;
      ss << CHAR_SPACE;
   }

   ss << monthToString(month);
   ss << CHAR_SPACE;
   ss << year;

   if (type == tt_DateShortClock || type == tt_DateClock) {
      ss << CHAR_COMMA;
      ss << CHAR_SPACE;
      addTimeUnit(ss, hour);
      ss << CHAR_COLON;
      addTimeUnit(ss, minute);
      if (type == tt_DateClock) {
         ss << CHAR_COLON;
         addTimeUnit(ss, second);
      }
   }

   return ss.str();
}

void Time::addYears(const p_tnum y)
{
   switch (type) {
      case TimeType::tt_Never:
      case TimeType::tt_ShortClock:
      case TimeType::tt_Clock: {
         return;
      }
   }

   year += y;
   if (type != tt_YearMonth && month == TNUM_FEBRUARY
       && day == TNUM_DAYS_IN_LEAP_FEBRUARY && !isLeapYear(year))
   {
      day = TNUM_DAYS_IN_FEBRUARY;
   }
}

void Time::addMonths(const p_tnum m)
{
   switch (type) {
      case TimeType::tt_Never:
      case TimeType::tt_ShortClock:
      case TimeType::tt_Clock: {
         return;
      }
   }

   const p_tnum m2 = m % TNUM_MONTHS_IN_YEAR;
   const p_tnum y = m / TNUM_MONTHS_IN_YEAR;
   month += m2;
   year += y;

   if (month > TNUM_MONTHS_IN_YEAR) {
      month -= TNUM_MONTHS_IN_YEAR;
      year++;
   }

   if (type != tt_YearMonth && day >= TNUM_DAYS_IN_LEAP_FEBRUARY) {
      const p_tnum max = daysInMonth(month, year);
      if (day > max) {
         day = max;
      }
   }
}

void Time::addWeeks(const p_tnum w)
{
   addDays(w * TNUM_DAYS_IN_WEEK);
}

void Time::addDays(const p_tnum d)
{
   switch (type) {
      case TimeType::tt_Never:
      case TimeType::tt_ShortClock:
      case TimeType::tt_Clock: {
         return;
      }
   }

   if (type == tt_YearMonth) {
      day = d < TNUM_ZERO ? TNUM_ONE : daysInMonth(month, year);
      type = tt_Date;
   }

   if (d > TNUM_ZERO) {
      p_tnum d2 = d;

      while (d2 != TNUM_ZERO) {
         const p_tnum dif = daysInMonth(month, year) - day;
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
      p_tnum d2 = -d;

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

void Time::addHours(const p_tnum h)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   initClock(false, h);

   const p_tnum h2 = h % TNUM_HOURS_IN_DAY;
   p_tnum d = h / TNUM_HOURS_IN_DAY;
   hour += h2;
   if (hour >= TNUM_HOURS_IN_DAY) {
      hour -= TNUM_HOURS_IN_DAY;
      d++;
   }
   else if (hour < TNUM_ZERO) {
      hour += TNUM_HOURS_IN_DAY;
      d--;
   }

   if (d != TNUM_ZERO && type != TimeType::tt_ShortClock && type != TimeType::tt_Clock) {
      addDays(d);
   }
}

void Time::addMinutes(const p_tnum m)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   initClock(false, m);

   const p_tnum m2 = m % TNUM_MINUTES_IN_HOUR;
   p_tnum h = m / TNUM_MINUTES_IN_HOUR;
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

void Time::addSeconds(const p_tnum s)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   initClock(true, s);

   const p_tnum s2 = s % TNUM_SECONDS_IN_MINUTE;
   p_tnum m = s / TNUM_SECONDS_IN_MINUTE;
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

void Time::setYear(const p_tnum y)
{
   switch (type) {
      case TimeType::tt_Never:
      case TimeType::tt_ShortClock:
      case TimeType::tt_Clock: {
         return;
      }
   }

   year = y;
}

void Time::setMonth(const p_tnum m)
{
   switch (type) {
      case TimeType::tt_Never:
      case TimeType::tt_ShortClock:
      case TimeType::tt_Clock: {
         return;
      }
   }

   if (m < TNUM_JANUARY || m > TNUM_DECEMBER) {
      type = TimeType::tt_Never;
      return;
   }

   month = m;
}

void Time::setDay(const p_tnum d)
{
   switch (type) {
      case TimeType::tt_Never:
      case TimeType::tt_ShortClock:
      case TimeType::tt_Clock: {
         return;
      }
   }

   if (d < TNUM_ONE) {
      type = TimeType::tt_Never;
      return;
   }

   const p_tnum inMonth = daysInMonth(month, year);

   if (d > inMonth) {
      type = TimeType::tt_Never;
      return;
   }

   day = d;

   if (type < tt_Date) {
      type = tt_Date;
   }
}

void Time::setHour(const p_tnum h)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   if (h < TNUM_ZERO || h >= TNUM_HOURS_IN_DAY) {
      type = TimeType::tt_Never;
      return;
   }

   hour = h;

   if (type < tt_DateShortClock) {
      type = tt_DateShortClock;
   }
}

void Time::setMinute(const p_tnum m)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   if (m < TNUM_ZERO || m >= TNUM_MINUTES_IN_HOUR) {
      type = TimeType::tt_Never;
      return;
   }

   minute = m;

   if (type < tt_DateShortClock) {
      type = tt_DateShortClock;
   }
}

void Time::setSecond(const p_tnum s)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   if (s < TNUM_ZERO || s >= TNUM_SECONDS_IN_MINUTE) {
      type = TimeType::tt_Never;
      return;
   }
   
   second = s;

   if (type == tt_ShortClock) {
      type = tt_Clock;
   }
   else if (type != tt_DateClock) {
      type = tt_DateClock;
   }
}

Time Time::toDate() const
{
   switch (type) {
      case tt_YearMonth: {
         return Time(month, year);
      }
      case tt_Never:
      case tt_ShortClock:
      case tt_Clock: {
         return Time();
      }
      default: {
         return Time(day, month, year);
      }
   }
}

p_tnum Time::getWeekDay() const
{
   switch (type) {
      case tt_YearMonth:
      case tt_Never:
      case tt_ShortClock:
      case tt_Clock: {
         return TNUM_MINUS_ONE;
      }
   }

   const p_tnum y = year - (month < TNUM_THREE);
   const p_tnum wd = (y + y / TNUM_FOUR - y / TNUM_100 + y / TNUM_400
      + TNUM_WEEKDAY_DATA[month - TNUM_ONE] + day) % TNUM_DAYS_IN_WEEK;
   return wd == TNUM_ZERO ? TNUM_DAYS_IN_WEEK : wd;
}

p_bool Time::equalsExactly(const Time& tim) const
{
   return type == tim.type && *this == tim;
}

void Time::setValue(const Time& tim)
{
   if (type == TimeType::tt_Never) {
      return;
   }

   if (tim.type == TimeType::tt_Never) {
      clear();
      return;
   }

   year = tim.year;
   month = tim.month;

   if (tim.type == TimeType::tt_YearMonth) {
      const p_tnum d = daysInMonth(month, year);
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

   if (tim.type == TimeType::tt_DateShortClock) {
      return;
   }

   second = tim.second;
}

void Time::clear()
{
   type = TimeType::tt_Never;
   year = TNUM_MINUS_ONE;
   day = TNUM_MINUS_ONE;
   month = TNUM_MINUS_ONE;
   hour = TNUM_MINUS_ONE;
   minute = TNUM_MINUS_ONE;
   second = TNUM_MINUS_ONE;
}

Time& Time::operator += (const Period& per)
{
   if (type == TimeType::tt_Never) {
      return *this;
   }

   if (per.seconds != TNUM_ZERO)
      addSeconds(per.seconds);

   if (per.minutes != TNUM_ZERO)
      addMinutes(per.minutes);

   if (per.hours != TNUM_ZERO)
      addHours(per.hours);

   const p_tnum d = per.days + per.years_ad + per.months_ad;

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
   if (type == TimeType::tt_Never) {
      return *this;
   }

   if (per.seconds != TNUM_ZERO)
      addSeconds(-per.seconds);

   if (per.minutes != TNUM_ZERO)
      addMinutes(-per.minutes);

   if (per.hours != TNUM_ZERO)
      addHours(-per.hours);

   const p_tnum d = per.days + per.years_ad + per.months_ad;

   if (d != TNUM_ZERO)
      addDays(-d);

   if (per.weeks != TNUM_ZERO)
      addDays(-per.weeks * TNUM_DAYS_IN_WEEK);

   if (per.months != TNUM_ZERO)
      addMonths(-per.months);

   if (per.years != TNUM_ZERO)
      addYears(-per.years);

   return *this;
}

// add clock to an existing time
void Time::initClock(const p_bool withSeconds, const p_tnum recentChange)
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

      type = withSeconds ? tt_DateClock : tt_DateShortClock;
   }
   else if (withSeconds && type == tt_ShortClock) {
      type = tt_Clock;
      second = TNUM_ZERO;
   }
}

p_bool Time::operator == (const Time& tim) const
{
   if (type == TimeType::tt_Never || tim.type == TimeType::tt_Never) {
      return false;
   }

   switch (type) {
      case TimeType::tt_ShortClock: {
         switch (tim.type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock:
            case tt_DateClock:
            case tt_Clock: {
               return hour == tim.hour
                   && minute == tim.minute;
            }
         }

         break;
      }
      case TimeType::tt_Clock: {
         switch (tim.type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock: {
               return hour == tim.hour
                   && minute == tim.minute;
            }
            case tt_DateClock:
            case tt_Clock: {
               return hour == tim.hour
                   && minute == tim.minute
                   && second == tim.second;
            }
         }

         break;
      }
   }

   switch (tim.type) {
      case TimeType::tt_ShortClock: {
         switch (type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock:
            case tt_DateClock:
            case tt_Clock: {
               return hour == tim.hour
                   && minute == tim.minute;
            }
         }

         break;
      }
      case TimeType::tt_Clock: {
         switch (type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock: {
               return hour == tim.hour
                   && minute == tim.minute;
            }
            case tt_DateClock:
            case tt_Clock: {
               return hour == tim.hour
                   && minute == tim.minute
                   && second == tim.second;
            }
         }

         break;
      }
   }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) {
      return month == tim.month
          && year == tim.year;
   }

   if (type == tt_Date || tim.type == tt_Date) {
      return day == tim.day
          && month == tim.month
          && year == tim.year;
   }

   if (type == tt_DateShortClock || tim.type == tt_DateShortClock) {
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

p_bool Time::operator != (const Time& tim) const
{
   if (type == TimeType::tt_Never || tim.type == TimeType::tt_Never) {
      return false;
   }

   switch (type) {
      case tt_ShortClock: {
         switch (tim.type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock:
            case tt_DateClock:
            case tt_Clock: {
               return hour != tim.hour
                   || minute != tim.minute;
            }
         }

         break;
      }
      case tt_Clock: {
         switch (tim.type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock: {
               return hour != tim.hour
                   || minute != tim.minute;
            }
            case tt_DateClock:
            case tt_Clock: {
               return hour != tim.hour
                   || minute != tim.minute
                   || second != tim.second;
            }
         }

         break;
      }
   }

   switch (tim.type) {
      case tt_ShortClock: {
         switch (type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock:
            case tt_DateClock:
            case tt_Clock: {
               return hour != tim.hour
                   || minute != tim.minute;
            }
         }

         break;
      }
      case tt_Clock: {
         switch (type) {
            case tt_YearMonth:
            case tt_Date: {
               return false;
            }
            case tt_DateShortClock:
            case tt_ShortClock: {
               return hour != tim.hour
                   || minute != tim.minute;
            }
            case tt_DateClock:
            case tt_Clock: {
               return hour != tim.hour
                   || minute != tim.minute
                   || second != tim.second;
            }
         }

         break;
      }
   }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) {
      return month != tim.month
          || year != tim.year;
   }

   if (type == tt_Date || tim.type == tt_Date) {
      return day != tim.day
          || month != tim.month
          || year != tim.year;
   }

   if (type == tt_DateShortClock || tim.type == tt_DateShortClock) {
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

p_bool Time::operator < (const Time& tim) const
{
   P_CHECK_TIME_COMPARISON_FAILURE;

   if (type >= tt_ShortClock || tim.type >= tt_ShortClock) {
      goto dateEscape;
   }

   if (year < tim.year) { return true; }
   else if (year > tim.year) { return false; }

   if (month < tim.month) { return true; }
   else if (month > tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return false; }

   if (day < tim.day) { return true; }
   else if (day > tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return false; }

dateEscape:

   if (hour < tim.hour) { return true; }
   else if (hour > tim.hour) { return false; }

   if (minute < tim.minute) { return true; }
   else if (minute > tim.minute) { return false; }

   if (type == tt_DateShortClock || type == tt_ShortClock
      || tim.type == tt_DateShortClock || tim.type == tt_ShortClock) { return false; }

   return second < tim.second;
}

p_bool Time::operator > (const Time& tim) const
{
   P_CHECK_TIME_COMPARISON_FAILURE;

   if (type >= tt_ShortClock || tim.type >= tt_ShortClock) {
      goto dateEscape;
   }

   if (year > tim.year) { return true; }
   else if (year < tim.year) { return false; }

   if (month > tim.month) { return true; }
   else if (month < tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return false; }

   if (day > tim.day) { return true; }
   else if (day < tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return false; }

dateEscape:

   if (hour > tim.hour) { return true; }
   else if (hour < tim.hour) { return false; }

   if (minute > tim.minute) { return true; }
   else if (minute < tim.minute) { return false; }

   if (type == tt_DateShortClock || type == tt_ShortClock
      || tim.type == tt_DateShortClock || tim.type == tt_ShortClock) { return false; }

   return second > tim.second;
}

p_bool Time::operator <= (const Time& tim) const
{
   P_CHECK_TIME_COMPARISON_FAILURE;

   if (type >= tt_ShortClock || tim.type >= tt_ShortClock) {
      goto dateEscape;
   }

   if (year < tim.year) { return true; }
   else if (year > tim.year) { return false; }

   if (month < tim.month) { return true; }
   else if (month > tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return true; }

   if (day < tim.day) { return true; }
   else if (day > tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return true; }

dateEscape:

   if (hour < tim.hour) { return true; }
   else if (hour > tim.hour) { return false; }

   if (minute < tim.minute) { return true; }
   else if (minute > tim.minute) { return false; }

   if (type == tt_DateShortClock || type == tt_ShortClock
      || tim.type == tt_DateShortClock || tim.type == tt_ShortClock) { return true; }

   return second <= tim.second;
}

p_bool Time::operator >= (const Time& tim) const
{
   P_CHECK_TIME_COMPARISON_FAILURE;

   if (type >= tt_ShortClock || tim.type >= tt_ShortClock) {
      goto dateEscape;
   }

   if (year > tim.year) { return true; }
   else if (year < tim.year) { return false; }

   if (month > tim.month) { return true; }
   else if (month < tim.month) { return false; }

   if (type == tt_YearMonth || tim.type == tt_YearMonth) { return true; }

   if (day > tim.day) { return true; }
   else if (day < tim.day) { return false; }

   if (type == tt_Date || tim.type == tt_Date) { return true; }

dateEscape:

   if (hour > tim.hour) { return true; }
   else if (hour < tim.hour) { return false; }

   if (minute > tim.minute) { return true; }
   else if (minute < tim.minute) { return false; }

   if (type == tt_DateShortClock || type == tt_ShortClock
      || tim.type == tt_DateShortClock || tim.type == tt_ShortClock) { return true; }

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

p_tnum toTimeNumber(const Number& num)
{
   if (num.state == NumberState::Int) {
      return static_cast<p_tnum>(num.value.i);
   }

   if (num.state == NumberState::Double) {
      return static_cast<p_tnum>(round(num.value.d));
   }

   return TNUM_ZERO;
}

p_str monthToString(const p_tnum month)
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
         return p_str();
   }
}

p_str weekdayToString(const p_tnum wday)
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
         return p_str();
   }
}

inline void addTimeUnit(p_stream& stream, const p_tnum val)
{
   if (val <= TNUM_NINE) {
      stream << CHAR_0;
   }
   stream << val;
}

inline p_bool isLeapYear(const p_tnum year)
{
   if (year % 4 == TNUM_ZERO) {
      if (year % TNUM_100 == TNUM_ZERO) {
         return year % TNUM_400 == TNUM_ZERO;
      }
      return true;
   }
   return false;
}

p_tnum daysInMonth(const p_tnum month, const p_tnum year)
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
   if (min.type == Time::TimeType::tt_Never || max.type == Time::TimeType::tt_Never) {
      return Period();
   }

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
         if (max.type == Time::tt_DateShortClock) {
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
         if (max.type == Time::tt_DateShortClock) {
            return p;
         }

         p.seconds = max.second;
         return p;
      }
      case Time::tt_DateShortClock: {
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

         if (max.type == Time::tt_DateShortClock) {
            return p;
         }

         p.seconds = max.second;
         return p;
      }
      case Time::tt_DateClock: {
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
inline p_tnum daysInYears(const p_tnum min, const p_tnum max)
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

inline void decrementMonth(Period& p, const Time& t, const p_bool addDays)
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
   const p_tnum d = daysInMonth(t.month, t.year);
   p.months_ad -= d;

   if (addDays) {
      p.days += d;
   }
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
   std::vector<Time> result;

   for (const Time& t : base) {
      if (t.type != Time::tt_Never) {
         result.emplace_back(t);
      }
   }

   if (result.size() <= 1) {
      return result;
   }

   std::sort(result.begin(), result.end());
   p_size len = result.size();

   for (p_size i = 1; i < len; i++) {
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
