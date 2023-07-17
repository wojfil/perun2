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
   ConditionUnit(_comptr* ptr);

   void finish();
   void setElse(_comptr& com);
   void addElseIf(_comptr& com, _genptr<p_bool>& cond);

   p_bool isClosed() const;
   p_bool isElseClosed() const;
   p_bool isLocked() const;

   void close();
   void closeElse();
   void lock();

   void setMain(_genptr<p_bool>& mainCond);
   void setMain(_comptr& mainCom, _genptr<p_bool>& mainCond);

   _comptr* const pointer;

private:
   p_bool closed = false;
   p_bool elseClosed = false;
   p_bool locked = false;
   p_bool hasElse = false;

   _comptr mainCommand;
   _genptr<p_bool> mainCondition;
   _comptr elseCommand;

   std::vector<_comptr> elseIfCommands;
   std::vector<_genptr<p_bool>> elseIfConditions;
};


struct ConditionContext
{
public:
   void add(_comptr* pntr);
   void addClosed(_comptr* pntr);
   void deleteClosedUnits();
   void deleteLast();
   void lockLast();
   p_bool isExpandable() const;
   void addElse(_comptr& com, const p_int line);
   void addEmptyElse(const p_int line);
   void addElseIf(_genptr<p_bool>& cond, _comptr& com, const p_int line);
   void setMain(_genptr<p_bool>& mainCond);
   void setMain(_comptr& mainCom, _genptr<p_bool>& mainCond);

private:
   std::vector<ConditionUnit> units;
};

}

#endif /* COM_PARSE_UNIT_H */
