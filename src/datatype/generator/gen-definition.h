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

#ifndef GEN_DEFINITION_H_INCLUDED
#define GEN_DEFINITION_H_INCLUDED

#include "gen-list.h"
#include "gen-os.h"
#include "../cast.h"
#include "../../attribute.h"
#include "../../attr-memory.h"
#include "../../uroboros.h"
#include <algorithm>


namespace uro::gen
{

struct DefFilter : _def
{
public:
   DefFilter(_def* def, uro::Uroboros* uro);
   ~DefFilter();
   void reset() override;
   _fdata* getDataPtr();

protected:
   uro::Uroboros* uroboros;
   _bool first;
   _def* definition;
};


struct Filter_WhereDef : DefFilter
{
public:
   Filter_WhereDef(_def* def, Generator<_bool>* cond, Attribute* attr, const _bool& hasMem, uro::Uroboros* uro)
      : DefFilter(def, uro), condition(cond), attribute(attr), finished(true), inner(&uro->vars.inner),
        hasMemory(hasMem), attrMemory(AttributeMemory(attr, &uro->vars.inner)), hasAttribute(true) {};

   Filter_WhereDef(_def* def, Generator<_bool>* cond, uro::Uroboros* uro)
      : DefFilter(def, uro), condition(cond), attribute(nullptr), finished(true), inner(&uro->vars.inner),
        hasMemory(false), attrMemory(AttributeMemory(&uro->vars.inner)), hasAttribute(false) {};

   ~Filter_WhereDef() {
      delete condition;
      if (hasAttribute) {
         delete attribute;
      }
   }

   _bool hasNext() override;
   void reset() override;

private:
   uro::InnerVariables* inner;
   _bool finished;
   Generator<_bool>* condition;
   Attribute* attribute;
   const _bool hasMemory;
   const _bool hasAttribute;
   AttributeMemory attrMemory;
   _numi index;
};


struct DefinitionChain : _def
{
public:
   DefinitionChain(_def* def, uro::Uroboros* uro);
   ~DefinitionChain();

   _bool hasNext() override;
   void reset() override;

private:
   uro::InnerVariables* inner;
   _def* definition;
   _nint index = 0LL;
   _bool finished = true;
};


struct Filter_LimitDef : DefFilter
{
public:
   Filter_LimitDef(_def* def, Generator<_num>* num, uro::Uroboros* uro)
      : DefFilter(def, uro), number(num) { };

   ~Filter_LimitDef() {
      delete number;
   }

   _bool hasNext() override;

private:
   _nint counter;
   _nint limit;
   Generator<_num>* number;
};


struct Filter_SkipDef : DefFilter
{
public:
   Filter_SkipDef(_def* def, Generator<_num>* num, uro::Uroboros* uro)
      : DefFilter(def, uro), number(num), inner(&uro->vars.inner) { };

   ~Filter_SkipDef() {
      delete number;
   }

   _bool hasNext() override;

private:
   uro::InnerVariables* inner;
   _nint counter;
   _nint limit;
   Generator<_num>* number;
};


struct Filter_EveryDef : DefFilter
{
public:
   Filter_EveryDef(_def* def, Generator<_num>* num, uro::Uroboros* uro)
      : DefFilter(def, uro), number(num), inner(&uro->vars.inner) { };

   ~Filter_EveryDef() {
      delete number;
   }

   _bool hasNext() override;

private:
   uro::InnerVariables* inner;
   _nint counter;
   _nint limit;
   Generator<_num>* number;
   _numi index;
};


struct Join_DefStr : _def
{

public:
   Join_DefStr(_def* lef, Generator<_str>* rig, uro::Uroboros* uro)
        : left(lef), right(rig), taken(false), uroboros(uro) {};
   ~Join_DefStr();

   void reset() override;
   _bool hasNext() override;

private:
   uro::Uroboros* uroboros;
   _def* left;
   Generator<_str>* right;
   _bool taken;
};


struct Join_StrDef : _def
{

public:
   Join_StrDef(Generator<_str>* lef, _def* rig, uro::Uroboros* uro)
        : left(lef), right(rig), first(true), uroboros(uro) {};
   ~Join_StrDef();

   void reset() override;
   _bool hasNext() override;

private:
   uro::Uroboros* uroboros;
   Generator<_str>* left;
   _def* right;
   _bool first;
};


struct Join_DefList : _def
{

public:
   Join_DefList(_def* lef, Generator<_list>* rig, uro::Uroboros* uro)
        : left(lef), right(rig), taken(false), uroboros(uro) {};
   ~Join_DefList();

   void reset() override;
   _bool hasNext() override;

private:
   uro::Uroboros* uroboros;
   _def* left;
   Generator<_list>* right;
   _bool taken;
   _size length;
   _size index;
   _list values;
};


struct Join_ListDef : _def
{

public:
   Join_ListDef(Generator<_list>* lef, _def* rig, uro::Uroboros* uro)
        : left(lef), right(rig), first(true), taken(false), uroboros(uro) {};
   ~Join_ListDef();

   void reset() override;
   _bool hasNext() override;

private:
   uro::Uroboros* uroboros;
   Generator<_list>* left;
   _def* right;
   _bool first;
   _bool taken;
   _size length;
   _size index;
   _list values;
};


struct Join_DefDef : _def
{

public:
   Join_DefDef(_def* lef, _def* rig, uro::Uroboros* uro)
        : left(lef), right(rig), first(true), taken(false), uroboros(uro) {};
   ~Join_DefDef();

   void reset() override;
   _bool hasNext() override;

private:
   uro::Uroboros* uroboros;
   _def* left;
   _def* right;
   _bool first;
   _bool taken;
};


// ternary and binary works with Definitions in its own way
// so instead of using templates from 'gen.generic.h'
// here are special structs

struct DefTernary : _def
{
public:
   DefTernary(Generator<_bool>* cond, _def* le, _def* ri)
      : condition(cond), left(le), right(ri), first(true), isLeft(true) {};

   ~DefTernary() {
      delete condition;
      delete left;
      delete right;
   }

   void reset() override;
   _bool hasNext() override;

private:
   Generator<_bool>* condition;
   _def* left;
   _def* right;
   _bool first;
   _bool isLeft;
};


struct DefBinary : _def
{
public:
   DefBinary(Generator<_bool>* cond, _def* le)
      : condition(cond), left(le), first(true) {};

   ~DefBinary() {
      delete condition;
      delete left;
   }

   void reset() override;
   _bool hasNext() override;

private:
   Generator<_bool>* condition;
   _def* left;
   _bool first;
};

}


#endif // GEN_DEFINITION_H_INCLUDED
