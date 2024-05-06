/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
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


enum CoreCommandMode
{
   ccm_Normal = 0,
   ccm_Force,
   ccm_Stack
};


p_bool keywordCommands(p_comptr& result, const Token& word, Tokens& tks,
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);

static void checkFileContextExistence(const p_str& commandName, const p_int line, Perun2Process& p2);

static p_bool kwCommandSimple(p_comptr& result, const Token& word, Tokens& tks,
   const p_int line, Perun2Process& p2);
static p_bool coreCommandSimple(p_comptr& result, const Token& word, FileContext* context, const p_bool saveChanges,Perun2Process& p2);
static p_bool kwCommandTime(p_comptr& result, const Token& word, Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool coreCommandTime(p_comptr& result, const Token& word, FileContext* context,
    p_genptr<p_tim>& time, const p_bool saveChanges, Perun2Process& p2);

static p_bool c_open(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool c_select(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool c_rename(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);

static p_bool c_create(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_createFile(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_createDirectory(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_createFiles(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_createDirectories(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);

static p_bool c_moveTo(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_moveToContextless(p_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_moveToAsContextless(p_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_moveToContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_moveToAsContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);

static p_bool c_copy(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_copySimple(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool c_copyToContextless(p_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_copyToAsContextless(p_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_copyToContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);
static p_bool c_copyToAsContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2);

p_bool c_print(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, const p_bool directError, Perun2Process& p2);
static p_bool c_sleep(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool c_popup(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool c_error(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);

static p_bool c_run(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2);
static p_bool c_runContextless(p_comptr& result, const Token& word, const Tokens& right, const p_int line, Perun2Process& p2);
static p_bool c_runContextless_simple(p_comptr& result, const Token& word, const Tokens& right, const p_int line, Perun2Process& p2);
static p_bool c_runContextless_with(p_comptr& result, const Token& word, const Tokens& right, const p_int line, Perun2Process& p2);
static p_bool c_runContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, const p_int line, Perun2Process& p2);
static p_bool c_runContextfull_simple(p_comptr& result, const Token& word, const Tokens& left, 
   const Tokens& right, const p_int line, Perun2Process& p2);
static p_bool c_runContextfull_with(p_comptr& result, const Token& word, const Tokens& left, 
   const Tokens& right, const p_int line, Perun2Process& p2);
   

static void checkUselessFlags(const Token& word, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2);
static void commandSyntaxError(const p_str& name, const p_int line);
static void commandNoArgException(const p_str& name, const p_int line);

}

#endif /* COM_PARSE_KW_H */

