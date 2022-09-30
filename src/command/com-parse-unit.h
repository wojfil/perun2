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
   ConditionUnit(CS_Condition* pntr);

   void finish();
   void setElse(Command* com);
   void addElseIf(Command* com, Generator<_bool>* cond);

   _bool isClosed() const;
   _bool isElseClosed() const;
   _bool isLocked() const;

   void close();
   void closeElse();
   void lock();

   CS_Condition* const pointer;


private:
   _bool closed = false;
   _bool elseClosed = false;
   _bool locked = false;

   Command* elseCommand = nullptr;
   std::vector<Command*> elseIfCommands;
   std::vector<Generator<_bool>*> elseIfConditions;
};


struct ConditionContext
{
public:

   void add(CS_Condition* pntr);
   void addClosed(CS_Condition* pntr);
   void deleteClosedUnits();
   void deleteLast();
   void lockLast();
   _bool isExpandable() const;
   void addElse(Command* com, const _int& line);
   void addEmptyElse(const _int& line);
   void addElseIf(Generator<_bool>* cond, Command* com, const _int& line);


private:
   std::vector<ConditionUnit> units;
};

}

#endif /* COM_PARSE_UNIT_H */
