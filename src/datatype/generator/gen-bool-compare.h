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

#include "../datatype.h"
#include "../comparison.h"
#include "../../perun2.h"


namespace perun2::gen
{


template <typename T>
struct Comparison : Generator<p_bool>
{
public:
   Comparison<T> (p_genptr<T>& val1, p_genptr<T>& val2)
      : value1(std::move(val1)), value2(std::move(val2)) { };

protected:
   p_genptr<T> value1;
   p_genptr<T> value2;
};


template <typename T>
struct Equals : Comparison<T>
{
public:
   Equals(p_genptr<T>& val1, p_genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue() == this->value2->getValue();
   }
};


template <typename T>
struct NotEquals : Comparison<T>
{
public:
   NotEquals(p_genptr<T>& val1, p_genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue() != this->value2->getValue();
   }
};


template <typename T>
struct Smaller : Comparison<T>
{
public:
   Smaller(p_genptr<T>& val1, p_genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue() < this->value2->getValue();
   }
};


template <typename T>
struct SmallerEquals : Comparison<T>
{
public:
   SmallerEquals(p_genptr<T>& val1, p_genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue() <= this->value2->getValue();
   }
};


template <typename T>
struct Bigger : Comparison<T>
{
public:
   Bigger(p_genptr<T>& val1, p_genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue() > this->value2->getValue();
   }
};


template <typename T>
struct BiggerEquals : Comparison<T>
{
public:
   BiggerEquals(p_genptr<T>& val1, p_genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue() >= this->value2->getValue();
   }
};


// here collections

template <typename T>
struct CollectionsEqual : Comparison<std::vector<T>>
{
public:
   CollectionsEqual(p_genptr<std::vector<T>>& val1,
      p_genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   p_bool getValue() override
   {
      const std::vector<T> v1 = this->value1->getValue();
      const std::vector<T> v2 = this->value2->getValue();
      const p_size s1 = v1.size();
      const p_size s2 = v2.size();

      if (s1 != s2) {
         return false;
      }

      for (p_size i = 0; i < s1; i++) {
         if (v1[i] != v2[i])
            return false;
      }

      return true;
   }
};


template <typename T>
struct CollectionsNotEqual : Comparison<std::vector<T>>
{
public:
   CollectionsNotEqual(p_genptr<std::vector<T>>& val1,
      p_genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   p_bool getValue() override
   {
      const std::vector<T> v1 = this->value1->getValue();
      const std::vector<T> v2 = this->value2->getValue();
      const p_size s1 = v1.size();
      const p_size s2 = v2.size();

      if (s1 != s2) {
         return true;
      }

      for (p_size i = 0; i < s1; i++) {
         if (v1[i] != v2[i])
            return true;
      }

      return false;
   }
};


template <typename T>
struct CollectionsSmaller : Comparison<std::vector<T>>
{
public:
   CollectionsSmaller(p_genptr<std::vector<T>>& val1,
      p_genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue().size() < this->value2->getValue().size();
   }
};


template <typename T>
struct CollectionsSmallerEquals : Comparison<std::vector<T>>
{
public:
   CollectionsSmallerEquals(p_genptr<std::vector<T>>& val1,
      p_genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   p_bool getValue() override
   {
      const p_size s1 = this->value1->getValue().size();

      return s1 == 0
         ? true
         : (s1 <= this->value2->getValue().size());
   }
};


template <typename T>
struct CollectionsBigger : Comparison<std::vector<T>>
{
public:
   CollectionsBigger(p_genptr<std::vector<T>>& val1,
      p_genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   p_bool getValue() override
   {
      return this->value1->getValue().size() > this->value2->getValue().size();
   }
};


template <typename T>
struct CollectionsBiggerEquals : Comparison<std::vector<T>>
{
public:
   CollectionsBiggerEquals(p_genptr<std::vector<T>>& val1,
      p_genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   p_bool getValue() override
   {
      const p_size s2 = this->value2->getValue().size();

      return s2 == 0
         ? true
         : (this->value1->getValue().size() >= s2);
   }
};


// collection - value


template <typename T>
struct CollectionValueEquals : Generator<p_bool>
{
public:
   CollectionValueEquals(p_genptr<std::vector<T>>& col, p_genptr<T>& val)
      : collection(std::move(col)), value(std::move(val)) { };

   p_bool getValue() override
   {
      const std::vector<T> c = collection->getValue();
      if (c.size() != 1) {
         return false;
      }

      return value->getValue() == c[0];
   }

private:
   p_genptr<std::vector<T>> collection;
   p_genptr<T> value;
};


template <typename T>
struct CollectionValueNotEquals : Generator<p_bool>
{
public:
   CollectionValueNotEquals(p_genptr<std::vector<T>>& col, p_genptr<T>& val)
      : collection(std::move(col)), value(std::move(val)) { };


   p_bool getValue() override
   {
      const std::vector<T> c = collection->getValue();
      if (c.size() != 1) {
         return true;
      }

      return value->getValue() != c[0];
   }

private:
   p_genptr<std::vector<T>> collection;
   p_genptr<T> value;
};


template <typename T>
struct CollectionValueSmaller: Generator<p_bool>
{
public:
   CollectionValueSmaller(p_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   p_bool getValue() override
   {
      return collection->getValue().size() < 1;
   }

private:
   p_genptr<std::vector<T>> collection;
};


template <typename T>
struct CollectionValueSmallerEquals: Generator<p_bool>
{
public:
   CollectionValueSmallerEquals(p_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   p_bool getValue() override
   {
      return collection->getValue().size() <= 1;
   }

private:
   p_genptr<std::vector<T>> collection;
};


template <typename T>
struct CollectionValueBigger: Generator<p_bool>
{
public:
   CollectionValueBigger(p_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   p_bool getValue() override
   {
      return collection->getValue().size() > 1;
   }

private:
   p_genptr<std::vector<T>> collection;
};


template <typename T>
struct CollectionValueBiggerEquals: Generator<p_bool>
{
public:
   CollectionValueBiggerEquals(p_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   p_bool getValue() override
   {
      return collection->getValue().size() >= 1;
   }

private:
   p_genptr<std::vector<T>> collection;
};

// definition + definition

struct DefComparison : Generator<p_bool>
{
public:
   DefComparison(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : value1(std::move(val1)), value2(std::move(val2)), perun2(p2) { };

protected:
   Perun2Process& perun2;
   p_defptr value1;
   p_defptr value2;
};


struct DefinitionsEqual : DefComparison
{
public:
   DefinitionsEqual(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : DefComparison(val1, val2, p2) { };

   p_bool getValue() override;
};


struct DefinitionsNotEqual : DefComparison
{
public:
   DefinitionsNotEqual(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : DefComparison(val1, val2, p2) { };

   p_bool getValue() override;
};


struct DefinitionsSmaller : DefComparison
{
public:
   DefinitionsSmaller(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : DefComparison(val1, val2, p2) { };

   p_bool getValue() override;
};


struct DefinitionsSmallerEquals : DefComparison
{
public:
   DefinitionsSmallerEquals(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : DefComparison(val1, val2, p2) { };

   p_bool getValue() override;
};


struct DefinitionsBigger : DefComparison
{
public:
   DefinitionsBigger(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : DefComparison(val1, val2, p2) { };

   p_bool getValue() override;
};


struct DefinitionsBiggerEquals : DefComparison
{
public:
   DefinitionsBiggerEquals(p_defptr& val1, p_defptr& val2, Perun2Process& p2)
      : DefComparison(val1, val2, p2) { };

   p_bool getValue() override;
};


// definition - collection


struct DefListComparison : Generator<p_bool>
{
public:
   DefListComparison(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : definition(std::move(def)), list(std::move(li)), perun2(p2) { };

protected:
   Perun2Process& perun2;
   p_defptr definition;
   p_genptr<p_list> list;
};


struct DefinitionListEqual : DefListComparison
{
public:
   DefinitionListEqual(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : DefListComparison(def, li, p2) { };

   p_bool getValue() override;
};


struct DefinitionListNotEqual : DefListComparison
{
public:
   DefinitionListNotEqual(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : DefListComparison(def, li, p2) { };

   p_bool getValue() override;
};


struct DefinitionListSmaller : DefListComparison
{
public:
   DefinitionListSmaller(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : DefListComparison(def, li, p2) { };

   p_bool getValue() override;
};


struct DefinitionListSmallerEquals : DefListComparison
{
public:
   DefinitionListSmallerEquals(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : DefListComparison(def, li, p2) { };

   p_bool getValue() override;
};


struct DefinitionListBigger : DefListComparison
{
public:
   DefinitionListBigger(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : DefListComparison(def, li, p2) { };

   p_bool getValue() override;
};


struct DefinitionListBiggerEquals : DefListComparison
{
public:
   DefinitionListBiggerEquals(p_defptr& def, p_genptr<p_list>& li, Perun2Process& p2)
      : DefListComparison(def, li, p2) { };

   p_bool getValue() override;
};

}
