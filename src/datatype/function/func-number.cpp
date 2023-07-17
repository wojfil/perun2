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

#include "func-number.h"
#include "../../exception.h"
#include "func-bool.h"
#include "../../util.h"
#include "../../lexer.h"
#include "../../perun2.h"
#include "../../os.h"
#include "../math.h"
#include <math.h>
#include <sstream>
#include <cmath>


namespace perun2::func
{

_num F_Absolute::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      if (n.value.d < NDOUBLE_ZERO) {
         n.value.d *= NDOUBLE_MINUS_ONE;
      }
   }
   else {
      if (n.value.i < NINT_ZERO) {
         n.value.i *= NINT_MINUS_ONE;
      }
   }

   return n;
}


_num F_Ceil::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const p_nint val = static_cast<p_nint>(ceil(n.value.d));
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


_num F_CountInside::getValue()
{
   if (!this->fileContext->v_exists->value || !this->fileContext->v_isdirectory->value) {
      return false;
   }

   this->locContext->loadData(this->fileContext->trimmed);
   p_nint n = NINT_ZERO;

   while (definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         definition->reset();
         break;
      }
      n++;
   }

   return _num(n);
}


_num F_Floor::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const p_nint val = static_cast<p_nint>(floor(n.value.d));
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


_num F_Length::getValue()
{
   return _num(static_cast<p_nint>(arg1->getValue().size()));
}


_num F_Number::getValue()
{
   const p_str s = this->arg1->getValue();
   if (!isNumber(s)) {
      return _num();
   }

   if (s.find(CHAR_DOT) == p_str::npos) {
      try {
         const p_nint i = std::stoll(s);
         return _num(i);
      }
      catch (...) {
         throw RuntimeError::numberTooBig(s);
      }
   }
   else {
      return _num(stringToDouble(s));
   }
}


_num F_Round::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const p_nint val = static_cast<p_nint>(round(n.value.d));
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


_num F_Power::getValue()
{
   const _num n1 = arg1->getValue();
   const _num n2 = arg2->getValue();

   if (n1.isDouble) {
      const p_ndouble base = n1.value.d;

      if (n2.isDouble) {
         return doublePower(base, n2.value.d);
      }
      else {
         p_nint exp = n2.value.i;

         switch (exp) {
            case NINT_MINUS_ONE: {
               return _num(NDOUBLE_ONE / static_cast<p_ndouble>(base));
            }
            case NINT_ZERO: {
               return _num(NINT_ONE);
            }
            case NINT_ONE: {
               return _num(base);
            }
            case NINT_TWO: {
               return _num(base * base);
            }
            case NINT_THREE: {
               return _num(base * base * base);
            }
            case NINT_FOUR: {
               const p_ndouble b2 = base * base;
               return _num(b2 * b2);
            }
         }

         return doublePower(base, static_cast<p_ndouble>(exp));
      }
   }
   else {
      p_nint base = n1.value.i;

      if (base == NINT_ZERO) {
         if (n2.isDouble) {
            if (n2.value.d == NDOUBLE_ZERO) {
               return _num(NINT_ONE);
            }
            else if (n2.value.d < NDOUBLE_ZERO) {
               throw RuntimeError::wrongResultOfExponentiation();
            }
            else {
               return _num();
            }
         }
         else {
            if (n2.value.i == NINT_ZERO) {
               return _num(NINT_ONE);
            }
            else if (n2.value.i <= NINT_ZERO) {
               throw RuntimeError::wrongResultOfExponentiation();
            }
            else {
               return _num();
            }
         }
      }

      if (n2.isDouble) {
         const p_ndouble exp = n2.value.d;
         return doublePower(static_cast<p_ndouble>(base), exp);
      }
      else {
         p_nint exp = n2.value.i;

         switch (exp) {
            case NINT_MINUS_ONE: {
               return _num(NDOUBLE_ONE / static_cast<p_ndouble>(base));
            }
            case NINT_ZERO: {
               return _num(NINT_ONE);
            }
            case NINT_ONE: {
               return _num(base);
            }
            case NINT_TWO: {
               return _num(base * base);
            }
            case NINT_THREE: {
               return _num(base * base * base);
            }
            case NINT_FOUR: {
               const p_nint b2 = base * base;
               return _num(b2 * b2);
            }
         }

         // if the number is inver
         p_bool inv = false;
         if (exp < NINT_ZERO) {
            exp *= NINT_MINUS_ONE;
            inv = true;
         }

         p_bool neg = false;
         if (base < NINT_ZERO) {
            base *= NINT_MINUS_ONE;
            neg = (exp % NINT_TWO == NINT_ONE);
         }

         p_nint result = NINT_ONE;
         while (true) {
            if (exp & NINT_ONE) {
               result *= base;
            }
            exp >>= 1;
            if (!exp) {
               break;
            }
            base *= base;
         }

         if (inv) {
            return _num((neg ? NDOUBLE_MINUS_ONE : NDOUBLE_ONE) / static_cast<p_ndouble>(result));
         }
         else {
            return _num(neg ? (-result) : result);
         }
      }
   }
}

_num F_Power::doublePower(const p_ndouble base, const p_ndouble exp)
{
   const p_ndouble v = pow(base, exp);

   if (isnan(v)) {
      throw RuntimeError::wrongResultOfExponentiation();
   }

   return _num(v);
}

_num F_Sqrt::getValue()
{
   const _num n = arg1->getValue();

   if (n.isDouble) {
      if (n.value.d == NDOUBLE_ZERO) {
         return _num();
      }
      else if (n.value.d == NDOUBLE_ONE) {
         return _num(NINT_ONE);
      }
      else if (n.value.d < NDOUBLE_ZERO) {
         throw RuntimeError::squareRootOfNegativeNumber(n.toString());
      }

      return _num(sqrt(n.value.d));
   }
   else {
      if (n.value.i == NINT_ZERO || n.value.i == NINT_ONE) {
         return n;
      }
      else if (n.value.i < NINT_ZERO) {
         throw RuntimeError::squareRootOfNegativeNumber(n.toString());
      }

      // look for perfect squares:
      p_nint left = NINT_ONE;
      p_nint right = n.value.i;

      while (left <= right) {
         const p_nint mid = (left + right) / NINT_TWO;

         if (mid * mid == n.value.i) {
            return _num(mid);
         }

         if (mid * mid < n.value.i) {
            left = mid + NINT_ONE;
         }
         else {
            right = mid - NINT_ONE;
         }
      }

      return _num(static_cast<p_ndouble>(sqrt(n.value.i)));
   }
}


_num F_Sign::getValue()
{
   const _num n = arg1->getValue();

   if (n.isDouble) {
      if (n.value.d > NDOUBLE_ZERO)
         return _num(NINT_ONE);
      else if (n.value.d < NDOUBLE_ZERO)
         return _num(NINT_MINUS_ONE);
      else
         return _num();
   }
   else {
      if (n.value.i > NINT_ZERO)
         return _num(NINT_ONE);
      else if (n.value.i < NINT_ZERO)
         return _num(NINT_MINUS_ONE);
      else
         return _num();
   }
}


_num F_Truncate::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const p_nint val = static_cast<p_nint>(trunc(n.value.d));
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


_num F_Random::getValue()
{
   return this->math.randomDouble();
}


_num F_RandomNumber::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      n.value.d *= this->math.randomDouble();
      return n;
   }
   else {
      return _num(this->math.randomInt(n.value.i - NINT_ONE));
   }
}


_num F_FromBinary::getValue()
{
   const p_str baseString = arg1->getValue();

   if (baseString.empty()) {
      return _num();
   }

   p_nint result = NINT_ZERO;
   p_size i = 0;
   p_bool negative = false;

   if (baseString[0] == CHAR_MINUS) {
      if (baseString.size() == 1) {
         return _num();
      }
      negative = true;
      i++;
      if (baseString.size() > BITS_IN_NINT - 2) {
         throw RuntimeError::numberTooBig(baseString);
      }
   }
   else {
      if (baseString.size() > BITS_IN_NINT - 1) {
         throw RuntimeError::numberTooBig(baseString);
      }
   }

   for (; i < baseString.size(); i++) {
      result = result << 1;

      switch (baseString[i]) {
         case CHAR_0: {
            break;
         }
         case CHAR_1: {
            result++;
            break;
         }
         default: {
            return _num();
         }
      }
   }

   if (negative) {
      result *= NINT_MINUS_ONE;
   }

   return _num(result);
}


_num F_FromHex::getValue()
{
   const p_str baseString = arg1->getValue();
   if (baseString.empty()) {
      return _num();
   }

   p_nint x;
   p_stream ss;
   ss << std::hex << baseString;
   ss >> x;

   return _num(x);
}


_num F_Count::getValue()
{
   p_nint result = NINT_ZERO;
   
   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         return NINT_MINUS_ONE;
      }
      result++;
   }

   return result;
}


_num F_ShiftMonth_Time::getValue()
{
   p_nint result = (static_cast<p_nint>(this->arg1->getValue().month) + this->arg2->getValue().toInt()) % NINT_MONTHS_IN_YEAR;

   if (result <= NINT_ZERO) {
      result += NINT_MONTHS_IN_YEAR;
   }

   return result;
}


_num F_ShiftMonth_Number::getValue()
{
   p_nint result = (this->arg1->getValue().toInt() + this->arg2->getValue().toInt()) % NINT_MONTHS_IN_YEAR;

   if (result <= NINT_ZERO) {
      result += NINT_MONTHS_IN_YEAR;
   }

   return result;
}


_num F_ShiftWeekDay_Time::getValue()
{
   p_nint result = (static_cast<p_nint>(this->arg1->getValue().getWeekDay()) + this->arg2->getValue().toInt()) % NINT_DAYS_IN_WEEK;

   if (result <= NINT_ZERO) {
      result += NINT_DAYS_IN_WEEK;
   }

   return result;
}


_num F_ShiftWeekDay_Number::getValue()
{
   p_nint result = (this->arg1->getValue().toInt() + this->arg2->getValue().toInt()) % NINT_DAYS_IN_WEEK;

   if (result <= NINT_ZERO) {
      result += NINT_DAYS_IN_WEEK;
   }

   return result;
}


}
