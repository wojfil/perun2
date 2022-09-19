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

#include "com-parse-kw.h"
#include "com-misc.h"
#include "com-core.h"
#include "../exception.h"
#include "../lexer.h"
#include "../datatype/parse-gen.h"
#include "com-struct.h"
#include "com-arg.h"
#include "com-core-aggr.h"
#include "com-create.h"
#include "../datatype/generator/gen-string.h"
#include "../hash.h"


namespace uro::comm
{

Command* keywordCommands(const Token& word, Tokens& tks,
   const _int& line, const bool& force, const bool& stack, uro::Uroboros* uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Unlock:
      case Keyword::kw_Unhide: {
         checkUselessFlags(word, line, force, stack, uro);
         return kwCommandSimple(word, tks, line, uro);
      }
      case Keyword::kw_Open: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_open(word, tks, line, uro);
      }
      case Keyword::kw_Copy: {
         return c_copy(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Create: {
         return c_create(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Download: {
         return c_downloadFrom(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Break:
      case Keyword::kw_Continue:
      case Keyword::kw_Exit: {
         checkUselessFlags(word, line, force, stack, uro);
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L"' cannot be called with an argument"), line);
      }
      case Keyword::kw_Error: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_error(word, tks, line, uro);
      }
      case Keyword::kw_Move: {
         return c_moveTo(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Print: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_print(word, tks, line, true, uro);
      }
      case Keyword::kw_Reaccess:
      case Keyword::kw_Recreate:
      case Keyword::kw_Rechange:
      case Keyword::kw_Remodify: {
         checkUselessFlags(word, line, force, stack, uro);
         return kwCommandTime(word, tks, line, uro);
      }
      case Keyword::kw_Rename: {
         return c_rename(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Run: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_run(word, tks, line, uro);
      }
      case Keyword::kw_Select: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_select(word, tks, line, uro);
      }
      case Keyword::kw_Sleep: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_sleep(word, tks, line, uro);
      }
   }

   throw SyntaxException(str(L"command cannot start with a keyword '", word.getOriginString(uro), L"'"), line);
}

_boo parseLooped(const Tokens& tks, Command* innerCommand, Command*& result, uro::Uroboros* uro,
   Attribute* attr, const _boo& hasMemory)
{
   Generator<_str>* str_;
   if (parse::parse(uro, tks, str_)) {
      result = new CS_StringComArg(str_, innerCommand, attr, hasMemory, uro);
      return true;
   }

   _def* def;
   if (parse::parse(uro, tks, def)) {
      _fdata* fdata = def->getDataPtr();

      if (fdata == nullptr) {
         result = new CS_DefinitionComArg(def, innerCommand, attr, hasMemory, uro);
      }
      else {
         const _aunit aval = attr->getValue();
         delete attr;
         result = new CS_DefinitionComArg(def, innerCommand,
            new BridgeAttribute(aval, uro, fdata), hasMemory, uro);
      }

      return true;
   }

   Generator<_list>* list;
   if (parse::parse(uro, tks, list)) {
      result = new CS_ListComArg(list, innerCommand, attr, hasMemory, uro);
      return true;
   }

   delete innerCommand;
   delete attr;
   return false;
}

_boo parseLooped(const Tokens& tks, Command* innerCommand, Command*& result, uro::Uroboros* uro)
{
   const _boo hasMemory = uro->vc.anyAttribute();
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();
   return parseLooped(tks, innerCommand, result, uro, attr, hasMemory);
}

static Command* kwCommandSimple(const Token& word, Tokens& tks,
   const _int& line, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      uro->vc.setCoreComAttribute(word.getOriginString(uro), line);
      return coreCommandSimpleSave(word, uro);
   }

   Command* result;
   if (parseLooped(tks, coreCommandSimpleNoSave(word, uro), result, uro)) {
      return result;
   }

   throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L"'"), line);
}

static Command* coreCommandSimpleSave(const Token& word, uro::Uroboros* uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete: {
         Attribute* lastAttr = uro->vc.getLastAttribute();
         return new C_Delete(lastAttr, uro);
      }
      case Keyword::kw_Drop: {
         Attribute* lastAttr = uro->vc.getLastAttribute();
         return new C_Drop(lastAttr, uro);
      }
      case Keyword::kw_Hide:
         return new C_Hide(true, uro);
      case Keyword::kw_Lock:
         return new C_Lock(true, uro);
      case Keyword::kw_Unlock:
         return new C_Unlock(true, uro);
      case Keyword::kw_Unhide:
         return new C_Unhide(true, uro);
      default:
         return nullptr;
   }
}

static Command* coreCommandSimpleNoSave(const Token& word, uro::Uroboros* uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete:
         return new C_Delete(uro);
      case Keyword::kw_Drop:
         return new C_Drop(uro);
      case Keyword::kw_Hide:
         return new C_Hide(false, uro);
      case Keyword::kw_Lock:
         return new C_Lock(false, uro);
      case Keyword::kw_Unlock:
         return new C_Unlock(false, uro);
      case Keyword::kw_Unhide:
         return new C_Unhide(false, uro);
      default:
         return nullptr;
   }
}

static Command* kwCommandTime(const Token& word, Tokens& tks, const _int& line, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to' is empty"), line);
   }

   if (!tks.containsKeyword(PG_KEYWORD_TO)) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro),
         L" to' does not contain keyword 'to'"), line);
   }

   Tokens left(tks.list);
   Tokens right(tks.list);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (right.isEmpty()) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro),
         L" to' does not contain its time argument"), line);
   }

   if (left.isEmpty()) {
      uro->vc.setTimeComAttribute(word.getOriginString(uro), line);
      Generator<_tim>* tim;
      if (!parse::parse(uro, right, tim)) {
         throw SyntaxException(str(L"time argument of command '", word.getOriginString(uro),
            L" to' is not valid"), line);
      }

      return coreCommandTime(word, tim, true, uro);
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = ThisState::ts_String;
   Attribute* attr = new Attribute(uro);
   attr->setTimeCommandBase();
   uro->vc.addAttribute(attr);

   Generator<_tim>* tim;
   if (!parse::parse(uro, right, tim)) {
      delete attr;
      throw SyntaxException(str(L"time argument of command '", word.getOriginString(uro),
         L" to' is not valid"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Command* result;
   if (parseLooped(left, coreCommandTime(word, tim, false, uro), result, uro, attr, hasMemory)) {
      return result;
   }

   throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L" to'"), line);
}

static Command* coreCommandTime(const Token& word,
   Generator<_tim>* time, const _boo& saveChanges, uro::Uroboros* uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Reaccess:
         return new C_ReaccessTo(time, saveChanges, uro);
      case Keyword::kw_Rechange:
         return new C_RechangeTo(time, saveChanges, uro);
      case Keyword::kw_Recreate:
         return new C_RecreateTo(time, saveChanges, uro);
      case Keyword::kw_Remodify:
         return new C_RemodifyTo(time, saveChanges, uro);
      default:
         return nullptr;
   }
}

static Command* c_open(const Token& word, const Tokens& tks, const _int& line, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      uro->vc.setCoreComAttribute(word.getOriginString(uro), line);
      return new C_Open(uro);
   }

   if (tks.containsKeyword(PG_KEYWORD_WITH)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_With, left, right);

      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro),  L" with' does not "
            L"contain its last argument"), line);
      }

      const _boo hasMemory = uro->vc.anyAttribute();
      const ThisState prevThisState = uro->vars.inner.thisState;
      uro->vars.inner.thisState = ThisState::ts_String;
      Attribute* attr = new Attribute(uro);
      attr->setCoreCommandBase();
      uro->vc.addAttribute(attr);

      Generator<_str>* prog;
      if (!parse::parse(uro, right, prog)) {
         delete attr;
         throw SyntaxException(str(L"last argument of command '", word.getOriginString(uro), L" with' "
            L"cannot be resolved to a string"), line);
      }

      uro->vars.inner.thisState = prevThisState;
      uro->vc.retreatAttribute();

      if (left.isEmpty()) {
         uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" with"), line);
         return new C_OpenWith(prog, uro);
      }
      else {
         Command* result;
         if (parseLooped(left, new C_OpenWith(prog, uro), result, uro, attr, hasMemory)) {
            return result;
         }

         delete prog;
         throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L" with'"), line);
      }
   }

   Command* result;
   if (parseLooped(tks, new C_Open(uro), result, uro)) {
      return result;
   }

   commandSyntaxException(word.getOriginString(uro), line);
   return nullptr;
}

static Command* c_select(const Token& word, const Tokens& tks, const _int& line, uro::Uroboros* uro)
{
   if (uro->vc.anyAggregate()) {
      Aggregate* aggr = uro->vc.getLastAggregate();
      aggr->set(AGGR_SELECT);

      if (tks.isEmpty()) {
         Generator<_str>* str;
         uro->vars.inner.createThisRef(str);
         return new C_AggrSelect_String(aggr, str, uro);
      }

      Generator<_str>* str;
      if (parse::parse(uro, tks, str)) {
         return new C_AggrSelect_String(aggr, str, uro);
      }

      Generator<_list>* list;
      if (parse::parse(uro, tks, list)) {
         return new C_AggrSelect_List(aggr, list, uro);
      }
   }
   else {
      if (tks.isEmpty()) {
         commandNoArgException(word.getOriginString(uro), line);
      }

      Generator<_str>* str;
      if (parse::parse(uro, tks, str)) {
         return new C_Select_String(str, uro);
      }

      Generator<_list>* list;
      if (parse::parse(uro, tks, list)) {
         return new C_Select_List(list, uro);
      }
   }

   commandSyntaxException(word.getOriginString(uro), line);
   return nullptr;
}

static Command* c_rename(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to' is empty"),
         line);
   }

   if (!tks.containsKeyword(PG_KEYWORD_TO)) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro),  L" to' ",
         L"does not contain keyword 'to'"), line);
   }

   Tokens left(tks.list);
   Tokens right(tks.list);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (right.isEmpty()) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to' ",
         L"does not contain declaration of new name"), line);
   }

   _boo extless = false;

   if (right.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      right.trimLeft();
      if (right.isEmpty()) {
         throw SyntaxException(L"keyword 'extensionless' is not followed by a declaration of new file name", line);
      }
   }

   if (left.isEmpty()) {
      uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" to"), line);

      Generator<_str>* newName;
      if (!parse::parse(uro, right, newName)) {
         throw SyntaxException(str(L"declaration of new name in command '", word.getOriginString(uro), L" to' is not valid"), line);
      }

      Attribute* lastAttr = uro->vc.getLastAttribute();
      lastAttr->markToEvaluate();

      if (stack) {
         return new C_RenameTo_Stack(newName, true, extless, uro);
      }
      else {
         return new C_RenameTo(newName, true, force, extless, uro);
      }
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = ThisState::ts_String;
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();
   uro->vc.addAttribute(attr);

   Generator<_str>* newName;
   if (!parse::parse(uro, right, newName)) {
      delete attr;
      throw SyntaxException(str(L"declaration of new name in command '", word.getOriginString(uro), L" to' is not valid"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Generator<_str>* str_;
   if (parse::parse(uro, left, str_)) {
      if (stack)
         return new CS_StringComArg(str_,
            new C_RenameTo_Stack(newName, false, extless, uro), attr, hasMemory, uro);
      else
         return new CS_StringComArg(str_,
            new C_RenameTo(newName, false, force, extless, uro), attr, hasMemory, uro);
   }

   Generator<_list>* list;
   if (parse::parse(uro, left, list)) {
      if (stack)
         return new CS_ListComArg(list,
            new C_RenameTo_Stack(newName, false, extless, uro), attr, hasMemory, uro);
      else
         return new CS_ListComArg(list,
            new C_RenameTo(newName, false, force, extless, uro), attr, hasMemory, uro);
   }

   delete newName;
   delete attr;
   commandSyntaxException(str(word.getOriginString(uro), L" to"), line);
   return nullptr;
}

static Command* c_create(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      uro->vc.setCoreComAttribute(word.getOriginString(uro), line);
      Attribute* lastAttr = uro->vc.getLastAttribute();

      if (stack) {
         return new C_Create_Stack(lastAttr, uro);
      }
      else {
         return new C_Create(force, lastAttr, uro);
      }
   }

   const Token& f = tks.first();

   if (f.type == Token::t_Word) {
      Tokens tks2 = tks;
      tks2.trimLeft();
      const _size& fk = f.value.word.h;

      if (fk == uro->hashes.HASH_VAR_FILE) {
         if (tks2.isEmpty()) {
            uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" ", f.getOriginString(uro)), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();

            if (stack) {
               return new C_CreateFile_Stack(lastAttr, uro);
            }
            else {
               return new C_CreateFile(force, lastAttr, uro);
            }
         }

         Generator<_str>* str_;
         if (parse::parse(uro, tks2, str_)) {
            if (stack) {
               return new C_CreateFile_String_Stack(str_, uro);
            }
            else {
               return new C_CreateFile_String(str_, force, uro);
            }
         }

         throw SyntaxException(str(L"the argument of command '", word.getOriginString(uro), L" ",
            f.getOriginString(uro), L"' cannot be resolved to a string"), line);
      }
      else if (fk == uro->hashes.HASH_VAR_FILES) {
         if (tks2.isEmpty()) {
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" ",
               f.getOriginString(uro), L"' needs an argument"), line);
         }

         Generator<_str>* str_;
         if (parse::parse(uro, tks2, str_)) {
            delete str_;
            const _str sub = (f.getOriginString(uro)).substr(0, 4);
            throw SyntaxException(str(L"write '", word.getOriginString(uro), L" ", sub, L"' instead of '",
               word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
         }

         _def* def;
         if (parse::parse(uro, tks2, def)) {
            delete def;
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" ", f.getOriginString(uro),
               L"' cannot be called with a definition argument"), line);
         }

         Generator<_list>* list;
         if (parse::parse(uro, tks2, list)) {
            if (stack) {
               return new C_CreateFiles_List_Stack(list, uro);
            }
            else {
               return new C_CreateFiles_List(list, force, uro);
            }
         }

         throw SyntaxException(str(L"wrong syntax of command '",
            word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
      }
      else if (fk == uro->hashes.HASH_VAR_DIRECTORY) {
         if (tks2.isEmpty()) {
            uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" ", f.getOriginString(uro)), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();
            if (stack) {
               return new C_CreateDirectory_Stack(lastAttr, uro);
            }
            else {
               return new C_CreateDirectory(force, lastAttr, uro);
            }
         }

         Generator<_str>* str_;
         if (parse::parse(uro, tks2, str_)) {
            if (stack) {
               return new C_CreateDirectory_String_Stack(str_, uro);
            }
            else {
               return new C_CreateDirectory_String(str_, force, uro);
            }
         }

         throw SyntaxException(str(L"argument of command '", word.getOriginString(uro), L" ",
            f.getOriginString(uro), L"' cannot be resolved to a string"), line);
      }
      else if (fk == uro->hashes.HASH_VAR_DIRECTORIES) {
         if (tks2.isEmpty()) {
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" ",
               f.getOriginString(uro), L"' needs an argument"), line);
         }

         Generator<_str>* str_;
         if (parse::parse(uro, tks2, str_)) {
            delete str_;
            const _str sub = str((f.getOriginString(uro)).substr(0, 8), L"y");
            throw SyntaxException(str(L"write '", word.getOriginString(uro), L" ", sub,
               L"' instead of '", word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
         }

         _def* def;
         if (parse::parse(uro, tks2, def)) {
            delete def;
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" ",
               f.getOriginString(uro), L"' cannot be called with a definition argument"), line);
         }

         Generator<_list>* list;
         if (parse::parse(uro, tks2, list)) {
            if (stack) {
               return new C_CreateDirectories_List_Stack(list, uro);
            }
            else {
               return new C_CreateDirectories_List(list, force, uro);
            }
         }

         throw SyntaxException(str(L"wrong syntax of command '",
            word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
      }
   }

   Generator<_str>* str_;
   if (parse::parse(uro, tks, str_)) {
      if (stack) {
         return new C_Create_String_Stack(str_, uro);
      }
      else {
         return new C_Create_String(str_, force, uro);
      }
   }

   _def* def;
   if (parse::parse(uro, tks, def)) {
      delete def;
      throw SyntaxException(str(L"command '", word.getOriginString(uro),
         L"' cannot be called with a definition argument"), line);
   }

   Generator<_list>* list;
   if (parse::parse(uro, tks, list)) {
      if (stack) {
         return new C_Create_List_Stack(list, uro);
      }
      else {
         return new C_Create_List(list, force, uro);
      }
   }

   commandSyntaxException(word.getOriginString(uro), line);
   return nullptr;
}

static Command* c_moveTo(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to' is empty"), line);
   }

   const _boo hasTo = tks.containsKeyword(PG_KEYWORD_TO);
   const _boo hasAs = tks.containsKeyword(PG_KEYWORD_AS);

   if (!hasTo) {
      throw SyntaxException(str(L"command '", word.getOriginString(uro),
         L" to' cannot be called without keyword 'to'"), line);
   }

   Tokens left(tks);
   Tokens right(tks);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (left.isEmpty()) {
      if (hasAs) {
         Tokens preAs(right);
         Tokens postAs(right);
         right.divideByKeyword(Keyword::kw_As, preAs, postAs);

         if (preAs.isEmpty()) {
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _boo extless = false;
         const Token& paf = postAs.first();

         if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxException(str(L"keyword '", paf.getOriginString(uro),
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         Generator<_str>* nname;
         if (!parse::parse(uro, postAs, nname)) {
            throw SyntaxException(str(L"new name in command '", word.getOriginString(uro),
               L" to as' cannot be resolved to a string"), line);
         }

         Generator<_str>* dest;
         if (!parse::parse(uro, preAs, dest)) {
            delete nname;
            throw SyntaxException(str(L"new location in command '", word.getOriginString(uro),
               L" to' cannot be resolved to a string"), line);
         }

         uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" to as"), line);
         Attribute* lastAttr = uro->vc.getLastAttribute();
         if (stack) {
            return new C_MoveToAs_Stack(dest, nname, extless, lastAttr, uro);
         }
         else {
            return new C_MoveToAs(dest, nname, force, extless, lastAttr, uro);
         }
      }

      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro),
            L" to' lacks declaration of a new location"), line);
      }

      Generator<_str>* str_;
      if (!parse::parse(uro, right, str_)) {
         throw SyntaxException(str(L"new location in command '",
            word.getOriginString(uro), L" to' cannot be resolved to a string"), line);
      }
      else {
         uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" to"), line);
         Attribute* lastAttr = uro->vc.getLastAttribute();
         if (stack) {
            return new C_MoveTo_Stack(str_, lastAttr, uro);
         }
         else {
            return new C_MoveTo(str_, force, lastAttr, uro);
         }
      }
   }

   if (hasAs) {
      if (left.containsKeyword(PG_KEYWORD_AS)) {
         throw SyntaxException(str(L"keywords 'to' and 'as' appear in command '",
            word.getOriginString(uro), L" to as' in reverse order"), line);
      }

      Tokens preAs(right);
      Tokens postAs(right);
      right.divideByKeyword(Keyword::kw_As, preAs, postAs);

      if (preAs.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain declaration of a new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain declaration of a new name written after keyword 'as'"), line);
      }

      _boo extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"keyword '", paf.getOriginString(uro),
               L"' is not followed by a declaration of a new file name"), line);
         }
      }

      const _boo hasMemory = uro->vc.anyAttribute();
      const ThisState prevThisState = uro->vars.inner.thisState;
      uro->vars.inner.thisState = ThisState::ts_String;
      Attribute* attr = new Attribute(uro);
      attr->setCoreCommandBase();
      uro->vc.addAttribute(attr);

      Generator<_str>* nname;
      if (!parse::parse(uro, postAs, nname)) {
         delete attr;
         throw SyntaxException(str(L"new name in command '", word.getOriginString(uro),
            L" to as' cannot be resolved to a string"), line);
      }

      Generator<_str>* dest;
      if (!parse::parse(uro, preAs, dest)) {
         delete attr;
         delete nname;
         throw SyntaxException(str(L"new location in command '", word.getOriginString(uro),
            L" to' cannot be resolved to a string"), line);
      }

      uro->vars.inner.thisState = prevThisState;
      uro->vc.retreatAttribute();

      Command* result;
      if (stack) {
         if (parseLooped(left, new C_MoveToAs_Stack(dest, nname, extless, uro),
            result, uro, attr, hasMemory))
         {
            return result;
         }
      }
      else {
         if (parseLooped(left, new C_MoveToAs(dest, nname, force, extless, uro),
            result, uro, attr, hasMemory))
         {
            return result;
         }
      }

      delete nname;
      delete dest;
      throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L" to as'"), line);
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = ThisState::ts_String;
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();
   uro->vc.addAttribute(attr);

   Generator<_str>* dest;
   if (!parse::parse(uro, right, dest)) {
      throw SyntaxException(str(L"new location in command '", word.getOriginString(uro),
         L" to' cannot be resolved to a string"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Command* result;
   if (stack) {
      if (parseLooped(left, new C_MoveTo_Stack(dest, uro), result, uro, attr, hasMemory)) {
         return result;
      }
   }
   else {
      if (parseLooped(left, new C_MoveTo(dest, force, uro), result, uro, attr, hasMemory)) {
         return result;
      }
   }

   delete dest;
   throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L" to'"), line);
}

static Command* c_downloadFrom(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, uro::Uroboros* uro)
{
   if (tks.containsKeyword(PG_KEYWORD_FROM)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_From, left, right);

      if (left.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L" from' does not "
            L"contain a declaration of elements to download"), line);
      }
      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L" from' does not "
            L"contain a declaration of location"), line);
      }

      Generator<_str>* loc;
      if (!parse::parse(uro, right, loc)) {
         throw SyntaxException(str(L"location in command '", word.getOriginString(uro),
            L" from' cannot be resolved to a string"), line);
      }

      Generator<_str>* str_;
      if (parse::parse(uro, left, str_)) {
         if (stack)
            return new C_DownloadFrom_String_Stack(loc, str_, uro);
         else
            return new C_DownloadFrom_String(loc, str_, force, uro);
      }

      _def* def;
      if (parse::parse(uro, left, def)) {
         if (stack)
            return new C_DownloadFrom_Definition_Stack(loc, def, uro);
         else
            return new C_DownloadFrom_Definition(loc, def, force, uro);
      }

      Generator<_list>* list;
      if (parse::parse(uro, left, list)) {
         if (stack)
            return new C_DownloadFrom_List_Stack(loc, list, uro);
         else
            return new C_DownloadFrom_List(loc, list, force, uro);
      }

      delete loc;
      throw SyntaxException(str(L"wrong syntax of command '",
         word.getOriginString(uro), L" from'"), line);
   }

   Generator<_str>* str_;
   if (parse::parse(uro, tks, str_)) {
      if (stack)
         return new C_Download_String_Stack(str_, uro);
      else
         return new C_Download_String(str_, force, uro);
   }

   _def* def;
   if (parse::parse(uro, tks, def)) {
      delete def;
      throw SyntaxException(str(L"the argument of command '",
         word.getOriginString(uro), L"' cannot be of type 'definition'"), line);
   }

   Generator<_list>* list;
   if (parse::parse(uro, tks, list)) {
      if (stack)
         return new C_Download_List_Stack(list, uro);
      else
         return new C_Download_List(list, force, uro);
   }

   throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L"'"), line);
}

static Command* c_copy(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, uro::Uroboros* uro)
{
   const _boo hasTo = tks.containsKeyword(PG_KEYWORD_TO);
   const _boo hasAs = tks.containsKeyword(PG_KEYWORD_AS);

   if (!hasTo) {
      if (hasAs) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro),
            L" to as' cannot be called without keyword 'to'"), line);
      }

      if (force) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro),
            L"' cannot be preceded by a flag 'forced'"), line);
      }

      if (stack) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro),
         L"' cannot be preceded by a flag 'stack'"), line);
      }

      if (uro->vc.anyAggregate()) {
         Aggregate* aggr = uro->vc.getLastAggregate();
         aggr->set(AGGR_COPY);

         if (tks.isEmpty()) {
            Generator<_str>* str;
            uro->vars.inner.createThisRef(str);
            return new C_AggrCopy_String(aggr, str, uro);
         }

         Generator<_str>* str;
         if (parse::parse(uro, tks, str)) {
            return new C_AggrCopy_String(aggr, str, uro);
         }

         Generator<_list>* list;
         if (parse::parse(uro, tks, list)) {
            return new C_AggrCopy_List(aggr, list, uro);
         }
      }
      else {
         if (tks.isEmpty()) {
            commandNoArgException(word.getOriginString(uro), line);
         }

         Generator<_str>* str;
         if (parse::parse(uro, tks, str)) {
            return new C_Copy_String(str, uro);
         }

         Generator<_list>* list;
         if (parse::parse(uro, tks, list)) {
            return new C_Copy_List(list, uro);
         }
      }

      commandSyntaxException(word.getOriginString(uro), line);
   }

   Tokens left(tks);
   Tokens right(tks);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (left.isEmpty()) {
      if (hasAs) {
         Tokens preAs(right);
         Tokens postAs(right);
         right.divideByKeyword(Keyword::kw_As, preAs, postAs);

         if (preAs.isEmpty()) {
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _boo extless = false;
         const Token& paf = postAs.first();

         if (paf.isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxException(str(L"keyword '", paf.getOriginString(uro),
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         Generator<_str>* nname;
         if (!parse::parse(uro, postAs, nname)) {
            throw SyntaxException(str(L"new name in command '", word.getOriginString(uro),
               L" to as' cannot be resolved to a string"), line);
         }

         Generator<_str>* dest;
         if (!parse::parse(uro, preAs, dest)) {
            delete dest;
            throw SyntaxException(str(L"new location in command '", word.getOriginString(uro),
               L" to' cannot be resolved to a string"), line);
         }

         uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" to as"), line);
         if (stack) {
            return new C_CopyToAs_Stack(dest, nname, true, extless, uro);
         }
         else {
            return new C_CopyToAs(dest, nname, true, force, extless, uro);
         }
      }

      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro),
            L" to' lacks a declaration of new location"), line);
      }

      Generator<_str>* str_;
      if (parse::parse(uro, right, str_)) {
         uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" to"), line);
         if (stack) {
            return new C_CopyTo_Stack(str_, true, uro);
         }
         else {
            return new C_CopyTo(str_, true, force, uro);
         }
      }
      else {
         throw SyntaxException(str(L"new location in command '",
            word.getOriginString(uro), L" to' cannot be resolved to a string"), line);
      }
   }

   if (hasAs) {
      if (left.containsKeyword(PG_KEYWORD_AS)) {
         throw SyntaxException(str(L"keywords 'to' and 'as' appear in "
            L"command '", word.getOriginString(uro), L" to as' in reverse order"), line);
      }

      Tokens preAs(right);
      Tokens postAs(right);
      right.divideByKeyword(Keyword::kw_As, preAs, postAs);

      if (preAs.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain a declaration of new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain a declaration of new name written after keyword 'as'"), line);
      }

      _boo extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"keyword '", paf.getOriginString(uro),
               L"' is not followed by a declaration of new file name"), line);
         }
      }

      const _boo hasMemory = uro->vc.anyAttribute();
      const ThisState prevThisState = uro->vars.inner.thisState;
      uro->vars.inner.thisState = ThisState::ts_String;
      Attribute* attr = new Attribute(uro);
      attr->setCoreCommandBase();
      uro->vc.addAttribute(attr);

      Generator<_str>* nname;
      if (!parse::parse(uro, postAs, nname)) {
         throw SyntaxException(str(L"new name in command '", word.getOriginString(uro),
            L" to as' cannot be resolved to a string"), line);
      }

      Generator<_str>* dest;
      if (!parse::parse(uro, preAs, dest)) {
         delete nname;
         throw SyntaxException(str(L"new location in command '", word.getOriginString(uro),
            L" to' cannot be resolved to a string"), line);
      }

      uro->vars.inner.thisState = prevThisState;
      uro->vc.retreatAttribute();

      Command* result;
      if (stack) {
         if (parseLooped(left, new C_CopyToAs_Stack(dest, nname, false, extless, uro),
            result, uro, attr, hasMemory))
         {
            return result;
         }
      }
      else {
         if (parseLooped(left, new C_CopyToAs(dest, nname, false, force, extless, uro),
            result, uro, attr, hasMemory))
         {
            return result;
         }
      }

      delete nname;
      delete dest;
      throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L" to as'"), line);
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = ThisState::ts_String;
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();
   uro->vc.addAttribute(attr);

   Generator<_str>* dest;
   if (!parse::parse(uro, right, dest)) {
      throw SyntaxException(str(L"new location in command '", word.getOriginString(uro),
         L" to' cannot be resolved to a string"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Command* result;
   if (stack) {
      if (parseLooped(left, new C_CopyTo_Stack(dest, false, uro), result, uro, attr, hasMemory)) {
         return result;
      }
   }
   else {
      if (parseLooped(left, new C_CopyTo(dest, false, force, uro), result, uro, attr, hasMemory)) {
         return result;
      }
   }

   delete dest;
   throw SyntaxException(str(L"wrong syntax of command '", word.getOriginString(uro), L" to'"), line);
}

Command* c_print(const Token& word, const Tokens& tks, const _int& line, const _boo& directError, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      switch (uro->vars.inner.thisState) {
         case ts_None: {
         throw SyntaxException(str(L"command '", word.getOriginString(uro), L"' needs an argument here. "
            L"Value of variable 'this' is undefined in this area"), line);
            break;
         }
         case ts_String: {
            return new C_PrintThis_Str(uro);
            break;
         }
         case ts_Time: {
            return new C_PrintThis_Tim(uro);
            break;
         }
         case ts_Number: {
            return new C_PrintThis_Num(uro);
            break;
         }
      }
   }

   Generator<_str>* str;
   if (parse::parse(uro, tks, str)) {
      return new C_PrintSingle(str, uro);
   }

   _def* def;
   if (parse::parse(uro, tks, def)) {
      return new C_PrintDefinition(def, uro);
   }

   Generator<_list>* list;
   if (parse::parse(uro, tks, list)) {
      return new C_PrintList(list, uro);
   }

   if (directError) {
      commandSyntaxException(word.getOriginString(uro), line);
   }
   else {
      throw SyntaxException(L"wrong syntax. No command can be formed of this code", line);
   }

   return nullptr;
}

static Command* c_sleep(const Token& word, const Tokens& tks, const _int& line, uro::Uroboros* uro)
{
   Generator<_per>* per;
   if (parse::parse(uro, tks, per)) {
      return new C_SleepPeriod(per, uro);
   }

   Generator<_num>* num;
   if (parse::parse(uro, tks, num)) {
      return new C_SleepMs(num, uro);
   }

   commandSyntaxException(word.getOriginString(uro), line);
   return nullptr;
}

static Command* c_run(const Token& word, const Tokens& tks, const _int& line, uro::Uroboros* uro)
{
   uro->vc.markAttributesToRun();

   if (tks.containsKeyword(PG_KEYWORD_WITH)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_With, left, right);

      if (right.isEmpty()) {
         throw SyntaxException(str(L"right side of command '", word.getOriginString(uro), L" with' is empty"), line);
      }

      if (left.isEmpty()) {
         if (right.containsKeyword(PG_KEYWORD_WITH)) {
            if (uro->vars.inner.thisState == ts_None) {
               throw SyntaxException(str(L"command '", word.getOriginString(uro), L" with with' needs first argument"), line);
            }

            uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" with with"), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();

            Tokens left2(right);
            Tokens right2(right);
            right.divideByKeyword(Keyword::kw_With, left2, right2);

            if (left2.isEmpty()) {
               throw SyntaxException(str(L"command '", word.getOriginString(uro),
                  L"' cannot be called with adjacent 'with' keywords"), line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxException(str(L"command '", word.getOriginString(uro),
                  L" with with' cannot be called without its last argument"), line);
            }

            Generator<_str>* exec;
            if (!parse::parse(uro, left2, exec)) {
               throw SyntaxException(str(L"second argument of command '", word.getOriginString(uro),
                  L" with with' cannot be resolved to a string"), line);
            }

            if (left2.getLength() == 1) {
               const Token& cf = left2.first();
               if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                  delete exec;
                  Generator<_str>* str_;

                  if (parse::parse(uro, right2, str_)) {
                     return new C_RunWithUroborosWithString(str_, lastAttr, uro);
                  }
                  else {
                     Generator<_list>* list;
                     if (parse::parse(uro, right2, list)) {
                        return new C_RunWithUroborosWith(list, lastAttr, uro);
                     }
                     else {
                        delete exec;
                        throw SyntaxException(str(L"last argument of command '",
                           word.getOriginString(uro), L" with Uroboros with' cannot be resolved to a list"), line);
                     }
                  }
               }
            }

            Generator<_str>* str_;
            if (parse::parse(uro, right2, str_)) {
               return new C_RunWithWithString(exec, str_, lastAttr, uro);
            }
            else {
               Generator<_list>* list;
               if (parse::parse(uro, right2, list)) {
                  return new C_RunWithWith(exec, list, lastAttr, uro);
               }
               else {
                  delete exec;
                  throw SyntaxException(str(L"last argument of command '",
                     word.getOriginString(uro), L" with with' cannot be resolved to a list"), line);
               }
            }
         }
         else {
            if (uro->vars.inner.thisState == ts_None) {
               throw SyntaxException(str(L"command '", word.getOriginString(uro),
                  L" with' needs first argument"), line);
            }

            uro->vc.setCoreComAttribute(str(word.getOriginString(uro), L" with"), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();

            Generator<_str>* exec;
            if (parse::parse(uro, right, exec)) {
               if (right.getLength() == 1) {
                  const Token& cf = right.first();
                  if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                     delete exec;
                     return new C_RunWithUroboros(lastAttr, uro);
                  }
               }
               return new C_RunWith(exec, lastAttr, uro);
            }
            else {
               throw SyntaxException(str(L"last argument of command '",
                  word.getOriginString(uro), L" with' cannot be resolved to a string"), line);
            }
         }
      }
      else {
         if (right.containsKeyword(PG_KEYWORD_WITH)) {
            Tokens left2(right);
            Tokens right2(right);
            right.divideByKeyword(Keyword::kw_With, left2, right2);

            if (left2.isEmpty()) {
               throw SyntaxException(str(L"command '", word.getOriginString(uro),
                  L"' cannot be called with adjacent 'with' keywords"), line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxException(str(L"command '", word.getOriginString(uro),
                  L" with with' cannot be called without its last argument"), line);
            }

            const _boo hasMemory = uro->vc.anyAttribute();
            Attribute* lastAttr = uro->vc.getLastAttribute();
            const ThisState prevThisState = uro->vars.inner.thisState;
            uro->vars.inner.thisState = ThisState::ts_String;
            Attribute* attr = new Attribute(uro);
            attr->setCoreCommandBase();
            uro->vc.addAttribute(attr);

            Generator<_str>* exec;
            if (!parse::parse(uro, left2, exec)) {
               throw SyntaxException(str(L"second argument of command '", word.getOriginString(uro),
                  L" with with' cannot be resolved to a string"), line);
            }

            Generator<_str>* lastStr;
            if (parse::parse(uro, right2, lastStr)) {
               uro->vars.inner.thisState = prevThisState;
               uro->vc.retreatAttribute();

               if (left2.getLength() == 1) {
                  const Token& cf = left2.first();
                  if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                     delete exec;

                     Command* result;
                     if (parseLooped(left, new C_RunWithUroborosWithString(lastStr, uro), result, uro, attr, hasMemory)) {
                        return result;
                     }

                     delete lastStr;
                     throw SyntaxException(str(L"first argument of command '", word.getOriginString(uro),
                        L" with Uroboros with' cannot be resolved to a list"), line);
                  }
               }

               Command* result;
               if (parseLooped(left, new C_RunWithWithString(exec, lastStr, uro), result, uro, attr, hasMemory)) {
                  return result;
               }

               delete exec;
               delete lastStr;
               throw SyntaxException(str(L"first argument of command '", word.getOriginString(uro),
                  L" with with' cannot be resolved to a list"), line);
            }
            else {
               Generator<_list>* lastList;

               if (!parse::parse(uro, right2, lastList)) {
                  uro->vars.inner.thisState = prevThisState;
                  uro->vc.retreatAttribute();
                  delete exec;
                  throw SyntaxException(str(L"last argument of command '", word.getOriginString(uro),
                     L" with with' cannot be resolved to a list"), line);
               }
               else {
                  uro->vars.inner.thisState = prevThisState;
                  uro->vc.retreatAttribute();

                  if (left2.getLength() == 1) {
                     const Token& cf = left2.first();
                     if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                        delete exec;

                        Command* result;
                        if (parseLooped(left, new C_RunWithUroborosWith(lastList, uro), result, uro, attr, hasMemory)) {
                           return result;
                        }

                        delete lastList;
                        throw SyntaxException(str(L"first argument of command '", word.getOriginString(uro),
                           L" with Uroboros with' cannot be resolved to a list"), line);
                     }
                  }

                  Command* result;
                  if (parseLooped(left, new C_RunWithWith(exec, lastList, uro), result, uro, attr, hasMemory)) {
                     return result;
                  }

                  delete exec;
                  delete lastList;
                  throw SyntaxException(str(L"first argument of command '", word.getOriginString(uro),
                     L" with with' cannot be resolved to a list"), line);
               }
            }
         }
         else {
            const _boo hasMemory = uro->vc.anyAttribute();
            Attribute* lastAttr = uro->vc.getLastAttribute();
            const ThisState prevThisState = uro->vars.inner.thisState;
            uro->vars.inner.thisState = ThisState::ts_String;
            Attribute* attr = new Attribute(uro);
            attr->setCoreCommandBase();
            uro->vc.addAttribute(attr);

            Generator<_str>* exec;
            if (!parse::parse(uro, right, exec)) {
               throw SyntaxException(str(L"last argument of command '", word.getOriginString(uro),
                  L" with' cannot be resolved to a string"), line);
            }

            uro->vars.inner.thisState = prevThisState;
            uro->vc.retreatAttribute();

            if (right.getLength() == 1) {
               const Token& cf = right.first();
               if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                  delete exec;

                  Command* result;
                  if (parseLooped(left, new C_RunWithUroboros(uro), result, uro, attr, hasMemory)) {
                     return result;
                  }

                  throw SyntaxException(str(L"first argument of command '", word.getOriginString(uro),
                     L" with Uroboros' cannot be resolved to a list"), line);
               }
            }

            Command* result;
            if (parseLooped(left, new C_RunWith(exec, uro), result, uro, attr, hasMemory)) {
               return result;
            }

            delete exec;
            throw SyntaxException(str(L"first argument of command '", word.getOriginString(uro),
               L" with' cannot be resolved to a list"), line);
         }
      }
   }
   else {
      Attribute* lastAttr = uro->vc.getLastAttribute();
      Generator<_str>* str;
      if (parse::parse(uro, tks, str)) {
         return new C_Run(str, lastAttr, uro);
      }
   }

   commandSyntaxException(word.getOriginString(uro), line);
   return nullptr;
}

static Command* c_error(const Token& word, const Tokens& tks, const _int& line, uro::Uroboros* uro)
{
   if (tks.isEmpty()) {
      return new C_Error(uro);
   }

   Generator<_num>* num;

   if (parse::parse(uro, tks, num)) {
      return new C_ErrorWithExitCode(num, uro);
   }
   else {
      throw SyntaxException(str(L"the argument of command '", word.getOriginString(uro),
         L"' cannot be resolved to a number"), line);
   }
}

static void checkUselessFlags(const Token& word, const _int& line,
   const bool& force, const bool& stack, uro::Uroboros* uro)
{
   if (force) {
      throw SyntaxException(str(L"keyword '", word.getOriginString(uro),
         L"' cannot be preceded by a flag 'forced'"), line);
   }

   if (stack) {
      throw SyntaxException(str(L"keyword '", word.getOriginString(uro),
         L"' cannot be preceded by a flag 'stack'"), line);
   }
}

static void commandSyntaxException(const _str& name, const _int& line)
{
   throw SyntaxException(str(L"wrong syntax of command '", name, L"'"), line);
}

static void commandNoArgException(const _str& name, const _int& line)
{
   throw SyntaxException(str(L"command '", name, L"' requires an argument"), line);
}

}
