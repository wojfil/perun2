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


namespace uro::vars
{

template <typename T>
struct Variable : Generator<T>
{
public:
   Variable<T>() {};
   Variable<T>(T val) : value(val) {};
   ~Variable<T>() override {};

   _bool isConstant() const override
   {
      return this->isConstant_;
   };


   T getValue() override {
      return this->value;
   };

   T value;
   _bool isConstant_ = false;
};


template <typename T>
struct ParseVariable
{

public:
   ParseVariable<T>() { };

   Variable<T>* getVarPtr() {
      return &this->var;
   }

   void bracketsUp()
   {
      if (this->isReachable_) {
         this->level++;
      }
   }

   void bracketsDown()
   {
      if (this->isReachable_) {
         this->level--;
         if (this->level < 0) {
            this->isReachable_ = false;
         }
      }
   }

   void resurrect(const _bool& isConst)
   {
      this->isReachable_ = true;
      this->var.isConstant_ = isConst;
      if (this->level < 0) {
         this->level = 0;
      }
   }

   _bool isReachable() const
   {
      return this->isReachable_;
   }

   void makeNotConstant()
   {
      this->var.isConstant_ = false;
   }

   void actualizeConstantness(const _bool& isConst)
   {
      if (this->var.isConstant_) {
         if (!isConst) {
            this->var.isConstant_ = false;
         }
      }
   }

   Variable<T> var;

private:
   _int level = 0;
   _bool isReachable_ = true;
};

}

#endif /* VAR_H */
