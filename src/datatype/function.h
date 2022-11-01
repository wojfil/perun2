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

#ifndef FUNCTION_H
#define FUNCTION_H

#include "generator.h"
#include "../tokens.h"
#include "../uroboros.h"


namespace uro::func
{

// this is function parsing 
// it could have been done in a more generalized way
// on the other hand, current approach
// enables variety of precise error messages


static std::vector<Tokens> toFunctionArgs(const Tokens& tks);
static _bool createThisReference(Uroboros& uro);

_bool boolFunction(_genptr<_bool>& result, const Tokens& tks, Uroboros& uro);
_bool simpleBoolFunction(_genptr<_bool>& result, const Tokens& tks, const Token& word, Uroboros& uro);

_bool numberFunction(_genptr<_num>& result, const Tokens& tks, Uroboros& uro);
static _bool simpleNumberFunction(_genptr<_num>& result, const Tokens& tks, const Token& word, Uroboros& uro);
static _bool aggrFunction(_genptr<_num>& result, const std::vector<Tokens>& args, const Token& word, Uroboros& uro);

_bool periodFunction(_genptr<_per>& result, const Tokens& tks, Uroboros& uro);

_bool stringFunction(_genptr<_str>& result, const Tokens& tks, Uroboros& uro);
static _bool stringTwoArgFunction(_genptr<_str>& result, const std::vector<Tokens>& args, const Token& word, Uroboros& uro);
static _bool simpleStringFunction(_genptr<_str>& result, const Tokens& tks, const Token& word, Uroboros& uro);

_bool timeFunction(_genptr<_tim>& result, const Tokens& tks, Uroboros& uro);
static _bool simpleTimeFunction(_genptr<_tim>& result, const Tokens& tks, const Token& word, Uroboros& uro);

static void functionArgNumberException(const _int& argNumber, const Token& word, Uroboros& uro);
static void functionArgException(const _int& argNumber, const _str& typeName, const Token& word, Uroboros& uro);
static _str ordinalNumber(const _int& number);

_bool listFunction(_genptr<_list>& result, const Tokens& tks, Uroboros& uro);
_bool numListFunction(_genptr<_nlist>& result, const Tokens& tks, Uroboros& uro);

static void checkFunctionAttribute(const Token& word, Uroboros& uro);
static void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, Uroboros& uro);

}

#endif /* FUNCTION_H */
