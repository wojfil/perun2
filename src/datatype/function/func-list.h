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

#ifndef FUNC_LIST_H_INCLUDED
#define FUNC_LIST_H_INCLUDED

#include "func-generic.h"


namespace perun2::func
{

inline p_list toChars(const p_str& value);

struct F_Characters : Func_1<p_str>, Generator<p_list>
{
public:
   F_Characters(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_list getValue() override;
};


struct F_Split : Func_2<p_str, p_str>, Generator<p_list>
{
public:
   F_Split(p_genptr<p_str>& a1, p_genptr<p_str>& a2) : Func_2(a1, a2) { };
   p_list getValue() override;
};


struct F_Words : Func_1<p_str>, Generator<p_list>
{
public:
   F_Words(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_list getValue() override;
};


struct F_Numbers : Func_1<p_str>, Generator<p_nlist>
{
public:
   F_Numbers(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_nlist getValue() override;

private:
   p_nint fromChar(const p_char ch);
};

}

#endif // FUNC_LIST_H_INCLUDED
