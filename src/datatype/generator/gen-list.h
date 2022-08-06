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

#ifndef GEN_LIST_H
#define GEN_LIST_H

#include "../generator.h"
#include "../order.h"
#include "../../var/var-runtime.h"
#include "../../attribute.h"
#include "../../uroboros.h"
#include "../../os.h"


struct Join_StrStr : Generator<_list>
{

public:
   Join_StrStr(Generator<_str>* lef, Generator<_str>* rig)
        : left(lef), right(rig) {};
   ~Join_StrStr();

   _list getValue() override;

private:
   Generator<_str>* left;
   Generator<_str>* right;
};


struct Join_StrList : Generator<_list>
{

public:
   Join_StrList(Generator<_str>* lef, Generator<_list>* rig)
        : left(lef), right(rig) {};
   ~Join_StrList();

   _list getValue() override;

private:
   Generator<_str>* left;
   Generator<_list>* right;
};


struct Join_ListStr : Generator<_list>
{

public:
   Join_ListStr(Generator<_list>* lef, Generator<_str>* rig)
        : left(lef), right(rig) {};
   ~Join_ListStr();

   _list getValue() override;

private:
   Generator<_list>* left;
   Generator<_str>* right;
};


struct Join_ListList : Generator<_list>
{

public:
   Join_ListList(Generator<_list>* lef, Generator<_list>* rig)
        : left(lef), right(rig) {};
   ~Join_ListList();

   _list getValue() override;

private:
   Generator<_list>* left;
   Generator<_list>* right;
};


#endif /* GEN_LIST_H */

