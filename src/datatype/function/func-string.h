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

#ifndef FUNC_STRING_H
#define FUNC_STRING_H

#include "func-generic.h"


namespace uro::func
{

struct F_After : Func_2<_str, _str>, Generator<_str>
{
public:
   F_After(_genptr<_str>& a1, _genptr<_str>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Before : Func_2<_str, _str>, Generator<_str>
{
public:
   F_Before(_genptr<_str>& a1, _genptr<_str>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Digits : Func_1<_str>, Generator<_str>
{
public:
   F_Digits(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Fill : Func_2<_str, _num>, Generator<_str>
{
public:
   F_Fill(_genptr<_str>& a1, _genptr<_num>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Letters : Func_1<_str>, Generator<_str>
{
public:
   F_Letters(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Lower : Func_1<_str>, Generator<_str>
{
public:
   F_Lower(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Trim : Func_1<_str>, Generator<_str>
{
public:
   F_Trim(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Upper : Func_1<_str>, Generator<_str>
{
public:
   F_Upper(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Repeat : Func_2<_str, _num>, Generator<_str>
{
public:
   F_Repeat(_genptr<_str>& a1, _genptr<_num>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Reverse : Func_1<_str>, Generator<_str>
{
public:
   F_Reverse(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Left : Func_2<_str, _num>, Generator<_str>
{
public:
   F_Left(_genptr<_str>& a1, _genptr<_num>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Right : Func_2<_str, _num>, Generator<_str>
{
public:
   F_Right(_genptr<_str>& a1, _genptr<_num>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Substring_2 : Func_2<_str, _num>, Generator<_str>
{
public:
   F_Substring_2(_genptr<_str>& a1, _genptr<_num>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Substring_3 : Func_3<_str, _num, _num>, Generator<_str>
{
public:
   F_Substring_3(_genptr<_str>& a1, _genptr<_num>& a2, _genptr<_num>& a3)
      : Func_3(a1, a2, a3) { };
   _str getValue() override;
};


struct F_ConcatenateUnit : Func_1<_list>, Generator<_str>
{
public:
   F_ConcatenateUnit(_genptr<_list>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Concatenate : Generator<_str>
{
public:
   F_Concatenate(std::vector<_genptr<_str>>& vals) : length(vals.size()) 
   {
      langutil::transferUniquePtrs(vals, values);
   };

   _str getValue() override;

private:
   std::vector<_genptr<_str>> values;
   const _size length;
};


struct F_FirstDef : Generator<_str>
{
public:
   F_FirstDef(_defptr& def) : definition(std::move(def)) { };

   _str getValue() override;

private:
   _defptr definition;
};


struct F_LastDef : Generator<_str>
{
public:
   F_LastDef(_defptr& def) : definition(std::move(def)) { };
   _str getValue() override;

private:
   _defptr definition;
};


struct F_Replace : Func_3<_str, _str, _str>, Generator<_str>
{
public:
   F_Replace(_genptr<_str>& a1, _genptr<_str>& a2, _genptr<_str>& a3) : Func_3(a1, a2, a3) { };
   _str getValue() override;
};


struct F_String_B : Func_1<_bool>, Generator<_str>
{
public:
   F_String_B(_genptr<_bool>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_String_N : Func_1<_num>, Generator<_str>
{
public:
   F_String_N(_genptr<_num>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_String_T : Func_1<_tim>, Generator<_str>
{
public:
   F_String_T(_genptr<_tim>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_String_P : Func_1<_per>, Generator<_str>
{
public:
   F_String_P(_genptr<_per>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_MonthName : Func_1<_num>, Generator<_str>
{
public:
   F_MonthName(_genptr<_num>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_WeekDayName : Func_1<_num>, Generator<_str>
{
public:
   F_WeekDayName(_genptr<_num>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_AfterDigits : Func_1<_str>, Generator<_str>
{
public:
   F_AfterDigits(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_AfterLetters : Func_1<_str>, Generator<_str>
{
public:
   F_AfterLetters(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_BeforeDigits : Func_1<_str>, Generator<_str>
{
public:
   F_BeforeDigits(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_BeforeLetters : Func_1<_str>, Generator<_str>
{
public:
   F_BeforeLetters(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_RandomChar : Func_1<_str>, Generator<_str>
{
public:
   F_RandomChar(_genptr<_str>& a1, Uroboros& uro)
      : Func_1(a1), math(uro.math) { };

   _str getValue() override;

private:
   Math& math;
};


struct F_Capitalize : Func_1<_str>, Generator<_str>
{
public:
   F_Capitalize(_genptr<_str>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Join : Func_2<_list, _str>, Generator<_str>
{
public:
   F_Join(_genptr<_list>& a1, _genptr<_str>& a2) : Func_2(a1, a2) { };
   _str getValue() override;
};


struct F_Roman : Func_1<_num>, Generator<_str>
{
public:
   F_Roman(_genptr<_num>& a1) : Func_1(a1) { };
   _str getValue() override;

private:
   inline void appendFraction(const _num& base, _stream& ss) const;
};


struct F_Binary : Func_1<_num>, Generator<_str>
{
public:
   F_Binary(_genptr<_num>& a1) : Func_1(a1) { };
   _str getValue() override;
};


struct F_Hex : Func_1<_num>, Generator<_str>
{
public:
   F_Hex(_genptr<_num>& a1) : Func_1(a1) { };
   _str getValue() override;
};

}

#endif /* FUNC_STRING_H */
