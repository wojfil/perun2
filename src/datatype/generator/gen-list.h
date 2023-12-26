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

#ifndef GEN_LIST_H
#define GEN_LIST_H

#include "../generator.h"
#include "../order.h"
#include "../../attribute.h"
#include "../../perun2.h"
#include "../../os.h"


namespace perun2::gen
{

struct Join_StrStr : Generator<p_list>
{
public:
   Join_StrStr(p_genptr<p_str>& lef, p_genptr<p_str>& rig);
   p_list getValue() override;

private:
   p_genptr<p_str> left;
   p_genptr<p_str> right;
};


struct Join_StrList : Generator<p_list>
{
public:
   Join_StrList(p_genptr<p_str>& lef, p_genptr<p_list>& rig);
   p_list getValue() override;

private:
   p_genptr<p_str> left;
   p_genptr<p_list> right;
};


struct Join_ListStr : Generator<p_list>
{
public:
   Join_ListStr(p_genptr<p_list>& lef, p_genptr<p_str>& rig);
   p_list getValue() override;

private:
   p_genptr<p_list> left;
   p_genptr<p_str> right;
};


struct Join_ListList : Generator<p_list>
{
public:
   Join_ListList(p_genptr<p_list>& lef, p_genptr<p_list>& rig);
   p_list getValue() override;

private:
   p_genptr<p_list> left;
   p_genptr<p_list> right;
};


struct ListFilter_Where : Generator<p_list>
{
public:
   ListFilter_Where(p_genptr<p_bool>& cond, p_genptr<p_list>& li, p_fcptr& ctx, p_perun2& p2);
   p_list getValue() override;

private:
   p_genptr<p_bool> condition;
   p_genptr<p_list> list;
   p_fcptr context;
   p_perun2& perun2;
};


struct ListFilter_Limit : Generator<p_list>
{
public:
   ListFilter_Limit(p_genptr<p_list>& li, p_genptr<p_num>& num);
   p_list getValue() override;

private:
   p_genptr<p_list> list;
   p_genptr<p_num> number;
};


struct ListFilter_Skip : Generator<p_list>
{
public:
   ListFilter_Skip(p_genptr<p_list>& li, p_genptr<p_num>& num);
   p_list getValue() override;

private:
   p_genptr<p_list> list;
   p_genptr<p_num> number;
};


struct ListFilter_Every : Generator<p_list>
{
public:
   ListFilter_Every(p_genptr<p_list>& li, p_genptr<p_num>& num);
   p_list getValue() override;

private:
   p_genptr<p_list> list;
   p_genptr<p_num> number;
};


struct ListFilter_Final : Generator<p_list>
{
public:
   ListFilter_Final(p_genptr<p_list>& li, p_genptr<p_num>& num);
   p_list getValue() override;

private:
   p_genptr<p_list> list;
   p_genptr<p_num> number;
};


}

#endif /* GEN_LIST_H */

