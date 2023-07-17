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

#include "gen-time.h"
#include "gen-generic.h"
#include "../generator.h"
#include "../../os.h"


namespace perun2::gen
{

IncreasedTime::IncreasedTime (_genptr<p_tim>& tim, _genptr<p_per>& per)
   : time(std::move(tim)), period(std::move(per)) { }

p_tim IncreasedTime::getValue()
{
   p_tim t = this->time->getValue();
   t += this->period->getValue();
   return t;
}

DecreasedTime::DecreasedTime(_genptr<p_tim>& tim, _genptr<p_per>& per)
   : time(std::move(tim)), period(std::move(per)) {};

p_tim DecreasedTime::getValue()
{
   p_tim t = this->time->getValue();
   t -= this->period->getValue();
   return t;
}

p_tim v_Now::getValue()
{
   return os_now();
}

p_tim v_Today::getValue()
{
   return os_today();
}

p_tim v_Yesterday::getValue()
{
   return os_yesterday();
}

p_tim v_Tomorrow::getValue()
{
   return os_tomorrow();
}

TimeDate::TimeDate(_genptr<p_tim>& val) 
   : UnaryOperation<p_tim>(val) { }

p_tim TimeDate::getValue() 
{
   return this->value->getValue().toDate();
}

}
