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


namespace perun2
{

enum PathType
{
   Absolute,
   Relative
};

enum PathSegmentType
{
   NotFinal,
   Final
};

}


// os-common    -> LINUX + MAC + WINDOWS
// os-linux     -> LINUX
// os-mac       -> MAC
// os-posix     -> LINUX + MAC
// os-windows   -> WINDOWS


#ifdef _WIN32
   #include "os-common.h"
   #include "os-windows.h"
#elif __APPLE__
   #include "os-common.h"
   #include "os-posix.h"
   #include "os-mac.h"
#elif __linux__
   #include "os-common.h"
   #include "os-posix.h"
   #include "os-linux.h"
#else
   #error "Unsupported platform"
#endif

