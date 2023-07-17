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

#ifndef TERMINATOR_H_INCLUDED
#define TERMINATOR_H_INCLUDED

#include "datatype/primitives.h"
#include <unordered_set>


namespace perun2
{

struct pp_perun2;

// Terminator keeps track of every initialized instance of Perun2
// it overrides the default Ctrl+C termination signal
// when this event happens, all Perun2 instances are stopped softly (as their commands are designed to be atomic)
// works only, if Terminator has been initialized
struct Terminator
{
public:
   Terminator() = delete;

   static void init();
   static void addPtr(pp_perun2* p2);
   static void removePtr(pp_perun2* p2);

private:
   static p_bool initialized;
   static std::unordered_set<pp_perun2*> processes;
   static p_int HandlerRoutine(p_ulong dwCtrlType);
};

void initTerminator();

}

#endif // TERMINATOR_H_INCLUDED
