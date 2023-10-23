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

#ifndef COM_STRUCT_H
#define COM_STRUCT_H

#include "../util.h"
#include "../attribute.h"
#include "../datatype/patterns.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-arg.h"


namespace perun2::comm
{


struct CS_RawBlock : Command
{
public:
   CS_RawBlock(std::vector<p_comptr>& coms, p_ucptr& ctx, p_perun2& p2)
      : context(std::move(ctx)), perun2(p2)
   {
      langutil::transferUniquePtrs(coms, commands);
   };

   void run() override;

private:
   std::vector<p_comptr> commands;
   p_perun2& perun2;
   p_ucptr context;
};


struct CS_Block : Command
{
public:
   CS_Block(std::vector<p_comptr>& coms, p_acptr& ctx, p_perun2& p2)
      : perun2(p2), context(std::move(ctx))
   {
      langutil::transferUniquePtrs(coms, commands);
   };

   void run() override;

private:
   std::vector<p_comptr> commands;
   p_perun2& perun2;
   p_acptr context;
};


struct CS_Times : Command
{
public:
   CS_Times(p_genptr<p_num>& ts, p_comptr& com, p_icptr& ctx, p_perun2& p2)
      : context(std::move(ctx)), times(std::move(ts)), command(std::move(com)), perun2(p2) { };

   void run() override;

private:
   p_genptr<p_num> times;
   p_comptr command;
   p_perun2& perun2;
   p_icptr context;
};


struct CS_While : Command
{
public:
   CS_While(p_genptr<p_bool>& cond, p_comptr& com, p_icptr& ctx, p_perun2& p2)
      : context(std::move(ctx)), condition(std::move(cond)), command(std::move(com)), perun2(p2)  { };

   void run() override;

private:
   p_genptr<p_bool> condition;
   p_comptr command;
   p_perun2& perun2;
   p_icptr context;
};


struct CS_StringLoop : IterationLoop
{
public:
   CS_StringLoop(p_genptr<p_str>& str, p_comptr& com, p_fcptr& ctx, p_perun2& p2)
      : IterationLoop(com, ctx, p2), string(std::move(str)) { };

   void run() override;

private:
   p_genptr<p_str> string;
};


struct CS_DefinitionLoop : IterationLoop
{
public:
   CS_DefinitionLoop(p_defptr& def, p_comptr& com, p_fcptr& ctx, p_perun2& p2)
      : IterationLoop(com, ctx, p2), definition(std::move(def)) { };

   void run() override;

private:
   p_defptr definition;
};


// this is a definition loop
// but does not own its context
// the real context is somwhere else and this struct has a dangling pointer to it
struct CS_ContextlessLoop : Command
{
public:
   CS_ContextlessLoop(p_defptr& def, p_comptr& com, p_perun2& p2);
   void run() override;

private:
   p_defptr definition;
   p_comptr command;
   p_perun2& perun2;
   FileContext* const context;
};


struct CS_ListLoop : IterationLoop
{
public:
   CS_ListLoop(p_genptr<p_list>& li, p_comptr& com, p_fcptr& ctx, p_perun2& p2)
      : IterationLoop(com, ctx, p2), list(std::move(li)) { };

   void run() override;

private:
   p_genptr<p_list> list;
};


struct CS_Inside : IterationLoop
{
public:
   CS_Inside(p_comptr& com, p_lcptr& lctx, p_fcptr& fctx, p_perun2& p2)
      : IterationLoop(com, fctx, p2), locContext(std::move(lctx)) { };

protected:
   p_lcptr locContext;
};


struct CS_InsideThis : Command
{
public:
   CS_InsideThis(p_comptr& com, p_lcptr& lctx, FileContext* fctx, p_perun2& p2)
      : command(std::move(com)), locContext(std::move(lctx)), fileContext(fctx), perun2(p2) { };

   void run() override;

private:
   p_comptr command;
   p_lcptr locContext;
   FileContext* const fileContext;
   p_perun2& perun2;
};


struct CS_InsideString : CS_Inside
{
public:
   CS_InsideString(p_genptr<p_str>& str, p_comptr& com, p_lcptr& lctx, p_fcptr& fctx, p_perun2& p2)
      : CS_Inside(com, lctx, fctx, p2), string(std::move(str)) { };

   void run() override;

private:
   p_genptr<p_str> string;
};


struct CS_InsideDefinition : CS_Inside
{
public:
   CS_InsideDefinition(p_defptr& def, p_comptr& com, p_lcptr& lctx, p_fcptr& fctx, p_perun2& p2)
      : CS_Inside(com, lctx, fctx, p2), definition(std::move(def)) { };

   void run() override;

private:
   p_defptr definition;
};


// analogous to CS_ContextlessLoop
struct CS_InsideContextless : Command
{
public:
   CS_InsideContextless(p_defptr& def, p_comptr& com, p_lcptr& lctx, p_perun2& p2);

   void run() override;

private:
   p_defptr definition;
   p_comptr command;
   p_lcptr locContext;
   FileContext* const fileContext;
   p_perun2& perun2;
};


struct CS_InsideList : CS_Inside
{
public:
   CS_InsideList(p_genptr<p_list>& li, p_comptr& com, p_lcptr& lctx, p_fcptr& fctx, p_perun2& p2)
      : CS_Inside(com, lctx, fctx, p2), list(std::move(li)) { };

   void run() override;

private:
   p_genptr<p_list> list;
};

}

#endif /* COM_STRUCT_H */

