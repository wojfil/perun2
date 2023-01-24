/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_REF_H_INCLUDED
#define GEN_REF_H_INCLUDED

#include "datatype.h"
#include "../var/var.h"


namespace uro::gen
{

template <typename T>
struct GeneratorRef : Generator<T>
{
public:
   GeneratorRef(Generator<T>& val)
      : value(val) { };

   _bool isConstant() const override
   {
      return this->value.isConstant();
   };

   T getValue() override
   {
      return this->value.getValue();
   };

private:
   Generator<T>& value;
};


template <typename T>
struct GeneratorPtrRef : Generator<T>
{
public:
   GeneratorPtrRef(_genptr<T>& val)
      : value(val) { };

   _bool isConstant() const override
   {
      return this->value->isConstant();
   };

   T getValue() override
   {
      return this->value->getValue();
   };

private:
   _genptr<T>& value;
};


// cast generator of int numbers
// to a generator of numbers
struct NumberIntRef : Generator<_num>
{
public:
   NumberIntRef(vars::Variable<_numi>& val);
   _bool isConstant() const override;
   _num getValue() override;

private:
   vars::Variable<_numi>& value;
};

}

#endif // GEN_REF_H_INCLUDED
