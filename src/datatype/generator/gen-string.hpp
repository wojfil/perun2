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

#pragma once

#include "../generator.hpp"
#include "../datatype.hpp"
#include "../../util.hpp"

namespace perun2
{
   struct Perun2Process;
   struct LocationContext;
}

namespace perun2::gen
{


struct ConcatString_2 : Generator<p_str>
{
public:
   ConcatString_2(p_genptr<p_str>& v1, p_genptr<p_str>& v2);
   p_str getValue() override;

private:
   p_genptr<p_str> value1;
   p_genptr<p_str> value2;
};


struct ConcatString_3 : Generator<p_str>
{
public:
   ConcatString_3(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3);
   p_str getValue() override;

private:
   p_genptr<p_str> value1;
   p_genptr<p_str> value2;
   p_genptr<p_str> value3;
};


struct ConcatString_4 : Generator<p_str>
{
public:
   ConcatString_4(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3, p_genptr<p_str>& v4);
   p_str getValue() override;

private:
   p_genptr<p_str> value1;
   p_genptr<p_str> value2;
   p_genptr<p_str> value3;
   p_genptr<p_str> value4;
};


struct ConcatString_5 : Generator<p_str>
{
public:
   ConcatString_5(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3, p_genptr<p_str>& v4, p_genptr<p_str>& v5);
   p_str getValue() override;

private:
   p_genptr<p_str> value1;
   p_genptr<p_str> value2;
   p_genptr<p_str> value3;
   p_genptr<p_str> value4;
   p_genptr<p_str> value5;
};


struct ConcatString_6 : Generator<p_str>
{
public:
   ConcatString_6(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3, p_genptr<p_str>& v4, p_genptr<p_str>& v5, p_genptr<p_str>& v6);
   p_str getValue() override;

private:
   p_genptr<p_str> value1;
   p_genptr<p_str> value2;
   p_genptr<p_str> value3;
   p_genptr<p_str> value4;
   p_genptr<p_str> value5;
   p_genptr<p_str> value6;
};


struct ConcatString_Multi : Generator<p_str>
{
public:
   ConcatString_Multi(std::vector<p_genptr<p_str>>& val);
   p_str getValue() override;

private:
   std::vector<p_genptr<p_str>> values;
};


struct StringBinary : Generator<p_str>
{
public:
   StringBinary (p_genptr<p_bool>& cond, p_genptr<p_str>& val)
      : condition(std::move(cond)), value(std::move(val)) { };

   p_str getValue() override;

private:
   p_genptr<p_bool> condition;
   p_genptr<p_str> value;
};


struct LocationReference : Generator<p_str>
{
public:
   LocationReference(Perun2Process& p2);
   p_str getValue() override;

private:
   LocationContext& context;
};


struct RelativeLocation : Generator<p_str>
{
public:
   RelativeLocation(p_genptr<p_str>& val, Perun2Process& p2, const p_int retr);
   p_str getValue() override;

private:
   LocationContext& context;
   p_genptr<p_str> value;
   const p_int retreats;
};


struct RetreatedPath : Generator<p_str>
{
public:
   RetreatedPath(p_genptr<p_str>& val, const p_int retr);
   p_str getValue() override;

private:
   p_genptr<p_str> value;
   const p_int reatreats;
};


struct CharAtIndex : Generator<p_str>
{
public:
   CharAtIndex (p_genptr<p_str>& val, p_genptr<p_num>& ind)
      : value(std::move(val)), index(std::move(ind)) { };

   p_str getValue() override;

private:
   p_genptr<p_str> value;
   p_genptr<p_num> index;
};


struct DefinitionElement : Generator<p_str>
{
public:
   DefinitionElement(p_defptr& def, p_genptr<p_num>& ind, Perun2Process& p2)
      : definition(std::move(def)), index(std::move(ind)), perun2(p2) {};

   p_str getValue() override;

private:
   Perun2Process& perun2;
   p_defptr definition;
   p_genptr<p_num> index;
};

}
