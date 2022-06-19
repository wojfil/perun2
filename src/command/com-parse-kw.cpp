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
   const _int& line, const bool& force, const bool& stack)
{
   switch (word.value.k) {
      case Keyword::kw_Delete:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Unlock:
      case Keyword::kw_Unhide: {
         checkUselessFlags(word, line, force, stack);
         return kwCommandSimple(word, tks, line);
      }
      case Keyword::kw_Open: {
         checkUselessFlags(word, line, force, stack);
         return c_open(word, tks, line);
      }
      case Keyword::kw_Copy: {
         return c_copy(word, tks, line, force, stack);
      }
      case Keyword::kw_Create: {
         return c_create(word, tks, line, force, stack);
      }
      case Keyword::kw_Download: {
         return c_downloadFrom(word, tks, line, force, stack);
      }
      case Keyword::kw_Break:
      case Keyword::kw_Continue:
      case Keyword::kw_Exit: {
         checkUselessFlags(word, line, force, stack);
         throw SyntaxException(L"command '" + word.originString + L"' cannot be called with an argument", line);
      }
      case Keyword::kw_Error: {
         checkUselessFlags(word, line, force, stack);
         return c_error(word, tks, line);
      }
      case Keyword::kw_Move: {
         return c_moveTo(word, tks, line, force, stack);
      }
      case Keyword::kw_Print: {
         checkUselessFlags(word, line, force, stack);
         return c_print(word, tks, line, true);
      }
      case Keyword::kw_Reaccess:
      case Keyword::kw_Recreate:
      case Keyword::kw_Rechange:
      case Keyword::kw_Remodify: {
         checkUselessFlags(word, line, force, stack);
         return kwCommandTime(word, tks, line);
      }
      case Keyword::kw_Rename: {
         return c_rename(word, tks, line, force, stack);
      }
      case Keyword::kw_Run: {
         checkUselessFlags(word, line, force, stack);
         return c_run(word, tks, line);
      }
      case Keyword::kw_Select: {
         checkUselessFlags(word, line, force, stack);
         return c_select(word, tks, line);
      }
      case Keyword::kw_Sleep: {
         checkUselessFlags(word, line, force, stack);
         return c_sleep(word, tks, line);
      }
      case Keyword::kw_Process: {
         checkUselessFlags(word, line, force, stack);
         return c_process(word, tks, line);
      }
   }

   throw SyntaxException(L"command cannot start with a keyword '" + word.originString + L"'", line);
}

static Command* kwCommandSimple(const Token& word, Tokens& tks,
   const _int& line)
{
   if (tks.isEmpty()) {
      setCoreComAttribute(word.originString, line);
      return coreCommandSimpleSave(word);
   }

   const _boo hasMemory = anyAttribute();
   Attribute* attr = new Attribute();
   attr->setCoreCommandBase();

   Generator<_str>* str;
   if (parse(tks, str)) {
      return new CS_StringComArg(str, coreCommandSimpleNoSave(word), attr, hasMemory);
   }

   _def* def;
   if (parse(tks, def)) {
      return new CS_DefinitionComArg(def, coreCommandSimpleNoSave(word), attr, hasMemory);
   }

   Generator<_list>* list;
   if (parse(tks, list)) {
      return new CS_ListComArg(list, coreCommandSimpleNoSave(word), attr, hasMemory);
   }

   throw SyntaxException(L"wrong syntax of command '" + word.originString + L"'", line);
}

static Command* coreCommandSimpleSave(const Token& word)
{
   switch (word.value.k) {
      case Keyword::kw_Delete: {
         Attribute* lastAttr = getLastAttribute();
         return new C_Delete(lastAttr);
      }
      case Keyword::kw_Drop: {
         Attribute* lastAttr = getLastAttribute();
         return new C_Drop(lastAttr);
      }
      case Keyword::kw_Hide:
         return new C_Hide(true);
      case Keyword::kw_Lock:
         return new C_Lock(true);
      case Keyword::kw_Unlock:
         return new C_Unlock(true);
      case Keyword::kw_Unhide:
         return new C_Unhide(true);
      default:
         return nullptr; // error
   }
}

static Command* coreCommandSimpleNoSave(const Token& word)
{
   switch (word.value.k) {
      case Keyword::kw_Delete:
         return new C_Delete();
      case Keyword::kw_Drop:
         return new C_Drop();
      case Keyword::kw_Hide:
         return new C_Hide(false);
      case Keyword::kw_Lock:
         return new C_Lock(false);
      case Keyword::kw_Unlock:
         return new C_Unlock(false);
      case Keyword::kw_Unhide:
         return new C_Unhide(false);
      default:
         return nullptr; // error
   }
}

static Command* kwCommandTime(const Token& word, Tokens& tks, const _int& line)
{
   if (tks.isEmpty()) {
      throw SyntaxException(L"command '" + word.originString + L" to' is empty",
         line);
   }

   if (!tks.containsKeyword(Keyword::kw_To)) {
      throw SyntaxException(L"command '" + word.originString +
         L" to' does not contain keyword 'to'", line);
   }

   Tokens left(tks.list);
   Tokens right(tks.list);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (right.isEmpty()) {
      throw SyntaxException(L"command '" + word.originString +
         L" to' does not contain its time argument", line);
   }


   if (left.isEmpty()) {
      setCoreComAttribute(word.originString + L" to", line);
      Generator<_tim>* tim;
      if (!parse(right, tim)) {
         throw SyntaxException(L"time argument of command '" + word.originString +
            L" to' is not valid", line);
      }

      switch (word.value.k) {
         case Keyword::kw_Reaccess: {
            Token t(HASH_VAR_ACCESS, line, L"access");
            setAttribute(t);
            break;
         }
         case Keyword::kw_Recreate: {
            Token t(HASH_VAR_CREATION, line, L"creation");
            setAttribute(t);
            break;
         }
         case Keyword::kw_Rechange: {
            Token t(HASH_VAR_CHANGE, line, L"change");
            setAttribute(t);
            break;
         }
         case Keyword::kw_Remodify: {
            Token t(HASH_VAR_MODIFICATION, line, L"modification");
            setAttribute(t);
            break;
         }
         default: {
            break;
         }
      }
      return coreCommandTime(word, tim, true);
   }

   const _boo hasMemory = anyAttribute();
   const ThisState prevThisState = g_thisstate;
   g_thisstate = ThisState::ts_String;
   Attribute* attr = new Attribute();
   attr->setCoreCommandBase();
   addAttribute(attr);

   switch (word.value.k) {
      case Keyword::kw_Reaccess: {
         Token t(HASH_VAR_ACCESS, line, L"access");
         setAttribute(t);
         break;
      }
      case Keyword::kw_Recreate: {
         Token t(HASH_VAR_CREATION, line, L"creation");
         setAttribute(t);
         break;
      }
      case Keyword::kw_Rechange: {
         Token t(HASH_VAR_CHANGE, line, L"change");
         setAttribute(t);
         break;
      }
      case Keyword::kw_Remodify: {
         Token t(HASH_VAR_MODIFICATION, line, L"modification");
         setAttribute(t);
         break;
      }
      default: {
         break;
      }
   }

   Generator<_tim>* tim;
   if (!parse(right, tim)) {
      throw SyntaxException(L"time argument of command '" + word.originString +
         L" to' is not valid", line);
   }

   g_thisstate = prevThisState;
   retreatAttribute();

   Generator<_str>* str;
   if (parse(left, str)) {
      return new CS_StringComArg(str, coreCommandTime(word, tim, false), attr, hasMemory);
   }

   _def* def;
   if (parse(left, def)) {
      return new CS_DefinitionComArg(def, coreCommandTime(word, tim, false), attr, hasMemory);
   }

   Generator<_list>* list;
   if (parse(left, list)) {
      return new CS_ListComArg(list, coreCommandTime(word, tim, false), attr, hasMemory);
   }

   throw SyntaxException(L"wrong syntax of command '" + word.originString + L" to'", line);
}

static Command* coreCommandTime(const Token& word,
   Generator<_tim>* time, const _boo& saveChanges)
{
   switch (word.value.k) {
      case Keyword::kw_Reaccess:
         return new C_ReaccessTo(time, saveChanges);
      case Keyword::kw_Rechange:
         return new C_RechangeTo(time, saveChanges);
      case Keyword::kw_Recreate:
         return new C_RecreateTo(time, saveChanges);
      case Keyword::kw_Remodify:
         return new C_RemodifyTo(time, saveChanges);
      default:
         return nullptr; // error
   }
}

static Command* c_open(const Token& word, const Tokens& tks, const _int& line)
{
   if (tks.isEmpty()) {
      setCoreComAttribute(word.originString, line);
      return new C_Open();
   }

   if (tks.containsKeyword(Keyword::kw_With)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_With, left, right);

      if (right.isEmpty()) {
         throw SyntaxException(L"command '" +word.originString + L" with' does not "
            L"contain its last argument", line);
      }

      Generator<_str>* prog;
      if (!parse(right, prog)) {
         throw SyntaxException(L"last argument of command '" + word.originString + L" with' "
            L"cannot be resolved to a string", line);
      }

      if (left.isEmpty()) {
         setCoreComAttribute(word.originString + L" with", line);
         return new C_OpenWith(prog);
      }
      else {
         const _boo hasMemory = anyAttribute();
         Attribute* attr = new Attribute();
         attr->setCoreCommandBase();

         Generator<_str>* str;
         if (parse(left, str)) {
            return new CS_StringComArg(str, new C_OpenWith(prog), attr, hasMemory);
         }

         _def* def;
         if (parse(left, def)) {
            return new CS_DefinitionComArg(def, new C_OpenWith(prog), attr, hasMemory);
         }

         Generator<_list>* list;
         if (parse(left, list)) {
            return new CS_ListComArg(list, new C_OpenWith(prog), attr, hasMemory);
         }

         delete prog;
         throw SyntaxException(L"wrong syntax of command '" + word.originString + L" with'", line);
      }
   }

   const _boo hasMemory = anyAttribute();
   Attribute* attr = new Attribute();
   attr->setCoreCommandBase();

   Generator<_str>* str;
   if (parse(tks, str)) {
      return new CS_StringComArg(str, new C_Open(), attr, hasMemory);
   }

   _def* def;
   if (parse(tks, def)) {
      return new CS_DefinitionComArg(def, new C_Open(), attr, hasMemory);
   }

   Generator<_list>* list;
   if (parse(tks, list)) {
      return new CS_ListComArg(list, new C_Open(), attr, hasMemory);
   }

   commandSyntaxException(word.originString, line);
}

static Command* c_select(const Token& word, const Tokens& tks, const _int& line)
{
   if (g_aggrs.empty()) {
      if (tks.isEmpty()) {
         commandNoArgException(word.originString, line);
      }

      Generator<_str>* str;
      if (parse(tks, str)) {
         return new C_Select_String(str);
      }

      Generator<_list>* list;
      if (parse(tks, list)) {
         return new C_Select_List(list);
      }
   }
   else {
      Aggregate* aggr = getLastAggregate();
      aggr->set(AGGR_SELECT);

      if (tks.isEmpty()) {
         Generator<_str>* str = new ThisReference();
         return new C_AggrSelect_String(aggr, str);
      }

      Generator<_str>* str;
      if (parse(tks, str)) {
         return new C_AggrSelect_String(aggr, str);
      }

      Generator<_list>* list;
      if (parse(tks, list)) {
         return new C_AggrSelect_List(aggr, list);
      }
   }

   commandSyntaxException(word.originString, line);
}

static Command* c_rename(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack)
{
   if (tks.isEmpty()) {
      throw SyntaxException(L"command '" + word.originString + L" to' is empty",
         line);
   }

   if (!tks.containsKeyword(Keyword::kw_To)) {
      throw SyntaxException(L"command '" + word.originString + L" to' "
         L"does not contain keyword 'to'", line);
   }

   Tokens left(tks.list);
   Tokens right(tks.list);
   tks.divideByKeyword(Keyword::kw_To, left, right);

   if (right.isEmpty()) {
      throw SyntaxException(L"command '" + word.originString + L" to' "
         L"does not contain declaration of new name", line);
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
      setCoreComAttribute(word.originString + L" to", line);

      Generator<_str>* newName;
      if (!parse(right, newName)) {
         throw SyntaxException(L"declaration of new name in command '" + word.originString + L" to' is not valid", line);
      }

      Attribute* lastAttr = getLastAttribute();
      lastAttr->markToEvaluate = true;

      if (stack) {
         return new C_RenameTo_Stack(newName, true, extless);
      }
      else {
         return new C_RenameTo(newName, true, force, extless);
      }
   }

   const _boo hasMemory = anyAttribute();
   const ThisState prevThisState = g_thisstate;
   g_thisstate = ThisState::ts_String;
   Attribute* attr = new Attribute();
   attr->setCoreCommandBase();
   addAttribute(attr);

   Generator<_str>* newName;
   if (!parse(right, newName)) {
      throw SyntaxException(L"declaration of new name in command '" + word.originString + L" to' is not valid", line);
   }

   g_thisstate = prevThisState;
   retreatAttribute();

   Generator<_str>* str;
   if (parse(left, str)) {
      if (stack)
         return new CS_StringComArg(str,
            new C_RenameTo_Stack(newName, false, extless), attr, hasMemory);
      else
         return new CS_StringComArg(str,
            new C_RenameTo(newName, false, force, extless), attr, hasMemory);
   }

   Generator<_list>* list;
   if (parse(left, list)) {
      if (stack)
         return new CS_ListComArg(list,
            new C_RenameTo_Stack(newName, false, extless), attr, hasMemory);
      else
         return new CS_ListComArg(list,
            new C_RenameTo(newName, false, force, extless), attr, hasMemory);
   }

   commandSyntaxException(word.originString + L" to", line);
}

static Command* c_create(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack)
{
   if (tks.isEmpty()) {
      setCoreComAttribute(word.originString, line);
      Attribute* lastAttr = getLastAttribute();

      if (stack) {
         return new C_Create_Stack(lastAttr);
      }
      else {
         return new C_Create(force, lastAttr);
      }
   }

   const Token& f = tks.first();

   if (f.type == Token::t_Word) {
      Tokens tks2 = tks;
      tks2.trimLeft();
      const _size& fk = f.value.h1;

      if (fk == HASH_VAR_FILE) {
         if (tks2.isEmpty()) {
            setCoreComAttribute(word.originString + L" " + f.originString, line);
            Attribute* lastAttr = getLastAttribute();

            if (stack) {
               return new C_CreateFile_Stack(lastAttr);
            }
            else {
               return new C_CreateFile(force, lastAttr);
            }
         }

         Generator<_str>* str;
         if (parse(tks2, str)) {
            if (stack) {
               return new C_CreateFile_String_Stack(str);
            }
            else {
               return new C_CreateFile_String(str, force);
            }
         }

         throw SyntaxException(L"argument of command '" + word.originString + L" " + f.originString + L"' cannot be resolved to a string", line);
      }
      else if (fk == HASH_VAR_FILES) {
         if (tks2.isEmpty()) {
            throw SyntaxException(L"command '" + word.originString + L" " + f.originString + L"' needs an argument", line);
         }

         Generator<_str>* str;
         if (parse(tks2, str)) {
            delete str;
            const _str sub = f.originString.substr(0, 4);
            throw SyntaxException(L"write '" + word.originString + L" " + sub + L"' instead of '" + word.originString + L" " + f.originString + L"'", line);
         }

         _def* def;
         if (parse(tks2, def)) {
            delete def;
            throw SyntaxException(L"command '" + word.originString + L" " + f.originString + L"' cannot be called with a definition argument", line);
         }

         Generator<_list>* list;
         if (parse(tks2, list)) {
            if (stack) {
               return new C_CreateFiles_List_Stack(list);
            }
            else {
               return new C_CreateFiles_List(list, force);
            }
         }

         throw SyntaxException(L"wrong syntax of command '" + word.originString + L" " + f.originString + L"'", line);
      }
      else if (fk == HASH_VAR_DIRECTORY) {
         if (tks2.isEmpty()) {
            setCoreComAttribute(word.originString + L" " + f.originString, line);
            Attribute* lastAttr = getLastAttribute();
            if (stack) {
               return new C_CreateDirectory_Stack(lastAttr);
            }
            else {
               return new C_CreateDirectory(force, lastAttr);
            }
         }

         Generator<_str>* str;
         if (parse(tks2, str)) {
            if (stack) {
               return new C_CreateDirectory_String_Stack(str);
            }
            else {
               return new C_CreateDirectory_String(str, force);
            }
         }

         throw SyntaxException(L"argument of command '" + word.originString + L" " + f.originString + L"' cannot be resolved to a string", line);
      }
      else if (fk == HASH_VAR_DIRECTORIES) {
         if (tks2.isEmpty()) {
            throw SyntaxException(L"command '" + word.originString + L" " + f.originString + L"' needs an argument", line);
         }

         Generator<_str>* str;
         if (parse(tks2, str)) {
            delete str;
            const _str sub = f.originString.substr(0, 8) + L"y";
            throw SyntaxException(L"write '" + word.originString + L" " + sub + L"' instead of '" + word.originString + L" " + f.originString + L"'", line);
         }

         _def* def;
         if (parse(tks2, def)) {
            delete def;
            throw SyntaxException(L"command '" + word.originString + L" " + f.originString + L"' cannot be called with a definition argument", line);
         }

         Generator<_list>* list;
         if (parse(tks2, list)) {
            if (stack) {
               return new C_CreateDirectories_List_Stack(list);
            }
            else {
               return new C_CreateDirectories_List(list, force);
            }
         }

         throw SyntaxException(L"wrong syntax of command '" + word.originString + L" " + f.originString + L"'", line);
      }
   }

   Generator<_str>* str;
   if (parse(tks, str)) {
      if (stack) {
         return new C_Create_String_Stack(str);
      }
      else {
         return new C_Create_String(str, force);
      }
   }

   _def* def;
   if (parse(tks, def)) {
      delete def;
      throw SyntaxException(L"command '" + word.originString + L"' cannot be called with a definition argument", line);
   }

   Generator<_list>* list;
   if (parse(tks, list)) {
      if (stack) {
         return new C_Create_List_Stack(list);
      }
      else {
         return new C_Create_List(list, force);
      }
   }

   commandSyntaxException(word.originString, line);
}

static Command* c_moveTo(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack)
{
   if (tks.isEmpty()) {
      throw SyntaxException(L"command '" + word.originString + L" to' is empty", line);
   }

   const _boo hasTo = tks.containsKeyword(Keyword::kw_To);
   const _boo hasAs = tks.containsKeyword(Keyword::kw_As);

   if (!hasTo) {
      throw SyntaxException(L"command '" + word.originString + L" to' cannot be called without keyword 'to'", line);
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
            throw SyntaxException(L"command '" + word.originString + L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'", line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxException(L"command '" + word.originString + L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'", line);
         }

         _boo extless = false;

         if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxException(L"keyword '" + postAs.first().originString + L"' is not followed by a declaration of new file name", line);
            }
         }

         Generator<_str>* nname;
         if (!parse(postAs, nname)) {
            throw SyntaxException(L"new name in command '" + word.originString + L" to as' cannot be resolved to a string", line);
         }

         Generator<_str>* dest;
         if (!parse(preAs, dest)) {
            throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
         }

         setCoreComAttribute(word.originString + L" to as", line);
         Attribute* lastAttr = getLastAttribute();
         if (stack) {
            return new C_MoveToAs_Stack(dest, nname, extless, lastAttr);
         }
         else {
            return new C_MoveToAs(dest, nname, force, extless, lastAttr);
         }
      }

      if (right.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" to' lacks declaration of a new location", line);
      }

      Generator<_str>* str;
      if (!parse(right, str)) {
         throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
      }
      else {
         setCoreComAttribute(word.originString + L" to", line);
         Attribute* lastAttr = getLastAttribute();
         if (stack) {
            return new C_MoveTo_Stack(str, lastAttr);
         }
         else {
            return new C_MoveTo(str, force, lastAttr);
         }
      }
   }

   if (hasAs) {
      if (left.containsKeyword(Keyword::kw_As)) {
         throw SyntaxException(L"keywords 'to' and 'as' appear in command '"
            + word.originString + L" to as' in reverse order", line);
      }

      Tokens preAs(right);
      Tokens postAs(right);
      right.divideByKeyword(Keyword::kw_As, preAs, postAs);

      if (preAs.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" to as' "
            L"does not contain declaration of a new location written between "
            L"keywords 'to' and 'as'", line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" to as' "
            L"does not contain declaration of a new name written after keyword 'as'", line);
      }

      _boo extless = false;

      if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxException(L"keyword '" + postAs.first().originString + L"' is not followed by a declaration of a new file name", line);
         }
      }

      const _boo hasMemory = anyAttribute();
      const ThisState prevThisState = g_thisstate;
      g_thisstate = ThisState::ts_String;
      Attribute* attr = new Attribute();
      attr->setCoreCommandBase();
      addAttribute(attr);

      Generator<_str>* nname;
      if (!parse(postAs, nname)) {
         throw SyntaxException(L"new name in command '" + word.originString + L" to as' cannot be resolved to a string", line);
      }

      Generator<_str>* dest;
      if (!parse(preAs, dest)) {
         delete nname;
         throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
      }

      g_thisstate = prevThisState;
      retreatAttribute();

      Generator<_str>* str;
      if (parse(left, str)) {
         if (stack) {
            return new CS_StringComArg(str,
               new C_MoveToAs_Stack(dest, nname, extless), attr, hasMemory);
         }
         else {
            return new CS_StringComArg(str,
               new C_MoveToAs(dest, nname, force, extless), attr, hasMemory);
         }
      }

      _def* def;
      if (parse(left, def)) {
         if (stack) {
            return new CS_DefinitionComArg(def,
               new C_MoveToAs_Stack(dest, nname, extless), attr, hasMemory);
         }
         else {
            return new CS_DefinitionComArg(def,
               new C_MoveToAs(dest, nname, force, extless), attr, hasMemory);
         }
      }

      Generator<_list>* list;
      if (parse(left, list)) {
         if (stack) {
            return new CS_ListComArg(list,
               new C_MoveToAs_Stack(dest, nname, extless), attr, hasMemory);
         }
         else {
            return new CS_ListComArg(list,
               new C_MoveToAs(dest, nname, force, extless), attr, hasMemory);
         }
      }

      delete nname;
      delete dest;
      throw SyntaxException(L"wrong syntax of command '" + word.originString + L" to as'", line);
   }

   const _boo hasMemory = anyAttribute();
   const ThisState prevThisState = g_thisstate;
   g_thisstate = ThisState::ts_String;
   Attribute* attr = new Attribute();
   attr->setCoreCommandBase();
   addAttribute(attr);

   Generator<_str>* dest;
   if (!parse(right, dest)) {
      throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
   }

   g_thisstate = prevThisState;
   retreatAttribute();

   Generator<_str>* str;
   if (parse(left, str)) {
      if (stack) {
         return new CS_StringComArg(str,
            new C_MoveTo_Stack(dest), attr, hasMemory);
      }
      else {
         return new CS_StringComArg(str,
            new C_MoveTo(dest, force), attr, hasMemory);
      }
   }

   _def* def;
   if (parse(left, def)) {
      if (stack) {
         return new CS_DefinitionComArg(def,
            new C_MoveTo_Stack(dest), attr, hasMemory);
      }
      else {
         return new CS_DefinitionComArg(def,
            new C_MoveTo(dest, force), attr, hasMemory);
      }
   }

   Generator<_list>* list;
   if (parse(left, list)) {
      if (stack) {
         return new CS_ListComArg(list,
            new C_MoveTo_Stack(dest), attr, hasMemory);
      }
      else {
         return new CS_ListComArg(list,
            new C_MoveTo(dest, force), attr, hasMemory);
      }
   }

   delete dest;
   throw SyntaxException(L"wrong syntax of command '" + word.originString + L" to'", line);
}

static Command* c_downloadFrom(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack)
{
   if (tks.containsKeyword(Keyword::kw_From)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_From, left, right);

      if (left.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" from' does not "
            L"contain a declaration of elements to download", line);
      }
      if (right.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" from' does not "
            L"contain a declaration of location", line);
      }

      Generator<_str>* loc;
      if (!parse(right, loc)) {
         throw SyntaxException(L"location in command '" + word.originString + L" from' cannot be resolved to a string", line);
      }

      Generator<_str>* str;
      if (parse(left, str)) {
         if (stack)
            return new C_DownloadFrom_String_Stack(loc, str);
         else
            return new C_DownloadFrom_String(loc, str, force);
      }

      _def* def;
      if (parse(left, def)) {
         if (stack)
            return new C_DownloadFrom_Definition_Stack(loc, def);
         else
            return new C_DownloadFrom_Definition(loc, def, force);
      }

      Generator<_list>* list;
      if (parse(left, list)) {
         if (stack)
            return new C_DownloadFrom_List_Stack(loc, list);
         else
            return new C_DownloadFrom_List(loc, list, force);
      }

      delete loc;
      throw SyntaxException(L"wrong syntax of command '" + word.originString + L" from'", line);
   }

   Generator<_str>* str;
   if (parse(tks, str)) {
      if (stack)
         return new C_Download_String_Stack(str);
      else
         return new C_Download_String(str, force);
   }

   _def* def;
   if (parse(tks, def)) {
      delete def;
      throw SyntaxException(L"the argument of command '" + word.originString + L"' cannot be of type 'definition'", line);
   }

   Generator<_list>* list;
   if (parse(tks, list)) {
      if (stack)
         return new C_Download_List_Stack(list);
      else
         return new C_Download_List(list, force);
   }

   throw SyntaxException(L"wrong syntax of command '" + word.originString + L"'", line);
}

static Command* c_copy(const Token& word, const Tokens& tks, const _int& line,
   const bool& force, const bool& stack)
{
   const _boo hasTo = tks.containsKeyword(Keyword::kw_To);
   const _boo hasAs = tks.containsKeyword(Keyword::kw_As);

   if (!hasTo) {
      if (hasAs) {
         throw SyntaxException(L"command '" + word.originString + L" to as' cannot be called without keyword 'to'", line);
      }

      if (force) {
         throw SyntaxException(
            L"command '" + word.originString + L"' cannot be preceded by a flag 'forced'", line);
      }

      if (stack) {
         throw SyntaxException(
            L"command '" + word.originString + L"' cannot be preceded by a flag 'stack'", line);
      }

      if (g_aggrs.empty()) {
         if (tks.isEmpty()) {
            commandNoArgException(word.originString, line);
         }

         Generator<_str>* str;
         if (parse(tks, str)) {
            return new C_Copy_String(str);
         }

         Generator<_list>* list;
         if (parse(tks, list)) {
            return new C_Copy_List(list);
         }
      }
      else {
         Aggregate* aggr = getLastAggregate();
         aggr->set(AGGR_COPY);

         if (tks.isEmpty()) {
            Generator<_str>* str = new ThisReference();
            return new C_AggrCopy_String(aggr, str);
         }

         Generator<_str>* str;
         if (parse(tks, str)) {
            return new C_AggrCopy_String(aggr, str);
         }

         Generator<_list>* list;
         if (parse(tks, list)) {
            return new C_AggrCopy_List(aggr, list);
         }
      }

      commandSyntaxException(word.originString, line);
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
            throw SyntaxException(L"command '" + word.originString + L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'", line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxException(L"command '" + word.originString + L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'", line);
         }

         _boo extless = false;

         if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxException(L"keyword '" + postAs.first().originString + L"' is not followed by a declaration of new file name", line);
            }
         }

         Generator<_str>* nname;
         if (!parse(postAs, nname)) {
            throw SyntaxException(L"new name in command '" + word.originString + L" to as' cannot be resolved to a string", line);
         }

         Generator<_str>* dest;
         if (!parse(preAs, dest)) {
            throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
         }

         setCoreComAttribute(word.originString + L" to as", line);
         if (stack) {
            return new C_CopyToAs_Stack(dest, nname, true, extless);
         }
         else {
            return new C_CopyToAs(dest, nname, true, force, extless);
         }
      }

      if (right.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" to' lacks a declaration of new location", line);
      }

      Generator<_str>* str;
      if (parse(right, str)) {
         setCoreComAttribute(word.originString + L" to", line);
         if (stack) {
            return new C_CopyTo_Stack(str, true);
         }
         else {
            return new C_CopyTo(str, true, force);
         }
      }
      else {
         throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
      }
   }

   if (hasAs) {
      if (left.containsKeyword(Keyword::kw_As)) {
         throw SyntaxException(L"keywords 'to' and 'as' appear in "
            L"command '" + word.originString + L" to as' in reverse order", line);
      }

      Tokens preAs(right);
      Tokens postAs(right);
      right.divideByKeyword(Keyword::kw_As, preAs, postAs);

      if (preAs.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" to as' "
            L"does not contain a declaration of new location written between "
            L"keywords 'to' and 'as'", line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxException(L"command '" + word.originString + L" to as' "
            L"does not contain a declaration of new name written after keyword 'as'", line);
      }

      _boo extless = false;

      if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxException(L"keyword '" + postAs.first().originString +
               L"' is not followed by a declaration of new file name", line);
         }
      }

      const _boo hasMemory = anyAttribute();
      const ThisState prevThisState = g_thisstate;
      g_thisstate = ThisState::ts_String;
      Attribute* attr = new Attribute();
      attr->setCoreCommandBase();
      addAttribute(attr);

      Generator<_str>* nname;
      if (!parse(postAs, nname)) {
         throw SyntaxException(L"new name in command '" + word.originString + L" to as' cannot be resolved to a string", line);
      }

      Generator<_str>* dest;
      if (!parse(preAs, dest)) {
         delete nname;
         throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
      }

      g_thisstate = prevThisState;
      retreatAttribute();

      Generator<_str>* str;
      if (parse(left, str)) {
         if (stack) {
            return new CS_StringComArg(str,
               new C_CopyToAs_Stack(dest, nname, false, extless), attr, hasMemory);
         }
         else {
            return new CS_StringComArg(str,
               new C_CopyToAs(dest, nname, false, force, extless), attr, hasMemory);
         }
      }

      _def* def;
      if (parse(left, def)) {
         if (stack) {
            return new CS_DefinitionComArg(def,
               new C_CopyToAs_Stack(dest, nname, false, extless), attr, hasMemory);
         }
         else {
            return new CS_DefinitionComArg(def,
               new C_CopyToAs(dest, nname, false, force, extless), attr, hasMemory);
         }
      }

      Generator<_list>* list;
      if (parse(left, list)) {
         if (stack) {
            return new CS_ListComArg(list,
               new C_CopyToAs_Stack(dest, nname, false, extless), attr, hasMemory);
         }
         else {
            return new CS_ListComArg(list,
               new C_CopyToAs(dest, nname, false, force, extless), attr, hasMemory);
         }
      }

      delete nname;
      delete dest;
      throw SyntaxException(L"wrong syntax of command '" + word.originString + L" to as'", line);
   }

   const _boo hasMemory = anyAttribute();
   const ThisState prevThisState = g_thisstate;
   g_thisstate = ThisState::ts_String;
   Attribute* attr = new Attribute();
   attr->setCoreCommandBase();
   addAttribute(attr);

   Generator<_str>* dest;
   if (!parse(right, dest)) {
      throw SyntaxException(L"new location in command '" + word.originString + L" to' cannot be resolved to a string", line);
   }

   g_thisstate = prevThisState;
   retreatAttribute();

   Generator<_str>* str;
   if (parse(left, str)) {
      if (stack) {
         return new CS_StringComArg(str,
            new C_CopyTo_Stack(dest, false), attr, hasMemory);
      }
      else {
         return new CS_StringComArg(str,
            new C_CopyTo(dest, false, force), attr, hasMemory);
      }
   }

   _def* def;
   if (parse(left, def)) {
      if (stack) {
         return new CS_DefinitionComArg(def,
            new C_CopyTo_Stack(dest, false), attr, hasMemory);
      }
      else {
         return new CS_DefinitionComArg(def,
            new C_CopyTo(dest, false, force), attr, hasMemory);
      }
   }

   Generator<_list>* list;
   if (parse(left, list)) {
      if (stack) {
         return new CS_ListComArg(list,
            new C_CopyTo_Stack(dest, false), attr, hasMemory);
      }
      else {
         return new CS_ListComArg(list,
            new C_CopyTo(dest, false, force), attr, hasMemory);
      }
   }

   delete dest;
   throw SyntaxException(L"wrong syntax of command '" + word.originString + L" to'", line);
}

Command* c_print(const Token& word, const Tokens& tks, const _int& line, const _boo& directError)
{
   if (tks.isEmpty()) {
      switch (g_thisstate) {
         case ts_None: {
         throw SyntaxException(L"command '" + word.originString + L"' needs an argument. "
            L"Value of variable 'this' is undefined here", line);
            break;
         }
         case ts_String: {
            return new C_PrintThis_Str();
            break;
         }
         case ts_Time: {
            return new C_PrintThis_Tim();
            break;
         }
         case ts_Number: {
            return new C_PrintThis_Num();
            break;
         }
      }
   }

   Generator<_str>* str;
   if (parse(tks, str)) {
      return new C_PrintSingle(str);
   }

   _def* def;
   if (parse(tks, def)) {
      return new C_PrintDefinition(def);
   }

   Generator<_list>* list;
   if (parse(tks, list)) {
      return new C_PrintList(list);
   }

   if (directError) {
      commandSyntaxException(word.originString, line);
   }
   else {
      throw SyntaxException(L"wrong syntax. No command can be formed of this code", line);
   }
}

static Command* c_sleep(const Token& word, const Tokens& tks, const _int& line)
{
   Generator<_per>* per;
   if (parse(tks, per)) {
      return new C_SleepPeriod(per);
   }

   Generator<_num>* num;
   if (parse(tks, num)) {
      return new C_SleepMs(num);
   }

   commandSyntaxException(word.originString, line);
}

static Command* c_run(const Token& word, const Tokens& tks, const _int& line)
{
   markAttributesToRun();

   if (tks.containsKeyword(Keyword::kw_With)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideByKeyword(Keyword::kw_With, left, right);

      if (right.isEmpty()) {
         throw SyntaxException(L"right side of command '" + word.originString + L" with' is empty", line);
      }

      if (left.isEmpty()) {
         if (right.containsKeyword(Keyword::kw_With)) {
            if (g_thisstate == ts_None) {
               throw SyntaxException(L"command '" + word.originString + L" with with' needs first argument", line);
            }

            setCoreComAttribute(word.originString + L" with with", line);
            Attribute* lastAttr = getLastAttribute();

            Tokens left2(right);
            Tokens right2(right);
            right.divideByKeyword(Keyword::kw_With, left2, right2);

            if (left2.isEmpty()) {
               throw SyntaxException(L"command '" + word.originString + L"' cannot be called with adjacent 'with' keywords", line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxException(L"command '" + word.originString + L" with with' cannot be called without its last argument", line);
            }

            Generator<_str>* exec;
            if (!parse(left2, exec)) {
               throw SyntaxException(L"second argument of command '" + word.originString + L" with with' cannot be resolved to a string", line);
            }

            if (left2.getLength() == 1) {
               const Token& cf = left2.first();
               if (cf.type == Token::t_Word && cf.value.h1 == HASH_VAR_UROBOROS) {
                  delete exec;
                  Generator<_str>* str;

                  if (parse(right2, str)) {
                     return new C_RunWithUroborosWithString(str, lastAttr);
                  }
                  else {
                     Generator<_list>* list;
                     if (parse(right2, list)) {
                        return new C_RunWithUroborosWith(list, lastAttr);
                     }
                     else {
                        delete exec;
                        throw SyntaxException(L"last argument of command '" + word.originString + L" with Uroboros with' cannot be resolved to a list", line);
                     }
                  }
               }
            }

            Generator<_str>* str;
            if (parse(right2, str)) {
               return new C_RunWithWithString(exec, str, lastAttr);
            }
            else {
               Generator<_list>* list;
               if (parse(right2, list)) {
                  return new C_RunWithWith(exec, list, lastAttr);
               }
               else {
                  delete exec;
                  throw SyntaxException(L"last argument of command '" + word.originString + L" with with' cannot be resolved to a list", line);
               }
            }
         }
         else {
            if (g_thisstate == ts_None) {
               throw SyntaxException(L"command '" + word.originString + L" with' needs first argument", line);
            }

            setCoreComAttribute(word.originString + L" with", line);
            Attribute* lastAttr = getLastAttribute();

            Generator<_str>* exec;
            if (parse(right, exec)) {
               if (right.getLength() == 1) {
                  const Token& cf = right.first();
                  if (cf.type == Token::t_Word && cf.value.h1 == HASH_VAR_UROBOROS) {
                     delete exec;
                     return new C_RunWithUroboros(lastAttr);
                  }
               }
               return new C_RunWith(exec, lastAttr);
            }
            else {
               throw SyntaxException(L"last argument of command '" + word.originString + L" with' cannot be resolved to a string", line);
            }
         }
      }
      else {
         if (right.containsKeyword(Keyword::kw_With)) {
            Tokens left2(right);
            Tokens right2(right);
            right.divideByKeyword(Keyword::kw_With, left2, right2);

            if (left2.isEmpty()) {
               throw SyntaxException(L"command '" + word.originString + L"' cannot be called with adjacent 'with' keywords", line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxException(L"command '" + word.originString + L" with with' cannot be called without its last argument", line);
            }

            const _boo hasMemory = anyAttribute();
            Attribute* lastAttr = getLastAttribute();
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_String;
            Attribute* attr = new Attribute();
            attr->setCoreCommandBase();
            addAttribute(attr);

            Generator<_str>* exec;
            if (!parse(left2, exec)) {
               throw SyntaxException(L"second argument of command '" + word.originString + L" with with' cannot be resolved to a string", line);
            }

            Generator<_str>* lastStr;
            if (parse(right2, lastStr)) {
               g_thisstate = prevThisState;
               retreatAttribute();

               if (left2.getLength() == 1) {
                  const Token& cf = left2.first();
                  if (cf.type == Token::t_Word && cf.value.h1 == HASH_VAR_UROBOROS) {
                     delete exec;

                     Generator<_str>* str;
                     if (parse(left, str)) {
                        return new CS_StringComArg(str, new C_RunWithUroborosWithString(lastStr), attr, lastAttr, hasMemory);
                     }

                     _def* def;
                     if (parse(left, def)) {
                        return new CS_DefinitionComArg(def, new C_RunWithUroborosWithString(lastStr), attr, lastAttr, hasMemory);
                     }

                     Generator<_list>* list;
                     if (parse(left, list)) {
                        return new CS_ListComArg(list, new C_RunWithUroborosWithString(lastStr), attr, lastAttr, hasMemory);
                     }

                     delete lastStr;
                     throw SyntaxException(L"first argument of command '" + word.originString + L" with Uroboros with' cannot be resolved to a list", line);
                  }
               }

               Generator<_str>* str;
               if (parse(left, str)) {
                  return new CS_StringComArg(str, new C_RunWithWithString(exec, lastStr), attr, lastAttr, hasMemory);
               }

               _def* def;
               if (parse(left, def)) {
                  return new CS_DefinitionComArg(def, new C_RunWithWithString(exec, lastStr), attr, lastAttr, hasMemory);
               }

               Generator<_list>* list;
               if (parse(left, list)) {
                  return new CS_ListComArg(list, new C_RunWithWithString(exec, lastStr), attr, lastAttr, hasMemory);
               }

               delete exec;
               delete lastStr;
               throw SyntaxException(L"first argument of command '" + word.originString + L" with with' cannot be resolved to a list", line);
            }
            else {
               Generator<_list>* lastList;

               if (!parse(right2, lastList)) {
                  g_thisstate = prevThisState;
                  retreatAttribute();
                  delete exec;
                  throw SyntaxException(L"last argument of command '" + word.originString + L" with with' cannot be resolved to a list", line);
               }
               else {
                  g_thisstate = prevThisState;
                  retreatAttribute();

                  if (left2.getLength() == 1) {
                     const Token& cf = left2.first();
                     if (cf.type == Token::t_Word && cf.value.h1 == HASH_VAR_UROBOROS) {
                        delete exec;

                        Generator<_str>* str;
                        if (parse(left, str)) {
                           return new CS_StringComArg(str, new C_RunWithUroborosWith(lastList), attr, lastAttr, hasMemory);
                        }

                        _def* def;
                        if (parse(left, def)) {
                           return new CS_DefinitionComArg(def, new C_RunWithUroborosWith(lastList), attr, lastAttr, hasMemory);
                        }

                        Generator<_list>* list;
                        if (parse(left, list)) {
                           return new CS_ListComArg(list, new C_RunWithUroborosWith(lastList), attr, lastAttr, hasMemory);
                        }

                        delete lastList;
                        throw SyntaxException(L"first argument of command '" + word.originString + L" with Uroboros with' cannot be resolved to a list", line);
                     }
                  }

                  Generator<_str>* str;
                  if (parse(left, str)) {
                     return new CS_StringComArg(str, new C_RunWithWith(exec, lastList), attr, lastAttr, hasMemory);
                  }

                  _def* def;
                  if (parse(left, def)) {
                     return new CS_DefinitionComArg(def, new C_RunWithWith(exec, lastList), attr, lastAttr, hasMemory);
                  }

                  Generator<_list>* list;
                  if (parse(left, list)) {
                     return new CS_ListComArg(list, new C_RunWithWith(exec, lastList), attr, lastAttr, hasMemory);
                  }

                  delete exec;
                  delete lastList;
                  throw SyntaxException(L"first argument of command '" + word.originString + L" with with' cannot be resolved to a list", line);
               }
            }
         }
         else {
            const _boo hasMemory = anyAttribute();
            Attribute* lastAttr = getLastAttribute();
            const ThisState prevThisState = g_thisstate;
            g_thisstate = ThisState::ts_String;
            Attribute* attr = new Attribute();
            attr->setCoreCommandBase();
            addAttribute(attr);

            Generator<_str>* exec;
            if (!parse(right, exec)) {
               throw SyntaxException(L"last argument of command '" + word.originString + L" with' cannot be resolved to a string", line);
            }

            g_thisstate = prevThisState;
            retreatAttribute();

            if (right.getLength() == 1) {
               const Token& cf = right.first();
               if (cf.type == Token::t_Word && cf.value.h1 == HASH_VAR_UROBOROS) {
                  delete exec;

                  Generator<_str>* str;
                  if (parse(left, str)) {
                     return new CS_StringComArg(str, new C_RunWithUroboros(), attr, lastAttr, hasMemory);
                  }

                  _def* def;
                  if (parse(left, def)) {
                     return new CS_DefinitionComArg(def, new C_RunWithUroboros(), attr, lastAttr, hasMemory);
                  }

                  Generator<_list>* list;
                  if (parse(left, list)) {
                     return new CS_ListComArg(list, new C_RunWithUroboros(), attr, lastAttr, hasMemory);
                  }

                  throw SyntaxException(L"first argument of command '" + word.originString + L" with Uroboros' cannot be resolved to a list", line);
               }
            }

            Generator<_str>* str;
            if (parse(left, str)) {
               return new CS_StringComArg(str, new C_RunWith(exec), attr, lastAttr, hasMemory);
            }

            _def* def;
            if (parse(left, def)) {
               return new CS_DefinitionComArg(def, new C_RunWith(exec), attr, lastAttr, hasMemory);
            }

            Generator<_list>* list;
            if (parse(left, list)) {
               return new CS_ListComArg(list, new C_RunWith(exec), attr, lastAttr, hasMemory);
            }

            delete exec;
            throw SyntaxException(L"first argument of command '" + word.originString + L" with' cannot be resolved to a list", line);
         }
      }
   }
   else {
      Attribute* lastAttr = getLastAttribute();
      Generator<_str>* str;
      if (parse(tks, str)) {
         return new C_Run(str, lastAttr);
      }
   }

   commandSyntaxException(word.originString, line);
}

static Command* c_error(const Token& word, const Tokens& tks, const _int& line)
{
   if (tks.isEmpty()) {
      return new C_Error();
   }

   Generator<_num>* num;

   if (parse(tks, num)) {
      return new C_ErrorWithExitCode(num);
   }
   else {
      throw SyntaxException(L"the argument of command '" + word.originString + L"' cannot be resolved to a number", line);
   }
}

static Command* c_process(const Token& word, const Tokens& tks, const _int& line)
{
   markAttributesToRun();

   if (tks.isEmpty()) {
      throw SyntaxException(L"command '" + word.originString + L"' needs a string argument", line);
   }

   Generator<_str>* str;

   if (parse(tks, str)) {
      return new C_Process(str);
   }
   else {
      throw SyntaxException(L"the argument of command '" + word.originString + L"' cannot be resolved to a string", line);
   }
}

static void checkUselessFlags(const Token& word, const _int& line,
   const bool& force, const bool& stack)
{
   if (force) {
      throw SyntaxException(
        L"keyword '" + word.originString + L"' cannot be preceded by a flag 'forced'", line);
   }

   if (stack) {
      throw SyntaxException(
        L"keyword '" + word.originString + L"' cannot be preceded by a flag 'stack'", line);
   }
}

static void commandSyntaxException(const _str& name, const _int& line)
{
   throw SyntaxException(L"wrong syntax of command '" + name + L"'", line);
}

static void commandNoArgException(const _str& name, const _int& line)
{
   throw SyntaxException(L"command '" + name + L"' requires an argument", line);
}
