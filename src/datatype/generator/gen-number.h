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
   Negation(Generator<_num>* val);
   _num getValue() override;
};


struct Addition : BinaryOperation<_num>
{
   Addition() = delete;
   Addition(Generator<_num>* val1, Generator<_num>* val2);
   _num getValue() override;
};


struct Subtraction : BinaryOperation<_num>
{
   Subtraction() = delete;
   Subtraction(Generator<_num>* val1, Generator<_num>* val2);
   _num getValue() override;
};


struct Multiplication : BinaryOperation<_num>
{
   Multiplication() = delete;
   Multiplication(Generator<_num>* val1, Generator<_num>* val2);
   _num getValue() override;
};


struct Division : BinaryOperation<_num>
{
   Division() = delete;
   Division(Generator<_num>* val1, Generator<_num>* val2);
   _num getValue() override;
};


struct Modulo : BinaryOperation<_num>
{
   Modulo() = delete;
   Modulo(Generator<_num>* val1, Generator<_num>* val2);
   _num getValue() override;
};


struct TimeMember : Generator<_num>
{
public:
   TimeMember() = delete;
   TimeMember(Generator<_tim>* tim, const Period::PeriodUnit& pu);
   ~TimeMember();
   _num getValue() override;

protected:
   Generator<_tim>* time;
   const Period::PeriodUnit unit;
};


struct TimeMemberAtIndex : Generator<_num>
{
public:
   TimeMemberAtIndex() = delete;
   TimeMemberAtIndex(Generator<_tim>* tim, const Period::PeriodUnit& pu);
   ~TimeMemberAtIndex();
   _num getValue() override;

protected:
   Generator<_tim>* time;
   const Period::PeriodUnit unit;
};


}

#endif /* GEN_NUMBER_H */

