/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>
#include "number.hpp"
#include "../exception.hpp"
#include "primitives.hpp"
#include "text/strings.hpp"
#include "numbers.hpp"


namespace perun2
{

Number::Number()
   : value(NINT_ZERO), state(NumberState::Int) { }

Number::Number(const NumberState st)
   : state(st), value(NINT_ZERO)
{
   if (state == NumberState::Double) {
      value = NDOUBLE_ZERO;
   }
};

Number::Number(const p_int val)
   : value(static_cast<p_nint>(val)), state(NumberState::Int) { }

Number::Number(const p_nint val)
   : value(val), state(NumberState::Int) { }

Number::Number(const p_ndouble val)
   : value(val), state(NumberState::Double) { }


// convert a number into string
// if is double-precision, cut off ending zeros from the fractional part
p_str Number::toString() const
{
   if (state == NumberState::Int) {
      return toStr(value.i);
   }
   else if (isNaN()) {
      return STRING_PRINTABLE_NAN;
   }

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

p_nint Number::toInt() const
{
   if (state == NumberState::Int) {
      return value.i;
   }
   else if (isNaN()) {
      return NINT_ZERO;
   }

   return static_cast<p_nint>(value.d);
}

p_bool Number::isZero() const
{
   if (state == NumberState::Int) {
      return value.i == NINT_ZERO;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d == NDOUBLE_ZERO;
}

p_bool Number::isOne() const
{
   if (state == NumberState::Int) {
      return value.i == NINT_ONE;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d == NDOUBLE_ONE;
}

p_bool Number::isMinusOne() const
{
   if (state == NumberState::Int) {
      return value.i == NINT_MINUS_ONE;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d == NDOUBLE_MINUS_ONE;
}

p_bool Number::isNaN() const
{
   return state == NumberState::NaN;
}

void Number::makeOpposite()
{
   if (state == NumberState::Int) {
      value.i *= NINT_MINUS_ONE;
   }
   else if (state == NumberState::Double) {
      value.d *= NDOUBLE_MINUS_ONE;
   }
}

void Number::setToNaN()
{
   state = NumberState::NaN;
}

void Number::setToZero()
{
   value.i = NINT_ZERO;
   state = NumberState::Int;
}

void Number::setToMinusOne()
{
   value.i = NINT_MINUS_ONE;
   state = NumberState::Int;
}

void Number::turnToIntIfPossible()
{
   if (std::trunc(value.d) == value.d) {
      const p_ndouble d = value.d;
      value.i = static_cast<p_nint>(d);
      state = NumberState::Int;
   }
}

Number& Number::operator += (const Number& num)
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         value.i += num.value.i;
         return *this;
      }
      
      if (num.isNaN()) {
         setToNaN();
         return *this;
      }

      value.d = static_cast<p_ndouble>(value.i) + num.value.d;
      state = NumberState::Double;
      return *this;
   }

   if (isNaN()) {
      setToNaN();
      return *this;
   }

   if (num.state == NumberState::Int) {
      value.d += static_cast<p_ndouble>(num.value.i);
      return *this;
   }

   if (num.isNaN()) {
      setToNaN();
      return *this;
   }
   
   value.d += num.value.d;
   turnToIntIfPossible();
   return *this;
}

Number& Number::operator -= (const Number& num)
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         value.i -= num.value.i;
         return *this;
      }
      
      if (num.isNaN()) {
         setToNaN();
         return *this;
      }

      value.d = static_cast<p_ndouble>(value.i) - num.value.d;
      state = NumberState::Double;
      return *this;
   }

   if (isNaN()) {
      setToNaN();
      return *this;
   }

   if (num.state == NumberState::Int) {
      value.d -= static_cast<p_ndouble>(num.value.i);
      return *this;
   }

   if (num.isNaN()) {
      setToNaN();
      return *this;
   }
   
   value.d -= num.value.d;
   turnToIntIfPossible();
   return *this;
}

Number& Number::operator *= (const Number& num)
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         value.i *= num.value.i;
         return *this;
      }
      
      if (num.isNaN()) {
         setToNaN();
         return *this;
      }

      value.d = value.i * num.value.d;
      state = NumberState::Double;
      turnToIntIfPossible();
      return *this;
   }

   if (isNaN()) {
      setToNaN();
      return *this;
   }

   if (num.state == NumberState::Int) {
      value.d *= num.value.i;
      turnToIntIfPossible();
      return *this;
   }

   if (num.isNaN()) {
      setToNaN();
      return *this;
   }
   
   value.d *= num.value.d;
   turnToIntIfPossible();
   return *this;
}

Number& Number::operator /= (const Number& num)
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         if (num.value.i == NINT_ZERO) {
            setToNaN();
            return *this;
         }

         if (value.i % num.value.i == NINT_ZERO) {
            value.i /= num.value.i;
            return *this;
         }

         const p_ndouble d = static_cast<p_ndouble>(value.i) / static_cast<p_ndouble>(num.value.i);
         value.d = d;
         state = NumberState::Double;
         return *this;
      }
      
      if (num.isNaN() || num.value.d == NDOUBLE_ZERO) {
         setToNaN();
         return *this;
      }

      const p_ndouble d = value.i / num.value.d;
      value.d = d;
      state = NumberState::Double;
      turnToIntIfPossible();
      return *this;
   }

   if (isNaN()) {
      setToNaN();
      return *this;
   }

   if (num.state == NumberState::Int) {
      if (num.value.i == NINT_ZERO) {
         setToNaN();
         return *this;
      }

      value.d /= num.value.i;
      return *this;
   }

   if (num.isNaN() || num.value.d == NDOUBLE_ZERO) {
      setToNaN();
      return *this;
   }
   
   value.d /= num.value.d;
   turnToIntIfPossible();
   return *this;
}

Number& Number::operator %= (const Number& num)
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         if (num.value.i == NINT_ZERO) {
            setToNaN();
            return *this;
         }

         value.i %= num.value.i;
         return *this;
      }
      
      if (num.isNaN() || num.value.d == NDOUBLE_ZERO) {
         setToNaN();
         return *this;
      }

      const p_ndouble d = std::fmod(value.i, num.value.d);
      value.d = d;
      state = NumberState::Double;
      turnToIntIfPossible();
      return *this;
   }

   if (isNaN()) {
      setToNaN();
      return *this;
   }

   if (num.state == NumberState::Int) {
      if (num.value.i == NINT_ZERO) {
         setToNaN();
         return *this;
      }

      const p_ndouble d = std::fmod(value.d, num.value.i);
      value.d = d;
      return *this;
   }

   if (num.isNaN() || num.value.d == NDOUBLE_ZERO) {
      setToNaN();
      return *this;
   }
   
   const p_ndouble d = std::fmod(value.d, num.value.d);
   value.d = d;
   turnToIntIfPossible();
   return *this;
}

Number& Number::operator ++ (int)
{
   if (state == NumberState::Int) {
      value.i++;
   }
   else if (state == NumberState::Double) {
      value.d += NDOUBLE_ONE;
   }

   return *this;
}

Number& Number::operator -- (int)
{
   if (state == NumberState::Int) {
      value.i--;
   }
   else if (state == NumberState::Double) {
      value.d -= NDOUBLE_ONE;
   }

   return *this;
}

Number Number::operator + (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i + num.value.i;
      }
      
      if (num.isNaN()) {
         return P_NaN;
      }

      if (num.value.d == NDOUBLE_ZERO) {
         return P_NaN;
      }

      const p_ndouble v = static_cast<p_ndouble>(value.i) + num.value.d;
      return v;
   }

   if (isNaN()) {
      return P_NaN;
   }

   if (num.state == NumberState::Int) {
      const p_ndouble v = value.d + static_cast<p_ndouble>(num.value.i);
      return Number(v);
   }

   if (num.isNaN()) {
      return P_NaN;
   }
   
   Number v(value.d + num.value.d);
   v.turnToIntIfPossible();
   return v;
}

Number Number::operator - (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i - num.value.i;
      }
      
      if (num.isNaN()) {
         return P_NaN;
      }

      if (num.value.d == NDOUBLE_ZERO) {
         return P_NaN;
      }

      const p_ndouble v = static_cast<p_ndouble>(value.i) - num.value.d;
      return v;
   }

   if (isNaN()) {
      return P_NaN;
   }

   if (num.state == NumberState::Int) {
      const p_ndouble v = value.d - static_cast<p_ndouble>(num.value.i);
      return Number(v);
   }

   if (num.isNaN()) {
      return P_NaN;
   }
   
   Number v(value.d - num.value.d);
   v.turnToIntIfPossible();
   return v;
}

Number Number::operator * (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i * num.value.i;
      }
      
      if (num.isNaN()) {
         return P_NaN;
      }

      if (num.value.d == NDOUBLE_ZERO) {
         return P_NaN;
      }

      const p_ndouble n = value.i * num.value.d;
      Number v(n);
      v.turnToIntIfPossible();
      return v;
   }

   if (isNaN()) {
      return P_NaN;
   }

   if (num.state == NumberState::Int) {
      const p_ndouble n = value.d * num.value.i;
      Number v(n);
      v.turnToIntIfPossible();
      return Number(v);
   }

   if (num.isNaN()) {
      return P_NaN;
   }

   const p_ndouble n = value.d * num.value.d;
   Number v(n);
   v.turnToIntIfPossible();
   return v;
}

Number Number::operator / (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         if (num.value.i == NINT_ZERO) {
            return P_NaN;
         }

         if (value.i % num.value.i == NINT_ZERO) {
            return Number(value.i / num.value.i);
         }

         const p_ndouble d = static_cast<p_ndouble>(value.i) / static_cast<p_ndouble>(num.value.i);
         return Number(d);
      }
      
      if (num.isNaN()) {
         return P_NaN;
      }

      if (num.value.d == NDOUBLE_ZERO) {
         return P_NaN;
      }

      const p_ndouble n = value.i / num.value.d;
      Number v(n);
      v.turnToIntIfPossible();
      return v;
   }

   if (isNaN()) {
      return P_NaN;
   }

   if (num.state == NumberState::Int) {
      if (num.value.i == NINT_ZERO) {
         return P_NaN;
      }

      const p_ndouble n = value.d / num.value.i;
      return Number(n);
   }

   if (num.isNaN()) {
      return P_NaN;
   }
   
   if (num.value.d == NDOUBLE_ZERO) {
      return P_NaN;
   }

   const p_ndouble n = value.d / num.value.d;
   Number v(n);
   v.turnToIntIfPossible();
   return v;
}

Number Number::operator % (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         if (num.value.i == NINT_ZERO) {
            return P_NaN;
         }

         return value.i % num.value.i;
      }
      
      if (num.isNaN()) {
         return P_NaN;
      }

      if (num.value.d == NDOUBLE_ZERO) {
         return P_NaN;
      }

      const p_ndouble n = std::fmod(value.i, num.value.d);
      Number v(n);
      v.turnToIntIfPossible();
      return v;
   }

   if (isNaN()) {
      return P_NaN;
   }

   if (num.state == NumberState::Int) {
      if (num.value.i == NINT_ZERO) {
         return P_NaN;
      }

      return std::fmod(value.d, num.value.i);
   }

   if (num.isNaN()) {
      return P_NaN;
   }
   
   if (num.value.d == NDOUBLE_ZERO) {
      return P_NaN;
   }

   const p_ndouble n = std::fmod(value.d, num.value.d);
   Number v(n);
   v.turnToIntIfPossible();
   return v;
}

Number Number::operator - () const
{
   if (state == NumberState::Int) {
      return Number(-value.i);
   }

   if (state == NumberState::Double) {
      return Number(-value.d);
   }

   return P_NaN;
}

p_bool Number::operator == (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i == num.value.i;
      }
      
      if (num.isNaN()) {
         return false;
      }

      return static_cast<p_ndouble>(value.i) == num.value.d;
   }

   if (isNaN()) {
      return false;
   }

   if (num.state == NumberState::Int) {
      return value.d == static_cast<p_ndouble>(num.value.i);
   }

   if (num.isNaN()) {
      return false;
   }
   
   return value.d == num.value.d;
}

p_bool Number::operator != (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i != num.value.i;
      }
      
      if (num.isNaN()) {
         return false;
      }

      return static_cast<p_ndouble>(value.i) != num.value.d;
   }

   if (isNaN()) {
      return false;
   }

   if (num.state == NumberState::Int) {
      return value.d != static_cast<p_ndouble>(num.value.i);
   }

   if (num.isNaN()) {
      return false;
   }
   
   return value.d != num.value.d;
}

p_bool Number::operator < (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i < num.value.i;
      }
      
      if (num.isNaN()) {
         return false;
      }

      return static_cast<p_ndouble>(value.i) < num.value.d;
   }

   if (isNaN()) {
      return false;
   }

   if (num.state == NumberState::Int) {
      return value.d < static_cast<p_ndouble>(num.value.i);
   }

   if (num.isNaN()) {
      return false;
   }
   
   return value.d < num.value.d;
}

p_bool Number::operator > (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i > num.value.i;
      }
      
      if (num.isNaN()) {
         return false;
      }

      return static_cast<p_ndouble>(value.i) > num.value.d;
   }

   if (isNaN()) {
      return false;
   }

   if (num.state == NumberState::Int) {
      return value.d > static_cast<p_ndouble>(num.value.i);
   }

   if (num.isNaN()) {
      return false;
   }
   
   return value.d > num.value.d;
}

p_bool Number::operator <= (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i <= num.value.i;
      }
      
      if (num.isNaN()) {
         return false;
      }

      return static_cast<p_ndouble>(value.i) <= num.value.d;
   }

   if (isNaN()) {
      return false;
   }

   if (num.state == NumberState::Int) {
      return value.d <= static_cast<p_ndouble>(num.value.i);
   }

   if (num.isNaN()) {
      return false;
   }
   
   return value.d <= num.value.d;
}

p_bool Number::operator >= (const Number& num) const
{
   if (state == NumberState::Int) {
      if (num.state == NumberState::Int) {
         return value.i >= num.value.i;
      }
      
      if (num.isNaN()) {
         return false;
      }

      return static_cast<p_ndouble>(value.i) >= num.value.d;
   }

   if (isNaN()) {
      return false;
   }

   if (num.state == NumberState::Int) {
      return value.d >= static_cast<p_ndouble>(num.value.i);
   }

   if (num.isNaN()) {
      return false;
   }
   
   return value.d >= num.value.d;
}

p_bool Number::operator == (const p_nint num) const
{
   if (state == NumberState::Int) {
      return value.i == num;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d == static_cast<p_ndouble>(num);
}

p_bool Number::operator != (const p_nint num) const
{
   if (state == NumberState::Int) {
      return value.i != num;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d != static_cast<p_ndouble>(num);
}

p_bool Number::operator < (const p_nint num) const
{
   if (state == NumberState::Int) {
      return value.i < num;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d < static_cast<p_ndouble>(num);
}

p_bool Number::operator > (const p_nint num) const
{
   if (state == NumberState::Int) {
      return value.i > num;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d > static_cast<p_ndouble>(num);
}

p_bool Number::operator <= (const p_nint num) const
{
   if (state == NumberState::Int) {
      return value.i <= num;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d <= static_cast<p_ndouble>(num);
}

p_bool Number::operator >= (const p_nint num) const
{
   if (state == NumberState::Int) {
      return value.i >= num;
   }
   else if (isNaN()) {
      return false;
   }

   return value.d >= static_cast<p_ndouble>(num);
}

}
