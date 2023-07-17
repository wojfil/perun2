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

#ifndef COM_PARSE_H
#define COM_PARSE_H

#include <vector>
#include "../token.h"
#include "../tokens.h"
#include "../lexer.h"
#include "../perun2.h"
#include "com.h"
#include "com-var.h"
#include "com-struct.h"
#include "com-condition.h"


namespace perun2::comm
{

_bool parseCommands(_comptr& result, const Tokens& tks, p_perun2& p2);

static _bool commandStruct(_comptr& result, const Tokens& tks, const _int sublen,
   const _int index, const _int open, p_perun2& p2);
static _bool parseIterationLoop(_comptr& result, const Tokens& left, const Tokens& right, p_perun2& p2);
static _bool parseInsideLoop(_comptr& result, const Token& keyword, const Tokens& left, const Tokens& right, p_perun2& p2);
static _bool parseCommandsAsMember(_comptr& result, const Tokens& tks, _comptr* cond, p_perun2& p2);
static _bool command(_comptr& result, Tokens& tks, p_perun2& p2);
static _bool commandMisc(_comptr& result, const Tokens& tks, p_perun2& p2);
static _bool commandVarChange(_comptr& result, const Tokens& left, const Tokens& right,
   const _char sign, p_perun2& p2);
static _bool commandVarIncrement(_comptr& result, const Token& first, const Tokens& tks,
   const _int line, p_perun2& p2);
static _bool commandVarAssign(_comptr& result, const Tokens& left, const Tokens& right, p_perun2& p2);

static _bool varSquareBrackets(const Tokens& tks);
static _bool commandVarAssign_Element(_comptr& result, const Tokens& left, const Tokens& right, p_perun2& p2);
static _bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, p_perun2& p2);
static void checkNoSemicolonBeforeBrackets(const Tokens& tkss, p_perun2& p2);

}

#endif /* COM_PARSE_H */
