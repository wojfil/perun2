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

#include "../primitives.h"

namespace perun2
{

// this file is not yet used in this project

// a hypothetical function that takes a string 
// and transforms all non-latin characters (Cyrillic, Chinese, Japanese, ...) to the latin alphabet
// using the most common standard in the world
// note that some characters belong to multiple alphabets
// and we should differenciate Serbian Cyrillic from Russian etc.
void str_toLatin(p_str& value);

}
