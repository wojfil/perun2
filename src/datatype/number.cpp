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

#include <cmath>
#include "number.h"
#include "../exception.h"
#include "primitives.h"


Number::Number()
   : value(0LL), isDouble(false) { }

Number::Number(_int val)
   : value((_nint)val), isDouble(false) { }

Number::Number(_nint val)
   : value(val), isDouble(false) { }

Number::Number(_ndouble val)
   : value(val), isDouble(true) { }


// convert a number into string
// if is double-precision, cut off ending zeros from the fractional part
_str Number::toString() const
{
   if (isDouble)  {
      const _str str = std::to_wstring(value.d);
      const _int len = str.size();
      _int i;

      for (i = len - 1; i >= 0; i--)  {
         const _char& ch = str[i];
         if (ch != L'0') {
            if (ch == L'.') {
               return i == 0
                  ? L"0"
                  : str.substr(0, i);
            }
            else {
               return str.substr(0, i + 1);
            }
         }
      }

      return L"0";
   }
   else {
      return toStr(value.i);
   }
}

_nint Number::toInt() const
{
   return isDouble
      ? (_nint)value.d
      : value.i;
}

_boo Number::isZero() const
{
   return isDouble
      ? (value.d == 0L)
      : (value.i == 0LL);
}

_boo Number::isMinusOne() const
{
   return isDouble
      ? (value.d == -1L)
      : (value.i == -1LL);
}

void Number::makeOpposite()
{
   if (isDouble) {
      value.d *= -1L;
   }
   else {
      value.i *= -1LL;
   }
}

Number& Number::operator += (const Number& num)
{
   if (isDouble)  {
      if (num.isDouble)  {
         value.d += num.value.d;
      }
      else {
         value.d += num.value.i;
      }
      return *this;
   }
   if (num.isDouble)  {
      _ndouble d = value.i + num.value.d;
      value.d = d;
      isDouble = true;
   }
   else {
      value.i += num.value.i;
   }
   return *this;
}

Number& Number::operator -= (const Number& num)
{
   if (isDouble) {
      if (num.isDouble)  {
         value.d -= num.value.d;
      }
      else {
         value.d -= num.value.i;
      }
      return *this;
   }
   if (num.isDouble) {
      _ndouble d = value.i - num.value.d;
      value.d = d;
      isDouble = true;
   }
   else {
      value.i -= num.value.i;
   }
   return *this;
}

Number& Number::operator *= (const Number& num)
{
   if (!num.isDouble && num.value.i == 0LL) {
      value.i = 0LL;
      isDouble = false;
      return *this;
   }

   if (isDouble) {
      if (num.isDouble) {
         value.d *= num.value.d;
      }
      else {
         value.d *= num.value.i;
      }
      return *this;
   }

   if (num.isDouble) {
      _ndouble d = value.i * num.value.d;
      value.d = d;
      isDouble = true;
   }
   else {
      value.i *= num.value.i;
   }
   return *this;
}

Number& Number::operator /= (const Number& num)
{
   if (num.isDouble) {
      if (num.value.d == 0L) {
         throw UroRuntimeException(L"division by zero");
      }

      if (isDouble) {
         value.d /= num.value.d;
      }
      else {
         const _ndouble d = value.i / num.value.d;
         value.d = d;
         isDouble = true;
      }
   }
   else
   {
      if (num.value.i == 0LL) {
         throw UroRuntimeException(L"division by zero");
      }

      if (isDouble) {
         value.d /= num.value.i;
      }
      else {
         if (value.i % num.value.i == 0) {
            value.i /= num.value.i;
         }
         else {
            const _ndouble d = (_ndouble)value.i / (_ndouble)num.value.i;
            value.d = d;
            isDouble = true;
         }
      }
   }

   return *this;
}

Number& Number::operator %= (const Number& num)
{
   if (num.isDouble) {
      if (num.value.d == 0L) {
         throw UroRuntimeException(L"modulo by zero");
      }

      if (isDouble) {
         const _ndouble d = std::fmod(value.d, num.value.d);
         value.d = d;
      }
      else {
         const _ndouble d = std::fmod(value.i, num.value.d);
         value.d = d;
         isDouble = true;
      }
   }
   else
   {
      if (num.value.i == 0LL) {
         throw UroRuntimeException(L"modulo by zero");
      }

      if (isDouble) {
         const _ndouble d = std::fmod(value.d, num.value.i);
         value.d = d;
      }
      else {
         value.i %= num.value.i;
      }
   }
   return *this;
}

Number& Number::operator ++ (int)
{
   if (isDouble) {
      value.d += 1L;
   }
   else {
      value.i++;
   }
   return *this;
}

Number& Number::operator -- (int)
{
   if (isDouble) {
      value.d -= 1L;
   }
   else {
      value.i--;
   }
   return *this;
}

Number Number::operator + (const Number& num) const
{
   if (isDouble) {
      if (num.isDouble) {
         const _ndouble v = value.d + num.value.d;
         return Number(v);
      }
      else {
         const _ndouble v = value.d + num.value.i;
         return Number(v);
      }
   }
   else
   {
      if (num.isDouble) {
         const _ndouble v = value.i + num.value.d;
         return Number(v);
      }
      else {
         const _nint v = value.i + num.value.i;
         return Number(v);
      }
   }
}

Number Number::operator - (const Number& num) const
{
   if (isDouble) {
      if (num.isDouble) {
         const _ndouble v = value.d - num.value.d;
         return Number(v);
      }
      else {
         const _ndouble v = value.d - num.value.i;
         return Number(v);
      }
   }
   else
   {
      if (num.isDouble) {
         const _ndouble v = value.i - num.value.d;
         return Number(v);
      }
      else {
         const _nint v = value.i - num.value.i;
         return Number(v);
      }
   }
}

Number Number::operator * (const Number& num) const
{
   if (isDouble) {
      if (num.isDouble) {
         const _ndouble v = value.d * num.value.d;
         return Number(v);
      }
      else {
         const _ndouble v = value.d * num.value.i;
         return Number(v);
      }
   }
   else
   {
      if (num.isDouble) {
         const _ndouble v = value.i * num.value.d;
         return Number(v);
      }
      else {
         const _nint v = value.i * num.value.i;
         return Number(v);
      }
   }
}

Number Number::operator / (const Number& num) const
{
   if (num.isDouble) {
      if (num.value.d == 0L) {
         throw UroRuntimeException(L"division by zero");
      }
      if (isDouble) {
         const _ndouble v = value.d / num.value.d;
         return Number(v);
      }
      else {
         const _ndouble v = value.i / num.value.d;
         return Number(v);
      }
   }
   else
   {
      if (num.value.i == 0LL) {
         throw UroRuntimeException(L"division by zero");
      }
      if (isDouble) {
         const _ndouble v = value.d / num.value.i;
         return Number(v);
      }
      else {
         if (value.i % num.value.i == 0) {
            return Number(value.i / num.value.i);
         }
         else {
            const _ndouble d = (_ndouble)value.i / (_ndouble)num.value.i;
            return Number(d);
         }
      }
   }
}

Number Number::operator % (const Number& num) const
{
   if (num.isDouble) {
      if (num.value.d == 0L) {
         throw UroRuntimeException(L"modulo by zero");
      }
      if (isDouble) {
         const _ndouble v = std::fmod(value.d, num.value.d);
         return Number(v);
      }
      else {
         const _ndouble v = std::fmod(value.i, num.value.d);
         return Number(v);
      }
   }
   else {
      if (num.value.i == 0LL) {
         throw UroRuntimeException(L"modulo by zero");
      }
      if (isDouble) {
         const _ndouble v = std::fmod(value.d, num.value.i);
         return Number(v);
      }
      else {
         const _nint v = value.i % num.value.i;
         return Number(v);
      }
   }
}

Number Number::operator - () const
{
   return isDouble
      ? Number(-value.d)
      : Number(-value.i);
}

bool Number::operator == (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i == num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d == num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d == (_ndouble)num.value.i;
   }
   return (_ndouble)value.i == num.value.d;
}

// below are duplicates for the rest of comparisons:

bool Number::operator != (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i != num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d != num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d != ((_ndouble)num.value.i);
   }
   return ((_ndouble)value.i) != num.value.d;
}

bool Number::operator < (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i < num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d < num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d < num.value.i;
   }
   return value.i < num.value.d;
}

bool Number::operator > (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i > num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d > num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d > num.value.i;
   }
   return value.i > num.value.d;
}

bool Number::operator <= (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i <= num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d <= num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d <= num.value.i;
   }
   return value.i <= num.value.d;
}

bool Number::operator >= (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i >= num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d >= num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d >= num.value.i;
   }
   return value.i >= num.value.d;
}
