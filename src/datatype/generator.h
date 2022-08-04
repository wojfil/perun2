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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "primitives.h"

// generator is a structure
// that generates a new instance of a certain data type
// when is called
template <typename T>
struct Generator
{
public:

   virtual T getValue() = 0;

   virtual ~Generator() { };

   _boo isConstant() const
   {
      // artificially implemented simple reflection for parsing
      // constant generators are detected
      // and used for optimizations in some situations
      return false;
   };
};


#endif /* GENERATOR_H */
