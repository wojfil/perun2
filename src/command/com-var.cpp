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

#include "com-var.h"


void VarNumMultiply::run()
{
   (variable->value) *= (value->getValue());
};

void VarNumDivide::run()
{
   (variable->value) /= (value->getValue());
};

void VarPerMultiply::run()
{
   (variable->value) *= (value->getValue());
};

void VarPerDivide::run()
{
   (variable->value) /= (value->getValue());
};

void VarModulo::run()
{
   (variable->value) %= (value->getValue());
};

void VarIncrement::run()
{
   (variable->value)++;
};

void VarDecrement::run()
{
   (variable->value)--;
};

void VarTimeAdd::run()
{
   (variable->value) += (period->getValue());
}

void VarTimeSubtract::run()
{
   (variable->value) -= (period->getValue());
}

void VarCharAssignment::run()
{
   const _size len = variable->value.size();
   const _nint n = index->getValue().toInt();

   if (n >= 0 && n < len) {
      const _str v = value->getValue();
      const _size vlen = v.size();

      switch (vlen) {
         case 0: {
            if (len != 0) {
               if (n == 0) {
                  variable->value = variable->value.substr(1);
               }
               else if (n == len - 1) {
                  variable->value = variable->value.substr(0, len - 1);
               }
               else {
                  variable->value =
                     variable->value.substr(0, n) +
                     variable->value.substr(n + 1);
               }
            }
            break;
         }
         case 1: {
            variable->value[n] = v[0];
            break;
         }
         default: {
            if (n == 0) {
               if (len == 1) {
                  variable->value = v;
               }
               else {
                  variable->value = v +
                     variable->value.substr(1);
               }
            }
            else if (n == len - 1) {
               if (len == 1) {
                  variable->value = v;
               }
               else {
                  variable->value =
                     variable->value.substr(0, n) + v;
               }
            }
            else {
               variable->value =
                  variable->value.substr(0, n) + v +
                  variable->value.substr(n + 1);
            }
            break;
         }
      }
   }
   else if (n == len) {
      const _str v = value->getValue();
      const _size vlen = v.size();

      if (!v.empty()) {
         variable->value += v;
      }
   }
}

void VarTimeUnitAssignment::run()
{
   const _tnum n = toTimeNumber(value->getValue());

   switch (unit) {
      case Period::u_Years: {
         variable->value.setYear(n);
         break;
      }
      case Period::u_Months: {
         variable->value.setMonth(n);
         break;
      }
      case Period::u_Days: {
         variable->value.setDay(n);
         break;
      }
      case Period::u_Hours: {
         variable->value.setHour(n);
         break;
      }
      case Period::u_Minutes: {
         variable->value.setMinute(n);
         break;
      }
      case Period::u_Seconds: {
         variable->value.setSecond(n);
         break;
      }
   }
};

void VarTimeUnitChange::run()
{
   const _tnum n = toTimeNumber(value->getValue()) * (negative ? -1 : 1);

   switch (unit) {
      case Period::u_Years: {
         variable->value.addYears(n);
         break;
      }
      case Period::u_Months: {
         variable->value.addMonths(n);
         break;
      }
      case Period::u_Weeks: {
         variable->value.addWeeks(n);
         break;
      }
      case Period::u_Days: {
         variable->value.addDays(n);
         break;
      }
      case Period::u_Hours: {
         variable->value.addHours(n);
         break;
      }
      case Period::u_Minutes: {
         variable->value.addMinutes(n);
         break;
      }
      case Period::u_Seconds: {
         variable->value.addSeconds(n);
         break;
      }
   }
};


void VarTimeUnitIncrement::run()
{
   switch (unit) {
      case Period::u_Years: {
         variable->value.addYears(1);
         break;
      }
      case Period::u_Months: {
         variable->value.addMonths(1);
         break;
      }
      case Period::u_Weeks: {
         variable->value.addWeeks(1);
         break;
      }
      case Period::u_Days: {
         variable->value.addDays(1);
         break;
      }
      case Period::u_Hours: {
         variable->value.addHours(1);
         break;
      }
      case Period::u_Minutes: {
         variable->value.addMinutes(1);
         break;
      }
      case Period::u_Seconds: {
         variable->value.addSeconds(1);
         break;
      }
   }
};


void VarTimeUnitDecrement::run()
{
   switch (unit) {
      case Period::u_Years: {
         variable->value.addYears(-1);
         break;
      }
      case Period::u_Months: {
         variable->value.addMonths(-1);
         break;
      }
      case Period::u_Weeks: {
         variable->value.addWeeks(-1);
         break;
      }
      case Period::u_Days: {
         variable->value.addDays(-1);
         break;
      }
      case Period::u_Hours: {
         variable->value.addHours(-1);
         break;
      }
      case Period::u_Minutes: {
         variable->value.addMinutes(-1);
         break;
      }
      case Period::u_Seconds: {
         variable->value.addSeconds(-1);
         break;
      }
   }
};

