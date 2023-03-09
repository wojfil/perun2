/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_GENERIC_H
#define GEN_GENERIC_H

#include <algorithm>
#include <set>
#include "vector"
#include "../generator.h"
#include "../datatype.h"
#include "../../util.h"


namespace perun2::gen
{

template <typename T>
struct Constant : Generator<T>
{
public:
   Constant<T> (const T& val) : value(val) {};
   T getValue () override { return value; };

   _bool isConstant() const override
   {
      return true;
   };

private:
   const T value;
};


template<typename T>
struct UnaryOperation : Generator<T>
{
public:
   UnaryOperation<T> (_genptr<T>& val) : value(std::move(val)) { };

protected:
   _genptr<T> value;
};


template <typename T>
struct BinaryOperation : Generator<T>
{
public:
   BinaryOperation<T> (_genptr<T>& val1, _genptr<T>& val2)
      : value1(std::move(val1)), value2(std::move(val2)) { };

protected:
   _genptr<T> value1;
   _genptr<T> value2;
};


template <typename T>
struct Ternary : Generator<T>
{
public:
   Ternary<T> (_genptr<_bool>& cond, _genptr<T>& le, _genptr<T>& ri)
      : condition(std::move(cond)), left(std::move(le)), right(std::move(ri)) {};

   T getValue() override {
      return condition->getValue() ? left->getValue() : right->getValue();
   };

private:
   _genptr<_bool> condition;
   _genptr<T> left;
   _genptr<T> right;
};


template <typename T>
struct Binary : Generator<T>
{
public:
   Binary<T> (_genptr<_bool>& cond, _genptr<T>& val)
      : condition(std::move(cond)), value(std::move(val)){};

   T getValue () override {
      return condition->getValue() ? value->getValue() : T();
   };

private:
   _genptr<_bool> condition;
   _genptr<T> value;
};


template <typename T>
struct Listed : Generator<std::vector<T>>
{
public:
   Listed(std::vector<_genptr<T>>& val) : length(val.size())
   {
      langutil::transferUniquePtrs(val, this->value);
   };

   std::vector<T> getValue() override {
      std::vector<T> list(length);
      for (_size i = 0; i < length; i++) {
         list[i] = value[i]->getValue();
      }
      return list;
   }

private:
   std::vector<_genptr<T>> value;
   const _size length;
};


template <typename T>
struct ListedLists : Generator<std::vector<T>>
{
public:
   ListedLists(std::vector<_genptr<std::vector<T>>>& val) : length(val.size())
   {
      langutil::transferUniquePtrs(val, this->value);
   };

   std::vector<T> getValue() override {
      std::vector<T> list;
      for (_size i = 0; i < length; i++) {
         const std::vector<T> vec = value[i]->getValue();
         langutil::appendVector(list, vec);
      }
      return list;
   }

private:
   std::vector<_genptr<std::vector<T>>> value;
   const _size length;
};


template <typename T>
struct ListElement : Generator<T>
{
public:
   ListElement(_genptr<std::vector<T>>& li, _genptr<_num>& id)
      : list(std::move(li)), index(std::move(id)) { };

   T getValue() override {
      const std::vector<T> lst = list->getValue();
      const _size length = lst.size();

      if (length == 0) {
         return T();
      }

      const _nint id = index->getValue().toInt();

      return id < NINT_ZERO || id >= length
         ? T()
         : lst[id];
   }

private:
   _genptr<std::vector<T>> list;
   _genptr<_num> index;
};

}

#endif /* GEN_GENERIC_H */
