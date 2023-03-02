/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
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
#include "../datatype/patterns.h"


namespace uro::comm
{

_bool keywordCommands(_comptr& result, const Token& word, Tokens& tks,
   const _int line, const _bool force, const _bool stack, _uro& uro)
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

static void checkFileContextExistence(const _str& commandName, const _int line, _uro& uro)
{
   if (!uro.contexts.hasFileContext()) {
      throw SyntaxError(str(L"the subject of command '", commandName, L"' is undefined here"), line);
   }
}

_bool parseLooped(const Tokens& tks, _comptr& innerCommand, _fcptr& ctx, _comptr& result, _uro& uro)
{
   _genptr<_str> str_;
   if (parse::parse(uro, tks, str_)) {
      result = std::make_unique<CS_StringComArg>(str_, innerCommand, ctx, uro);
      return true;
   }

   _defptr def;
   if (parse::parse(uro, tks, def)) {
      result = std::make_unique<CS_DefinitionComArg>(def, innerCommand, ctx, uro);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(uro, tks, list)) {
      result = std::make_unique<CS_ListComArg>(list, innerCommand, ctx, uro);
      return true;
   }

   return false;
}

/*_bool parseLooped(const Tokens& tks, _comptr& innerCommand, _comptr& result, _uro& uro)
{
   _attrptr attr = std::make_unique<Attribute>(uro);
   attr->setCoreCommandBase();
   return parseLooped(tks, innerCommand, result, uro, attr);
}*/

static void makeCoreCommandContext(_fcptr& result, _uro& uro)
{
   _attrptr attr = std::make_unique<Attribute>(uro);
   attr->setCoreCommandBase();
   result = std::make_unique<FileContext>(attr, uro);
}

static _bool kwCommandSimple(_comptr& result, const Token& word, Tokens& tks, const _int line, _uro& uro)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(uro), line, uro);
      FileContext* ctx = uro.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      return coreCommandSimple(result, word, ctx, true, uro);
   }

   /*_attrptr attr = std::make_unique<Attribute>(uro);
   attr->setCoreCommandBase();
   _fcptr ctx = std::make_unique<FileContext>(attr, uro);*/

   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);

   _comptr inner;
   if (coreCommandSimple(result, word, ctx.get(), false, uro) && parseLooped(tks, inner, ctx, result, uro)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool coreCommandSimple(_comptr& result, const Token& word, FileContext* context, const _bool saveChanges, _uro& uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete: {
         result = std::make_unique<C_Delete>(saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Drop: {
         result = std::make_unique<C_Drop>(saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Hide: {
         result = std::make_unique<C_Hide>(saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Lock: {
         result = std::make_unique<C_Lock>(saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Unlock: {
         result = std::make_unique<C_Unlock>(saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Unhide: {
         result = std::make_unique<C_Unhide>(saveChanges, context, uro);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool kwCommandTime(_comptr& result, const Token& word, Tokens& tks, const _int line, _uro& uro)
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
      checkFileContextExistence(word.getOriginString(uro), line, uro);
      FileContext* ctx = uro.contexts.getFileContext();
      ctx->attribute->setTimeCommandBase();

      _genptr<_tim> tim;
      if (!parse::parse(uro, right, tim)) {
         throw SyntaxError(str(L"time argument of command '", word.getOriginString(uro), L" to' is not valid"), line);
      }

      if (coreCommandTime(result, word, ctx, tim, true, uro)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" to'"), line);
   }

   _attrptr attr = std::make_unique<Attribute>(uro);
   attr->setTimeCommandBase();
   _fcptr ctx = std::make_unique<FileContext>(attr, uro);

   uro.contexts.addFileContext(ctx.get());

   _genptr<_tim> tim;
   if (!parse::parse(uro, right, tim)) {
      throw SyntaxError(str(L"time argument of command '", word.getOriginString(uro), L" to' is not valid"), line);
   }

   uro.contexts.retreatFileContext();

   _comptr inner;
   if (coreCommandTime(inner, word, ctx.get(), tim, false, uro) && parseLooped(left, inner, ctx, result, uro)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool coreCommandTime(_comptr& result, const Token& word, FileContext* context,
   _genptr<_tim>& time, const _bool saveChanges, _uro& uro)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Reaccess: {
         result = std::make_unique<C_ReaccessTo>(time, saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Rechange: {
         result = std::make_unique<C_RechangeTo>(time, saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Recreate: {
         result = std::make_unique<C_RecreateTo>(time, saveChanges, context, uro);
         break;
      }
      case Keyword::kw_Remodify: {
         result = std::make_unique<C_RemodifyTo>(time, saveChanges, context, uro);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool c_open(_comptr& result, const Token& word, const Tokens& tks, const _int line, _uro& uro)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(uro), line, uro);
      FileContext* ctx = uro.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      result = std::make_unique<C_Open>(true, ctx, uro);
      return true;
   }

   if (tks.check(TI_HAS_KEYWORD_WITH)) {
      P_DIVIDE_BY_KEYWORD(kw_With);

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),  L" with' does not "
            L"contain its last argument"), line);
      }

      _fcptr ctx;
      makeCoreCommandContext(ctx, uro);
      uro.contexts.addFileContext(ctx.get());

      _genptr<_str> prog;
      if (!parse::parse(uro, right, prog)) {
         throw SyntaxError(str(L"last argument of command '", word.getOriginString(uro), L" with' "
            L"cannot be resolved to a string"), line);
      }

      uro.contexts.retreatFileContext();

      if (left.isEmpty()) {
         checkFileContextExistence(str(word.getOriginString(uro), L" with"), line, uro);
         FileContext* ctx = uro.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();
         result = std::make_unique<C_OpenWith>(prog, true, ctx, uro);
         return true;
      }
      else {
         _comptr inner= std::make_unique<C_OpenWith>(prog, false, ctx.get(), uro);
         if (parseLooped(left, inner, ctx, result, uro)) {
            return true;
         }

         throw SyntaxError(str(L"wrong syntax of command '", word.getOriginString(uro), L" with'"), line);
      }
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);

   _comptr inner= std::make_unique<C_Open>(false, ctx.get(), uro);
   if (parseLooped(tks, inner, ctx, result, uro)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_select(_comptr& result, const Token& word, const Tokens& tks, const _int line, _uro& uro)
{
   if (uro.contexts.hasAggregate()) {
      Aggregate* aggr = uro.contexts.getAggregate();
      aggr->set(AGGR_SELECT);

      if (tks.isEmpty()) {
         FileContext* fc = uro.contexts.getFileContext();
         _genptr<_str> str = std::make_unique<VariableReference<_str>>(fc->this_.get());
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

static _bool c_rename(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _uro& uro)
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
      checkFileContextExistence(str(word.getOriginString(uro), L" to"), line, uro);
      FileContext* ctx = uro.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      ctx->attribute->markToEvaluate();

      _genptr<_str> newName;
      if (!parse::parse(uro, right, newName)) {
         throw SyntaxError(str(L"declaration of new name in command '", word.getOriginString(uro), L" to' is not valid"), line);
      }

      if (stack) {
         result = std::make_unique<C_RenameTo_Stack>(newName, true, extless, ctx, uro);
      }
      else {
         result = std::make_unique<C_RenameTo>(newName, true, force, extless, ctx, uro);
      }

      return true;
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);
   uro.contexts.addFileContext(ctx.get());

   _genptr<_str> newName;
   if (!parse::parse(uro, right, newName)) {
      throw SyntaxError(str(L"declaration of new name in command '", word.getOriginString(uro), L" to' is not valid"), line);
   }

   uro.contexts.retreatFileContext();

   _genptr<_str> str_;
   if (parse::parse(uro, left, str_)) {
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, ctx.get(), uro);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, force, extless, ctx.get(), uro);
      }

      result = std::make_unique<CS_StringComArg>(str_, inner, ctx, uro);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(uro, left, list)) {
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, ctx.get(), uro);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, force, extless, ctx.get(), uro);
      }

      result = std::make_unique<CS_ListComArg>(list, inner, ctx, uro);
      return true;
   }

   commandSyntaxError(str(word.getOriginString(uro), L" to"), line);
   return false;
}

static _bool c_create(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _uro& uro)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(uro), line, uro);
      FileContext* ctx = uro.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();

      if (stack) {
         result = std::make_unique<C_Create_Stack>(ctx, uro);
      }
      else {
         result = std::make_unique<C_Create>(force, ctx, uro);
      }

      return true;
   }

   const Token& f = tks.first();

   if (f.type == Token::t_Word) {
      Tokens tks2 = tks;
      tks2.trimLeft();
      const _hash& fk = f.value.word.h;

      if (fk == uro.hashes.HASH_VAR_FILE) {
         if (tks2.isEmpty()) {
            checkFileContextExistence(str(word.getOriginString(uro), L" ", f.getOriginString(uro)), line, uro);
            FileContext* ctx = uro.contexts.getFileContext();
            ctx->attribute->setCoreCommandBase();

            if (stack) {
               result = std::make_unique<C_CreateFile_Stack>(ctx, uro);
            }
            else {
               result = std::make_unique<C_CreateFile>(force, ctx, uro);
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
            checkFileContextExistence(str(word.getOriginString(uro), L" ", f.getOriginString(uro)), line, uro);
            FileContext* ctx = uro.contexts.getFileContext();
            ctx->attribute->setCoreCommandBase();

            if (stack) {
               result = std::make_unique<C_CreateDirectory_Stack>(ctx, uro);
            }
            else {
               result = std::make_unique<C_CreateDirectory>(force, ctx, uro);
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

static _bool c_moveTo(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _uro& uro)
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

         checkFileContextExistence(str(word.getOriginString(uro), L" to as"), line, uro);
         FileContext* ctx = uro.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, ctx, uro);
         }
         else {
            result = std::make_unique<C_MoveToAs>(dest, nname, force, extless, ctx, uro);
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
         checkFileContextExistence(str(word.getOriginString(uro), L" to"), line, uro);
         FileContext* ctx = uro.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_MoveTo_Stack>(str_, ctx, uro);
         }
         else {
            result = std::make_unique<C_MoveTo>(str_, force, ctx, uro);
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

      _fcptr ctx;
      makeCoreCommandContext(ctx, uro);
      uro.contexts.addFileContext(ctx.get());

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

      uro.contexts.retreatFileContext();
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, ctx.get(), uro);
      }
      else {
         inner = std::make_unique<C_MoveToAs>(dest, nname, force, extless, ctx.get(), uro);
      }

      if (parseLooped(left, inner, ctx, result, uro)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" to as"), line);
      return false;
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);
   uro.contexts.addFileContext(ctx.get());

   _genptr<_str> dest;
   if (!parse::parse(uro, right, dest)) {
      throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
         L" to' cannot be resolved to a string"), line);
   }

   uro.contexts.retreatFileContext();

   _comptr inner;
   if (stack) {
      inner = std::make_unique<C_MoveTo_Stack>(dest, ctx.get(), uro);
   }
   else {
      inner = std::make_unique<C_MoveTo>(dest, force, ctx.get(), uro);
   }

   if (parseLooped(left, inner, ctx, result, uro)) {
      return true;
   }

   commandSyntaxError(str(word.getOriginString(uro), L" to"), line);
   return false;
}

static _bool c_copy(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _uro& uro)
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

      if (uro.contexts.hasAggregate()) {
         Aggregate* aggr = uro.contexts.getAggregate();
         aggr->set(AGGR_COPY);

         if (tks.isEmpty()) {
            FileContext* fc = uro.contexts.getFileContext();
            _genptr<_str> str = std::make_unique<VariableReference<_str>>(fc->this_.get());
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

         checkFileContextExistence(str(word.getOriginString(uro), L" to as"), line, uro);
         FileContext* ctx = uro.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_CopyToAs_Stack>(dest, nname, true, extless, ctx, uro);
         }
         else {
            result = std::make_unique<C_CopyToAs>(dest, nname, true, force, extless, ctx, uro);
         }

         return true;
      }

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro),
            L" to' lacks a declaration of new location"), line);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, right, str_)) {
         checkFileContextExistence(str(word.getOriginString(uro), L" to"), line, uro);
         FileContext* ctx = uro.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_CopyTo_Stack>(str_, true, ctx, uro);
         }
         else {
            result = std::make_unique<C_CopyTo>(str_, true, force, ctx, uro);
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

      _fcptr ctx;
      makeCoreCommandContext(ctx, uro);
      uro.contexts.addFileContext(ctx.get());

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

      uro.contexts.retreatFileContext();

      _comptr inner;
      if (stack) {
         inner = std::make_unique<C_CopyToAs_Stack>(dest, nname, false, extless, ctx.get(), uro);
      }
      else {
         inner = std::make_unique<C_CopyToAs>(dest, nname, false, force, extless, ctx.get(), uro);
      }

      if (parseLooped(left, inner, ctx, result, uro)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(uro), L" to as"), line);
      return false;
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);
   uro.contexts.addFileContext(ctx.get());

   _genptr<_str> dest;
   if (!parse::parse(uro, right, dest)) {
      throw SyntaxError(str(L"new location in command '", word.getOriginString(uro),
         L" to' cannot be resolved to a string"), line);
   }

   uro.contexts.retreatFileContext();

   _comptr inner;
   if (stack) {
      inner = std::make_unique<C_CopyTo_Stack>(dest, false, ctx.get(), uro);
   }
   else {
      inner = std::make_unique<C_CopyTo>(dest, false, force, ctx.get(), uro);
   }

   if (parseLooped(left, inner, ctx, result, uro)) {
      return true;
   }

   commandSyntaxError(str(word.getOriginString(uro), L" to"), line);
   return false;
}

_bool c_print(_comptr& result, const Token& word, const Tokens& tks, const _int line, const _bool directError, _uro& uro)
{
   if (tks.isEmpty()) {
      if (!uro.contexts.hasIterationContext()) {
         throw SyntaxError(str(L"command '", word.getOriginString(uro), L"' needs an argument here. "
            L"Value of variable 'this' is undefined in this area"), line);
         return false;
      }

      FileContext* fc = uro.contexts.getFileContext();
      if (fc != nullptr) {
         result = std::make_unique<C_PrintThis>(uro, *fc);
         return true;
      }
      
      commandSyntaxError(word.getOriginString(uro), line);
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
      throw SyntaxError::wrongSyntax(line);
   }

   return false;
}

static _bool c_sleep(_comptr& result, const Token& word, const Tokens& tks, const _int line, _uro& uro)
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

static _bool c_error(_comptr& result, const Token& word, const Tokens& tks, const _int line, _uro& uro)
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

static _bool c_run(_comptr& result, const Token& word, const Tokens& tks, const _int line, _uro& uro)
{
   uro.contexts.markAllAttributesToRun();

   if (!tks.check(TI_HAS_KEYWORD_WITH)) {
      FileContext* ctx = uro.contexts.getFileContext();
      _genptr<_str> str;
      if (parse::parse(uro, tks, str)) {
         result = std::make_unique<C_Run>(str, ctx, uro);
         return true;
      }
      else {
         commandSyntaxError(word.getOriginString(uro), line);
      }
   }

   P_DIVIDE_BY_KEYWORD(kw_With);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"right side of command '", word.getOriginString(uro), L" with' is empty"), line);
   }

   if (left.isEmpty()) {
      return c_runContextless(result, word, right, line, uro);
   }
   else {
      return c_runContextfull(result, word, left, right, line, uro);
   }

   commandSyntaxError(word.getOriginString(uro), line);
   return false;
}

static _bool c_runContextless(_comptr& result, const Token& word, const Tokens& right, const _int line, _uro& uro)
{
   if (right.check(TI_HAS_KEYWORD_WITH)) {
      return c_runContextless_with(result, word, right, line, uro);
   }
   else {
      return c_runContextless_simple(result, word, right, line, uro);
   }
}

static _bool c_runContextless_simple(_comptr& result, const Token& word, const Tokens& right, const _int line, _uro& uro)
{
   if (!uro.contexts.hasFileContext()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro),
         L" with' needs first argument here"), line);
   }

   checkFileContextExistence(str(word.getOriginString(uro), L" with"), line, uro);
   FileContext* ctx = uro.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   _genptr<_str> exec;
   if (parse::parse(uro, right, exec)) {
      if (right.getLength() == 1) {
         const Token& cf = right.first();
         if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS2) {
            result = std::make_unique<C_RunWithUroboros2>(ctx, uro);
            return true;
         }
      }
      result = std::make_unique<C_RunWith>(exec, ctx, uro);
      return true;
   }
   else {
      throw SyntaxError(str(L"last argument of command '",
         word.getOriginString(uro), L" with' cannot be resolved to a string"), line);
   }

   return false;
}

static _bool c_runContextless_with(_comptr& result, const Token& word, const Tokens& right, const _int line, _uro& uro)
{
   if (!uro.contexts.hasFileContext()) {
      throw SyntaxError(str(L"command '", word.getOriginString(uro), L" with with' needs first argument here"), line);
   }

   checkFileContextExistence(str(word.getOriginString(uro), L" with with"), line, uro);
   FileContext* ctx = uro.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

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
      if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS2) {
         _genptr<_str> str_;

         if (parse::parse(uro, right2, str_)) {
            result = std::make_unique<C_RunWithUroboros2WithString>(str_, ctx, uro);
            return true;
         }
         else {
            _genptr<_list> list;
            if (parse::parse(uro, right2, list)) {
               result = std::make_unique<C_RunWithUroboros2With>(list, ctx, uro);
               return true;
            }
            else {
               throw SyntaxError(str(L"last argument of command '",
                  word.getOriginString(uro), L" with Uroboros2 with' cannot be resolved to a list"), line);
            }
         }
      }
   }

   _genptr<_str> str_;
   if (parse::parse(uro, right2, str_)) {
      result = std::make_unique<C_RunWithWithString>(exec, str_, ctx, uro);
      return true;
   }
   else {
      _genptr<_list> list;
      if (parse::parse(uro, right2, list)) {
         result = std::make_unique<C_RunWithWith>(exec, list, ctx, uro);
         return true;
      }
      else {
         throw SyntaxError(str(L"last argument of command '",
            word.getOriginString(uro), L" with with' cannot be resolved to a list"), line);
      }
   }
}

static _bool c_runContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, const _int line, _uro& uro)
{
   if (right.check(TI_HAS_KEYWORD_WITH)) {
      return c_runContextfull_with(result, word, left, right, line, uro);
   }
   else {
      return c_runContextfull_simple(result, word, left, right, line, uro);
   }
}

static _bool c_runContextfull_simple(_comptr& result, const Token& word, const Tokens& left,
   const Tokens& right, const _int line, _uro& uro)
{
   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);
   uro.contexts.addFileContext(ctx.get());

   _genptr<_str> exec;
   if (!parse::parse(uro, right, exec)) {
      throw SyntaxError(str(L"last argument of command '", word.getOriginString(uro),
         L" with' cannot be resolved to a string"), line);
   }

   uro.contexts.retreatFileContext();

   if (right.getLength() == 1) {
      const Token& cf = right.first();
      if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS2) {
         _comptr inner= std::make_unique<C_RunWithUroboros2>(ctx.get(), uro);
         if (parseLooped(left, inner, ctx, result, uro)) {
            return true;
         }

         throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
            L" with Uroboros2' cannot be resolved to a list"), line);
      }
   }

   _comptr inner= std::make_unique<C_RunWith>(exec, ctx.get(), uro);
   if (parseLooped(left, inner, ctx, result, uro)) {
      return true;
   }

   throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
      L" with' cannot be resolved to a list"), line);
}

static _bool c_runContextfull_with(_comptr& result, const Token& word, const Tokens& left,
   const Tokens& right, const _int line, _uro& uro)
{
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

   _fcptr ctx;
   makeCoreCommandContext(ctx, uro);
   uro.contexts.addFileContext(ctx.get());

   _genptr<_str> exec;
   if (!parse::parse(uro, left2, exec)) {
      throw SyntaxError(str(L"second argument of command '", word.getOriginString(uro),
         L" with with' cannot be resolved to a string"), line);
   }

   _genptr<_str> lastStr;
   if (parse::parse(uro, right2, lastStr)) {
      uro.contexts.retreatFileContext();

      if (left2.getLength() == 1) {
         const Token& cf = left2.first();
         if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS2) {
            _comptr inner= std::make_unique<C_RunWithUroboros2WithString>(lastStr, ctx.get(), uro);
            if (parseLooped(left, inner, ctx, result, uro)) {
               return true;
            }

            throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
               L" with Uroboros2 with' cannot be resolved to a list"), line);
         }
      }

      _comptr inner= std::make_unique<C_RunWithWithString>(exec, lastStr, ctx.get(), uro);
      if (parseLooped(left, inner, ctx, result, uro)) {
         return true;
      }

      throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
         L" with with' cannot be resolved to a list"), line);
   }
   else {
      _genptr<_list> lastList;

      if (!parse::parse(uro, right2, lastList)) {
         uro.contexts.retreatFileContext();
         throw SyntaxError(str(L"last argument of command '", word.getOriginString(uro),
            L" with with' cannot be resolved to a list"), line);
      }
      else {
         uro.contexts.retreatFileContext();

         if (left2.getLength() == 1) {
            const Token& cf = left2.first();
            if (cf.type == Token::t_Word && cf.value.word.h == uro.hashes.HASH_VAR_UROBOROS2) {
               _comptr inner= std::make_unique<C_RunWithUroboros2With>(lastList, ctx.get(), uro);
               if (parseLooped(left, inner, ctx, result, uro)) {
                  return true;
               }

               throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
                  L" with Uroboros2 with' cannot be resolved to a list"), line);
            }
         }

         _comptr inner= std::make_unique<C_RunWithWith>(exec, lastList, ctx.get(), uro);
         if (parseLooped(left, inner, ctx, result, uro)) {
            return true;
         }

         throw SyntaxError(str(L"first argument of command '", word.getOriginString(uro),
            L" with with' cannot be resolved to a list"), line);
      }
   }

   return false;
}

static void checkUselessFlags(const Token& word, const _int line,
   const _bool force, const _bool stack, _uro& uro)
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

static void commandSyntaxError(const _str& name, const _int line)
{
   throw SyntaxError(str(L"wrong syntax of command '", name, L"'"), line);
}

static void commandNoArgException(const _str& name, const _int line)
{
   throw SyntaxError(str(L"command '", name, L"' requires an argument"), line);
}

}
