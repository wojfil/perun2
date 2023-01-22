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
#include "strings.h"
#include "numbers.h"


namespace uro
{

Number::Number()
   : value(NINT_ZERO), isDouble(false) { }

Number::Number(const _int& val)
   : value(static_cast<_nint>(val)), isDouble(false) { }

Number::Number(const _nint& val)
   : value(val), isDouble(false) { }

Number::Number(const _ndouble& val)
   : value(val), isDouble(true) { }


// convert a number into string
// if is double-precision, cut off ending zeros from the fractional part
_str Number::toString() const
{
   if (isDouble)  {
      const _str str = toStr(value.d);
      const _size len = str.size();

      for (_int i = len - 1; i >= 0; i--)  {
         const _char& ch = str[i];
         if (ch != DIGIT_0) {
            if (ch == CHAR_DOT) {
               return i == 0
                  ? STRING_0
                  : str.substr(0, i);
            }
            else {
               for (_size j = 0; j < len; j++) {
                  if (str[j] == CHAR_DOT) {
                     return str.substr(0, i + 1);
                  }
               }

               return str;
            }
         }
      }

      return STRING_0;
   }
   else {
      return toStr(value.i);
   }
}

_nint Number::toInt() const
{
   return isDouble
      ? static_cast<_nint>(value.d)
      : value.i;
}

_bool Number::isZero() const
{
   return isDouble
      ? (value.d == NDOUBLE_ZERO)
      : (value.i == NINT_ZERO);
}

_bool Number::isOne() const
{
   return isDouble
      ? (value.d == NDOUBLE_ONE)
      : (value.i == NINT_ONE);
}

_bool Number::isMinusOne() const
{
   return isDouble
      ? (value.d == NDOUBLE_MINUS_ONE)
      : (value.i == NINT_MINUS_ONE);
}

void Number::makeOpposite()
{
   if (isDouble) {
      value.d *= NDOUBLE_MINUS_ONE;
   }
   else {
      value.i *= NINT_MINUS_ONE;
   }
}

void Number::setToZero()
{
   if (isDouble) {
      value.d = NDOUBLE_ZERO;
   }
   else {
      value.i = NINT_ZERO;
   }
}

void Number::setToMinusOne()
{
   if (isDouble) {
      value.d = NDOUBLE_MINUS_ONE;
   }
   else {
      value.i = NINT_MINUS_ONE;
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
   if (!num.isDouble && num.value.i == NINT_ZERO) {
      value.i = NINT_ZERO;
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
      if (num.value.d == NDOUBLE_ZERO) {
         throw RuntimeError::divisionByZero();
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
      if (num.value.i == NINT_ZERO) {
         throw RuntimeError::divisionByZero();
      }

      if (isDouble) {
         value.d /= num.value.i;
      }
      else {
         if (value.i % num.value.i == NINT_ZERO) {
            value.i /= num.value.i;
         }
         else {
            const _ndouble d = static_cast<_ndouble>(value.i) / static_cast<_ndouble>(num.value.i);
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
      if (num.value.d == NDOUBLE_ZERO) {
         throw RuntimeError::moduloByZero();
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
      if (num.value.i == NINT_ZERO) {
         throw RuntimeError::moduloByZero();
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
      value.d += NDOUBLE_ONE;
   }
   else {
      value.i++;
   }
   return *this;
}

Number& Number::operator -- (int)
{
   if (isDouble) {
      value.d -= NDOUBLE_ONE;
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
      if (num.value.d == NDOUBLE_ZERO) {
         throw RuntimeError::divisionByZero();
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
      if (num.value.i == NINT_ZERO) {
         throw RuntimeError::divisionByZero();
      }
      if (isDouble) {
         const _ndouble v = value.d / num.value.i;
         return Number(v);
      }
      else {
         if (value.i % num.value.i == NINT_ZERO) {
            return Number(value.i / num.value.i);
         }
         else {
            const _ndouble d = static_cast<_ndouble>(value.i) / static_cast<_ndouble>(num.value.i);
            return Number(d);
         }
      }
   }
}

Number Number::operator % (const Number& num) const
{
   if (num.isDouble) {
      if (num.value.d == NDOUBLE_ZERO) {
         throw RuntimeError::moduloByZero();
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
      if (num.value.i == NINT_ZERO) {
         throw RuntimeError::moduloByZero();
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

_bool Number::operator == (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i == num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d == num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d == static_cast<_ndouble>(num.value.i);
   }
   return static_cast<_ndouble>(value.i) == num.value.d;
}

// below are duplicates for the rest of comparisons:

_bool Number::operator != (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i != num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d != num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d != static_cast<_ndouble>(num.value.i);
   }
   return static_cast<_ndouble>(value.i) != num.value.d;
}

_bool Number::operator < (const Number& num) const
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

_bool Number::operator > (const Number& num) const
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

_bool Number::operator <= (const Number& num) const
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

_bool Number::operator >= (const Number& num) const
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

}
