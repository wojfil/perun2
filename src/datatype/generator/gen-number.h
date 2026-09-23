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

#include "../generator.h"
#include "gen-generic.h"


namespace perun2::gen
{

struct Negation : UnaryOperation<p_num>
{
   Negation() = delete;
   Negation(p_genptr<p_num>& val);
   p_num getValue() override;
};


struct Addition : BinaryOperation<p_num>
{
   Addition() = delete;
   Addition(p_genptr<p_num>& val1, p_genptr<p_num>& val2);
   p_num getValue() override;
};


struct Subtraction : BinaryOperation<p_num>
{
   Subtraction() = delete;
   Subtraction(p_genptr<p_num>& val1, p_genptr<p_num>& val2);
   p_num getValue() override;
};


struct Multiplication : BinaryOperation<p_num>
{
   Multiplication() = delete;
   Multiplication(p_genptr<p_num>& val1, p_genptr<p_num>& val2);
   p_num getValue() override;
};


struct Division : BinaryOperation<p_num>
{
   Division() = delete;
   Division(p_genptr<p_num>& val1, p_genptr<p_num>& val2);
   p_num getValue() override;
};


struct Modulo : BinaryOperation<p_num>
{
   Modulo() = delete;
   Modulo(p_genptr<p_num>& val1, p_genptr<p_num>& val2);
   p_num getValue() override;
};


struct TimeMember : Generator<p_num>
{
public:
   TimeMember() = delete;
   TimeMember(p_genptr<p_tim>& tim, const Period::PeriodUnit pu);
   p_num getValue() override;

protected:
   p_genptr<p_tim> time;
   const Period::PeriodUnit unit;
};

}
