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

#ifndef COM_STRUCT_H
#define COM_STRUCT_H

#include "com.h"
#include "../util.h"
#include "../attribute.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-arg.h"


struct CS_Pair : Command
{
public:
   CS_Pair(Command* fst, Command* snd) : first(fst), second(snd){};

   ~CS_Pair() {
      delete first;
      delete second;
   };

   void run() override;

private:
   Command* first;
   Command* second;
};


struct CS_Block : Command
{
public:
   CS_Block(std::vector<Command*> coms) : commands(coms),
      length(coms.size()) { };

   ~CS_Block() {
      deleteVector(commands);
   };

   void run() override;

private:
   std::vector<Command*> commands;
   const _size length;
};


struct CS_Times : Command
{
public:
   CS_Times(Generator<_num>* ts, Command* com, Aggregate* aggr)
      : aggregate(aggr), times(ts), command(com) { };

   ~CS_Times() {
      delete times;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Aggregate* aggregate;
   Generator<_num>* times;
   Command* command;
};


struct CS_While : Command
{
public:
   CS_While(Generator<_boo>* cond, Command* com, Aggregate* aggr)
      : aggregate(aggr), condition(cond), command(com) { };

   ~CS_While() {
      delete condition;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Aggregate* aggregate;
   Generator<_boo>* condition;
   Command* command;
};


struct CS_TimeLoop : Command
{
public:
   CS_TimeLoop(Generator<_tlist>* tList, Command* com, Aggregate* aggr)
      : aggregate(aggr), timeList(tList), command(com) { };

   ~CS_TimeLoop() {
      delete timeList;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Aggregate* aggregate;
   Generator<_tlist>* timeList;
   Command* command;
};


struct CS_NumberLoop : Command
{
public:
   CS_NumberLoop(Generator<_nlist>* nList, Command* com, Aggregate* aggr)
      : aggregate(aggr), numberList(nList), command(com) { };

   ~CS_NumberLoop() {
      delete numberList;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Aggregate* aggregate;
   Generator<_nlist>* numberList;
   Command* command;
};


struct CS_StringLoop : IterationLoop
{
public:
   CS_StringLoop(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr, Attribute* memAttr, const _boo& hasmem)
      : IterationLoop(com, attr, memAttr, hasmem), string(str), aggregate(aggr) { };

   CS_StringLoop(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr, const _boo& hasmem)
      : IterationLoop(com, attr, hasmem), string(str), aggregate(aggr) { };

   ~CS_StringLoop() {
      delete string;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Generator<_str>* string;
   Aggregate* aggregate;
};


struct CS_DefinitionLoop : IterationLoop
{
public:
   CS_DefinitionLoop(_def* def, Command* com, Attribute* attr, Aggregate* aggr, Attribute* memAttr, const _boo& hasmem)
      : IterationLoop(com, attr, memAttr, hasmem), definition(def), aggregate(aggr) { };

   CS_DefinitionLoop(_def* def, Command* com, Attribute* attr, Aggregate* aggr, const _boo& hasmem)
      : IterationLoop(com, attr, hasmem), definition(def), aggregate(aggr) { };

   ~CS_DefinitionLoop() {
      delete definition;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   _def* definition;
   Aggregate* aggregate;
};


struct CS_ListLoop : IterationLoop
{
public:
   CS_ListLoop(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr, Attribute* memAttr, const _boo& hasmem)
      : IterationLoop(com, attr, memAttr, hasmem), list(li), aggregate(aggr) { };

   CS_ListLoop(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr, const _boo& hasmem)
      : IterationLoop(com, attr, hasmem), list(li), aggregate(aggr) { };

   ~CS_ListLoop() {
      delete list;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Generator<_list>* list;
   Aggregate* aggregate;
};


struct CS_InsideString : IterationLoop
{
public:
   CS_InsideString(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr, Attribute* memAttr, const _boo& hasmem)
      : IterationLoop(com, attr, memAttr, hasmem), string(str), aggregate(aggr) { };

   CS_InsideString(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr, const _boo& hasmem)
      : IterationLoop(com, attr, hasmem), string(str), aggregate(aggr) { };

   ~CS_InsideString() {
      delete string;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Generator<_str>* string;
   Aggregate* aggregate;
   _str prevLocation;
};


struct CS_InsideDefinition : IterationLoop
{
public:
   CS_InsideDefinition(_def* def, Command* com, Attribute* attr, Aggregate* aggr, Attribute* memAttr, const _boo& hasmem)
      : IterationLoop(com, attr, memAttr, hasmem), definition(def), aggregate(aggr) { };

   CS_InsideDefinition(_def* def, Command* com, Attribute* attr, Aggregate* aggr, const _boo& hasmem)
      : IterationLoop(com, attr, hasmem), definition(def), aggregate(aggr) { };

   ~CS_InsideDefinition() {
      delete definition;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   _def* definition;
   Aggregate* aggregate;
   _str prevLocation;
};


struct CS_InsideList : IterationLoop
{
public:
   CS_InsideList(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr, Attribute* memAttr, const _boo& hasmem)
      : IterationLoop(com, attr, memAttr, hasmem), list(li), aggregate(aggr) { };

   CS_InsideList(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr, const _boo& hasmem)
      : IterationLoop(com, attr, hasmem), list(li), aggregate(aggr) { };

   ~CS_InsideList() {
      delete list;
      delete command;
      delete aggregate;
   };

   void run() override;

private:
   Generator<_list>* list;
   Aggregate* aggregate;
   _str prevLocation;
};


#endif /* COM_STRUCT_H */

