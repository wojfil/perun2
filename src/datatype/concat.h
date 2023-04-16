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

#ifndef CONCAT_H_INCLUDED
#define CONCAT_H_INCLUDED

#include "primitives.h"

namespace perun2
{


void insertStr(_str& result);

template<typename T, typename... Args>
void insertStr(_str& result, const T& firstValue, Args const&... args)
{
   result += firstValue;
   insertStr(result, args...);
}

template<typename... Args>
_str str(Args const&... args)
{
   _str result;
   insertStr(result, args...);
   return result;
}

}

#endif // CONCAT_H_INCLUDED
