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



Command* keywordCommands(const Token& word, Tokens& tks,
   const _int& line, const bool& force, const bool& stack, Uroboros* uro);

static Command* kwCommandSimple(const Token& word, Tokens& tks,
   const _int& line, Uroboros* uro);
static Command* coreCommandSimpleSave(const Token& word, Uroboros* uro);
static Command* coreCommandSimpleNoSave(const Token& word, Uroboros* uro);
static Command* kwCommandTime(const Token& word, Tokens& tks,
   const _int& line, Uroboros* uro);
static Command* coreCommandTime(const Token& word,
   Generator<_tim>* time, const _boo& saveChanges, Uroboros* uro);

static Command* c_open(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro);
static Command* c_select(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro);
static Command* c_rename(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro);
static Command* c_create(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro);
static Command* c_moveTo(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro);
static Command* c_downloadFrom(const Token& word, const Tokens& tks, const _int& line,
    const bool& force, const bool& stack, Uroboros* uro);
static Command* c_copy(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro);

Command* c_print(const Token& word, const Tokens& tks, const _int& line, const _boo& directError, Uroboros* uro);
static Command* c_sleep(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro);
static Command* c_run(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro);
static Command* c_error(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro);
static Command* c_process(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro);

static void checkUselessFlags(const Token& word, const _int& line,
   const bool& force, const bool& stack);
static void commandSyntaxException(const _str& name, const _int& line);
static void commandNoArgException(const _str& name, const _int& line);



#endif /* COM_PARSE_KW_H */

