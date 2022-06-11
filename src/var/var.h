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

#ifndef VAR_H
#define VAR_H

#include "../datatype/generator.h"
#include "../datatype/primitives.h"


template <typename T>
struct Variable : Generator<T>
{
public:
   Variable<T>() {};
   Variable<T>(T val) : value(val) {};
   ~Variable<T>() {};

   T getValue() override {
      return value;
   };

   T value;
};


template <typename T>
struct ParseVariable
{

public:
   ParseVariable<T>() : level(0), reachable(true), var(Variable<T>()) {};

   Variable<T>* getVarPtr() {
      return &var;
   }

   void bracketsUp() {
      if (reachable) {
         level++;
      }
   }

   void bracketsDown() {
      if (reachable) {
         level--;
         if (level < 0) {
            reachable = false;
         }
      }
   }

   void resurrect() {
      reachable = true;
      if (level < 0) {
         level = 0;
      }
   }

   _boo isReachable() const {
      return reachable;
   }

   Variable<T> var;

private:
   _int level;
   _boo reachable;
};

#endif /* VAR_H */
