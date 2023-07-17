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

p_bool parseCommands(_comptr& result, const Tokens& tks, pp_perun2& p2);

static p_bool commandStruct(_comptr& result, const Tokens& tks, const p_int sublen,
   const p_int index, const p_int open, pp_perun2& p2);
static p_bool parseIterationLoop(_comptr& result, const Tokens& left, const Tokens& right, pp_perun2& p2);
static p_bool parseInsideLoop(_comptr& result, const Token& keyword, const Tokens& left, const Tokens& right, pp_perun2& p2);
static p_bool parseCommandsAsMember(_comptr& result, const Tokens& tks, _comptr* cond, pp_perun2& p2);
static p_bool command(_comptr& result, Tokens& tks, pp_perun2& p2);
static p_bool commandMisc(_comptr& result, const Tokens& tks, pp_perun2& p2);
static p_bool commandVarChange(_comptr& result, const Tokens& left, const Tokens& right,
   const p_char sign, pp_perun2& p2);
static p_bool commandVarIncrement(_comptr& result, const Token& first, const Tokens& tks,
   const p_int line, pp_perun2& p2);
static p_bool commandVarAssign(_comptr& result, const Tokens& left, const Tokens& right, pp_perun2& p2);

static p_bool varSquareBrackets(const Tokens& tks);
static p_bool commandVarAssign_Element(_comptr& result, const Tokens& left, const Tokens& right, pp_perun2& p2);
static p_bool parseListElementIndex(_genptr<p_num>& result, const Tokens& tks, pp_perun2& p2);
static void checkNoSemicolonBeforeBrackets(const Tokens& tkss, pp_perun2& p2);

}

#endif /* COM_PARSE_H */
