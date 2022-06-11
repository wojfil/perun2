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

#ifndef GEN_PERIOD_H
#define GEN_PERIOD_H

#include "../generator.h"
#include "gen-generic.h"


struct PeriodUnit : Generator<_per>
{
public:
   PeriodUnit(Generator<_num>* val, Period::PeriodUnit un)
      : value(val), unit(un) { };

   ~PeriodUnit() {
      delete value;
   }

   _per getValue() override;

private:
   Generator<_num>* value;
   const Period::PeriodUnit unit;
};


struct PeriodAddition : BinaryOperation<_per>
{
   PeriodAddition(Generator<_per>* val1, Generator<_per>* val2)
      : BinaryOperation<_per>(val1, val2) { };

   _per getValue() override;
};


struct PeriodSubtraction : Generator<_per>
{
public:
   PeriodSubtraction(Generator<_per>* val1, Generator<_per>* val2)
      : value1(val1), value2(val2) { };

   ~PeriodSubtraction() {
      delete value1;
      delete value2;
   }

   _per getValue() override;

private:
   Generator<_per>* value1;
   Generator<_per>* value2;
};


struct TimeDifference : Generator<_per>
{
public:
   TimeDifference(Generator<_tim>* val1, Generator<_tim>* val2)
      : value1(val1), value2(val2) { };

   ~TimeDifference() {
      delete value1;
      delete value2;
   }

   _per getValue() override;

private:
   Generator<_tim>* value1;
   Generator<_tim>* value2;
};


struct NegatedPeriod : Generator<_per>
{
public:
   NegatedPeriod(Generator<_per>* val) : value(val){ };

   ~NegatedPeriod() {
      delete value;
   }

   _per getValue() override;

private:
   Generator<_per>* value;
};


#endif /* GEN_PERIOD_H */
