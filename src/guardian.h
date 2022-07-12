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

#ifndef GUARDIAN_H_INCLUDED
#define GUARDIAN_H_INCLUDED

#include "datatype/primitives.h"

#define _pgunit _uint64

// Parse Guardians:
const _pgunit PG_NULL =          0b00000000000000000000000000000000;
const _pgunit PG_CNT_COMMA =     0b00000000000000000000000000000001;

// what are Parse Guardians for?
// they provide memory for a sequence of tokens
// for example, if we checked once in the past that certain sequence does not contain any comma
// then we do not have to do that again - potential parsing possibilities are already narrowed

struct ParseGuardian
{
public:
   ParseGuardian();
   _boo knows(const _pgunit& unit) const;
   _boo protects(const _pgunit& unit) const;
   void set(const _pgunit& unit, const _boo& value);

private:
   _pgunit visited;
   _pgunit protect;

};


#endif // GUARDIAN_H_INCLUDED
