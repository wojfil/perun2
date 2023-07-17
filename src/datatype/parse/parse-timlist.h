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

#ifndef PARSE_TIMLIST_H
#define PARSE_TIMLIST_H

#include "../parse-gen.h"


namespace perun2::parse
{

p_bool parseTimList(_genptr<_tlist>& result, const Tokens& tks, p_perun2& p2);

static p_bool parseTimListed(_genptr<_tlist>& result, const Tokens& tks, p_perun2& p2);
static p_bool parseListedTimes(_genptr<_tlist>& res, const std::vector<Tokens>& elements, p_perun2& p2);
static p_bool timeFromTwoSeqs(_genptr<_tim>& result, const Tokens& prev, const Tokens& curr, p_perun2& p2);
static p_bool parseListedTimLists(_genptr<_tlist>& res, const std::vector<Tokens>& elements, p_perun2& p2);

}

#endif /* PARSE_TIMLIST_H */
