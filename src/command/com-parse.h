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

_bool parseCommands(_comptr& result, const Tokens& tks, Uroboros& uro);

static _bool commandStruct(_comptr& result, const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open, Uroboros& uro);
static _bool parseIterationLoop(_comptr& result, const _bool& isInside, const Tokens& left, const Tokens& right,
   const ThisState& prevState, Uroboros& uro);
static _bool parseLoopBase(_comptr& result, const Tokens& rightTokens, Uroboros& uro,
   const ThisState& prevState, Attribute*& attr, _aggrptr& aggr, _bool& hasMemory);
static _bool parseLoopBase(_comptr& result, const Tokens& rightTokens, Uroboros& uro,
   const ThisState& prevState, _aggrptr& aggr);
static _bool parseCommandsAsMember(_comptr& result, const Tokens& tks, _comptr* cond, Uroboros& uro);
static _bool command(_comptr& result, Tokens& tks, Uroboros& uro);
static _bool commandMisc(_comptr& result, const Tokens& tks, Uroboros& uro);
static _bool commandVarChange(_comptr& result, const Tokens& left, const Tokens& right,
   const _char& sign, Uroboros& uro);
static _bool commandVarIncrement(_comptr& result, const Token& first, const Tokens& tks,
   const _int& line, Uroboros& uro);
static _bool commandVarAssign(_comptr& result, const Tokens& left, const Tokens& right, Uroboros& uro);

static _bool varSquareBrackets(const Tokens& tks);
static _bool commandVarAssign_Element(_comptr& result, const Tokens& left, const Tokens& right, Uroboros& uro);
static _bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, Uroboros& uro);
static void checkNoSemicolonBeforeBrackets(const Tokens& tkss, Uroboros& uro);

}

#endif /* COM_PARSE_H */
