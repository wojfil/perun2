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

#include "../../tokens.hpp"
#include "../../perun2.hpp"


namespace perun2::parse
{

p_bool parseTime(p_genptr<p_tim>& result, const Tokens& tks, Perun2Process& p2);

p_bool parseTimeConst(p_genptr<p_tim>& result, const Tokens& tks, Perun2Process& p2);
p_bool parseClockConst(p_genptr<p_tim>& result, const Tokens& tks, Perun2Process& p2);
static p_tnum tokenToTimeNumber(const Token& tk);
static void checkDayCorrectness(const p_tnum day, const p_tnum month, const p_tnum year, const Token& tk);
static p_bool parseTimeExp(p_genptr<p_tim>& result, const Tokens& tks, Perun2Process& p2);
static p_bool timeExpUnit(p_int& sublen, const p_bool subtract, p_bool& prevSubtract,
   p_genptr<p_tim>& prevTim, p_genptr<p_tim>& time, const Tokens& tks, p_int& numReserve, Perun2Process& p2);

}
