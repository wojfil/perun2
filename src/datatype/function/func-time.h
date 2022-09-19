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

#ifndef FUNC_TIME_H_INCLUDED
#define FUNC_TIME_H_INCLUDED

#include "func-generic.h"


namespace uro::func
{

struct F_Christmas : Func_1<_num>, Generator<_tim>
{
public:
   F_Christmas(Generator<_num>* a1) : Func_1(a1) { };
   _tim getValue() override;
};


struct F_Easter : Func_1<_num>, Generator<_tim>
{
public:
   F_Easter(Generator<_num>* a1) : Func_1(a1) { };
   _tim getValue() override;
};


struct F_NewYear : Func_1<_num>, Generator<_tim>
{
public:
   F_NewYear(Generator<_num>* a1) : Func_1(a1) { };
   _tim getValue() override;
};


inline void checkMonthRuntime(const _tnum& month);
inline void checkDayRuntime(const _tnum& day, const _tnum& month,
   const _tnum& year);
inline void checkSmallClockRuntime(const _tnum& hour, const _tnum& minute);
inline void checkSecondsRuntime(const _tnum& second);


struct F_Time_2 : Func_2<_num, _num>, Generator<_tim>
{
public:
   F_Time_2(Generator<_num>* a1, Generator<_num>* a2) : Func_2(a1, a2) { };
   _tim getValue() override;
};


struct F_Time_3 : Func_3<_num, _num, _num>, Generator<_tim>
{
public:
   F_Time_3(Generator<_num>* a1, Generator<_num>* a2, Generator<_num>* a3)
      : Func_3(a1, a2, a3) { };
   _tim getValue() override;
};


struct F_Time_5 : Generator<_tim>
{
public:
   F_Time_5(Generator<_num>* a1, Generator<_num>* a2, Generator<_num>* a3,
      Generator<_num>* a4, Generator<_num>* a5)
      : arg1(a1), arg2(a2), arg3(a3), arg4(a4), arg5(a5) { };

   ~F_Time_5() {
      delete arg1;
      delete arg2;
      delete arg3;
      delete arg4;
      delete arg5;
   }

   _tim getValue() override;

private:
   Generator<_num>* arg1;
   Generator<_num>* arg2;
   Generator<_num>* arg3;
   Generator<_num>* arg4;
   Generator<_num>* arg5;
};


struct F_Time_6 : Generator<_tim>
{
public:
   F_Time_6(Generator<_num>* a1, Generator<_num>* a2, Generator<_num>* a3,
      Generator<_num>* a4, Generator<_num>* a5, Generator<_num>* a6)
      : arg1(a1), arg2(a2), arg3(a3), arg4(a4), arg5(a5), arg6(a6) { };

   ~F_Time_6() {
      delete arg1;
      delete arg2;
      delete arg3;
      delete arg4;
      delete arg5;
      delete arg6;
   }

   _tim getValue() override;

private:
   Generator<_num>* arg1;
   Generator<_num>* arg2;
   Generator<_num>* arg3;
   Generator<_num>* arg4;
   Generator<_num>* arg5;
   Generator<_num>* arg6;
};

}

#endif // FUNC_TIME_H_INCLUDED
