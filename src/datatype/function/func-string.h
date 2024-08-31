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

#include "func-generic.h"


namespace perun2::func
{

struct F_After : Func_2<p_str, p_str>, Generator<p_str>
{
public:
   F_After(p_genptr<p_str>& a1, p_genptr<p_str>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Before : Func_2<p_str, p_str>, Generator<p_str>
{
public:
   F_Before(p_genptr<p_str>& a1, p_genptr<p_str>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Digits : Func_1<p_str>, Generator<p_str>
{
public:
   F_Digits(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Fill : Func_2<p_str, p_num>, Generator<p_str>
{
public:
   F_Fill(p_genptr<p_str>& a1, p_genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Letters : Func_1<p_str>, Generator<p_str>
{
public:
   F_Letters(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Lower : Func_1<p_str>, Generator<p_str>
{
public:
   F_Lower(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Trim : Func_1<p_str>, Generator<p_str>
{
public:
   F_Trim(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Upper : Func_1<p_str>, Generator<p_str>
{
public:
   F_Upper(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Repeat : Func_2<p_str, p_num>, Generator<p_str>
{
public:
   F_Repeat(p_genptr<p_str>& a1, p_genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Reverse : Func_1<p_str>, Generator<p_str>
{
public:
   F_Reverse(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Left : Func_2<p_str, p_num>, Generator<p_str>
{
public:
   F_Left(p_genptr<p_str>& a1, p_genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Right : Func_2<p_str, p_num>, Generator<p_str>
{
public:
   F_Right(p_genptr<p_str>& a1, p_genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Substring_2 : Func_2<p_str, p_num>, Generator<p_str>
{
public:
   F_Substring_2(p_genptr<p_str>& a1, p_genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Substring_3 : Func_3<p_str, p_num, p_num>, Generator<p_str>
{
public:
   F_Substring_3(p_genptr<p_str>& a1, p_genptr<p_num>& a2, p_genptr<p_num>& a3)
      : Func_3(a1, a2, a3) { };
   p_str getValue() override;
};


struct F_ConcatenateUnit : Func_1<p_list>, Generator<p_str>
{
public:
   F_ConcatenateUnit(p_genptr<p_list>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Concatenate : Generator<p_str>
{
public:
   F_Concatenate(std::vector<p_genptr<p_str>>& vals) : length(vals.size()) 
   {
      langutil::transferUniquePtrs(vals, values);
   };

   p_str getValue() override;

private:
   std::vector<p_genptr<p_str>> values;
   const p_size length;
};


struct F_FirstDef : Generator<p_str>
{
public:
   F_FirstDef(p_defptr& def) : definition(std::move(def)) { };

   p_str getValue() override;

private:
   p_defptr definition;
};


struct F_LastDef : Generator<p_str>
{
public:
   F_LastDef(p_defptr& def) : definition(std::move(def)) { };
   p_str getValue() override;

private:
   p_defptr definition;
};


struct F_Replace : Func_3<p_str, p_str, p_str>, Generator<p_str>
{
public:
   F_Replace(p_genptr<p_str>& a1, p_genptr<p_str>& a2, p_genptr<p_str>& a3) : Func_3(a1, a2, a3) { };
   p_str getValue() override;
};


struct F_String_B : Func_1<p_bool>, Generator<p_str>
{
public:
   F_String_B(p_genptr<p_bool>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_String_N : Func_1<p_num>, Generator<p_str>
{
public:
   F_String_N(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_String_T : Func_1<p_tim>, Generator<p_str>
{
public:
   F_String_T(p_genptr<p_tim>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_String_P : Func_1<p_per>, Generator<p_str>
{
public:
   F_String_P(p_genptr<p_per>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_MonthName : Func_1<p_num>, Generator<p_str>
{
public:
   F_MonthName(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_str getValue() override;
};

struct F_MonthNameFromTime : Func_1<p_tim>, Generator<p_str>
{
public:
   F_MonthNameFromTime(p_genptr<p_tim>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_WeekDayName : Func_1<p_num>, Generator<p_str>
{
public:
   F_WeekDayName(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_WeekDayNameFromTime : Func_1<p_tim>, Generator<p_str>
{
public:
   F_WeekDayNameFromTime(p_genptr<p_tim>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_AfterDigits : Func_1<p_str>, Generator<p_str>
{
public:
   F_AfterDigits(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_AfterLetters : Func_1<p_str>, Generator<p_str>
{
public:
   F_AfterLetters(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_BeforeDigits : Func_1<p_str>, Generator<p_str>
{
public:
   F_BeforeDigits(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_BeforeLetters : Func_1<p_str>, Generator<p_str>
{
public:
   F_BeforeLetters(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_RandomChar : Func_1<p_str>, Generator<p_str>
{
public:
   F_RandomChar(p_genptr<p_str>& a1, Perun2Process& p2)
      : Func_1(a1), math(p2.math) { };

   p_str getValue() override;

private:
   Math& math;
};


struct F_Capitalize : Func_1<p_str>, Generator<p_str>
{
public:
   F_Capitalize(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Join : Func_2<p_list, p_str>, Generator<p_str>
{
public:
   F_Join(p_genptr<p_list>& a1, p_genptr<p_str>& a2) : Func_2(a1, a2) { };
   p_str getValue() override;
};


struct F_Roman : Func_1<p_num>, Generator<p_str>
{
public:
   F_Roman(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_str getValue() override;

private:
   inline void appendFraction(const p_num& base, p_stream& ss) const;
};


struct F_Binary : Func_1<p_num>, Generator<p_str>
{
public:
   F_Binary(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Hex : Func_1<p_num>, Generator<p_str>
{
public:
   F_Hex(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


struct F_Raw : Func_1<p_str>, Generator<p_str>
{
public:
   F_Raw(p_genptr<p_str>& a1) : Func_1(a1) { };
   p_str getValue() override;
};


}
