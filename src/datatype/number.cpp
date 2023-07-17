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

#include <cmath>
#include "number.h"
#include "../exception.h"
#include "primitives.h"
#include "strings.h"
#include "numbers.h"


namespace perun2
{

Number::Number()
   : value(NINT_ZERO), isDouble(false) { }

Number::Number(const p_int val)
   : value(static_cast<p_nint>(val)), isDouble(false) { }

Number::Number(const p_nint val)
   : value(val), isDouble(false) { }

Number::Number(const p_ndouble val)
   : value(val), isDouble(true) { }


// convert a number into string
// if is double-precision, cut off ending zeros from the fractional part
p_str Number::toString() const
{
   if (isDouble)  {
      p_ostream stream;
      stream << std::fixed << value.d;
      const p_str str = stream.str();
      const p_size len = str.size();

      for (p_int i = len - 1; i >= 0; i--)  {
         const p_char ch = str[i];
         if (ch != CHAR_0) {
            if (ch == CHAR_DOT) {
               return i == 0
                  ? toStr(CHAR_0)
                  : str.substr(0, i);
            }
            else {
               for (p_size j = 0; j < len; j++) {
                  if (str[j] == CHAR_DOT) {
                     return str.substr(0, i + 1);
                  }
               }

               return str;
            }
         }
      }

      return toStr(CHAR_0);
   }
   else {
      return toStr(value.i);
   }
}

p_nint Number::toInt() const
{
   return isDouble
      ? static_cast<p_nint>(value.d)
      : value.i;
}

p_bool Number::isZero() const
{
   return isDouble
      ? (value.d == NDOUBLE_ZERO)
      : (value.i == NINT_ZERO);
}

p_bool Number::isOne() const
{
   return isDouble
      ? (value.d == NDOUBLE_ONE)
      : (value.i == NINT_ONE);
}

p_bool Number::isMinusOne() const
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
      p_ndouble d = value.i + num.value.d;
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
      p_ndouble d = value.i - num.value.d;
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
      p_ndouble d = value.i * num.value.d;
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
         const p_ndouble d = value.i / num.value.d;
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
            const p_ndouble d = static_cast<p_ndouble>(value.i) / static_cast<p_ndouble>(num.value.i);
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
         const p_ndouble d = std::fmod(value.d, num.value.d);
         value.d = d;
      }
      else {
         const p_ndouble d = std::fmod(value.i, num.value.d);
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
         const p_ndouble d = std::fmod(value.d, num.value.i);
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
         const p_ndouble v = value.d + num.value.d;
         return Number(v);
      }
      else {
         const p_ndouble v = value.d + num.value.i;
         return Number(v);
      }
   }
   else
   {
      if (num.isDouble) {
         const p_ndouble v = value.i + num.value.d;
         return Number(v);
      }
      else {
         const p_nint v = value.i + num.value.i;
         return Number(v);
      }
   }
}

Number Number::operator - (const Number& num) const
{
   if (isDouble) {
      if (num.isDouble) {
         const p_ndouble v = value.d - num.value.d;
         return Number(v);
      }
      else {
         const p_ndouble v = value.d - num.value.i;
         return Number(v);
      }
   }
   else
   {
      if (num.isDouble) {
         const p_ndouble v = value.i - num.value.d;
         return Number(v);
      }
      else {
         const p_nint v = value.i - num.value.i;
         return Number(v);
      }
   }
}

Number Number::operator * (const Number& num) const
{
   if (isDouble) {
      if (num.isDouble) {
         const p_ndouble v = value.d * num.value.d;
         return Number(v);
      }
      else {
         const p_ndouble v = value.d * num.value.i;
         return Number(v);
      }
   }
   else
   {
      if (num.isDouble) {
         const p_ndouble v = value.i * num.value.d;
         return Number(v);
      }
      else {
         const p_nint v = value.i * num.value.i;
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
         const p_ndouble v = value.d / num.value.d;
         return Number(v);
      }
      else {
         const p_ndouble v = value.i / num.value.d;
         return Number(v);
      }
   }
   else
   {
      if (num.value.i == NINT_ZERO) {
         throw RuntimeError::divisionByZero();
      }
      if (isDouble) {
         const p_ndouble v = value.d / num.value.i;
         return Number(v);
      }
      else {
         if (value.i % num.value.i == NINT_ZERO) {
            return Number(value.i / num.value.i);
         }
         else {
            const p_ndouble d = static_cast<p_ndouble>(value.i) / static_cast<p_ndouble>(num.value.i);
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
         const p_ndouble v = std::fmod(value.d, num.value.d);
         return Number(v);
      }
      else {
         const p_ndouble v = std::fmod(value.i, num.value.d);
         return Number(v);
      }
   }
   else {
      if (num.value.i == NINT_ZERO) {
         throw RuntimeError::moduloByZero();
      }
      if (isDouble) {
         const p_ndouble v = std::fmod(value.d, num.value.i);
         return Number(v);
      }
      else {
         const p_nint v = value.i % num.value.i;
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

p_bool Number::operator == (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i == num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d == num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d == static_cast<p_ndouble>(num.value.i);
   }
   return static_cast<p_ndouble>(value.i) == num.value.d;
}

// below are duplicates for the rest of comparisons:

p_bool Number::operator != (const Number& num) const
{
   if (!isDouble && !num.isDouble) {
      return value.i != num.value.i;
   }
   else if (isDouble && num.isDouble) {
      return value.d != num.value.d;
   }
   else if (isDouble && !num.isDouble) {
      return value.d != static_cast<p_ndouble>(num.value.i);
   }
   return static_cast<p_ndouble>(value.i) != num.value.d;
}

p_bool Number::operator < (const Number& num) const
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

p_bool Number::operator > (const Number& num) const
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

p_bool Number::operator <= (const Number& num) const
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

p_bool Number::operator >= (const Number& num) const
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
