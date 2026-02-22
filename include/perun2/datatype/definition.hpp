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

#pragma once

#include "definition-action.hpp"
#include "generator.hpp"
#include <memory>


namespace perun2
{

struct FileContext;

// a lazy evaluated collection of strings
// returns next element on demand
struct Definition : Generator<p_str>
{
public:
   virtual p_bool hasNext() = 0;
   virtual void reset() = 0;

   p_str getValue() override;

   // reflection for parsing
   virtual FileContext* getFileContext();

   // explained in 'definition-action.h'
   virtual p_bool setAction(p_daptr& act);

protected:
   p_str value;
   p_daptr action;
};

typedef std::unique_ptr<Definition> p_defptr;

}
