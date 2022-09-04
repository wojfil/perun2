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


ConditionUnit::ConditionUnit(CS_Condition* pntr)
  : pointer(pntr) { };

void ConditionUnit::finish()
{
   if (this->pointer == nullptr) {
      return;
   }

   Command* mainCommand = this->pointer->getMainCommand();
   Generator<_boo>* mainCondition = this->pointer->getMainCondition();
   const _size altCount = this->elseIfCommands.size();

   switch (altCount) {
      case 0: {
         if (this->elseCommand == nullptr) {
            this->pointer->setCommand(new If_Raw(mainCondition, mainCommand));
         }
         else {
            this->pointer->setCommand(new If_Else(mainCondition, mainCommand, this->elseCommand));
         }
         break;
      }
      case 1: {
         if (this->elseCommand == nullptr) {
            this->pointer->setCommand(new If_ElseIf(mainCondition, mainCommand,
               this->elseIfConditions[0], this->elseIfCommands[0]));
         }
         else {
            this->pointer->setCommand(new If_ElseIfElse(mainCondition, mainCommand, this->elseIfConditions[0],
               this->elseIfCommands[0], this->elseCommand));
         }
         break;
      }
      default: {
         if (this->elseCommand == nullptr) {
            this->pointer->setCommand(
               new If_Alts(mainCondition, mainCommand, this->elseIfConditions, this->elseIfCommands));
         }
         else {
            this->pointer->setCommand(
               new If_AltsElse(mainCondition, mainCommand, this->elseIfConditions,
                  this->elseIfCommands, this->elseCommand));
         }
         break;
      }
   }
}

void ConditionUnit::setElse(Command* com)
{
   this->elseCommand = com;
}

void ConditionUnit::addElseIf(Command* com, Generator<_boo>* cond)
{
   this->elseIfCommands.push_back(com);
   this->elseIfConditions.push_back(cond);
}

_boo ConditionUnit::isClosed() const
{
   return this->closed;
}

_boo ConditionUnit::isElseClosed() const
{
   return this->elseClosed;
}

_boo ConditionUnit::isLocked() const
{
   return this->locked;
}

void ConditionUnit::close()
{
   this->closed = true;
}

void ConditionUnit::closeElse()
{
   this->elseClosed = true;
}

void ConditionUnit::unlock()
{
   this->locked = false;
}


void ConditionContext::addIfParseUnit()
{
   if (!this->units.empty() && this->units.back().isClosed()) {
      this->units.back().finish();
      this->units.pop_back();
   }

   this->units.emplace_back(nullptr);
}

void ConditionContext::addIfParseUnit(CS_Condition* pntr)
{
   if (!this->units.empty() && this->units.back().isClosed()) {
      this->units.back().finish();
      this->units.pop_back();
   }

   this->units.emplace_back(pntr);
}

void ConditionContext::retreatIfParseUnit()
{
   if (this->units.empty()) {
      return;
   }

   if (this->units.back().isClosed()) {
      this->units.back().finish();
      this->units.pop_back();
   }

   if (!this->units.empty()) {
      ConditionUnit& cond = this->units.back();

      if (cond.pointer == nullptr) {
         cond.finish();
         this->units.pop_back();
      }
      else {
         cond.close();
         cond.unlock();
      }
   }
}

void ConditionContext::lockLastIf()
{
   if (!this->units.empty()) {
      ConditionUnit& cond = this->units.back();

      if (cond.pointer != nullptr && cond.isClosed()) {
         cond.finish();
         this->units.pop_back();
      }
   }
}

void ConditionContext::setElse(Command* com, const _int& line)
{
   if (this->units.empty() || this->units.back().pointer == nullptr || this->units.back().isLocked()) {
      throw SyntaxException(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   ConditionUnit& cond = this->units.back();

   if (cond.isElseClosed()) {
      throw SyntaxException(
         L"structure 'if' already contains an assigned structure 'else'", line);
   }

   cond.setElse(com);
   cond.closeElse();
}

void ConditionContext::setEmptyElse(const _int& line)
{
   if (this->units.empty() || this->units.back().pointer == nullptr || this->units.back().isLocked()) {
      throw SyntaxException(
         L"structure 'else' is not preceded by a structure 'if'", line);
   }

   ConditionUnit& cond = this->units.back();

   if (cond.isElseClosed()) {
      throw SyntaxException(
         L"structure 'if' already contains an assigned structure 'else'", line);
   }

   cond.closeElse();
}

void ConditionContext::addElseIf(Generator<_boo>* cond, Command* com, const _int& line)
{
   if (this->units.empty() || this->units.back().pointer == nullptr || this->units.back().isLocked()) {
      throw SyntaxException(
         L"structure 'else if' is not preceded by a structure 'if'", line);
   }

   ConditionUnit& condUnit = this->units.back();

   if (condUnit.isElseClosed()) {
      throw SyntaxException(
         L"structure 'else if' should appear before the structure 'else'", line);
   }

   condUnit.addElseIf(com, cond);
}

void ConditionContext::finish()
{
   if (!this->units.empty()) {
      this->units.back().finish();
      this->units.pop_back();
   }
}

