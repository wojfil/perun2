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

#ifndef FUNC_BOOL_H_INCLUDED
#define FUNC_BOOL_H_INCLUDED

#include "func-generic.h"
#include <wctype.h>


namespace uro::func
{

template <typename T>
struct F_Any : Func_1<std::vector<T>>, Generator<_boo>
{
public:
   F_Any<T>(Generator<std::vector<T>>* a1) : Func_1<std::vector<T>>(a1) { };
   _boo getValue() override {
      return this->arg1->getValue().size() > 0;
   }
};


struct F_AnyDef : Generator<_boo>
{
public:
   F_AnyDef(_def* def) : definition(def) { };
   ~F_AnyDef() {
      delete definition;
   }

   _boo getValue() override;

private:
   _def* definition;
};


struct F_AnyInside : Generator<_boo>
{
public:
   F_AnyInside(_def* def, Generator<_str>* val, Uroboros* uro)
      : definition(def), value(val), inner(&uro->vars.inner) { };

   ~F_AnyInside() {
      delete definition;
      delete value;
   }

   _boo getValue() override;

private:
   InnerVariables* inner;
   _def* definition;
   Generator<_str>* value;
};


template <typename T>
struct F_ContainsCol : Func_2<std::vector<T>, T>, Generator<_boo>
{
public:
   F_ContainsCol<T>(Generator<std::vector<T>>* a1, Generator<T>* a2)
      : Func_2<std::vector<T>, T>(a1, a2) { };

   _boo getValue() override {
      const std::vector<T> col = this->arg1->getValue();
      const _size length = col.size();

      if (length == 0) {
         return false;
      }

      const T v = this->arg2->getValue();

      switch (length) {
         case 1: {
            return v == col[0];
         }
         case 2: {
            return v == col[0] || v == col[1];
         }
         case 3: {
            return v == col[0] || v == col[1] || v == col[2];
         }
      }

      for (_size i = 0; i < length; i++) {
         if (v == col[i]) {
            return true;
         }
      }

      return false;
   }
};


struct F_ContainsStr : Func_2<_str, _str>, Generator<_boo>
{
public:
   F_ContainsStr(Generator<_str>* a1, Generator<_str>* a2)
      : Func_2<_str, _str>(a1, a2) { };

   _boo getValue() override;
};


struct F_ContainsDef : Generator<_boo>
{
public:
   F_ContainsDef(_def* def, Generator<_str>* val, Uroboros* uro)
      : definition(def), value(val), uroboros(uro) { };

   ~F_ContainsDef() {
      delete definition;
      delete value;
   }

   _boo getValue() override;

private:
   Uroboros* uroboros;
   _def* definition;
   Generator<_str>* value;
};


struct F_IsLower : Func_1<_str>, Generator<_boo>
{
public:
   F_IsLower(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};


struct F_IsUpper : Func_1<_str>, Generator<_boo>
{
public:
   F_IsUpper(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};


struct F_IsNumber : Func_1<_str>, Generator<_boo>
{
public:
   F_IsNumber(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};


_boo isNumber(const _str& value);


struct F_StartsWith : Func_2<_str, _str>, Generator<_boo>
{
public:
   F_StartsWith(Generator<_str>* a1, Generator<_str>* a2) : Func_2(a1, a2) { };
   _boo getValue() override;
};


struct F_EndsWith : Func_2<_str, _str>, Generator<_boo>
{
public:
   F_EndsWith(Generator<_str>* a1, Generator<_str>* a2) : Func_2(a1, a2) { };
   _boo getValue() override;
};


struct F_ExistsInside : Func_2<_str, _str>, Generator<_boo>
{
public:
   F_ExistsInside(Generator<_str>* a1, Generator<_str>* a2, Uroboros* uro)
      : Func_2(a1, a2), inner(&uro->vars.inner) { };
   _boo getValue() override;

private:
   InnerVariables* inner;
};


struct F_ExistInside : Func_2<_list, _str>, Generator<_boo>
{
public:
   F_ExistInside(Generator<_list>* a1, Generator<_str>* a2, Uroboros* uro)
      : Func_2(a1, a2), inner(&uro->vars.inner) { };
   _boo getValue() override;

private:
   InnerVariables* inner;
};


struct F_StartsWithChar : Generator<_boo>
{
public:
   F_StartsWithChar(Generator<_str>* val, const _char& ch)
      : value(val), character(ch) { };
   ~F_StartsWithChar();

   _boo getValue() override;

private:
   Generator<_str>* value;
   const _char character;
};


struct F_EndsWithChar : Generator<_boo>
{
public:
   F_EndsWithChar(Generator<_str>* val, const _char& ch)
      : value(val), character(ch) { };
   ~F_EndsWithChar();

   _boo getValue() override;

private:
   Generator<_str>* value;
   const _char character;
};


struct F_StartsWithConst : Generator<_boo>
{
public:
   F_StartsWithConst(Generator<_str>* val, const _str& cnst)
      : value(val), constant(cnst), constantLength(cnst.size()) { };
   ~F_StartsWithConst();

   _boo getValue() override;

private:
   Generator<_str>* value;
   const _str constant;
   const _size constantLength;
};


struct F_EndsWithConst : Generator<_boo>
{
public:
   F_EndsWithConst(Generator<_str>* val, const _str& cnst)
      : value(val), constant(cnst), constantLength(cnst.size()) { };
   ~F_EndsWithConst();

   _boo getValue() override;

private:
   Generator<_str>* value;
   const _str constant;
   const _size constantLength;
};


struct F_IsLetter : Func_1<_str>, Generator<_boo>
{
public:
   F_IsLetter(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};


struct F_IsDigit : Func_1<_str>, Generator<_boo>
{
public:
   F_IsDigit(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};


struct F_IsBinary : Func_1<_str>, Generator<_boo>
{
public:
   F_IsBinary(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};


struct F_IsHex : Func_1<_str>, Generator<_boo>
{
public:
   F_IsHex(Generator<_str>* a1) : Func_1(a1) { };
   _boo getValue() override;
};

}

#endif // FUNC_BOOL_H_INCLUDED
