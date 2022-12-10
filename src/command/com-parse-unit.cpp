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

ConditionUnit::ConditionUnit(_comptr* ptr)
  : pointer(ptr) { };


void ConditionUnit::finish()
{
   if (this->pointer == nullptr) {
      return;
   }

   const _size altCount = this->elseIfCommands.size();

   switch (altCount) {
      case 0: {
         if (this->hasElse) {
            (*this->pointer) = std::make_unique<If_Else>(this->mainCondition, this->mainCommand, this->elseCommand);
         }
         else {
            (*this->pointer) = std::make_unique<If_Raw>(this->mainCondition, this->mainCommand);
         }
         break;
      }
      case 1: {
         if (this->hasElse) {
            (*this->pointer) = std::make_unique<If_ElseIfElse>(this->mainCondition, this->mainCommand, 
               this->elseIfConditions[0], this->elseIfCommands[0], this->elseCommand);
         }
         else {
            (*this->pointer) = std::make_unique<If_ElseIf>(this->mainCondition, this->mainCommand, 
               this->elseIfConditions[0], this->elseIfCommands[0]);
         }
         break;
      }
      default: {
         if (this->hasElse) {
            (*this->pointer) = std::make_unique<If_AltsElse>(this->mainCondition, this->mainCommand, 
               this->elseIfConditions, this->elseIfCommands, this->elseCommand);
         }
         else {
            (*this->pointer) = std::make_unique<If_Alts>(this->mainCondition, this->mainCommand, 
               this->elseIfConditions, this->elseIfCommands);
         }
         break;
      }
   }
}

void ConditionUnit::setElse(_comptr& com)
{
   this->hasElse = true;
   this->elseCommand = std::move(com);
}

void ConditionUnit::addElseIf(_comptr& com, _genptr<_bool>& cond)
{
   this->elseIfCommands.push_back(std::move(com));
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

void ConditionUnit::setMain(_genptr<_bool>& mainCond)
{
   this->mainCommand = std::make_unique<C_DoNothing>();
   this->mainCondition = std::move(mainCond);
}

void ConditionUnit::setMain(_comptr& mainCom, _genptr<_bool>& mainCond)
{
   this->mainCommand = std::move(mainCom);
   this->mainCondition = std::move(mainCond);
}


void ConditionContext::add(_comptr* pntr)
{
   this->units.emplace_back(pntr);
}

void ConditionContext::addClosed(_comptr* pntr)
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

void ConditionContext::addElse(_comptr& com, const _int& line)
{
   if (!this->isExpandable()) {
      throw SyntaxError(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   ConditionUnit& cu = this->units.back();

   if (cu.isElseClosed()) {
      throw SyntaxError(L"structure 'if' already contains an assigned structure 'else'", line);
   }

   cu.setElse(com);
   cu.closeElse();
}

void ConditionContext::addEmptyElse(const _int& line)
{
   if (!this->isExpandable()) {
      throw SyntaxError(L"structure 'else' is not preceded by a structure 'if' ", line);
   }

   ConditionUnit& cu = this->units.back();

   if (cu.isElseClosed()) {
      throw SyntaxError(L"structure 'if' already contains an assigned structure 'else'", line);
   }

   cu.closeElse();
}

void ConditionContext::addElseIf(_genptr<_bool>& cond, _comptr& com, const _int& line)
{
   if (!this->isExpandable()) {
      throw SyntaxError(L"structure 'else if' is not preceded by a structure 'if'", line);
   }

   ConditionUnit& cu = this->units.back();

   if (cu.isElseClosed()) {
      throw SyntaxError(L"structure 'else if' should have appeared before the structure 'else'", line);
   }

   cu.addElseIf(com, cond);
}

void ConditionContext::setMain(_genptr<_bool>& mainCond)
{ 
   ConditionUnit& cu = this->units.back();
   cu.setMain(mainCond);
}

void ConditionContext::setMain(_comptr& mainCom, _genptr<_bool>& mainCond)
{
   ConditionUnit& cu = this->units.back();
   cu.setMain(mainCom, mainCond);
}

}
