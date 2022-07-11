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
