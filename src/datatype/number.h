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

#ifndef NUMBER_H
#define NUMBER_H

#include "primitives.h"


namespace perun2
{

enum NumberState
{
   NaN,
   Int,
   Double
};


union NumberValue
{
   p_nint i;
   p_ndouble d;

   NumberValue(const p_nint i) : i(i) {};
   NumberValue(const p_ndouble d) : d(d) {};
};


#define P_NaN Number(NumberState::NaN)


struct Number
{
   Number();
   Number(const NumberState st);
   Number(const p_int val);
   Number(const p_nint val);
   Number(const p_ndouble val);

   p_str toString() const;
   p_nint toInt() const;
   p_bool isZero() const;
   p_bool isOne() const;
   p_bool isMinusOne() const;
   p_bool isNaN() const;
   void makeOpposite();
   void setToNaN();
   void setToZero();
   void setToMinusOne();
   void turnToIntIfPossible();

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

   p_bool operator == (const Number& num) const;
   p_bool operator != (const Number& num) const;
   p_bool operator < (const Number& num) const;
   p_bool operator > (const Number& num) const;
   p_bool operator <= (const Number& num) const;
   p_bool operator >= (const Number& num) const;

   p_bool operator == (const p_nint num) const;
   p_bool operator != (const p_nint num) const;
   p_bool operator < (const p_nint num) const;
   p_bool operator > (const p_nint num) const;
   p_bool operator <= (const p_nint num) const;
   p_bool operator >= (const p_nint num) const;

   // number consists of the value (int or double)
   // and the current state
   NumberValue value;
   NumberState state;
};


}

#endif /* NUMBER_H */
