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

#ifndef BETWEEN_H
#define BETWEEN_H


#include "datatype.h"
#include "generator.h"


namespace perun2::gen
{

template <typename T>
struct Between : Generator<p_bool>
{
public:
   Between<T>(p_genptr<T>& val, p_genptr<T>& b1, p_genptr<T>& b2)
      : value(std::move(val)), bound1(std::move(b1)), bound2(std::move(b2)) { };

   p_bool getValue() override 
   {
      const T v = value->getValue();
      const T b1 = bound1->getValue();
      const T b2 = bound2->getValue();

      if (b1 < b2) {
         return v >= b1 && v <= b2;
      }

      return v >= b2 && v <= b1;
   };

private:
   p_genptr<T> value;
   p_genptr<T> bound1;
   p_genptr<T> bound2;
};



template <typename T>
struct BetweenHalfConst : Generator<p_bool>
{
public:
   BetweenHalfConst<T>(p_genptr<T>& val, p_genptr<T>& b1, const T& b2)
      : value(std::move(val)), bound1(std::move(b1)), bound2(b2) { };

   p_bool getValue() override 
   {
      const T v = value->getValue();
      const T b1 = bound1->getValue();

      if (b1 < bound2) {
         return v >= b1 && v <= bound2;
      }

      return v >= bound2 && v <= b1;
   };

private:
   p_genptr<T> value;
   p_genptr<T> bound1;
   const T bound2;
};



template <typename T>
struct BetweenConst : Generator<p_bool>
{
public:
   BetweenConst<T>(p_genptr<T>& val, const T& b1, const T& b2)
      : value(std::move(val)), 
        bound1((b1 < b2) ? b1 : b2), 
        bound2((b1 > b2) ? b1 : b2) { };

   p_bool getValue() override 
   {
      const T v = value->getValue();
      return v >= bound1 && v <= bound2;
   };

private:
   p_genptr<T> value;
   const T bound1;
   const T bound2;
};


}

#endif /* BETWEEN_H */
