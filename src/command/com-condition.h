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


namespace uro::comm
{

struct CS_Condition : Command
{
public:
   CS_Condition();
   //void setMain(_comptr& mainCom, _genptr<_bool>& mainCond);
   //void setMain(_genptr<_bool>& mainCond);
   //void setCommand(_comptr& com);
   //_comptr& getMainCommand();
   //_genptr<_bool>& getMainCondition();

   _comptr* getCommandPtr() {
      return &command;
   };

   void run() override;

private:
   _comptr command;
   //_comptr mainCommand;
   //_genptr<_bool> mainCondition;
};



struct If_Base : Command
{
public:
   If_Base(_genptr<_bool>& cond, _comptr& com);

protected:
   _genptr<_bool> condition;
   _comptr mainCommand;
};


struct If_Raw : If_Base
{
public:
   If_Raw(_genptr<_bool>& cond, _comptr& com);
   void run() override;
};


struct If_Else : If_Base
{
public:
   If_Else(_genptr<_bool>& cond, _comptr& com, _comptr& alt);
   void run() override;

private:
   _comptr altCommand;
};


struct If_ElseIf : If_Base
{
public:
   If_ElseIf(_genptr<_bool>& cond, _comptr& com, _genptr<_bool>& altCond, _comptr& alt);
   void run() override;

private:
   _genptr<_bool> altCondition;
   _comptr altCommand;
};


struct If_ElseIfElse : If_Base
{
public:
   If_ElseIfElse(_genptr<_bool>& cond, _comptr& com, _genptr<_bool>& altCond, _comptr& alt, _comptr& els);
   void run() override;

private:
   _genptr<_bool> altCondition;
   _comptr altCommand;
   _comptr elseCommand;
};


struct If_ManyAlternatives : If_Base
{
public:
   If_ManyAlternatives(_genptr<_bool>& cond, _comptr& com,
      std::vector<_genptr<_bool>>& altConds, std::vector<_comptr>& altComms);

protected:
   std::vector<_genptr<_bool>> altConditions;
   std::vector<_comptr> altCommands;
   const _size altCount;
};


struct If_Alts : If_ManyAlternatives
{
public:
   If_Alts(_genptr<_bool>& cond, _comptr& com, std::vector<_genptr<_bool>>& altConds,
      std::vector<_comptr>& altComms);
   void run() override;
};


struct If_AltsElse : If_ManyAlternatives
{
public:
   If_AltsElse(_genptr<_bool>& cond, _comptr& com, std::vector<_genptr<_bool>>& altConds,
      std::vector<_comptr>& altComms, _comptr& els);
   void run() override;

private:
   _comptr elseCommand;
};

}

#endif // COM_CONDITION_H_INCLUDED
