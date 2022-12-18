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

#ifndef PERIOD_H
#define PERIOD_H

#include <vector>
#include "number.h"
#include "../exception.h"


namespace uro
{

// _tnum is a data structure used for every element of Time and Period
// let it be "int" as default
// it can be anything, but necessarily non-unsigned
typedef _int _tnum;

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

   _tnum years = 0;
   _tnum months = 0;
   _tnum weeks = 0;
   _tnum days = 0;
   _tnum hours = 0;
   _tnum minutes = 0;
   _tnum seconds = 0;
   _tnum years_sec = 0; // years with known amount of days
   _tnum months_sec = 0; // months with known amount of days
   _tnum years_ad = 0; // days in years from variable 'years_sec'
   _tnum months_ad = 0; // days in months from variable 'months_sec'
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

