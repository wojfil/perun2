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

#ifndef DEFINITION_H
#define DEFINITION_H

#include "generator.h"
#include "primitives.h"
#include <windows.h>
#include <memory>


namespace perun2
{

struct FileContext;

// a lazy evaluated collection of strings
// return next element on demand
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
      // some reflection for parsing
      return nullptr;
   };

protected:
   _str value;
};

typedef std::unique_ptr<Definition> _defptr;

}

#endif /* DEFINITION_H */
