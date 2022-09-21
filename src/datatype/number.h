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

#ifndef NUMBER_H
#define NUMBER_H

#include "primitives.h"


namespace uro
{

union NumberValue
{
   _nint i;
   _ndouble d;

   NumberValue(const _nint& i) : i(i) {};
   NumberValue(const _ndouble& d) : d(d) {};
};


struct Number
{
   Number();
   Number(const _int& val);
   Number(const _nint& val);
   Number(const _ndouble& val);

   _str toString() const;
   _nint toInt() const;
   _boo isZero() const;
   _boo isMinusOne() const;
   void makeOpposite();
   void setToZero();
   void setToMinusOne();

   Number& operator += (const Number& num);
   Number& operator -= (const Number& num);
   Number& operator *= (const Number& num);
   Number& operator /= (const Number& num);
   Number& operator %= (const Number& num);

   Number& operator ++ (int);
   Number& operator -- (int);

   Number operator + (const Number& num) const;
   Number operator - (const Number& num) const;
   Number operator * (const Number& num) const;
   Number operator / (const Number& num) const;
   Number operator % (const Number& num) const;

   Number operator - () const;

   _boo operator == (const Number& num) const;
   _boo operator != (const Number& num) const;
   _boo operator < (const Number& num) const;
   _boo operator > (const Number& num) const;
   _boo operator <= (const Number& num) const;
   _boo operator >= (const Number& num) const;

   // number consists of a value (int or double)
   // and a boolean flag indicating current state
   NumberValue value;
   _boo isDouble;
};

}

#endif /* NUMBER_H */
