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


template <typename T>
struct Constant : Generator<T>
{
public:
   Constant<T> (const T val) : value(val) {};
   T getValue () override { return value; };

   _boo isConstant()
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
   UnaryOperation<T> (Generator<T>* val) : value(val) { };
protected:
   ~UnaryOperation<T> () { delete value; };
   Generator<T>* value;
};


template <typename T>
struct BinaryOperation : Generator<T>
{
public:
   BinaryOperation<T> (Generator<T>* val1, Generator<T>* val2)
      : value1(val1), value2(val2) { };
protected:
   ~BinaryOperation<T>() {
      delete value1;
      delete value2;
   };
   Generator<T>* value1;
   Generator<T>* value2;
};


template <typename T>
struct Ternary : Generator<T>
{
public:
   Ternary<T> (Generator<_boo>* cond, Generator<T>* le, Generator<T>* ri)
      : condition(cond), left(le), right(ri) {};

   ~Ternary<T>() {
      delete condition;
      delete left;
      delete right;
   }

   T getValue() override {
      return condition->getValue() ? left->getValue() : right->getValue();
   };

private:
   Generator<_boo>* condition;
   Generator<T>* left;
   Generator<T>* right;
};


template <typename T>
struct Binary : Generator<T>
{
public:
   Binary<T> (Generator<_boo>* cond, Generator<T>* val)
      : condition(cond), value(val){};

   T getValue () override {
      return condition->getValue() ? value->getValue() : T();
   };

   ~Binary<T>() {
      delete condition;
      delete value;
   }

private:
   Generator<_boo>* condition;
   Generator<T>* value;
};


template <typename T>
struct Listed : Generator<std::vector<T>>
{
public:
   Listed(std::vector<Generator<T>*>* val) : value(val),
      length(val->size()) { };

   ~Listed() {
      deleteVectorPtr(value);
   }

   std::vector<T> getValue() override {
      std::vector<T> list(length);
      for (_size i = 0; i < length; i++) {
         list[i] = (*value)[i]->getValue();
      }
      return list;
   }

private:
   std::vector<Generator<T>*>* value;
   const _size length;
};


template <typename T>
struct ListedLists : Generator<std::vector<T>>
{
public:
   ListedLists(std::vector<Generator<std::vector<T>>*>* val) : value(val),
      length(val->size()) { };

   ~ListedLists() {
      deleteVectorPtr(value);
   }

   std::vector<T> getValue() override {
      std::vector<T> list;
      for (_size i = 0; i < length; i++) {
         const std::vector<T> vec = (*value)[i]->getValue();
         appendVector(list, vec);
      }
      return list;
   }

private:
   std::vector<Generator<std::vector<T>>*>* value;
   const _size length;
};


template <typename T>
struct ListElement : Generator<T>
{
public:
   ListElement(Generator<std::vector<T>>* li, Generator<_num>* id) :
      list(li), index(id) { };

   ~ListElement() {
      delete list;
      delete index;
   }

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
   Generator<std::vector<T>>* list;
   Generator<_num>* index;
};


template <typename T>
struct Filter_Where : Generator<std::vector<T>>
{
public:
   Filter_Where(Generator<std::vector<T>>* li, Generator<_boo>* cond, Uroboros* uro)
      : list(li), condition(cond), uroboros(uro), inner(&uro->vars.inner),
        this_(nullptr)
   {
      uro->vars.inner.createThisVarRef(this_);
   };

   ~Filter_Where() {
      delete list;
      delete condition;
   }

   std::vector<T> getValue() override {
      const std::vector<T> values = list->getValue();
      std::vector<T> result;
      const _numi length = _numi(static_cast<_nint>(values.size()));

      const _numi prevIndex = this->inner->index.value;
      const T prevThis = this->this_->value;

      this->inner->index.value.setToZero();
      _numi index(0LL);

      while (this->uroboros->running && index != length) {
         const T& unit = values[index.value.i];
         this->this_->value = unit;

         if (condition->getValue()) {
            result.push_back(unit);
         }

         this->inner->index.value++;
         index++;
      }

      this->inner->index.value = prevIndex;
      this->this_->value = prevThis;

      return result;
   }

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   Variable<T>* this_;
   Generator<std::vector<T>>* list;
   Generator<_boo>* condition;
};



template <typename T>
struct Filter_Limit : Generator<std::vector<T>>
{
public:
   Filter_Limit(Generator<std::vector<T>>* li, Generator<_num>* num) :
      list(li), number(num) { };

   ~Filter_Limit() {
      delete list;
      delete number;
   }

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();
      if (n <= 0) {
         return std::vector<T>();
      }

      const std::vector<T> lst = list->getValue();

      return n >= lst.size()
         ? lst
         : std::vector<T>(lst.begin(), lst.begin() + n);
   }

private:
   Generator<std::vector<T>>* list;
   Generator<_num>* number;
};


template <typename T>
struct Filter_Skip : Generator<std::vector<T>>
{
public:
   Filter_Skip(Generator<std::vector<T>>* li, Generator<_num>* num) :
      list(li), number(num) { };

   ~Filter_Skip() {
      delete list;
      delete number;
   }

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();
      const std::vector<T> lst = list->getValue();

      if (n <= 0) {
         return lst;
      }

      return n >= lst.size()
         ? std::vector<T>()
         : std::vector<T>(lst.begin() + n, lst.end());
   }

private:
   Generator<std::vector<T>>* list;
   Generator<_num>* number;
};


template <typename T>
struct Filter_Every : Generator<std::vector<T>>
{
public:
   Filter_Every(Generator<std::vector<T>>* li, Generator<_num>* num) :
      list(li), number(num) { };

   ~Filter_Every() {
      delete list;
      delete number;
   }

   std::vector<T> getValue() override {
      const _nint n = number->getValue().toInt();
      const std::vector<T> lst = list->getValue();

      if (n <= 1) {
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
   Generator<std::vector<T>>* list;
   Generator<_num>* number;
};


template <typename T>
struct VarReference : Generator<T>
{
public:
   VarReference(Variable<T>* var) : variable(var) { };

   T getValue() override {
      return variable->value;
   }

private:
   Variable<T>* variable;
};



#endif /* GEN_GENERIC_H */
