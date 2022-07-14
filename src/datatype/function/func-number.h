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

#ifndef FUNC_NUMBER_H
#define FUNC_NUMBER_H

#include "func-generic.h"
#include "../../uroboros.h"


struct F_Absolute : Func_1<_num>, Generator<_num>
{
public:
   F_Absolute(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Ceil : Func_1<_num>, Generator<_num>
{
public:
   F_Ceil(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


template <typename T>
struct F_Count : Func_1<std::vector<T>>, Generator<_num>
{
public:
   F_Count<T>(Generator<std::vector<T>>* a1) : Func_1<std::vector<T>>(a1) { };
   _num getValue() override {
      return _num((_nint) this->arg1->getValue().size());
   }
};


struct F_CountDef : Generator<_num>
{
public:
   F_CountDef(_def* def, Uroboros* uro)
      : definition(def), uroboros(uro) { };

   ~F_CountDef() {
      delete definition;
   }

   _num getValue() override;

private:
   Uroboros* uroboros;
   _def* definition;
};


struct F_CountInside : Generator<_num>
{
public:
   F_CountInside(_def* def, Generator<_str>* val, Uroboros* uro)
      : definition(def), value(val), uroboros(uro), inner(&uro->vars.inner) { };

   ~F_CountInside() {
      delete definition;
      delete value;
   }

   _num getValue() override;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   _def* definition;
   Generator<_str>* value;
};


struct F_Floor : Func_1<_num>, Generator<_num>
{
public:
   F_Floor(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Length : Func_1<_str>, Generator<_num>
{
public:
   F_Length(Generator<_str>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Number : Func_1<_str>, Generator<_num>
{
public:
   F_Number(Generator<_str>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Power : Func_2<_num, _num>, Generator<_num>
{
public:
   F_Power(Generator<_num>* a1, Generator<_num>* a2) : Func_2(a1, a2) { };
   _num getValue() override;

private:
   _num doublePower(const _ndouble& base, const _ndouble& exp);
};

struct F_Round : Func_1<_num>, Generator<_num>
{
public:
   F_Round(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Sign : Func_1<_num>, Generator<_num>
{
public:
   F_Sign(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Sqrt : Func_1<_num>, Generator<_num>
{
public:
   F_Sqrt(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Truncate : Func_1<_num>, Generator<_num>
{
public:
   F_Truncate(Generator<_num>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_Random : Generator<_num>
{
public:
   F_Random(Uroboros* uro)
      : math(&uro->math) { };

   _num getValue() override;

private:
   Math* math;
};


struct F_RandomNumber : Func_1<_num>, Generator<_num>
{
public:
   F_RandomNumber(Generator<_num>* a1, Uroboros* uro)
      : Func_1(a1), math(&uro->math) { };

   _num getValue() override;

private:
   Math* math;
};


struct F_FromBinary : Func_1<_str>, Generator<_num>
{
public:
   F_FromBinary(Generator<_str>* a1) : Func_1(a1) { };
   _num getValue() override;
};


struct F_FromHex : Func_1<_str>, Generator<_num>
{
public:
   F_FromHex(Generator<_str>* a1) : Func_1(a1) { };
   _num getValue() override;
};


#endif /* FUNC_NUMBER_H */

