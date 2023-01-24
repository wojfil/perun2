/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_GENERIC_FILTER_H_INCLUDED
#define GEN_GENERIC_FILTER_H_INCLUDED

#include "../../uroboros.h"


namespace uro::gen
{

template <typename T>
struct Filter_Where : Generator<std::vector<T>>
{
public:
   Filter_Where(_genptr<std::vector<T>>& li, _genptr<_bool>& cond, _attrptr& attr, _uro& uro)
      : list(std::move(li)), condition(std::move(cond)), uroboros(uro), inner(uro.vars.inner),
        this_(nullptr), attribute(std::move(attr)), hasAttribute(true)
   {
      uro.vars.inner.createThisVarRef(this_);
   };

   Filter_Where(_genptr<std::vector<T>>& li, _genptr<_bool>& cond, _uro& uro)
      : list(std::move(li)), condition(std::move(cond)), uroboros(uro), inner(uro.vars.inner),
        this_(nullptr), hasAttribute(false)
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
      _numi index(NINT_ZERO);

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
   _uro& uroboros;
   InnerVariables& inner;
   vars::Variable<T>* this_;
   _genptr<std::vector<T>> list;
   _genptr<_bool> condition;
   _attrptr attribute;
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

      if (n <= NINT_ZERO) {
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

      if (n <= NINT_ZERO) {
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

      if (n <= NINT_ONE) {
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

      if (n <= NINT_ZERO) {
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

}

#endif // GEN_GENERIC_FILTER_H_INCLUDED
