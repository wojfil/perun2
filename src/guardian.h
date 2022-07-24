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

#define _pgcs_unit _uint32
#define _pg_unit _uint64

// Parse Guardian "contains symbol"
inline constexpr _pgcs_unit PGCS_NULL =            0b0000000;
inline constexpr _pgcs_unit PGCS_QUESTION_MARK =   0b0000001;
inline constexpr _pgcs_unit PGCS_COMMA =           0b0000010;
inline constexpr _pgcs_unit PGCS_COLON =           0b0000100;
inline constexpr _pgcs_unit PGCS_PLUS =            0b0001000;
inline constexpr _pgcs_unit PGCS_MINUS =           0b0010000;
inline constexpr _pgcs_unit PGCS_EQUALS =          0b0100000;

// usual Parse Guardians:
inline constexpr _pg_unit PG_NULL =           0b00000000000000000000000000000000;
inline constexpr _pg_unit PG_COMPARISON =     0b00000000000000000000000000000001;

// what are Parse Guardians for?
// they provide memory for a sequence of tokens
// for example, if we checked once in the past that certain sequence does not contain any comma
// then we do not have to do that again somewhere else
// parsing possibilities are already narrowed

struct ParseGuardian
{
public:
   ParseGuardian();

   _boo knows(const _pg_unit& unit) const;
   _boo protects(const _pg_unit& unit) const;
   void set(const _pg_unit& unit, const _boo& value);

   _boo knowsSymbol(const _pgcs_unit& unit) const;
   _boo protectsSymbol(const _pgcs_unit& unit) const;
   void setSymbol(const _pgcs_unit& unit, const _boo& value);

   _char pgcsToChar(const _pgcs_unit& pgcs) const;

private:
   _pg_unit visited;
   _pg_unit protect;
   _pgcs_unit visitedSymbol;
   _pgcs_unit protectSymbol;
};



#endif // GUARDIAN_H_INCLUDED
