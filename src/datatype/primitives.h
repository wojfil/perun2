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

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cstdlib>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>


namespace perun2
{

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
   #define p_constexpr              inline constexpr
   #define p_const                  inline static const
#else
   #define p_constexpr              constexpr
   #define p_const                  static const
#endif

#define p_cout                      std::wcout
#define p_cin                       std::wcin

typedef bool                        p_bool;
typedef int32_t                     p_int;
typedef long unsigned int           p_ulong;
typedef std::size_t                 p_size;
typedef wchar_t                     p_char;
typedef std::wstring                p_str;
typedef std::vector<p_str>          p_list;

typedef int64_t                     p_nint;
typedef long double                 p_ndouble;
p_constexpr p_size BITS_IN_NINT = static_cast<p_size>(sizeof(p_nint) * 8);

typedef WIN32_FILE_ATTRIBUTE_DATA   p_adata;
typedef WIN32_FIND_DATAW            p_fdata;
typedef FILETIME                    p_ftim;
typedef std::wstringstream          p_stream;
typedef std::wostringstream         p_ostream;

typedef HANDLE                      p_entry;


}

#endif /* PRIMITIVES_H */
