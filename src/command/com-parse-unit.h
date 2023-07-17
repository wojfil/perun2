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

#ifndef COM_PARSE_UNIT_H
#define COM_PARSE_UNIT_H

#include "../datatype/primitives.h"
#include "../datatype/generator.h"
#include "com.h"
#include <vector>


namespace perun2::comm
{

struct CS_Condition;

struct ConditionUnit
{
public:
   ConditionUnit(p_comptr* ptr);

   void finish();
   void setElse(p_comptr& com);
   void addElseIf(p_comptr& com, p_genptr<p_bool>& cond);

   p_bool isClosed() const;
   p_bool isElseClosed() const;
   p_bool isLocked() const;

   void close();
   void closeElse();
   void lock();

   void setMain(p_genptr<p_bool>& mainCond);
   void setMain(p_comptr& mainCom, p_genptr<p_bool>& mainCond);

   p_comptr* const pointer;

private:
   p_bool closed = false;
   p_bool elseClosed = false;
   p_bool locked = false;
   p_bool hasElse = false;

   p_comptr mainCommand;
   p_genptr<p_bool> mainCondition;
   p_comptr elseCommand;

   std::vector<p_comptr> elseIfCommands;
   std::vector<p_genptr<p_bool>> elseIfConditions;
};


struct ConditionContext
{
public:
   void add(p_comptr* pntr);
   void addClosed(p_comptr* pntr);
   void deleteClosedUnits();
   void deleteLast();
   void lockLast();
   p_bool isExpandable() const;
   void addElse(p_comptr& com, const p_int line);
   void addEmptyElse(const p_int line);
   void addElseIf(p_genptr<p_bool>& cond, p_comptr& com, const p_int line);
   void setMain(p_genptr<p_bool>& mainCond);
   void setMain(p_comptr& mainCom, p_genptr<p_bool>& mainCond);

private:
   std::vector<ConditionUnit> units;
};

}

#endif /* COM_PARSE_UNIT_H */
