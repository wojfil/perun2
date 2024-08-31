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

#include "com-core.h"


namespace perun2::comm
{

struct C_RenameTo : CoreCommand
{
public:
   C_RenameTo(p_genptr<p_str>& na, const p_bool save, const p_bool forc,
      const p_bool extless, FileContext* ctx, Perun2Process& p2)
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
   C_RenameTo_Stack(p_genptr<p_str>& na, const p_bool save, const p_bool extless, FileContext* ctx, Perun2Process& p2)
      : name(std::move(na)), saveChanges(save), extensionless(extless), CoreCommand(save, ctx, p2) { };

   void run() override;

protected:
   p_genptr<p_str> name;
   const p_bool saveChanges;
   const p_bool extensionless;
};


}
