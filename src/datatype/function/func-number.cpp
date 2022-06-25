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

#include "func-number.h"
#include "../../exception.h"
#include "func-bool.h"
#include "../../util.h"
#include "../../uroboros.h"
#include "../../os.h"
#include "../math.h"
#include <math.h>
#include <sstream>
#include <cmath>


_num F_Absolute::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      if (n.value.d < 0L)
         n.value.d *= -1L;
   }
   else {
      if (n.value.i < 0LL)
         n.value.i *= -1LL;
   }

   return n;
}


_num F_Ceil::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const _nint val = (_nint) ceil(n.value.d);
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


F_Count::~F_Count()
{
   deleteVectorPtr(tlists);
   deleteVectorPtr(nlists);
   deleteVectorPtr(lists);
   deleteVectorPtr(defs);
}


_num F_Count::getValue()
{
   _nint n = 0LL;

   if (countTlists != 0) {
      for (_size i = 0; i < countTlists; i++) {
         n += (*tlists)[i]->getValue().size();
      }
   }
   if (countNlists != 0) {
      for (_size i = 0; i < countNlists; i++) {
         n += (*nlists)[i]->getValue().size();
      }
   }
   if (countLists != 0) {
      for (_size i = 0; i < countLists; i++) {
         n += (*lists)[i]->getValue().size();
      }
   }
   if (countDefs != 0) {
      for (_size i = 0; i < countDefs; i++) {
         _def*& def = (*defs)[i];

         while (def->hasNext()) {
            if (!g_running) {
               def->reset();
               break;
            }
            n++;
         }
      }
   }

   return _num(n);
}


_num F_CountUnitDef::getValue()
{
   _nint n = 0LL;

   while (definition->hasNext()) {
      if (!g_running) {
         definition->reset();
         break;
      }
      n++;
   }

   return _num(n);
}


_num F_CountInside::getValue()
{
   const _str& v = os_trim(value->getValue());
   if (v.empty() || os_isInvaild(v)) {
      return _num(-1LL);
   }

   const _str path = os_join(g_location.value, v);
   if (!os_directoryExists(path)) {
      return _num(-1LL);
   }

   const _str prevLoc = g_location.value;
   g_location.value = path;
   _nint n = 0LL;

   while (definition->hasNext()) {
      if (!g_running) {
         definition->reset();
         break;
      }
      n++;
   }

   g_location.value = prevLoc;
   return _num(n);
}


_num F_Floor::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const _nint val = (_nint) floor(n.value.d);
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


_num F_Length::getValue()
{
   return _num((_nint)arg1->getValue().size());
}


_num F_Number::getValue()
{
   const _str s = this->arg1->getValue();
   if (!isNumber(s)) {
      return _num(0LL);
   }

   _int dotIndex = s.find(L'.');

   if (dotIndex == _str::npos) {
      try {
         _nint i = std::stoll(s);
         return _num(i);
      }
      catch (...) {
         throw UroRuntimeException(str(L"number '", s,
            L"' is too big to be stored in the memory"));
      }
   }
   else {
      std::wstringstream ss(s);
      _ndouble n;
      ss >> n;
      return _num(n);
   }
}


_num F_Round::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const _nint val = (_nint) round(n.value.d);
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
      const _ndouble& base = n1.value.d;

      if (n2.isDouble) {
         return doublePower(base, n2.value.d);
      }
      else {
         _nint exp = n2.value.i;

         // optimization for common cases
         switch (exp) {
            case -1LL: {
               return _num(1L / ((_ndouble) base));
            }
            case 0LL: {
               return _num(1LL);
            }
            case 1LL: {
               return _num(base);
            }
            case 2LL: {
               return _num(base * base);
            }
            case 3LL: {
               return _num(base * base * base);
            }
            case 4LL: {
               const _ndouble b2 = base * base;
               return _num(b2 * b2);
            }
         }

         return doublePower(base, (_ndouble)exp);
      }
   }
   else {
      _nint base = n1.value.i;

      if (base == 0LL) {
         if (n2.isDouble) {
            if (n2.value.d == 0L) {
               return _num(1LL);
            }
            else if (n2.value.d < 0L) {
               throw UroRuntimeException(L"result of exponentiation cannot be expressed");
            }
            else {
               return _num(0LL);
            }
         }
         else {
            if (n2.value.i == 0LL) {
               return _num(1LL);
            }
            else if (n2.value.i <= 0LL) {
               throw UroRuntimeException(L"result of exponentiation cannot be expressed");
            }
            else {
               return _num(0LL);
            }
         }
      }

      if (n2.isDouble) {
         const _ndouble& exp = n2.value.d;
         return doublePower((_ndouble)base, exp);
      }
      else {
         _nint exp = n2.value.i;

         // optimization for common cases
         switch (exp) {
            case -1LL: {
               return _num(1L / ((_ndouble) base));
            }
            case 0LL: {
               return _num(1LL);
            }
            case 1LL: {
               return _num(base);
            }
            case 2LL: {
               return _num(base * base);
            }
            case 3LL: {
               return _num(base * base * base);
            }
            case 4LL: {
               const _nint b2 = base * base;
               return _num(b2 * b2);
            }
         }

         // if the number is inver
         _boo inv = false;
         if (exp < 0LL) {
            exp *= -1LL;
            inv = true;
         }

         _boo neg = false;
         if (base < 0LL) {
            base *= -1LL;
            neg = (exp % 2LL == 1LL);
         }

         _nint result = 1;
         while (true) {
            if (exp & 1) {
               result *= base;
            }
            exp >>= 1;
            if (!exp) {
               break;
            }
            base *= base;
         }

         if (inv) {
            return _num((neg ? -1L : 1L) / ((_ndouble)result));
         }
         else {
            return _num(neg ? (-result) : result);
         }
      }
   }
}

_num F_Power::doublePower(const _ndouble& base, const _ndouble& exp)
{
   const _ndouble v = pow(base, exp);

   if (isnan(v)) {
      throw UroRuntimeException(L"result of exponentiation cannot be expressed");
   }

   return _num(v);
}

_num F_Sqrt::getValue()
{
   const _num n = arg1->getValue();

   if (n.isDouble) {
      if (n.value.d == 0L) {
         return _num(0LL);
      }
      else if (n.value.d == 1L) {
         return _num(1LL);
      }
      else if (n.value.d < 0L) {
         throw UroRuntimeException(str(L"square root of a negative number ",
            n.toString()));
      }

      return _num(sqrt(n.value.d));
   }
   else {
      if (n.value.i == 0LL || n.value.i == 1LL) {
         return n;
      }
      else if (n.value.i < 0LL) {
         throw UroRuntimeException(str(L"square root of a negative number ",
            n.toString()));
      }

      // look for perfect squares:
      _nint left = 1LL;
      _nint right = n.value.i;

      while (left <= right) {
         const _nint mid = (left + right) / 2;

         if (mid * mid == n.value.i) {
            return _num(mid);
         }

         if (mid * mid < n.value.i) {
            left = mid + 1;
         }
         else {
            right = mid - 1;
         }
      }

      return _num((_ndouble)sqrt(n.value.i));
   }
}


_num F_Sign::getValue()
{
   const _num n = arg1->getValue();

   if (n.isDouble) {
      if (n.value.d > 0L)
         return _num(1LL);
      else if (n.value.d < 0L)
         return _num(-1LL);
      else
         return _num(0LL);
   }
   else {
      if (n.value.i > 0LL)
         return _num(1LL);
      else if (n.value.i < 0LL)
         return _num(-1LL);
      else
         return _num(0LL);
   }
}


_num F_Truncate::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      const _nint val = (_nint) trunc(n.value.d);
      n.value.i = val;
      n.isDouble = false;
   }

   return n;
}


_num F_Random::getValue()
{
   return g_math->randomDouble();
}


_num F_RandomNumber::getValue()
{
   _num n = arg1->getValue();

   if (n.isDouble) {
      n.value.d *= g_math->randomDouble();
      return n;
   }
   else {
      return _num(g_math->randomInt(n.value.i - 1LL));
   }
}


_num F_FromBinary::getValue()
{
   const _str baseString = arg1->getValue();
   _size len = baseString.size();

   if (len == 0) {
      return _num(0LL);
   }

   _nint result = 0LL;
   _size i = 0;
   _boo negative = false;

   if (baseString[0] == L'-') {
      if (len == 1) {
         return _num(0LL);
      }
      negative = true;
      i++;
      if (len > 62) {
         throw UroRuntimeException(str(L"number '", baseString,
            L"' is too big to be stored in the memory"));
      }
   }
   else {
      if (len > 63) {
         throw UroRuntimeException(str(L"number '", baseString,
            L"' is too big to be stored in the memory"));
      }
   }

   for (; i < len; i++) {
      result = result << 1;

      switch (baseString[i]) {
         case L'0': {
            break;
         }
         case L'1': {
            result++;
            break;
         }
         default: {
            return _num(0LL);
         }
      }
   }

   if (negative) {
      result *= -1;
   }

   return _num(result);
}


_num F_FromHex::getValue()
{
   const _str baseString = arg1->getValue();
   if (baseString.size() == 0) {
      return _num(0LL);
   }

   _nint x;
   std::wstringstream ss;
   ss << std::hex << baseString;
   ss >> x;

   return _num(x);
}
