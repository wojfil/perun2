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

#include "com-core.hpp"


namespace perun2::comm
{

struct C_CopyTo : CoreCommand
{
public:
   C_CopyTo(p_genptr<p_str>& loc, const p_bool save,
      const p_bool forc, FileContext* ctx, Perun2Process& p2)
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
   C_CopyTo_Stack(p_genptr<p_str>& loc, const p_bool save, FileContext* ctx, Perun2Process& p2)
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
      const p_bool save, const p_bool forc, const p_bool extless, FileContext* ctx, Perun2Process& p2)
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
      const p_bool save, const p_bool extless, FileContext* ctx, Perun2Process& p2)
      : location(std::move(loc)), name(std::move(na)), extensionless(extless), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> location;
   p_genptr<p_str> name;
   const p_bool extensionless;
};

}
