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

#ifndef DATATYPE_H
#define DATATYPE_H

#include "time.h"
#include "definition.h"
#include "chars.h"
#include "strings.h"
#include "numbers.h"
#include <unordered_set>


namespace perun2
{

typedef Number                              _num;
typedef Time                                _tim;
typedef Period                              _per;
typedef std::vector<_num>                   _nlist;
typedef std::vector<_tim>                   _tlist;
typedef Definition                          _def;
typedef std::unordered_set<_str>            _set;



}

#endif /* DATATYPE_H */
