/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_PARSE_UNIT_H
#define COM_PARSE_UNIT_H

#include "../datatype/primitives.h"
#include "../datatype/generator.h"
#include "com.h"
#include <vector>


namespace uro::comm
{

struct CS_Condition;

struct ConditionUnit
{
public:
   ConditionUnit(_comptr* ptr);

   void finish();
   void setElse(_comptr& com);
   void addElseIf(_comptr& com, _genptr<_bool>& cond);

   _bool isClosed() const;
   _bool isElseClosed() const;
   _bool isLocked() const;

   void close();
   void closeElse();
   void lock();

   void setMain(_genptr<_bool>& mainCond);
   void setMain(_comptr& mainCom, _genptr<_bool>& mainCond);

   _comptr* const pointer;

private:
   _bool closed = false;
   _bool elseClosed = false;
   _bool locked = false;
   _bool hasElse = false;

   _comptr mainCommand;
   _genptr<_bool> mainCondition;
   _comptr elseCommand;

   std::vector<_comptr> elseIfCommands;
   std::vector<_genptr<_bool>> elseIfConditions;
};


struct ConditionContext
{
public:
   void add(_comptr* pntr);
   void addClosed(_comptr* pntr);
   void deleteClosedUnits();
   void deleteLast();
   void lockLast();
   _bool isExpandable() const;
   void addElse(_comptr& com, const _int line);
   void addEmptyElse(const _int line);
   void addElseIf(_genptr<_bool>& cond, _comptr& com, const _int line);
   void setMain(_genptr<_bool>& mainCond);
   void setMain(_comptr& mainCom, _genptr<_bool>& mainCond);

private:
   std::vector<ConditionUnit> units;
};

}

#endif /* COM_PARSE_UNIT_H */
