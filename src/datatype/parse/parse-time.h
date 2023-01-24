/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSE_TIME_H
#define PARSE_TIME_H

#include "../../tokens.h"
#include "../../uroboros.h"


namespace uro::parse
{

_bool parseTime(_genptr<_tim>& result, const Tokens& tks, _uro& uro);

_bool parseTimeConst(_genptr<_tim>& result, const Tokens& tks, _uro& uro);
static _tnum tokenToTimeNumber(const Token& tk);
static void checkDayCorrectness(const _tnum& day, const _tnum& month, const _tnum& year, const Token& tk);
static _bool parseTimeExp(_genptr<_tim>& result, const Tokens& tks, _uro& uro);
static _bool timeExpUnit(_int& sublen, const _bool& subtract, _bool& prevSubtract,
   _genptr<_tim>& prevTim, _genptr<_tim>& time, const Tokens& tks, _int& numReserve, _uro& uro);

}

#endif /* PARSE_TIME_H */
