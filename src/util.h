/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <memory>
#include "datatype/primitives.h"


namespace uro::langutil
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
void clearAndReserve(std::vector<T>& vec, const _size& length)
{
   vec.clear();
   vec.reserve(length);
}


template <typename T>
void transferUniquePtrs(std::vector<std::unique_ptr<T>>& source, std::vector<std::unique_ptr<T>>& destination)
{
   const _size len = source.size();
   for (_size i = 0; i < len; i++) {
      std::unique_ptr<T>& s = source[i];
      destination.push_back(std::move(s));
   }
};


}

#endif /* UTIL_H */
