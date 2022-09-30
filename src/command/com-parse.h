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

#ifndef COM_PARSE_H
#define COM_PARSE_H

#include <vector>
#include "../token.h"
#include "../tokens.h"
#include "../lexer.h"
#include "../uroboros.h"
#include "com.h"
#include "com-var.h"
#include "com-struct.h"
#include "com-condition.h"


namespace uro::comm
{

Command* parseCommands(const Tokens& tks, uro::Uroboros* uro);

static Command* commandStruct(const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open, uro::Uroboros* uro);
static Command* parseIterationLoop(const _bool& isInside, const Tokens& left, const Tokens& right,
   const ThisState& prevState, uro::Uroboros* uro);
static _bool parseLoopBase(Command*& com, const Tokens& rightTokens, uro::Uroboros* uro,
   const ThisState& prevState, Attribute*& attr, Aggregate*& aggr, _bool& hasMemory);
static _bool parseLoopBase(Command*& com, const Tokens& rightTokens, uro::Uroboros* uro,
   const ThisState& prevState, Aggregate*& aggr);
static Command* parseCommandsAsMember(const Tokens& tks, CS_Condition* cond, uro::Uroboros* uro);
static Command* command(Tokens& tks, uro::Uroboros* uro);
static Command* commandMisc(const Tokens& tks, uro::Uroboros* uro);
static Command* commandVarChange(const Tokens& left, const Tokens& right,
   const _char& sign, uro::Uroboros* uro);
static Command* commandVarIncrement(const Token& first, const Tokens& tks,
   const _int& line, uro::Uroboros* uro);
static Command* commandVarAssign(const Tokens& left, const Tokens& right, uro::Uroboros* uro);

static _bool varSquareBrackets(const Tokens& tks);
static Command* commandVarAssign_Element(const Tokens& left, const Tokens& right, uro::Uroboros* uro);
static Generator<_num>* parseListElementIndex(const Tokens& tks, uro::Uroboros* uro);
static void checkNoSemicolonBeforeBrackets(const Tokens& tkss, uro::Uroboros* uro);

}

#endif /* COM_PARSE_H */
