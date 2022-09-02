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


void CS_If::setMain(Command* com)
{
   mainCommand = com;
   hasMain = true;
}


void CS_If::setElse(Command* com)
{
   elseCommand = com;
   hasElse = true;
   hasAlternatives = true;
}


void CS_If::addElseIf(Generator<_boo>* cond, Command* com)
{
   elseIfConditions.push_back(cond);
   elseIfCommands.push_back(com);
   elseIfCount++;
   hasAlternatives = true;
}


void CS_If::run()
{
   if (this->uroboros->running) {
      if (condition->getValue()) {
         if (hasMain) {
            mainCommand->run();
         }
      }
      else if (hasAlternatives) {
         for (_size i = 0; i < elseIfCount && this->uroboros->running; i++) {
            if ((elseIfConditions[i])->getValue()) {
               (elseIfCommands[i])->run();
               return;
            }
         }
         if (this->uroboros->running && hasElse) {
            elseCommand->run();
         }
      }
   }
}



If_Base::If_Base(Generator<_boo>* cond, Command* com)
   : condition(cond), mainCommand(com) { }

If_Base::~If_Base()
{
   delete condition;
   delete mainCommand;
}


If_Raw::If_Raw(Generator<_boo>* cond, Command* com)
   : If_Base(cond, com) { }

void If_Raw::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
}


If_Else::If_Else(Generator<_boo>* cond, Command* com, Command* alt)
   : If_Base(cond, com), altCommand(alt) { }

If_Else::~If_Else()
{
   delete altCommand;
}

void If_Else::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else {
      this->altCommand->run();
   }
}


If_ElseIf::If_ElseIf(Generator<_boo>* cond, Command* com, Generator<_boo>* altCond, Command* alt)
   : If_Base(cond, com), altCondition(altCond), altCommand(alt) { }

If_ElseIf::~If_ElseIf()
{
   delete altCondition;
   delete altCommand;
}

void If_ElseIf::run()
{
   if (this->condition->getValue()) {
      this->mainCommand->run();
   }
   else if (this->altCondition->getValue()) {
      this->altCommand->run();
   }
}


If_ElseIfElse::If_ElseIfElse(Generator<_boo>* cond, Command* com, Generator<_boo>* altCond, Command* alt, Command* els)
   : If_Base(cond, com), altCondition(altCond), altCommand(alt), elseCommand(els) { }

If_ElseIfElse::~If_ElseIfElse()
{
   delete altCondition;
   delete altCommand;
   delete elseCommand;
}

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


If_ManyAlternatives::If_ManyAlternatives(Generator<_boo>* cond, Command* com, 
      const std::vector<Generator<_boo>*>& altConds, const std::vector<Command*>& altComms)
   : If_Base(cond, com), altConditions(altConds), altCommands(altComms), altCount(altConds.size()) { }

If_ManyAlternatives::~If_ManyAlternatives()
{
   deleteVector(altConditions);
   deleteVector(altCommands);
}


If_Alts::If_Alts(Generator<_boo>* cond, Command* com, const std::vector<Generator<_boo>*>& altConds, 
   const std::vector<Command*>& altComms)
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


If_AltsElse::If_AltsElse(Generator<_boo>* cond, Command* com, const std::vector<Generator<_boo>*>& altConds, 
   const std::vector<Command*>& altComms, Command* els)
   : If_ManyAlternatives(cond, com, altConds, altComms), elseCommand(els) { }

If_AltsElse::~If_AltsElse()
{
   delete elseCommand;
}

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
