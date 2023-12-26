/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FUNC_GENERIC_H
#define FUNC_GENERIC_H

#include "../generator.h"
#include "../datatype.h"
#include "../math.h"
#include "../../perun2.h"


namespace perun2::func
{

// one arg
template <typename T>
struct Func_1
{
public:
   Func_1<T> (p_genptr<T>& a1) : arg1(std::move(a1)) {};

protected:
   p_genptr<T> arg1;
};


// two args
template <typename T1, typename T2>
struct Func_2
{
public:
   Func_2<T1, T2> (p_genptr<T1>& a1, p_genptr<T2>& a2) 
      : arg1(std::move(a1)), arg2(std::move(a2)) {};

protected:
   p_genptr<T1> arg1;
   p_genptr<T2> arg2;
};


// three args
template <typename T1, typename T2, typename T3>
struct Func_3
{
public:
   Func_3<T1, T2, T3> (p_genptr<T1>& a1, p_genptr<T2>& a2, p_genptr<T3>& a3) 
      : arg1(std::move(a1)), arg2(std::move(a2)), arg3(std::move(a3)) {};

protected:
   p_genptr<T1> arg1;
   p_genptr<T2> arg2;
   p_genptr<T3> arg3;
};


template <typename T>
struct F_First : Func_1<std::vector<T>>, Generator<T>
{
public:
   F_First<T>(p_genptr<std::vector<T>>& a1) : Func_1<std::vector<T>>(a1) {};

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
   F_Last<T>(p_genptr<std::vector<T>>& a1) : Func_1<std::vector<T>>(a1) {};

   T getValue() override {
      const std::vector<T> value = this->arg1->getValue();
      return value.empty()
         ? T()
         : value[value.size() - 1];
   }
};


template <typename T>
struct F_RandomElement : Func_1<std::vector<T>>, Generator<T>
{
public:

   F_RandomElement<T> (p_genptr<std::vector<T>>& a1, p_perun2& p2)
      : Func_1<std::vector<T>>(a1), math(p2.math) {};

   T getValue() override {
      const std::vector<T> value = this->arg1->getValue();

      switch (value.size()) {
         case 0: {
            return T();
         }
         case 1: {
            return value[0];
         }
         default: {
            return value[static_cast<p_size>(this->math.randomInt(value.size() - 1))];
         }
      }
   }

private:
   Math& math;
};

}

#endif /* FUNC_GENERIC_H */
