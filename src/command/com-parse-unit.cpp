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
#include "com-condition.h"


void IfContext::addIfParseUnit(CS_If* pntr)
{
   if (!this->units.empty() && this->units.back().closed) {
      this->units.pop_back();
   }

   this->units.push_back(IfParseUnit(pntr));
}

void IfContext::retreatIfParseUnit()
{
   if (!this->units.empty() && this->units.back().closed) {
      this->units.pop_back();
   }

   if (!this->units.empty()) {
      if (this->units.back().pointer == nullptr) {
         this->units.pop_back();
      }
      else {
         this->units.back().closed = true;
         this->units.back().locked = false;
      }
   }
}

void IfContext::lockLastIf()
{
   if (!this->units.empty()) {
      if (this->units.back().pointer != nullptr) {
         this->units.back().locked = true;
      }
   }
}

void IfContext::setElse(Command* com, const _int& line)
{
   if (this->units.empty() || this->units.back().pointer == nullptr || this->units.back().locked) {
      throw SyntaxException(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   if (this->units.back().elseClosed) {
      throw SyntaxException(
         L"structure 'if' already contains an assigned structure 'else'", line);
   }

   this->units.back().pointer->setElse(com);
   this->units.back().elseClosed = true;
}

void IfContext::setEmptyElse(const _int& line)
{
   if (this->units.empty() || this->units.back().pointer == nullptr || this->units.back().locked) {
      throw SyntaxException(
         L"structure 'else' is not preceded by a structure 'if'", line);
   }

   if (this->units.back().elseClosed) {
      throw SyntaxException(
         L"structure 'if' already contains an assigned structure 'else'", line);
   }

   this->units.back().elseClosed = true;
}

void IfContext::addElseIf(Generator<_boo>* cond, Command* com, const _int& line)
{
   if (this->units.empty() || this->units.back().pointer == nullptr  || this->units.back().locked) {
      throw SyntaxException(
         L"structure 'else if' is not preceded by a structure 'if'", line);
   }

   if (this->units.back().elseClosed) {
      throw SyntaxException(
         L"structure 'else if' should appear before the structure 'else'", line);
   }

   this->units.back().pointer->addElseIf(cond, com);
}
