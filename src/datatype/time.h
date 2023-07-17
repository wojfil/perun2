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

#ifndef TIME_H
#define TIME_H

#include "period.h"
#include <vector>


namespace perun2
{

p_constexpr p_bool TIME_WITH_SECONDS = true;
p_constexpr p_bool TIME_WITHOUT_SECONDS = false;


struct Time
{
   // time can be defined in 4 forms:
   enum TimeType
   {
      tt_Null = -1,
      tt_YearMonth = 0,     // contains month and year
      tt_Date = 1,          // contains day, month and year
      tt_ShortClock = 2,    // contains day, month, year, hour, minute
      tt_Clock = 3          // contains day, month, year, hour, minute, second
      // these numbers above are important and do not delete them
   };

   Time();
   Time(const _tnum mo, const _tnum ye);
   Time(const _tnum da, const _tnum mo, const _tnum ye);
   Time(const _tnum da, const _tnum mo, const _tnum ye, const _tnum ho, const _tnum mi);
   Time(const _tnum da, const _tnum mo, const _tnum ye, const _tnum ho, const _tnum mi, const _tnum sec);

// todo:
// change these 6 numbers into one value: time_t
   _tnum year;
   _tnum day;
   _tnum month;
   _tnum hour = TNUM_ZERO;
   _tnum minute = TNUM_ZERO;
   _tnum second = TNUM_ZERO;
   TimeType type;

   p_str toString() const;
   void initClock(const p_bool withSeconds, const _tnum recentChange);

   void addYears(const _tnum y);
   void addMonths(const _tnum m);
   void addWeeks(const _tnum w);
   void addDays(const _tnum d);
   void addHours(const _tnum h);
   void addMinutes(const _tnum m);
   void addSeconds(const _tnum s);

   void setYear(const _tnum y);
   void setMonth(const _tnum m);
   void setDay(const _tnum d);
   void setHour(const _tnum h);
   void setMinute(const _tnum m);
   void setSecond(const _tnum s);

   Time toDate() const;
   _tnum getWeekDay() const;
   p_bool equalsExactly(const Time& tim) const;
   void setValue(const Time& tim);
   void clear();

   Time& operator += (const Period& per);
   Time& operator -= (const Period& per);

   p_bool operator == (const Time& tim) const;
   p_bool operator != (const Time& tim) const;
   p_bool operator < (const Time& tim) const;
   p_bool operator > (const Time& tim) const;
   p_bool operator <= (const Time& tim) const;
   p_bool operator >= (const Time& tim) const;

   Period operator - (const Time& tim) const;

};

_tnum toTimeNumber(const Number& num);
p_str monthToString(const _tnum month);
p_str weekdayToString(const _tnum wday);

inline void addTimeUnit(p_stream& stream, const _tnum val);
inline p_bool isLeapYear(const _tnum year);
_tnum daysInMonth(const _tnum month, const _tnum year);
inline Period timeDifference(const Time& min, const Time& max);
inline _tnum daysInYears(const _tnum min, const _tnum max);
inline void decrementMonth(Period& p, const Time& t, const p_bool addDays);
inline void decrementDay(Period& p, const Time& t);
inline void shortClockTillMidnight(Period& p, const Time& t);
inline void clockTillMidnight(Period& p, const Time& t);
std::vector<Time> sortedAndUniqueTimeList(const std::vector<Time>& base);

}

#endif /* TIME_H */
