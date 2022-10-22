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


namespace uro::gen
{

struct PeriodUnit : Generator<_per>
{
public:
   PeriodUnit() = delete;
   PeriodUnit(Generator<_num>* val, Period::PeriodUnit un);
   ~PeriodUnit();
   _per getValue() override;

private:
   Generator<_num>* value;
   const Period::PeriodUnit unit;
};


struct PeriodAddition : BinaryOperation<_per>
{
public:
   PeriodAddition() = delete;
   PeriodAddition(Generator<_per>* val1, Generator<_per>* val2);
   _per getValue() override;
};


struct PeriodSubtraction : BinaryOperation<_per>
{
public:
   PeriodSubtraction() = delete;
   PeriodSubtraction(Generator<_per>* val1, Generator<_per>* val2);
   _per getValue() override;
};


struct TimeDifference : Generator<_per>
{
public:
   TimeDifference() = delete;
   TimeDifference(Generator<_tim>* val1, Generator<_tim>* val2);
   ~TimeDifference();
   _per getValue() override;

private:
   Generator<_tim>* value1;
   Generator<_tim>* value2;
};


struct NegatedPeriod : Generator<_per>
{
public:
   NegatedPeriod() = delete;
   NegatedPeriod(Generator<_per>* val);
   ~NegatedPeriod();
   _per getValue() override;

private:
   Generator<_per>* value;
};

}

#endif /* GEN_PERIOD_H */
