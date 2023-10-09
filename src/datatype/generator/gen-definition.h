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


struct DefWithContext : p_def
{
public:
   DefWithContext() = delete;
   DefWithContext(p_defptr& def, p_perun2& p2);
   DefWithContext(p_defptr& def, p_fcptr& ctx);

   void reset() override;
   p_bool hasNext() override;
   FileContext* getFileContext() override;

private:
   p_defptr definition;
   p_fcptr context;
};


struct DefFilter : p_def
{
public:
   DefFilter() = delete;
   DefFilter(p_defptr& def, FileContext* ctx, p_perun2& p2);
   p_bool setAction(p_daptr& act) override;

   void reset() override;
   FileContext* getFileContext() override;

protected:
   p_perun2& perun2;
   p_bool first;
   p_defptr definition;
   FileContext* context;
};


struct DefFilter_Where : DefFilter
{
public:
   DefFilter_Where(p_genptr<p_bool>& cond, p_defptr& def, FileContext* ctx, p_perun2& p2);

   p_bool hasNext() override;
   void reset() override;

private:
   p_bool finished = true;
   p_genptr<p_bool> condition;
   p_num index;
};


struct LocationVessel : Generator<p_str>
{
public:
   LocationVessel(const PathType pt, p_genptr<p_str>& loc);
   p_str getValue() override;
   const p_str& getRawValue() const;
   void setValue(const p_str& val);

private:
   p_str value;
   const PathType pathType;
   p_genptr<p_str> location;
};


struct NestedDefiniton : p_def
{
public:
   NestedDefiniton(LocationVessel& ves, p_defptr& def, p_defptr& locs, const PathType pt, const SegmentType segmType, const p_int retr);
   p_bool hasNext() override;
   void reset() override;
   FileContext* getFileContext() override;

private:
   LocationVessel& vessel;
   p_defptr definition;
   p_defptr locations;
   p_num index;
   p_bool defOpened = false;
   p_bool locsOpened = false;
   p_num locDepth;
   FileContext* context;
   const PathType pathType;
   const SegmentType segmentType;
   const p_int retreats;
};


struct DefFilter_Limit : DefFilter
{
public:
   DefFilter_Limit(p_defptr& def, p_genptr<p_num>& num, FileContext* ctx, p_perun2& p2)
      : DefFilter(def, ctx, p2), number(std::move(num)) { };

   p_bool hasNext() override;

private:
   p_nint counter;
   p_nint limit;
   p_genptr<p_num> number;
};


struct DefFilter_Skip : DefFilter
{
public:
   DefFilter_Skip(p_defptr& def, p_genptr<p_num>& num, FileContext* ctx, p_perun2& p2)
      : DefFilter(def, ctx, p2), number(std::move(num)) { };

   p_bool hasNext() override;

private:
   p_nint counter;
   p_nint limit;
   p_genptr<p_num> number;
};


struct DefFilter_Every : DefFilter
{
public:
   DefFilter_Every(p_defptr& def, p_genptr<p_num>& num, FileContext* ctx, p_perun2& p2)
      : DefFilter(def, ctx, p2), number(std::move(num)) { };

   p_bool hasNext() override;

private:
   p_nint counter;
   p_nint limit;
   p_genptr<p_num> number;
   p_num index;
};


struct DefFilter_Final : DefFilter
{
public:
   DefFilter_Final(p_defptr& def, p_genptr<p_num>& num, p_fcptr& ctx, FileContext* pcxt, p_perun2& p2)
      : DefFilter(def, ctx.get(), p2), nextContext(std::move(ctx)), prevContext(pcxt), number(std::move(num)) { };

   FileContext* getFileContext() override;
   p_bool hasNext() override;

private:
   p_fcptr nextContext;
   FileContext* prevContext;
   p_genptr<p_num> number;

   std::deque<p_str> values;
   p_nint length;
   p_nint index;
};



struct Join_DefStr : p_def
{

public:
   Join_DefStr(p_defptr& lef, p_genptr<p_str>& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   p_defptr left;
   p_genptr<p_str> right;
   p_bool taken;
};


struct Join_StrDef : p_def
{

public:
   Join_StrDef(p_genptr<p_str>& lef, p_defptr& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), first(true), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   p_genptr<p_str> left;
   p_defptr right;
   p_bool first;
};


struct Join_DefList : p_def
{

public:
   Join_DefList(p_defptr& lef, p_genptr<p_list>& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   p_defptr left;
   p_genptr<p_list> right;
   p_bool taken;
   p_size length;
   p_size index;
   p_list values;
};


struct Join_ListDef : p_def
{

public:
   Join_ListDef(p_genptr<p_list>& lef, p_defptr& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   p_genptr<p_list> left;
   p_defptr right;
   p_bool first;
   p_bool taken;
   p_size length;
   p_size index;
   p_list values;
};


struct Join_DefDef : p_def
{

public:
   Join_DefDef(p_defptr& lef, p_defptr& rig, p_perun2& p2)
        : left(std::move(lef)), right(std::move(rig)), first(true), taken(false), perun2(p2) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_perun2& perun2;
   p_defptr left;
   p_defptr right;
   p_bool first;
   p_bool taken;
};


// below are some variants of DefinitionSuffix
// they add a constant segment to a path, like 'a/b' -> 'a/b/c'
// DefinitionSuffix is final, if it appears at the end of a pattern
// if not, we can optimize and return only directories

struct DefinitionSuffix : p_def
{
public:
   DefinitionSuffix(p_defptr& def, const p_str& suf, const SegmentType segmType);
   void reset() override;

protected:
   p_defptr definition;
   FileContext* fileContext;
   p_bool first = true;
   p_num index;
   const p_str suffix;
   const SegmentType segmentType;
};


struct AbsoluteDefSuffix : DefinitionSuffix
{
public:
   AbsoluteDefSuffix(p_defptr& def, const p_str& suf, const SegmentType segmType);
   p_bool hasNext() override;
};


struct RelativeDefSuffix : DefinitionSuffix
{
public:
   RelativeDefSuffix(p_defptr& def, p_perun2& p2, const p_str& suf, const SegmentType segmType, p_def* const prev);
   p_bool hasNext() override;

private:
   LocationContext* const locContext;
   p_def* const previous;
};


struct RetreatedDefSuffix : DefinitionSuffix
{
public:
   RetreatedDefSuffix(p_defptr& def, p_perun2& p2, const p_str& suf, const SegmentType segmType, const p_int retr, p_def* const prev);
   p_bool hasNext() override;

private:
   LocationContext* const locContext;
   const p_int retreats;
   p_def* const previous;
};


struct FarRetreatedDefSuffix : DefinitionSuffix
{
public:
   FarRetreatedDefSuffix(p_defptr& def, p_perun2& p2, const p_str& suf, const SegmentType segmType, const p_int retr, p_def* const prev);
   p_bool hasNext() override;

private:
   LocationContext* const locContext;
   const p_int retreats;
   p_def* const previous;
};



// ternary and binary works with Definitions in its own way
// so instead of using templates from 'gen-generic.h'
// here are special structs

struct DefTernary : p_def
{
public:
   DefTernary(p_genptr<p_bool>& cond, p_defptr& le, p_defptr& ri)
      : condition(std::move(cond)), left(std::move(le)), right(std::move(ri)) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_genptr<p_bool> condition;
   p_defptr left;
   p_defptr right;
   p_bool first = true;
   p_bool isLeft = true;
};


struct DefBinary : p_def
{
public:
   DefBinary(p_genptr<p_bool>& cond, p_defptr& le)
      : condition(std::move(cond)), left(std::move(le)) { };

   void reset() override;
   p_bool hasNext() override;

private:
   p_genptr<p_bool> condition;
   p_defptr left;
   p_bool first = true;
};

}


#endif // GEN_DEFINITION_H_INCLUDED
