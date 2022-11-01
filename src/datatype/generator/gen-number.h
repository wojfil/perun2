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

#ifndef GEN_NUMBER_H
#define GEN_NUMBER_H

#include "../generator.h"
#include "gen-generic.h"


namespace uro::gen
{

struct Negation : UnaryOperation<_num>
{
   Negation() = delete;
   Negation(_genptr<_num>& val);
   _num getValue() override;
};


struct Addition : BinaryOperation<_num>
{
   Addition() = delete;
   Addition(_genptr<_num>& val1, _genptr<_num>& val2);
   _num getValue() override;
};


struct Subtraction : BinaryOperation<_num>
{
   Subtraction() = delete;
   Subtraction(_genptr<_num>& val1, _genptr<_num>& val2);
   _num getValue() override;
};


struct Multiplication : BinaryOperation<_num>
{
   Multiplication() = delete;
   Multiplication(_genptr<_num>& val1, _genptr<_num>& val2);
   _num getValue() override;
};


struct Division : BinaryOperation<_num>
{
   Division() = delete;
   Division(_genptr<_num>& val1, _genptr<_num>& val2);
   _num getValue() override;
};


struct Modulo : BinaryOperation<_num>
{
   Modulo() = delete;
   Modulo(_genptr<_num>& val1, _genptr<_num>& val2);
   _num getValue() override;
};


struct TimeMember : Generator<_num>
{
public:
   TimeMember() = delete;
   TimeMember(_genptr<_tim>& tim, const Period::PeriodUnit& pu);
   _num getValue() override;

protected:
   _genptr<_tim> time;
   const Period::PeriodUnit unit;
};


struct TimeMemberAtIndex : Generator<_num>
{
public:
   TimeMemberAtIndex() = delete;
   TimeMemberAtIndex(_genptr<_tim>& tim, const Period::PeriodUnit& pu);
   _num getValue() override;

protected:
   _genptr<_tim> time;
   const Period::PeriodUnit unit;
};


}

#endif /* GEN_NUMBER_H */

