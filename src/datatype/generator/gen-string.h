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
   ConcatString(std::vector<Generator<_str>*>* val) : value(val),
      length(val->size()) { };

   ~ConcatString()
   {
      langutil::deleteVectorPtr(value);
   }

   _str getValue() override;

private:
   std::vector<Generator<_str>*>* value;
   const _size length;
};


struct StringBinary : Generator<_str>
{
public:
   StringBinary (Generator<_bool>* cond, Generator<_str>* val)
      : condition(cond), value(val) { };

   ~StringBinary()
   {
      delete condition;
      delete value;
   }

   _str getValue() override;

private:
   Generator<_bool>* condition;
   Generator<_str>* value;
};


struct LocationReference : Generator<_str>
{
public:
   LocationReference(uro::Uroboros* uro) : inner(&uro->vars.inner) { };
   _str getValue() override;

private:
   uro::InnerVariables* inner;
};


struct RelativeLocation : Generator<_str>
{
public:
   RelativeLocation(Generator<_str>* val, uro::Uroboros* uro) 
      : value(val), inner(&uro->vars.inner) { };

   ~RelativeLocation()
   {
      delete value;
   }

   _str getValue() override;

private:
   uro::InnerVariables* inner;
   Generator<_str>* value;
};


struct CharAtIndex : Generator<_str>
{
public:
   CharAtIndex (Generator<_str>* val, Generator<_num>* ind)
      : value(val), index(ind) { };

   ~CharAtIndex()
   {
      delete value;
      delete index;
   }

   _str getValue() override;

private:
   Generator<_str>* value;
   Generator<_num>* index;
};


struct DefinitionElement : Generator<_str>
{
public:
   DefinitionElement(_def* def, Generator<_num>* in)
      : definition(def), index(in) {};

   ~DefinitionElement()
   {
      delete definition;
      delete index;
   }

   _str getValue() override;

private:
   _def* definition;
   Generator<_num>* index;
};

}

#endif /* GEN_STRING_H */

