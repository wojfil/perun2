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

#ifndef GEN_REF_H_INCLUDED
#define GEN_REF_H_INCLUDED

#include "datatype.h"
#include "generator.h"


template <typename T>
struct GeneratorRef : Generator<T>
{
public:
   GeneratorRef(Generator<T>* val) : value(val) {};

   T getValue() override {
      return value->getValue();
   }

private:
   Generator<T>* value;
   // do not call "delete" here
   // this struct is a weak reference to a generator
};


// cast generator of int numbers
// to a generator of numbers
struct NumberIntRef : Generator<_num>
{
public:
   NumberIntRef(Generator<_numi>* val);
   _num getValue() override;

private:
   Generator<_numi>* value;
};


#endif // GEN_REF_H_INCLUDED
