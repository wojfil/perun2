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

#ifndef GEN_GENERIC_H
#define GEN_GENERIC_H

#include <algorithm>
#include <set>
#include "vector"
#include "../generator.h"
#include "../datatype.h"
#include "../../util.h"
#include "../../uroboros.h"
#include "../../var/var.h"


namespace uro::gen
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

      return id < 0LL || id >= length
         ? T()
         : lst[id];
   }

private:
   _genptr<std::vector<T>> list;
   _genptr<_num> index;
};


template <typename T>
struct Filter_Where : Generator<std::vector<T>>
{
public:
   Filter_Where(_genptr<std::vector<T>>& li, _genptr<_bool>& cond, Attribute* attr, Uroboros& uro)
      : list(std::move(li)), condition(std::move(cond)), uroboros(uro), inner(uro.vars.inner),
        this_(nullptr), attribute(attr), hasAttribute(attr != nullptr)
   {
      uro.vars.inner.createThisVarRef(this_);
   };

   std::vector<T> getValue() override {
      const std::vector<T> values = list->getValue();
      std::vector<T> result;
      const _numi length = _numi(static_cast<_nint>(values.size()));

      const _numi prevIndex = this->inner.index.value;
      const T prevThis = this->this_->value;

      this->inner.index.value.setToZero();
      _numi index(0LL);

      while (this->uroboros.state == State::s_Running && index != length) {
         const T& unit = values[index.value.i];
         this->this_->value = unit;
         if (this->hasAttribute) {
            this->attribute->run();
         }

         if (condition->getValue()) {
            result.emplace_back(unit);
         }

         this->inner.index.value++;
         index++;
      }

      this->inner.index.value = prevIndex;
      this->this_->value = prevThis;

      return result;
   }

private:
   Uroboros& uroboros;
   InnerVariables& inner;
   vars::Variable<T>* this_;
   _genptr<std::vector<T>> list;
   _genptr<_bool> condition;
   Attribute* attribute;
   const _bool hasAttribute;
};



template <typename T>
struct Filter_Limit : Generator<std::vector<T>>
{
public:
   Filter_Limit(_genptr<std::vector<T>>& li, _genptr<_num>& num)
      : list(std::move(li)), number(std::move(num)) { };

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();

      if (n <= 0LL) {
         return std::vector<T>();
      }

      const std::vector<T> lst = list->getValue();

      return n >= lst.size()
         ? lst
         : std::vector<T>(lst.begin(), lst.begin() + n);
   }

private:
   _genptr<std::vector<T>> list;
   _genptr<_num> number;
};


template <typename T>
struct Filter_Skip : Generator<std::vector<T>>
{
public:
   Filter_Skip(_genptr<std::vector<T>>& li, _genptr<_num>& num)
      : list(std::move(li)), number(std::move(num)) { };

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();
      const std::vector<T> lst = list->getValue();

      if (n <= 0LL) {
         return lst;
      }

      return n >= lst.size()
         ? std::vector<T>()
         : std::vector<T>(lst.begin() + n, lst.end());
   }

private:
   _genptr<std::vector<T>> list;
   _genptr<_num> number;
};


template <typename T>
struct Filter_Every : Generator<std::vector<T>>
{
public:
   Filter_Every(_genptr<std::vector<T>>& li, _genptr<_num>& num)
      : list(std::move(li)), number(std::move(num)) { };

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();
      const std::vector<T> lst = list->getValue();

      if (n <= 1LL) {
         return lst;
      }

      const _size baseSize = lst.size();
      const _size newSize = (baseSize / n) + ((baseSize % n == 0) ? 0 : 1);
      std::vector<T> result(newSize);

      for (_size i = 0; i < newSize; i++) {
         result[i] = lst[i * n];
      }

      return result;
   }

private:
   _genptr<std::vector<T>> list;
   _genptr<_num> number;
};


template <typename T>
struct Filter_Final : Generator<std::vector<T>>
{
public:
   Filter_Final(_genptr<std::vector<T>>& li, _genptr<_num>& num)
      : list(std::move(li)), number(std::move(num)) { };

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();

      if (n <= 0LL) {
         return std::vector<T>();
      }

      const std::vector<T> lst = list->getValue();

      return n >= lst.size()
         ? lst
         : std::vector<T>(lst.end() - n, lst.end());
   }

private:
   _genptr<std::vector<T>> list;
   _genptr<_num> number;
};


template <typename T>
struct VarReference : Generator<T>
{
public:
   VarReference(vars::Variable<T>* var) : variable(var) { };

   T getValue() override {
      return variable->value;
   }

private:
   vars::Variable<T>* variable;
};

}

#endif /* GEN_GENERIC_H */
