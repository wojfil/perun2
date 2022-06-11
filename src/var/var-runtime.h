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

#ifndef VAR_RUNTIME_H
#define VAR_RUNTIME_H

#include <map>
#include "var-group.h"
#include "var.h"
#include "var-inner.h"
#include "../datatype/def-var.h"
#include "../datatype/def-gen.h"

// these maps contain hashes of variable names
// and references to them
// they are only needed for the parsing process
// runtime uses direct pointers

// user variables:
extern std::map<_size, ParseVariable<_boo>> g_var_boo;
extern std::map<_size, ParseVariable<_per>> g_var_per;
extern std::map<_size, ParseVariable<_tim>> g_var_tim;
extern std::map<_size, ParseVariable<_num>> g_var_num;
extern std::map<_size, ParseVariable<_str>> g_var_str;
extern std::map<_size, ParseVariable<_nlist>> g_var_nlist;
extern std::map<_size, ParseVariable<_tlist>> g_var_tlist;
extern std::map<_size, ParseVariable<_list>> g_var_list;

// internal variables (unmodifiable by user):
extern std::map<_size, Variable<_boo>*> g_ivar_boo;
extern std::map<_size, Variable<_per>*> g_ivar_per;
extern std::map<_size, Variable<_tim>*> g_ivar_tim;
extern std::map<_size, Variable<_num>*> g_ivar_num;
extern std::map<_size, Variable<_str>*> g_ivar_str;
extern std::map<_size, Variable<_nlist>*> g_ivar_nlist;
extern std::map<_size, Variable<_tlist>*> g_ivar_tlist;
extern std::map<_size, Variable<_list>*> g_ivar_list;

// special variables:
extern std::map<_size, Generator<_boo>*> g_svar_boo;
extern std::map<_size, Generator<_per>*> g_svar_per;
extern std::map<_size, Generator<_tim>*> g_svar_tim;
extern std::map<_size, Generator<_num>*> g_svar_num;
extern std::map<_size, Generator<_str>*> g_svar_str;
extern std::map<_size, Generator<_nlist>*> g_svar_nlist;
extern std::map<_size, Generator<_tlist>*> g_svar_tlist;
extern std::map<_size, DefinitionGenerator*> g_svar_def;
extern std::map<_size, Generator<_list>*> g_svar_list;

void initVars(const _list& args);
void finishVars();
void varsLevelUp();
void varsLevelDown();

GeneratorRef<_boo>* getVarValueBoo(const Token& tk);
GeneratorRef<_per>* getVarValuePer(const Token& tk);
GeneratorRef<_tim>* getVarValueTim(const Token& tk);
GeneratorRef<_num>* getVarValueNum(const Token& tk);
GeneratorRef<_str>* getVarValueStr(const Token& tk);
GeneratorRef<_nlist>* getVarValueNlist(const Token& tk);
GeneratorRef<_tlist>* getVarValueTlist(const Token& tk);
_def* getVarValueDef(const Token& tk);
GeneratorRef<_list>* getVarValueList(const Token& tk);

_boo variableExists(const Token& tk);

ParseVariable<_boo>* getVarPtrBoo(const Token& tk);
ParseVariable<_per>* getVarPtrPer(const Token& tk);
ParseVariable<_tim>* getVarPtrTim(const Token& tk);
ParseVariable<_num>* getVarPtrNum(const Token& tk);
ParseVariable<_str>* getVarPtrStr(const Token& tk);
ParseVariable<_nlist>* getVarPtrNlist(const Token& tk);
ParseVariable<_tlist>* getVarPtrTlist(const Token& tk);
ParseVariable<_list>* getVarPtrList(const Token& tk);




#endif /* VAR_RUNTIME_H */

