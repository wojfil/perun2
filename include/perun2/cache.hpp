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

#include "datatype/datatype.hpp"
#include <unordered_map>

namespace perun2
{


p_constexpr size_t CACHE_SIZE = 100;


struct CacheUnit
{


};


struct Cache
{
public:
   Cache();


private:

   std::unordered_map<std::wstring, size_t> elements;

   std::vector<CacheUnit> realData;

};

}
