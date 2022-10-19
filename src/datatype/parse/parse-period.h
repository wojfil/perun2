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

Generator<_per>* parsePeriod(const Tokens& tks, Uroboros& uro);

static Generator<_per>* parsePeriodConst(const Tokens& tks, const _bool& negated, Uroboros& uro);
static Generator<_per>* parsePeriodUnit(const Tokens& tks, Uroboros& uro);
static void unitNameException(const _str& name, const Tokens& tks);
static Generator<_per>* parsePeriodExp(const Tokens& tks, Uroboros& uro);
static Generator<_per>* parsePeriodExpDiff(const Tokens& tks, Uroboros& uro);
static Generator<_per>* parseTimeDifference(const Tokens& tks, Uroboros& uro);

}

#endif /* PARSE_PERIOD_H */
