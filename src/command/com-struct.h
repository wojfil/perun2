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

#include "com-listener.h"
#include "../util.h"
#include "../attribute.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-arg.h"


namespace uro::comm
{

struct CS_Pair : Command_L
{
public:
   CS_Pair(Command* fst, Command* snd, Uroboros* uro)
      : first(fst), second(snd), Command_L(uro) {};

   ~CS_Pair() {
      delete first;
      delete second;
   };

   void run() override;

private:
   Command* first;
   Command* second;
};


struct CS_Block : Command_L
{
public:
   CS_Block(std::vector<Command*> coms, Uroboros* uro)
      : commands(coms), length(coms.size()), Command_L(uro) { };

   ~CS_Block() {
      deleteVector(commands);
   };

   void run() override;

private:
   std::vector<Command*> commands;
   const _size length;
};


struct CS_Times : Command_L
{
public:
   CS_Times(Generator<_num>* ts, Command* com, Aggregate* aggr, Uroboros* uro)
      : aggregate(aggr), times(ts), command(com), Command_L(uro)  { };

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


struct CS_While : Command_L
{
public:
   CS_While(Generator<_boo>* cond, Command* com, Aggregate* aggr, Uroboros* uro)
      : aggregate(aggr), condition(cond), command(com), Command_L(uro)  { };

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


struct CS_TimeLoop : Command_L
{
public:
   CS_TimeLoop(Generator<_tlist>* tList, Command* com, Aggregate* aggr, Uroboros* uro)
      : aggregate(aggr), timeList(tList), command(com), Command_L(uro)  { };

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


struct CS_NumberLoop : Command_L
{
public:
   CS_NumberLoop(Generator<_nlist>* nList, Command* com, Aggregate* aggr, Uroboros* uro)
      : aggregate(aggr), numberList(nList), command(com), Command_L(uro)  { };

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
   CS_StringLoop(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr,
      Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), string(str), aggregate(aggr) { };

   CS_StringLoop(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr,
      const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), string(str), aggregate(aggr) { };

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
   CS_DefinitionLoop(_def* def, Command* com, Attribute* attr, Aggregate* aggr,
      Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), definition(def), aggregate(aggr) { };

   CS_DefinitionLoop(_def* def, Command* com, Attribute* attr, Aggregate* aggr,
      const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), definition(def), aggregate(aggr) { };

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
   CS_ListLoop(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr,
      Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), list(li), aggregate(aggr) { };

   CS_ListLoop(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr,
      const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), list(li), aggregate(aggr) { };

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
   CS_InsideString(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr,
      Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), string(str), aggregate(aggr) { };

   CS_InsideString(Generator<_str>* str, Command* com, Attribute* attr, Aggregate* aggr,
      const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), string(str), aggregate(aggr) { };

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
   CS_InsideDefinition(_def* def, Command* com, Attribute* attr, Aggregate* aggr,
      Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), definition(def), aggregate(aggr) { };

   CS_InsideDefinition(_def* def, Command* com, Attribute* attr, Aggregate* aggr,
      const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), definition(def), aggregate(aggr) { };

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
   CS_InsideList(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr,
      Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), list(li), aggregate(aggr) { };

   CS_InsideList(Generator<_list>* li, Command* com, Attribute* attr, Aggregate* aggr,
      const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), list(li), aggregate(aggr) { };

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

}

#endif /* COM_STRUCT_H */

