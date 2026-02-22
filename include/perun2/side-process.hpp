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

#include "datatype/primitives.hpp"
#include <Windows.h>


namespace perun2
{


enum Python3State
{
    P3_Unknown,
    P3_Installed,
    P3_NotInstalled,
    P3_DifferentVersionThan3
};


struct SideProcess
{
public:
    void terminate();

    p_bool running = false;
    PROCESS_INFORMATION info;
};

}
