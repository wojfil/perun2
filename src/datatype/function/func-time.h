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
   F_Christmas(_genptr<p_num>& a1) : Func_1(a1) { };
   p_tim getValue() override;
};


struct F_Easter : Func_1<p_num>, Generator<p_tim>
{
public:
   F_Easter(_genptr<p_num>& a1) : Func_1(a1) { };
   p_tim getValue() override;
};


struct F_NewYear : Func_1<p_num>, Generator<p_tim>
{
public:
   F_NewYear(_genptr<p_num>& a1) : Func_1(a1) { };
   p_tim getValue() override;
};


inline void checkMonthRuntime(const _tnum month);
inline void checkDayRuntime(const _tnum day, const _tnum month,
   const _tnum year);
inline void checkSmallClockRuntime(const _tnum hour, const _tnum minute);
inline void checkSecondsRuntime(const _tnum second);


struct F_Time_2 : Func_2<p_num, p_num>, Generator<p_tim>
{
public:
   F_Time_2(_genptr<p_num>& a1, _genptr<p_num>& a2) : Func_2(a1, a2) { };
   p_tim getValue() override;
};


struct F_Time_3 : Func_3<p_num, p_num, p_num>, Generator<p_tim>
{
public:
   F_Time_3(_genptr<p_num>& a1, _genptr<p_num>& a2, _genptr<p_num>& a3)
      : Func_3(a1, a2, a3) { };
   p_tim getValue() override;
};


struct F_Time_5 : Generator<p_tim>
{
public:
   F_Time_5(_genptr<p_num>& a1, _genptr<p_num>& a2, _genptr<p_num>& a3,
      _genptr<p_num>& a4, _genptr<p_num>& a5)
      : arg1(std::move(a1)), arg2(std::move(a2)), arg3(std::move(a3)), 
        arg4(std::move(a4)), arg5(std::move(a5)) { };

   p_tim getValue() override;

private:
   _genptr<p_num> arg1;
   _genptr<p_num> arg2;
   _genptr<p_num> arg3;
   _genptr<p_num> arg4;
   _genptr<p_num> arg5;
};


struct F_Time_6 : Generator<p_tim>
{
public:
   F_Time_6(_genptr<p_num>& a1, _genptr<p_num>& a2, _genptr<p_num>& a3,
      _genptr<p_num>& a4, _genptr<p_num>& a5, _genptr<p_num>& a6)
      : arg1(std::move(a1)), arg2(std::move(a2)), arg3(std::move(a3)), 
        arg4(std::move(a4)), arg5(std::move(a5)), arg6(std::move(a6)) { };

   p_tim getValue() override;

private:
   _genptr<p_num> arg1;
   _genptr<p_num> arg2;
   _genptr<p_num> arg3;
   _genptr<p_num> arg4;
   _genptr<p_num> arg5;
   _genptr<p_num> arg6;
};

}

#endif // FUNC_TIME_H_INCLUDED
