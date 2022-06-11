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

#include "../parse-gen.h"
#include "../generator/gen-bool-compare.h"


Generator<_boo>* parseBool(const Tokens& tks);

static Generator<_boo>* parseBoolTernary(const Tokens& tks);
static Generator<_boo>* parseBoolExp(const Tokens& tks);
static Generator<_boo>* boolExpTree(
   const std::vector<ExpElement<_boo>*>& infList,
   std::vector<ExpElement<_boo>*>& pntList);
static Generator<_boo>* boolExpIntegrateNegations(
   const std::vector<ExpElement<_boo>*>& elements,
   std::vector<ExpElement<_boo>*>& pntList);
static Generator<_boo>* boolExpTreeMerge(
   const std::vector<ExpElement<_boo>*>& elements,
   std::vector<ExpElement<_boo>*>& pntList);
static _boo isBoolExpComputable(const std::vector<ExpElement<_boo>*>& infList);
_boo isBoolExpOperator(const Token& tk);
static _char toBoolExpOperator(const Token& tk);

static Generator<_boo>* parseLike(const Tokens& tks);
static Generator<_boo>* parseIn(const Tokens& tks);
static Generator<_boo>* parseInNumList(const bool& negated, const Tokens& left,
   const Tokens& right);
static Generator<_boo>* parseInTimList(const bool& negated, const Tokens& left,
   const Tokens& right);
static Generator<_boo>* parseInList(const bool& negated, const Tokens& left,
   const Tokens& right);
static void emptyOperSideException(const Token& oper, const bool& isLeft);
static void timeInNumberException(const Token& timeVar, const Token& numVar,
   const _str& timeMember, const _boo& negated, const Tokens& tks);
static Generator<_boo>* parseComparisons(const Tokens& tks);
static Generator<_boo>* parseCollectionComparisons(const Tokens& left,
   const Tokens& right, const CompType& ct);
static Generator<_boo>* parseComparison(const Tokens& tks, const _char& sign);
static CompType prepareComparison(const Tokens& tks, const _char& sign,
   Tokens& left, Tokens& right);


#endif /* EXP_PARSE_BOOL_H */
