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


namespace uro::gen
{

struct Join_StrStr : Generator<_list>
{

public:
   Join_StrStr(_genptr<_str>& lef, _genptr<_str>& rig)
        : left(std::move(lef)), right(std::move(rig)) { };

   _list getValue() override;

private:
   _genptr<_str> left;
   _genptr<_str> right;
};


struct Join_StrList : Generator<_list>
{

public:
   Join_StrList(_genptr<_str>& lef, _genptr<_list>& rig)
        : left(std::move(lef)), right(std::move(rig)) { };

   _list getValue() override;

private:
   _genptr<_str> left;
   _genptr<_list> right;
};


struct Join_ListStr : Generator<_list>
{

public:
   Join_ListStr(_genptr<_list>& lef, _genptr<_str>& rig)
        : left(std::move(lef)), right(std::move(rig)) { };

   _list getValue() override;

private:
   _genptr<_list> left;
   _genptr<_str> right;
};


struct Join_ListList : Generator<_list>
{

public:
   Join_ListList(_genptr<_list>& lef, _genptr<_list>& rig)
        : left(std::move(lef)), right(std::move(rig)) { };

   _list getValue() override;

private:
   _genptr<_list> left;
   _genptr<_list> right;
};

}

#endif /* GEN_LIST_H */

