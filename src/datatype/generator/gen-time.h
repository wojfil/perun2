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

#ifndef GEN_TIME_H
#define GEN_TIME_H

#include "../generator.h"
#include "gen-generic.h"
#include "gen-number.h"


namespace uro::gen
{

struct IncreasedTime : Generator<_tim>
{
public:
   IncreasedTime (Generator<_tim>* tim, Generator<_per>* per)
      : time(tim), period(per) {};

   ~IncreasedTime() {
      delete time;
      delete period;
   }

   _tim getValue() override;

private:
   Generator<_tim>* time;
   Generator<_per>* period;
};


// the same, but with reversed operator
struct DecreasedTime : Generator<_tim>
{
public:
   DecreasedTime (Generator<_tim>* tim, Generator<_per>* per)
      : time(tim), period(per) {};

   ~DecreasedTime() {
      delete time;
      delete period;
   }

   _tim getValue() override;

private:
   Generator<_tim>* time;
   Generator<_per>* period;
};


struct v_Now : Generator<_tim>
{
public:
   v_Now() {};

   _tim getValue() override;
};


struct v_Today : Generator<_tim>
{
public:
   v_Today() {};

   _tim getValue() override;
};


struct v_Yesterday : Generator<_tim>
{
public:
   v_Yesterday() {};

   _tim getValue() override;
};


struct v_Tomorrow : Generator<_tim>
{
public:
   v_Tomorrow() {};

   _tim getValue() override;
};


struct TimeDate : UnaryOperation<_tim>
{
   TimeDate(Generator<_tim>* val) : UnaryOperation<_tim>(val) { };

   _tim getValue() override;
};


struct TimeDateAtIndex : Generator<_tim>, TimeMemberAtIndex
{
   TimeDateAtIndex(Generator<_tlist>* tims, Generator<_num>* in)
      : TimeMemberAtIndex(tims, in) {};
   _tim getValue() override;
};

}

#endif /* GEN_TIME_H */

