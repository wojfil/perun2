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
#include "../terminator.h"


namespace uro::vars
{

Variables::Variables(uro::Uroboros& uro)
   : uroboros(uro), hashes(uro.hashes), vc(uro.vc), uroPath(os_uroborosPath()),
   inner(os_trim(uro.arguments.getLocation()), str(os_quoteEmbraced(this->uroPath), L" -s ")),
   boo ({
      { this->hashes.HASH_VAR_EMPTY, &this->inner.empty },
      { this->hashes.HASH_VAR_EXISTS, &this->inner.exists },
      { this->hashes.HASH_VAR_HIDDEN, &this->inner.hidden },
      { this->hashes.HASH_VAR_ISDIRECTORY, &this->inner.isdirectory },
      { this->hashes.HASH_VAR_ISFILE, &this->inner.isfile },
      { this->hashes.HASH_VAR_READONLY, &this->inner.readonly },
      { this->hashes.HASH_VAR_SUCCESS, &this->inner.success },
      { this->hashes.HASH_VAR_ARCHIVE, &this->inner.archive },
      { this->hashes.HASH_VAR_COMPRESSED, &this->inner.compressed },
      { this->hashes.HASH_VAR_ENCRYPTED, &this->inner.encrypted },
   }, { }, uro),
   per ({
      { this->hashes.HASH_VAR_LIFETIME, &this->inner.lifetime }
   }, { }, uro),
   tim ({
      { this->hashes.HASH_VAR_ACCESS, &this->inner.access },
      { this->hashes.HASH_VAR_CHANGE, &this->inner.change },
      { this->hashes.HASH_VAR_CREATION, &this->inner.creation },
      { this->hashes.HASH_VAR_MODIFICATION, &this->inner.modification }
   }, {
      { this->hashes.HASH_VAR_NOW, new gen::v_Now() },
      { this->hashes.HASH_VAR_TODAY, new gen::v_Today() },
      { this->hashes.HASH_VAR_YESTERDAY, new gen::v_Yesterday() },
      { this->hashes.HASH_VAR_TOMORROW, new gen::v_Tomorrow() }
   }, uro),
   num ({
      { this->hashes.HASH_VAR_SIZE, &this->inner.size }
   }, { }, uro),
   str_ ( {
      { this->hashes.HASH_VAR_DRIVE, &this->inner.drive },
      { this->hashes.HASH_VAR_EXTENSION, &this->inner.extension },
      { this->hashes.HASH_VAR_FULLNAME, &this->inner.fullname },
      { this->hashes.HASH_VAR_NAME, &this->inner.name },
      { this->hashes.HASH_VAR_PARENT, &this->inner.parent },
      { this->hashes.HASH_VAR_PATH, &this->inner.path }
   }, {
      { this->hashes.HASH_VAR_DESKTOP, new gen::Constant<_str>(os_desktopPath()) },
      { this->hashes.HASH_VAR_UROBOROS, new gen::Constant<_str>(this->uroPath) },
      { this->hashes.HASH_VAR_LOCATION, &this->inner.location }
   }, uro),
   nlist ( { }, { }, uro ),
   tlist ( { }, { }, uro ),
   list ( { },
   {
      { this->hashes.HASH_VAR_ALPHABET, new gen::Constant<_list>(inner.getAlphabet()) },
      { this->hashes.HASH_VAR_ASCII, new gen::Constant<_list>(inner.getAscii()) },
      { this->hashes.HASH_VAR_ARGUMENTS, new gen::Constant<_list>(uroboros.arguments.getArgs()) }
   }, uro),
   defGenerators({
      { this->hashes.HASH_VAR_DIRECTORIES, new gen::DefinitionGenerator(gen::OsElement::oe_Directories, uro) },
      { this->hashes.HASH_VAR_FILES, new gen::DefinitionGenerator(gen::OsElement::oe_Files, uro) },
      { this->hashes.HASH_VAR_RECURSIVEFILES, new gen::DefinitionGenerator(gen::OsElement::oe_RecursiveFiles, uro) },
      { this->hashes.HASH_VAR_RECURSIVEDIRECTORIES, new gen::DefinitionGenerator(gen::OsElement::oe_RecursiveDirectories, uro) }
   }),
   intVars({
      { this->hashes.HASH_VAR_INDEX, &this->inner.index },
      { this->hashes.HASH_VAR_DEPTH, &this->inner.depth }
   }) { };

void Variables::varsLevelUp()
{
   this->boo.levelUp();
   this->per.levelUp();
   this->tim.levelUp();
   this->num.levelUp();
   this->str_.levelUp();
   this->nlist.levelUp();
   this->tlist.levelUp();
   this->list.levelUp();
}

void Variables::varsLevelDown()
{
   this->boo.levelDown();
   this->per.levelDown();
   this->tim.levelDown();
   this->num.levelDown();
   this->str_.levelDown();
   this->nlist.levelDown();
   this->tlist.levelDown();
   this->list.levelDown();
}

void Variables::takeBundlePointer(VarBundle<_bool>*& bundle)
{
   bundle = &this->boo;
}

void Variables::takeBundlePointer(VarBundle<_per>*& bundle)
{
   bundle = &this->per;
}

void Variables::takeBundlePointer(VarBundle<_tim>*& bundle)
{
   bundle = &this->tim;
}

void Variables::takeBundlePointer(VarBundle<_num>*& bundle)
{
   bundle = &this->num;
}

void Variables::takeBundlePointer(VarBundle<_str>*& bundle)
{
   bundle = &this->str_;
}

void Variables::takeBundlePointer(VarBundle<_nlist>*& bundle)
{
   bundle = &this->nlist;
}

void Variables::takeBundlePointer(VarBundle<_tlist>*& bundle)
{
   bundle = &this->tlist;
}

void Variables::takeBundlePointer(VarBundle<_list>*& bundle)
{
   bundle = &this->list;
}

template <typename T>
_bool getVarValueIncludingThis(const Token& tk, Generator<T>*& result, const ThisState& thisState,
   Hashes& hashes, InnerVariables& inner, VariablesContext& vc, Variables& vars, uro::Uroboros& uro)
{
   if (tk.value.word.h == hashes.HASH_VAR_THIS) {
      if (inner.thisState == thisState) {
         inner.createThisRef(result);
         return true;
      }
      else if (inner.thisState == ThisState::ts_None) {
         vc.attributeException(tk, uro);
      }
      else {
         return false;
      }
   }

   VarBundle<T>* bundle;
   vars.takeBundlePointer(bundle);
   return bundle->getValue(vc, tk, result, inner);
}

_bool Variables::getVarValue(const Token& tk, Generator<_tim>*& result)
{
   return getVarValueIncludingThis(tk, result, ThisState::ts_Time,
      this->hashes, this->inner, this->vc, *this, this->uroboros);
}

_bool Variables::getVarValue(const Token& tk, Generator<_num>*& result)
{
   if (tk.value.word.h == this->hashes.HASH_VAR_INDEX) {
      if (this->uroboros.vars.inner.thisState == ThisState::ts_None && !this->vc.anyAggregate()) {
         throw SyntaxException(str(L"variable '", tk.getOriginString(this->uroboros),
            L"' can be accessed only inside a loop"), tk.line);
      }
      else {
         result = new gen::NumberIntRef(this->intVars[tk.value.word.h]);
         return true;
      }
   }
   else if (this->intVars.find(tk.value.word.h) != this->intVars.end()) {
      this->vc.setAttribute(tk, this->uroboros);
      result = new gen::NumberIntRef(this->intVars[tk.value.word.h]);
      return true;
   }

   return getVarValueIncludingThis(tk, result, ThisState::ts_Number,
      this->hashes, this->inner, this->vc, *this, this->uroboros);
}

_bool Variables::getVarValue(const Token& tk, Generator<_str>*& result)
{
   return getVarValueIncludingThis(tk, result, ThisState::ts_String,
      this->hashes, this->inner, this->vc, *this, this->uroboros);
}

_bool Variables::getVarValue(const Token& tk, _def*& result)
{
   if (this->defGenerators.find(tk.value.word.h) != this->defGenerators.end()) {
      result = this->defGenerators[tk.value.word.h]->generateDefault();
      return true;
   }

   return false;
}

// check if hash of variable exists in any hash map
_bool Variables::variableExists(const Token& tk)
{
   const _size& h = tk.value.word.h;

   return this->boo.hasVar(h)
       || this->per.hasVar(h)
       || this->tim.hasVar(h)
       || this->num.hasVar(h)
       || this->str_.hasVar(h)
       || this->nlist.hasVar(h)
       || this->tlist.hasVar(h)
       || this->list.hasVar(h)
       || this->defGenerators.find(h) != this->defGenerators.end()
       || this->intVars.find(h) != this->intVars.end();
}

void Variables::makeAllNotConstant()
{
   this->boo.makeNotConstant();
   this->per.makeNotConstant();
   this->tim.makeNotConstant();
   this->num.makeNotConstant();
   this->str_.makeNotConstant();
   this->nlist.makeNotConstant();
   this->tlist.makeNotConstant();
   this->list.makeNotConstant();
}

}
