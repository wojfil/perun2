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

#include "guardian.h"


ParseGuardian::ParseGuardian()
   : visited(PG_NULL), protect(PG_NULL),
     visitedSymbol(PGCS_NULL), protectSymbol(PGCS_NULL) { };


_boo ParseGuardian::knows(const _pg_unit& unit) const
{
   return this->visited & unit;
}

_boo ParseGuardian::protects(const _pg_unit& unit) const
{
   return this->protect & unit;
}

void ParseGuardian::set(const _pg_unit& unit, const _boo& value)
{
   this->visited |= unit;
   if (value) {
      this->protect |= unit;
   }
}

_boo ParseGuardian::knowsSymbol(const _pgcs_unit& unit) const
{
   return this->visitedSymbol & unit;
}

_boo ParseGuardian::protectsSymbol(const _pgcs_unit& unit) const
{
   return this->protectSymbol & unit;
}

void ParseGuardian::setSymbol(const _pgcs_unit& unit, const _boo& value)
{
   this->visitedSymbol |= unit;
   if (value) {
      this->protectSymbol |= unit;
   }
}

_char ParseGuardian::pgcsToChar(const _pgcs_unit& pgcs) const
{
   switch (pgcs) {
      case PGCS_QUESTION_MARK:
         return L'?';
      case PGCS_COMMA:
         return L',';
      case PGCS_COLON:
         return L':';
      case PGCS_PLUS:
         return L'+';
      case PGCS_MINUS:
         return L'-';
      case PGCS_EQUALS:
         return L'=';
      default:
         return L'/0';
   }
}
