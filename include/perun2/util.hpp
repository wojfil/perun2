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

#include <map>
#include <memory>
#include "datatype/primitives.hpp"


namespace perun2::langutil
{

// add a vector at the end of another vector
template <typename T>
void appendVector(std::vector<T>& dest, const std::vector<T>& source)
{
   dest.reserve(dest.size() + source.size());
   dest.insert(dest.end(), source.begin(), source.end());
}


// delete previous content of vector
// and reserve deterministic new memory
template <typename T>
void clearAndReserve(std::vector<T>& vec, const p_size length)
{
   vec.clear();
   vec.reserve(length);
}


template <typename T>
void transferUniquePtrs(std::vector<std::unique_ptr<T>>& source, std::vector<std::unique_ptr<T>>& destination)
{
   for (std::unique_ptr<T>& s : source)
   {
      destination.push_back(std::move(s));
   }
};


// take the biggest value of two values
template <typename T>
T maximum(const T& a, const T& b)
{
   return ((a) < (b)) ? (b) : (a);
}


// take the smallest value of two values
template <typename T>
T minimum(const T& a, const T& b)
{
   return ((a) < (b)) ? (a) : (b);
}

// take the smallest value of three values
template <typename T>
T minimum(const T& a, const T& b, const T& c)
{
   if (a < b) {
      return ((a) < (c)) ? (a) : (c);
   }

   return ((b) < (c)) ? (b) : (c);
}



}
