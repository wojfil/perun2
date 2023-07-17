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

#ifndef EXP_PARSE_NUMBER_H
#define EXP_PARSE_NUMBER_H

#include "../../tokens.h"
#include "../../perun2.h"
#include "../exp-element.h"


namespace perun2::parse
{

p_bool parseNumber(_genptr<p_num>& result, const Tokens& tks, p_perun2& p2);

static p_bool parseNumExp(_genptr<p_num>& result, const Tokens& tks, p_perun2& p2);
static p_bool numExpTree(_genptr<p_num>& result, std::vector<ExpElement<p_num>>& infList);
static p_bool numExpIntegrateUnary(_genptr<p_num>& result, std::vector<ExpElement<p_num>>& elements);
static p_bool numExpTreeMerge(_genptr<p_num>& result, std::vector<ExpElement<p_num>>& elements);
static p_bool numExpTreeMerge2(_genptr<p_num>& result, std::vector<ExpElement<p_num>>& elements);
static p_bool isNumExpComputable(const std::vector<ExpElement<p_num>>& infList);
static p_bool isNumExpOperator(const p_char ch);
static p_bool isNumExpHighPriority(const p_char ch);
void timeVariableMemberException(const Token& tk, p_perun2& p2);

}

#endif /* EXP_PARSE_NUMBER_H */
