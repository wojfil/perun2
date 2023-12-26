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

#ifndef GEN_TIME_H
#define GEN_TIME_H

#include "../generator.h"
#include "gen-generic.h"
#include "../../var.h"


namespace perun2::gen
{

struct IncreasedTime : Generator<p_tim>
{
public:
   IncreasedTime() = delete;
   IncreasedTime(p_genptr<p_tim>& tim, p_genptr<p_per>& per);
   p_tim getValue() override;

private:
   p_genptr<p_tim> time;
   p_genptr<p_per> period;
};


struct DecreasedTime : Generator<p_tim>
{
public:
   DecreasedTime() = delete;
   DecreasedTime(p_genptr<p_tim>& tim, p_genptr<p_per>& per);
   p_tim getValue() override;

private:
   p_genptr<p_tim> time;
   p_genptr<p_per> period;
};


struct v_Now : Variable<p_tim>
{
public:
   v_Now() : Variable<p_tim>(VarType::vt_Special) { };
   p_tim getValue() override;
};


struct v_Today : Variable<p_tim>
{
public:
   v_Today() : Variable<p_tim>(VarType::vt_Special) { };
   p_tim getValue() override;
};


struct v_Yesterday : Variable<p_tim>
{
public:
   v_Yesterday() : Variable<p_tim>(VarType::vt_Special) { };
   p_tim getValue() override;
};


struct v_Tomorrow : Variable<p_tim>
{
public:
   v_Tomorrow() : Variable<p_tim>(VarType::vt_Special) { };
   p_tim getValue() override;
};


struct TimeDate : UnaryOperation<p_tim>
{
public:
   TimeDate() = delete;
   TimeDate(p_genptr<p_tim>& val);
   p_tim getValue() override;
};

}

#endif /* GEN_TIME_H */

