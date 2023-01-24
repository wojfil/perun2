/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PERIOD_H
#define PERIOD_H

#include <vector>
#include "number.h"
#include "../exception.h"
#include "time-const.h"


namespace uro
{

struct Period
{

public:

   enum PeriodUnit
   {
      u_Years = 0,
      u_Months = 1,
      u_Weeks = 2,
      u_Days = 3,
      u_Hours = 4,
      u_Minutes = 5,
      u_Seconds = 6
   };

   enum PeriodType
   {
      pt_Unit = 0,       // period is just one unit (certain amount of years / months / days...)
      pt_Difference = 1, // period is a result of a time subtraction
      pt_Mingled = 2     // period is a result of complex operations
   };

   Period();
   Period(const _tnum& val, const PeriodUnit& unit);

   _tnum years = TNUM_ZERO;
   _tnum months = TNUM_ZERO;
   _tnum weeks = TNUM_ZERO;
   _tnum days = TNUM_ZERO;
   _tnum hours = TNUM_ZERO;
   _tnum minutes = TNUM_ZERO;
   _tnum seconds = TNUM_ZERO;
   _tnum years_sec = TNUM_ZERO; // years with known amount of days
   _tnum months_sec = TNUM_ZERO; // months with known amount of days
   _tnum years_ad = TNUM_ZERO; // days in years from variable 'years_sec'
   _tnum months_ad = TNUM_ZERO; // days in months from variable 'months_sec'
   PeriodUnit periodUnit;
   PeriodType periodType;

   _nint toSeconds() const;
   _str toString() const;

   void reverse();
   inline void subtractUnit(const Period& per);
   void turnWeeksToDays();

   Period& operator += (const Period& per);
   Period& operator -= (const Period& per);
   Period& operator *= (const Number& num);
   Period& operator /= (const Number& num);
   Period operator + (const Period& per) const;
   Period operator - (const Period& per) const;
   Period operator - () const;

   _bool operator == (const Period& per) const;
   _bool operator != (const Period& per) const;
   _bool operator < (const Period& per) const;
   _bool operator > (const Period& per) const;
   _bool operator <= (const Period& per) const;
   _bool operator >= (const Period& per) const;

private:

   inline void addUnit(const Period& per);
   inline _bool isNegativeDifference() const;
   inline void checkDifference();

   inline _tnum periodUnitsCmp(const Period& per) const;
   inline _tnum periodDiffsCmp(const Period& diff) const;
   inline _tnum periodCmp(const Period& per) const;
};

inline _tnum periodDiffUnitCmp(const Period& diff, const Period& unit);
// if return value = 0       periods are equal
// if return value < 0       left period is smaller
// if return value > 0       left period is greater

}

#endif /* PERIOD_H */

