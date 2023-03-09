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

#ifndef GEN_PERIOD_H
#define GEN_PERIOD_H

#include "../generator.h"
#include "gen-generic.h"


namespace perun2::gen
{

struct PeriodUnit : Generator<_per>
{
public:
   PeriodUnit() = delete;
   PeriodUnit(_genptr<_num>& val, Period::PeriodUnit un);
   _per getValue() override;

private:
   _genptr<_num> value;
   const Period::PeriodUnit unit;
};


struct PeriodAddition : BinaryOperation<_per>
{
public:
   PeriodAddition() = delete;
   PeriodAddition(_genptr<_per>& val1, _genptr<_per>& val2);
   _per getValue() override;
};


struct PeriodSubtraction : BinaryOperation<_per>
{
public:
   PeriodSubtraction() = delete;
   PeriodSubtraction(_genptr<_per>& val1, _genptr<_per>& val2);
   _per getValue() override;
};


struct TimeDifference : Generator<_per>
{
public:
   TimeDifference() = delete;
   TimeDifference(_genptr<_tim>& val1, _genptr<_tim>& val2);
   _per getValue() override;

private:
   _genptr<_tim> value1;
   _genptr<_tim> value2;
};


struct NegatedPeriod : Generator<_per>
{
public:
   NegatedPeriod() = delete;
   NegatedPeriod(_genptr<_per>& val);
   _per getValue() override;

private:
   _genptr<_per> value;
};

}

#endif /* GEN_PERIOD_H */
