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

#ifndef GEN_LIST_H
#define GEN_LIST_H

#include "../generator.h"
#include "../order.h"
#include "../../var/var-runtime.h"
#include "../../attribute.h"
#include "../../uroboros.h"
#include "../../os.h"



struct Filter_WhereString : Generator<_list>
{
public:
   Filter_WhereString(Generator<_list>* li, Generator<_boo>* cond, Attribute attr, Uroboros* uro) 
      : list(li), condition(cond), attribute(attr), uroboros(uro), inner(&uro->vars.inner) {};

   ~Filter_WhereString();

   _list getValue() override;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   Generator<_list>* list;
   Generator<_boo>* condition;
   const Attribute attribute;
};


struct Filter_OrderByString : Generator<_list>, OrderBy<_str>
{
public:
   Filter_OrderByString(Generator<_list>* val, Attribute* attr, Uroboros* uro)
      : OrderBy(val, uro), attribute(attr) {};


   _list getValue() override;
   _boo isSmallerOrEquals(const _numi& leftId, const _numi& rightId,
      const _str& left, const _str& right) const override;


   template <typename T>
   _boo finalComparison(const OrderUnit<T>* ou, const _numi& leftId,
      const _numi& rightId, const _str& left, const _str& right) const {

      this->inner->index.value = leftId;
      this->inner->this_s.value = left;
      this->attribute->run();
      const T leftValue = ou->value->getValue();

      this->inner->index.value = rightId;
      this->inner->this_s.value = right;
      this->attribute->run();
      const T rightValue = ou->value->getValue();

      return ou->descending
         ? leftValue >= rightValue
         : leftValue <= rightValue;
   }

   template <typename T>
   _boo usualComparison(const OrderUnit<T>* ou, const _numi& leftId,
      const _numi& rightId, const _str& left, const _str& right,
      _boo& success) const {

      this->inner->index.value = leftId;
      this->inner->this_s.value = left;
      this->attribute->run();
      const T leftValue = ou->value->getValue();

      this->inner->index.value = rightId;
      this->inner->this_s.value = right;
      this->attribute->run();
      const T rightValue = ou->value->getValue();

      if (leftValue == rightValue) {
         return false;
      }

      success = true;

      return ou->descending
         ? leftValue > rightValue
         : leftValue < rightValue;
   }

private:

   Attribute* attribute;
};


struct Join_StrStr : Generator<_list>
{

public:
   Join_StrStr(Generator<_str>* lef, Generator<_str>* rig)
        : left(lef), right(rig) {};
   ~Join_StrStr();

   _list getValue() override;

private:
   Generator<_str>* left;
   Generator<_str>* right;
};


struct Join_StrList : Generator<_list>
{

public:
   Join_StrList(Generator<_str>* lef, Generator<_list>* rig)
        : left(lef), right(rig) {};
   ~Join_StrList();

   _list getValue() override;

private:
   Generator<_str>* left;
   Generator<_list>* right;
};


struct Join_ListStr : Generator<_list>
{

public:
   Join_ListStr(Generator<_list>* lef, Generator<_str>* rig)
        : left(lef), right(rig) {};
   ~Join_ListStr();

   _list getValue() override;

private:
   Generator<_list>* left;
   Generator<_str>* right;
};


struct Join_ListList : Generator<_list>
{

public:
   Join_ListList(Generator<_list>* lef, Generator<_list>* rig)
        : left(lef), right(rig) {};
   ~Join_ListList();

   _list getValue() override;

private:
   Generator<_list>* left;
   Generator<_list>* right;
};


#endif /* GEN_LIST_H */

