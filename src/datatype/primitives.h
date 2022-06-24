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

#define _boo bool
#define _char wchar_t
#define _str std::wstring

#define _int int32_t
#define _uint32 uint32_t
#define _uint64 uint64_t
#define _nint int64_t
#define _ndouble long double
#define _size std::size_t


template<typename T>
_str toStr(const T &n)
{
    std::wostringstream s;
    s << n;
    return s.str();
}


template<typename... Args>
_str str(Args... args)
{
   _size len = 0;
   using value_type = std::common_type_t<Args...>;
   for (auto const &arg : {static_cast<value_type>(args)...}) {
      len += arg.size();
   }

   _str result;
   result.reserve(len);
   for (auto const &arg : {static_cast<value_type>(args)...}) {
      result += arg;
   }

   return result;
}

_str charStr(const _char& ch);
void toLower(_str& value);
void toUpper(_str& value);
void toLowerChar(_char& ch);
void toUpperChar(_char& ch);

#endif /* PRIMITIVES_H */
