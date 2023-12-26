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

#ifndef COM_TIME_H
#define COM_TIME_H

#include "com-core.h"


namespace perun2::comm
{


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


}

#endif /* COM_TIME_H */
