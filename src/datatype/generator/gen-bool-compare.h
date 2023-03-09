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

#ifndef GEN_BOOL_COMPARE_H
#define GEN_BOOL_COMPARE_H

#include "../datatype.h"
#include "../../perun2.h"


namespace perun2::gen
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
   Comparison<T> (_genptr<T>& val1, _genptr<T>& val2)
      : value1(std::move(val1)), value2(std::move(val2)) { };

protected:
   _genptr<T> value1;
   _genptr<T> value2;
};


template <typename T>
struct Equals : Comparison<T>
{
public:
   Equals(_genptr<T>& val1, _genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue() == this->value2->getValue();
   }
};


template <typename T>
struct NotEquals : Comparison<T>
{
public:
   NotEquals(_genptr<T>& val1, _genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue() != this->value2->getValue();
   }
};


template <typename T>
struct Smaller : Comparison<T>
{
public:
   Smaller(_genptr<T>& val1, _genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue() < this->value2->getValue();
   }
};


template <typename T>
struct SmallerEquals : Comparison<T>
{
public:
   SmallerEquals(_genptr<T>& val1, _genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue() <= this->value2->getValue();
   }
};


template <typename T>
struct Bigger : Comparison<T>
{
public:
   Bigger(_genptr<T>& val1, _genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue() > this->value2->getValue();
   }
};


template <typename T>
struct BiggerEquals : Comparison<T>
{
public:
   BiggerEquals(_genptr<T>& val1, _genptr<T>& val2)
      : Comparison<T>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue() >= this->value2->getValue();
   }
};


// here collections

template <typename T>
struct CollectionsEqual : Comparison<std::vector<T>>
{
public:
   CollectionsEqual(_genptr<std::vector<T>>& val1,
      _genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override
   {
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
public:
   CollectionsNotEqual(_genptr<std::vector<T>>& val1,
      _genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override
   {
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
public:
   CollectionsSmaller(_genptr<std::vector<T>>& val1,
      _genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue().size() < this->value2->getValue().size();
   }
};


template <typename T>
struct CollectionsSmallerEquals : Comparison<std::vector<T>>
{
public:
   CollectionsSmallerEquals(_genptr<std::vector<T>>& val1,
      _genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override
   {
      const _size s1 = this->value1->getValue().size();

      return s1 == 0
         ? true
         : (s1 <= this->value2->getValue().size());
   }
};


template <typename T>
struct CollectionsBigger : Comparison<std::vector<T>>
{
public:
   CollectionsBigger(_genptr<std::vector<T>>& val1,
      _genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override
   {
      return this->value1->getValue().size() > this->value2->getValue().size();
   }
};


template <typename T>
struct CollectionsBiggerEquals : Comparison<std::vector<T>>
{
public:
   CollectionsBiggerEquals(_genptr<std::vector<T>>& val1,
      _genptr<std::vector<T>>& val2)
      : Comparison<std::vector<T>>(val1, val2) { };

   _bool getValue() override
   {
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
   CollectionValueEquals(_genptr<std::vector<T>>& col, _genptr<T>& val)
      : collection(std::move(col)), value(std::move(val)) { };

   _bool getValue() override
   {
      const std::vector<T> c = collection->getValue();
      if (c.size() != 1) {
         return false;
      }

      return value->getValue() == c[0];
   }

private:
   _genptr<std::vector<T>> collection;
   _genptr<T> value;
};


template <typename T>
struct CollectionValueNotEquals : Generator<_bool>
{
public:
   CollectionValueNotEquals(_genptr<std::vector<T>>& col, _genptr<T>& val)
      : collection(std::move(col)), value(std::move(val)) { };


   _bool getValue() override
   {
      const std::vector<T> c = collection->getValue();
      if (c.size() != 1) {
         return true;
      }

      return value->getValue() != c[0];
   }

private:
   _genptr<std::vector<T>> collection;
   _genptr<T> value;
};


template <typename T>
struct CollectionValueSmaller: Generator<_bool>
{
public:
   CollectionValueSmaller(_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   _bool getValue() override
   {
      return collection->getValue().size() < 1;
   }

private:
   _genptr<std::vector<T>> collection;
};


template <typename T>
struct CollectionValueSmallerEquals: Generator<_bool>
{
public:
   CollectionValueSmallerEquals(_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   _bool getValue() override
   {
      return collection->getValue().size() <= 1;
   }

private:
   _genptr<std::vector<T>> collection;
};


template <typename T>
struct CollectionValueBigger: Generator<_bool>
{
public:
   CollectionValueBigger(_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   _bool getValue() override
   {
      return collection->getValue().size() > 1;
   }

private:
   _genptr<std::vector<T>> collection;
};


template <typename T>
struct CollectionValueBiggerEquals: Generator<_bool>
{
public:
   CollectionValueBiggerEquals(_genptr<std::vector<T>>& col)
      : collection(std::move(col)) { };

   _bool getValue() override
   {
      return collection->getValue().size() >= 1;
   }

private:
   _genptr<std::vector<T>> collection;
};

// definition + definition

struct DefComparison : Generator<_bool>
{
public:
   DefComparison(_defptr& val1, _defptr& val2, _p2& p2)
      : value1(std::move(val1)), value2(std::move(val2)), perun2(p2) { };

protected:
   _p2& perun2;
   _defptr value1;
   _defptr value2;
};


struct DefinitionsEqual : DefComparison
{
public:
   DefinitionsEqual(_defptr& val1, _defptr& val2, _p2& p2)
      : DefComparison(val1, val2, p2) { };

   _bool getValue() override;
};


struct DefinitionsNotEqual : DefComparison
{
public:
   DefinitionsNotEqual(_defptr& val1, _defptr& val2, _p2& p2)
      : DefComparison(val1, val2, p2) { };

   _bool getValue() override;
};


struct DefinitionsSmaller : DefComparison
{
public:
   DefinitionsSmaller(_defptr& val1, _defptr& val2, _p2& p2)
      : DefComparison(val1, val2, p2) { };

   _bool getValue() override;
};


struct DefinitionsSmallerEquals : DefComparison
{
public:
   DefinitionsSmallerEquals(_defptr& val1, _defptr& val2, _p2& p2)
      : DefComparison(val1, val2, p2) { };

   _bool getValue() override;
};


struct DefinitionsBigger : DefComparison
{
public:
   DefinitionsBigger(_defptr& val1, _defptr& val2, _p2& p2)
      : DefComparison(val1, val2, p2) { };

   _bool getValue() override;
};


struct DefinitionsBiggerEquals : DefComparison
{
public:
   DefinitionsBiggerEquals(_defptr& val1, _defptr& val2, _p2& p2)
      : DefComparison(val1, val2, p2) { };

   _bool getValue() override;
};


// definition - collection


struct DefListComparison : Generator<_bool>
{
public:
   DefListComparison(_defptr& def, _genptr<_list>& li, _p2& p2)
      : definition(std::move(def)), list(std::move(li)), perun2(p2) { };

protected:
   _p2& perun2;
   _defptr definition;
   _genptr<_list> list;
};


struct DefinitionListEqual : DefListComparison
{
public:
   DefinitionListEqual(_defptr& def, _genptr<_list>& li, _p2& p2)
      : DefListComparison(def, li, p2) { };

   _bool getValue() override;
};


struct DefinitionListNotEqual : DefListComparison
{
public:
   DefinitionListNotEqual(_defptr& def, _genptr<_list>& li, _p2& p2)
      : DefListComparison(def, li, p2) { };

   _bool getValue() override;
};


struct DefinitionListSmaller : DefListComparison
{
public:
   DefinitionListSmaller(_defptr& def, _genptr<_list>& li, _p2& p2)
      : DefListComparison(def, li, p2) { };

   _bool getValue() override;
};


struct DefinitionListSmallerEquals : DefListComparison
{
public:
   DefinitionListSmallerEquals(_defptr& def, _genptr<_list>& li, _p2& p2)
      : DefListComparison(def, li, p2) { };

   _bool getValue() override;
};


struct DefinitionListBigger : DefListComparison
{
public:
   DefinitionListBigger(_defptr& def, _genptr<_list>& li, _p2& p2)
      : DefListComparison(def, li, p2) { };

   _bool getValue() override;
};


struct DefinitionListBiggerEquals : DefListComparison
{
public:
   DefinitionListBiggerEquals(_defptr& def, _genptr<_list>& li, _p2& p2)
      : DefListComparison(def, li, p2) { };

   _bool getValue() override;
};

}


#endif /* GEN_BOOL_COMPARE_H */
