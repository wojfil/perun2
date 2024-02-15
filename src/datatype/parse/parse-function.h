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

std::vector<Tokens> toFunctionArgs(const Tokens& tks);

p_bool boolFunction(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2);
p_bool simpleBoolFunction(p_genptr<p_bool>& result, const Tokens& tks, const Token& word, Perun2Process& p2);

p_bool numberFunction(p_genptr<p_num>& result, const Tokens& tks, Perun2Process& p2);
static p_bool simpleNumberFunction(p_genptr<p_num>& result, const Tokens& tks, const Token& word, Perun2Process& p2);
static p_bool aggrFunction(p_genptr<p_num>& result, const std::vector<Tokens>& args, const Token& word, Perun2Process& p2);

p_bool periodFunction(p_genptr<p_per>& result, const Tokens& tks, Perun2Process& p2);

p_bool stringFunction(p_genptr<p_str>& result, const Tokens& tks, Perun2Process& p2);
static p_bool stringTwoArgFunction(p_genptr<p_str>& result, const std::vector<Tokens>& args, const Token& word, Perun2Process& p2);
static p_bool simpleStringFunction(p_genptr<p_str>& result, const Tokens& tks, const Token& word, Perun2Process& p2);

p_bool timeFunction(p_genptr<p_tim>& result, const Tokens& tks, Perun2Process& p2);
static p_bool simpleTimeFunction(p_genptr<p_tim>& result, const Tokens& tks, const Token& word, Perun2Process& p2);

void functionArgNumberException(const p_int argNumber, const Token& word, Perun2Process& p2);
void functionArgException(const p_int argNumber, const p_str& typeName, const Token& word, Perun2Process& p2);
p_str ordinalNumber(const p_int number);

p_bool listFunction(p_genptr<p_list>& result, const Tokens& tks, Perun2Process& p2);
p_bool numListFunction(p_genptr<p_nlist>& result, const Tokens& tks, Perun2Process& p2);

void checkFunctionAttribute(const Token& word, Perun2Process& p2);
void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, Perun2Process& p2);

}


#endif // PARSE_FUNCTION_H_INCLUDED
