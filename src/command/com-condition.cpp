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

#include "com-condition.h"
#include "../uroboros.h"
#include "../util.h"


namespace uro::comm
{

CS_Condition::CS_Condition()
  : command(std::make_unique<C_DoNothing>()) { }


_comptr* CS_Condition::getCommandPtr()
{
   return &this->command;
};


void CS_Condition::run()
{
   this->command->run();
}


If_Base::If_Base(_genptr<_bool>& cond, _comptr& com)
   : condition(std::move(cond)), mainCommand(std::move(com)) { }


If_Raw::If_Raw(_genptr<_bool>& cond, _comptr& com)
   : If_Base(cond, com) { }

void If_Raw::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
}


If_Else::If_Else(_genptr<_bool>& cond, _comptr& com, _comptr& alt)
   : If_Base(cond, com), altCommand(std::move(alt)) { }


void If_Else::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else {
      this->altCommand->run();
   }
}


If_ElseIf::If_ElseIf(_genptr<_bool>& cond, _comptr& com, _genptr<_bool>& altCond, _comptr& alt)
   : If_Base(cond, com), altCondition(std::move(altCond)), altCommand(std::move(alt)) { }


void If_ElseIf::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else if (this->altCondition->getValue()) {
      this->altCommand->run();
   }
}


If_ElseIfElse::If_ElseIfElse(_genptr<_bool>& cond, _comptr& com, _genptr<_bool>& altCond, _comptr& alt, _comptr& els)
   : If_Base(cond, com), altCondition(std::move(altCond)), altCommand(std::move(alt)), elseCommand(std::move(els)) { }


void If_ElseIfElse::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else if (this->altCondition->getValue()) {
      this->altCommand->run();
   }
   else {
      this->elseCommand->run();
   }
}


If_ManyAlternatives::If_ManyAlternatives(_genptr<_bool>& cond, _comptr& com,
      std::vector<_genptr<_bool>>& altConds, std::vector<_comptr>& altComms)
   : If_Base(cond, com), altCount(altConds.size())
{
   langutil::transferUniquePtrs(altConds, this->altConditions);
   langutil::transferUniquePtrs(altComms, this->altCommands);
}

If_Alts::If_Alts(_genptr<_bool>& cond, _comptr& com, std::vector<_genptr<_bool>>& altConds,
   std::vector<_comptr>& altComms)
   : If_ManyAlternatives(cond, com, altConds, altComms) { }

void If_Alts::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else {
      for (_size i = 0; i < this->altCount; i++) {
         if (this->altConditions[i]->getValue()) {
            this->altCommands[i]->run();
            return;
         }
      }
   }
}


If_AltsElse::If_AltsElse(_genptr<_bool>& cond, _comptr& com, std::vector<_genptr<_bool>>& altConds,
   std::vector<_comptr>& altComms, _comptr& els)
   : If_ManyAlternatives(cond, com, altConds, altComms), elseCommand(std::move(els)) { }

void If_AltsElse::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else {
      for (_size i = 0; i < this->altCount; i++) {
         if (this->altConditions[i]->getValue()) {
            this->altCommands[i]->run();
            return;
         }
      }

      this->elseCommand->run();
   }
}

}
