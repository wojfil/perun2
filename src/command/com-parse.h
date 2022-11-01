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

Command* parseCommands(const Tokens& tks, Uroboros& uro);

static Command* commandStruct(const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open, Uroboros& uro);
static Command* parseIterationLoop(const _bool& isInside, const Tokens& left, const Tokens& right,
   const ThisState& prevState, Uroboros& uro);
static _bool parseLoopBase(Command*& com, const Tokens& rightTokens, Uroboros& uro,
   const ThisState& prevState, Attribute*& attr, Aggregate*& aggr, _bool& hasMemory);
static _bool parseLoopBase(Command*& com, const Tokens& rightTokens, Uroboros& uro,
   const ThisState& prevState, Aggregate*& aggr);
static Command* parseCommandsAsMember(const Tokens& tks, CS_Condition* cond, Uroboros& uro);
static Command* command(Tokens& tks, Uroboros& uro);
static Command* commandMisc(const Tokens& tks, Uroboros& uro);
static Command* commandVarChange(const Tokens& left, const Tokens& right,
   const _char& sign, Uroboros& uro);
static Command* commandVarIncrement(const Token& first, const Tokens& tks,
   const _int& line, Uroboros& uro);
static Command* commandVarAssign(const Tokens& left, const Tokens& right, Uroboros& uro);

static _bool varSquareBrackets(const Tokens& tks);
static Command* commandVarAssign_Element(const Tokens& left, const Tokens& right, Uroboros& uro);
static _bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, Uroboros& uro);
static void checkNoSemicolonBeforeBrackets(const Tokens& tkss, Uroboros& uro);

}

#endif /* COM_PARSE_H */
