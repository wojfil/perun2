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

#ifndef GEN_LIST_H
#define GEN_LIST_H

#include "../generator.h"
#include "../order.h"
#include "../../attribute.h"
#include "../../perun2.h"
#include "../../os.h"


namespace perun2::gen
{

struct Join_StrStr : Generator<_list>
{
public:
   Join_StrStr(_genptr<_str>& lef, _genptr<_str>& rig);
   _list getValue() override;

private:
   _genptr<_str> left;
   _genptr<_str> right;
};


struct Join_StrList : Generator<_list>
{
public:
   Join_StrList(_genptr<_str>& lef, _genptr<_list>& rig);
   _list getValue() override;

private:
   _genptr<_str> left;
   _genptr<_list> right;
};


struct Join_ListStr : Generator<_list>
{
public:
   Join_ListStr(_genptr<_list>& lef, _genptr<_str>& rig);
   _list getValue() override;

private:
   _genptr<_list> left;
   _genptr<_str> right;
};


struct Join_ListList : Generator<_list>
{
public:
   Join_ListList(_genptr<_list>& lef, _genptr<_list>& rig);
   _list getValue() override;

private:
   _genptr<_list> left;
   _genptr<_list> right;
};


struct ListFilter_Where : Generator<_list>
{
public:
   ListFilter_Where(_genptr<_bool>& cond, _genptr<_list>& li, _fcptr& ctx, _p2& p2);
   _list getValue() override;

private:
   _genptr<_bool> condition;
   _genptr<_list> list;
   _fcptr context;
   _p2& perun2;
};


struct ListFilter_Limit : Generator<_list>
{
public:
   ListFilter_Limit(_genptr<_list>& li, _genptr<_num>& num);
   _list getValue() override;

private:
   _genptr<_list> list;
   _genptr<_num> number;
};


struct ListFilter_Skip : Generator<_list>
{
public:
   ListFilter_Skip(_genptr<_list>& li, _genptr<_num>& num);
   _list getValue() override;

private:
   _genptr<_list> list;
   _genptr<_num> number;
};


struct ListFilter_Every : Generator<_list>
{
public:
   ListFilter_Every(_genptr<_list>& li, _genptr<_num>& num);
   _list getValue() override;

private:
   _genptr<_list> list;
   _genptr<_num> number;
};


struct ListFilter_Final : Generator<_list>
{
public:
   ListFilter_Final(_genptr<_list>& li, _genptr<_num>& num);
   _list getValue() override;

private:
   _genptr<_list> list;
   _genptr<_num> number;
};


}

#endif /* GEN_LIST_H */

