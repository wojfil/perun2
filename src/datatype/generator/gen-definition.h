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

#ifndef GEN-DEFINITION_H_INCLUDED
#define GEN-DEFINITION_H_INCLUDED

#include <algorithm>
#include "gen-list.h"
#include "../cast.h"
#include "../../attribute.h"
#include "../../attr-memory.h"
#include "gen-os.h"


struct DefFilter : Definition
{
public:
   DefFilter(_def* def) : first(true), definition(def) {};
   ~DefFilter() {
      delete definition;
   }
   void reset() override;

protected:
   _boo first;
   _def* definition;
};


struct Filter_WhereDef : DefFilter
{
public:
   Filter_WhereDef(_def* def, Generator<_boo>* cond, Attribute* attr, const _boo& hasMem)
      : DefFilter(def), condition(cond), attribute(attr), finished(true),
        hasMemory(hasMem), attrMemory(AttributeMemory(attr)) {};

   ~Filter_WhereDef() {
      delete condition;
      delete attribute;
   }

   _boo hasNext() override;
   void reset() override;

private:
   _boo finished;
   Generator<_boo>* condition;
   Attribute* attribute;
   const _boo hasMemory;
   AttributeMemory attrMemory;
   _num index;
};


struct Filter_LimitDef : DefFilter
{
public:
   Filter_LimitDef(_def* def, Generator<_num>* num)
      : DefFilter(def), number(num) { };

   ~Filter_LimitDef() {
      delete number;
   }

   _boo hasNext() override;

private:
   _size counter;
   _size limit;
   Generator<_num>* number;
};


struct Filter_SkipDef : DefFilter
{
public:
   Filter_SkipDef(_def* def, Generator<_num>* num)
      : DefFilter(def), number(num) { };

   ~Filter_SkipDef() {
      delete number;
   }

   _boo hasNext() override;

private:
   _size counter;
   _size limit;
   Generator<_num>* number;
};


struct Filter_EveryDef : DefFilter
{
public:
   Filter_EveryDef(_def* def, Generator<_num>* num)
      : DefFilter(def), number(num) { };

   ~Filter_EveryDef() {
      delete number;
   }

   _boo hasNext() override;

private:
   _size counter;
   _size limit;
   Generator<_num>* number;
   _num index;
};


struct Join_DefStr : Definition
{

public:
   Join_DefStr(_def* lef, Generator<_str>* rig)
        : left(lef), right(rig), taken(false) {};
   ~Join_DefStr();

   void reset() override;
   _boo hasNext() override;

private:
   _def* left;
   Generator<_str>* right;
   _boo taken;
};


struct Join_StrDef : Definition
{

public:
   Join_StrDef(Generator<_str>* lef, _def* rig)
        : left(lef), right(rig), first(true) {};
   ~Join_StrDef();

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_str>* left;
   _def* right;
   _boo first;
};


struct Join_DefList : Definition
{

public:
   Join_DefList(_def* lef, Generator<_list>* rig)
        : left(lef), right(rig), taken(false) {};
   ~Join_DefList();

   void reset() override;
   _boo hasNext() override;

private:
   _def* left;
   Generator<_list>* right;
   _boo taken;
   _size length;
   _size index;
   _list values;
};


struct Join_ListDef : Definition
{

public:
   Join_ListDef(Generator<_list>* lef, _def* rig)
        : left(lef), right(rig), first(true), taken(false) {};
   ~Join_ListDef();

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_list>* left;
   _def* right;
   _boo first;
   _boo taken;
   _size length;
   _size index;
   _list values;
};


struct Join_DefDef : Definition
{

public:
   Join_DefDef(_def* lef, _def* rig)
        : left(lef), right(rig), first(true), taken(false) {};
   ~Join_DefDef();

   void reset() override;
   _boo hasNext() override;

private:
   _def* left;
   _def* right;
   _boo first;
   _boo taken;
};


struct OrderByCast : _def
{
public:
   OrderByCast(Generator<_list>* b, Attribute* attr, const _boo& hasMem)
      : base(b), first(true), attribute(attr), hasMemory(hasMem), attrMemory(AttributeMemory(attr)) { };

   ~OrderByCast() {
      delete base;
   }

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_list>* base;
   _list values;
   _size index;
   _num indexAsNumber;
   _size length;
   _boo first;
   Attribute* attribute;
   const _boo hasMemory;
   AttributeMemory attrMemory;
   _num prevIndex;
   _str prevThis;
};




#endif // GEN-DEFINITION_H_INCLUDED
