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


_tim os_yesterday();
_tim os_tomorrow();
void os_sleepForMs(const _nint ms, p_perun2& p2);

void os_loadEmptyAttributes(FileContext& context);

// get values of filesystem variables:
_str os_extension(const _str& value);
_str os_fullname(const _str& value);
_str os_name(const _str& value);
_str os_parent(const _str& path);

_bool os_bothAreSeparators(const _char left, const _char right);
_str os_trim(const _str& path);
_str os_softTrim(const _str& value);
inline void os_escapeQuote(_str& path);

}

#endif /* OS_COMMON_H */
