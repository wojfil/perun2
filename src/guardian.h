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

#define _pg_unit _uint64

// all Parse Guardian units:
inline constexpr _pg_unit PG_NULL =                0b00000000000000;
inline constexpr _pg_unit PG_COMPARISON_CHAR =     0b00000000000001;
inline constexpr _pg_unit PG_FILTER_KEYWORD =      0b00000000000010;
inline constexpr _pg_unit PG_INDEP_BRACKETS =      0b00000000000100;
inline constexpr _pg_unit PG_INDEP_SQ_BRACKETS =   0b00000000001000;
inline constexpr _pg_unit PG_CHAR_QUESTION_MARK =  0b00000000010000;
inline constexpr _pg_unit PG_CHAR_COMMA =          0b00000000100000;
inline constexpr _pg_unit PG_CHAR_COLON =          0b00000001000000;
inline constexpr _pg_unit PG_CHAR_PLUS =           0b00000010000000;
inline constexpr _pg_unit PG_CHAR_MINUS =          0b00000100000000;
inline constexpr _pg_unit PG_CHAR_EQUALS =         0b00001000000000;

// what are Parse Guardians for?
// they provide memory for a sequence of tokens
// for example, if we checked once in the past that certain sequence does not contain any comma
// then we do not have to do that again somewhere else
// parsing possibilities are already narrowed

struct ParseGuardian
{
public:
   _boo knows(const _pg_unit& unit) const;
   _boo protects(const _pg_unit& unit) const;
   void set(const _pg_unit& unit, const _boo& value);

   _char unitToChar(const _pg_unit& pgcs) const;

private:
   _pg_unit visited = PG_NULL;
   _pg_unit protect = PG_NULL;
};



#endif // GUARDIAN_H_INCLUDED
