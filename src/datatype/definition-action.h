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

#include "primitives.h"
#include <memory>


namespace perun2
{

// Definition can contain some special actions
// that are executed on recursive tranversation of directories 
// for example, some files are selected in the File Explorer when we know
// that we do not enter certain directory ever again
struct DefinitionAction
{
public:
   virtual void reset() = 0;
   virtual void onDirectoryEnter() = 0;
   virtual void onDirectoryExit() = 0;
   virtual void add(const p_str& path) = 0;
   virtual void finish() = 0;

};

typedef std::unique_ptr<DefinitionAction> p_daptr;

}
