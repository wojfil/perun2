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
#include "../exp-element.hpp"


namespace perun2::parse
{

p_bool parseNumber(p_genptr<p_num>& result, const Tokens& tks, Perun2Process& p2);

static p_bool parseNumExp(p_genptr<p_num>& result, const Tokens& tks, Perun2Process& p2);
static p_bool numExpTree(p_genptr<p_num>& result, std::vector<ExpElement<p_num>>& infList);
static p_bool numExpIntegrateUnary(p_genptr<p_num>& result, std::vector<ExpElement<p_num>>& elements);
static p_bool numExpTreeMerge(p_genptr<p_num>& result, std::vector<ExpElement<p_num>>& elements);
static p_bool numExpTreeMerge2(p_genptr<p_num>& result, std::vector<ExpElement<p_num>>& elements);
static p_bool isNumExpComputable(const std::vector<ExpElement<p_num>>& infList);
static p_bool isNumExpOperator(const p_char ch);
static p_bool isNumExpHighPriority(const p_char ch);
void timeVariableMemberException(const Token& tk, Perun2Process& p2);

}
