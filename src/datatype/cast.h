/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAST_H
#define CAST_H


#include "generator.h"
#include "datatype.h"


template <typename T1, typename T2>
struct Cast : Generator<T2>
{
public:
   Cast (Generator<T1>* b) : base(b) { };
   ~Cast() override {
      delete base;
   }

protected:
   Generator<T1>* base;
};



struct Cast_B_N : Cast<_boo, _num>
{
   Cast_B_N(Generator<_boo>* b) : Cast(b) { };
   _num getValue() override;
};

struct Cast_B_NL : Cast<_boo, _nlist>
{
   Cast_B_NL(Generator<_boo>* b) : Cast(b) { };
   _nlist getValue() override;
};

struct Cast_B_S : Cast<_boo, _str>
{
   Cast_B_S(Generator<_boo>* b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_B_L : Cast<_boo, _list>
{
   Cast_B_L(Generator<_boo>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_N_NL : Cast<_num, _nlist>
{
   Cast_N_NL(Generator<_num>* b) : Cast(b) { };
   _nlist getValue() override;
};

struct Cast_N_S : Cast<_num, _str>
{
   Cast_N_S(Generator<_num>* b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_N_L : Cast<_num, _list>
{
   Cast_N_L(Generator<_num>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_T_S : Cast<_tim, _str>
{
   Cast_T_S(Generator<_tim>* b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_T_L : Cast<_tim, _list>
{
   Cast_T_L(Generator<_tim>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_P_S : Cast<_per, _str>
{
   Cast_P_S(Generator<_per>* b) : Cast(b) { };
   _str getValue() override;
};

struct Cast_P_L : Cast<_per, _list>
{
   Cast_P_L(Generator<_per>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_NL_L : Cast<_nlist, _list>
{
   Cast_NL_L(Generator<_nlist>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_S_L : Cast<_str, _list>
{
   Cast_S_L(Generator<_str>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_T_TL : Cast<_tim, _tlist>
{
   Cast_T_TL(Generator<_tim>* b) : Cast(b) { };
   _tlist getValue() override;
};

struct Cast_TL_L : Cast<_tlist, _list>
{
   Cast_TL_L(Generator<_tlist>* b) : Cast(b) { };
   _list getValue() override;
};

struct Cast_D_L : Generator<_list>
{
public:
   Cast_D_L(_def* b) : base(b) { };
   ~Cast_D_L() {
      delete base;
   }
   _list getValue() override;

private:
   _def* base;
};


#endif /* CAST_H */
