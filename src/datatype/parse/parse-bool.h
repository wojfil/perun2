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

#ifndef EXP_PARSE_BOOL_H
#define EXP_PARSE_BOOL_H

#include "../../tokens.h"
#include "../exp-element.h"
#include "../generator/gen-bool-compare.h"


namespace perun2::parse
{

p_bool parseBool(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);

static p_bool tryToParseBoolExp(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseBoolExp(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
static p_bool boolExpTree(p_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& infList);
static p_bool boolExpIntegrateNegations(p_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& elements);
static p_bool boolExpTreeMerge(p_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& elements);
static p_bool isBoolExpComputable(const std::vector<ExpElement<p_bool>>& infList);
p_bool isBoolExpOperator(const Token& tk);
static p_char toBoolExpOperator(const Token& tk);

static p_bool parseLike(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseIn(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseInTimList(p_genptr<p_bool>& result, const bool& negated, 
   const std::pair<Tokens, Tokens>& pair, Perun2Process& p2);
static p_bool parseResembles(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);

static p_bool parseBetween(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseBetweenNumbers(p_genptr<p_bool>& result, 
   p_genptr<p_num>& value, const Tokens& left, const Tokens& right, Perun2Process& p2);
static p_bool parseBetweenTimes(p_genptr<p_bool>& result, 
   p_genptr<p_tim>& value, const Tokens& left, const Tokens& right, Perun2Process& p2);



static p_bool parseComparisons(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseCollectionComparisons(p_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const CompType ct, Perun2Process& p2);
static p_bool parseComparison(p_genptr<p_bool>& result, const Tokens& tks, const p_char sign, Perun2Process& p2);
static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const p_char sign, CompType& ctype);

}

#endif /* EXP_PARSE_BOOL_H */
