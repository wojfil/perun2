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

#ifndef EXP_PARSE_BOOL_H
#define EXP_PARSE_BOOL_H

#include "../../tokens.h"
#include "../exp-element.h"
#include "../generator/gen-bool-compare.h"


namespace perun2::parse
{

p_bool parseBool(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2);

static p_bool parseBoolExp(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2);
static p_bool boolExpTree(_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& infList);
static p_bool boolExpIntegrateNegations(_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& elements);
static p_bool boolExpTreeMerge(_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& elements);
static p_bool isBoolExpComputable(const std::vector<ExpElement<p_bool>>& infList);
p_bool isBoolExpOperator(const Token& tk);
static p_char toBoolExpOperator(const Token& tk);

static p_bool parseLike(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2);
static p_bool parseIn(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2);
static p_bool parseInTimList(_genptr<p_bool>& result, const bool& negated, 
   const std::pair<Tokens, Tokens>& pair, pp_perun2& p2);
static p_bool parseComparisons(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2);
static p_bool parseCollectionComparisons(_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const gen::CompType& ct, pp_perun2& p2);
static p_bool parseComparison(_genptr<p_bool>& result, const Tokens& tks, const p_char sign, pp_perun2& p2);
static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const p_char sign, gen::CompType& ctype);

}

#endif /* EXP_PARSE_BOOL_H */
