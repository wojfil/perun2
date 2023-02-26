/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEFINITION_H
#define DEFINITION_H

#include "generator.h"
#include "primitives.h"
#include <windows.h>
#include <memory>


namespace uro
{

struct FileContext;

// this data structure is crucial for Uroboros2
// as it represents a lazy evaluated collection of strings
// this is what file system elements (as their names and paths) usually are
struct Definition : Generator<_str>
{
public:
    virtual _bool hasNext() = 0;
    virtual void reset() = 0;

   _str getValue() override
   {
      return value;
   }

   virtual FileContext* getFileContext()
   {
      // designed to be overriden
      return nullptr;
   };

protected:
   _str value;
};

typedef std::unique_ptr<Definition> _defptr;

}

#endif /* DEFINITION_H */
