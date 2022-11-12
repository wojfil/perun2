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

#ifndef COM_PARSE_KW_H
#define COM_PARSE_KW_H


#include "../token.h"
#include "../tokens.h"
#include "com.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "../uroboros.h"


namespace uro::comm
{

_bool keywordCommands(_comptr& result, const Token& word, Tokens& tks,
   const _int& line, const _bool& force, const _bool& stack, Uroboros& uro);

static _bool kwCommandSimple(_comptr& result, const Token& word, Tokens& tks,
   const _int& line, Uroboros& uro);
static _bool coreCommandSimpleSave(_comptr& result, const Token& word, Uroboros& uro);
static _bool coreCommandSimpleNoSave(_comptr& result, const Token& word, Uroboros& uro);
static _bool kwCommandTime(_comptr& result, const Token& word, Tokens& tks, const _int& line, Uroboros& uro);
static _bool coreCommandTime(_comptr& result, const Token& word, _genptr<_tim>& time, const _bool& saveChanges, Uroboros& uro);

static _bool c_open(_comptr& result, const Token& word, const Tokens& tks, const _int& line, Uroboros& uro);
static _bool c_select(_comptr& result, const Token& word, const Tokens& tks, const _int& line, Uroboros& uro);
static _bool c_rename(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, Uroboros& uro);
static _bool c_create(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, Uroboros& uro);
static _bool c_moveTo(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, Uroboros& uro);
static _bool c_downloadFrom(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
    const _bool& force, const _bool& stack, Uroboros& uro);
static _bool c_copy(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, Uroboros& uro);

_bool c_print(_comptr& result, const Token& word, const Tokens& tks, const _int& line, const _bool& directError, Uroboros& uro);
static _bool c_sleep(_comptr& result, const Token& word, const Tokens& tks, const _int& line, Uroboros& uro);
static _bool c_run(_comptr& result, const Token& word, const Tokens& tks, const _int& line, Uroboros& uro);
static _bool c_error(_comptr& result, const Token& word, const Tokens& tks, const _int& line, Uroboros& uro);

static void checkUselessFlags(const Token& word, const _int& line,
   const _bool& force, const _bool& stack, Uroboros& uro);
static void commandSyntaxException(const _str& name, const _int& line);
static void commandNoArgException(const _str& name, const _int& line);

}

#endif /* COM_PARSE_KW_H */

