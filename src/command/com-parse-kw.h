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

#ifndef COM_PARSE_KW_H
#define COM_PARSE_KW_H


#include "../token.h"
#include "../tokens.h"
#include "com.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "../perun2.h"


namespace perun2::comm
{

p_bool keywordCommands(_comptr& result, const Token& word, Tokens& tks,
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);

static void checkFileContextExistence(const p_str& commandName, const p_int line, p_perun2& p2);

static p_bool kwCommandSimple(_comptr& result, const Token& word, Tokens& tks,
   const p_int line, p_perun2& p2);
static p_bool coreCommandSimple(_comptr& result, const Token& word, FileContext* context, const p_bool saveChanges,p_perun2& p2);
static p_bool kwCommandTime(_comptr& result, const Token& word, Tokens& tks, const p_int line, p_perun2& p2);
static p_bool coreCommandTime(_comptr& result, const Token& word, FileContext* context,
    _genptr<_tim>& time, const p_bool saveChanges, p_perun2& p2);

static p_bool c_open(_comptr& result, const Token& word, const Tokens& tks, const p_int line, p_perun2& p2);
static p_bool c_select(_comptr& result, const Token& word, const Tokens& tks, const p_int line, p_perun2& p2);
static p_bool c_rename(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);

static p_bool c_create(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_createFile(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_createDirectory(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_createFiles(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_createDirectories(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);

static p_bool c_moveTo(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_moveToContextless(_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_moveToAsContextless(_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_moveToContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_moveToAsContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);

static p_bool c_copy(_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_copySimple(_comptr& result, const Token& word, const Tokens& tks, const p_int line, p_perun2& p2);
static p_bool c_copyToContextless(_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_copyToAsContextless(_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_copyToContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);
static p_bool c_copyToAsContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const p_bool force, const p_bool stack, p_perun2& p2);

p_bool c_print(_comptr& result, const Token& word, const Tokens& tks, const p_int line, const p_bool directError, p_perun2& p2);
static p_bool c_sleep(_comptr& result, const Token& word, const Tokens& tks, const p_int line, p_perun2& p2);
static p_bool c_error(_comptr& result, const Token& word, const Tokens& tks, const p_int line, p_perun2& p2);

static p_bool c_run(_comptr& result, const Token& word, const Tokens& tks, const p_int line, p_perun2& p2);
static p_bool c_runContextless(_comptr& result, const Token& word, const Tokens& right, const p_int line, p_perun2& p2);
static p_bool c_runContextless_simple(_comptr& result, const Token& word, const Tokens& right, const p_int line, p_perun2& p2);
static p_bool c_runContextless_with(_comptr& result, const Token& word, const Tokens& right, const p_int line, p_perun2& p2);
static p_bool c_runContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, const p_int line, p_perun2& p2);
static p_bool c_runContextfull_simple(_comptr& result, const Token& word, const Tokens& left, 
   const Tokens& right, const p_int line, p_perun2& p2);
static p_bool c_runContextfull_with(_comptr& result, const Token& word, const Tokens& left, 
   const Tokens& right, const p_int line, p_perun2& p2);
   

static void checkUselessFlags(const Token& word, const p_int line,
   const p_bool force, const p_bool stack, p_perun2& p2);
static void commandSyntaxError(const p_str& name, const p_int line);
static void commandNoArgException(const p_str& name, const p_int line);

}

#endif /* COM_PARSE_KW_H */

