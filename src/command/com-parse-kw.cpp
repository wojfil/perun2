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
#include "../patterns.h"


namespace uro::comm
{

_bool keywordCommands(_comptr& result, const Token& word, Tokens& tks,
   const _int& line, const _bool& force, const _bool& stack, _uro& uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Unlock:
      case Keyword::kw_Unhide: {
         checkUselessFlags(word, line, force, stack, uro);
         return kwCommandSimple(result, word, tks, line, uro);
      }
      case Keyword::kw_Open: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_open(result, word, tks, line, uro);
      }
      case Keyword::kw_Copy: {
         return c_copy(result, word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Create: {
         return c_create(result, word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Download: {
         return c_downloadFrom(result, word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Break:
      case Keyword::kw_Continue:
      case Keyword::kw_Exit: {
         checkUselessFlags(word, line, force, stack, uro);
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L"' cannot be called with an argument"), line);
      }
      case Keyword::kw_Error: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_error(result, word, tks, line, uro);
      }
      case Keyword::kw_Move: {
         return c_moveTo(result, word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Print: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_print(result, word, tks, line, true, uro);
      }
      case Keyword::kw_Reaccess:
      case Keyword::kw_Recreate:
      case Keyword::kw_Rechange:
      case Keyword::kw_Remodify: {
         checkUselessFlags(word, line, force, stack, uro);
         return kwCommandTime(result, word, tks, line, uro);
      }
      case Keyword::kw_Rename: {
         return c_rename(result, word, tks, line, force, stack, uro);
      }
      case Keyword::kw_Run: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_run(result, word, tks, line, uro);
      }
      case Keyword::kw_Select: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_select(result, word, tks, line, uro);
      }
      case Keyword::kw_Sleep: {
         checkUselessFlags(word, line, force, stack, uro);
         return c_sleep(result, word, tks, line, uro);
      }
   }

   throw SyntaxError(str(L"command cannot start with a keyword '", word.getOriginString(uro), L"'"), line);
   return false;
}

_bool parseLooped(const Tokens& tks, _comptr& innerCommand, _comptr& result, _uro& uro,
   _attrptr& attr, const _bool& hasMemory)
{
   _genptr<_str> str_;
   if (parse::parse(uro, tks, str_)) {
      result = std::make_unique<CS_StringComArg>(str_, innerCommand, attr, hasMemory, uro);
      return true;
   }

   _defptr def;
   if (parse::parse(uro, tks, def)) {
      _fdata* fdata = def->getDataPtr();

      if (fdata == nullptr) {
         result = std::make_unique<CS_DefinitionComArg>(def, innerCommand, attr, hasMemory, uro);
      }
      else {
         const _aunit aval = attr->getValue();
         _attrptr bridge(new BridgeAttribute(aval, uro, fdata));
         result = std::make_unique<CS_DefinitionComArg>(def, innerCommand, bridge, hasMemory, uro);
      }

      return true;
   }

   _genptr<_list> list;
   if (parse::parse(uro, tks, list)) {
      result = std::make_unique<CS_ListComArg>(list, innerCommand, attr, hasMemory, uro);
      return true;
   }

   return false;
}

_bool parseLooped(const Tokens& tks, _comptr& innerCommand, _comptr& result, _uro& uro)
{
   const _bool hasMemory = uro.vc.anyAttribute();
   _attrptr attr(new Attribute(uro));
   attr->setCoreCommandBase();
   return parseLooped(tks, innerCommand, result, uro, attr, hasMemory);
}

static _bool kwCommandSimple(_comptr& result, const Token& word, Tokens& tks, const _int& line, _uro& uro)
{
   if (tks.isEmpty()) {
      uro.vc.setCoreComAttribute(word.getOriginString(uro), line);
      return coreCommandSimpleSave(result, word, uro);
   }

   _comptr inner;
   if (coreCommandSimpleNoSave(inner, word, uro) && parseLooped(tks, inner, result, uro)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool coreCommandSimpleSave(_comptr& result, const Token& word, _uro& uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete: {
         Attribute* lastAttr = uro.vc.getLastAttribute();
         result = std::make_unique<C_Delete>(lastAttr, uro);
         break;
      }
      case Keyword::kw_Drop: {
         Attribute* lastAttr = uro.vc.getLastAttribute();
         result = std::make_unique<C_Drop>(lastAttr, uro);
         break;
      }
      case Keyword::kw_Hide: {
         result = std::make_unique<C_Hide>(true, uro);
         break;
      }
      case Keyword::kw_Lock: {
         result = std::make_unique<C_Lock>(true, uro);
         break;
      }
      case Keyword::kw_Unlock: {
         result = std::make_unique<C_Unlock>(true, uro);
         break;
      }
      case Keyword::kw_Unhide: {
         result = std::make_unique<C_Unhide>(true, uro);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool coreCommandSimpleNoSave(_comptr& result, const Token& word, _uro& uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete: {
         result = std::make_unique<C_Delete>(uro);
         break;
      }
      case Keyword::kw_Drop: {
         result = std::make_unique<C_Drop>(uro);
         break;
      }
      case Keyword::kw_Hide: {
         result = std::make_unique<C_Hide>(false, uro);
         break;
      }
      case Keyword::kw_Lock: {
         result = std::make_unique<C_Lock>(false, uro);
         break;
      }
      case Keyword::kw_Unlock: {
         result = std::make_unique<C_Unlock>(false, uro);
         break;
      }
      case Keyword::kw_Unhide: {
         result = std::make_unique<C_Unhide>(false, uro);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool kwCommandTime(_comptr& result, const Token& word, Tokens& tks, const _int& line, _uro& uro)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to' is empty"), line);
   }

   if (!tks.check(TI_HAS_KEYWORD_TO)) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to' does not contain keyword 'to'"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to' does not contain its time argument"), line);
   }

   if (left.isEmpty()) {
      uro.vc.setTimeComAttribute(word.getOriginString(uro), line);
      _genptr<_tim> tim;
      if (!parse::parse(uro, right, tim)) {
         throw SyntaxError(str(L"time argument of command '", word.getOriginString(uro), L" to' is not valid"), line);
      }

      if (coreCommandTime(result, word, tim, true, uro)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" to'"), line);
   }

   const _bool hasMemory = uro.vc.anyAttribute();
   const ThisState prevThisState = uro.vars.inner.thisState;
   uro.vars.inner.thisState = ThisState::ts_String;
   _attrptr attr(new Attribute(uro));
   attr->setTimeCommandBase();
   uro.vc.addAttribute(attr);

   _genptr<_tim> tim;
   if (!parse::parse(uro, right, tim)) {
      throw SyntaxError(str(L"time argument of command '", word.getOriginString(uro), L" to' is not valid"), line);
   }

   uro.vars.inner.thisState = prevThisState;
   uro.vc.retreatAttribute();

   _comptr inner;
   if (coreCommandTime(inner, word, tim, false, uro) && parseLooped(left, inner, result, uro, attr, hasMemory)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool coreCommandTime(_comptr& result, const Token& word, _genptr<_tim>& time, const _bool& saveChanges, _uro& uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Reaccess: {
         result = std::make_unique<C_ReaccessTo>(time, saveChanges, uro);
         break;
      }
      case Keyword::kw_Rechange: {
         result = std::make_unique<C_RechangeTo>(time, saveChanges, uro);
         break;
      }
      case Keyword::kw_Recreate: {
         result = std::make_unique<C_RecreateTo>(time, saveChanges, uro);
         break;
      }
      case Keyword::kw_Remodify: {
         result = std::make_unique<C_RemodifyTo>(time, saveChanges, uro);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool c_open(_comptr& result, const Token& word, const Tokens& tks, const _int& line, _uro& uro)
{
   if (tks.isEmpty()) {
      uro.vc.setCoreComAttribute(word.getOriginString(uro), line);
      result = std::make_unique<C_Open>(uro);
      return true;
   }

   if (tks.check(TI_HAS_KEYWORD_WITH)) {
      P_DIVIDE_BY_KEYWORD(kw_With);

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),  L" with' does not "
            L"contain its last argument"), line);
      }

      const _bool hasMemory = uro.vc.anyAttribute();
      const ThisState prevThisState = uro.vars.inner.thisState;
      uro.vars.inner.thisState = ThisState::ts_String;
      _attrptr attr(new Attribute(uro));
      attr->setCoreCommandBase();
      uro.vc.addAttribute(attr);

      _genptr<_str> prog;
      if (!parse::parse(uro, right, prog)) {
         throw SyntaxError(str(L"last argument of command '", word.getOriginString(uro), L" with' "
            L"cannot be resolved to a string"), line);
      }

      uro.vars.inner.thisState = prevThisState;
      uro.vc.retreatAttribute();

      if (left.isEmpty()) {
         uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" with"), line);
         result = std::make_unique<C_OpenWith>(prog, uro);
         return true;
      }
      else {
         _comptr inner(new C_OpenWith(prog, uro));
         if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
            return true;
         }

         throw SyntaxError(str(L"wrong syntax of command '", word.getOriginString(uro), L" with'"), line);
      }
   }

   _comptr inner(new C_Open(uro));
   if (parseLooped(tks, inner, result, uro)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_select(_comptr& result, const Token& word, const Tokens& tks, const _int& line, _uro& uro)
{
   if (uro.vc.anyAggregate()) {
      Aggregate* aggr = uro.vc.getLastAggregate();
      aggr->set(AGGR_SELECT);

      if (tks.isEmpty()) {
         _genptr<_str> str;
         uro.vars.inner.createThisRef(str);
         result = std::make_unique<C_AggrSelect_String>(aggr, str, uro);
         return true;
      }

      _genptr<_str> str;
      if (parse::parse(uro, tks, str)) {
         result = std::make_unique<C_AggrSelect_String>(aggr, str, uro);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, tks, list)) {
         result = std::make_unique<C_AggrSelect_List>(aggr, list, uro);
         return true;
      }
   }
   else {
      if (tks.isEmpty()) {
         commandNoArgException(word.getOriginString(uro), line);
      }

      _genptr<_str> str;
      if (parse::parse(uro, tks, str)) {
         result = std::make_unique<C_Select_String>(str, uro);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, tks, list)) {
         result = std::make_unique<C_Select_List>(list, uro);
         return true;
      }
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_rename(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, _uro& uro)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to' is empty"), line);
   }

   if (!tks.check(TI_HAS_KEYWORD_TO)) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro),  L" to' does not contain keyword 'to'"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to' ",
         L"does not contain declaration of new name"), line);
   }

   _bool extless = false;

   if (right.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      right.trimLeft();
      if (right.isEmpty()) {
         throw SyntaxError(L"keyword 'extensionless' is not followed by a declaration of new file name", line);
      }
   }

   if (left.isEmpty()) {
      uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" to"), line);

      _genptr<_str> newName;
      if (!parse::parse(uro, right, newName)) {
         throw SyntaxError(str(L"declaration of new name in command '", word.getOriginString(uro), L" to' is not valid"), line);
      }

      Attribute* lastAttr = uro.vc.getLastAttribute();
      lastAttr->markToEvaluate();

      if (stack) {
         result = std::make_unique<C_RenameTo_Stack>(newName, true, extless, uro);
      }
      else {
         result = std::make_unique<C_RenameTo>(newName, true, force, extless, uro);
      }

      return true;
   }

   const _bool hasMemory = uro.vc.anyAttribute();
   const ThisState prevThisState = uro.vars.inner.thisState;
   uro.vars.inner.thisState = ThisState::ts_String;
   _attrptr attr(new Attribute(uro));
   attr->setCoreCommandBase();
   uro.vc.addAttribute(attr);

   _genptr<_str> newName;
   if (!parse::parse(uro, right, newName)) {
      throw SyntaxError(str(L"declaration of new name in command '", word.getOriginString(uro), L" to' is not valid"), line);
   }

   uro.vars.inner.thisState = prevThisState;
   uro.vc.retreatAttribute();

   _genptr<_str> str_;
   if (parse::parse(uro, left, str_)) {
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, uro);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, force, extless, uro);
      }

      result = std::make_unique<CS_StringComArg>(str_, inner, attr, hasMemory, uro);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(uro, left, list)) {
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, uro);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, force, extless, uro);
      }

      result = std::make_unique<CS_ListComArg>(list, inner, attr, hasMemory, uro);
      return true;
   }

   commandSyntaxError(str(word.getOriginString(uro), L" to"), line);
   return false;
}

static _bool c_create(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, _uro& uro)
{
   if (tks.isEmpty()) {
      uro.vc.setCoreComAttribute(word.getOriginString(uro), line);
      Attribute* lastAttr = uro.vc.getLastAttribute();

      if (stack) {
         result = std::make_unique<C_Create_Stack>(lastAttr, uro);
      }
      else {
         result = std::make_unique<C_Create>(force, lastAttr, uro);
      }

      return true;
   }

   const Token& f = tks.first();

   if (f.type == Token::t_Word) {
      Tokens tks2 = tks;
      tks2.trimLeft();
      const _size& fk = f.value.word.h;

      if (fk == uro.hashes.HASH_VAR_FILE) {
         if (tks2.isEmpty()) {
            uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" ", f.getOriginString(uro)), line);
            Attribute* lastAttr = uro.vc.getLastAttribute();

            if (stack) {
               result = std::make_unique<C_CreateFile_Stack>(lastAttr, uro);
            }
            else {
               result = std::make_unique<C_CreateFile>(force, lastAttr, uro);
            }

            return true;
         }

         _genptr<_str> str_;
         if (parse::parse(uro, tks2, str_)) {
            if (stack) {
               result = std::make_unique<C_CreateFile_String_Stack>(str_, uro);
            }
            else {
               result = std::make_unique<C_CreateFile_String>(str_, force, uro);
            }

            return true;
         }

         throw SyntaxError(str(L"the argument of command '", word.getOriginString(uro), L" ",
            f.getOriginString(uro), L"' cannot be resolved to a string"), line);
      }
      else if (fk == uro.hashes.HASH_VAR_FILES) {
         if (tks2.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" ",
               f.getOriginString(uro), L"' needs an argument"), line);
         }

         _genptr<_str> str_;
         if (parse::parse(uro, tks2, str_)) {
            const _str sub = (f.getOriginString(uro)).substr(0, 4);
            throw SyntaxError(str(L"write '", word.getOriginString(uro), L" ", sub, L"' instead of '",
               word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
         }

         _defptr def;
         if (parse::parse(uro, tks2, def)) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" ", f.getOriginString(uro),
               L"' cannot be called with a definition argument"), line);
         }

         _genptr<_list> list;
         if (parse::parse(uro, tks2, list)) {
            if (stack) {
               result = std::make_unique<C_CreateFiles_List_Stack>(list, uro);
            }
            else {
               result = std::make_unique<C_CreateFiles_List>(list, force, uro);
            }

            return true;
         }

         throw SyntaxError(str(L"wrong syntax of command '",
            word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
      }
      else if (fk == uro.hashes.HASH_VAR_DIRECTORY) {
         if (tks2.isEmpty()) {
            uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" ", f.getOriginString(uro)), line);
            Attribute* lastAttr = uro.vc.getLastAttribute();
            if (stack) {
               result = std::make_unique<C_CreateDirectory_Stack>(lastAttr, uro);
            }
            else {
               result = std::make_unique<C_CreateDirectory>(force, lastAttr, uro);
            }

            return true;
         }

         _genptr<_str> str_;
         if (parse::parse(uro, tks2, str_)) {
            if (stack) {
               result = std::make_unique<C_CreateDirectory_String_Stack>(str_, uro);
            }
            else {
               result = std::make_unique<C_CreateDirectory_String>(str_, force, uro);
            }

            return true;
         }

         throw SyntaxError(str(L"argument of command '", word.getOriginString(uro), L" ",
            f.getOriginString(uro), L"' cannot be resolved to a string"), line);
      }
      else if (fk == uro.hashes.HASH_VAR_DIRECTORIES) {
         if (tks2.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" ",
               f.getOriginString(uro), L"' needs an argument"), line);
         }

         _genptr<_str> str_;
         if (parse::parse(uro, tks2, str_)) {
            const _str sub = str((f.getOriginString(uro)).substr(0, 8), L"y");
            throw SyntaxError(str(L"write '", word.getOriginString(uro), L" ", sub,
               L"' instead of '", word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
         }

         _defptr def;
         if (parse::parse(uro, tks2, def)) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" ",
               f.getOriginString(uro), L"' cannot be called with a definition argument"), line);
         }

         _genptr<_list> list;
         if (parse::parse(uro, tks2, list)) {
            if (stack) {
               result = std::make_unique<C_CreateDirectories_List_Stack>(list, uro);
            }
            else {
               result = std::make_unique<C_CreateDirectories_List>(list, force, uro);
            }

            return true;
         }

         throw SyntaxError(str(L"wrong syntax of command '",
            word.getOriginString(uro), L" ", f.getOriginString(uro), L"'"), line);
      }
   }

   _genptr<_str> str_;
   if (parse::parse(uro, tks, str_)) {
      if (stack) {
         result = std::make_unique<C_Create_String_Stack>(str_, uro);
      }
      else {
         result = std::make_unique<C_Create_String>(str_, force, uro);
      }

      return true;
   }

   _defptr def;
   if (parse::parse(uro, tks, def)) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro),
         L"' cannot be called with a definition argument"), line);
   }

   _genptr<_list> list;
   if (parse::parse(uro, tks, list)) {
      if (stack) {
         result = std::make_unique<C_Create_List_Stack>(list, uro);
      }
      else {
         result = std::make_unique<C_Create_List>(list, force, uro);
      }

      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_moveTo(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, _uro& uro)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to' is empty"), line);
   }

   const _bool hasTo = tks.check(TI_HAS_KEYWORD_TO);
   const _bool hasAs = tks.check(TI_HAS_KEYWORD_AS);

   if (!hasTo) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro),
         L" to' cannot be called without keyword 'to'"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (left.isEmpty()) {
      if (hasAs) {
         std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
         Tokens& preAs = pair2.first;
         Tokens& postAs = pair2.second;

         if (preAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _bool extless = false;
         const Token& paf = postAs.first();

         if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxError(str(L"keyword '", paf.getOriginString(uro),
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         _genptr<_str> nname;
         if (!parse::parse(uro, postAs, nname)) {
            throw SyntaxError(str(L"new name in command '", word.getOriginString(uro),
               L" to as' cannot be resolved to a string"), line);
         }

         _genptr<_str> dest;
         if (!parse::parse(uro, preAs, dest)) {
            throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
               L" to' cannot be resolved to a string"), line);
         }

         uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" to as"), line);
         Attribute* lastAttr = uro.vc.getLastAttribute();
         if (stack) {
            result = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, lastAttr, uro);
         }
         else {
            result = std::make_unique<C_MoveToAs>(dest, nname, force, extless, lastAttr, uro);
         }

         return true;
      }

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),
            L" to' lacks declaration of a new location"), line);
      }

      _genptr<_str> str_;
      if (!parse::parse(uro, right, str_)) {
         throw SyntaxError(str(L"new location in command '",
            word.getOriginString(uro), L" to' cannot be resolved to a string"), line);
      }
      else {
         uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" to"), line);
         Attribute* lastAttr = uro.vc.getLastAttribute();
         if (stack) {
            result = std::make_unique<C_MoveTo_Stack>(str_, lastAttr, uro);
         }
         else {
            result = std::make_unique<C_MoveTo>(str_, force, lastAttr, uro);
         }

         return true;
      }
   }

   if (hasAs) {
      if (left.check(TI_HAS_KEYWORD_AS)) {
         throw SyntaxError(str(L"keywords 'to' and 'as' appear in command '",
            word.getOriginString(uro), L" to as' in reverse order"), line);
      }

      std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
      Tokens& preAs = pair2.first;
      Tokens& postAs = pair2.second;

      if (preAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain declaration of a new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain declaration of a new name written after keyword 'as'"), line);
      }

      _bool extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"keyword '", paf.getOriginString(uro),
               L"' is not followed by a declaration of a new file name"), line);
         }
      }

      const _bool hasMemory = uro.vc.anyAttribute();
      const ThisState prevThisState = uro.vars.inner.thisState;
      uro.vars.inner.thisState = ThisState::ts_String;
      _attrptr attr(new Attribute(uro));
      attr->setCoreCommandBase();
      uro.vc.addAttribute(attr);

      _genptr<_str> nname;
      if (!parse::parse(uro, postAs, nname)) {
         throw SyntaxError(str(L"new name in command '", word.getOriginString(uro),
            L" to as' cannot be resolved to a string"), line);
      }

      _genptr<_str> dest;
      if (!parse::parse(uro, preAs, dest)) {
         throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
            L" to' cannot be resolved to a string"), line);
      }

      uro.vars.inner.thisState = prevThisState;
      uro.vc.retreatAttribute();

      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, uro);
      }
      else {
         inner = std::make_unique<C_MoveToAs>(dest, nname, force, extless, uro);  
      }

      if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" to as"), line);
      return false;
   }

   const _bool hasMemory = uro.vc.anyAttribute();
   const ThisState prevThisState = uro.vars.inner.thisState;
   uro.vars.inner.thisState = ThisState::ts_String;
   _attrptr attr(new Attribute(uro));
   attr->setCoreCommandBase();
   uro.vc.addAttribute(attr);

   _genptr<_str> dest;
   if (!parse::parse(uro, right, dest)) {
      throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
         L" to' cannot be resolved to a string"), line);
   }

   uro.vars.inner.thisState = prevThisState;
   uro.vc.retreatAttribute();

   _comptr inner;
   if (stack) {
      inner = std::make_unique<C_MoveTo_Stack>(dest, uro);
   }
   else {
      inner = std::make_unique<C_MoveTo>(dest, force, uro);
   }

   if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
      return true;
   }

   commandSyntaxError(str(word.getOriginString(uro), L" to"), line);
   return false;
}

static _bool c_downloadFrom(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, _uro& uro)
{
   if (tks.check(TI_HAS_KEYWORD_FROM)) {
      P_DIVIDE_BY_KEYWORD(kw_From);

      if (left.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L" from' does not "
            L"contain a declaration of elements to download"), line);
      }
      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L" from' does not "
            L"contain a declaration of location"), line);
      }

      _genptr<_str> loc;
      if (!parse::parse(uro, right, loc)) {
         throw SyntaxError(str(L"location in command '", word.getOriginString(uro),
            L" from' cannot be resolved to a string"), line);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, left, str_)) {
         if (stack)
            result = std::make_unique<C_DownloadFrom_String_Stack>(loc, str_, uro);
         else
            result = std::make_unique<C_DownloadFrom_String>(loc, str_, force, uro);

         return true;
      }

      _defptr def;
      if (parse::parse(uro, left, def)) {
         if (stack)
            result = std::make_unique<C_DownloadFrom_Definition_Stack>(loc, def, uro);
         else
            result = std::make_unique<C_DownloadFrom_Definition>(loc, def, force, uro);

         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, left, list)) {
         if (stack)
            result = std::make_unique<C_DownloadFrom_List_Stack>(loc, list, uro);
         else
            result = std::make_unique<C_DownloadFrom_List>(loc, list, force, uro);

         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" from"), line);
      return false;
   }

   _genptr<_str> str_;
   if (parse::parse(uro, tks, str_)) {
      if (stack)
         result = std::make_unique<C_Download_String_Stack>(str_, uro);
      else
         result = std::make_unique<C_Download_String>(str_, force, uro);

      return true;
   }

   _defptr def;
   if (parse::parse(uro, tks, def)) {
      throw SyntaxError(str(L"the argument of command '", 
         word.getOriginString(uro), L"' cannot be of type 'definition'"), line);
   }

   _genptr<_list> list;
   if (parse::parse(uro, tks, list)) {
      if (stack)
         result = std::make_unique<C_Download_List_Stack>(list, uro);
      else
         result = std::make_unique<C_Download_List>(list, force, uro);

      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_copy(_comptr& result, const Token& word, const Tokens& tks, const _int& line,
   const _bool& force, const _bool& stack, _uro& uro)
{
   const _bool hasTo = tks.check(TI_HAS_KEYWORD_TO);
   const _bool hasAs = tks.check(TI_HAS_KEYWORD_AS);

   if (!hasTo) {
      if (hasAs) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),
            L" to as' cannot be called without keyword 'to'"), line);
      }

      if (force) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),
            L"' cannot be preceded by a flag 'forced'"), line);
      }

      if (stack) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),
         L"' cannot be preceded by a flag 'stack'"), line);
      }

      if (uro.vc.anyAggregate()) {
         Aggregate* aggr = uro.vc.getLastAggregate();
         aggr->set(AGGR_COPY);

         if (tks.isEmpty()) {
            _genptr<_str> str;
            uro.vars.inner.createThisRef(str);
            result = std::make_unique<C_AggrCopy_String>(aggr, str, uro);
            return true;
         }

         _genptr<_str> str;
         if (parse::parse(uro, tks, str)) {
            result = std::make_unique<C_AggrCopy_String>(aggr, str, uro);
            return true;
         }

         _genptr<_list> list;
         if (parse::parse(uro, tks, list)) {
            result = std::make_unique<C_AggrCopy_List>(aggr, list, uro);
            return true;
         }
      }
      else {
         if (tks.isEmpty()) {
            commandNoArgException(word.getOriginString(uro), line);
         }

         _genptr<_str> str;
         if (parse::parse(uro, tks, str)) {
            result = std::make_unique<C_Copy_String>(str, uro);
            return true;
         }

         _genptr<_list> list;
         if (parse::parse(uro, tks, list)) {
            result = std::make_unique<C_Copy_List>(list, uro);
            return true;
         }
      }

      commandSyntaxError(word.getOriginString(uro), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (left.isEmpty()) {
      if (hasAs) {
         std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
         Tokens& preAs = pair2.first;
         Tokens& postAs = pair2.second;

         if (preAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _bool extless = false;
         const Token& paf = postAs.first();

         if (paf.isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxError(str(L"keyword '", paf.getOriginString(uro),
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         _genptr<_str> nname;
         if (!parse::parse(uro, postAs, nname)) {
            throw SyntaxError(str(L"new name in command '", word.getOriginString(uro),
               L" to as' cannot be resolved to a string"), line);
         }

         _genptr<_str> dest;
         if (!parse::parse(uro, preAs, dest)) {
            throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
               L" to' cannot be resolved to a string"), line);
         }

         uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" to as"), line);
         if (stack) {
            result = std::make_unique<C_CopyToAs_Stack>(dest, nname, true, extless, uro);
         }
         else {
            result = std::make_unique<C_CopyToAs>(dest, nname, true, force, extless, uro);
         }

         return true;
      }

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),
            L" to' lacks a declaration of new location"), line);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, right, str_)) {
         uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" to"), line);
         if (stack) {
            result = std::make_unique<C_CopyTo_Stack>(str_, true, uro);
         }
         else {
            result = std::make_unique<C_CopyTo>(str_, true, force, uro);
         }

         return true;
      }
      else {
         throw SyntaxError(str(L"new location in command '",
            word.getOriginString(uro), L" to' cannot be resolved to a string"), line);
      }
   }

   if (hasAs) {
      if (left.check(TI_HAS_KEYWORD_AS)) {
         throw SyntaxError(str(L"keywords 'to' and 'as' appear in "
            L"command '", word.getOriginString(uro), L" to as' in reverse order"), line);
      }

      std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
      Tokens& preAs = pair2.first;
      Tokens& postAs = pair2.second;

      if (preAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain a declaration of new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L" to as' "
            L"does not contain a declaration of new name written after keyword 'as'"), line);
      }

      _bool extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"keyword '", paf.getOriginString(uro),
               L"' is not followed by a declaration of new file name"), line);
         }
      }

      const _bool hasMemory = uro.vc.anyAttribute();
      const ThisState prevThisState = uro.vars.inner.thisState;
      uro.vars.inner.thisState = ThisState::ts_String;
      _attrptr attr(new Attribute(uro));
      attr->setCoreCommandBase();
      uro.vc.addAttribute(attr);

      _genptr<_str> nname;
      if (!parse::parse(uro, postAs, nname)) {
         throw SyntaxError(str(L"new name in command '", word.getOriginString(uro),
            L" to as' cannot be resolved to a string"), line);
      }

      _genptr<_str> dest;
      if (!parse::parse(uro, preAs, dest)) {
         throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
            L" to' cannot be resolved to a string"), line);
      }

      uro.vars.inner.thisState = prevThisState;
      uro.vc.retreatAttribute();

      _comptr inner;
      if (stack) {
         inner = std::make_unique<C_CopyToAs_Stack>(dest, nname, false, extless, uro);
      }
      else {
         inner = std::make_unique<C_CopyToAs>(dest, nname, false, force, extless, uro);
      }

      if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" to as"), line);
      return false;
   }

   const _bool hasMemory = uro.vc.anyAttribute();
   const ThisState prevThisState = uro.vars.inner.thisState;
   uro.vars.inner.thisState = ThisState::ts_String;
   _attrptr attr(new Attribute(uro));
   attr->setCoreCommandBase();
   uro.vc.addAttribute(attr);

   _genptr<_str> dest;
   if (!parse::parse(uro, right, dest)) {
      throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
         L" to' cannot be resolved to a string"), line);
   }

   uro.vars.inner.thisState = prevThisState;
   uro.vc.retreatAttribute();

   _comptr inner;
   if (stack) {
      inner = std::make_unique<C_CopyTo_Stack>(dest, false, uro);
   }
   else {
      inner = std::make_unique<C_CopyTo>(dest, false, force, uro);
   }

   if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
      return true;
   }

   commandSyntaxError(str(word.getOriginString(uro), L" to"), line);
   return false;
}

_bool c_print(_comptr& result, const Token& word, const Tokens& tks, const _int& line, const _bool& directError, _uro& uro)
{
   if (tks.isEmpty()) {
      switch (uro.vars.inner.thisState) {
         case ts_None: {
            throw SyntaxError(str(L"command '", word.getOriginString(uro), L"' needs an argument here. "
               L"Value of variable 'this' is undefined in this area"), line);
            return false;
         }
         case ts_String: {
            result = std::make_unique<C_PrintThis_Str>(uro);
            return true;
         }
         case ts_Time: {
            result = std::make_unique<C_PrintThis_Tim>(uro);
            return true;
         }
         case ts_Number: {
            result = std::make_unique<C_PrintThis_Num>(uro);
            return true;
         }
      }
   }

   _genptr<_str> str;
   if (parse::parse(uro, tks, str)) {
      result = std::make_unique<C_PrintSingle>(str, uro);
      return true;
   }

   _defptr def;
   if (parse::parse(uro, tks, def)) {
      result = std::make_unique<C_PrintDefinition>(def, uro);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(uro, tks, list)) {
      result = std::make_unique<C_PrintList>(list, uro);
      return true;
   }

   if (directError) {
      commandSyntaxError(word.getOriginString(uro), line);
   }
   else {
      throw SyntaxError(L"wrong syntax. No command can be formed of this code", line);
   }

   return false;
}

static _bool c_sleep(_comptr& result, const Token& word, const Tokens& tks, const _int& line, _uro& uro)
{
   _genptr<_per> per;
   if (parse::parse(uro, tks, per)) {
      result = std::make_unique<C_SleepPeriod>(per, uro);
      return true;
   }

   _genptr<_num> num;
   if (parse::parse(uro, tks, num)) {
      result = std::make_unique<C_SleepMs>(num, uro);
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_run(_comptr& result, const Token& word, const Tokens& tks, const _int& line, _uro& uro)
{
   uro.vc.markAttributesToRun();

   if (tks.check(TI_HAS_KEYWORD_WITH)) {
      P_DIVIDE_BY_KEYWORD(kw_With);

      if (right.isEmpty()) {
         throw SyntaxError(str(L"right side of command '", word.getOriginString(uro), L" with' is empty"), line);
      }

      if (left.isEmpty()) {
         if (right.check(TI_HAS_KEYWORD_WITH)) {
            if (uro.vars.inner.thisState == ts_None) {
               throw SyntaxError(str(L"command '", word.getOriginString(uro), L" with with' needs first argument"), line);
            }

            uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" with with"), line);
            Attribute* lastAttr = uro.vc.getLastAttribute();

            std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_With);
            Tokens& left2 = pair2.first;
            Tokens& right2 = pair2.second;

            if (left2.isEmpty()) {
               throw SyntaxError(str(L"command '", word.getOriginString(uro),
                  L"' cannot be called with adjacent 'with' keywords"), line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxError(str(L"command '", word.getOriginString(uro),
                  L" with with' cannot be called without its last argument"), line);
            }

            _genptr<_str> exec;
            if (!parse::parse(uro, left2, exec)) {
               throw SyntaxError(str(L"second argument of command '", word.getOriginString(uro),
                  L" with with' cannot be resolved to a string"), line);
            }

            if (left2.getLength() == 1) {
               const Token& cf = left2.first();
               if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS) {
                  _genptr<_str> str_;

                  if (parse::parse(uro, right2, str_)) {
                     result = std::make_unique<C_RunWithUroborosWithString>(str_, lastAttr, uro);
                     return true;
                  }
                  else {
                     _genptr<_list> list;
                     if (parse::parse(uro, right2, list)) {
                        result = std::make_unique<C_RunWithUroborosWith>(list, lastAttr, uro);
                        return true;
                     }
                     else {
                        throw SyntaxError(str(L"last argument of command '",
                           word.getOriginString(uro), L" with Uroboros with' cannot be resolved to a list"), line);
                     }
                  }
               }
            }

            _genptr<_str> str_;
            if (parse::parse(uro, right2, str_)) {
               result = std::make_unique<C_RunWithWithString>(exec, str_, lastAttr, uro);
               return true;
            }
            else {
               _genptr<_list> list;
               if (parse::parse(uro, right2, list)) {
                  result = std::make_unique<C_RunWithWith>(exec, list, lastAttr, uro);
                  return true;
               }
               else {
                  throw SyntaxError(str(L"last argument of command '",
                     word.getOriginString(uro), L" with with' cannot be resolved to a list"), line);
               }
            }
         }
         else {
            if (uro.vars.inner.thisState == ts_None) {
               throw SyntaxError(str(L"command '", word.getOriginString(uro),
                  L" with' needs first argument"), line);
            }

            uro.vc.setCoreComAttribute(str(word.getOriginString(uro), L" with"), line);
            Attribute* lastAttr = uro.vc.getLastAttribute();

            _genptr<_str> exec;
            if (parse::parse(uro, right, exec)) {
               if (right.getLength() == 1) {
                  const Token& cf = right.first();
                  if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS) {
                     result = std::make_unique<C_RunWithUroboros>(lastAttr, uro);
                     return true;
                  }
               }
               result = std::make_unique<C_RunWith>(exec, lastAttr, uro);
               return true;
            }
            else {
               throw SyntaxError(str(L"last argument of command '",
                  word.getOriginString(uro), L" with' cannot be resolved to a string"), line);
            }
         }
      }
      else {
         if (right.check(TI_HAS_KEYWORD_WITH)) {
            std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_With);
            Tokens& left2 = pair2.first;
            Tokens& right2 = pair2.second;

            if (left2.isEmpty()) {
               throw SyntaxError(str(L"command '", word.getOriginString(uro),
                  L"' cannot be called with adjacent 'with' keywords"), line);

            }
            else if (right2.isEmpty()) {
               throw SyntaxError(str(L"command '", word.getOriginString(uro),
                  L" with with' cannot be called without its last argument"), line);
            }

            const _bool hasMemory = uro.vc.anyAttribute();
            Attribute* lastAttr = uro.vc.getLastAttribute();
            const ThisState prevThisState = uro.vars.inner.thisState;
            uro.vars.inner.thisState = ThisState::ts_String;
            _attrptr attr(new Attribute(uro));
            attr->setCoreCommandBase();
            uro.vc.addAttribute(attr);

            _genptr<_str> exec;
            if (!parse::parse(uro, left2, exec)) {
               throw SyntaxError(str(L"second argument of command '", word.getOriginString(uro),
                  L" with with' cannot be resolved to a string"), line);
            }

            _genptr<_str> lastStr;
            if (parse::parse(uro, right2, lastStr)) {
               uro.vars.inner.thisState = prevThisState;
               uro.vc.retreatAttribute();

               if (left2.getLength() == 1) {
                  const Token& cf = left2.first();
                  if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS) {

                     _comptr inner(new C_RunWithUroborosWithString(lastStr, uro));
                     if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
                        return true;
                     }

                     throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
                        L" with Uroboros with' cannot be resolved to a list"), line);
                  }
               }

               _comptr inner(new C_RunWithWithString(exec, lastStr, uro));
               if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
                  return true;
               }

               throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
                  L" with with' cannot be resolved to a list"), line);
            }
            else {
               _genptr<_list> lastList;

               if (!parse::parse(uro, right2, lastList)) {
                  uro.vars.inner.thisState = prevThisState;
                  uro.vc.retreatAttribute();
                  throw SyntaxError(str(L"last argument of command '", word.getOriginString(uro),
                     L" with with' cannot be resolved to a list"), line);
               }
               else {
                  uro.vars.inner.thisState = prevThisState;
                  uro.vc.retreatAttribute();

                  if (left2.getLength() == 1) {
                     const Token& cf = left2.first();
                     if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS) {

                        _comptr inner(new C_RunWithUroborosWith(lastList, uro));
                        if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
                           return true;
                        }

                        throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
                           L" with Uroboros with' cannot be resolved to a list"), line);
                     }
                  }

                  _comptr inner(new C_RunWithWith(exec, lastList, uro));
                  if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
                     return true;
                  }

                  throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
                     L" with with' cannot be resolved to a list"), line);
               }
            }
         }
         else {
            const _bool hasMemory = uro.vc.anyAttribute();
            Attribute* lastAttr = uro.vc.getLastAttribute();
            const ThisState prevThisState = uro.vars.inner.thisState;
            uro.vars.inner.thisState = ThisState::ts_String;
            _attrptr attr(new Attribute(uro));
            attr->setCoreCommandBase();
            uro.vc.addAttribute(attr);

            _genptr<_str> exec;
            if (!parse::parse(uro, right, exec)) {
               throw SyntaxError(str(L"last argument of command '", word.getOriginString(uro),
                  L" with' cannot be resolved to a string"), line);
            }

            uro.vars.inner.thisState = prevThisState;
            uro.vc.retreatAttribute();

            if (right.getLength() == 1) {
               const Token& cf = right.first();
               if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS) {

                  _comptr inner(new C_RunWithUroboros(uro));
                  if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
                     return true;
                  }

                  throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
                     L" with Uroboros' cannot be resolved to a list"), line);
               }
            }

            _comptr inner(new C_RunWith(exec, uro));
            if (parseLooped(left, inner, result, uro, attr, hasMemory)) {
               return true;
            }

            throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
               L" with' cannot be resolved to a list"), line);
         }
      }
   }
   else {
      Attribute* lastAttr = uro.vc.getLastAttribute();
      _genptr<_str> str;
      if (parse::parse(uro, tks, str)) {
         result = std::make_unique<C_Run>(str, lastAttr, uro);
         return true;
      }
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_error(_comptr& result, const Token& word, const Tokens& tks, const _int& line, _uro& uro)
{
   if (tks.isEmpty()) {
      result = std::make_unique<C_Error>(uro);
      return true;
   }

   _genptr<_num> num;

   if (parse::parse(uro, tks, num)) {
      result = std::make_unique<C_ErrorWithExitCode>(num, uro);
      return true;
   }
   else {
      throw SyntaxError(str(L"the argument of command '", word.getOriginString(uro),
         L"' cannot be resolved to a number"), line);
   }

   return false;
}

static void checkUselessFlags(const Token& word, const _int& line,
   const _bool& force, const _bool& stack, _uro& uro)
{
   if (force) {
      throw SyntaxError(str(L"keyword '", word.getOriginString(uro),
         L"' cannot be preceded by a flag 'forced'"), line);
   }

   if (stack) {
      throw SyntaxError(str(L"keyword '", word.getOriginString(uro),
         L"' cannot be preceded by a flag 'stack'"), line);
   }
}

static void commandSyntaxError(const _str& name, const _int& line)
{
   throw SyntaxError(str(L"wrong syntax of command '", name, L"'"), line);
}

static void commandNoArgException(const _str& name, const _int& line)
{
   throw SyntaxError(str(L"command '", name, L"' requires an argument"), line);
}

}
