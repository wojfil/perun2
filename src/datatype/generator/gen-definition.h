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


struct DefWithContext : _def
{
public:
   DefWithContext() = delete;
   DefWithContext(_defptr& def, _fcptr& ctx);

   void reset() override;
   _bool hasNext() override;
   FileContext* getFileContext() override;

private:
   _defptr definition;
   _fcptr context;
};


struct DefFilter : _def
{
public:
   DefFilter() = delete;
   DefFilter(_defptr& def, FileContext* ctx, _uro& uro);

   void reset() override;
   FileContext* getFileContext() override;

protected:
   _uro& uroboros;
   _bool first;
   _defptr definition;
   FileContext* context;
};


struct Filter_WhereDef : DefFilter
{
public:
   Filter_WhereDef(_genptr<_bool>& cond, _defptr& def, FileContext* ctx, _uro& uro);

   _bool hasNext() override;
   void reset() override;

private:
   _bool finished = true;
   _genptr<_bool> condition;
   _num index;
};


struct DefinitionChain : _def
{
public:
   DefinitionChain(_defptr& def, _uro& uro);
   FileContext* getFileContext() override;

   _bool hasNext() override;
   void reset() override;

private:
   _defptr definition;
   _nint index = NINT_ZERO;
   _bool finished = true;
   _fcptr context;
   _uro& uroboros;
};


struct LocationVessel : Generator<_str>
{
public:
   LocationVessel(const _bool& abs, LocationContext* ctx);
   _str getValue() override;
   const _str& getRawValue() const;
   void setValue(const _str& val);

private:
   _str value;
   const _bool isAbsolute;
   LocationContext* context;
};


struct NestedDefiniton : _def
{
public:
   NestedDefiniton(LocationVessel& ves, _defptr& def, _defptr& locs, const _bool& abs, const _bool& fin);
   _bool hasNext() override;
   void reset() override;
   FileContext* getFileContext() override;

private:
   LocationVessel& vessel;
   _defptr definition;
   _defptr locations;
   _num index;
   _bool defOpened = false;
   _bool locsOpened = false;
   _num locDepth;
   FileContext* context;
   const _bool isAbsolute;
   const _bool isFinal;
};


struct Filter_LimitDef : DefFilter
{
public:
   Filter_LimitDef(_defptr& def, _genptr<_num>& num, FileContext* ctx, _uro& uro)
      : DefFilter(def, ctx, uro), number(std::move(num)) { };

   _bool hasNext() override;

private:
   _nint counter;
   _nint limit;
   _genptr<_num> number;
};


struct Filter_SkipDef : DefFilter
{
public:
   Filter_SkipDef(_defptr& def, _genptr<_num>& num, FileContext* ctx, _uro& uro)
      : DefFilter(def, ctx, uro), number(std::move(num)) { };

   _bool hasNext() override;

private:
   _nint counter;
   _nint limit;
   _genptr<_num> number;
};


struct Filter_EveryDef : DefFilter
{
public:
   Filter_EveryDef(_defptr& def, _genptr<_num>& num, FileContext* ctx, _uro& uro)
      : DefFilter(def, ctx, uro), number(std::move(num)) { };


   _bool hasNext() override;

private:
   _nint counter;
   _nint limit;
   _genptr<_num> number;
   _num index;
};


struct Filter_FinalDef : DefFilter
{
public:
   Filter_FinalDef(_defptr& def, _genptr<_num>& num, FileContext* ctx, _uro& uro)
      : DefFilter(def, ctx, uro), number(std::move(num)) { };


   _bool hasNext() override;

private:
   _genptr<_num> number;

   std::deque<_str> values;
   _nint length;
   _nint index;
};



struct Join_DefStr : _def
{

public:
   Join_DefStr(_defptr& lef, _genptr<_str>& rig, _uro& uro)
        : left(std::move(lef)), right(std::move(rig)), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   _uro& uroboros;
   _defptr left;
   _genptr<_str> right;
   _bool taken;
};


struct Join_StrDef : _def
{

public:
   Join_StrDef(_genptr<_str>& lef, _defptr& rig, _uro& uro)
        : left(std::move(lef)), right(std::move(rig)), first(true), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   _uro& uroboros;
   _genptr<_str> left;
   _defptr right;
   _bool first;
};


struct Join_DefList : _def
{

public:
   Join_DefList(_defptr& lef, _genptr<_list>& rig, _uro& uro)
        : left(std::move(lef)), right(std::move(rig)), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   _uro& uroboros;
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
   Join_ListDef(_genptr<_list>& lef, _defptr& rig, _uro& uro)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   _uro& uroboros;
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
   Join_DefDef(_defptr& lef, _defptr& rig, _uro& uro)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), uroboros(uro) { };

   void reset() override;
   _bool hasNext() override;

private:
   _uro& uroboros;
   _defptr left;
   _defptr right;
   _bool first;
   _bool taken;
};


struct DefinitionSuffix : _def
{
public:
   DefinitionSuffix(_defptr& def, _uro& uro, const _str& suf, const _bool& abs, const _bool& fin);

   _bool hasNext() override;
   void reset() override;

private:
   _defptr definition;
   FileContext* fileContext;
   LocationContext* locContext;
   _bool first = true;
   _num index;
   const _str suffix;
   const _bool absoluteBase;
   const _bool isFinal;
   // DefSuffix is final, if it appears at the end of a pattern
   // if not, we can optimize and return only directories
};


// ternary and binary works with Definitions in its own way
// so instead of using templates from 'gen-generic.h'
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
   DefBinary(_genptr<_bool>& cond, _defptr& le)
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
