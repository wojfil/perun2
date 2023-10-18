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

#ifndef FUNC_TIME_H_INCLUDED
#define FUNC_TIME_H_INCLUDED

#include "func-generic.h"


namespace perun2::func
{

struct F_Christmas : Func_1<p_num>, Generator<p_tim>
{
public:
   F_Christmas(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_tim getValue() override;
};


struct F_Easter : Func_1<p_num>, Generator<p_tim>
{
public:
   F_Easter(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_tim getValue() override;
};


struct F_NewYear : Func_1<p_num>, Generator<p_tim>
{
public:
   F_NewYear(p_genptr<p_num>& a1) : Func_1(a1) { };
   p_tim getValue() override;
};


inline p_bool wrongMonthRuntime(const p_tnum month);
inline p_bool wrongDayRuntime(const p_tnum day, const p_tnum month,
   const p_tnum year);
inline p_bool wrongSmallClockRuntime(const p_tnum hour, const p_tnum minute);
inline p_bool wrongSecondsRuntime(const p_tnum second);


struct F_Time_2 : Func_2<p_num, p_num>, Generator<p_tim>
{
public:
   F_Time_2(p_genptr<p_num>& a1, p_genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_tim getValue() override;
};


struct F_Time_3 : Func_3<p_num, p_num, p_num>, Generator<p_tim>
{
public:
   F_Time_3(p_genptr<p_num>& a1, p_genptr<p_num>& a2, p_genptr<p_num>& a3)
      : Func_3(a1, a2, a3) { };
   p_tim getValue() override;
};


struct F_Time_5 : Generator<p_tim>
{
public:
   F_Time_5(p_genptr<p_num>& a1, p_genptr<p_num>& a2, p_genptr<p_num>& a3,
      p_genptr<p_num>& a4, p_genptr<p_num>& a5)
      : arg1(std::move(a1)), arg2(std::move(a2)), arg3(std::move(a3)), 
        arg4(std::move(a4)), arg5(std::move(a5)) { };

   p_tim getValue() override;

private:
   p_genptr<p_num> arg1;
   p_genptr<p_num> arg2;
   p_genptr<p_num> arg3;
   p_genptr<p_num> arg4;
   p_genptr<p_num> arg5;
};


struct F_Time_6 : Generator<p_tim>
{
public:
   F_Time_6(p_genptr<p_num>& a1, p_genptr<p_num>& a2, p_genptr<p_num>& a3,
      p_genptr<p_num>& a4, p_genptr<p_num>& a5, p_genptr<p_num>& a6)
      : arg1(std::move(a1)), arg2(std::move(a2)), arg3(std::move(a3)), 
        arg4(std::move(a4)), arg5(std::move(a5)), arg6(std::move(a6)) { };

   p_tim getValue() override;

private:
   p_genptr<p_num> arg1;
   p_genptr<p_num> arg2;
   p_genptr<p_num> arg3;
   p_genptr<p_num> arg4;
   p_genptr<p_num> arg5;
   p_genptr<p_num> arg6;
};

}

#endif // FUNC_TIME_H_INCLUDED
