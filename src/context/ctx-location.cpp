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

#include "ctx-location.h"
#include "../os/os.h"


namespace perun2
{

   LocationContext::LocationContext()
      : location(std::make_unique<Variable<p_str>>(VarType::vt_Special)),
        prevLocation(nullptr) { };

   LocationContext::LocationContext(LocationContext* prev)
      : location(std::make_unique<Variable<p_str>>(VarType::vt_Special)),
        prevLocation(prev) { };

   void LocationContext::loadData(const p_str& trimmedValue)
   {
      this->location->value = this->prevLocation == nullptr
         ? trimmedValue
         : os_leftJoin(this->prevLocation->location->value, trimmedValue);
   }
   
}
