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

#ifndef GEN_STRING_H
#define GEN_STRING_H

#include "../generator.h"
#include "../datatype.h"
#include "../../util.h"
#include "../../uroboros.h"


namespace uro::gen
{

struct ConcatString : Generator<_str>
{
public:
   ConcatString(std::vector<_genptr<_str>>& val) : length(val.size()) 
   { 
      langutil::transferUniquePtrs(val, this->value);
   };

   _str getValue() override;

private:
   std::vector<_genptr<_str>> value;
   const _size length;
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
   LocationReference(Uroboros& uro) : inner(uro.vars.inner) { };
   _str getValue() override;

private:
   InnerVariables& inner;
};


struct RelativeLocation : Generator<_str>
{
public:
   RelativeLocation(_genptr<_str>& val, Uroboros& uro) 
      : value(std::move(val)), inner(uro.vars.inner) { };

   _str getValue() override;

private:
   InnerVariables& inner;
   _genptr<_str> value;
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
   DefinitionElement(_defptr& def, _genptr<_num>& ind, Uroboros& uro)
      : definition(std::move(def)), index(std::move(ind)), uroboros(uro) {};

   _str getValue() override;

private:
   Uroboros& uroboros;
   _defptr definition;
   _genptr<_num> index;
};

}

#endif /* GEN_STRING_H */

