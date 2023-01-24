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

#ifndef COM_STRUCT_H
#define COM_STRUCT_H

#include "com-listener.h"
#include "../util.h"
#include "../attribute.h"
#include "../datatype/patterns.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-arg.h"


namespace uro::comm
{

struct CS_Pair : Command_L
{
public:
   CS_Pair(_comptr& fst, _comptr& snd, _uro& uro)
      : first(std::move(fst)), second(std::move(snd)), Command_L(uro) { };

   void run() override;

private:
   _comptr first;
   _comptr second;
};


struct CS_Block : Command_L
{
public:
   CS_Block(std::vector<_comptr>& coms, _uro& uro)
      : length(coms.size()), Command_L(uro) 
   { 
      langutil::transferUniquePtrs(coms, commands);
   };

   void run() override;

private:
   std::vector<_comptr> commands;
   const _size length;
};


struct CS_Times : Command_L
{
public:
   CS_Times(_genptr<_num>& ts, _comptr& com, _aggrptr& aggr, _uro& uro)
      : aggregate(std::move(aggr)), times(std::move(ts)), command(std::move(com)), Command_L(uro)  { };

   void run() override;

private:
   _aggrptr aggregate;
   _genptr<_num> times;
   _comptr command;
};


struct CS_While : Command_L
{
public:
   CS_While(_genptr<_bool>& cond, _comptr& com, _aggrptr& aggr, _uro& uro)
      : aggregate(std::move(aggr)), condition(std::move(cond)), command(std::move(com)), Command_L(uro)  { };

   void run() override;

private:
   _aggrptr aggregate;
   _genptr<_bool> condition;
   _comptr command;
};


struct CS_TimeLoop : Command_L
{
public:
   CS_TimeLoop(_genptr<_tlist>& tList, _comptr& com, _aggrptr& aggr, _uro& uro)
      : aggregate(std::move(aggr)), timeList(std::move(tList)), command(std::move(com)), Command_L(uro)  { };

   void run() override;

private:
   _aggrptr aggregate;
   _genptr<_tlist> timeList;
   _comptr command;
};


struct CS_NumberLoop : Command_L
{
public:
   CS_NumberLoop(_genptr<_nlist>& nList, _comptr& com, _aggrptr& aggr, _uro& uro)
      : aggregate(std::move(aggr)), numberList(std::move(nList)), command(std::move(com)), Command_L(uro)  { };

   void run() override;

private:
   _aggrptr aggregate;
   _genptr<_nlist> numberList;
   _comptr command;
};


struct CS_StringLoop : IterationLoop
{
public:
   CS_StringLoop(_genptr<_str>& str, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      _attrptr& memAttr, const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), string(std::move(str)), aggregate(std::move(aggr)) { };

   CS_StringLoop(_genptr<_str>& str, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, hasmem, uro), string(std::move(str)), aggregate(std::move(aggr)) { };

   void run() override;

private:
   _genptr<_str> string;
   _aggrptr aggregate;
};


struct CS_DefinitionLoop : IterationLoop
{
public:
   CS_DefinitionLoop(_defptr& def, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      _attrptr& memAttr, const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), definition(std::move(def)), aggregate(std::move(aggr)) { };

   CS_DefinitionLoop(_defptr& def, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, hasmem, uro), definition(std::move(def)), aggregate(std::move(aggr)) { };

   void run() override;

private:
   _defptr definition;
   _aggrptr aggregate;
};


struct CS_ListLoop : IterationLoop
{
public:
   CS_ListLoop(_genptr<_list>& li, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      _attrptr& memAttr, const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), list(std::move(li)), aggregate(std::move(aggr)) { };

   CS_ListLoop(_genptr<_list>& li, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, hasmem, uro), list(std::move(li)), aggregate(std::move(aggr)) { };

   void run() override;

private:
   _genptr<_list> list;
   _aggrptr aggregate;
};


struct CS_InsideString : IterationLoop
{
public:
   CS_InsideString(_genptr<_str>& str, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      _attrptr& memAttr, const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), string(std::move(str)), aggregate(std::move(aggr)) { };

   CS_InsideString(_genptr<_str>& str, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, hasmem, uro), string(std::move(str)), aggregate(std::move(aggr)) { };

   void run() override;

private:
   _genptr<_str> string;
   _aggrptr aggregate;
   _str prevLocation;
};


struct CS_InsideDefinition : IterationLoop
{
public:
   CS_InsideDefinition(_defptr& def, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      _attrptr& memAttr, const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), definition(std::move(def)), aggregate(std::move(aggr)) { };

   CS_InsideDefinition(_defptr& def, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, hasmem, uro), definition(std::move(def)), aggregate(std::move(aggr)) { };

   void run() override;

private:
   _defptr definition;
   _aggrptr aggregate;
   _str prevLocation;
};


struct CS_InsideList : IterationLoop
{
public:
   CS_InsideList(_genptr<_list>& li, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      _attrptr& memAttr, const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), list(std::move(li)), aggregate(std::move(aggr)) { };

   CS_InsideList(_genptr<_list>& li, _comptr& com, _attrptr& attr, _aggrptr& aggr,
      const _bool& hasmem, _uro& uro)
      : IterationLoop(com, attr, hasmem, uro), list(std::move(li)), aggregate(std::move(aggr)) { };

   void run() override;

private:
   _genptr<_list> list;
   _aggrptr aggregate;
   _str prevLocation;
};

}

#endif /* COM_STRUCT_H */

