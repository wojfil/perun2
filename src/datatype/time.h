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


struct Time
{
public:

   Time();
   Time(const p_tnum mo, const p_tnum ye);
   Time(const p_tnum da, const p_tnum mo, const p_tnum ye);
   Time(const p_tnum da, const p_tnum mo, const p_tnum ye, const p_tnum ho, const p_tnum mi);
   Time(const p_tnum da, const p_tnum mo, const p_tnum ye, const p_tnum ho, const p_tnum mi, const p_tnum sec);
   static Time clock(const p_tnum ho, const p_tnum mi);
   static Time clock(const p_tnum ho, const p_tnum mi, const p_tnum sec);

   p_str toString() const;
   void initClock(const p_bool withSeconds, const p_tnum recentChange);

   void addYears(const p_tnum y);
   void addMonths(const p_tnum m);
   void addWeeks(const p_tnum w);
   void addDays(const p_tnum d);
   void addHours(const p_tnum h);
   void addMinutes(const p_tnum m);
   void addSeconds(const p_tnum s);

   void setYear(const p_tnum y);
   void setMonth(const p_tnum m);
   void setDay(const p_tnum d);
   void setHour(const p_tnum h);
   void setMinute(const p_tnum m);
   void setSecond(const p_tnum s);

   Time toDate() const;
   p_tnum getWeekDay() const;
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


   // time can take one of these forms:
   enum TimeType
   {
      tt_Null = -1,
      tt_YearMonth = 0,         // contains month and year
      tt_Date = 1,              // contains day, month and year
      tt_DateShortClock = 2,    // contains day, month, year, hour, minute
      tt_DateClock = 3,         // contains day, month, year, hour, minute, second
      tt_ShortClock = 4,        // contains hour, minute
      tt_Clock = 5              // contains hour, minute, second
   };


   p_tnum year;
   p_tnum day;
   p_tnum month;
   p_tnum hour = TNUM_ZERO;
   p_tnum minute = TNUM_ZERO;
   p_tnum second = TNUM_ZERO;
   TimeType type;
};

p_tnum toTimeNumber(const Number& num);
p_str monthToString(const p_tnum month);
p_str weekdayToString(const p_tnum wday);

inline void addTimeUnit(p_stream& stream, const p_tnum val);
inline p_bool isLeapYear(const p_tnum year);
p_tnum daysInMonth(const p_tnum month, const p_tnum year);
inline Period timeDifference(const Time& min, const Time& max);
inline p_tnum daysInYears(const p_tnum min, const p_tnum max);
inline void decrementMonth(Period& p, const Time& t, const p_bool addDays);
inline void decrementDay(Period& p, const Time& t);
inline void shortClockTillMidnight(Period& p, const Time& t);
inline void clockTillMidnight(Period& p, const Time& t);
std::vector<Time> sortedAndUniqueTimeList(const std::vector<Time>& base);

}

#endif /* TIME_H */
