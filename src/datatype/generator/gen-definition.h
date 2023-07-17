/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_DEFINITION_H_INCLUDED
#define GEN_DEFINITION_H_INCLUDED

#include "gen-list.h"
#include "gen-os.h"
#include "../cast.h"
#include "../../attribute.h"
#include "../../perun2.h"
#include <algorithm>
#include <deque>


namespace perun2::gen
{


struct DefWithContext : _def
{
public:
   DefWithContext() = delete;
   DefWithContext(_defptr& def, p_perun2& p2);
   DefWithContext(_defptr& def, _fcptr& ctx);

   void reset() override;
   p_bool hasNext() override;
   FileContext* getFileContext() override;

private:
   _defptr definition;
   _fcptr context;
};


struct DefFilter : _def
{
public:
   DefFilter() = delete;
   DefFilter(_defptr& def, FileContext* ctx, p_perun2& p2);

   void reset() override;
   FileContext* getFileContext() override;

protected:
   p_perun2& perun2;
   p_bool first;
   _defptr definition;
   FileContext* context;
};


struct DefFilter_Where : DefFilter
{
public:
   DefFilter_Where(_genptr<p_bool>& cond, _defptr& def, FileContext* ctx, p_perun2& p2);

   p_bool hasNext() override;
   void reset() override;

private:
   p_bool finished = true;
   _genptr<p_bool> condition;
   _num index;
};


struct LocationVessel : Generator<p_str>
{
public:
   LocationVessel(const p_bool abs, _genptr<p_str>& loc);
   p_str getValue() override;
   const p_str& getRawValue() const;
   void setValue(const p_str& val);

private:
   p_str value;
   const p_bool isAbsolute;
   _genptr<p_str> location;
};


struct NestedDefiniton : _def
{
public:
   NestedDefiniton(LocationVessel& ves, _defptr& def, _defptr& locs, const p_bool abs, const p_bool fin, const p_int retr);
   p_bool hasNext() override;
   void reset() override;
   FileContext* getFileContext() override;

private:
   LocationVessel& vessel;
   _defptr definition;
   _defptr locations;
   _num index;
   p_bool defOpened = false;
   p_bool locsOpened = false;
   _num locDepth;
   FileContext* context;
   const p_bool isAbsolute;
   const p_bool isFinal;
   const p_int retreats;
};


struct DefFilter_Limit : DefFilter
{
public:
   DefFilter_Limit(_defptr& def, _genptr<_num>& num, FileContext* ctx, p_perun2& p2)
      : DefFilter(def, ctx, p2), number(std::move(num)) { };

   p_bool hasNext() override;

private:
   p_nint counter;
   p_nint limit;
   _genptr<_num> number;
};


struct DefFilter_Skip : DefFilter
{
public:
   DefFilter_Skip(_defptr& def, _genptr<_num>& num, FileContext* ctx, p_perun2& p2)
      : DefFilter(def, ctx, p2), number(std::move(num)) { };

   p_bool hasNext() override;

private:
   p_nint counter;
   p_nint limit;
   _genptr<_num> number;
};


struct DefFilter_Every : DefFilter
{
public:
   DefFilter_Every(_defptr& def, _genptr<_num>& num, FileContext* ctx, p_perun2& p2)
      : DefFilter(def, ctx, p2), number(std::move(num)) { };

   p_bool hasNext() override;

private:
   p_nint counter;
   p_nint limit;
   _genptr<_num> number;
   _num index;
};


struct DefFilter_Final : DefFilter
{
public:
   DefFilter_Final(_defptr& def, _genptr<_num>& num, _fcptr& ctx, FileContext* pcxt, p_perun2& p2)
      : DefFilter(def, ctx.get(), p2), nextContext(std::move(ctx)), prevContext(pcxt), number(std::move(num)) { };

   FileContext* getFileContext() override;
   p_bool hasNext() override;

private:
   _fcptr nextContext;
   FileContext* prevContext;
   _genptr<_num> number;

   std::deque<p_str> values;
   p_nint length;
   p_nint index;
};



struct Join_DefStr : _def
{

public:
   Join_DefStr(_defptr& lef, _genptr<p_str>& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   _defptr left;
   _genptr<p_str> right;
   p_bool taken;
};


struct Join_StrDef : _def
{

public:
   Join_StrDef(_genptr<p_str>& lef, _defptr& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), first(true), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   _genptr<p_str> left;
   _defptr right;
   p_bool first;
};


struct Join_DefList : _def
{

public:
   Join_DefList(_defptr& lef, _genptr<p_list>& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   _defptr left;
   _genptr<p_list> right;
   p_bool taken;
   p_size length;
   p_size index;
   p_list values;
};


struct Join_ListDef : _def
{

public:
   Join_ListDef(_genptr<p_list>& lef, _defptr& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   _genptr<p_list> left;
   _defptr right;
   p_bool first;
   p_bool taken;
   p_size length;
   p_size index;
   p_list values;
};


struct Join_DefDef : _def
{

public:
   Join_DefDef(_defptr& lef, _defptr& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   _defptr left;
   _defptr right;
   p_bool first;
   p_bool taken;
};


// below are some variants of DefinitionSuffix
// they add a constant segment to a path, like 'a/b' -> 'a/b/c'
// DefinitionSuffix is final, if it appears at the end of a pattern
// if not, we can optimize and return only directories

struct DefinitionSuffix : _def
{
public:
   DefinitionSuffix(_defptr& def, const p_str& suf, const p_bool fin);
   void reset() override;

protected:
   _defptr definition;
   FileContext* fileContext;
   p_bool first = true;
   _num index;
   const p_str suffix;
   const p_bool isFinal;
};


struct AbsoluteDefSuffix : DefinitionSuffix
{
public:
   AbsoluteDefSuffix(_defptr& def, const p_str& suf, const p_bool fin);
   p_bool hasNext() override;
};


struct RelativeDefSuffix : DefinitionSuffix
{
public:
   RelativeDefSuffix(_defptr& def, p_perun2& p2, const p_str& suf, const p_bool fin, _def* const prev);
   p_bool hasNext() override;

private:
   LocationContext* const locContext;
   _def* const previous;
};


struct RetreatedDefSuffix : DefinitionSuffix
{
public:
   RetreatedDefSuffix(_defptr& def, p_perun2& p2, const p_str& suf, const p_bool fin, const p_int retr, _def* const prev);
   p_bool hasNext() override;

private:
   LocationContext* const locContext;
   const p_int retreats;
   _def* const previous;
};


struct FarRetreatedDefSuffix : DefinitionSuffix
{
public:
   FarRetreatedDefSuffix(_defptr& def, p_perun2& p2, const p_str& suf, const p_bool fin, const p_int retr, _def* const prev);
   p_bool hasNext() override;

private:
   LocationContext* const locContext;
   const p_int retreats;
   _def* const previous;
};



// ternary and binary works with Definitions in its own way
// so instead of using templates from 'gen-generic.h'
// here are special structs

struct DefTernary : _def
{
public:
   DefTernary(_genptr<p_bool>& cond, _defptr& le, _defptr& ri)
      : condition(std::move(cond)), left(std::move(le)), right(std::move(ri)) { };

   void reset() override;
   p_bool hasNext() override;

private:
   _genptr<p_bool> condition;
   _defptr left;
   _defptr right;
   p_bool first = true;
   p_bool isLeft = true;
};


struct DefBinary : _def
{
public:
   DefBinary(_genptr<p_bool>& cond, _defptr& le)
      : condition(std::move(cond)), left(std::move(le)) { };

   void reset() override;
   p_bool hasNext() override;

private:
   _genptr<p_bool> condition;
   _defptr left;
   p_bool first = true;
};

}


#endif // GEN_DEFINITION_H_INCLUDED
