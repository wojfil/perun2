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

#include "number-int.h"
#include "numbers.h"


namespace uro
{

NumberInt::NumberInt()
   : Number() { }

NumberInt::NumberInt(const _nint& val)
   : Number(val) { }

_str NumberInt::toString() const
{
   return toStr(value.i);
}

_nint NumberInt::toInt() const
{
   return value.i;
}

_bool NumberInt::isZero() const
{
   return value.i == NINT_ZERO;
}

_bool NumberInt::isOne() const
{
   return value.i == NINT_ONE;
}

_bool NumberInt::isMinusOne() const
{
   return value.i == NINT_MINUS_ONE;
}

void NumberInt::makeOpposite()
{
   value.i *= NINT_MINUS_ONE;
}

void NumberInt::setToZero()
{
   value.i = NINT_ZERO;
}

void NumberInt::setToMinusOne()
{
   value.i = NINT_MINUS_ONE;
}

NumberInt& NumberInt::operator ++ (int)
{
   value.i++;
   return *this;
}

NumberInt& NumberInt::operator -- (int)
{
   value.i--;
   return *this;
}

_bool NumberInt::operator == (const NumberInt& num) const
{
   return value.i == num.value.i;
}

_bool NumberInt::operator != (const NumberInt& num) const
{
   return value.i != num.value.i;
}

NumberInt& NumberInt::operator += (const _nint& num)
{
   value.i += num;
   return *this;
}

NumberInt& NumberInt::operator -= (const _nint& num)
{
   value.i -= num;
   return *this;
}

}
