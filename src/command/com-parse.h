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
#include "com.h"
#include "com-var.h"
#include "com-struct.h"
#include "com-condition.h"


Command* parseCommands(const Tokens& tks);
static Command* commandStruct(const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open);
static void beforeCommandStruct(CS_If* pntr);
static void afterCommandStruct();
static Command* command(Tokens& tks);
static Command* commandMisc(const Tokens& tks);
static Command* commandVarChange(const Tokens& left, const Tokens& right,
   const _char& sign);
static Command* commandVarIncrement(const Token& first, const Tokens& tks,
   const _int& line);
static Command* commandVarAssign(const Tokens& left, const Tokens& right);
static void varAssignException(const _str& name, const _str& type,
   const Tokens& tks);

static _boo varSquareBrackets(const Tokens& tks);
static Command* commandVarAssign_Element(const Tokens& left,
   const Tokens& right);
static Generator<_num>* parseListElementIndex(const Tokens& tks);
static void checkNoSemicolonBeforeBrackets(const Tokens& tks);


#endif /* COM_PARSE_H */
