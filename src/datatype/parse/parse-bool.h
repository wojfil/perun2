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

_bool parseBool(_genptr<_bool>& result, const Tokens& tks, p_perun2& p2);

static _bool parseBoolExp(_genptr<_bool>& result, const Tokens& tks, p_perun2& p2);
static _bool boolExpTree(_genptr<_bool>& result, std::vector<ExpElement<_bool>>& infList);
static _bool boolExpIntegrateNegations(_genptr<_bool>& result, std::vector<ExpElement<_bool>>& elements);
static _bool boolExpTreeMerge(_genptr<_bool>& result, std::vector<ExpElement<_bool>>& elements);
static _bool isBoolExpComputable(const std::vector<ExpElement<_bool>>& infList);
_bool isBoolExpOperator(const Token& tk);
static _char toBoolExpOperator(const Token& tk);

static _bool parseLike(_genptr<_bool>& result, const Tokens& tks, p_perun2& p2);
static _bool parseIn(_genptr<_bool>& result, const Tokens& tks, p_perun2& p2);
static _bool parseInTimList(_genptr<_bool>& result, const bool& negated, 
   const std::pair<Tokens, Tokens>& pair, p_perun2& p2);
static _bool parseComparisons(_genptr<_bool>& result, const Tokens& tks, p_perun2& p2);
static _bool parseCollectionComparisons(_genptr<_bool>& result, const Tokens& left,
   const Tokens& right, const gen::CompType& ct, p_perun2& p2);
static _bool parseComparison(_genptr<_bool>& result, const Tokens& tks, const _char sign, p_perun2& p2);
static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const _char sign, gen::CompType& ctype);

}

#endif /* EXP_PARSE_BOOL_H */
