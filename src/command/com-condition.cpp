/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "com-condition.h"
#include "../perun2.h"
#include "../util.h"


namespace perun2::comm
{

CS_Condition::CS_Condition()
  : command(std::make_unique<C_DoNothing>()) { }


p_comptr* CS_Condition::getCommandPtr()
{
   return &this->command;
};


void CS_Condition::run()
{
   this->command->run();
}


If_Base::If_Base(p_genptr<p_bool>& cond, p_comptr& com)
   : condition(std::move(cond)), mainCommand(std::move(com)) { }


If_Raw::If_Raw(p_genptr<p_bool>& cond, p_comptr& com)
   : If_Base(cond, com) { }

void If_Raw::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
}


If_Else::If_Else(p_genptr<p_bool>& cond, p_comptr& com, p_comptr& alt)
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


If_ElseIf::If_ElseIf(p_genptr<p_bool>& cond, p_comptr& com, p_genptr<p_bool>& altCond, p_comptr& alt)
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


If_ElseIfElse::If_ElseIfElse(p_genptr<p_bool>& cond, p_comptr& com, p_genptr<p_bool>& altCond, p_comptr& alt, p_comptr& els)
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


If_ManyAlternatives::If_ManyAlternatives(p_genptr<p_bool>& cond, p_comptr& com,
      std::vector<p_genptr<p_bool>>& altConds, std::vector<p_comptr>& altComms)
   : If_Base(cond, com), altCount(altConds.size())
{
   langutil::transferUniquePtrs(altConds, this->altConditions);
   langutil::transferUniquePtrs(altComms, this->altCommands);
}

If_Alts::If_Alts(p_genptr<p_bool>& cond, p_comptr& com, std::vector<p_genptr<p_bool>>& altConds,
   std::vector<p_comptr>& altComms)
   : If_ManyAlternatives(cond, com, altConds, altComms) { }

void If_Alts::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else {
      for (p_size i = 0; i < this->altCount; i++) {
         if (this->altConditions[i]->getValue()) {
            this->altCommands[i]->run();
            return;
         }
      }
   }
}


If_AltsElse::If_AltsElse(p_genptr<p_bool>& cond, p_comptr& com, std::vector<p_genptr<p_bool>>& altConds,
   std::vector<p_comptr>& altComms, p_comptr& els)
   : If_ManyAlternatives(cond, com, altConds, altComms), elseCommand(std::move(els)) { }

void If_AltsElse::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else {
      for (p_size i = 0; i < this->altCount; i++) {
         if (this->altConditions[i]->getValue()) {
            this->altCommands[i]->run();
            return;
         }
      }

      this->elseCommand->run();
   }
}

}
