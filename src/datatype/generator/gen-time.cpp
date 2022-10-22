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

#include "gen-time.h"
#include "gen-generic.h"
#include "../generator.h"
#include "../../os.h"


namespace uro::gen
{

IncreasedTime::IncreasedTime (Generator<_tim>* tim, Generator<_per>* per)
   : time(tim), period(per) { }

IncreasedTime::~IncreasedTime() 
{
   delete time;
   delete period;
}

_tim IncreasedTime::getValue()
{
   _tim t = this->time->getValue();
   t += this->period->getValue();
   return t;
}

DecreasedTime::DecreasedTime(Generator<_tim>* tim, Generator<_per>* per)
   : time(tim), period(per) {};

DecreasedTime::~DecreasedTime() 
{
   delete time;
   delete period;
}

_tim DecreasedTime::getValue()
{
   _tim t = this->time->getValue();
   t -= this->period->getValue();
   return t;
}

_tim v_Now::getValue()
{
   return os_now();
}

_tim v_Today::getValue()
{
   return os_today();
}

_tim v_Yesterday::getValue()
{
   return os_yesterday();
}

_tim v_Tomorrow::getValue()
{
   return os_tomorrow();
}

TimeDate::TimeDate(Generator<_tim>* val) 
   : UnaryOperation<_tim>(val) { }

_tim TimeDate::getValue() 
{
   return this->value->getValue().toDate();
}

TimeDateAtIndex::TimeDateAtIndex(Generator<_tim>* tim)
   : time(tim) { }

TimeDateAtIndex::~TimeDateAtIndex() 
{
   delete this->time;
}

_tim TimeDateAtIndex::getValue() 
{
   return this->time->getValue().toDate();
}

}
