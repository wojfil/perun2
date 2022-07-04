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
#include "var.h"
#include "var-inner.h"
#include "../datatype/def-var.h"
#include "../datatype/def-gen.h"
#include "../hash.h"
#include "../token.h"


struct Uroboros;

struct Variables
{
public:

   Variables(Uroboros* uro);

   InnerVariables inner;

   // user variables:
   std::map<_size, ParseVariable<_boo>> var_boo;
   std::map<_size, ParseVariable<_per>> var_per;
   std::map<_size, ParseVariable<_tim>> var_tim;
   std::map<_size, ParseVariable<_num>> var_num;
   std::map<_size, ParseVariable<_str>> var_str;
   std::map<_size, ParseVariable<_nlist>> var_nlist;
   std::map<_size, ParseVariable<_tlist>> var_tlist;
   std::map<_size, ParseVariable<_list>> var_list;

   // internal variables (unmodifiable by user):
   std::map<_size, Variable<_boo>*> ivar_boo;
   std::map<_size, Variable<_per>*> ivar_per;
   std::map<_size, Variable<_tim>*> ivar_tim;
   std::map<_size, Variable<_num>*> ivar_num;
   std::map<_size, Variable<_str>*> ivar_str;
   std::map<_size, Variable<_nlist>*> ivar_nlist;
   std::map<_size, Variable<_tlist>*> ivar_tlist;
   std::map<_size, Variable<_list>*> ivar_list;

   // special variables:
   std::map<_size, Generator<_boo>*> svar_boo;
   std::map<_size, Generator<_per>*> svar_per;
   std::map<_size, Generator<_tim>*> svar_tim;
   std::map<_size, Generator<_num>*> svar_num;
   std::map<_size, Generator<_str>*> svar_str;
   std::map<_size, Generator<_nlist>*> svar_nlist;
   std::map<_size, Generator<_tlist>*> svar_tlist;
   std::map<_size, DefinitionGenerator*> svar_def;
   std::map<_size, Generator<_list>*> svar_list;

   void initVars(const _list& args);
   void varsLevelUp();
   void varsLevelDown();

   _boo getVarValue(const Token& tk, Generator<_boo>*& result);
   _boo getVarValue(const Token& tk, Generator<_per>*& result);
   _boo getVarValue(const Token& tk, Generator<_tim>*& result);
   _boo getVarValue(const Token& tk, Generator<_num>*& result);
   _boo getVarValue(const Token& tk, Generator<_str>*& result);
   _boo getVarValue(const Token& tk, Generator<_nlist>*& result);
   _boo getVarValue(const Token& tk, Generator<_tlist>*& result);
   _boo getVarValue(const Token& tk, _def*& result);
   _boo getVarValue(const Token& tk, Generator<_list>*& result);

   _boo variableExists(const Token& tk);

   _boo getVarPtr(const Token& tk, ParseVariable<_boo>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_per>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_tim>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_num>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_str>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_nlist>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_tlist>*& result);
   _boo getVarPtr(const Token& tk, ParseVariable<_list>*& result);

private:
   Uroboros* uroboros;
   Hashes* hashes;

};

#endif /* VAR_RUNTIME_H */
