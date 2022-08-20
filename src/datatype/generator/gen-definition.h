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

#include "gen-list.h"
#include "gen-os.h"
#include "../cast.h"
#include "../../attribute.h"
#include "../../attr-memory.h"
#include "../../uroboros.h"
#include <algorithm>


struct DefFilter : _def
{
public:
   DefFilter(_def* def, Uroboros* uro);
   ~DefFilter();
   void reset() override;
   _fdata* getDataPtr();

protected:
   Uroboros* uroboros;
   _boo first;
   _def* definition;
};


struct Filter_WhereDef : DefFilter
{
public:
   Filter_WhereDef(_def* def, Generator<_boo>* cond, Attribute* attr, const _boo& hasMem, Uroboros* uro)
      : DefFilter(def, uro), condition(cond), attribute(attr), finished(true), inner(&uro->vars.inner),
        hasMemory(hasMem), attrMemory(AttributeMemory(attr, &uro->vars.inner)), hasAttribute(true) {};

   Filter_WhereDef(_def* def, Generator<_boo>* cond, Uroboros* uro)
      : DefFilter(def, uro), condition(cond), attribute(nullptr), finished(true), inner(&uro->vars.inner),
        hasMemory(false), attrMemory(AttributeMemory(&uro->vars.inner)), hasAttribute(false) {};

   ~Filter_WhereDef() {
      delete condition;
      if (hasAttribute) {
         delete attribute;
      }
   }

   _boo hasNext() override;
   void reset() override;

private:
   InnerVariables* inner;
   _boo finished;
   Generator<_boo>* condition;
   Attribute* attribute;
   const _boo hasMemory;
   const _boo hasAttribute;
   AttributeMemory attrMemory;
   _numi index;
};


struct Filter_LimitDef : DefFilter
{
public:
   Filter_LimitDef(_def* def, Generator<_num>* num, Uroboros* uro)
      : DefFilter(def, uro), number(num) { };

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
   Filter_SkipDef(_def* def, Generator<_num>* num, Uroboros* uro)
      : DefFilter(def, uro), number(num), inner(&uro->vars.inner) { };

   ~Filter_SkipDef() {
      delete number;
   }

   _boo hasNext() override;

private:
   InnerVariables* inner;
   _size counter;
   _size limit;
   Generator<_num>* number;
};


struct Filter_EveryDef : DefFilter
{
public:
   Filter_EveryDef(_def* def, Generator<_num>* num, Uroboros* uro)
      : DefFilter(def, uro), number(num), inner(&uro->vars.inner) { };

   ~Filter_EveryDef() {
      delete number;
   }

   _boo hasNext() override;

private:
   InnerVariables* inner;
   _size counter;
   _size limit;
   Generator<_num>* number;
   _numi index;
};


struct Join_DefStr : _def
{

public:
   Join_DefStr(_def* lef, Generator<_str>* rig, Uroboros* uro)
        : left(lef), right(rig), taken(false), uroboros(uro) {};
   ~Join_DefStr();

   void reset() override;
   _boo hasNext() override;

private:
   Uroboros* uroboros;
   _def* left;
   Generator<_str>* right;
   _boo taken;
};


struct Join_StrDef : _def
{

public:
   Join_StrDef(Generator<_str>* lef, _def* rig, Uroboros* uro)
        : left(lef), right(rig), first(true), uroboros(uro) {};
   ~Join_StrDef();

   void reset() override;
   _boo hasNext() override;

private:
   Uroboros* uroboros;
   Generator<_str>* left;
   _def* right;
   _boo first;
};


struct Join_DefList : _def
{

public:
   Join_DefList(_def* lef, Generator<_list>* rig, Uroboros* uro)
        : left(lef), right(rig), taken(false), uroboros(uro) {};
   ~Join_DefList();

   void reset() override;
   _boo hasNext() override;

private:
   Uroboros* uroboros;
   _def* left;
   Generator<_list>* right;
   _boo taken;
   _size length;
   _size index;
   _list values;
};


struct Join_ListDef : _def
{

public:
   Join_ListDef(Generator<_list>* lef, _def* rig, Uroboros* uro)
        : left(lef), right(rig), first(true), taken(false), uroboros(uro) {};
   ~Join_ListDef();

   void reset() override;
   _boo hasNext() override;

private:
   Uroboros* uroboros;
   Generator<_list>* left;
   _def* right;
   _boo first;
   _boo taken;
   _size length;
   _size index;
   _list values;
};


struct Join_DefDef : _def
{

public:
   Join_DefDef(_def* lef, _def* rig, Uroboros* uro)
        : left(lef), right(rig), first(true), taken(false), uroboros(uro) {};
   ~Join_DefDef();

   void reset() override;
   _boo hasNext() override;

private:
   Uroboros* uroboros;
   _def* left;
   _def* right;
   _boo first;
   _boo taken;
};


struct OrderByCast : _def
{
public:
   OrderByCast(Generator<_list>* b, Attribute* attr, const _boo& hasMem, Uroboros* uro)
      : base(b), first(true), attribute(attr), hasMemory(hasMem), attrMemory(AttributeMemory(attr, &uro->vars.inner)),
      uroboros(uro), inner(&uro->vars.inner) { };

   ~OrderByCast() {
      delete base;
   }

   void reset() override;
   _boo hasNext() override;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   Generator<_list>* base;
   _list values;
   _size index;
   _numi indexAsNumber;
   _size length;
   _boo first;
   Attribute* attribute;
   const _boo hasMemory;
   AttributeMemory attrMemory;
   _numi prevIndex;
   _str prevThis;
};


// ternary and binary works with Definitions in its own way
// so instead of using templates from 'gen.generic.h'
// here are special structs

struct DefTernary : _def
{
public:
   DefTernary(Generator<_boo>* cond, _def* le, _def* ri)
      : condition(cond), left(le), right(ri), first(true), isLeft(true) {};

   ~DefTernary() {
      delete condition;
      delete left;
      delete right;
   }

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_boo>* condition;
   _def* left;
   _def* right;
   _boo first;
   _boo isLeft;
};


struct DefBinary : _def
{
public:
   DefBinary(Generator<_boo>* cond, _def* le)
      : condition(cond), left(le), first(true) {};

   ~DefBinary() {
      delete condition;
      delete left;
   }

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_boo>* condition;
   _def* left;
   _boo first;
};




#endif // GEN-DEFINITION_H_INCLUDED
