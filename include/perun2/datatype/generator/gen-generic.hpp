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

#pragma once

#include <algorithm>
#include <set>
#include "vector"
#include "../generator.hpp"
#include "../datatype.hpp"
#include "../../util.hpp"


namespace perun2::gen
{

template <typename T>
struct Constant : Generator<T>
{
public:
   Constant<T> (const T& val) : value(val) {};
   T getValue () override { return value; };

   p_bool isConstant() const override
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
   UnaryOperation<T> (p_genptr<T>& val) : value(std::move(val)) { };

protected:
   p_genptr<T> value;
};


template <typename T>
struct BinaryOperation : Generator<T>
{
public:
   BinaryOperation<T> (p_genptr<T>& val1, p_genptr<T>& val2)
      : value1(std::move(val1)), value2(std::move(val2)) { };

protected:
   p_genptr<T> value1;
   p_genptr<T> value2;
};


template <typename T>
struct Ternary : Generator<T>
{
public:
   Ternary<T> (p_genptr<p_bool>& cond, p_genptr<T>& le, p_genptr<T>& ri)
      : condition(std::move(cond)), left(std::move(le)), right(std::move(ri)) {};

   T getValue() override {
      return condition->getValue() ? left->getValue() : right->getValue();
   };

private:
   p_genptr<p_bool> condition;
   p_genptr<T> left;
   p_genptr<T> right;
};


template <typename T>
struct Binary : Generator<T>
{
public:
   Binary<T> (p_genptr<p_bool>& cond, p_genptr<T>& val)
      : condition(std::move(cond)), value(std::move(val)){};

   T getValue () override {
      return condition->getValue() ? value->getValue() : T();
   };

private:
   p_genptr<p_bool> condition;
   p_genptr<T> value;
};


template <typename T>
struct Listed : Generator<std::vector<T>>
{
public:
   Listed(std::vector<p_genptr<T>>& val) : length(val.size())
   {
      langutil::transferUniquePtrs(val, this->value);
   };

   std::vector<T> getValue() override {
      std::vector<T> list(length);
      for (p_size i = 0; i < length; i++) {
         list[i] = value[i]->getValue();
      }
      return list;
   }

private:
   std::vector<p_genptr<T>> value;
   const p_size length;
};


template <typename T>
struct ListedLists : Generator<std::vector<T>>
{
public:
   ListedLists(std::vector<p_genptr<std::vector<T>>>& val) : length(val.size())
   {
      langutil::transferUniquePtrs(val, this->value);
   };

   std::vector<T> getValue() override {
      std::vector<T> list;
      for (p_size i = 0; i < length; i++) {
         const std::vector<T> vec = value[i]->getValue();
         langutil::appendVector(list, vec);
      }
      return list;
   }

private:
   std::vector<p_genptr<std::vector<T>>> value;
   const p_size length;
};


template <typename T>
struct ListElement : Generator<T>
{
public:
   ListElement(p_genptr<std::vector<T>>& li, p_genptr<p_num>& id)
      : list(std::move(li)), index(std::move(id)) { };

   T getValue() override {
      const std::vector<T> lst = list->getValue();

      if (lst.empty()) {
         return T();
      }

      const p_num num = index->getValue();
      if (num.isNaN()) {
         return T();
      }

      p_nint id = num.toInt();

      if (id < NINT_ZERO) {
         id += lst.size();
      }

      return (id < NINT_ZERO || id >= static_cast<p_nint>(lst.size()))
         ? T()
         : lst[id];
   }

private:
   p_genptr<std::vector<T>> list;
   p_genptr<p_num> index;
};

}
