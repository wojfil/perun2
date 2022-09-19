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

#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <map>


namespace uro::langutil
{

// delete all elements from a vector of pointers
template <typename T>
void deleteVector(std::vector<T*>& vec)
{
   for (auto p : vec) {
      delete p;
   }
}


// delete all elements from a vector of pointers
// finally, delete the vector itself
template <typename T>
void deleteVectorPtr(std::vector<T*>*& vec)
{
   for (auto p : *vec) {
      delete p;
   }
   vec->clear();
   delete vec;
}


// add a vector at the end of another vector
template <typename T>
void appendVector(std::vector<T>& dest, const std::vector<T>& source)
{
   dest.reserve(dest.size() + source.size());
   dest.insert(dest.end(), source.begin(), source.end());
}


// delete all elements from a map of pointers
template <typename T1, typename T2>
void deleteMap(std::map<T1, T2*>& map)
{
   for (const auto& kv : map) {
      delete kv.second;
   }
}

// delete two
template <typename T1, typename T2>
void deleteTwo(T1* value1, T2* value2)
{
   if (value1 != nullptr) {
      delete value1;
   }

   if (value2 != nullptr) {
      delete value2;
   }
}

// delete previous content of vector
// and reserve known new memory
template <typename T>
void clearAndReserve(std::vector<T>& vec, const _size& length)
{
   vec.clear();
   vec.reserve(length);
}

}


#endif /* UTIL_H */
