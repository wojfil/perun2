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

#ifndef EXP_PARSE_BOOL_H
#define EXP_PARSE_BOOL_H

#include "../../tokens.h"
#include "../exp-element.h"
#include "../generator/gen-bool-compare.h"


namespace uro::parse
{

Generator<_bool>* parseBool(const Tokens& tks, Uroboros& uro);

static Generator<_bool>* parseBoolTernary(const Tokens& tks, Uroboros& uro);
static Generator<_bool>* parseBoolExp(const Tokens& tks, Uroboros& uro);
static Generator<_bool>* boolExpTree(
   const std::vector<ExpElement<_bool>*>& infList,
   std::vector<ExpElement<_bool>*>& pntList);
static Generator<_bool>* boolExpIntegrateNegations(
   const std::vector<ExpElement<_bool>*>& elements,
   std::vector<ExpElement<_bool>*>& pntList);
static Generator<_bool>* boolExpTreeMerge(
   const std::vector<ExpElement<_bool>*>& elements,
   std::vector<ExpElement<_bool>*>& pntList);
static _bool isBoolExpComputable(const std::vector<ExpElement<_bool>*>& infList);
_bool isBoolExpOperator(const Token& tk);
static _char toBoolExpOperator(const Token& tk);

static Generator<_bool>* parseLike(const Tokens& tks, Uroboros& uro);
static Generator<_bool>* parseIn(const Tokens& tks, Uroboros& uro);
static Generator<_bool>* parseInTimList(const bool& negated, 
   const std::pair<Tokens, Tokens>& pair, Uroboros& uro);
static void emptyOperSideException(const Token& oper, const bool& isLeft, Uroboros& uro);
static void timeInNumberException(const Token& timeVar, const Token& numVar,
   const _str& timeMember, const _bool& negated, const Tokens& tks, Uroboros& uro);
static Generator<_bool>* parseComparisons(const Tokens& tks, Uroboros& uro);
static Generator<_bool>* parseCollectionComparisons(const Tokens& left,
   const Tokens& right, const gen::CompType& ct, Uroboros& uro);
static Generator<_bool>* parseComparison(const Tokens& tks, const _char& sign, Uroboros& uro);
static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const _char& sign, gen::CompType& ctype);

}

#endif /* EXP_PARSE_BOOL_H */
