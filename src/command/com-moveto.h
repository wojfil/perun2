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

#ifndef COM_MOVETO_H
#define COM_MOVETO_H

#include "com-core.h"


namespace perun2::comm
{


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

}

#endif /* COM_MOVETO_H */
