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

#ifndef FUNC_BOOL_H_INCLUDED
#define FUNC_BOOL_H_INCLUDED

#include "func-generic.h"
#include <wctype.h>


namespace perun2::func
{


struct F_AnyInside : Generator<_bool>
{
public:
   F_AnyInside() = delete;
   F_AnyInside(_defptr& def, _lcptr& lctx, FileContext* fctx)
      : definition(std::move(def)), locContext(std::move(lctx)), fileContext(fctx) { };

   _bool getValue() override;

private:
   _lcptr locContext;
   FileContext* fileContext;
   _defptr definition;
};


template <typename T>
struct F_ContainsCol : Func_2<std::vector<T>, T>, Generator<_bool>
{
public:
   F_ContainsCol<T>(_genptr<std::vector<T>>& a1, _genptr<T>& a2)
      : Func_2<std::vector<T>, T>(a1, a2) { };

   _bool getValue() override {
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


struct F_ContainsStr : Func_2<_str, _str>, Generator<_bool>
{
public:
   F_ContainsStr(_genptr<_str>& a1, _genptr<_str>& a2)
      : Func_2<_str, _str>(a1, a2) { };

   _bool getValue() override;
};


struct F_ContainsDef : Generator<_bool>
{
public:
   F_ContainsDef(_defptr& def, _genptr<_str>& val, _p2& p2)
      : definition(std::move(def)), value(std::move(val)), perun2(p2) { };

   _bool getValue() override;

private:
   _p2& perun2;
   _defptr definition;
   _genptr<_str> value;
};


struct F_IsLower : Func_1<_str>, Generator<_bool>
{
public:
   F_IsLower(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


struct F_IsUpper : Func_1<_str>, Generator<_bool>
{
public:
   F_IsUpper(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


struct F_IsNumber : Func_1<_str>, Generator<_bool>
{
public:
   F_IsNumber(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


_bool isNumber(const _str& value);


struct F_StartsWith : Func_2<_str, _str>, Generator<_bool>
{
public:
   F_StartsWith(_genptr<_str>& a1, _genptr<_str>& a2) : Func_2(a1, a2) { };
   _bool getValue() override;
};


struct F_EndsWith : Func_2<_str, _str>, Generator<_bool>
{
public:
   F_EndsWith(_genptr<_str>& a1, _genptr<_str>& a2) : Func_2(a1, a2) { };
   _bool getValue() override;
};


struct F_ExistsInside : Func_1<_str>, Generator<_bool>
{
public:
   F_ExistsInside(_genptr<_str>& a1, LocationContext* lctx, FileContext* fctx)
      : Func_1(a1), locContext(lctx), fileContext(fctx) { };
   _bool getValue() override;

private:
   LocationContext* locContext;
   FileContext* fileContext;
};


struct F_ExistInside: Func_1<_list>, Generator<_bool>
{
public:
   F_ExistInside(_genptr<_list>& a1, LocationContext* lctx, FileContext* fctx)
      : Func_1(a1), locContext(lctx), fileContext(fctx) { };
   _bool getValue() override;

private:
   LocationContext* locContext;
   FileContext* fileContext;
};


struct F_StartsWithChar : Generator<_bool>
{
public:
   F_StartsWithChar(_genptr<_str>& val, const _char ch)
      : value(std::move(val)), character(ch) { };

   _bool getValue() override;

private:
   _genptr<_str> value;
   const _char character;
};


struct F_EndsWithChar : Generator<_bool>
{
public:
   F_EndsWithChar(_genptr<_str>& val, const _char ch)
      : value(std::move(val)), character(ch) { };

   _bool getValue() override;

private:
   _genptr<_str> value;
   const _char character;
};


struct F_StartsWithConst : Generator<_bool>
{
public:
   F_StartsWithConst(_genptr<_str>& val, const _str& cnst)
      : value(std::move(val)), constant(cnst), constantLength(cnst.size()) { };

   _bool getValue() override;

private:
   _genptr<_str> value;
   const _str constant;
   const _size constantLength;
};


struct F_EndsWithConst : Generator<_bool>
{
public:
   F_EndsWithConst(_genptr<_str>& val, const _str& cnst)
      : value(std::move(val)), constant(cnst), constantLength(cnst.size()) { };

   _bool getValue() override;

private:
   _genptr<_str> value;
   const _str constant;
   const _size constantLength;
};



struct F_FindText : Func_1<_str>, Generator<_bool>
{
public:
   F_FindText(_genptr<_str>& a1,  FileContext* ctx)
      : Func_1(a1), context(ctx) { };
   _bool getValue() override;

private:
   FileContext* context;
};



struct F_IsLetter : Func_1<_str>, Generator<_bool>
{
public:
   F_IsLetter(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


struct F_IsDigit : Func_1<_str>, Generator<_bool>
{
public:
   F_IsDigit(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


struct F_IsBinary : Func_1<_str>, Generator<_bool>
{
public:
   F_IsBinary(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


struct F_IsHex : Func_1<_str>, Generator<_bool>
{
public:
   F_IsHex(_genptr<_str>& a1) : Func_1(a1) { };
   _bool getValue() override;
};


struct F_Any : Generator<_bool>
{
public:
   F_Any() = delete;
   F_Any(_defptr& def) 
      : definition(std::move(def)) { };

   _bool getValue() override;

private:
   _defptr definition;
};


template <typename T>
struct F_AnyList : Generator<_bool>
{
public:
   F_AnyList() = delete;
   F_AnyList(_genptr<std::vector<T>>& val) 
      : value(std::move(val)) { };

   _bool getValue() override
   {
      return this->value->getValue().size() > 0;
   }

private:
   _genptr<std::vector<T>> value;
};


struct F_Exists : Func_1<_str>, Generator<_bool>
{
public:
   F_Exists(_genptr<_str>& a1, _p2& perun2);
   _bool getValue() override;

private:
   LocationContext* context;
};


struct F_Exist: Func_1<_list>, Generator<_bool>
{
public:
   F_Exist(_genptr<_list>& a1, _p2& perun2);
   _bool getValue() override;

private:
   LocationContext* context;
};


}

#endif // FUNC_BOOL_H_INCLUDED
