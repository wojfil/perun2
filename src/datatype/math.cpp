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

#include "math.h"
#include "numbers.h"


namespace perun2
{

Math::Math() {
   anyRandomDouble = false;
}

void Math::init() {
   std::random_device rd;
   generator = std::mt19937(rd());

   if (anyRandomDouble) {
      doubleDist = std::uniform_real_distribution<>(NDOUBLE_ZERO, NDOUBLE_ONE);
   }
}

_ndouble Math::randomDouble() {
   return doubleDist(generator);
}

_nint Math::randomInt(const _nint limit) {
   std::uniform_int_distribution<_nint> distribution(NINT_ZERO, limit);
   return distribution(generator);
}

void Math::setAnyRandomDouble() {
   anyRandomDouble = true;
}

}
