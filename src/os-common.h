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

#ifndef OS_COMMON_H
#define OS_COMMON_H

#include "datatype\datatype.h"
#include "attribute.h"

// here are all OS functions, that are implemented the same for all operating systems

namespace perun2
{

// while sleeping
// check every 300 ms if the program received an interruption signal
_constexpr _nint OS_SLEEP_UNIT = NINT_300;



}

#endif /* OS_COMMON_H */
