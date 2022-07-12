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


NumberInt::NumberInt()
   : Number() { }

NumberInt::NumberInt(const _nint& val)
   : Number(val) { }

_str NumberInt::toString() const
{
   return toStr(value.i);
}

_boo NumberInt::isZero() const
{
   return value.i == 0LL;
}

_boo NumberInt::isMinusOne() const
{
   return value.i == -1LL;
}

void NumberInt::setToZero()
{
   value.i = 0LL;
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

_boo NumberInt::operator == (const NumberInt& num) const
{
   return value.i == num.value.i;
}

_boo NumberInt::operator != (const NumberInt& num) const
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
