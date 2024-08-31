/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

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
   CoreCommand(FileContext* ctx, Perun2Process& p2)
      : saveChanges(true), context(ctx), perun2(p2), locationContext(p2.contexts.getLocationContext()) { };
   CoreCommand(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : saveChanges(save), context(ctx), perun2(p2), locationContext(p2.contexts.getLocationContext()) { };

protected:

   FileContext* context;
   const p_bool saveChanges;
   Perun2Process& perun2;
   LocationContext const* locationContext;
};

struct C_Delete : CoreCommand
{
public:
   C_Delete(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Drop : CoreCommand
{
public:
   C_Drop(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Hide : CoreCommand
{
   C_Hide(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Lock : CoreCommand
{
   C_Lock(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Open : CoreCommand
{
   C_Open(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Unlock : CoreCommand
{
   C_Unlock(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_Unhide : CoreCommand
{
   C_Unhide(const p_bool save, FileContext* ctx, Perun2Process& p2)
      : CoreCommand(save, ctx, p2) { };

   void run() override;
};

struct C_OpenWith : CoreCommand
{
public:
   C_OpenWith(p_genptr<p_str>& pro, const p_bool save, FileContext* ctx, Perun2Process& p2)
      : program(std::move(pro)), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> program;
};


p_str getCCName(const p_str& path);
p_str getCCNameShort(const p_str& path);

}
