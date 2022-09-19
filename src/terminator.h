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

#ifndef TERMINATOR_H_INCLUDED
#define TERMINATOR_H_INCLUDED

#include "datatype/primitives.h"


namespace uro
{

struct Uroboros;

struct Terminator
{
public:
   Terminator(Uroboros* uro);

private:
   static Uroboros* uroboros;
   static _int HandlerRoutine(_ulong dwCtrlType);
};

}

#endif // TERMINATOR_H_INCLUDED
