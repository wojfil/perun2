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

#ifndef EXP_ELEMENT_H
#define EXP_ELEMENT_H

#include "generator.h"
#include "generator\gen-generic.h"
#include "primitives.h"


enum ElementType
{
   et_Operator = 0,
   et_Constant = 1,
   et_Generator = 2
};


// element of an expression:  logic (&|^) or numeric (+-*/%)
// this element is either an operator, a constant value or a pointer to a generator
template <typename T>
struct ExpElement
{
public:

   ExpElement<T> (const T& cnst) : generator(new Constant<T>(cnst)),
      _hasValue(true), constant(cnst), type(ElementType::et_Constant),
      _operator(0) {};
   ExpElement<T> (const _char& op) : generator(nullptr), _hasValue(false),
      _operator(op), type(ElementType::et_Operator), constant(T()) {};
   ExpElement<T> (Generator<T>* gen) : generator(gen), _hasValue(true),
      type(et_Generator), constant(T()), _operator(0) {};

   ~ExpElement<T> () {
      if (_hasValue) {
         delete generator;
      }
   }

   Generator<T>* takeValue() {
      _hasValue = false;
      Generator<T>* v = generator;
      generator = nullptr;
      return v;
   }

   _char _operator;
   const T constant;
   const ElementType type;

private:

   Generator<T>* generator;
   _boo _hasValue;
};

#endif /* EXP_ELEMENT_H */

