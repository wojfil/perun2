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

#ifndef COM_CORE_H
#define COM_CORE_H

#include "com-listener.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "../attribute.h"
#include "../perun2.h"


namespace perun2::comm
{

struct CoreCommand : Command
{
public:
   CoreCommand() = delete;
   CoreCommand(FileContext* ctx, p_perun2& p2)
      : saveChanges(true), context(ctx), perun2(p2), locationContext(p2.contexts.getLocationContext()) { };
   CoreCommand(const p_bool save, FileContext* ctx, p_perun2& p2)
      : saveChanges(save), context(ctx), perun2(p2), locationContext(p2.contexts.getLocationContext()) { };

protected:

   FileContext* context;
   const p_bool saveChanges;
   p_perun2& perun2;
   LocationContext const* locationContext;
};

struct C_Delete : CoreCommand
{
public:
   C_Delete(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Drop : CoreCommand
{
public:
   C_Drop(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Hide : CoreCommand
{
   C_Hide(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Lock : CoreCommand
{
   C_Lock(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Open : CoreCommand
{
   C_Open(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Unlock : CoreCommand
{
   C_Unlock(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Unhide : CoreCommand
{
   C_Unhide(const p_bool save, FileContext* ctx, p_perun2& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_OpenWith : CoreCommand
{
public:
   C_OpenWith(p_genptr<p_str>& pro, const p_bool save, FileContext* ctx, p_perun2& p2)
      : program(std::move(pro)), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> program;
};




// time commands:



struct C_TimeAlter : CoreCommand
{
public:
   C_TimeAlter(p_genptr<p_tim>& ti, const p_bool save, FileContext* ctx, p_perun2& p2)
      : time(std::move(ti)), CoreCommand(save, ctx, p2) { };

protected:
   p_genptr<p_tim> time;
};


struct C_ReaccessTo : C_TimeAlter
{
   C_ReaccessTo(p_genptr<p_tim>& ti, const p_bool save, FileContext* ctx, p_perun2& p2)
      : C_TimeAlter(ti, save, ctx, p2) { };

   void run() override;
};


struct C_RechangeTo : C_TimeAlter
{
   C_RechangeTo(p_genptr<p_tim>& ti, const p_bool save, FileContext* ctx, p_perun2& p2)
      : C_TimeAlter(ti, save, ctx, p2) { };

   void run() override;
};


struct C_RecreateTo : C_TimeAlter
{
   C_RecreateTo(p_genptr<p_tim>& ti, const p_bool save, FileContext* ctx, p_perun2& p2)
      : C_TimeAlter(ti, save, ctx, p2) { };

   void run() override;
};


struct C_RemodifyTo : C_TimeAlter
{
   C_RemodifyTo(p_genptr<p_tim>& ti, const p_bool save, FileContext* ctx, p_perun2& p2)
      : C_TimeAlter(ti, save, ctx, p2) { };

   void run() override;
};


struct C_RenameTo : CoreCommand
{
public:
   C_RenameTo(p_genptr<p_str>& na, const p_bool save, const p_bool forc,
      const p_bool extless, FileContext* ctx, p_perun2& p2)
      : name(std::move(na)), saveChanges(save), forced(forc), extensionless(extless), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> name;
   const p_bool saveChanges;
   const p_bool forced;
   const p_bool extensionless;
};


struct C_RenameTo_Stack : CoreCommand
{
public:
   C_RenameTo_Stack(p_genptr<p_str>& na, const p_bool save, const p_bool extless, FileContext* ctx, p_perun2& p2)
      : name(std::move(na)), saveChanges(save), extensionless(extless), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> name;
   const p_bool saveChanges;
   const p_bool extensionless;
};


struct C_MoveTo : CoreCommand
{
public:
   C_MoveTo(p_genptr<p_str>& loc, const p_bool forc, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), forced(forc), CoreCommand(ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   const p_bool forced;
};


struct C_MoveTo_Stack : CoreCommand
{
public:
   C_MoveTo_Stack(p_genptr<p_str>& loc, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), CoreCommand(ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
};


struct C_MoveToAs : CoreCommand
{
public:
   C_MoveToAs(p_genptr<p_str>& loc, p_genptr<p_str>& na, const p_bool forc,
      const p_bool extless, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), name(std::move(na)),
        forced(forc), extensionless(extless), CoreCommand(ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   p_genptr<p_str> name;
   const p_bool forced;
   const p_bool extensionless;
};


struct C_MoveToAs_Stack : CoreCommand
{
public:
   C_MoveToAs_Stack(p_genptr<p_str>& loc, p_genptr<p_str>& na, const p_bool extless, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), name(std::move(na)), extensionless(extless), CoreCommand(ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   p_genptr<p_str> name;
   const p_bool extensionless;
};


struct C_CopyTo : CoreCommand
{
public:
   C_CopyTo(p_genptr<p_str>& loc, const p_bool save,
      const p_bool forc, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), saveChanges(save), forced(forc), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   const p_bool saveChanges;
   const p_bool forced;
};


struct C_CopyTo_Stack : CoreCommand
{
public:
   C_CopyTo_Stack(p_genptr<p_str>& loc, const p_bool save, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), saveChanges(save), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   const p_bool saveChanges;
};


struct C_CopyToAs : CoreCommand
{
public:
   C_CopyToAs(p_genptr<p_str>& loc, p_genptr<p_str>& na,
      const p_bool save, const p_bool forc, const p_bool extless, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), name(std::move(na)), forced(forc), extensionless(extless), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   p_genptr<p_str> name;
   const p_bool forced;
   const p_bool extensionless;
};


struct C_CopyToAs_Stack : CoreCommand
{
public:
   C_CopyToAs_Stack(p_genptr<p_str>& loc, p_genptr<p_str>& na,
      const p_bool save, const p_bool extless, FileContext* ctx, p_perun2& p2)
      : location(std::move(loc)), name(std::move(na)), extensionless(extless), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   p_genptr<p_str> name;
   const p_bool extensionless;
};


p_str getCCName(const p_str& path);
p_str getCCNameShort(const p_str& path);

}

#endif /* COM_CORE_H */

