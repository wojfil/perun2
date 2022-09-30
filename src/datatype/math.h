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

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include "primitives.h"
#include <random>


namespace uro
{

struct Math {

public:

   Math();
   void init();
   _ndouble randomDouble();
   _nint randomInt(const _nint& limit);
   void setAnyRandomDouble();

private:

   _bool anyRandomDouble;
   std::mt19937 generator;
   std::uniform_real_distribution<> doubleDist;
};

}

#endif // MATH_H_INCLUDED
