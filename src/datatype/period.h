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

#pragma once

#include <vector>
#include "number.h"
#include "../exception.h"
#include "time-const.h"


namespace perun2
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
   Period(const p_tnum val, const PeriodUnit unit);

   p_tnum years = TNUM_ZERO;
   p_tnum months = TNUM_ZERO;
   p_tnum weeks = TNUM_ZERO;
   p_tnum days = TNUM_ZERO;
   p_tnum hours = TNUM_ZERO;
   p_tnum minutes = TNUM_ZERO;
   p_tnum seconds = TNUM_ZERO;
   p_tnum years_sec = TNUM_ZERO; // years with known amount of days
   p_tnum months_sec = TNUM_ZERO; // months with known amount of days
   p_tnum years_ad = TNUM_ZERO; // days in years from variable 'years_sec'
   p_tnum months_ad = TNUM_ZERO; // days in months from variable 'months_sec'
   PeriodUnit periodUnit;
   PeriodType periodType;

   p_nint toSeconds() const;
   p_str toString() const;

   void clear();
   void reverse();
   inline void subtractUnit(const Period& per);

   Period& operator += (const Period& per);
   Period& operator -= (const Period& per);
   Period& operator *= (const Number& num);
   Period& operator /= (const Number& num);
   Period operator + (const Period& per) const;
   Period operator - (const Period& per) const;
   Period operator - () const;

   p_bool operator == (const Period& per) const;
   p_bool operator != (const Period& per) const;
   p_bool operator < (const Period& per) const;
   p_bool operator > (const Period& per) const;
   p_bool operator <= (const Period& per) const;
   p_bool operator >= (const Period& per) const;

private:

   inline void addUnit(const Period& per);
   inline p_bool isNegativeDifference() const;
   inline void checkDifference();

   inline p_tnum periodUnitsCmp(const Period& per) const;
   inline p_tnum periodDiffsCmp(const Period& diff) const;
   inline p_tnum periodCmp(const Period& per) const;
};

inline p_tnum periodDiffUnitCmp(const Period& diff, const Period& unit);
// return = 0       periods are equal
// return < 0       left period is smaller

}
