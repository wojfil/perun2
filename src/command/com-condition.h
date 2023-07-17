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

#ifndef COM_CONDITION_H_INCLUDED
#define COM_CONDITION_H_INCLUDED

#include "com.h"
#include "../datatype/generator.h"
#include "../datatype/primitives.h"
#include "../util.h"
#include "../perun2.h"


namespace perun2::comm
{

struct CS_Condition : Command
{
public:
   CS_Condition();
   p_comptr* getCommandPtr();
   void run() override;

private:
   p_comptr command;
};


struct If_Base : Command
{
public:
   If_Base(p_genptr<p_bool>& cond, p_comptr& com);

protected:
   p_genptr<p_bool> condition;
   p_comptr mainCommand;
};


struct If_Raw : If_Base
{
public:
   If_Raw(p_genptr<p_bool>& cond, p_comptr& com);
   void run() override;
};


struct If_Else : If_Base
{
public:
   If_Else(p_genptr<p_bool>& cond, p_comptr& com, p_comptr& alt);
   void run() override;

private:
   p_comptr altCommand;
};


struct If_ElseIf : If_Base
{
public:
   If_ElseIf(p_genptr<p_bool>& cond, p_comptr& com, p_genptr<p_bool>& altCond, p_comptr& alt);
   void run() override;

private:
   p_genptr<p_bool> altCondition;
   p_comptr altCommand;
};


struct If_ElseIfElse : If_Base
{
public:
   If_ElseIfElse(p_genptr<p_bool>& cond, p_comptr& com, p_genptr<p_bool>& altCond, p_comptr& alt, p_comptr& els);
   void run() override;

private:
   p_genptr<p_bool> altCondition;
   p_comptr altCommand;
   p_comptr elseCommand;
};


struct If_ManyAlternatives : If_Base
{
public:
   If_ManyAlternatives(p_genptr<p_bool>& cond, p_comptr& com,
      std::vector<p_genptr<p_bool>>& altConds, std::vector<p_comptr>& altComms);

protected:
   std::vector<p_genptr<p_bool>> altConditions;
   std::vector<p_comptr> altCommands;
   const p_size altCount;
};


struct If_Alts : If_ManyAlternatives
{
public:
   If_Alts(p_genptr<p_bool>& cond, p_comptr& com, std::vector<p_genptr<p_bool>>& altConds,
      std::vector<p_comptr>& altComms);
   void run() override;
};


struct If_AltsElse : If_ManyAlternatives
{
public:
   If_AltsElse(p_genptr<p_bool>& cond, p_comptr& com, std::vector<p_genptr<p_bool>>& altConds,
      std::vector<p_comptr>& altComms, p_comptr& els);
   void run() override;

private:
   p_comptr elseCommand;
};

}

#endif // COM_CONDITION_H_INCLUDED
