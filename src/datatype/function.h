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


static std::vector<Tokens> toFunctionArgs(const Tokens& tks);
static Generator<_str>* createThisReference(Uroboros* uro);

Generator<_boo>* boolFunction(const Tokens& tks, Uroboros* uro);
static Generator<_boo>* simpleBoolFunction(const Tokens& tks, const Token& word, Uroboros* uro);

Generator<_num>* numberFunction(const Tokens& tks, Uroboros* uro);
static Generator<_num>* simpleNumberFunction(const Tokens& tks, const Token& word, Uroboros* uro);
static Generator<_num>* aggrFunction(const std::vector<Tokens>& args, const Token& word, Uroboros* uro);

Generator<_per>* periodFunction(const Tokens& tks, Uroboros* uro);

Generator<_str>* stringFunction(const Tokens& tks, Uroboros* uro);
static Generator<_str>* stringTwoArgFunction(const std::vector<Tokens>& args, const Token& word, Uroboros* uro);
static Generator<_str>* simpleStringFunction(const Tokens& tks, const Token& word, Uroboros* uro);

Generator<_tim>* timeFunction(const Tokens& tks, Uroboros* uro);
static Generator<_tim>* simpleTimeFunction(const Tokens& tks, const Token& word, Uroboros* uro);

static void functionArgNumberException(const _int& argNumber, const Token& word, Uroboros* uro);
static void functionArgException(const _int& argNumber, const _str& typeName, const Token& word, Uroboros* uro);
static _str ordinalNumber(const _int& number);

Generator<_list>* listFunction(const Tokens& tks, Uroboros* uro);
Generator<_nlist>* numListFunction(const Tokens& tks, Uroboros* uro);

static void checkFunctionAttribute(const Token& word, Uroboros* uro);
static void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, Uroboros* uro);


#endif /* FUNCTION_H */
