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

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cstdlib>
#include <cstdint>
#include <string>
#include <sstream>
#include <windows.h>


namespace uro
{

typedef bool                        _bool;
typedef wchar_t                     _char;
typedef std::wstring                _str;

typedef int32_t                     _int;
typedef uint32_t                    _uint32;
typedef uint64_t                    _uint64;
typedef long unsigned int           _ulong;
typedef int64_t                     _nint;
typedef long double                 _ndouble;
typedef std::size_t                 _size;

typedef WIN32_FILE_ATTRIBUTE_DATA   _adata;
typedef WIN32_FIND_DATAW            _fdata;
typedef FILETIME                    _ftim;
typedef std::wstringstream          _stream;


static const _str EMPTY_STRING = L"";


_str toStr(const _char& ch);

template<typename T>
_str toStr(const T& n)
{
    std::wostringstream s;
    s << n;
    return s.str();
}

template<typename... Args>
_str str(Args const&... args)
{
   _size len = 0;
   using value_type = std::common_type_t<Args const&...>;
   for (auto const& arg : { static_cast<value_type>(args)...} ) {
      len += arg.size();
   }

   _str result;
   result.reserve(len);
   for (auto const& arg : { static_cast<value_type>(args)...} ) {
      result += arg;
   }

   return result;
}

void toLower(_char& ch);
void toLower(_str& value);
void toUpper(_char& ch);
void toUpper(_str& value);

}

#endif /* PRIMITIVES_H */
