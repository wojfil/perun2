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

#include "com-parse-unit.h"
#include "../datatype/number.h"
#include "../exception.h"

std::vector<IfParseUnit> g_ifparseunits;


void addIfParseUnit(CS_If* pntr)
{
   if (!g_ifparseunits.empty() && g_ifparseunits.back().closed) {
      g_ifparseunits.pop_back();
   }

   g_ifparseunits.push_back(IfParseUnit(pntr));
}

void retreatIfParseUnit()
{
   if (!g_ifparseunits.empty() && g_ifparseunits.back().closed) {
      g_ifparseunits.pop_back();
   }

   if (!g_ifparseunits.empty()) {
      if (g_ifparseunits.back().pointer == nullptr) {
         g_ifparseunits.pop_back();
      }
      else {
         g_ifparseunits.back().closed = true;
         g_ifparseunits.back().locked = false;
      }
   }
}

void lockLastIf()
{
   if (!g_ifparseunits.empty()) {
      if (g_ifparseunits.back().pointer != nullptr) {
         g_ifparseunits.back().locked = true;
      }
   }
}

void setElse(Command* com, const _int& line)
{
   if (g_ifparseunits.empty() || g_ifparseunits.back().pointer == nullptr || g_ifparseunits.back().locked) {
      throw SyntaxException(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   if (g_ifparseunits.back().elseClosed) {
      throw SyntaxException(
         L"structure 'if' already contains an assigned structure 'else'", line);
   }

   g_ifparseunits.back().pointer->setElse(com);
   g_ifparseunits.back().elseClosed = true;
}

void setEmptyElse(const _int& line)
{
   if (g_ifparseunits.empty() || g_ifparseunits.back().pointer == nullptr || g_ifparseunits.back().locked) {
      throw SyntaxException(
         L"structure 'else' is not preceded by a structure 'if'", line);
   }

   if (g_ifparseunits.back().elseClosed) {
      throw SyntaxException(
         L"structure 'if' already contains an assigned structure 'else'", line);
   }

   g_ifparseunits.back().elseClosed = true;
}

void addElseIf(Generator<_boo>* cond, Command* com, const _int& line)
{
   if (g_ifparseunits.empty() || g_ifparseunits.back().pointer == nullptr  || g_ifparseunits.back().locked) {
      throw SyntaxException(
         L"structure 'else if' is not preceded by a structure 'if'", line);
   }

   if (g_ifparseunits.back().elseClosed) {
      throw SyntaxException(
         L"structure 'else if' should appear before the structure 'else'", line);
   }

   g_ifparseunits.back().pointer->addElseIf(cond, com);
}
