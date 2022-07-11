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
#include "../datatype/gen-ref.h"
#include "../exception.h"
#include "../util.h"
#include "../os.h"
#include "../uroboros.h"


Variables::Variables(Uroboros* uro)
{
   this->uroboros = uro;
   this->hashes = &uro->hashes;
   inner = InnerVariables();
   inner.depth.value.setToZero();
   inner.location.value = os_trim(uro->arguments.getLocation());

   this->ivar_boo =
   {
      { this->hashes->HASH_VAR_EMPTY, &inner.empty },
      { this->hashes->HASH_VAR_EXISTS, &inner.exists },
      { this->hashes->HASH_VAR_HIDDEN, &inner.hidden },
      { this->hashes->HASH_VAR_ISDIRECTORY, &inner.isdirectory },
      { this->hashes->HASH_VAR_ISFILE, &inner.isfile },
      { this->hashes->HASH_VAR_READONLY, &inner.readonly },
      { this->hashes->HASH_VAR_SUCCESS, &inner.success },
      { this->hashes->HASH_VAR_ARCHIVE, &inner.archive },
      { this->hashes->HASH_VAR_COMPRESSED, &inner.compressed },
      { this->hashes->HASH_VAR_ENCRYPTED, &inner.encrypted },
   };

   this->ivar_per =
   {
      { this->hashes->HASH_VAR_LIFETIME, &inner.lifetime }
   };

   this->ivar_tim =
   {
      { this->hashes->HASH_VAR_ACCESS, &inner.access },
      { this->hashes->HASH_VAR_CHANGE, &inner.change },
      { this->hashes->HASH_VAR_CREATION, &inner.creation },
      { this->hashes->HASH_VAR_MODIFICATION, &inner.modification }
   };

   this->svar_tim =
   {
      { this->hashes->HASH_VAR_NOW, new v_Now() },
      { this->hashes->HASH_VAR_TODAY, new v_Today() },
      { this->hashes->HASH_VAR_YESTERDAY, new v_Yesterday() },
      { this->hashes->HASH_VAR_TOMORROW, new v_Tomorrow() }
   };

   this->ivar_num =
   {
      { this->hashes->HASH_VAR_SIZE, &inner.size },
   };

   this->ivar_numi =
   {
      { this->hashes->HASH_VAR_INDEX, &inner.index },
      { this->hashes->HASH_VAR_DEPTH, &inner.depth }
   };

   this->svar_num = { };

   this->svar_list =
   {
      { this->hashes->HASH_VAR_ALPHABET, new Constant<_list>(inner.getAlphabet()) },
      { this->hashes->HASH_VAR_ASCII, new Constant<_list>(inner.getAscii()) },
      { this->hashes->HASH_VAR_ARGUMENTS, new Constant<_list>(uroboros->arguments.getArgs()) }
   };

   const _str uroPath = os_uroborosPath();
   inner.urocom = str(os_quoteEmbraced(uroPath), L" -s ");
   const _str desktopPath = os_desktopPath();

   this->svar_str =
   {
      { this->hashes->HASH_VAR_DESKTOP, new Constant<_str>(desktopPath) },
      { this->hashes->HASH_VAR_UROBOROS, new Constant<_str>(uroPath) }
   };

   this->ivar_str =
   {
      { this->hashes->HASH_VAR_DRIVE, &inner.drive },
      { this->hashes->HASH_VAR_EXTENSION, &inner.extension },
      { this->hashes->HASH_VAR_FULLNAME, &inner.fullname },
      { this->hashes->HASH_VAR_LOCATION, &inner.location },
      { this->hashes->HASH_VAR_NAME, &inner.name },
      { this->hashes->HASH_VAR_PARENT, &inner.parent },
      { this->hashes->HASH_VAR_PATH, &inner.path }
   };

   this->svar_def =
   {
      { this->hashes->HASH_VAR_DIRECTORIES, new Gen_ElementsAtLocation(ELEM_DIRECTORIES, uro) },
      { this->hashes->HASH_VAR_FILES, new Gen_ElementsAtLocation(ELEM_FILES, uro) },
      { this->hashes->HASH_VAR_RECURSIVEFILES, new Gen_RecursiveFiles(uro) },
      { this->hashes->HASH_VAR_RECURSIVEDIRECTORIES, new Gen_RecursiveDirectories(uro) }
   };
}


void Variables::varsLevelUp()
{
   for (auto& kv : this->var_boo) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_per) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_tim) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_num) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_str) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_tlist) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_nlist) {
      kv.second.bracketsUp();
   }
   for (auto& kv : this->var_list) {
      kv.second.bracketsUp();
   }
}

void Variables::varsLevelDown()
{
   for (auto& kv : this->var_boo) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_per) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_tim) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_num) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_str) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_tlist) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_nlist) {
      kv.second.bracketsDown();
   }
   for (auto& kv : this->var_list) {
      kv.second.bracketsDown();
   }
}

_boo Variables::getVarValue(const Token& tk, Generator<_boo>*& result)
{
   if (this->var_boo.find(tk.value.word.h) != this->var_boo.end()) {
      ParseVariable<_boo>* pv = &this->var_boo[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_boo>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_boo.find(tk.value.word.h) != this->ivar_boo.end()) {
      this->uroboros->vc.setAttribute(tk);
      result = new GeneratorRef<_boo>(this->ivar_boo[tk.value.word.h]);
      return true;
   }
   else if (this->svar_boo.find(tk.value.word.h) != this->svar_boo.end()) {
      result = new GeneratorRef<_boo>(this->svar_boo[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_per>*& result)
{
   if (this->var_per.find(tk.value.word.h) != this->var_per.end()) {
      ParseVariable<_per>* pv = &this->var_per[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_per>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_per.find(tk.value.word.h) != this->ivar_per.end()) {
      this->uroboros->vc.setAttribute(tk);
      result = new GeneratorRef<_per>(this->ivar_per[tk.value.word.h]);
      return true;
   }
   else if (this->svar_per.find(tk.value.word.h) != this->svar_per.end()) {
      result = new GeneratorRef<_per>(this->svar_per[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_tim>*& result)
{
   if (tk.value.word.h == this->hashes->HASH_VAR_THIS) {
      if (this->inner.thisState == ThisState::ts_Time) {
         result = new GeneratorRef<_tim>(&this->inner.this_t);
         return true;
      }
      else if (this->inner.thisState == ThisState::ts_None)
         this->uroboros->vc.attributeException(tk);
      else
         return false;
   }

   if (this->var_tim.find(tk.value.word.h) != this->var_tim.end()) {
      ParseVariable<_tim>* pv = &this->var_tim[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_tim>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_tim.find(tk.value.word.h) != this->ivar_tim.end()) {
      this->uroboros->vc.setAttribute(tk);
      result = new GeneratorRef<_tim>(this->ivar_tim[tk.value.word.h]);
      return true;
   }
   else if (this->svar_tim.find(tk.value.word.h) != this->svar_tim.end()) {
      result = new GeneratorRef<_tim>(this->svar_tim[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_num>*& result)
{
   if (tk.value.word.h == this->hashes->HASH_VAR_THIS) {
      if (this->inner.thisState == ThisState::ts_Number) {
         result = new GeneratorRef<_num>(&this->inner.this_n);
         return true;
      }
      else if (this->inner.thisState == ThisState::ts_None)
         this->uroboros->vc.attributeException(tk);
      else
         return false;
   }

   if (this->var_num.find(tk.value.word.h) != this->var_num.end()) {
      ParseVariable<_num>* pv = &this->var_num[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_num>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_numi.find(tk.value.word.h) != this->ivar_numi.end()) {
      this->uroboros->vc.setAttribute(tk);
      result = new NumberIntRef(this->ivar_numi[tk.value.word.h]);
      return true;
   }
   else if (this->ivar_num.find(tk.value.word.h) != this->ivar_num.end()) {
      this->uroboros->vc.setAttribute(tk);
      result = new GeneratorRef<_num>(this->ivar_num[tk.value.word.h]);
      return true;
   }
   else if (this->svar_num.find(tk.value.word.h) != this->svar_num.end()) {
      result = new GeneratorRef<_num>(this->svar_num[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_str>*& result)
{
   if (tk.value.word.h == this->hashes->HASH_VAR_THIS) {
      if (this->inner.thisState == ThisState::ts_String) {
         result = new GeneratorRef<_str>(&this->inner.this_s);
         return true;
      }
      else if (this->inner.thisState == ThisState::ts_None)
         this->uroboros->vc.attributeException(tk);
      else
         return false;
   }

   if (this->var_str.find(tk.value.word.h) != this->var_str.end()) {
      ParseVariable<_str>* pv = &this->var_str[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_str>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_str.find(tk.value.word.h) != this->ivar_str.end()) {
      this->uroboros->vc.setAttribute(tk);
      result = new GeneratorRef<_str>(this->ivar_str[tk.value.word.h]);
      return true;
   }
   else if (this->svar_str.find(tk.value.word.h) != this->svar_str.end()) {
      result = new GeneratorRef<_str>(this->svar_str[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_nlist>*& result)
{
   if (this->var_nlist.find(tk.value.word.h) != this->var_nlist.end()) {
      ParseVariable<_nlist>* pv = &this->var_nlist[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_nlist>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_nlist.find(tk.value.word.h) != this->ivar_nlist.end()) {
      result = new GeneratorRef<_nlist>(this->ivar_nlist[tk.value.word.h]);
      return true;
   }
   else if (this->svar_nlist.find(tk.value.word.h) != this->svar_nlist.end()) {
      result = new GeneratorRef<_nlist>(this->svar_nlist[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_tlist>*& result)
{
   if (this->var_tlist.find(tk.value.word.h) != this->var_tlist.end()) {
      ParseVariable<_tlist>* pv = &this->var_tlist[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_tlist>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_tlist.find(tk.value.word.h) != this->ivar_tlist.end()) {
      result = new GeneratorRef<_tlist>(this->ivar_tlist[tk.value.word.h]);
      return true;
   }
   else if (this->svar_tlist.find(tk.value.word.h) != this->svar_tlist.end()) {
      result = new GeneratorRef<_tlist>(this->svar_tlist[tk.value.word.h]);
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, _def*& result)
{
   if (this->svar_def.find(tk.value.word.h) != this->svar_def.end()) {
      result = this->svar_def[tk.value.word.h]->generate(new LocationReference(this->uroboros));
      return true;
   }

   return false;
}

_boo Variables::getVarValue(const Token& tk, Generator<_list>*& result)
{
   if (this->var_list.find(tk.value.word.h) != this->var_list.end()) {
      ParseVariable<_list>* pv = &this->var_list[tk.value.word.h];
      if (pv->isReachable()) {
         result = new GeneratorRef<_list>(pv->getVarPtr());
         return true;
      }
   }
   else if (this->ivar_list.find(tk.value.word.h) != this->ivar_list.end()) {
      result = new GeneratorRef<_list>(this->ivar_list[tk.value.word.h]);
      return true;
   }
   else if (this->svar_list.find(tk.value.word.h) != this->svar_list.end()) {
      result = new GeneratorRef<_list>(this->svar_list[tk.value.word.h]);
      return true;
   }

   return false;
}


// check if hash of variable exists in any hash map
_boo Variables::variableExists(const Token& tk)
{
   const _size& h = tk.value.word.h;

   return this->ivar_boo.find(h) != this->ivar_boo.end()
       || this->svar_boo.find(h) != this->svar_boo.end()
       || this->var_boo.find(h) != this->var_boo.end()

       || this->ivar_per.find(h) != this->ivar_per.end()
       || this->svar_per.find(h) != this->svar_per.end()
       || this->var_per.find(h) != this->var_per.end()

       || this->ivar_tim.find(h) != this->ivar_tim.end()
       || this->svar_tim.find(h) != this->svar_tim.end()
       || this->var_tim.find(h) != this->var_tim.end()

       || this->ivar_num.find(h) != this->ivar_num.end()
       || this->ivar_numi.find(h) != this->ivar_numi.end()
       || this->svar_num.find(h) != this->svar_num.end()
       || this->var_num.find(h) != this->var_num.end()

       || this->ivar_str.find(h) != this->ivar_str.end()
       || this->svar_str.find(h) != this->svar_str.end()
       || this->var_str.find(h) != this->var_str.end()

       || this->ivar_nlist.find(h) != this->ivar_nlist.end()
       || this->svar_nlist.find(h) != this->svar_nlist.end()
       || this->var_nlist.find(h) != this->var_nlist.end()

       || this->ivar_tlist.find(h) != this->ivar_tlist.end()
       || this->svar_tlist.find(h) != this->svar_tlist.end()
       || this->var_tlist.find(h) != this->var_tlist.end()

       || this->svar_def.find(h) != this->svar_def.end()

       || this->ivar_list.find(h) != this->ivar_list.end()
       || this->svar_list.find(h) != this->svar_list.end()
       || this->var_list.find(h) != this->var_list.end();
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_boo>*& result)
{
   if (this->var_boo.find(tk.value.word.h) == this->var_boo.end()) {
      return false;
   }
   else {
      result = &(this->var_boo.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_per>*& result)
{
   if (this->var_per.find(tk.value.word.h) == this->var_per.end()) {
      return false;
   }
   else {
      result = &(this->var_per.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_tim>*& result)
{
   if (this->var_tim.find(tk.value.word.h) == this->var_tim.end()) {
      return false;
   }
   else {
      result = &(this->var_tim.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_num>*& result)
{
   if (this->var_num.find(tk.value.word.h) == this->var_num.end()) {
      return false;
   }
   else {
      result = &(this->var_num.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_str>*& result)
{
   if (this->var_str.find(tk.value.word.h) == this->var_str.end()) {
      return false;
   }
   else {
      result = &(this->var_str.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_nlist>*& result)
{
   if (this->var_nlist.find(tk.value.word.h) == this->var_nlist.end()) {
      return false;
   }
   else {
      result = &(this->var_nlist.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_tlist>*& result)
{
   if (this->var_tlist.find(tk.value.word.h) == this->var_tlist.end()) {
      return false;
   }
   else {
      result = &(this->var_tlist.find(tk.value.word.h)->second);
      return true;
   }
}

_boo Variables::getVarPtr(const Token& tk, ParseVariable<_list>*& result)
{
   if (this->var_list.find(tk.value.word.h) == this->var_list.end()) {
      return false;
   }
   else {
      result = &(this->var_list.find(tk.value.word.h)->second);
      return true;
   }
}
