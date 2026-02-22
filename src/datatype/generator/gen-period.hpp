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

#pragma once

#include "../generator.hpp"
#include "gen-generic.hpp"


namespace perun2::gen
{

struct PeriodUnit : Generator<p_per>
{
public:
   PeriodUnit() = delete;
   PeriodUnit(p_genptr<p_num>& val, Period::PeriodUnit un);
   p_per getValue() override;

private:
   p_genptr<p_num> value;
   const Period::PeriodUnit unit;
};


struct PeriodAddition : BinaryOperation<p_per>
{
public:
   PeriodAddition() = delete;
   PeriodAddition(p_genptr<p_per>& val1, p_genptr<p_per>& val2);
   p_per getValue() override;
};


struct PeriodSubtraction : BinaryOperation<p_per>
{
public:
   PeriodSubtraction() = delete;
   PeriodSubtraction(p_genptr<p_per>& val1, p_genptr<p_per>& val2);
   p_per getValue() override;
};


struct TimeDifference : Generator<p_per>
{
public:
   TimeDifference() = delete;
   TimeDifference(p_genptr<p_tim>& val1, p_genptr<p_tim>& val2);
   p_per getValue() override;

private:
   p_genptr<p_tim> value1;
   p_genptr<p_tim> value2;
};


struct NegatedPeriod : Generator<p_per>
{
public:
   NegatedPeriod() = delete;
   NegatedPeriod(p_genptr<p_per>& val);
   p_per getValue() override;

private:
   p_genptr<p_per> value;
};

}
