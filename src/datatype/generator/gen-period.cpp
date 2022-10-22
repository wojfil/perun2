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

#include "gen-period.h"

namespace uro::gen
{


PeriodUnit::PeriodUnit(Generator<_num>* val, Period::PeriodUnit un)
   : value(val), unit(un) { };

PeriodUnit::PeriodUnit::~PeriodUnit() 
{
   delete this->value;
}

_per PeriodUnit::getValue()
{
   const _tnum n = toTimeNumber(this->value->getValue());
   return _per(n, this->unit);
}

PeriodAddition::PeriodAddition(Generator<_per>* val1, Generator<_per>* val2)
   : BinaryOperation<_per>(val1, val2) { }

_per PeriodAddition::getValue()
{
   return this->value1->getValue() + this->value2->getValue();
}

PeriodSubtraction::PeriodSubtraction(Generator<_per>* val1, Generator<_per>* val2)
   : BinaryOperation<_per>(val1, val2) { }

_per PeriodSubtraction::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

TimeDifference::TimeDifference(Generator<_tim>* val1, Generator<_tim>* val2)
      : value1(val1), value2(val2) { };

TimeDifference::~TimeDifference()
{
   delete this->value1;
   delete this->value2;
}

_per TimeDifference::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

NegatedPeriod::NegatedPeriod(Generator<_per>* val) 
   : value(val){ };

NegatedPeriod::~NegatedPeriod()
{
   delete this->value;
}

_per NegatedPeriod::getValue()
{
   return -(this->value->getValue());
}

}
