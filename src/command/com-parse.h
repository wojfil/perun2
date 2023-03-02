/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
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

_bool parseCommands(_comptr& result, const Tokens& tks, _uro& uro);

static _bool commandStruct(_comptr& result, const Tokens& tks, const _int sublen,
   const _int index, const _int open, _uro& uro);
static _bool parseIterationLoop(_comptr& result, const Tokens& left, const Tokens& right, _uro& uro);
static _bool parseInsideLoop(_comptr& result, const Token& keyword, const Tokens& left, const Tokens& right, _uro& uro);
static _bool parseCommandsAsMember(_comptr& result, const Tokens& tks, _comptr* cond, _uro& uro);
static _bool command(_comptr& result, Tokens& tks, _uro& uro);
static _bool commandMisc(_comptr& result, const Tokens& tks, _uro& uro);
static _bool commandVarChange(_comptr& result, const Tokens& left, const Tokens& right,
   const _char sign, _uro& uro);
static _bool commandVarIncrement(_comptr& result, const Token& first, const Tokens& tks,
   const _int line, _uro& uro);
static _bool commandVarAssign(_comptr& result, const Tokens& left, const Tokens& right, _uro& uro);

static _bool varSquareBrackets(const Tokens& tks);
static _bool commandVarAssign_Element(_comptr& result, const Tokens& left, const Tokens& right, _uro& uro);
static _bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, _uro& uro);
static void checkNoSemicolonBeforeBrackets(const Tokens& tkss, _uro& uro);

}

#endif /* COM_PARSE_H */
