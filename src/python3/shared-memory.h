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

#include "../datatype/datatype.h"


namespace perun2::shm
{


p_constexpr p_char SHM_NAME_HEAD[] = L"Local\\SharedMemoryPerunPython";


//p_constexpr int SHM_SIZE = 12;  // 3 * 4 bytes (int32)

static p_bool sharedMemoryExists(const p_str& name);
p_int nextSharedMemoryId();


struct SharedMemory
{

};


}
