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

#include "comparison.h"


namespace perun2
{

CompType mirrorCompType(const CompType value)
{
   switch (value) {
      case CompType::ct_Smaller: {
         return CompType::ct_Bigger;
      }
      case CompType::ct_SmallerEquals: {
         return CompType::ct_BiggerEquals;
      }
      case CompType::ct_Bigger: {
         return CompType::ct_Smaller;
      }
      case CompType::ct_BiggerEquals: {
         return CompType::ct_SmallerEquals;
      }
      default: { // Equals, NotEquals
         return value;
      }
   };
}

}
