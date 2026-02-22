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
   #include "os-common.hpp"
   #include "os-windows.hpp"
#elif __APPLE__
   #include "os-common.hpp"
   #include "os-posix.hpp"
   #include "os-mac.hpp"
#elif __linux__
   #include "os-common.hpp"
   #include "os-posix.hpp"
   #include "os-linux.hpp"
#else
   #error "Unsupported platform"
#endif

