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
   struct _p2;
   struct LocationContext;
}

namespace perun2::gen
{


struct ConcatString_2 : Generator<_str>
{
public:
   ConcatString_2(_genptr<_str>& v1, _genptr<_str>& v2);
   _str getValue() override;

private:
   _genptr<_str> value1;
   _genptr<_str> value2;
};


struct ConcatString_3 : Generator<_str>
{
public:
   ConcatString_3(_genptr<_str>& v1, _genptr<_str>& v2, _genptr<_str>& v3);
   _str getValue() override;

private:
   _genptr<_str> value1;
   _genptr<_str> value2;
   _genptr<_str> value3;
};


struct ConcatString_4 : Generator<_str>
{
public:
   ConcatString_4(_genptr<_str>& v1, _genptr<_str>& v2, _genptr<_str>& v3, _genptr<_str>& v4);
   _str getValue() override;

private:
   _genptr<_str> value1;
   _genptr<_str> value2;
   _genptr<_str> value3;
   _genptr<_str> value4;
};


struct ConcatString_5 : Generator<_str>
{
public:
   ConcatString_5(_genptr<_str>& v1, _genptr<_str>& v2, _genptr<_str>& v3, _genptr<_str>& v4, _genptr<_str>& v5);
   _str getValue() override;

private:
   _genptr<_str> value1;
   _genptr<_str> value2;
   _genptr<_str> value3;
   _genptr<_str> value4;
   _genptr<_str> value5;
};


struct ConcatString_6 : Generator<_str>
{
public:
   ConcatString_6(_genptr<_str>& v1, _genptr<_str>& v2, _genptr<_str>& v3, _genptr<_str>& v4, _genptr<_str>& v5, _genptr<_str>& v6);
   _str getValue() override;

private:
   _genptr<_str> value1;
   _genptr<_str> value2;
   _genptr<_str> value3;
   _genptr<_str> value4;
   _genptr<_str> value5;
   _genptr<_str> value6;
};


struct ConcatString_Multi : Generator<_str>
{
public:
   ConcatString_Multi(std::vector<_genptr<_str>>& val);
   _str getValue() override;

private:
   std::vector<_genptr<_str>> values;
};


struct StringBinary : Generator<_str>
{
public:
   StringBinary (_genptr<_bool>& cond, _genptr<_str>& val)
      : condition(std::move(cond)), value(std::move(val)) { };

   _str getValue() override;

private:
   _genptr<_bool> condition;
   _genptr<_str> value;
};


struct LocationReference : Generator<_str>
{
public:
   LocationReference(_p2& p2);
   _str getValue() override;

private:
   LocationContext& context;
};


struct RelativeLocation : Generator<_str>
{
public:
   RelativeLocation(_genptr<_str>& val, _p2& p2, const _int retr);
   _str getValue() override;

private:
   LocationContext& context;
   _genptr<_str> value;
   const _int retreats;
};


struct RetreatedPath : Generator<_str>
{
public:
   RetreatedPath(_genptr<_str>& val, const _int retr);
   _str getValue() override;

private:
   _genptr<_str> value;
   const _int reatreats;
};


struct CharAtIndex : Generator<_str>
{
public:
   CharAtIndex (_genptr<_str>& val, _genptr<_num>& ind)
      : value(std::move(val)), index(std::move(ind)) { };

   _str getValue() override;

private:
   _genptr<_str> value;
   _genptr<_num> index;
};


struct DefinitionElement : Generator<_str>
{
public:
   DefinitionElement(_defptr& def, _genptr<_num>& ind, _p2& p2)
      : definition(std::move(def)), index(std::move(ind)), perun2(p2) {};

   _str getValue() override;

private:
   _p2& perun2;
   _defptr definition;
   _genptr<_num> index;
};

}

#endif /* GEN_STRING_H */

