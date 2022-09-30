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

#ifndef GEN_NUMBER_H
#define GEN_NUMBER_H

#include "../generator.h"
#include "gen-generic.h"


namespace uro::gen
{

struct Negation : UnaryOperation<_num>
{
   Negation(Generator<_num>* val) : UnaryOperation<_num>(val) { };

   _num getValue() override;
};


struct Addition : BinaryOperation<_num>
{
   Addition(Generator<_num>* val1, Generator<_num>* val2)
      : BinaryOperation<_num>(val1, val2) { };

   _num getValue() override;
};


struct Subtraction : BinaryOperation<_num>
{
   Subtraction(Generator<_num>* val1, Generator<_num>* val2)
      : BinaryOperation<_num>(val1, val2) { };

   _num getValue() override;
};


struct Multiplication : BinaryOperation<_num>
{
   Multiplication(Generator<_num>* val1, Generator<_num>* val2)
      : BinaryOperation<_num>(val1, val2) { };

   _num getValue() override;
};


struct Division : BinaryOperation<_num>
{
   Division(Generator<_num>* val1, Generator<_num>* val2)
      : BinaryOperation<_num>(val1, val2) { };

   _num getValue() override;
};


struct Modulo : BinaryOperation<_num>
{
   Modulo(Generator<_num>* val1, Generator<_num>* val2)
      : BinaryOperation<_num>(val1, val2) { };

   _num getValue() override;
};


struct TimeMember
{
public:
   TimeMember (Generator<_tim>* tim) : time(tim) {};

   ~TimeMember() {
      delete time;
   }

protected:
   Generator<_tim>* time;
};


struct TimeYears : Generator<_num>, TimeMember
{
   TimeYears(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeMonths : Generator<_num>, TimeMember
{
   TimeMonths(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeWeekDay : Generator<_num>, TimeMember
{
   TimeWeekDay(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeDays : Generator<_num>, TimeMember
{
   TimeDays(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeHours : Generator<_num>, TimeMember
{
   TimeHours(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeMinutes : Generator<_num>, TimeMember
{
   TimeMinutes(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeSeconds : Generator<_num>, TimeMember
{
   TimeSeconds(Generator<_tim>* tim) : TimeMember(tim) {};
   _num getValue() override;
};


struct TimeMemberAtIndex
{
public:
   TimeMemberAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : times(tims), index(in) {};

   ~TimeMemberAtIndex();
   _bool getElement();

protected:
   Generator<_tlist>* times;
   Generator<_num>* index;
   _tlist result;
   _tim* time;
};


struct TimeYearsAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeYearsAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};


struct TimeMonthsAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeMonthsAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};


struct TimeWeekDayAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeWeekDayAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};


struct TimeDaysAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeDaysAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};


struct TimeHoursAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeHoursAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};


struct TimeMinutesAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeMinutesAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};


struct TimeSecondsAtIndex : Generator<_num>, TimeMemberAtIndex
{
   TimeSecondsAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _num getValue() override;
};

}

#endif /* GEN_NUMBER_H */

