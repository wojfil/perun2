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

#ifndef PARSE_PERIOD_H
#define PARSE_PERIOD_H

#include "../../tokens.h"
#include "../../uroboros.h"


namespace uro::parse
{

_bool parsePeriod(_genptr<_per>& result, const Tokens& tks, _uro& uro);

_bool parsePeriodConst(_genptr<_per>& result, const Tokens& tks, const _bool& negated, _uro& uro);
_bool parsePeriodUnit(_genptr<_per>& result, const Tokens& tks, _uro& uro);
_bool parsePeriodExp(_genptr<_per>& result, const Tokens& tks, _uro& uro);
_bool parsePeriodExpDiff(_genptr<_per>& result, const Tokens& tks, _uro& uro);
_bool parseTimeDifference(_genptr<_per>& result, const Tokens& tks, _uro& uro);

}

#endif /* PARSE_PERIOD_H */
