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

#ifndef FUNC_GENERIC_H
#define FUNC_GENERIC_H

#include "../generator.h"
#include "../datatype.h"
#include "../math.h"
#include "../../uroboros.h"


namespace uro::func
{

// one arg
template <typename T>
struct Func_1
{
public:
   Func_1<T> (_genptr<T>& a1) : arg1(std::move(a1)) {};

protected:
   _genptr<T> arg1;
};


// two args
template <typename T1, typename T2>
struct Func_2
{
public:
   Func_2<T1, T2> (_genptr<T1>& a1, _genptr<T2>& a2) 
      : arg1(std::move(a1)), arg2(std::move(a2)) {};

protected:
   _genptr<T1> arg1;
   _genptr<T2> arg2;
};


// three args
template <typename T1, typename T2, typename T3>
struct Func_3
{
public:
   Func_3<T1, T2, T3> (_genptr<T1>& a1, _genptr<T2>& a2, _genptr<T3>& a3) 
      : arg1(std::move(a1)), arg2(std::move(a2)), arg3(std::move(a3)) {};

protected:
   _genptr<T1> arg1;
   _genptr<T2> arg2;
   _genptr<T3> arg3;
};


template <typename T>
struct F_First : Func_1<std::vector<T>>, Generator<T>
{
public:
   F_First<T>(_genptr<std::vector<T>>& a1) : Func_1<std::vector<T>>(a1) {};

   T getValue() override {
      const std::vector<T> value = this->arg1->getValue();
      return value.empty()
         ? T()
         : value[0];
   }
};


template <typename T>
struct F_Last : Func_1<std::vector<T>>, Generator<T>
{
public:
   F_Last<T>(_genptr<std::vector<T>>& a1) : Func_1<std::vector<T>>(a1) {};

   T getValue() override {
      const std::vector<T> value = this->arg1->getValue();
      const _size length = value.size();
      return length == 0
         ? T()
         : value[length - 1];
   }
};


template <typename T>
struct F_RandomElement : Func_1<std::vector<T>>, Generator<T>
{
public:

   F_RandomElement<T> (_genptr<std::vector<T>>& a1, _uro& uro)
      : Func_1<std::vector<T>>(a1), math(uro.math) {};

   T getValue() override {
      const std::vector<T> value = this->arg1->getValue();
      const _size length = value.size();

      switch (length) {
         case 0: {
            return T();
         }
         case 1: {
            return value[0];
         }
         default: {
            return value[static_cast<_size>(this->math.randomInt(length - 1))];
         }
      }
   }

private:
   Math& math;
};

}

#endif /* FUNC_GENERIC_H */
