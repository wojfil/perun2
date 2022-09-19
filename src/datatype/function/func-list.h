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

#ifndef FUNC_LIST_H_INCLUDED
#define FUNC_LIST_H_INCLUDED

#include "func-generic.h"


namespace uro::func
{

inline _list toChars(const _str& value);

struct F_Characters : Func_1<_str>, Generator<_list>
{
public:
   F_Characters(Generator<_str>* a1) : Func_1(a1) { };
   _list getValue() override;
};


struct F_Split : Func_2<_str, _str>, Generator<_list>
{
public:
   F_Split(Generator<_str>* a1, Generator<_str>* a2) : Func_2(a1, a2) { };
   _list getValue() override;
};


struct F_Words : Func_1<_str>, Generator<_list>
{
public:
   F_Words(Generator<_str>* a1) : Func_1(a1) { };
   _list getValue() override;
};


struct F_Numbers : Func_1<_str>, Generator<_nlist>
{
public:
   F_Numbers(Generator<_str>* a1) : Func_1(a1) { };
   _nlist getValue() override;

private:
   _num fromChar(const _char& ch);
};

}

#endif // FUNC_LIST_H_INCLUDED
