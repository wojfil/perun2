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
#include <deque>


namespace uro::gen
{

struct DefFilter : _def
{
public:
   DefFilter(_defptr& def, Uroboros& uro);
   void reset() override;
   _fdata* getDataPtr();

protected:
   Uroboros& uroboros;
   _bool first;
   _defptr definition;
};


struct Filter_WhereDef : DefFilter
{
public:
   Filter_WhereDef(_defptr& def, _genptr<_bool>& cond, _attrptr& attr, const _bool& hasMem, Uroboros& uro);
   Filter_WhereDef(_defptr& def, _genptr<_bool>& cond, Uroboros& uro);

   _bool hasNext() override;
   void reset() override;

private:
   InnerVariables& inner;
   _bool finished;
   _genptr<_bool> condition;
   _attrptr attribute;
   const _bool hasMemory;
   const _bool hasAttribute;
   AttributeMemory attrMemory;
   _numi index;
};


struct DefinitionChain : _def
{
public:
   DefinitionChain(_defptr& def, Uroboros& uro);

   _bool hasNext() override;
   void reset() override;

private:
   InnerVariables& inner;
   _defptr definition;
   _nint index = 0LL;
   _bool finished = true;
};


struct LocationVessel : Generator<_str>
{
public:
   LocationVessel(const _bool& abs, Uroboros& uro);
   _str getValue() override;
   const _str& getRawValue() const;
   void setValue(const _str& val);

private:
   InnerVariables& inner;
   _str value;
   const _bool isAbsolute;
};


struct NestedDefiniton : _def
{
public:
   NestedDefiniton(LocationVessel& ves, _defptr& def, _defptr& locs, Uroboros& uro, const _bool& abs, const _bool& fin);
   _bool hasNext() override;
   void reset() override;
   _fdata* getDataPtr();

private:
   LocationVessel& vessel;
   _defptr definition;
   _defptr locations;
   InnerVariables& inner;
   _numi index;
   _bool defOpened = false;
   _bool locsOpened = false;
   const _bool isAbsolute;
   const _bool isFinal;
};


struct Filter_LimitDef : DefFilter
{
public:
   Filter_LimitDef(_defptr& def, _genptr<_num>& num, Uroboros& uro)
      : DefFilter(def, uro), number(std::move(num)) { };

   _bool hasNext() override;

private:
   _nint counter;
   _nint limit;
   _genptr<_num> number;
};


struct Filter_SkipDef : DefFilter
{
public:
   Filter_SkipDef(_defptr& def, _genptr<_num>& num, Uroboros& uro)
      : DefFilter(def, uro), number(std::move(num)), inner(uro.vars.inner) { };

   _bool hasNext() override;

private:
   InnerVariables& inner;
   _nint counter;
   _nint limit;
   _genptr<_num> number;
};


struct Filter_EveryDef : DefFilter
{
public:
   Filter_EveryDef(_defptr& def, _genptr<_num>& num, Uroboros& uro)
      : DefFilter(def, uro), number(std::move(num)), inner(uro.vars.inner) { };


   _bool hasNext() override;

private:
   InnerVariables& inner;
   _nint counter;
   _nint limit;
   _genptr<_num> number;
   _numi index;
};


struct Filter_FinalDef : DefFilter
{
public:
   Filter_FinalDef(_defptr& def, _genptr<_num>& num, Uroboros& uro)
      : DefFilter(def, uro), number(std::move(num)), inner(uro.vars.inner) { };


   _bool hasNext() override;

private:
   InnerVariables& inner;
   _genptr<_num> number;

   std::deque<_str> values;
   _nint length;
   _nint index;
};



struct Join_DefStr : _def
{

public:
   Join_DefStr(_defptr& lef, _genptr<_str>& rig, Uroboros& uro)
        : left(std::move(lef)), right(std::move(rig)), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   Uroboros& uroboros;
   _defptr left;
   _genptr<_str> right;
   _bool taken;
};


struct Join_StrDef : _def
{

public:
   Join_StrDef(_genptr<_str>& lef, _defptr& rig, Uroboros& uro)
        : left(std::move(lef)), right(std::move(rig)), first(true), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   Uroboros& uroboros;
   _genptr<_str> left;
   _defptr right;
   _bool first;
};


struct Join_DefList : _def
{

public:
   Join_DefList(_defptr& lef, _genptr<_list>& rig, Uroboros& uro)
        : left(std::move(lef)), right(std::move(rig)), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   Uroboros& uroboros;
   _defptr left;
   _genptr<_list> right;
   _bool taken;
   _size length;
   _size index;
   _list values;
};


struct Join_ListDef : _def
{

public:
   Join_ListDef(_genptr<_list>& lef, _defptr& rig, Uroboros& uro)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   Uroboros& uroboros;
   _genptr<_list> left;
   _defptr right;
   _bool first;
   _bool taken;
   _size length;
   _size index;
   _list values;
};


struct Join_DefDef : _def
{

public:
   Join_DefDef(_defptr& lef, _defptr& rig, Uroboros& uro)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   Uroboros& uroboros;
   _defptr left;
   _defptr right;
   _bool first;
   _bool taken;
};


struct DefinitionSuffix : _def
{
public:
   DefinitionSuffix(_defptr& def, Uroboros& uro, const _str& suf, const _bool& abs, const _bool& fin);

   _bool hasNext() override;
   void reset() override;

private:
   Uroboros& uroboros;
   InnerVariables& inner;
   _defptr definition;
   _bool first = true;
   _numi index;
   const _str suffix;
   const _bool absoluteBase;
   const _bool isFinal;
   // DefSuffix is final, if it appears at the end of a pattern
   // if not, we can introduce optimalizations:
   // 1) return only directories
   // 2) do not calculate indexes (the last Definition of the pattern does that anyways)
};


// ternary and binary works with Definitions in its own way
// so instead of using templates from 'gen.generic.h'
// here are special structs

struct DefTernary : _def
{
public:
   DefTernary(_genptr<_bool>& cond, _defptr& le, _defptr& ri)
      : condition(std::move(cond)), left(std::move(le)), right(std::move(ri)) { };

   void reset() override;
   _bool hasNext() override;

private:
   _genptr<_bool> condition;
   _defptr left;
   _defptr right;
   _bool first = true;
   _bool isLeft = true;
};


struct DefBinary : _def
{
public:
   DefBinary(_genptr<_bool>& cond, _defptr&le)
      : condition(std::move(cond)), left(std::move(le)) { };

   void reset() override;
   _bool hasNext() override;

private:
   _genptr<_bool> condition;
   _defptr left;
   _bool first = true;
};

}


#endif // GEN_DEFINITION_H_INCLUDED
