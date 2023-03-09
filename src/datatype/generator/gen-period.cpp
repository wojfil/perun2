/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "gen-period.h"

namespace perun2::gen
{


PeriodUnit::PeriodUnit(_genptr<_num>& val, Period::PeriodUnit un)
   : value(std::move(val)), unit(un) { };

_per PeriodUnit::getValue()
{
   const _tnum n = toTimeNumber(this->value->getValue());
   return _per(n, this->unit);
}

PeriodAddition::PeriodAddition(_genptr<_per>& val1, _genptr<_per>& val2)
   : BinaryOperation<_per>(val1, val2) { }

_per PeriodAddition::getValue()
{
   return this->value1->getValue() + this->value2->getValue();
}

PeriodSubtraction::PeriodSubtraction(_genptr<_per>& val1, _genptr<_per>& val2)
   : BinaryOperation<_per>(val1, val2) { }

_per PeriodSubtraction::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

TimeDifference::TimeDifference(_genptr<_tim>& val1, _genptr<_tim>& val2)
   : value1(std::move(val1)), value2(std::move(val2)) { };

_per TimeDifference::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

NegatedPeriod::NegatedPeriod(_genptr<_per>& val) 
   : value(std::move(val)) { };

_per NegatedPeriod::getValue()
{
   return -(this->value->getValue());
}

}
