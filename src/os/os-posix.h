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

#ifndef OS_POSIX_H
#define OS_POSIX_H

#include "../datatype/datatype.h"


namespace perun2
{

// default file path separator
// in Linux and MacOS, this separator is / and the 'wrong separator' is \  
p_constexpr p_char OS_SEPARATOR = CHAR_SLASH;
p_constexpr p_char OS_WRONG_SEPARATOR = CHAR_BACKSLASH;


}


#endif /* OS_POSIX_H */
