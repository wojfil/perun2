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
#include "../util.h"
#include "../uroboros.h"


struct CS_Condition : Command
{
public:
   CS_Condition();
   ~CS_Condition();
   void setMain(Command* mainCom, Generator<_boo>* mainCond);
   void setMain(Generator<_boo>* mainCond);
   void setCommand(Command* com);
   Command* getMainCommand();
   Generator<_boo>* getMainCondition();

   void run() override;

private:
   Command* command;

   Command* mainCommand = nullptr;
   Generator<_boo>* mainCondition = nullptr;
};



struct If_Base : Command
{
public:
   If_Base(Generator<_boo>* cond, Command* com);
   ~If_Base();

protected:
   Generator<_boo>* condition;
   Command* mainCommand;
};


struct If_Raw : If_Base
{
public:
   If_Raw(Generator<_boo>* cond, Command* com);
   void run() override;
};


struct If_Else : If_Base
{
public:
   If_Else(Generator<_boo>* cond, Command* com, Command* alt);
   ~If_Else();
   void run() override;

private:
   Command* altCommand;
};


struct If_ElseIf : If_Base
{
public:
   If_ElseIf(Generator<_boo>* cond, Command* com, Generator<_boo>* altCond, Command* alt);
   ~If_ElseIf();
   void run() override;

private:
   Generator<_boo>* altCondition;
   Command* altCommand;
};


struct If_ElseIfElse : If_Base
{
public:
   If_ElseIfElse(Generator<_boo>* cond, Command* com, Generator<_boo>* altCond, Command* alt, Command* els);
   ~If_ElseIfElse();
   void run() override;

private:
   Generator<_boo>* altCondition;
   Command* altCommand;
   Command* elseCommand;
};


struct If_ManyAlternatives : If_Base
{
public:
   If_ManyAlternatives(Generator<_boo>* cond, Command* com,
      const std::vector<Generator<_boo>*>& altConds, const std::vector<Command*>& altComms);
   ~If_ManyAlternatives();

protected:
   std::vector<Generator<_boo>*> altConditions;
   std::vector<Command*> altCommands;
   const _size altCount;
};


struct If_Alts : If_ManyAlternatives
{
public:
   If_Alts(Generator<_boo>* cond, Command* com, const std::vector<Generator<_boo>*>& altConds,
      const std::vector<Command*>& altComms);
   void run() override;
};


struct If_AltsElse : If_ManyAlternatives
{
public:
   If_AltsElse(Generator<_boo>* cond, Command* com, const std::vector<Generator<_boo>*>& altConds,
      const std::vector<Command*>& altComms, Command* els);
   ~If_AltsElse();
   void run() override;

private:
   Command* elseCommand;
};


#endif // COM_CONDITION_H_INCLUDED
