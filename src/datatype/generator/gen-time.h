/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_TIME_H
#define GEN_TIME_H

#include "../generator.h"
#include "gen-generic.h"
#include "../../var.h"


namespace uro::gen
{

struct IncreasedTime : Generator<_tim>
{
public:
   IncreasedTime() = delete;
   IncreasedTime(_genptr<_tim>& tim, _genptr<_per>& per);
   _tim getValue() override;

private:
   _genptr<_tim> time;
   _genptr<_per> period;
};


struct DecreasedTime : Generator<_tim>
{
public:
   DecreasedTime() = delete;
   DecreasedTime(_genptr<_tim>& tim, _genptr<_per>& per);
   _tim getValue() override;

private:
   _genptr<_tim> time;
   _genptr<_per> period;
};


struct v_Now : Variable<_tim>
{
public:
   v_Now() : Variable<_tim>(VarType::vt_Special) { };
   _tim getValue() override;
};


struct v_Today : Variable<_tim>
{
public:
   v_Today() : Variable<_tim>(VarType::vt_Special) { };
   _tim getValue() override;
};


struct v_Yesterday : Variable<_tim>
{
public:
   v_Yesterday() : Variable<_tim>(VarType::vt_Special) { };
   _tim getValue() override;
};


struct v_Tomorrow : Variable<_tim>
{
public:
   v_Tomorrow() : Variable<_tim>(VarType::vt_Special) { };
   _tim getValue() override;
};


struct TimeDate : UnaryOperation<_tim>
{
public:
   TimeDate() = delete;
   TimeDate(_genptr<_tim>& val);
   _tim getValue() override;
};


struct TimeDateAtIndex : Generator<_tim>
{
public:
   TimeDateAtIndex() = delete;
   TimeDateAtIndex(_genptr<_tim>& tim);
   _tim getValue() override;

private:
   _genptr<_tim> time;
};

}

#endif /* GEN_TIME_H */

