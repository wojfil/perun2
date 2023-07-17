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

#ifndef CAST_H
#define CAST_H


#include "generator.h"
#include "datatype.h"
#include "../perun2.h"


namespace perun2::gen
{

template <typename T1, typename T2>
struct Cast : Generator<T2>
{
public:
   Cast (_genptr<T1>& b) : base(std::move(b)) { };

   p_bool isConstant() const override
   {
      return this->base->isConstant();
   };

protected:
   _genptr<T1> base;
};



struct Cast_B_N : Cast<p_bool, p_num>
{
   Cast_B_N(_genptr<p_bool>& b) : Cast(b) { };
   p_num getValue() override;
};

struct Cast_B_NL : Cast<p_bool, p_nlist>
{
   Cast_B_NL(_genptr<p_bool>& b) : Cast(b) { };
   p_nlist getValue() override;
};

struct Cast_B_S : Cast<p_bool, p_str>
{
   Cast_B_S(_genptr<p_bool>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_B_L : Cast<p_bool, p_list>
{
   Cast_B_L(_genptr<p_bool>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_N_NL : Cast<p_num, p_nlist>
{
   Cast_N_NL(_genptr<p_num>& b) : Cast(b) { };
   p_nlist getValue() override;
};

struct Cast_N_S : Cast<p_num, p_str>
{
   Cast_N_S(_genptr<p_num>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_N_L : Cast<p_num, p_list>
{
   Cast_N_L(_genptr<p_num>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_T_S : Cast<p_tim, p_str>
{
   Cast_T_S(_genptr<p_tim>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_T_L : Cast<p_tim, p_list>
{
   Cast_T_L(_genptr<p_tim>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_P_S : Cast<p_per, p_str>
{
   Cast_P_S(_genptr<p_per>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_P_L : Cast<p_per, p_list>
{
   Cast_P_L(_genptr<p_per>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_NL_L : Cast<p_nlist, p_list>
{
   Cast_NL_L(_genptr<p_nlist>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_S_L : Cast<p_str, p_list>
{
   Cast_S_L(_genptr<p_str>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_T_TL : Cast<p_tim, p_tlist>
{
   Cast_T_TL(_genptr<p_tim>& b) : Cast(b) { };
   p_tlist getValue() override;
};

struct Cast_TL_L : Cast<p_tlist, p_list>
{
   Cast_TL_L(_genptr<p_tlist>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_D_L : Generator<p_list>
{
public:
   Cast_D_L(p_defptr& b, pp_perun2& p2)
      : base(std::move(b)), perun2(p2) { };

   p_list getValue() override;

private:
   pp_perun2& perun2;
   p_defptr base;
};

}

#endif /* CAST_H */
