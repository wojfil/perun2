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

#include "var-runtime.h"
#include "../datatype/generator/gen-generic.h"
#include "../datatype/generator/gen-time.h"
#include "../datatype/generator/gen-definition.h"
#include "../datatype/generator/gen-string.h"
#include "../exception.h"
#include "../util.h"
#include "../os.h"
#include "../hash.h"


// user variables:
std::map<_size, ParseVariable<_boo>> g_var_boo;
std::map<_size, ParseVariable<_per>> g_var_per;
std::map<_size, ParseVariable<_tim>> g_var_tim;
std::map<_size, ParseVariable<_num>> g_var_num;
std::map<_size, ParseVariable<_str>> g_var_str;
std::map<_size, ParseVariable<_nlist>> g_var_nlist;
std::map<_size, ParseVariable<_tlist>> g_var_tlist;
std::map<_size, ParseVariable<_list>> g_var_list;

// internal variables (unmodifiable by user):
std::map<_size, Variable<_boo>*> g_ivar_boo;
std::map<_size, Variable<_per>*> g_ivar_per;
std::map<_size, Variable<_tim>*> g_ivar_tim;
std::map<_size, Variable<_num>*> g_ivar_num;
std::map<_size, Variable<_str>*> g_ivar_str;
std::map<_size, Variable<_nlist>*> g_ivar_nlist;
std::map<_size, Variable<_tlist>*> g_ivar_tlist;
std::map<_size, Variable<_list>*> g_ivar_list;

// special variables:
std::map<_size, Generator<_boo>*> g_svar_boo;
std::map<_size, Generator<_per>*> g_svar_per;
std::map<_size, Generator<_tim>*> g_svar_tim;
std::map<_size, Generator<_num>*> g_svar_num;
std::map<_size, Generator<_str>*> g_svar_str;
std::map<_size, Generator<_nlist>*> g_svar_nlist;
std::map<_size, Generator<_tlist>*> g_svar_tlist;
std::map<_size, DefinitionGenerator*> g_svar_def;
std::map<_size, Generator<_list>*> g_svar_list;




void initVars(const _list& args)
{
   g_depth.value = _num(0LL);

   g_ivar_boo =
   {
      { HASH_VAR_EMPTY, &g_empty },
      { HASH_VAR_EXISTS, &g_exists },
      { HASH_VAR_HIDDEN, &g_hidden },
      { HASH_VAR_ISDIRECTORY, &g_isdirectory },
      { HASH_VAR_ISFILE, &g_isfile },
      { HASH_VAR_READONLY, &g_readonly },
      { HASH_VAR_SUCCESS, &g_success },
      { HASH_VAR_ARCHIVE, &g_archive },
      { HASH_VAR_COMPRESSED, &g_compressed },
      { HASH_VAR_ENCRYPTED, &g_encrypted },
   };

   g_ivar_per =
   {
      { HASH_VAR_LIFETIME, &g_lifetime }
   };

   g_ivar_tim =
   {
      { HASH_VAR_ACCESS, &g_access },
      { HASH_VAR_CHANGE, &g_change },
      { HASH_VAR_CREATION, &g_creation },
      { HASH_VAR_MODIFICATION, &g_modification }
   };

   g_svar_tim =
   {
      { HASH_VAR_NOW, new v_Now() },
      { HASH_VAR_TODAY, new v_Today() },
      { HASH_VAR_YESTERDAY, new v_Yesterday() },
      { HASH_VAR_TOMORROW, new v_Tomorrow() }
   };

   g_ivar_num =
   {
      { HASH_VAR_SIZE, &g_size },
      { HASH_VAR_INDEX, &g_index },
      { HASH_VAR_DEPTH, &g_depth }
   };

   g_svar_num = { };

   g_svar_list =
   {
      { HASH_VAR_ALPHABET, new Constant<_list>(vinit_getAlphabet()) },
      { HASH_VAR_ASCII, new Constant<_list>(vinit_getAscii()) },
      { HASH_VAR_ARGUMENTS, new Constant<_list>(args) }
   };

   const _str uroPath = os_uroborosPath();
   g_urocom = os_quoteEmbraced(uroPath) + L" -s ";
   const _str desktopPath = os_desktopPath();

   g_svar_str =
   {
      { HASH_VAR_DESKTOP, new Constant<_str>(desktopPath) },
      { HASH_VAR_UROBOROS, new Constant<_str>(uroPath) }
   };

   g_ivar_str =
   {
      { HASH_VAR_DRIVE, &g_drive },
      { HASH_VAR_EXTENSION, &g_extension },
      { HASH_VAR_FULLNAME, &g_fullname },
      { HASH_VAR_LOCATION, &g_location },
      { HASH_VAR_NAME, &g_name },
      { HASH_VAR_PARENT, &g_parent },
      { HASH_VAR_PATH, &g_path }
   };

   g_svar_def =
   {
      { HASH_VAR_ALL, new Gen_ElementsAtLocation(ELEM_ALL) },
      { HASH_VAR_DIRECTORIES, new Gen_ElementsAtLocation(ELEM_DIRECTORIES) },
      { HASH_VAR_FILES, new Gen_ElementsAtLocation(ELEM_FILES) },
      { HASH_VAR_RECURSIVEFILES, new Gen_RecursiveFiles() },
      { HASH_VAR_RECURSIVEDIRECTORIES, new Gen_RecursiveDirectories() }
   };
}


void varsLevelUp()
{
   for (auto& kv : g_var_boo) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_per) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_tim) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_num) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_str) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_tlist) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_nlist) {
      kv.second.bracketsUp();
   }
   for (auto& kv : g_var_list) {
      kv.second.bracketsUp();
   }
}

void varsLevelDown()
{
   for (auto& kv : g_var_boo) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_per) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_tim) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_num) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_str) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_tlist) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_nlist) {
      kv.second.bracketsDown();
   }
   for (auto& kv : g_var_list) {
      kv.second.bracketsDown();
   }
}

_boo getVarValue(const Token& tk, Generator<_boo>*& result)
{
   if (g_var_boo.find(tk.value.h1) != g_var_boo.end()) {
      ParseVariable<_boo>* pv = &g_var_boo[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_boo>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_boo.find(tk.value.h1) != g_ivar_boo.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_boo>(g_ivar_boo[tk.value.h1]);
      return true;
   }
   else if (g_svar_boo.find(tk.value.h1) != g_svar_boo.end()) {
      result = new GeneratorRef<_boo>(g_svar_boo[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_per>*& result)
{
   if (g_var_per.find(tk.value.h1) != g_var_per.end()) {
      ParseVariable<_per>* pv = &g_var_per[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_per>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_per.find(tk.value.h1) != g_ivar_per.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_per>(g_ivar_per[tk.value.h1]);
      return true;
   }
   else if (g_svar_per.find(tk.value.h1) != g_svar_per.end()) {
      result = new GeneratorRef<_per>(g_svar_per[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_tim>*& result)
{
   if (tk.value.h1 == HASH_VAR_THIS) {
      if (g_thisstate == ThisState::ts_Time) {
         result = new GeneratorRef<_tim>(&g_this_t);
         return true;
      }
      else if (g_thisstate == ThisState::ts_None)
         attributeException(tk);
      else
         return false;
   }

   if (g_var_tim.find(tk.value.h1) != g_var_tim.end()) {
      ParseVariable<_tim>* pv = &g_var_tim[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_tim>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_tim.find(tk.value.h1) != g_ivar_tim.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_tim>(g_ivar_tim[tk.value.h1]);
      return true;
   }
   else if (g_svar_tim.find(tk.value.h1) != g_svar_tim.end()) {
      result = new GeneratorRef<_tim>(g_svar_tim[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_num>*& result)
{
   if (tk.value.h1 == HASH_VAR_THIS) {
      if (g_thisstate == ThisState::ts_Number) {
         result = new GeneratorRef<_num>(&g_this_n);
         return true;
      }
      else if (g_thisstate == ThisState::ts_None)
         attributeException(tk);
      else
         return false;
   }

   if (g_var_num.find(tk.value.h1) != g_var_num.end()) {
      ParseVariable<_num>* pv = &g_var_num[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_num>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_num.find(tk.value.h1) != g_ivar_num.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_num>(g_ivar_num[tk.value.h1]);
      return true;
   }
   else if (g_svar_num.find(tk.value.h1) != g_svar_num.end()) {
      result = new GeneratorRef<_num>(g_svar_num[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_str>*& result)
{
   if (tk.value.h1 == HASH_VAR_THIS) {
      if (g_thisstate == ThisState::ts_String) {
         result = new GeneratorRef<_str>(&g_this_s);
         return true;
      }
      else if (g_thisstate == ThisState::ts_None)
         attributeException(tk);
      else
         return false;
   }

   if (g_var_str.find(tk.value.h1) != g_var_str.end()) {
      ParseVariable<_str>* pv = &g_var_str[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_str>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_str.find(tk.value.h1) != g_ivar_str.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_str>(g_ivar_str[tk.value.h1]);
      return true;
   }
   else if (g_svar_str.find(tk.value.h1) != g_svar_str.end()) {
      result = new GeneratorRef<_str>(g_svar_str[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_nlist>*& result)
{
   if (g_var_nlist.find(tk.value.h1) != g_var_nlist.end()) {
      ParseVariable<_nlist>* pv = &g_var_nlist[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_nlist>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_nlist.find(tk.value.h1) != g_ivar_nlist.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_nlist>(g_ivar_nlist[tk.value.h1]);
      return true;
   }
   else if (g_svar_nlist.find(tk.value.h1) != g_svar_nlist.end()) {
      result = new GeneratorRef<_nlist>(g_svar_nlist[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_tlist>*& result)
{
   if (g_var_tlist.find(tk.value.h1) != g_var_tlist.end()) {
      ParseVariable<_tlist>* pv = &g_var_tlist[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_tlist>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_tlist.find(tk.value.h1) != g_ivar_tlist.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_tlist>(g_ivar_tlist[tk.value.h1]);
      return true;
   }
   else if (g_svar_tlist.find(tk.value.h1) != g_svar_tlist.end()) {
      result = new GeneratorRef<_tlist>(g_svar_tlist[tk.value.h1]);
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, _def*& result)
{
   if (g_svar_def.find(tk.value.h1) != g_svar_def.end()) {
      result = g_svar_def[tk.value.h1]->generate(new LocationReference());
      return true;
   }

   return false;
}

_boo getVarValue(const Token& tk, Generator<_list>*& result)
{
   if (g_var_list.find(tk.value.h1) != g_var_list.end()) {
      ParseVariable<_list>* pv = &g_var_list[tk.value.h1];
      if (pv->isReachable()) {
         result = new GeneratorRef<_list>(pv->getVarPtr());
         return true;
      }
   }
   else if (g_ivar_list.find(tk.value.h1) != g_ivar_list.end()) {
      setAttribute(tk);
      result = new GeneratorRef<_list>(g_ivar_list[tk.value.h1]);
      return true;
   }
   else if (g_svar_list.find(tk.value.h1) != g_svar_list.end()) {
      result = new GeneratorRef<_list>(g_svar_list[tk.value.h1]);
      return true;
   }

   return false;
}


// check if hash of variable exists in any hash map
_boo variableExists(const Token& tk)
{
   const _size& h = tk.value.h1;

   return g_ivar_boo.find(h) != g_ivar_boo.end()
       || g_svar_boo.find(h) != g_svar_boo.end()
       || g_var_boo.find(h) != g_var_boo.end()

       || g_ivar_per.find(h) != g_ivar_per.end()
       || g_svar_per.find(h) != g_svar_per.end()
       || g_var_per.find(h) != g_var_per.end()

       || g_ivar_tim.find(h) != g_ivar_tim.end()
       || g_svar_tim.find(h) != g_svar_tim.end()
       || g_var_tim.find(h) != g_var_tim.end()

       || g_ivar_num.find(h) != g_ivar_num.end()
       || g_svar_num.find(h) != g_svar_num.end()
       || g_var_num.find(h) != g_var_num.end()

       || g_ivar_str.find(h) != g_ivar_str.end()
       || g_svar_str.find(h) != g_svar_str.end()
       || g_var_str.find(h) != g_var_str.end()

       || g_ivar_nlist.find(h) != g_ivar_nlist.end()
       || g_svar_nlist.find(h) != g_svar_nlist.end()
       || g_var_nlist.find(h) != g_var_nlist.end()

       || g_ivar_tlist.find(h) != g_ivar_tlist.end()
       || g_svar_tlist.find(h) != g_svar_tlist.end()
       || g_var_tlist.find(h) != g_var_tlist.end()

       || g_svar_def.find(h) != g_svar_def.end()

       || g_ivar_list.find(h) != g_ivar_list.end()
       || g_svar_list.find(h) != g_svar_list.end()
       || g_var_list.find(h) != g_var_list.end();
}

_boo getVarPtr(const Token& tk, ParseVariable<_boo>*& result)
{
   if (g_var_boo.find(tk.value.h1) == g_var_boo.end()) {
      return false;
   }
   else {
      result = &(g_var_boo.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_per>*& result)
{
   if (g_var_per.find(tk.value.h1) == g_var_per.end()) {
      return false;
   }
   else {
      result = &(g_var_per.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_tim>*& result)
{
   if (g_var_tim.find(tk.value.h1) == g_var_tim.end()) {
      return false;
   }
   else {
      result = &(g_var_tim.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_num>*& result)
{
   if (g_var_num.find(tk.value.h1) == g_var_num.end()) {
      return false;
   }
   else {
      result = &(g_var_num.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_str>*& result)
{
   if (g_var_str.find(tk.value.h1) == g_var_str.end()) {
      return false;
   }
   else {
      result = &(g_var_str.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_nlist>*& result)
{
   if (g_var_nlist.find(tk.value.h1) == g_var_nlist.end()) {
      return false;
   }
   else {
      result = &(g_var_nlist.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_tlist>*& result)
{
   if (g_var_tlist.find(tk.value.h1) == g_var_tlist.end()) {
      return false;
   }
   else {
      result = &(g_var_tlist.find(tk.value.h1)->second);
      return true;
   }
}

_boo getVarPtr(const Token& tk, ParseVariable<_list>*& result)
{
   if (g_var_list.find(tk.value.h1) == g_var_list.end()) {
      return false;
   }
   else {
      result = &(g_var_list.find(tk.value.h1)->second);
      return true;
   }
}
