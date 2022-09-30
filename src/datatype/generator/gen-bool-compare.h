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

#ifndef GEN_BOOL_COMPARE_H
#define GEN_BOOL_COMPARE_H

#include "../generator.h"
#include "../datatype.h"
#include "../../uroboros.h"


namespace uro::gen
{

enum CompType
{
   ct_Equals = 0,
   ct_NotEquals,
   ct_Smaller,
   ct_SmallerEquals,
   ct_Bigger,
   ct_BiggerEquals
};



template <typename T>
struct Comparison : Generator<_bool>
{
public:
   Comparison<T> (Generator<T>* val1, Generator<T>* val2)
      : value1(val1), value2(val2) { };

protected:
   ~Comparison<T>() {
      delete value1;
      delete value2;
   };

   Generator<T>* value1;
   Generator<T>* value2;
};


template <typename T>
struct Equals : Comparison<T>
{
   Equals(Generator<T>* val1, Generator<T>* val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue() == this->value2->getValue();
   }
};


template <typename T>
struct NotEquals : Comparison<T>
{
   NotEquals(Generator<T>* val1, Generator<T>* val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue() != this->value2->getValue();
   }
};


template <typename T>
struct Smaller : Comparison<T>
{
   Smaller(Generator<T>* val1, Generator<T>* val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue() < this->value2->getValue();
   }
};


template <typename T>
struct SmallerEquals : Comparison<T>
{
   SmallerEquals(Generator<T>* val1, Generator<T>* val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue() <= this->value2->getValue();
   }
};


template <typename T>
struct Bigger : Comparison<T>
{
   Bigger(Generator<T>* val1, Generator<T>* val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue() > this->value2->getValue();
   }
};


template <typename T>
struct BiggerEquals : Comparison<T>
{
   BiggerEquals(Generator<T>* val1, Generator<T>* val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue() >= this->value2->getValue();
   }
};


// here collections

template <typename T>
struct CollectionsEqual : Comparison<std::vector<T>>
{
   CollectionsEqual(Generator<std::vector<T>>* val1,
      Generator<std::vector<T>>* val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override {
      const std::vector<T> v1 = this->value1->getValue();
      const std::vector<T> v2 = this->value2->getValue();
      const _size s1 = v1.size();
      const _size s2 = v2.size();

      if (s1 != s2) {
         return false;
      }

      for (_size i = 0; i < s1; i++) {
         if (v1[i] != v2[i])
            return false;
      }

      return true;
   }
};


template <typename T>
struct CollectionsNotEqual : Comparison<std::vector<T>>
{
   CollectionsNotEqual(Generator<std::vector<T>>* val1,
      Generator<std::vector<T>>* val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override {
      const std::vector<T> v1 = this->value1->getValue();
      const std::vector<T> v2 = this->value2->getValue();
      const _size s1 = v1.size();
      const _size s2 = v2.size();

      if (s1 != s2) {
         return true;
      }

      for (_size i = 0; i < s1; i++) {
         if (v1[i] != v2[i])
            return true;
      }

      return false;
   }
};


template <typename T>
struct CollectionsSmaller : Comparison<std::vector<T>>
{
   CollectionsSmaller(Generator<std::vector<T>>* val1,
      Generator<std::vector<T>>* val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue().size() < this->value2->getValue().size();
   }
};


template <typename T>
struct CollectionsSmallerEquals : Comparison<std::vector<T>>
{
   CollectionsSmallerEquals(Generator<std::vector<T>>* val1,
      Generator<std::vector<T>>* val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override {
      const _size s1 = this->value1->getValue().size();

      return s1 == 0
         ? true
         : (s1 <= this->value2->getValue().size());
   }
};


template <typename T>
struct CollectionsBigger : Comparison<std::vector<T>>
{
   CollectionsBigger(Generator<std::vector<T>>* val1,
      Generator<std::vector<T>>* val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override {
      return this->value1->getValue().size() > this->value2->getValue().size();
   }
};


template <typename T>
struct CollectionsBiggerEquals : Comparison<std::vector<T>>
{
   CollectionsBiggerEquals(Generator<std::vector<T>>* val1,
      Generator<std::vector<T>>* val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override {
      const _size s2 = this->value2->getValue().size();

      return s2 == 0
         ? true
         : (this->value1->getValue().size() >= s2);
   }
};


// collection - value


template <typename T>
struct CollectionValueEquals : Generator<_bool>
{
public:
   CollectionValueEquals(Generator<std::vector<T>>* col,
      Generator<T>* val)
      : collection(col), value(val) { };

   ~CollectionValueEquals() {
      delete collection;
      delete value;
   }

   _bool getValue() override {
      const std::vector<T> c = collection->getValue();
      if (c.size() != 1) {
         return false;
      }

      return value->getValue() == c[0];
   }

private:
   Generator<std::vector<T>>* collection;
   Generator<T>* value;
};


template <typename T>
struct CollectionValueNotEquals : Generator<_bool>
{
public:
   CollectionValueNotEquals(Generator<std::vector<T>>* col,
      Generator<T>* val)
      : collection(col), value(val) { };

   ~CollectionValueNotEquals() {
      delete collection;
      delete value;
   }

   _bool getValue() override {
      const std::vector<T> c = collection->getValue();
      if (c.size() != 1) {
         return true;
      }

      return value->getValue() != c[0];
   }

private:
   Generator<std::vector<T>>* collection;
   Generator<T>* value;
};


template <typename T>
struct CollectionValueSmaller: Generator<_bool>
{
public:
   CollectionValueSmaller(Generator<std::vector<T>>* col)
      : collection(col) { };

   ~CollectionValueSmaller() {
      delete collection;
   }

   _bool getValue() override {
      return collection->getValue().size() < 1;
   }

private:
   Generator<std::vector<T>>* collection;
};


template <typename T>
struct CollectionValueSmallerEquals: Generator<_bool>
{
public:
   CollectionValueSmallerEquals(Generator<std::vector<T>>* col)
      : collection(col) { };

   ~CollectionValueSmallerEquals() {
      delete collection;
   }

   _bool getValue() override {
      return collection->getValue().size() <= 1;
   }

private:
   Generator<std::vector<T>>* collection;
};


template <typename T>
struct CollectionValueBigger: Generator<_bool>
{
public:
   CollectionValueBigger(Generator<std::vector<T>>* col)
      : collection(col) { };

   ~CollectionValueBigger() {
      delete collection;
   }

   _bool getValue() override {
      return collection->getValue().size() > 1;
   }

private:
   Generator<std::vector<T>>* collection;
};


template <typename T>
struct CollectionValueBiggerEquals: Generator<_bool>
{
public:
   CollectionValueBiggerEquals(Generator<std::vector<T>>* col)
      : collection(col) { };

   ~CollectionValueBiggerEquals() {
      delete collection;
   }

   _bool getValue() override {
      return collection->getValue().size() >= 1;
   }

private:
   Generator<std::vector<T>>* collection;
};

// definition + definition

struct DefComparison : Generator<_bool>
{
public:
   DefComparison(_def* val1, _def* val2, uro::Uroboros* uro)
      : value1(val1), value2(val2), uroboros(uro) { };

protected:
   ~DefComparison();
   uro::Uroboros* uroboros;
   _def* value1;
   _def* value2;
};


struct DefinitionsEqual : DefComparison
{
   DefinitionsEqual(_def* val1, _def* val2, uro::Uroboros* uro)
      : DefComparison(val1, val2, uro) { };

   _bool getValue() override;
};


struct DefinitionsNotEqual : DefComparison
{
   DefinitionsNotEqual(_def* val1, _def* val2, uro::Uroboros* uro)
      : DefComparison(val1, val2, uro) { };

   _bool getValue() override;
};


struct DefinitionsSmaller : DefComparison
{
   DefinitionsSmaller(_def* val1, _def* val2, uro::Uroboros* uro)
      : DefComparison(val1, val2, uro) { };

   _bool getValue() override;
};


struct DefinitionsSmallerEquals : DefComparison
{
   DefinitionsSmallerEquals(_def* val1, _def* val2, uro::Uroboros* uro)
      : DefComparison(val1, val2, uro) { };

   _bool getValue() override;
};


struct DefinitionsBigger : DefComparison
{
   DefinitionsBigger(_def* val1, _def* val2, uro::Uroboros* uro)
      : DefComparison(val1, val2, uro) { };

   _bool getValue() override;
};


struct DefinitionsBiggerEquals : DefComparison
{
   DefinitionsBiggerEquals(_def* val1, _def* val2, uro::Uroboros* uro)
      : DefComparison(val1, val2, uro) { };

   _bool getValue() override;
};


// definition - collection


struct DefListComparison : Generator<_bool>
{
public:
   DefListComparison(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : definition(def), list(li), uroboros(uro) { };

protected:
   ~DefListComparison();
   uro::Uroboros* uroboros;
   _def* definition;
   Generator<_list>* list;
};


struct DefinitionListEqual : DefListComparison
{
   DefinitionListEqual(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : DefListComparison(def, li, uro) { };

   _bool getValue() override;
};


struct DefinitionListNotEqual : DefListComparison
{
   DefinitionListNotEqual(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : DefListComparison(def, li, uro) { };

   _bool getValue() override;
};


struct DefinitionListSmaller : DefListComparison
{
   DefinitionListSmaller(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : DefListComparison(def, li, uro) { };

   _bool getValue() override;
};


struct DefinitionListSmallerEquals : DefListComparison
{
   DefinitionListSmallerEquals(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : DefListComparison(def, li, uro) { };

   _bool getValue() override;
};


struct DefinitionListBigger : DefListComparison
{
   DefinitionListBigger(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : DefListComparison(def, li, uro) { };

   _bool getValue() override;
};


struct DefinitionListBiggerEquals : DefListComparison
{
   DefinitionListBiggerEquals(_def* def, Generator<_list>* li, uro::Uroboros* uro)
      : DefListComparison(def, li, uro) { };

   _bool getValue() override;
};

}


#endif /* GEN_BOOL_COMPARE_H */
