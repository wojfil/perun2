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

#ifndef COM_CONDITION_H_INCLUDED
#define COM_CONDITION_H_INCLUDED

#include "com.h"
#include "../datatype/generator.h"
#include "../datatype/primitives.h"
#include <vector>
#include "../util.h"


struct CS_If : Command
{
public:
   CS_If(Generator<_boo>* cond)
      : condition(cond), mainCommand(nullptr), elseCommand(nullptr),
        elseIfConditions(std::vector<Generator<_boo>*>()),
        elseIfCommands(std::vector<Command*>()),
        hasMain(false), hasAlternatives(false), hasElse(false),
        elseIfCount(0) { };

   ~CS_If() {
      delete condition;
      if (mainCommand != nullptr) {
         delete mainCommand;
      }
      if (hasElse) {
         delete elseCommand;
      }
      if (elseIfConditions.size() != 0) {
         deleteVector(elseIfConditions);
         deleteVector(elseIfCommands);
      }
   };

   void setMain(Command* com);
   void setElse(Command* com);
   void addElseIf(Generator<_boo>* cond, Command* com);
   void run() override;

   Command* mainCommand;

private:
   Generator<_boo>* condition;
   Command* elseCommand;
   std::vector<Generator<_boo>*> elseIfConditions;
   std::vector<Command*> elseIfCommands;
   _boo hasMain;
   _boo hasAlternatives;
   _boo hasElse;
   _size elseIfCount;
};



#endif // COM_CONDITION_H_INCLUDED
