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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "primitives.h"
#include <memory>


namespace perun2
{

// generator is a structure
// that generates a new instance of a certain data type
// when its method getValue() is called
template <typename T>
struct Generator
{
public:

   virtual T getValue() = 0;

   virtual _bool isConstant() const
   {
      // simple reflection for parsing
      // detect generators that return deterministic values
      // and use this knowledge in optimizations
      return false;
   };
};


template <typename T>
using _genptr = std::unique_ptr<Generator<T>>;

}

#endif /* GENERATOR_H */
