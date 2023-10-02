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

#ifndef CTX_LOCATION_H_INCLUDED
#define CTX_LOCATION_H_INCLUDED

#include "ctx-vars.h"


namespace perun2
{

   struct LocationContext
   {
   public:
      LocationContext();
      LocationContext(LocationContext* prev);
      void loadData(const p_str& trimmedValue);

      p_varptr<p_str> location;

   private:
      LocationContext* prevLocation;
   };
   
   
   typedef std::unique_ptr<LocationContext>        p_lcptr;
}

#endif // CTX_LOCATION_H_INCLUDED
