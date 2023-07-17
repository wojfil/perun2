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

#ifndef PARSE_PERIOD_H
#define PARSE_PERIOD_H

#include "../../tokens.h"
#include "../../perun2.h"


namespace perun2::parse
{

_bool parsePeriod(_genptr<_per>& result, const Tokens& tks, p_perun2& p2);

_bool parsePeriodConst(_genptr<_per>& result, const Tokens& tks, const _bool negated, p_perun2& p2);
_bool parsePeriodUnit(_genptr<_per>& result, const Tokens& tks, p_perun2& p2);
_bool parsePeriodExp(_genptr<_per>& result, const Tokens& tks, p_perun2& p2);
_bool parsePeriodExpDiff(_genptr<_per>& result, const Tokens& tks, p_perun2& p2);
_bool parseTimeDifference(_genptr<_per>& result, const Tokens& tks, p_perun2& p2);
Period::PeriodUnit toPeriodUnit(const Token& tk, p_perun2& p2);

}

#endif /* PARSE_PERIOD_H */
