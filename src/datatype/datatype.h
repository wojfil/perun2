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

#ifndef DATATYPE_H
#define DATATYPE_H

#include "number.h"
#include "time.h"
#include "definition.h"

#define _num Number
#define _tim Time
#define _per Period
#define _nlist std::vector<Number>
#define _tlist std::vector<Time>
#define _def Definition
#define _list std::vector<std::wstring>


enum UroDataType
{
   dt_Bool = 0,
   dt_Number,
   dt_Period,
   dt_Time,
   dt_String,
   dt_Definition,
   dt_TimeList,
   dt_NumericList,
   dt_List
};


#endif /* DATATYPE_H */
