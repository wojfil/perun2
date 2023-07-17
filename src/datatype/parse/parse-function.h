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

#ifndef PARSE_FUNCTION_H_INCLUDED
#define PARSE_FUNCTION_H_INCLUDED

#include "../generator.h"
#include "../../tokens.h"
#include "../../perun2.h"


namespace perun2::func
{

// this is function parsing
// it could have been done in a more generalized way
// on the other hand, current approach
// enables variety of precise error messages

static std::vector<Tokens> toFunctionArgs(const Tokens& tks);

p_bool boolFunction(_genptr<p_bool>& result, const Tokens& tks, p_perun2& p2);
p_bool simpleBoolFunction(_genptr<p_bool>& result, const Tokens& tks, const Token& word, p_perun2& p2);

p_bool numberFunction(_genptr<_num>& result, const Tokens& tks, p_perun2& p2);
static p_bool simpleNumberFunction(_genptr<_num>& result, const Tokens& tks, const Token& word, p_perun2& p2);
static p_bool aggrFunction(_genptr<_num>& result, const std::vector<Tokens>& args, const Token& word, p_perun2& p2);

p_bool periodFunction(_genptr<_per>& result, const Tokens& tks, p_perun2& p2);

p_bool stringFunction(_genptr<p_str>& result, const Tokens& tks, p_perun2& p2);
static p_bool stringTwoArgFunction(_genptr<p_str>& result, const std::vector<Tokens>& args, const Token& word, p_perun2& p2);
static p_bool simpleStringFunction(_genptr<p_str>& result, const Tokens& tks, const Token& word, p_perun2& p2);

p_bool timeFunction(_genptr<_tim>& result, const Tokens& tks, p_perun2& p2);
static p_bool simpleTimeFunction(_genptr<_tim>& result, const Tokens& tks, const Token& word, p_perun2& p2);

static void functionArgNumberException(const p_int argNumber, const Token& word, p_perun2& p2);
static void functionArgException(const p_int argNumber, const p_str& typeName, const Token& word, p_perun2& p2);
static p_str ordinalNumber(const p_int number);

p_bool listFunction(_genptr<p_list>& result, const Tokens& tks, p_perun2& p2);
p_bool numListFunction(_genptr<_nlist>& result, const Tokens& tks, p_perun2& p2);

static void checkFunctionAttribute(const Token& word, p_perun2& p2);
static void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, p_perun2& p2);

}


#endif // PARSE_FUNCTION_H_INCLUDED
