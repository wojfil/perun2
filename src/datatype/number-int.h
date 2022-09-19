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

#ifndef NUMBER_INT_H_INCLUDED
#define NUMBER_INT_H_INCLUDED

#include "number.h"


namespace uro
{

// sometimes, Number is always an integer
// for example - index of loop iteration, depth of recursive element
// this struct provides optimizations

struct NumberInt : Number
{
   NumberInt();
   NumberInt(const _nint& val);

   _str toString() const;
   _boo isZero() const;
   _boo isMinusOne() const;
   void setToZero();

   NumberInt& operator ++ (int);
   NumberInt& operator -- (int);
   _boo operator == (const NumberInt& num) const;
   _boo operator != (const NumberInt& num) const;
   NumberInt& operator += (const _nint& num);
   NumberInt& operator -= (const _nint& num);
};

}

#endif // NUMBER_INT_H_INCLUDED
