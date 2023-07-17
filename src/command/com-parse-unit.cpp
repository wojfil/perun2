/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "com-parse-unit.h"
#include "../datatype/number.h"
#include "../exception.h"
#include "com-condition.h"


namespace perun2::comm
{

ConditionUnit::ConditionUnit(p_comptr* ptr)
  : pointer(ptr) { };


void ConditionUnit::finish()
{
   if (this->pointer == nullptr) {
      return;
   }

   const p_size altCount = this->elseIfCommands.size();

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

void ConditionUnit::setElse(p_comptr& com)
{
   this->hasElse = true;
   this->elseCommand = std::move(com);
}

void ConditionUnit::addElseIf(p_comptr& com, p_genptr<p_bool>& cond)
{
   this->elseIfCommands.push_back(std::move(com));
   this->elseIfConditions.push_back(std::move(cond));
}

p_bool ConditionUnit::isClosed() const
{
   return this->closed;
}

p_bool ConditionUnit::isElseClosed() const
{
   return this->elseClosed;
}

p_bool ConditionUnit::isLocked() const
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

void ConditionUnit::setMain(p_genptr<p_bool>& mainCond)
{
   this->mainCommand = std::make_unique<C_DoNothing>();
   this->mainCondition = std::move(mainCond);
}

void ConditionUnit::setMain(p_comptr& mainCom, p_genptr<p_bool>& mainCond)
{
   this->mainCommand = std::move(mainCom);
   this->mainCondition = std::move(mainCond);
}


void ConditionContext::add(p_comptr* pntr)
{
   this->units.emplace_back(pntr);
}

void ConditionContext::addClosed(p_comptr* pntr)
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

p_bool ConditionContext::isExpandable() const
{
   if (this->units.empty()) {
      return false;
   }

   const ConditionUnit& cu = this->units.back();
   return cu.isClosed() && !cu.isLocked() && cu.pointer != nullptr;
}

void ConditionContext::addElse(p_comptr& com, const p_int line)
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

void ConditionContext::addEmptyElse(const p_int line)
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

void ConditionContext::addElseIf(p_genptr<p_bool>& cond, p_comptr& com, const p_int line)
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

void ConditionContext::setMain(p_genptr<p_bool>& mainCond)
{ 
   ConditionUnit& cu = this->units.back();
   cu.setMain(mainCond);
}

void ConditionContext::setMain(p_comptr& mainCom, p_genptr<p_bool>& mainCond)
{
   ConditionUnit& cu = this->units.back();
   cu.setMain(mainCom, mainCond);
}

}
