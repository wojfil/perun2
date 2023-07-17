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



struct Cast_B_N : Cast<p_bool, _num>
{
   Cast_B_N(_genptr<p_bool>& b) : Cast(b) { };
   _num getValue() override;
};

struct Cast_B_NL : Cast<p_bool, _nlist>
{
   Cast_B_NL(_genptr<p_bool>& b) : Cast(b) { };
   _nlist getValue() override;
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

struct Cast_N_NL : Cast<_num, _nlist>
{
   Cast_N_NL(_genptr<_num>& b) : Cast(b) { };
   _nlist getValue() override;
};

struct Cast_N_S : Cast<_num, p_str>
{
   Cast_N_S(_genptr<_num>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_N_L : Cast<_num, p_list>
{
   Cast_N_L(_genptr<_num>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_T_S : Cast<_tim, p_str>
{
   Cast_T_S(_genptr<_tim>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_T_L : Cast<_tim, p_list>
{
   Cast_T_L(_genptr<_tim>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_P_S : Cast<_per, p_str>
{
   Cast_P_S(_genptr<_per>& b) : Cast(b) { };
   p_str getValue() override;
};

struct Cast_P_L : Cast<_per, p_list>
{
   Cast_P_L(_genptr<_per>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_NL_L : Cast<_nlist, p_list>
{
   Cast_NL_L(_genptr<_nlist>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_S_L : Cast<p_str, p_list>
{
   Cast_S_L(_genptr<p_str>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_T_TL : Cast<_tim, _tlist>
{
   Cast_T_TL(_genptr<_tim>& b) : Cast(b) { };
   _tlist getValue() override;
};

struct Cast_TL_L : Cast<_tlist, p_list>
{
   Cast_TL_L(_genptr<_tlist>& b) : Cast(b) { };
   p_list getValue() override;
};

struct Cast_D_L : Generator<p_list>
{
public:
   Cast_D_L(_defptr& b, p_perun2& p2)
      : base(std::move(b)), perun2(p2) { };

   p_list getValue() override;

private:
   p_perun2& perun2;
   _defptr base;
};

}

#endif /* CAST_H */
