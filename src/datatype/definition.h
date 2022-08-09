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

#ifndef DEFINITION_H
#define DEFINITION_H

#include "generator.h"
#include "primitives.h"
#include <windows.h>


// this data structure is the crucial for Uroboros
// it provides lazy evaluation
struct Definition : Generator<_str>
{
public:
    virtual _boo hasNext() = 0;
    virtual void reset() = 0;

   _str getValue() override {
      return value;
   }

   virtual _fdata* getDataPtr()
   {
      // artificially implemented simple reflection for parsing optimizations
      // by default, return nullptr
      // detect definitions, that produce _fdata as a side effect
      // that data is used later in other places
      // as a result, we do not have to load the same data again from the file system
      return nullptr;
   };

protected:
   _str value;
};


#endif /* DEFINITION_H */
