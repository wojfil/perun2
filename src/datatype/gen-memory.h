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

#ifndef GEN_MEMORY_H_INCLUDED
#define GEN_MEMORY_H_INCLUDED

#include "definition.h"
#include <memory>
#include <vector>


namespace uro
{

template <typename T>
using _genptr = std::unique_ptr<Generator<T>>;

typedef std::unique_ptr<Definition> _defptr;


template <typename T>
void transferGenPtrs(std::vector<_genptr<T>>& source, std::vector<_genptr<T>>& destination)
{
   const _size len = source.size();
   for (_size i = 0; i < len; i++) {
      _genptr<T>& s = source[i];
      destination.push_back(std::move(source[i]));
   }
};

}

#endif // GEN_MEMORY_H_INCLUDED
