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

#include "gen-number.h"
#include "gen-generic.h"


namespace perun2::gen
{

Negation::Negation(_genptr<_num>& val) 
   : UnaryOperation<_num>(val) { };

Addition::Addition(_genptr<_num>& val1, _genptr<_num>& val2)
   : BinaryOperation<_num>(val1, val2) { };

Subtraction::Subtraction(_genptr<_num>& val1, _genptr<_num>& val2)
   : BinaryOperation<_num>(val1, val2) { };
      
Multiplication::Multiplication(_genptr<_num>& val1, _genptr<_num>& val2)
   : BinaryOperation<_num>(val1, val2) { };

Division::Division(_genptr<_num>& val1, _genptr<_num>& val2)
   : BinaryOperation<_num>(val1, val2) { };

Modulo::Modulo(_genptr<_num>& val1, _genptr<_num>& val2)
   : BinaryOperation<_num>(val1, val2) { };

_num Negation::getValue()
{
   return -this->value->getValue();
}

_num Addition::getValue()
{
   return this->value1->getValue() + this->value2->getValue();
}

_num Subtraction::getValue()
{
   return this->value1->getValue() - this->value2->getValue();
}

_num Multiplication::getValue()
{
   return this->value1->getValue() * this->value2->getValue();
}

_num Division::getValue()
{
   return this->value1->getValue() / this->value2->getValue();
}

_num Modulo::getValue()
{
   return this->value1->getValue() % this->value2->getValue();
}

TimeMember::TimeMember(_genptr<_tim>& tim, const Period::PeriodUnit& pu) 
   : time(std::move(tim)), unit(pu) { };

_num TimeMember::getValue() 
{
   switch (this->unit) {
      case Period::u_Years:
         return _num(static_cast<_nint>(this->time->getValue().year));
      case Period::u_Months:
         return _num(static_cast<_nint>(this->time->getValue().month));
      case Period::u_Weeks:
         return _num(static_cast<_nint>(this->time->getValue().getWeekDay()));
      case Period::u_Days:
         return _num(static_cast<_nint>(this->time->getValue().day));
      case Period::u_Hours:
         return _num(static_cast<_nint>(this->time->getValue().hour));
      case Period::u_Minutes:
         return _num(static_cast<_nint>(this->time->getValue().minute));
      case Period::u_Seconds:
         return _num(static_cast<_nint>(this->time->getValue().second));
   }

   return _num();
}

TimeMemberAtIndex::TimeMemberAtIndex(_genptr<_tim>& tim, const Period::PeriodUnit& pu) 
   : time(std::move(tim)), unit(pu) { };

_num TimeMemberAtIndex::getValue() 
{
   switch (this->unit) {
      case Period::u_Years:
         return _num(static_cast<_nint>(this->time->getValue().year));
      case Period::u_Months:
         return _num(static_cast<_nint>(this->time->getValue().month));
      case Period::u_Weeks:
         return _num(static_cast<_nint>(this->time->getValue().getWeekDay()));
      case Period::u_Days:
         return _num(static_cast<_nint>(this->time->getValue().day));
      case Period::u_Hours:
         return _num(static_cast<_nint>(this->time->getValue().hour));
      case Period::u_Minutes:
         return _num(static_cast<_nint>(this->time->getValue().minute));
      case Period::u_Seconds:
         return _num(static_cast<_nint>(this->time->getValue().second));
   }

   return _num();
}


}
