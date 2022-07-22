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


Command* keywordCommands(const Token& word, Tokens& tks,
   const _int& line, const bool& force, const bool& stack, Uroboros* uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Unlock:
      case Keyword::kw_Unhide: {
         checkUselessFlags(word, line, force, stack);
         return kwCommandSimple(word, tks, line, uro);
      }
      case Keyword::kw_Open: {
         checkUselessFlags(word, line, force, stack);
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
         checkUselessFlags(word, line, force, stack);
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L"' cannot be called with an argument"), line);
      }
      case Keyword::kw_Error: {
         checkUselessFlags(word, line, force, stack);
         return c_error(word, tks, line, uro);
      }
      case Keyword::kw_Move: {
         return c_moveTo(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Print: {
         checkUselessFlags(word, line, force, stack);
         return c_print(word, tks, line, true, uro);
      }
      case Keyword::kw_Reaccess:
      case Keyword::kw_Recreate:
      case Keyword::kw_Rechange:
      case Keyword::kw_Remodify: {
         checkUselessFlags(word, line, force, stack);
         return kwCommandTime(word, tks, line, uro);
      }
      case Keyword::kw_Rename: {
         return c_rename(word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Run: {
         checkUselessFlags(word, line, force, stack);
         return c_run(word, tks, line, uro);
      }
      case Keyword::kw_Select: {
         checkUselessFlags(word, line, force, stack);
         return c_select(word, tks, line, uro);
      }
      case Keyword::kw_Sleep: {
         checkUselessFlags(word, line, force, stack);
         return c_sleep(word, tks, line, uro);
      }
      case Keyword::kw_Process: {
         checkUselessFlags(word, line, force, stack);
         return c_process(word, tks, line, uro);
      }
   }

   throw SyntaxException(str(L"command cannot start with a keyword '", *word.value.keyword.os, L"'"), line);
}

static Command* kwCommandSimple(const Token& word, Tokens& tks,
   const _int& line, Uroboros* uro)
{
   if (tks.isEmpty()) {
      uro->vc.setCoreComAttribute(*word.value.keyword.os, line);
      return coreCommandSimpleSave(word, uro);
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();

   Generator<_str>* str_;
   if (parse(uro, tks, str_)) {
      return new CS_StringComArg(str_,
         coreCommandSimpleNoSave(word, uro), attr, hasMemory, uro);
   }

   _def* def;
   if (parse(uro, tks, def)) {
      return new CS_DefinitionComArg(def,
         coreCommandSimpleNoSave(word, uro), attr, hasMemory, uro);
   }

   Generator<_list>* list;
   if (parse(uro, tks, list)) {
      return new CS_ListComArg(list,
         coreCommandSimpleNoSave(word, uro), attr, hasMemory, uro);
   }

   throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L"'"), line);
}

static Command* coreCommandSimpleSave(const Token& word, Uroboros* uro)
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
         return nullptr; // error
   }
}

static Command* coreCommandSimpleNoSave(const Token& word, Uroboros* uro)
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
         return nullptr; // error
   }
}

static Command* kwCommandTime(const Token& word, Tokens& tks, const _int& line, Uroboros* uro)
{
   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to' is empty"), line);
   }

   if (!tks.containsKeyword(Keyword::kw_To)) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os,
         L" to' does not contain keyword 'to'"), line);
   }

   Tokens left(tks.list);
   Tokens right(tks.list);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (right.isEmpty()) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os,
         L" to' does not contain its time argument"), line);
   }


   if (left.isEmpty()) {
      uro->vc.setTimeComAttribute(*word.value.keyword.os, line);
      Generator<_tim>* tim;
      if (!parse(uro, right, tim)) {
         throw SyntaxException(str(L"time argument of command '", *word.value.keyword.os,
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
   if (!parse(uro, right, tim)) {
      throw SyntaxException(str(L"time argument of command '", *word.value.keyword.os,
         L" to' is not valid"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Generator<_str>* str_;
   if (parse(uro, left, str_)) {
      return new CS_StringComArg(str_,
         coreCommandTime(word, tim, false, uro), attr, hasMemory, uro);
   }

   _def* def;
   if (parse(uro, left, def)) {
      return new CS_DefinitionComArg(def,
         coreCommandTime(word, tim, false, uro), attr, hasMemory, uro);
   }

   Generator<_list>* list;
   if (parse(uro, left, list)) {
      return new CS_ListComArg(list,
         coreCommandTime(word, tim, false, uro), attr, hasMemory, uro);
   }

   throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L" to'"), line);
}

static Command* coreCommandTime(const Token& word,
   Generator<_tim>* time, const _boo& saveChanges, Uroboros* uro)
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
         return nullptr; // error
   }
}

static Command* c_open(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro)
{
   if (tks.isEmpty()) {
      uro->vc.setCoreComAttribute(*word.value.keyword.os, line);
      return new C_Open(uro);
   }

   if (tks.containsKeyword(Keyword::kw_With)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_With, left, right);

      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os,  L" with' does not "
            L"contain its last argument"), line);
      }

      Generator<_str>* prog;
      if (!parse(uro, right, prog)) {
         throw SyntaxException(str(L"last argument of command '", *word.value.keyword.os, L" with' "
            L"cannot be resolved to a string"), line);
      }

      if (left.isEmpty()) {
         uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" with"), line);
         return new C_OpenWith(prog, uro);
      }
      else {
         const _boo hasMemory = uro->vc.anyAttribute();
         Attribute* attr = new Attribute(uro);
         attr->setCoreCommandBase();

         Generator<_str>* str_;
         if (parse(uro, left, str_)) {
            return new CS_StringComArg(str_, new C_OpenWith(prog, uro), attr, hasMemory, uro);
         }

         _def* def;
         if (parse(uro, left, def)) {
            return new CS_DefinitionComArg(def, new C_OpenWith(prog, uro), attr, hasMemory, uro);
         }

         Generator<_list>* list;
         if (parse(uro, left, list)) {
            return new CS_ListComArg(list, new C_OpenWith(prog, uro), attr, hasMemory, uro);
         }

         delete prog;
         throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L" with'"), line);
      }
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();

   Generator<_str>* str;
   if (parse(uro, tks, str)) {
      return new CS_StringComArg(str, new C_Open(uro), attr, hasMemory, uro);
   }

   _def* def;
   if (parse(uro, tks, def)) {
      return new CS_DefinitionComArg(def, new C_Open(uro), attr, hasMemory, uro);
   }

   Generator<_list>* list;
   if (parse(uro, tks, list)) {
      return new CS_ListComArg(list, new C_Open(uro), attr, hasMemory, uro);
   }

   commandSyntaxException(*word.value.keyword.os, line);
}

static Command* c_select(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro)
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
      if (parse(uro, tks, str)) {
         return new C_AggrSelect_String(aggr, str, uro);
      }

      Generator<_list>* list;
      if (parse(uro, tks, list)) {
         return new C_AggrSelect_List(aggr, list, uro);
      }
   }
   else {
      if (tks.isEmpty()) {
         commandNoArgException(*word.value.keyword.os, line);
      }

      Generator<_str>* str;
      if (parse(uro, tks, str)) {
         return new C_Select_String(str, uro);
      }

      Generator<_list>* list;
      if (parse(uro, tks, list)) {
         return new C_Select_List(list, uro);
      }
   }

   commandSyntaxException(*word.value.keyword.os, line);
}

static Command* c_rename(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro)
{
   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to' is empty"),
         line);
   }

   if (!tks.containsKeyword(Keyword::kw_To)) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os,  L" to' ",
         L"does not contain keyword 'to'"), line);
   }

   Tokens left(tks.list);
   Tokens right(tks.list);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (right.isEmpty()) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to' ",
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
      uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" to"), line);

      Generator<_str>* newName;
      if (!parse(uro, right, newName)) {
         throw SyntaxException(str(L"declaration of new name in command '", *word.value.keyword.os, L" to' is not valid"), line);
      }

      Attribute* lastAttr = uro->vc.getLastAttribute();
      lastAttr->markToEvaluate = true;

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
   if (!parse(uro, right, newName)) {
      throw SyntaxException(str(L"declaration of new name in command '", *word.value.keyword.os, L" to' is not valid"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Generator<_str>* str_;
   if (parse(uro, left, str_)) {
      if (stack)
         return new CS_StringComArg(str_,
            new C_RenameTo_Stack(newName, false, extless, uro), attr, hasMemory, uro);
      else
         return new CS_StringComArg(str_,
            new C_RenameTo(newName, false, force, extless, uro), attr, hasMemory, uro);
   }

   Generator<_list>* list;
   if (parse(uro, left, list)) {
      if (stack)
         return new CS_ListComArg(list,
            new C_RenameTo_Stack(newName, false, extless, uro), attr, hasMemory, uro);
      else
         return new CS_ListComArg(list,
            new C_RenameTo(newName, false, force, extless, uro), attr, hasMemory, uro);
   }

   commandSyntaxException(str(*word.value.keyword.os, L" to"), line);
}

static Command* c_create(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro)
{
   if (tks.isEmpty()) {
      uro->vc.setCoreComAttribute(*word.value.keyword.os, line);
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
            uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" ", *f.value.word.os), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();

            if (stack) {
               return new C_CreateFile_Stack(lastAttr, uro);
            }
            else {
               return new C_CreateFile(force, lastAttr, uro);
            }
         }

         Generator<_str>* str_;
         if (parse(uro, tks2, str_)) {
            if (stack) {
               return new C_CreateFile_String_Stack(str_, uro);
            }
            else {
               return new C_CreateFile_String(str_, force, uro);
            }
         }

         throw SyntaxException(str(L"the argument of command '", *word.value.keyword.os, L" ",
            *f.value.word.os, L"' cannot be resolved to a string"), line);
      }
      else if (fk == uro->hashes.HASH_VAR_FILES) {
         if (tks2.isEmpty()) {
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" ",
               *f.value.word.os, L"' needs an argument"), line);
         }

         Generator<_str>* str_;
         if (parse(uro, tks2, str_)) {
            delete str_;
            const _str sub = (*f.value.word.os).substr(0, 4);
            throw SyntaxException(str(L"write '", *word.value.keyword.os, L" ", sub, L"' instead of '",
               *word.value.keyword.os, L" ", *f.value.word.os, L"'"), line);
         }

         _def* def;
         if (parse(uro, tks2, def)) {
            delete def;
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" ", *f.value.word.os,
               L"' cannot be called with a definition argument"), line);
         }

         Generator<_list>* list;
         if (parse(uro, tks2, list)) {
            if (stack) {
               return new C_CreateFiles_List_Stack(list, uro);
            }
            else {
               return new C_CreateFiles_List(list, force, uro);
            }
         }

         throw SyntaxException(str(L"wrong syntax of command '",
            *word.value.keyword.os, L" ", *f.value.word.os, L"'"), line);
      }
      else if (fk == uro->hashes.HASH_VAR_DIRECTORY) {
         if (tks2.isEmpty()) {
            uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" ", *f.value.word.os), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();
            if (stack) {
               return new C_CreateDirectory_Stack(lastAttr, uro);
            }
            else {
               return new C_CreateDirectory(force, lastAttr, uro);
            }
         }

         Generator<_str>* str_;
         if (parse(uro, tks2, str_)) {
            if (stack) {
               return new C_CreateDirectory_String_Stack(str_, uro);
            }
            else {
               return new C_CreateDirectory_String(str_, force, uro);
            }
         }

         throw SyntaxException(str(L"argument of command '", *word.value.keyword.os, L" ",
            *f.value.word.os, L"' cannot be resolved to a string"), line);
      }
      else if (fk == uro->hashes.HASH_VAR_DIRECTORIES) {
         if (tks2.isEmpty()) {
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" ",
               *f.value.word.os, L"' needs an argument"), line);
         }

         Generator<_str>* str_;
         if (parse(uro, tks2, str_)) {
            delete str_;
            const _str sub = str((*f.value.word.os).substr(0, 8), L"y");
            throw SyntaxException(str(L"write '", *word.value.keyword.os, L" ", sub,
               L"' instead of '", *word.value.keyword.os, L" ", *f.value.word.os, L"'"), line);
         }

         _def* def;
         if (parse(uro, tks2, def)) {
            delete def;
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" ",
               *f.value.word.os, L"' cannot be called with a definition argument"), line);
         }

         Generator<_list>* list;
         if (parse(uro, tks2, list)) {
            if (stack) {
               return new C_CreateDirectories_List_Stack(list, uro);
            }
            else {
               return new C_CreateDirectories_List(list, force, uro);
            }
         }

         throw SyntaxException(str(L"wrong syntax of command '",
            *word.value.keyword.os, L" ", *f.value.word.os, L"'"), line);
      }
   }

   Generator<_str>* str_;
   if (parse(uro, tks, str_)) {
      if (stack) {
         return new C_Create_String_Stack(str_, uro);
      }
      else {
         return new C_Create_String(str_, force, uro);
      }
   }

   _def* def;
   if (parse(uro, tks, def)) {
      delete def;
      throw SyntaxException(str(L"command '", *word.value.keyword.os,
         L"' cannot be called with a definition argument"), line);
   }

   Generator<_list>* list;
   if (parse(uro, tks, list)) {
      if (stack) {
         return new C_Create_List_Stack(list, uro);
      }
      else {
         return new C_Create_List(list, force, uro);
      }
   }

   commandSyntaxException(*word.value.keyword.os, line);
}

static Command* c_moveTo(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro)
{
   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to' is empty"), line);
   }

   const _boo hasTo = tks.containsKeyword(Keyword::kw_To);
   const _boo hasAs = tks.containsKeyword(Keyword::kw_As);

   if (!hasTo) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os,
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
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _boo extless = false;
         const Token& paf = postAs.first();

         if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxException(str(L"keyword '", *paf.value.keyword.os,
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         Generator<_str>* nname;
         if (!parse(uro, postAs, nname)) {
            throw SyntaxException(str(L"new name in command '", *word.value.keyword.os,
               L" to as' cannot be resolved to a string"), line);
         }

         Generator<_str>* dest;
         if (!parse(uro, preAs, dest)) {
            throw SyntaxException(str(L"new location in command '", *word.value.keyword.os,
               L" to' cannot be resolved to a string"), line);
         }

         uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" to as"), line);
         Attribute* lastAttr = uro->vc.getLastAttribute();
         if (stack) {
            return new C_MoveToAs_Stack(dest, nname, extless, lastAttr, uro);
         }
         else {
            return new C_MoveToAs(dest, nname, force, extless, lastAttr, uro);
         }
      }

      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os,
            L" to' lacks declaration of a new location"), line);
      }

      Generator<_str>* str_;
      if (!parse(uro, right, str_)) {
         throw SyntaxException(str(L"new location in command '",
            *word.value.keyword.os, L" to' cannot be resolved to a string"), line);
      }
      else {
         uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" to"), line);
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
      if (left.containsKeyword(Keyword::kw_As)) {
         throw SyntaxException(str(L"keywords 'to' and 'as' appear in command '",
            *word.value.keyword.os, L" to as' in reverse order"), line);
      }

      Tokens preAs(right);
      Tokens postAs(right);
      right.divideByKeyword(Keyword::kw_As, preAs, postAs);

      if (preAs.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
            L"does not contain declaration of a new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
            L"does not contain declaration of a new name written after keyword 'as'"), line);
      }

      _boo extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"keyword '", *paf.value.keyword.os,
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
      if (!parse(uro, postAs, nname)) {
         throw SyntaxException(str(L"new name in command '", *word.value.keyword.os,
            L" to as' cannot be resolved to a string"), line);
      }

      Generator<_str>* dest;
      if (!parse(uro, preAs, dest)) {
         delete nname;
         throw SyntaxException(str(L"new location in command '", *word.value.keyword.os,
            L" to' cannot be resolved to a string"), line);
      }

      uro->vars.inner.thisState = prevThisState;
      uro->vc.retreatAttribute();

      Generator<_str>* str_;
      if (parse(uro, left, str_)) {
         if (stack) {
            return new CS_StringComArg(str_,
               new C_MoveToAs_Stack(dest, nname, extless, uro), attr, hasMemory, uro);
         }
         else {
            return new CS_StringComArg(str_,
               new C_MoveToAs(dest, nname, force, extless, uro), attr, hasMemory, uro);
         }
      }

      _def* def;
      if (parse(uro, left, def)) {
         if (stack) {
            return new CS_DefinitionComArg(def,
               new C_MoveToAs_Stack(dest, nname, extless, uro), attr, hasMemory, uro);
         }
         else {
            return new CS_DefinitionComArg(def,
               new C_MoveToAs(dest, nname, force, extless, uro), attr, hasMemory, uro);
         }
      }

      Generator<_list>* list;
      if (parse(uro, left, list)) {
         if (stack) {
            return new CS_ListComArg(list,
               new C_MoveToAs_Stack(dest, nname, extless, uro), attr, hasMemory, uro);
         }
         else {
            return new CS_ListComArg(list,
               new C_MoveToAs(dest, nname, force, extless, uro), attr, hasMemory, uro);
         }
      }

      delete nname;
      delete dest;
      throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L" to as'"), line);
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = ThisState::ts_String;
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();
   uro->vc.addAttribute(attr);

   Generator<_str>* dest;
   if (!parse(uro, right, dest)) {
      throw SyntaxException(str(L"new location in command '", *word.value.keyword.os,
         L" to' cannot be resolved to a string"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Generator<_str>* str_;
   if (parse(uro, left, str_)) {
      if (stack) {
         return new CS_StringComArg(str_,
            new C_MoveTo_Stack(dest, uro), attr, hasMemory, uro);
      }
      else {
         return new CS_StringComArg(str_,
            new C_MoveTo(dest, force, uro), attr, hasMemory, uro);
      }
   }

   _def* def;
   if (parse(uro, left, def)) {
      if (stack) {
         return new CS_DefinitionComArg(def,
            new C_MoveTo_Stack(dest, uro), attr, hasMemory, uro);
      }
      else {
         return new CS_DefinitionComArg(def,
            new C_MoveTo(dest, force, uro), attr, hasMemory, uro);
      }
   }

   Generator<_list>* list;
   if (parse(uro, left, list)) {
      if (stack) {
         return new CS_ListComArg(list,
            new C_MoveTo_Stack(dest, uro), attr, hasMemory, uro);
      }
      else {
         return new CS_ListComArg(list,
            new C_MoveTo(dest, force, uro), attr, hasMemory, uro);
      }
   }

   delete dest;
   throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L" to'"), line);
}

static Command* c_downloadFrom(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro)
{
   if (tks.containsKeyword(Keyword::kw_From)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_From, left, right);

      if (left.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L" from' does not "
            L"contain a declaration of elements to download"), line);
      }
      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L" from' does not "
            L"contain a declaration of location"), line);
      }

      Generator<_str>* loc;
      if (!parse(uro, right, loc)) {
         throw SyntaxException(str(L"location in command '", *word.value.keyword.os,
            L" from' cannot be resolved to a string"), line);
      }

      Generator<_str>* str_;
      if (parse(uro, left, str_)) {
         if (stack)
            return new C_DownloadFrom_String_Stack(loc, str_, uro);
         else
            return new C_DownloadFrom_String(loc, str_, force, uro);
      }

      _def* def;
      if (parse(uro, left, def)) {
         if (stack)
            return new C_DownloadFrom_Definition_Stack(loc, def, uro);
         else
            return new C_DownloadFrom_Definition(loc, def, force, uro);
      }

      Generator<_list>* list;
      if (parse(uro, left, list)) {
         if (stack)
            return new C_DownloadFrom_List_Stack(loc, list, uro);
         else
            return new C_DownloadFrom_List(loc, list, force, uro);
      }

      delete loc;
      throw SyntaxException(str(L"wrong syntax of command '",
         *word.value.keyword.os, L" from'"), line);
   }

   Generator<_str>* str_;
   if (parse(uro, tks, str_)) {
      if (stack)
         return new C_Download_String_Stack(str_, uro);
      else
         return new C_Download_String(str_, force, uro);
   }

   _def* def;
   if (parse(uro, tks, def)) {
      delete def;
      throw SyntaxException(str(L"the argument of command '",
         *word.value.keyword.os, L"' cannot be of type 'definition'"), line);
   }

   Generator<_list>* list;
   if (parse(uro, tks, list)) {
      if (stack)
         return new C_Download_List_Stack(list, uro);
      else
         return new C_Download_List(list, force, uro);
   }

   throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L"'"), line);
}

static Command* c_copy(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack, Uroboros* uro)
{
   const _boo hasTo = tks.containsKeyword(Keyword::kw_To);
   const _boo hasAs = tks.containsKeyword(Keyword::kw_As);

   if (!hasTo) {
      if (hasAs) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os,
            L" to as' cannot be called without keyword 'to'"), line);
      }

      if (force) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os,
            L"' cannot be preceded by a flag 'forced'"), line);
      }

      if (stack) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os,
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
         if (parse(uro, tks, str)) {
            return new C_AggrCopy_String(aggr, str, uro);
         }

         Generator<_list>* list;
         if (parse(uro, tks, list)) {
            return new C_AggrCopy_List(aggr, list, uro);
         }
      }
      else {
         if (tks.isEmpty()) {
            commandNoArgException(*word.value.keyword.os, line);
         }

         Generator<_str>* str;
         if (parse(uro, tks, str)) {
            return new C_Copy_String(str, uro);
         }

         Generator<_list>* list;
         if (parse(uro, tks, list)) {
            return new C_Copy_List(list, uro);
         }
      }

      commandSyntaxException(*word.value.keyword.os, line);
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
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _boo extless = false;
         const Token& paf = postAs.first();

         if (paf.isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxException(str(L"keyword '", *paf.value.keyword.os,
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         Generator<_str>* nname;
         if (!parse(uro, postAs, nname)) {
            throw SyntaxException(str(L"new name in command '", *word.value.keyword.os,
               L" to as' cannot be resolved to a string"), line);
         }

         Generator<_str>* dest;
         if (!parse(uro, preAs, dest)) {
            throw SyntaxException(str(L"new location in command '", *word.value.keyword.os,
               L" to' cannot be resolved to a string"), line);
         }

         uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" to as"), line);
         if (stack) {
            return new C_CopyToAs_Stack(dest, nname, true, extless, uro);
         }
         else {
            return new C_CopyToAs(dest, nname, true, force, extless, uro);
         }
      }

      if (right.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os,
            L" to' lacks a declaration of new location"), line);
      }

      Generator<_str>* str_;
      if (parse(uro, right, str_)) {
         uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" to"), line);
         if (stack) {
            return new C_CopyTo_Stack(str_, true, uro);
         }
         else {
            return new C_CopyTo(str_, true, force, uro);
         }
      }
      else {
         throw SyntaxException(str(L"new location in command '",
            *word.value.keyword.os, L" to' cannot be resolved to a string"), line);
      }
   }

   if (hasAs) {
      if (left.containsKeyword(Keyword::kw_As)) {
         throw SyntaxException(str(L"keywords 'to' and 'as' appear in "
            L"command '", *word.value.keyword.os, L" to as' in reverse order"), line);
      }

      Tokens preAs(right);
      Tokens postAs(right);
      right.divideByKeyword(Keyword::kw_As, preAs, postAs);

      if (preAs.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
            L"does not contain a declaration of new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L" to as' "
            L"does not contain a declaration of new name written after keyword 'as'"), line);
      }

      _boo extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxException(str(L"keyword '", *paf.value.keyword.os,
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
      if (!parse(uro, postAs, nname)) {
         throw SyntaxException(str(L"new name in command '", *word.value.keyword.os,
            L" to as' cannot be resolved to a string"), line);
      }

      Generator<_str>* dest;
      if (!parse(uro, preAs, dest)) {
         delete nname;
         throw SyntaxException(str(L"new location in command '", *word.value.keyword.os,
            L" to' cannot be resolved to a string"), line);
      }

      uro->vars.inner.thisState = prevThisState;
      uro->vc.retreatAttribute();

      Generator<_str>* str_;
      if (parse(uro, left, str_)) {
         if (stack) {
            return new CS_StringComArg(str_,
               new C_CopyToAs_Stack(dest, nname, false, extless, uro), attr, hasMemory, uro);
         }
         else {
            return new CS_StringComArg(str_,
               new C_CopyToAs(dest, nname, false, force, extless, uro), attr, hasMemory, uro);
         }
      }

      _def* def;
      if (parse(uro, left, def)) {
         if (stack) {
            return new CS_DefinitionComArg(def,
               new C_CopyToAs_Stack(dest, nname, false, extless, uro), attr, hasMemory, uro);
         }
         else {
            return new CS_DefinitionComArg(def,
               new C_CopyToAs(dest, nname, false, force, extless, uro), attr, hasMemory, uro);
         }
      }

      Generator<_list>* list;
      if (parse(uro, left, list)) {
         if (stack) {
            return new CS_ListComArg(list,
               new C_CopyToAs_Stack(dest, nname, false, extless, uro), attr, hasMemory, uro);
         }
         else {
            return new CS_ListComArg(list,
               new C_CopyToAs(dest, nname, false, force, extless, uro), attr, hasMemory, uro);
         }
      }

      delete nname;
      delete dest;
      throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L" to as'"), line);
   }

   const _boo hasMemory = uro->vc.anyAttribute();
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = ThisState::ts_String;
   Attribute* attr = new Attribute(uro);
   attr->setCoreCommandBase();
   uro->vc.addAttribute(attr);

   Generator<_str>* dest;
   if (!parse(uro, right, dest)) {
      throw SyntaxException(str(L"new location in command '", *word.value.keyword.os,
         L" to' cannot be resolved to a string"), line);
   }

   uro->vars.inner.thisState = prevThisState;
   uro->vc.retreatAttribute();

   Generator<_str>* str_;
   if (parse(uro, left, str_)) {
      if (stack) {
         return new CS_StringComArg(str_,
            new C_CopyTo_Stack(dest, false, uro), attr, hasMemory, uro);
      }
      else {
         return new CS_StringComArg(str_,
            new C_CopyTo(dest, false, force, uro), attr, hasMemory, uro);
      }
   }

   _def* def;
   if (parse(uro, left, def)) {
      if (stack) {
         return new CS_DefinitionComArg(def,
            new C_CopyTo_Stack(dest, false, uro), attr, hasMemory, uro);
      }
      else {
         return new CS_DefinitionComArg(def,
            new C_CopyTo(dest, false, force, uro), attr, hasMemory, uro);
      }
   }

   Generator<_list>* list;
   if (parse(uro, left, list)) {
      if (stack) {
         return new CS_ListComArg(list,
            new C_CopyTo_Stack(dest, false, uro), attr, hasMemory, uro);
      }
      else {
         return new CS_ListComArg(list,
            new C_CopyTo(dest, false, force, uro), attr, hasMemory, uro);
      }
   }

   delete dest;
   throw SyntaxException(str(L"wrong syntax of command '", *word.value.keyword.os, L" to'"), line);
}

Command* c_print(const Token& word, const Tokens& tks, const _int& line, const _boo& directError, Uroboros* uro)
{
   if (tks.isEmpty()) {
      switch (uro->vars.inner.thisState) {
         case ts_None: {
         throw SyntaxException(str(L"command '", *word.value.keyword.os, L"' needs an argument. "
            L"Value of variable 'this' is undefined here"), line);
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
   if (parse(uro, tks, str)) {
      return new C_PrintSingle(str, uro);
   }

   _def* def;
   if (parse(uro, tks, def)) {
      return new C_PrintDefinition(def, uro);
   }

   Generator<_list>* list;
   if (parse(uro, tks, list)) {
      return new C_PrintList(list, uro);
   }

   if (directError) {
      commandSyntaxException(*word.value.keyword.os, line);
   }
   else {
      throw SyntaxException(L"wrong syntax. No command can be formed of this code", line);
   }
}

static Command* c_sleep(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro)
{
   Generator<_per>* per;
   if (parse(uro, tks, per)) {
      return new C_SleepPeriod(per, uro);
   }

   Generator<_num>* num;
   if (parse(uro, tks, num)) {
      return new C_SleepMs(num, uro);
   }

   commandSyntaxException(*word.value.keyword.os, line);
}

static Command* c_run(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro)
{
   uro->vc.markAttributesToRun();

   if (tks.containsKeyword(Keyword::kw_With)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_With, left, right);

      if (right.isEmpty()) {
         throw SyntaxException(str(L"right side of command '", *word.value.keyword.os, L" with' is empty"), line);
      }

      if (left.isEmpty()) {
         if (right.containsKeyword(Keyword::kw_With)) {
            if (uro->vars.inner.thisState == ts_None) {
               throw SyntaxException(str(L"command '", *word.value.keyword.os, L" with with' needs first argument"), line);
            }

            uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" with with"), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();

            Tokens left2(right);
            Tokens right2(right);
            right.divideByKeyword(Keyword::kw_With, left2, right2);

            if (left2.isEmpty()) {
               throw SyntaxException(str(L"command '", *word.value.keyword.os,
                  L"' cannot be called with adjacent 'with' keywords"), line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxException(str(L"command '", *word.value.keyword.os,
                  L" with with' cannot be called without its last argument"), line);
            }

            Generator<_str>* exec;
            if (!parse(uro, left2, exec)) {
               throw SyntaxException(str(L"second argument of command '", *word.value.keyword.os,
                  L" with with' cannot be resolved to a string"), line);
            }

            if (left2.getLength() == 1) {
               const Token& cf = left2.first();
               if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                  delete exec;
                  Generator<_str>* str_;

                  if (parse(uro, right2, str_)) {
                     return new C_RunWithUroborosWithString(str_, lastAttr, uro);
                  }
                  else {
                     Generator<_list>* list;
                     if (parse(uro, right2, list)) {
                        return new C_RunWithUroborosWith(list, lastAttr, uro);
                     }
                     else {
                        delete exec;
                        throw SyntaxException(str(L"last argument of command '",
                           *word.value.keyword.os, L" with Uroboros with' cannot be resolved to a list"), line);
                     }
                  }
               }
            }

            Generator<_str>* str_;
            if (parse(uro, right2, str_)) {
               return new C_RunWithWithString(exec, str_, lastAttr, uro);
            }
            else {
               Generator<_list>* list;
               if (parse(uro, right2, list)) {
                  return new C_RunWithWith(exec, list, lastAttr, uro);
               }
               else {
                  delete exec;
                  throw SyntaxException(str(L"last argument of command '",
                     *word.value.keyword.os, L" with with' cannot be resolved to a list"), line);
               }
            }
         }
         else {
            if (uro->vars.inner.thisState == ts_None) {
               throw SyntaxException(str(L"command '", *word.value.keyword.os,
                  L" with' needs first argument"), line);
            }

            uro->vc.setCoreComAttribute(str(*word.value.keyword.os, L" with"), line);
            Attribute* lastAttr = uro->vc.getLastAttribute();

            Generator<_str>* exec;
            if (parse(uro, right, exec)) {
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
                  *word.value.keyword.os, L" with' cannot be resolved to a string"), line);
            }
         }
      }
      else {
         if (right.containsKeyword(Keyword::kw_With)) {
            Tokens left2(right);
            Tokens right2(right);
            right.divideByKeyword(Keyword::kw_With, left2, right2);

            if (left2.isEmpty()) {
               throw SyntaxException(str(L"command '", *word.value.keyword.os,
                  L"' cannot be called with adjacent 'with' keywords"), line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxException(str(L"command '", *word.value.keyword.os,
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
            if (!parse(uro, left2, exec)) {
               throw SyntaxException(str(L"second argument of command '", *word.value.keyword.os,
                  L" with with' cannot be resolved to a string"), line);
            }

            Generator<_str>* lastStr;
            if (parse(uro, right2, lastStr)) {
               uro->vars.inner.thisState = prevThisState;
               uro->vc.retreatAttribute();

               if (left2.getLength() == 1) {
                  const Token& cf = left2.first();
                  if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                     delete exec;

                     Generator<_str>* str_;
                     if (parse(uro, left, str_)) {
                        return new CS_StringComArg(str_,
                           new C_RunWithUroborosWithString(lastStr, uro), attr, lastAttr, hasMemory, uro);
                     }

                     _def* def;
                     if (parse(uro, left, def)) {
                        return new CS_DefinitionComArg(def,
                           new C_RunWithUroborosWithString(lastStr, uro), attr, lastAttr, hasMemory, uro);
                     }

                     Generator<_list>* list;
                     if (parse(uro, left, list)) {
                        return new CS_ListComArg(list,
                           new C_RunWithUroborosWithString(lastStr, uro), attr, lastAttr, hasMemory, uro);
                     }

                     delete lastStr;
                     throw SyntaxException(str(L"first argument of command '", *word.value.keyword.os,
                        L" with Uroboros with' cannot be resolved to a list"), line);
                  }
               }

               Generator<_str>* str_;
               if (parse(uro, left, str_)) {
                  return new CS_StringComArg(str_,
                     new C_RunWithWithString(exec, lastStr, uro), attr, lastAttr, hasMemory, uro);
               }

               _def* def;
               if (parse(uro, left, def)) {
                  return new CS_DefinitionComArg(def,
                     new C_RunWithWithString(exec, lastStr, uro), attr, lastAttr, hasMemory, uro);
               }

               Generator<_list>* list;
               if (parse(uro, left, list)) {
                  return new CS_ListComArg(list,
                     new C_RunWithWithString(exec, lastStr, uro), attr, lastAttr, hasMemory, uro);
               }

               delete exec;
               delete lastStr;
               throw SyntaxException(str(L"first argument of command '", *word.value.keyword.os,
                  L" with with' cannot be resolved to a list"), line);
            }
            else {
               Generator<_list>* lastList;

               if (!parse(uro, right2, lastList)) {
                  uro->vars.inner.thisState = prevThisState;
                  uro->vc.retreatAttribute();
                  delete exec;
                  throw SyntaxException(str(L"last argument of command '", *word.value.keyword.os,
                     L" with with' cannot be resolved to a list"), line);
               }
               else {
                  uro->vars.inner.thisState = prevThisState;
                  uro->vc.retreatAttribute();

                  if (left2.getLength() == 1) {
                     const Token& cf = left2.first();
                     if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                        delete exec;

                        Generator<_str>* str_;
                        if (parse(uro, left, str_)) {
                           return new CS_StringComArg(str_, new C_RunWithUroborosWith(lastList, uro), attr, lastAttr, hasMemory, uro);
                        }

                        _def* def;
                        if (parse(uro, left, def)) {
                           return new CS_DefinitionComArg(def, new C_RunWithUroborosWith(lastList, uro), attr, lastAttr, hasMemory, uro);
                        }

                        Generator<_list>* list;
                        if (parse(uro, left, list)) {
                           return new CS_ListComArg(list, new C_RunWithUroborosWith(lastList, uro), attr, lastAttr, hasMemory, uro);
                        }

                        delete lastList;
                        throw SyntaxException(str(L"first argument of command '", *word.value.keyword.os,
                           L" with Uroboros with' cannot be resolved to a list"), line);
                     }
                  }

                  Generator<_str>* str_;
                  if (parse(uro, left, str_)) {
                     return new CS_StringComArg(str_,
                        new C_RunWithWith(exec, lastList, uro), attr, lastAttr, hasMemory, uro);
                  }

                  _def* def;
                  if (parse(uro, left, def)) {
                     return new CS_DefinitionComArg(def,
                        new C_RunWithWith(exec, lastList, uro), attr, lastAttr, hasMemory, uro);
                  }

                  Generator<_list>* list;
                  if (parse(uro, left, list)) {
                     return new CS_ListComArg(list,
                        new C_RunWithWith(exec, lastList, uro), attr, lastAttr, hasMemory, uro);
                  }

                  delete exec;
                  delete lastList;
                  throw SyntaxException(str(L"first argument of command '", *word.value.keyword.os,
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
            if (!parse(uro, right, exec)) {
               throw SyntaxException(str(L"last argument of command '", *word.value.keyword.os,
                  L" with' cannot be resolved to a string"), line);
            }

            uro->vars.inner.thisState = prevThisState;
            uro->vc.retreatAttribute();

            if (right.getLength() == 1) {
               const Token& cf = right.first();
               if (cf.type == Token::t_Word && cf.value.word.h == uro->hashes.HASH_VAR_UROBOROS) {
                  delete exec;

                  Generator<_str>* str_;
                  if (parse(uro, left, str_)) {
                     return new CS_StringComArg(str_, new C_RunWithUroboros(uro), attr, lastAttr, hasMemory, uro);
                  }

                  _def* def;
                  if (parse(uro, left, def)) {
                     return new CS_DefinitionComArg(def, new C_RunWithUroboros(uro), attr, lastAttr, hasMemory, uro);
                  }

                  Generator<_list>* list;
                  if (parse(uro, left, list)) {
                     return new CS_ListComArg(list, new C_RunWithUroboros(uro), attr, lastAttr, hasMemory, uro);
                  }

                  throw SyntaxException(str(L"first argument of command '", *word.value.keyword.os,
                     L" with Uroboros' cannot be resolved to a list"), line);
               }
            }

            Generator<_str>* str_;
            if (parse(uro, left, str_)) {
               return new CS_StringComArg(str_, new C_RunWith(exec, uro), attr, lastAttr, hasMemory, uro);
            }

            _def* def;
            if (parse(uro, left, def)) {
               return new CS_DefinitionComArg(def, new C_RunWith(exec, uro), attr, lastAttr, hasMemory, uro);
            }

            Generator<_list>* list;
            if (parse(uro, left, list)) {
               return new CS_ListComArg(list, new C_RunWith(exec, uro), attr, lastAttr, hasMemory, uro);
            }

            delete exec;
            throw SyntaxException(str(L"first argument of command '", *word.value.keyword.os,
               L" with' cannot be resolved to a list"), line);
         }
      }
   }
   else {
      Attribute* lastAttr = uro->vc.getLastAttribute();
      Generator<_str>* str;
      if (parse(uro, tks, str)) {
         return new C_Run(str, lastAttr, uro);
      }
   }

   commandSyntaxException(*word.value.keyword.os, line);
}

static Command* c_error(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro)
{
   if (tks.isEmpty()) {
      return new C_Error(uro);
   }

   Generator<_num>* num;

   if (parse(uro, tks, num)) {
      return new C_ErrorWithExitCode(num, uro);
   }
   else {
      throw SyntaxException(str(L"the argument of command '", *word.value.keyword.os,
         L"' cannot be resolved to a number"), line);
   }
}

static Command* c_process(const Token& word, const Tokens& tks, const _int& line, Uroboros* uro)
{
   uro->vc.markAttributesToRun();

   if (tks.isEmpty()) {
      throw SyntaxException(str(L"command '", *word.value.keyword.os,
         L"' needs a string argument"), line);
   }

   Generator<_str>* str_;

   if (parse(uro, tks, str_)) {
      return new C_Process(str_, uro);
   }
   else {
      throw SyntaxException(str(L"the argument of command '", *word.value.keyword.os,
         L"' cannot be resolved to a string"), line);
   }
}

static void checkUselessFlags(const Token& word, const _int& line,
   const bool& force, const bool& stack)
{
   if (force) {
      throw SyntaxException(str(L"keyword '", *word.value.keyword.os,
         L"' cannot be preceded by a flag 'forced'"), line);
   }

   if (stack) {
      throw SyntaxException(str(L"keyword '", *word.value.keyword.os,
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
