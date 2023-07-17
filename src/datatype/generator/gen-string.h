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

#ifndef GEN_STRING_H
#define GEN_STRING_H

#include "../generator.h"
#include "../datatype.h"
#include "../../util.h"

namespace perun2
{
   struct p_perun2;
   struct LocationContext;
}

namespace perun2::gen
{


struct ConcatString_2 : Generator<p_str>
{
public:
   ConcatString_2(_genptr<p_str>& v1, _genptr<p_str>& v2);
   p_str getValue() override;

private:
   _genptr<p_str> value1;
   _genptr<p_str> value2;
};


struct ConcatString_3 : Generator<p_str>
{
public:
   ConcatString_3(_genptr<p_str>& v1, _genptr<p_str>& v2, _genptr<p_str>& v3);
   p_str getValue() override;

private:
   _genptr<p_str> value1;
   _genptr<p_str> value2;
   _genptr<p_str> value3;
};


struct ConcatString_4 : Generator<p_str>
{
public:
   ConcatString_4(_genptr<p_str>& v1, _genptr<p_str>& v2, _genptr<p_str>& v3, _genptr<p_str>& v4);
   p_str getValue() override;

private:
   _genptr<p_str> value1;
   _genptr<p_str> value2;
   _genptr<p_str> value3;
   _genptr<p_str> value4;
};


struct ConcatString_5 : Generator<p_str>
{
public:
   ConcatString_5(_genptr<p_str>& v1, _genptr<p_str>& v2, _genptr<p_str>& v3, _genptr<p_str>& v4, _genptr<p_str>& v5);
   p_str getValue() override;

private:
   _genptr<p_str> value1;
   _genptr<p_str> value2;
   _genptr<p_str> value3;
   _genptr<p_str> value4;
   _genptr<p_str> value5;
};


struct ConcatString_6 : Generator<p_str>
{
public:
   ConcatString_6(_genptr<p_str>& v1, _genptr<p_str>& v2, _genptr<p_str>& v3, _genptr<p_str>& v4, _genptr<p_str>& v5, _genptr<p_str>& v6);
   p_str getValue() override;

private:
   _genptr<p_str> value1;
   _genptr<p_str> value2;
   _genptr<p_str> value3;
   _genptr<p_str> value4;
   _genptr<p_str> value5;
   _genptr<p_str> value6;
};


struct ConcatString_Multi : Generator<p_str>
{
public:
   ConcatString_Multi(std::vector<_genptr<p_str>>& val);
   p_str getValue() override;

private:
   std::vector<_genptr<p_str>> values;
};


struct StringBinary : Generator<p_str>
{
public:
   StringBinary (_genptr<p_bool>& cond, _genptr<p_str>& val)
      : condition(std::move(cond)), value(std::move(val)) { };

   p_str getValue() override;

private:
   _genptr<p_bool> condition;
   _genptr<p_str> value;
};


struct LocationReference : Generator<p_str>
{
public:
   LocationReference(p_perun2& p2);
   p_str getValue() override;

private:
   LocationContext& context;
};


struct RelativeLocation : Generator<p_str>
{
public:
   RelativeLocation(_genptr<p_str>& val, p_perun2& p2, const p_int retr);
   p_str getValue() override;

private:
   LocationContext& context;
   _genptr<p_str> value;
   const p_int retreats;
};


struct RetreatedPath : Generator<p_str>
{
public:
   RetreatedPath(_genptr<p_str>& val, const p_int retr);
   p_str getValue() override;

private:
   _genptr<p_str> value;
   const p_int reatreats;
};


struct CharAtIndex : Generator<p_str>
{
public:
   CharAtIndex (_genptr<p_str>& val, _genptr<_num>& ind)
      : value(std::move(val)), index(std::move(ind)) { };

   p_str getValue() override;

private:
   _genptr<p_str> value;
   _genptr<_num> index;
};


struct DefinitionElement : Generator<p_str>
{
public:
   DefinitionElement(_defptr& def, _genptr<_num>& ind, p_perun2& p2)
      : definition(std::move(def)), index(std::move(ind)), perun2(p2) {};

   p_str getValue() override;

private:
   p_perun2& perun2;
   _defptr definition;
   _genptr<_num> index;
};

}

#endif /* GEN_STRING_H */

