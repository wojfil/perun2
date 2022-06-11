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

#include "period.h"
#include <sstream>


Period::Period()
{
   init();
   periodUnit = PeriodUnit::u_Years;
   periodType = PeriodType::pt_Unit;
}

Period::Period(const _tnum& val, const PeriodUnit& unit)
{
   init();
   periodUnit = unit;
   periodType = PeriodType::pt_Unit;

   switch (periodUnit) {
      case PeriodUnit::u_Years: {
         years = val;
         break;
      }
      case PeriodUnit::u_Months: {
         months = val;
         break;
      }
      case PeriodUnit::u_Weeks: {
         weeks = val;
         break;
      }
      case PeriodUnit::u_Days: {
         days = val;
         break;
      }
      case PeriodUnit::u_Hours: {
         hours = val;
         break;
      }
      case PeriodUnit::u_Minutes: {
         minutes = val;
         break;
      }
      case PeriodUnit::u_Seconds: {
         seconds = val;
         break;
      }
   }
}

void Period::init()
{
   years = 0;
   months = 0;
   weeks = 0;
   days = 0;
   hours = 0;
   minutes = 0;
   seconds = 0;
   years_sec = 0;
   months_sec = 0;
   years_ad = 0;
   months_ad = 0;
}

_nint Period::toSeconds() const
{
   _nint secs = (_nint)seconds;
   const _tnum d = days + years_ad + months_ad;

   if (years != 0) { // year is 365 days
      secs += 31536000LL * (_nint)years;
   }
   if (months != 0) { // month is 30 days
      secs += 2592000LL * (_nint)months;
   }
   if (weeks != 0) { // week is 7 days
      secs += 604800LL * (_nint)weeks;
   }
   if (d != 0) {
      secs += 86400LL * (_nint)d;
   }
   if (hours != 0) {
      secs += 3600LL * (_nint)hours;
   }
   if (minutes != 0) {
      secs += 60LL * (_nint)minutes;
   }

   return secs;
}

_str Period::toString() const
{
   std::wstringstream ss;
   _boo first = true;

   const _tnum y = years + years_sec;
   const _tnum m = months + months_sec;

   if (y != 0 || years != 0) {
      if (!first) ss << L' ';
      ss << y;
      if (y == 1 || y == -1)
         ss << L" year";
      else
         ss << L" years";
      if (first) first = false;
   }

   if (m != 0 || months != 0) {
      if (!first) ss << L' ';
      ss << m;
      if (m == 1 || m == -1)
         ss << L" month";
      else
         ss << L" months";
      if (first) first = false;
   }

   if (weeks != 0) {
      if (!first) ss << L' ';
      ss << weeks;
      if (weeks == 1 || weeks == -1)
         ss << L" week";
      else
         ss << L" weeks";
      if (first) first = false;
   }

   if (days != 0) {
      if (!first) ss << L' ';
      ss << days;
      if (days == 1 || days == -1)
         ss << L" day";
      else
         ss << L" days";
      if (first) first = false;
   }

   if (hours != 0) {
      if (!first) ss << L' ';
      ss << hours;
      if (hours == 1 || hours == -1)
         ss << L" hour";
      else
         ss << L" hours";
      if (first) first = false;
   }

   if (minutes != 0) {
      if (!first) ss << L' ';
      ss << minutes;
      if (minutes == 1 || minutes == -1)
         ss << L" minute";
      else
         ss << L" minutes";
      if (first) first = false;
   }

   if (seconds != 0) {
      if (!first) ss << L' ';
      ss << seconds;
      if (seconds == 1 || seconds == -1)
         ss << L" second";
      else
         ss << L" seconds";
      if (first) first = false;
   }

   return first
      ? L"no period"
      : ss.str();
}

inline _boo Period::isNegativeDifference() const
{
   return years < 0
       || months < 0
       || months_ad < 0
       || years_ad < 0
       || days < 0
       || hours < 0
       || minutes < 0
       || seconds < 0;
}

void Period::reverse()
{
   if (periodType == PeriodType::pt_Unit) {
      switch (periodUnit) {
         case PeriodUnit::u_Years: {
            years *= -1;
            break;
         }
         case PeriodUnit::u_Months: {
            months *= -1;
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks *= -1;
            break;
         }
         case PeriodUnit::u_Days: {
            days *= -1;
            break;
         }
         case PeriodUnit::u_Hours: {
            hours *= -1;
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes *= -1;
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds *= -1;
            break;
         }
      }
   }
   else {
      years *= -1;
      months *= -1;
      weeks *= -1;
      days *= -1;
      hours *= -1;
      minutes *= -1;
      seconds *= -1;
      years_sec *= -1;
      months_sec *= -1;
      years_ad *= -1;
      months_ad *= -1;
   }
}

inline void Period::checkDifference()
{
   if (isNegativeDifference()) {
      if (years > 0 || months > 0 || days > 0 || hours > 0
       || minutes > 0 || seconds > 0 || months <= -12 || days <= -28
       || hours <= -24 || minutes <= -60 || seconds <= -60) {
         periodType = PeriodType::pt_Mingled;
      }
   }
   else {
      if (years < 0 || months < 0 || days < 0 || hours < 0
       || minutes < 0 || seconds < 0 || months >= 12 || days >= 28
       || hours >= 24 || minutes >= 60 || seconds >= 60) {
         periodType = PeriodType::pt_Mingled;
      }
   }
}

Period& Period::operator += (const Period& per)
{
   if (per.periodType == PeriodType::pt_Unit) {
      addUnit(per);
   }
   else {
      years += per.years;
      months += per.months;
      weeks += per.weeks;
      days += per.days;
      hours += per.hours;
      minutes += per.minutes;
      seconds += per.seconds;
      years_sec += per.years_sec;
      months_sec += per.months_sec;
      years_ad += per.years_ad;
      months_ad += per.months_ad;

      if (periodType == PeriodType::pt_Difference) {
         checkDifference();
      }
   }

   return *this;
}

Period& Period::operator -= (const Period& per)
{
   if (per.periodType == PeriodType::pt_Unit) {
      subtractUnit(per);
   }
   else {
      years -= per.years;
      months -= per.months;
      weeks -= per.weeks;
      days -= per.days;
      hours -= per.hours;
      minutes -= per.minutes;
      seconds -= per.seconds;
      years_sec -= per.years_sec;
      months_sec -= per.months_sec;
      years_ad -= per.years_ad;
      months_ad -= per.months_ad;

      if (periodType == PeriodType::pt_Difference) {
         checkDifference();
      }
   }

   return *this;
}

inline void Period::addUnit(const Period& per)
{
   if (periodType == PeriodType::pt_Unit) {
      switch (per.periodUnit) {
         case PeriodUnit::u_Years: {
            years += per.years;
            if (years != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Months: {
            months += per.months;
            if (months != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks += per.weeks;
            if (weeks != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Days: {
            days += per.days;
            if (days != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Hours: {
            hours += per.hours;
            if (hours != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes += per.minutes;
            if (minutes != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds += per.seconds;
            if (seconds != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
      }
   }
   else {
      switch (per.periodUnit) {
         case PeriodUnit::u_Years: {
            years += per.years;
            break;
         }
         case PeriodUnit::u_Months: {
            months += per.months;
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks += per.weeks;
            break;
         }
         case PeriodUnit::u_Days: {
            days += per.days;
            break;
         }
         case PeriodUnit::u_Hours: {
            hours += per.hours;
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes += per.minutes;
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds += per.seconds;
            break;
         }
      }

      if (periodType == PeriodType::pt_Difference) {
         checkDifference();
      }
   }
}

inline void Period::subtractUnit(const Period& per)
{
   if (periodType == PeriodType::pt_Unit) {
      switch (per.periodUnit) {
         case PeriodUnit::u_Years: {
            years -= per.years;
            if (years != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Months: {
            months -= per.months;
            if (months != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks -= per.weeks;
            if (weeks != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Days: {
            days -= per.days;
            if (days != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Hours: {
            hours -= per.hours;
            if (hours != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes -= per.minutes;
            if (minutes != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds -= per.seconds;
            if (seconds != 0 && periodUnit != per.periodUnit) {
               periodType = PeriodType::pt_Mingled;
            }
            break;
         }
      }
   }
   else {
      switch (per.periodUnit) {
         case PeriodUnit::u_Years: {
            years -= per.years;
            break;
         }
         case PeriodUnit::u_Months: {
            months -= per.months;
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks -= per.weeks;
            break;
         }
         case PeriodUnit::u_Days: {
            days -= per.days;
            break;
         }
         case PeriodUnit::u_Hours: {
            hours -= per.hours;
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes -= per.minutes;
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds -= per.seconds;
            break;
         }
      }

      if (periodType == PeriodType::pt_Difference) {
         checkDifference();
      }
   }
}

Period& Period::operator *= (const Number& num)
{
   const _tnum n = (_tnum)num.toInt();

   if (periodType ==  PeriodType::pt_Unit) {
      switch (periodUnit) {
         case PeriodUnit::u_Years: {
            years *= n;
            break;
         }
         case PeriodUnit::u_Months: {
            months *= n;
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks *= n;
            break;
         }
         case PeriodUnit::u_Days: {
            days *= n;
            break;
         }
         case PeriodUnit::u_Hours: {
            hours *= n;
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes *= n;
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds *= n;
            break;
         }
      }
   }
   else {
      years *= n;
      months *= n;
      weeks *= n;
      days *= n;
      hours *= n;
      minutes *= n;
      seconds *= n;
      years_sec *= n;
      months_sec *= n;
      years_ad *= n;
      months_ad *= n;
      if (periodType == PeriodType::pt_Difference) {
         checkDifference();
      }
   }

   return *this;
}

Period& Period::operator /= (const Number& num)
{
   const _tnum n = (_tnum)num.toInt();

   if (n == 0) {
      throw UroRuntimeException(L"division by zero");
   }

   if (periodType ==  PeriodType::pt_Unit) {
      switch (periodUnit) {
         case PeriodUnit::u_Years: {
            years /= n;
            break;
         }
         case PeriodUnit::u_Months: {
            months /= n;
            break;
         }
         case PeriodUnit::u_Weeks: {
            weeks /= n;
            break;
         }
         case PeriodUnit::u_Days: {
            days /= n;
            break;
         }
         case PeriodUnit::u_Hours: {
            hours /= n;
            break;
         }
         case PeriodUnit::u_Minutes: {
            minutes /= n;
            break;
         }
         case PeriodUnit::u_Seconds: {
            seconds /= n;
            break;
         }
      }
   }
   else {
      years /= n;
      months /= n;
      weeks /= n;
      days /= n;
      hours /= n;
      minutes /= n;
      seconds /= n;
      years_sec /= n;
      months_sec /= n;
      years_ad /= n;
      months_ad /= n;
   }

   return *this;
}

Period Period::operator + (const Period& per) const
{
   Period result = *this;
   result += per;
   return result;
}

Period Period::operator - (const Period& per) const
{
   Period result = *this;
   result -= per;
   return result;
}

Period Period::operator - () const
{
   Period p;

   p.years = -years;
   p.months = -months;
   p.weeks = -weeks;
   p.days = -days;
   p.hours = -hours;
   p.minutes = -minutes;
   p.seconds = -seconds;
   p.years_sec = -years_sec;
   p.months_sec = -months_sec;
   p.years_ad = -years_ad;
   p.months_ad = -months_ad;

   p.periodType = periodType;
   p.periodUnit = periodUnit;

   return p;
}

_boo Period::operator == (const Period& per) const
{
   switch (periodType) {
      case PeriodType::pt_Unit: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodUnitsCmp(per) == 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffUnitCmp(per, *this) == 0;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) == 0;
            }
         }
         break;
      }
      case PeriodType::pt_Difference: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodDiffUnitCmp(*this, per) == 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffsCmp(per) == 0;
               break;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) == 0;
            }
         }
         break;
      }
   }

   return periodCmp(per) == 0;
}

_boo Period::operator != (const Period& per) const
{
   switch (periodType) {
      case PeriodType::pt_Unit: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodUnitsCmp(per) != 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffUnitCmp(per, *this) != 0;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) != 0;
            }
         }
         break;
      }
      case PeriodType::pt_Difference: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodDiffUnitCmp(*this, per) != 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffsCmp(per) != 0;
               break;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) != 0;
            }
         }
         break;
      }
   }

   return periodCmp(per) != 0;
}

_boo Period::operator < (const Period& per) const
{
   switch (periodType) {
      case PeriodType::pt_Unit: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodUnitsCmp(per) < 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffUnitCmp(per, *this) < 0;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) < 0;
            }
         }
         break;
      }
      case PeriodType::pt_Difference: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodDiffUnitCmp(*this, per) < 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffsCmp(per) < 0;
               break;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) < 0;
            }
         }
         break;
      }
   }

   return periodCmp(per) < 0;
}

_boo Period::operator > (const Period& per) const
{
   switch (periodType) {
      case PeriodType::pt_Unit: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodUnitsCmp(per) > 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffUnitCmp(per, *this) > 0;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) > 0;
            }
         }
         break;
      }
      case PeriodType::pt_Difference: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodDiffUnitCmp(*this, per) > 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffsCmp(per) > 0;
               break;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) > 0;
            }
         }
         break;
      }
   }

   return periodCmp(per) > 0;
}

_boo Period::operator <= (const Period& per) const
{
   switch (periodType) {
      case PeriodType::pt_Unit: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodUnitsCmp(per) <= 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffUnitCmp(per, *this) <= 0;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) <= 0;
            }
         }
         break;
      }
      case PeriodType::pt_Difference: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodDiffUnitCmp(*this, per) <= 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffsCmp(per) <= 0;
               break;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) <= 0;
            }
         }
         break;
      }
   }

   return periodCmp(per) <= 0;
}

_boo Period::operator >= (const Period& per) const
{
   switch (periodType) {
      case PeriodType::pt_Unit: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodUnitsCmp(per) >= 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffUnitCmp(per, *this) >= 0;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) >= 0;
            }
         }
         break;
      }
      case PeriodType::pt_Difference: {
         switch (per.periodType) {
            case PeriodType::pt_Unit: {
               return periodDiffUnitCmp(*this, per) >= 0;
            }
            case PeriodType::pt_Difference: {
               return periodDiffsCmp(per) >= 0;
               break;
            }
            case PeriodType::pt_Mingled: {
               return periodCmp(per) >= 0;
            }
         }
         break;
      }
   }

   return periodCmp(per) >= 0;
}

// all 49 possibilities hardcoded
inline _tnum Period::periodUnitsCmp(const Period& per) const
{
   switch (periodUnit) {
      case PeriodUnit::u_Years: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return years - per.years;
            case PeriodUnit::u_Months:
               return years * 12 - per.months;
            case PeriodUnit::u_Weeks:
               return years * 365 - per.weeks * 7;
            case PeriodUnit::u_Days:
               return years * 365 - per.days;
            case PeriodUnit::u_Hours:
               return years * 8760 - per.hours;
            case PeriodUnit::u_Minutes:
               return years * 525600 - per.minutes;
            case PeriodUnit::u_Seconds:
               return years * 31536000 - per.seconds;
         }
         break;
      }
      case PeriodUnit::u_Months: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return months - 12 * per.years;
            case PeriodUnit::u_Months:
               return months - per.months;
            case PeriodUnit::u_Weeks:
               return months * 30 - 7 * per.weeks;
            case PeriodUnit::u_Days:
               return months * 30 - per.days;
            case PeriodUnit::u_Hours:
               return months * 720 - per.hours;
            case PeriodUnit::u_Minutes:
               return months * 43200 - per.minutes;
            case PeriodUnit::u_Seconds:
               return months * 2592000 - per.seconds;
         }
         break;
      }
      case PeriodUnit::u_Weeks: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return weeks * 7 - 365 * per.years;
            case PeriodUnit::u_Months:
               return weeks * 7 - 30 * per.months;
            case PeriodUnit::u_Weeks:
               return weeks - per.weeks;
            case PeriodUnit::u_Days:
               return weeks * 7 - per.days;
            case PeriodUnit::u_Hours:
               return weeks * 168 - per.hours;
            case PeriodUnit::u_Minutes:
               return weeks * 10080 - per.minutes;
            case PeriodUnit::u_Seconds:
               return weeks * 604800 - per.seconds;
         }
         break;
      }
      case PeriodUnit::u_Days: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return days - 365 * per.years;
            case PeriodUnit::u_Months:
               return days - 30 * per.months;
            case PeriodUnit::u_Weeks:
               return days - 7 * per.weeks;
            case PeriodUnit::u_Days:
               return days - per.days;
            case PeriodUnit::u_Hours:
               return days * 24 - per.hours;
            case PeriodUnit::u_Minutes:
               return days * 1440 - per.minutes;
            case PeriodUnit::u_Seconds:
               return days * 86400 - per.seconds;
         }
         break;
      }
      case PeriodUnit::u_Hours: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return hours - 8760 * per.years;
            case PeriodUnit::u_Months:
               return hours - 720 * per.months;
            case PeriodUnit::u_Weeks:
               return hours - 168 * per.weeks;
            case PeriodUnit::u_Days:
               return hours - 24 * per.days;
            case PeriodUnit::u_Hours:
               return hours - per.hours;
            case PeriodUnit::u_Minutes:
               return hours * 60 - per.minutes;
            case PeriodUnit::u_Seconds:
               return hours * 3600 - per.seconds;
         }
         break;
      }
      case PeriodUnit::u_Minutes: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return minutes - 525600 * per.years;
            case PeriodUnit::u_Months:
               return minutes - 43200 * per.months;
            case PeriodUnit::u_Weeks:
               return minutes - 10080 * per.weeks;
            case PeriodUnit::u_Days:
               return minutes - 1440 * per.days;
            case PeriodUnit::u_Hours:
               return minutes - 60 * per.hours;
            case PeriodUnit::u_Minutes:
               return minutes - per.minutes;
            case PeriodUnit::u_Seconds:
               return minutes * 60 - per.seconds;
         }
         break;
      }
      case PeriodUnit::u_Seconds: {
         switch (per.periodUnit) {
            case PeriodUnit::u_Years:
               return seconds - 31536000 * per.years;
            case PeriodUnit::u_Months:
               return seconds - 2592000 * per.months;
            case PeriodUnit::u_Weeks:
               return seconds - 604800 * per.weeks;
            case PeriodUnit::u_Days:
               return seconds - 86400 * per.days;
            case PeriodUnit::u_Hours:
               return seconds - 3600 * per.hours;
            case PeriodUnit::u_Minutes:
               return seconds - 60 * per.minutes;
            case PeriodUnit::u_Seconds:
               return seconds - per.seconds;
         }
         break;
      }
   }

   return 0;
}

inline _tnum Period::periodDiffsCmp(const Period& diff) const
{
   _tnum value = days + months_ad + years_ad
      - diff.days - diff.months_ad - diff.years_ad;

   if (value != 0) return value;

   value = hours - diff.hours;
   if (value != 0) return value;

   value = minutes - diff.minutes;

   return value == 0
      ? seconds - diff.seconds
      : value;
}

inline _tnum Period::periodCmp(const Period& per) const
{
   _tnum s = (hours - per.hours) * 3600
      + (minutes - per.minutes) * 60 + seconds - per.seconds;
   _tnum d = (s / 86400) + days - per.days + 7 * (weeks - per.weeks);
   s %= 86400;

   _tnum y1 = years - per.years;
   _tnum y2 = years_sec - per.years_sec;
   _tnum m1 = months - per.months;
   _tnum m2 = months_sec - per.months_sec;

   const _boo noYear = (y1 == 0) && (y2 == 0);
   const _boo noMonth = (m1 == 0) && (m2 == 0);

   if (noYear || noMonth) {
      d += months_ad - per.months_ad + years_ad - per.years_ad;

      if (noYear) {
         if (noMonth) {
            return d == 0 ? s : d;
         }

         if (m1 == -m2) {
            return d == 0 ? s : d;
         }

         d += m1 * 30;
         return d == 0 ? s : d;
      }

      if (noMonth) {
         if (y1 == -y2) {
            return d == 0 ? s : d;
         }

         d += y1 * 365;
         return d == 0 ? s : d;
      }
   }

   if (m2 == 0 && y2 == 0) {
      d += months_ad - per.months_ad + years_ad - per.years_ad;
      if ((y1 * 12) == -m1) {
         return d == 0 ? s : d;
      }

      if (y1 >= 0) {
         if (m1 >= 0) {
            d += 365 * y1 + 30 * m1;
         }
         else {
            const _tnum x = (-m1) / 12;
            const _tnum mx = x > y1 ? y1 : x;

            d += mx == 0
               ? (365 * y1 + 30 * m1)
               : (365 * (y1 - mx) + 30 * (m1 + 12 * mx));
         }
      }
      else {
         if (m1 >= 0) {
            const _tnum x = m1 / 12;
            const _tnum mx = x > (-y1) ? (-y1) : x;

            d += mx == 0
               ? (365 * y1 + 30 * m1)
               : (365 * (y1 + mx) + 30 * (m1 - 12 * mx));
         }
         else {
            d += 365 * y1 + 30 * m1;
         }
      }

      return d == 0 ? s : d;
   }

   // final
   _boo ney = false;
   _boo nem = false;

   if ((y1 > 0 && y2 < 0 && y1 >= (-y2)) ||
       (y1 < 0 && y2 > 0 && (-y1) >= y2))
   {
      ney = true;
      y1 += y2;
      y2 = 0;
   }

   if ((m1 > 0 && m2 < 0 && m1 >= (-m2)) ||
       (m1 < 0 && m2 > 0 && (-m1) >= m2))
   {
      nem = true;
      m1 += m2;
      m2 = 0;
   }

   if (!ney)
   {
      if ((m1 > 0 && y2 < 0 && m1 >= (-y2 * 12)) ||
         (m1 < 0 && y2 > 0 && (-m1) >= y2 * 12))
      {
         ney = true;
         m1 += y2 * 12;
         y2 = 0;
      }
   }

   if (!nem) {
      if ((y1 > 0 && m2 < 0 && (y1 * 12) >= (-m2)))
      {
         const _tnum a = ((-m2) / 12)
            + (((-m2) % 12) == 0 ? 0 : 1);

         y1 -= a;
         m1 += m2 + a * 12;
         m2 = 0;
         nem = true;
      }
      else if (y1 < 0 && m2 > 0 && (-y1 * 12) >= m2)
      {
         const _tnum a = (m2 / 12)
            + ((m2 % 12) == 0 ? 0 : 1);

         y1 += a;
         m1 -= m2 + 12 * a;
         m2 = 0;
         nem = true;
      }
   }

   if (!ney) {
      d += years_ad - per.years_ad;
   }
   if (!nem) {
      d += months_ad - per.months_ad;
   }

   if ((y1 * 12) == -m1) {
      return d == 0 ? s : d;
   }

   if (y1 >= 0) {
      if (m1 >= 0) {
         d += 365 * y1 + 30 * m1;
      }
      else {
         const _tnum x = (-m1) / 12;
         const _tnum mx = x > y1 ? y1 : x;

         d += mx == 0
            ? (365 * y1 + 30 * m1)
            : (365 * (y1 - mx) + 30 * (m1 + 12 * mx));
      }
   }
   else {
      if (m1 >= 0) {
         const _tnum x = m1 / 12;
         const _tnum mx = x > (-y1) ? (-y1) : x;

         d += mx == 0
            ? (365 * y1 + 30 * m1)
            : (365 * (y1 + mx) + 30 * (m1 - 12 * mx));
      }
      else {
         d += 365 * y1 + 30 * m1;
      }
   }

   return d == 0 ? s : d;
}

inline _tnum periodDiffUnitCmp(const Period& diff, const Period& unit)
{
   switch (unit.periodUnit) {
      case Period::u_Years: {
         const _tnum v = diff.years + diff.years_sec - unit.years;
         if (v != 0) {
            return v;
         }

         if (diff.months != 0) return diff.months;
         if (diff.months_sec != 0) return diff.months_sec;
         if (diff.months_ad != 0) return diff.months_ad;
         if (diff.days != 0) return diff.days;
         if (diff.hours != 0) return diff.hours;
         if (diff.minutes != 0) return diff.minutes;
         if (diff.seconds != 0) return diff.seconds;

         return 0;
      }
      case Period::u_Months: {
         const _tnum v = 12 * (diff.years + diff.years_sec)
            + diff.months + diff.months_sec - unit.months;

         if (v != 0) {
            return v;
         }

         if (diff.days != 0) return diff.days;
         if (diff.hours != 0) return diff.hours;
         if (diff.minutes != 0) return diff.minutes;
         if (diff.seconds != 0) return diff.seconds;

         return 0;
      }
      case Period::u_Weeks: {
         const _tnum v = 365 * diff.years + diff.years_ad + 30
            * diff.months + diff.months_ad + diff.days - 7 * unit.weeks;

         if (v != 0) {
            return v;
         }

         if (diff.hours != 0) return diff.hours;
         if (diff.minutes != 0) return diff.minutes;
         if (diff.seconds != 0) return diff.seconds;

         return 0;
      }
      case Period::u_Days: {
         const _tnum v = 365 * diff.years + diff.years_ad + 30 * diff.months
            + diff.months_ad + diff.days - unit.days;

         if (v != 0) {
            return v;
         }

         if (diff.hours != 0) return diff.hours;
         if (diff.minutes != 0) return diff.minutes;
         if (diff.seconds != 0) return diff.seconds;

         return 0;
      }
      case Period::u_Hours: {
         const _tnum v = 24 * (365 * diff.years + diff.years_ad
            + 30 * diff.months + diff.months_ad + diff.days)
            + diff.hours - unit.hours;

         if (v != 0) {
            return v;
         }

         if (diff.minutes != 0) return diff.minutes;
         if (diff.seconds != 0) return diff.seconds;

         return 0;
      }
      case Period::u_Minutes: {
         const _tnum v = 60 * (24 * (365 * diff.years + diff.years_ad
            + 30 * diff.months + diff.months_ad + diff.days) + diff.hours)
            + diff.minutes - unit.minutes;

         return v == 0 ? diff.seconds : v;
      }
      case Period::u_Seconds: {
         return 60 * (60 * (24 * (365 * diff.years + diff.years_ad
            + 30 * diff.months + diff.months_ad + diff.days)
            + diff.hours) + diff.minutes) + diff.seconds - unit.seconds;
      }
   }

   return 0;
}


