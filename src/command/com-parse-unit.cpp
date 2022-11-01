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


namespace uro::comm
{

ConditionUnit::ConditionUnit(CS_Condition* pntr)
  : pointer(pntr) { };


void ConditionUnit::finish()
{
   if (this->pointer == nullptr) {
      return;
   }

   Command* mainCommand = this->pointer->getMainCommand();
   _genptr<_bool> mainCondition = std::move(this->pointer->getMainCondition());
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

void ConditionUnit::addElseIf(Command* com, _genptr<_bool>& cond)
{
   this->elseIfCommands.push_back(com);
   this->elseIfConditions.push_back(std::move(cond));
}

_bool ConditionUnit::isClosed() const
{
   return this->closed;
}

_bool ConditionUnit::isElseClosed() const
{
   return this->elseClosed;
}

_bool ConditionUnit::isLocked() const
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

void ConditionUnit::lock()
{
   this->locked = true;
}



void ConditionContext::add(CS_Condition* pntr)
{
   this->units.emplace_back(pntr);
}

void ConditionContext::addClosed(CS_Condition* pntr)
{
   this->units.emplace_back(pntr);
   this->units.back().close();
}

void ConditionContext::deleteClosedUnits()
{
   while (!this->units.empty()) {
      ConditionUnit& cu = this->units.back();
      if (cu.isClosed()) {
         cu.finish();
         this->units.pop_back();
      }
      else {
         cu.close();
         break;
      }
   }
}

void ConditionContext::deleteLast()
{
   this->units.pop_back();
}

void ConditionContext::lockLast()
{
   if (!this->units.empty()) {
      ConditionUnit& cu = this->units.back();
      if (cu.isClosed()) {
         cu.lock();
      }
   }
}

_bool ConditionContext::isExpandable() const
{
   if (this->units.empty()) {
      return false;
   }

   const ConditionUnit& cu = this->units.back();
   return cu.isClosed() && !cu.isLocked() && cu.pointer != nullptr;
}

void ConditionContext::addElse(Command* com, const _int& line)
{
   if (!this->isExpandable()) {
      throw SyntaxException(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   ConditionUnit& cu = this->units.back();

   if (cu.isElseClosed()) {
      throw SyntaxException(L"structure 'if' already contains an assigned structure 'else'", line);
   }

   cu.setElse(com);
   cu.closeElse();
}

void ConditionContext::addEmptyElse(const _int& line)
{
   if (!this->isExpandable()) {
      throw SyntaxException(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   ConditionUnit& cu = this->units.back();

   if (cu.isElseClosed()) {
      throw SyntaxException(L"structure 'if' already contains an assigned structure 'else'", line);
   }

   cu.closeElse();
}

void ConditionContext::addElseIf(_genptr<_bool>& cond, Command* com, const _int& line)
{
   if (!this->isExpandable()) {
      throw SyntaxException(L"structure 'else if' is not preceded by a structure 'if'", line);
   }

   ConditionUnit& cu = this->units.back();

   if (cu.isElseClosed()) {
      throw SyntaxException(L"structure 'else if' should have appeared before the structure 'else'", line);
   }

   cu.addElseIf(com, cond);
}

}
