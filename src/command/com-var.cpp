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

#include "com-var.h"


namespace perun2::comm
{

void VarNumMultiply::run()
{
   variable.value *= value->getValue();
};

void VarNumDivide::run()
{
   variable.value /= value->getValue();
};

void VarPerMultiply::run()
{
   variable.value *= value->getValue();
};

void VarPerDivide::run()
{
   variable.value /= value->getValue();
};

void VarModulo::run()
{
   variable.value %= value->getValue();
};

void VarIncrement::run()
{
   variable.value++;
};

void VarDecrement::run()
{
   variable.value--;
};

void VarTimeAdd::run()
{
   variable.value += period->getValue();
}

void VarTimeSubtract::run()
{
   variable.value -= period->getValue();
}

void VarCharAssignment::run()
{
   const p_size len = variable.value.size();
   const p_num num = index->getValue();

   if (num.state == NumberState::NaN) {
      return;
   }

   p_nint n = num.toInt();

   if (n < NINT_ZERO) {
      n += len;
   }

   if (n < NINT_ZERO || n >= static_cast<p_nint>(len)) {
      return;
   }

   const p_str v = value->getValue();
   const p_size vlen = v.size();

   switch (vlen) {
      case 0: {
         if (len != 0) {
            if (n == NINT_ZERO) {
               variable.value = variable.value.substr(1);
            }
            else if (n == static_cast<p_nint>(len) - NINT_ONE) {
               variable.value = variable.value.substr(0, len - 1);
            }
            else {
               variable.value = str(variable.value.substr(0, static_cast<p_size>(n)), 
                  variable.value.substr(static_cast<p_size>(n) + 1));
            }
         }
         break;
      }
      case 1: {
         variable.value[n] = v[0];
         break;
      }
      default: {
         if (n == NINT_ZERO) {
            if (len == 1) {
               variable.value = v;
            }
            else {
               variable.value = str(v, variable.value.substr(1));
            }
         }
         else if (n == static_cast<p_nint>(len) - NINT_ONE) {
            if (len == 1) {
               variable.value = v;
            }
            else {
               variable.value = str(variable.value.substr(0, static_cast<p_size>(n)), v);
            }
         }
         else {
            variable.value = str(variable.value.substr(0, static_cast<p_size>(n)), v, 
               variable.value.substr(static_cast<p_size>(n) + 1));
         }
         break;
      }
   }
}

void VarTimeUnitAssignment::run()
{
   const p_num num = value->getValue();
   if (num.state == NumberState::NaN) {
      variable.value.clear();
      return;
   }

   const p_tnum n = toTimeNumber(num);

   switch (unit) {
      case Period::u_Years: {
         variable.value.setYear(n);
         break;
      }
      case Period::u_Months: {
         variable.value.setMonth(n);
         break;
      }
      case Period::u_Days: {
         variable.value.setDay(n);
         break;
      }
      case Period::u_Hours: {
         variable.value.setHour(n);
         break;
      }
      case Period::u_Minutes: {
         variable.value.setMinute(n);
         break;
      }
      case Period::u_Seconds: {
         variable.value.setSecond(n);
         break;
      }
   }
};

void VarTimeUnitChange::run()
{
   const p_num num = value->getValue();
   if (num.state == NumberState::NaN) {
      variable.value.clear();
      return;
   }

   const p_tnum n = toTimeNumber(num) * (negative ? TNUM_MINUS_ONE : TNUM_ONE);

   switch (unit) {
      case Period::u_Years: {
         variable.value.addYears(n);
         break;
      }
      case Period::u_Months: {
         variable.value.addMonths(n);
         break;
      }
      case Period::u_Weeks: {
         variable.value.addWeeks(n);
         break;
      }
      case Period::u_Days: {
         variable.value.addDays(n);
         break;
      }
      case Period::u_Hours: {
         variable.value.addHours(n);
         break;
      }
      case Period::u_Minutes: {
         variable.value.addMinutes(n);
         break;
      }
      case Period::u_Seconds: {
         variable.value.addSeconds(n);
         break;
      }
   }
};


void VarTimeUnitIncrement::run()
{
   switch (unit) {
      case Period::u_Years: {
         variable.value.addYears(TNUM_ONE);
         break;
      }
      case Period::u_Months: {
         variable.value.addMonths(TNUM_ONE);
         break;
      }
      case Period::u_Weeks: {
         variable.value.addWeeks(TNUM_ONE);
         break;
      }
      case Period::u_Days: {
         variable.value.addDays(TNUM_ONE);
         break;
      }
      case Period::u_Hours: {
         variable.value.addHours(TNUM_ONE);
         break;
      }
      case Period::u_Minutes: {
         variable.value.addMinutes(TNUM_ONE);
         break;
      }
      case Period::u_Seconds: {
         variable.value.addSeconds(TNUM_ONE);
         break;
      }
   }
};


void VarTimeUnitDecrement::run()
{
   switch (unit) {
      case Period::u_Years: {
         variable.value.addYears(TNUM_MINUS_ONE);
         break;
      }
      case Period::u_Months: {
         variable.value.addMonths(TNUM_MINUS_ONE);
         break;
      }
      case Period::u_Weeks: {
         variable.value.addWeeks(TNUM_MINUS_ONE);
         break;
      }
      case Period::u_Days: {
         variable.value.addDays(TNUM_MINUS_ONE);
         break;
      }
      case Period::u_Hours: {
         variable.value.addHours(TNUM_MINUS_ONE);
         break;
      }
      case Period::u_Minutes: {
         variable.value.addMinutes(TNUM_MINUS_ONE);
         break;
      }
      case Period::u_Seconds: {
         variable.value.addSeconds(TNUM_MINUS_ONE);
         break;
      }
   }
};

}
