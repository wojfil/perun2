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

#ifndef GEN_NUMBER_H
#define GEN_NUMBER_H

#include "../generator.h"
#include "gen-generic.h"


namespace perun2::gen
{

struct Negation : UnaryOperation<p_num>
{
   Negation() = delete;
   Negation(_genptr<p_num>& val);
   p_num getValue() override;
};


struct Addition : BinaryOperation<p_num>
{
   Addition() = delete;
   Addition(_genptr<p_num>& val1, _genptr<p_num>& val2);
   p_num getValue() override;
};


struct Subtraction : BinaryOperation<p_num>
{
   Subtraction() = delete;
   Subtraction(_genptr<p_num>& val1, _genptr<p_num>& val2);
   p_num getValue() override;
};


struct Multiplication : BinaryOperation<p_num>
{
   Multiplication() = delete;
   Multiplication(_genptr<p_num>& val1, _genptr<p_num>& val2);
   p_num getValue() override;
};


struct Division : BinaryOperation<p_num>
{
   Division() = delete;
   Division(_genptr<p_num>& val1, _genptr<p_num>& val2);
   p_num getValue() override;
};


struct Modulo : BinaryOperation<p_num>
{
   Modulo() = delete;
   Modulo(_genptr<p_num>& val1, _genptr<p_num>& val2);
   p_num getValue() override;
};


struct TimeMember : Generator<p_num>
{
public:
   TimeMember() = delete;
   TimeMember(_genptr<p_tim>& tim, const Period::PeriodUnit& pu);
   p_num getValue() override;

protected:
   _genptr<p_tim> time;
   const Period::PeriodUnit unit;
};

}

#endif /* GEN_NUMBER_H */

