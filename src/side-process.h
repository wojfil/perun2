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

#ifndef SIDE_PROCESS_H_INCLUDED
#define SIDE_PROCESS_H_INCLUDED

#include "datatype/primitives.h"
#include <Windows.h>


namespace uro
{

struct SideProcess
{
public:
   _bool running = false;
   PROCESS_INFORMATION info;
   
};

}

#endif // SIDE_PROCESS_H_INCLUDED
