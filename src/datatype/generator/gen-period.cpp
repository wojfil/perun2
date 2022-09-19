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

_per PeriodUnit::getValue()
{
   const _tnum n = toTimeNumber(value->getValue());
   return _per(n, unit);
};

_per PeriodAddition::getValue()
{
   return value1->getValue() + value2->getValue();
};

_per PeriodSubtraction::getValue()
{
   return value1->getValue() - value2->getValue();
};

_per TimeDifference::getValue()
{
   return value1->getValue() - value2->getValue();
};

_per NegatedPeriod::getValue()
{
   return -(value->getValue());
};

}
