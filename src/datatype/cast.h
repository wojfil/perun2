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

   _bool isConstant() const override
   {
      return this->base->isConstant();
   };

protected:
   _genptr<T1> base;
};



struct Cast_B_N : Cast<_bool, _num>
{
   Cast_B_N(_genptr<_bool>& b) : Cast(b) { };
   _num getValue() override;
};

struct Cast_B_NL : Cast<_bool, _nlist>
{
   Cast_B_NL(_genptr<_bool>& b) : Cast(b) { };
   _nlist getValue() override;
};

struct Cast_B_S : Cast<_bool, _str>
{
   Cast_B_S(_genptr<_bool>& b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_B_L : Cast<_bool, _list>
{
   Cast_B_L(_genptr<_bool>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_N_NL : Cast<_num, _nlist>
{
   Cast_N_NL(_genptr<_num>& b) : Cast(b) { };
   _nlist getValue() override;
};

struct Cast_N_S : Cast<_num, _str>
{
   Cast_N_S(_genptr<_num>& b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_N_L : Cast<_num, _list>
{
   Cast_N_L(_genptr<_num>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_T_S : Cast<_tim, _str>
{
   Cast_T_S(_genptr<_tim>& b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_T_L : Cast<_tim, _list>
{
   Cast_T_L(_genptr<_tim>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_P_S : Cast<_per, _str>
{
   Cast_P_S(_genptr<_per>& b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_P_L : Cast<_per, _list>
{
   Cast_P_L(_genptr<_per>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_NL_L : Cast<_nlist, _list>
{
   Cast_NL_L(_genptr<_nlist>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_S_L : Cast<_str, _list>
{
   Cast_S_L(_genptr<_str>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_T_TL : Cast<_tim, _tlist>
{
   Cast_T_TL(_genptr<_tim>& b) : Cast(b) { };
   _tlist getValue() override;
};

struct Cast_TL_L : Cast<_tlist, _list>
{
   Cast_TL_L(_genptr<_tlist>& b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_D_L : Generator<_list>
{
public:
   Cast_D_L(_defptr& b, _p2& p2)
      : base(std::move(b)), perun2(p2) { };

   _list getValue() override;

private:
   _p2& perun2;
   _defptr base;
};

}

#endif /* CAST_H */
