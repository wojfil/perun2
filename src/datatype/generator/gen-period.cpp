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

#include "gen-period.hpp"

namespace perun2::gen
{


PeriodUnit::PeriodUnit(p_genptr<p_num>& val, Period::PeriodUnit un)
   : value(std::move(val)), unit(un) { };

p_per PeriodUnit::getValue()
{
   const p_num num = this->value->getValue();
   if (num.isNaN()) {
      return p_per();
   }

   const p_tnum n = toTimeNumber(num);
   return p_per(n, this->unit);
}

PeriodAddition::PeriodAddition(p_genptr<p_per>& val1, p_genptr<p_per>& val2)
   : BinaryOperation<p_per>(val1, val2) { }

p_per PeriodAddition::getValue()
{
   return this->value1->getValue() + this->value2->getValue();
}

PeriodSubtraction::PeriodSubtraction(p_genptr<p_per>& val1, p_genptr<p_per>& val2)
   : BinaryOperation<p_per>(val1, val2) { }

p_per PeriodSubtraction::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

TimeDifference::TimeDifference(p_genptr<p_tim>& val1, p_genptr<p_tim>& val2)
   : value1(std::move(val1)), value2(std::move(val2)) { };

p_per TimeDifference::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

NegatedPeriod::NegatedPeriod(p_genptr<p_per>& val) 
   : value(std::move(val)) { };

p_per NegatedPeriod::getValue()
{
   return -(this->value->getValue());
}

}
