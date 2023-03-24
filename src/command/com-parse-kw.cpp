/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
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


namespace perun2::comm
{

_bool keywordCommands(_comptr& result, const Token& word, Tokens& tks,
   const _int line, const _bool force, const _bool stack, _p2& p2)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Unlock:
      case Keyword::kw_Unhide: {
         checkUselessFlags(word, line, force, stack, p2);
         return kwCommandSimple(result, word, tks, line, p2);
      }
      case Keyword::kw_Open: {
         checkUselessFlags(word, line, force, stack, p2);
         return c_open(result, word, tks, line, p2);
      }
      case Keyword::kw_Copy: {
         return c_copy(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_Create: {
         return c_create(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_CreateFile: {
         return c_createFile(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_CreateDirectory: {
         return c_createDirectory(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_CreateFiles: {
         return c_createFiles(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_CreateDirectories: {
         return c_createDirectories(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_Break:
      case Keyword::kw_Continue:
      case Keyword::kw_Exit: {
         checkUselessFlags(word, line, force, stack, p2);
         throw SyntaxError(str(L"command '", word.getOriginString(p2), L"' cannot be called with an argument"), line);
      }
      case Keyword::kw_Error: {
         checkUselessFlags(word, line, force, stack, p2);
         return c_error(result, word, tks, line, p2);
      }
      case Keyword::kw_Move: {
         return c_moveTo(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_Print: {
         checkUselessFlags(word, line, force, stack, p2);
         return c_print(result, word, tks, line, true, p2);
      }
      case Keyword::kw_Reaccess:
      case Keyword::kw_Recreate:
      case Keyword::kw_Rechange:
      case Keyword::kw_Remodify: {
         checkUselessFlags(word, line, force, stack, p2);
         return kwCommandTime(result, word, tks, line, p2);
      }
      case Keyword::kw_Rename: {
         return c_rename(result, word, tks, line, force, stack, p2);
      }
      case Keyword::kw_Run: {
         checkUselessFlags(word, line, force, stack, p2);
         return c_run(result, word, tks, line, p2);
      }
      case Keyword::kw_Select: {
         checkUselessFlags(word, line, force, stack, p2);
         return c_select(result, word, tks, line, p2);
      }
      case Keyword::kw_Sleep: {
         checkUselessFlags(word, line, force, stack, p2);
         return c_sleep(result, word, tks, line, p2);
      }
   }

   throw SyntaxError(str(L"command cannot start with a keyword '", word.getOriginString(p2), L"'"), line);
   return false;
}

static void checkFileContextExistence(const _str& commandName, const _int line, _p2& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"the subject of command '", commandName, L"' is undefined here"), line);
   }
}

static _bool parseLooped(const Tokens& tks, _comptr& innerCommand, _fcptr& ctx, _comptr& result, _p2& p2)
{
   _genptr<_str> str_;
   if (parse::parse(p2, tks, str_)) {
      result = std::make_unique<CS_StringComArg>(str_, innerCommand, ctx, p2);
      return true;
   }

   _defptr def;
   if (parse::parse(p2, tks, def)) {
      result = std::make_unique<CS_DefinitionComArg>(def, innerCommand, ctx, p2);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(p2, tks, list)) {
      result = std::make_unique<CS_ListComArg>(list, innerCommand, ctx, p2);
      return true;
   }

   return false;
}

static void makeCoreCommandContext(_fcptr& result, _p2& p2)
{
   _attrptr attr = std::make_unique<Attribute>(p2);
   attr->setCoreCommandBase();
   result = std::make_unique<FileContext>(attr, p2);
}

static _bool kwCommandSimple(_comptr& result, const Token& word, Tokens& tks, const _int line, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();
      return coreCommandSimple(result, word, ctx, true, p2);
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.closeAttributeScope();

   _comptr inner;
   if (coreCommandSimple(inner, word, ctx.get(), false, p2) && parseLooped(tks, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool coreCommandSimple(_comptr& result, const Token& word, FileContext* context, const _bool saveChanges, _p2& p2)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Delete: {
         result = std::make_unique<C_Delete>(saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Drop: {
         result = std::make_unique<C_Drop>(saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Hide: {
         result = std::make_unique<C_Hide>(saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Lock: {
         result = std::make_unique<C_Lock>(saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Unlock: {
         result = std::make_unique<C_Unlock>(saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Unhide: {
         result = std::make_unique<C_Unhide>(saveChanges, context, p2);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool kwCommandTime(_comptr& result, const Token& word, Tokens& tks, const _int line, _p2& p2)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to' is empty"), line);
   }

   if (!tks.check(TI_HAS_KEYWORD_TO)) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to' does not contain keyword 'to'"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to' does not contain its time argument"), line);
   }

   if (left.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setTimeCommandBase();
      p2.contexts.closeDeepAttributeScope();

      _genptr<_tim> tim;
      if (!parse::parse(p2, right, tim)) {
         throw SyntaxError(str(L"time argument of command '", word.getOriginString(p2), L" to' is not valid"), line);
      }

      if (coreCommandTime(result, word, ctx, tim, true, p2)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(p2), L" to'"), line);
   }

   _attrptr attr = std::make_unique<Attribute>(p2);
   attr->setTimeCommandBase();
   p2.contexts.closeAttributeScope();
   _fcptr ctx = std::make_unique<FileContext>(attr, p2);

   p2.contexts.addFileContext(ctx.get());

   _genptr<_tim> tim;
   if (!parse::parse(p2, right, tim)) {
      throw SyntaxError(str(L"time argument of command '", word.getOriginString(p2), L" to' is not valid"), line);
   }

   p2.contexts.retreatFileContext();

   _comptr inner;
   if (coreCommandTime(inner, word, ctx.get(), tim, false, p2) && parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool coreCommandTime(_comptr& result, const Token& word, FileContext* context,
   _genptr<_tim>& time, const _bool saveChanges, _p2& p2)
{
   switch (word.value.keyword.k) {
      case Keyword::kw_Reaccess: {
         result = std::make_unique<C_ReaccessTo>(time, saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Rechange: {
         result = std::make_unique<C_RechangeTo>(time, saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Recreate: {
         result = std::make_unique<C_RecreateTo>(time, saveChanges, context, p2);
         break;
      }
      case Keyword::kw_Remodify: {
         result = std::make_unique<C_RemodifyTo>(time, saveChanges, context, p2);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

static _bool c_open(_comptr& result, const Token& word, const Tokens& tks, const _int line, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();
      result = std::make_unique<C_Open>(true, ctx, p2);
      return true;
   }

   if (tks.check(TI_HAS_KEYWORD_WITH)) {
      P_DIVIDE_BY_KEYWORD(kw_With);

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2),  L" with' does not "
            L"contain its last argument"), line);
      }

      _fcptr ctx;
      makeCoreCommandContext(ctx, p2);
      p2.contexts.addFileContext(ctx.get());

      _genptr<_str> prog;
      if (!parse::parse(p2, right, prog)) {
         throw SyntaxError(str(L"last argument of command '", word.getOriginString(p2), L" with' "
            L"cannot be resolved to a string"), line);
      }

      p2.contexts.retreatFileContext();

      if (left.isEmpty()) {
         checkFileContextExistence(str(word.getOriginString(p2), L" with"), line, p2);
         FileContext* ctx = p2.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();
         p2.contexts.closeDeepAttributeScope();
         result = std::make_unique<C_OpenWith>(prog, true, ctx, p2);
         return true;
      }
      else {
         p2.contexts.closeAttributeScope();
         _comptr inner= std::make_unique<C_OpenWith>(prog, false, ctx.get(), p2);
         if (parseLooped(left, inner, ctx, result, p2)) {
            return true;
         }

         throw SyntaxError(str(L"wrong syntax of command '", word.getOriginString(p2), L" with'"), line);
      }
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.closeAttributeScope();

   _comptr inner= std::make_unique<C_Open>(false, ctx.get(), p2);
   if (parseLooped(tks, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool c_select(_comptr& result, const Token& word, const Tokens& tks, const _int line, _p2& p2)
{
   if (p2.contexts.hasAggregate()) {
      Aggregate* aggr = p2.contexts.getAggregate();
      aggr->set(AGGR_SELECT);

      if (tks.isEmpty()) {
         FileContext* fc = p2.contexts.getFileContext();
         fc->attribute->setCoreCommandBase();
         fc->attribute->add(p2.hashes.HASH_VAR_PARENT);
         result = std::make_unique<C_AggrSelect_This>(aggr, *fc);
         return true;
      }

      _genptr<_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_AggrSelect_String>(aggr, str, p2);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, tks, list)) {
         result = std::make_unique<C_AggrSelect_List>(aggr, list, p2);
         return true;
      }
   }
   else {
      if (tks.isEmpty()) {
         commandNoArgException(word.getOriginString(p2), line);
      }

      _genptr<_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_Select_String>(str, p2);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, tks, list)) {
         result = std::make_unique<C_Select_List>(list, p2);
         return true;
      }
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool c_rename(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to' is empty"), line);
   }

   if (!tks.check(TI_HAS_KEYWORD_TO)) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),  L" to' does not contain keyword 'to'"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to' ",
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
      checkFileContextExistence(str(word.getOriginString(p2), L" to"), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();
      //ctx->attribute->markToEvaluate();

      _genptr<_str> newName;
      if (!parse::parse(p2, right, newName)) {
         throw SyntaxError(str(L"declaration of new name in command '", word.getOriginString(p2), L" to' is not valid"), line);
      }

      if (stack) {
         result = std::make_unique<C_RenameTo_Stack>(newName, true, extless, ctx, p2);
      }
      else {
         result = std::make_unique<C_RenameTo>(newName, true, force, extless, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   _genptr<_str> newName;
   if (!parse::parse(p2, right, newName)) {
      throw SyntaxError(str(L"declaration of new name in command '", word.getOriginString(p2), L" to' is not valid"), line);
   }

   p2.contexts.retreatFileContext();

   _genptr<_str> str_;
   if (parse::parse(p2, left, str_)) {
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, ctx.get(), p2);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, force, extless, ctx.get(), p2);
      }

      result = std::make_unique<CS_StringComArg>(str_, inner, ctx, p2);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(p2, left, list)) {
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, ctx.get(), p2);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, force, extless, ctx.get(), p2);
      }

      result = std::make_unique<CS_ListComArg>(list, inner, ctx, p2);
      return true;
   }

   commandSyntaxError(str(word.getOriginString(p2), L" to"), line);
   return false;
}

static _bool c_create(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (stack) {
         result = std::make_unique<C_Create_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_Create>(force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();
   const Token& f = tks.first();

   _genptr<_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (stack) {
         result = std::make_unique<C_Create_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_Create_String>(str_, force, p2);
      }

      return true;
   }

   _genptr<_list> list;
   if (parse::parse(p2, tks, list)) {
      if (stack) {
         result = std::make_unique<C_Create_List_Stack>(list, p2);
      }
      else {
         result = std::make_unique<C_Create_List>(list, force, p2);
      }

      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}


static _bool c_createFile(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (stack) {
         result = std::make_unique<C_CreateFile_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateFile>(force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   _genptr<_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (stack) {
         result = std::make_unique<C_CreateFile_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateFile_String>(str_, force, p2);
      }

      return true;
   }

   throw SyntaxError(str(L"the argument of command '", word.getOriginString(p2), L"' cannot be resolved to a string"), line);
}

static _bool c_createDirectory(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (stack) {
         result = std::make_unique<C_CreateDirectory_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory>(force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   _genptr<_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (stack) {
         result = std::make_unique<C_CreateDirectory_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory_String>(str_, force, p2);
      }

      return true;
   }

   throw SyntaxError(str(L"argument of command '", word.getOriginString(p2), L"' cannot be resolved to a string"), line);
}

static _bool c_createFiles(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (stack) {
         result = std::make_unique<C_CreateFile_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateFile>(force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();
   const Token& f = tks.first();

   _genptr<_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (stack) {
         result = std::make_unique<C_CreateFile_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateFile_String>(str_, force, p2);
      }

      return true;
   }

   _genptr<_list> list;
   if (parse::parse(p2, tks, list)) {
      if (stack) {
         result = std::make_unique<C_CreateFiles_List_Stack>(list, p2);
      }
      else {
         result = std::make_unique<C_CreateFiles_List>(list, force, p2);
      }

      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool c_createDirectories(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.getOriginString(p2), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (stack) {
         result = std::make_unique<C_CreateDirectory_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory>(force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();
   const Token& f = tks.first();

   _genptr<_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (stack) {
         result = std::make_unique<C_CreateDirectory_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory_String>(str_, force, p2);
      }

      return true;
   }

   _genptr<_list> list;
   if (parse::parse(p2, tks, list)) {
      if (stack) {
         result = std::make_unique<C_CreateDirectories_List_Stack>(list, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectories_List>(list, force, p2);
      }

      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool c_moveTo(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to' is empty"), line);
   }

   const _bool hasTo = tks.check(TI_HAS_KEYWORD_TO);
   const _bool hasAs = tks.check(TI_HAS_KEYWORD_AS);

   if (!hasTo) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L" to' cannot be called without keyword 'to'"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (left.isEmpty()) {
      if (hasAs) {
         std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
         Tokens& preAs = pair2.first;
         Tokens& postAs = pair2.second;

         if (preAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _bool extless = false;
         const Token& paf = postAs.first();

         if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxError(str(L"keyword '", paf.getOriginString(p2),
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         _genptr<_str> nname;
         if (!parse::parse(p2, postAs, nname)) {
            throw SyntaxError(str(L"new name in command '", word.getOriginString(p2),
               L" to as' cannot be resolved to a string"), line);
         }

         _genptr<_str> dest;
         if (!parse::parse(p2, preAs, dest)) {
            throw SyntaxError(str(L"new location in command '", word.getOriginString(p2),
               L" to' cannot be resolved to a string"), line);
         }

         checkFileContextExistence(str(word.getOriginString(p2), L" to as"), line, p2);
         FileContext* ctx = p2.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();
         p2.contexts.closeDeepAttributeScope();

         if (stack) {
            result = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, ctx, p2);
         }
         else {
            result = std::make_unique<C_MoveToAs>(dest, nname, force, extless, ctx, p2);
         }

         return true;
      }

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2),
            L" to' lacks declaration of a new location"), line);
      }

      _genptr<_str> str_;
      if (!parse::parse(p2, right, str_)) {
         throw SyntaxError(str(L"new location in command '",
            word.getOriginString(p2), L" to' cannot be resolved to a string"), line);
      }
      else {
         checkFileContextExistence(str(word.getOriginString(p2), L" to"), line, p2);
         FileContext* ctx = p2.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_MoveTo_Stack>(str_, ctx, p2);
         }
         else {
            result = std::make_unique<C_MoveTo>(str_, force, ctx, p2);
         }

         return true;
      }
   }

   p2.contexts.closeAttributeScope();

   if (hasAs) {
      if (left.check(TI_HAS_KEYWORD_AS)) {
         throw SyntaxError(str(L"keywords 'to' and 'as' appear in command '",
            word.getOriginString(p2), L" to as' in reverse order"), line);
      }

      std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
      Tokens& preAs = pair2.first;
      Tokens& postAs = pair2.second;

      if (preAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
            L"does not contain declaration of a new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
            L"does not contain declaration of a new name written after keyword 'as'"), line);
      }

      _bool extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"keyword '", paf.getOriginString(p2),
               L"' is not followed by a declaration of a new file name"), line);
         }
      }

      _fcptr ctx;
      makeCoreCommandContext(ctx, p2);
      p2.contexts.addFileContext(ctx.get());

      _genptr<_str> nname;
      if (!parse::parse(p2, postAs, nname)) {
         throw SyntaxError(str(L"new name in command '", word.getOriginString(p2),
            L" to as' cannot be resolved to a string"), line);
      }

      _genptr<_str> dest;
      if (!parse::parse(p2, preAs, dest)) {
         throw SyntaxError(str(L"new location in command '", word.getOriginString(p2),
            L" to' cannot be resolved to a string"), line);
      }

      p2.contexts.retreatFileContext();
      _comptr inner;

      if (stack) {
         inner = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, ctx.get(), p2);
      }
      else {
         inner = std::make_unique<C_MoveToAs>(dest, nname, force, extless, ctx.get(), p2);
      }

      if (parseLooped(left, inner, ctx, result, p2)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(p2), L" to as"), line);
      return false;
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   _genptr<_str> dest;
   if (!parse::parse(p2, right, dest)) {
      throw SyntaxError(str(L"new location in command '", word.getOriginString(p2),
         L" to' cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   _comptr inner;
   if (stack) {
      inner = std::make_unique<C_MoveTo_Stack>(dest, ctx.get(), p2);
   }
   else {
      inner = std::make_unique<C_MoveTo>(dest, force, ctx.get(), p2);
   }

   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(str(word.getOriginString(p2), L" to"), line);
   return false;
}

static _bool c_copy(_comptr& result, const Token& word, const Tokens& tks, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   const _bool hasTo = tks.check(TI_HAS_KEYWORD_TO);
   const _bool hasAs = tks.check(TI_HAS_KEYWORD_AS);

   if (!hasTo) {
      if (hasAs) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2),
            L" to as' cannot be called without keyword 'to'"), line);
      }

      if (force) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2),
            L"' cannot be preceded by a flag 'forced'"), line);
      }

      if (stack) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L"' cannot be preceded by a flag 'stack'"), line);
      }

      if (p2.contexts.hasAggregate()) {
         Aggregate* aggr = p2.contexts.getAggregate();
         aggr->set(AGGR_COPY);

         if (tks.isEmpty()) {
            FileContext* fc = p2.contexts.getFileContext();
            fc->attribute->setCoreCommandBase();
            result = std::make_unique<C_AggrCopy_This>(aggr, *fc);
            return true;
         }

         _genptr<_str> str;
         if (parse::parse(p2, tks, str)) {
            result = std::make_unique<C_AggrCopy_String>(aggr, str, p2);
            return true;
         }

         _genptr<_list> list;
         if (parse::parse(p2, tks, list)) {
            result = std::make_unique<C_AggrCopy_List>(aggr, list, p2);
            return true;
         }
      }
      else {
         if (tks.isEmpty()) {
            commandNoArgException(word.getOriginString(p2), line);
         }

         _genptr<_str> str;
         if (parse::parse(p2, tks, str)) {
            result = std::make_unique<C_Copy_String>(str, p2);
            return true;
         }

         _genptr<_list> list;
         if (parse::parse(p2, tks, list)) {
            result = std::make_unique<C_Copy_List>(list, p2);
            return true;
         }
      }

      commandSyntaxError(word.getOriginString(p2), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (left.isEmpty()) {
      p2.contexts.closeDeepAttributeScope();

      if (hasAs) {
         std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
         Tokens& preAs = pair2.first;
         Tokens& postAs = pair2.second;

         if (preAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
               L"does not contain a declaration of new location written between "
               L"keywords 'to' and 'as'"), line);
         }
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
               L"does not contain a declaration of new name written after keyword 'as'"), line);
         }

         _bool extless = false;
         const Token& paf = postAs.first();

         if (paf.isKeyword(Keyword::kw_Extensionless)) {
            extless = true;
            postAs.trimLeft();
            if (postAs.isEmpty()) {
               throw SyntaxError(str(L"keyword '", paf.getOriginString(p2),
                  L"' is not followed by a declaration of new file name"), line);
            }
         }

         _genptr<_str> nname;
         if (!parse::parse(p2, postAs, nname)) {
            throw SyntaxError(str(L"new name in command '", word.getOriginString(p2),
               L" to as' cannot be resolved to a string"), line);
         }

         _genptr<_str> dest;
         if (!parse::parse(p2, preAs, dest)) {
            throw SyntaxError(str(L"new location in command '", word.getOriginString(p2),
               L" to' cannot be resolved to a string"), line);
         }

         checkFileContextExistence(str(word.getOriginString(p2), L" to as"), line, p2);
         FileContext* ctx = p2.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_CopyToAs_Stack>(dest, nname, true, extless, ctx, p2);
         }
         else {
            result = std::make_unique<C_CopyToAs>(dest, nname, true, force, extless, ctx, p2);
         }

         return true;
      }

      if (right.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2),
            L" to' lacks a declaration of new location"), line);
      }

      _genptr<_str> str_;
      if (parse::parse(p2, right, str_)) {
         checkFileContextExistence(str(word.getOriginString(p2), L" to"), line, p2);
         FileContext* ctx = p2.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();

         if (stack) {
            result = std::make_unique<C_CopyTo_Stack>(str_, true, ctx, p2);
         }
         else {
            result = std::make_unique<C_CopyTo>(str_, true, force, ctx, p2);
         }

         return true;
      }
      else {
         throw SyntaxError(str(L"new location in command '",
            word.getOriginString(p2), L" to' cannot be resolved to a string"), line);
      }
   }

   p2.contexts.closeAttributeScope();

   if (hasAs) {
      if (left.check(TI_HAS_KEYWORD_AS)) {
         throw SyntaxError(str(L"keywords 'to' and 'as' appear in "
            L"command '", word.getOriginString(p2), L" to as' in reverse order"), line);
      }

      std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
      Tokens& preAs = pair2.first;
      Tokens& postAs = pair2.second;

      if (preAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
            L"does not contain a declaration of new location written between "
            L"keywords 'to' and 'as'"), line);
      }
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2), L" to as' "
            L"does not contain a declaration of new name written after keyword 'as'"), line);
      }

      _bool extless = false;
      const Token& paf = postAs.first();

      if (paf.isKeyword(Keyword::kw_Extensionless)) {
         extless = true;
         postAs.trimLeft();
         if (postAs.isEmpty()) {
            throw SyntaxError(str(L"keyword '", paf.getOriginString(p2),
               L"' is not followed by a declaration of new file name"), line);
         }
      }

      _fcptr ctx;
      makeCoreCommandContext(ctx, p2);
      p2.contexts.addFileContext(ctx.get());

      _genptr<_str> nname;
      if (!parse::parse(p2, postAs, nname)) {
         throw SyntaxError(str(L"new name in command '", word.getOriginString(p2),
            L" to as' cannot be resolved to a string"), line);
      }

      _genptr<_str> dest;
      if (!parse::parse(p2, preAs, dest)) {
         throw SyntaxError(str(L"new location in command '", word.getOriginString(p2),
            L" to' cannot be resolved to a string"), line);
      }

      p2.contexts.retreatFileContext();

      _comptr inner;
      if (stack) {
         inner = std::make_unique<C_CopyToAs_Stack>(dest, nname, false, extless, ctx.get(), p2);
      }
      else {
         inner = std::make_unique<C_CopyToAs>(dest, nname, false, force, extless, ctx.get(), p2);
      }

      if (parseLooped(left, inner, ctx, result, p2)) {
         return true;
      }

      commandSyntaxError(str(word.getOriginString(p2), L" to as"), line);
      return false;
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   _genptr<_str> dest;
   if (!parse::parse(p2, right, dest)) {
      throw SyntaxError(str(L"new location in command '", word.getOriginString(p2),
         L" to' cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   _comptr inner;
   if (stack) {
      inner = std::make_unique<C_CopyTo_Stack>(dest, false, ctx.get(), p2);
   }
   else {
      inner = std::make_unique<C_CopyTo>(dest, false, force, ctx.get(), p2);
   }

   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(str(word.getOriginString(p2), L" to"), line);
   return false;
}

_bool c_print(_comptr& result, const Token& word, const Tokens& tks, const _int line, const _bool directError, _p2& p2)
{
   if (tks.isEmpty()) {
      if (!p2.contexts.hasIterationContext()) {
         throw SyntaxError(str(L"command '", word.getOriginString(p2), L"' needs an argument here. "
            L"Value of variable 'this' is undefined in this area"), line);
         return false;
      }

      FileContext* fc = p2.contexts.getFileContext();
      if (fc != nullptr) {
         result = std::make_unique<C_PrintThis>(p2, *fc);
         return true;
      }
      
      commandSyntaxError(word.getOriginString(p2), line);
   }

   _genptr<_str> str;
   if (parse::parse(p2, tks, str)) {
      result = std::make_unique<C_PrintSingle>(str, p2);
      return true;
   }

   _defptr def;
   if (parse::parse(p2, tks, def)) {
      result = std::make_unique<C_PrintDefinition>(def, p2);
      return true;
   }

   _genptr<_list> list;
   if (parse::parse(p2, tks, list)) {
      result = std::make_unique<C_PrintList>(list, p2);
      return true;
   }

   if (directError) {
      commandSyntaxError(word.getOriginString(p2), line);
   }
   else {
      throw SyntaxError::wrongSyntax(line);
   }

   return false;
}

static _bool c_sleep(_comptr& result, const Token& word, const Tokens& tks, const _int line, _p2& p2)
{
   _genptr<_per> per;
   if (parse::parse(p2, tks, per)) {
      result = std::make_unique<C_SleepPeriod>(per, p2);
      return true;
   }

   _genptr<_num> num;
   if (parse::parse(p2, tks, num)) {
      result = std::make_unique<C_SleepMs>(num, p2);
      return true;
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool c_error(_comptr& result, const Token& word, const Tokens& tks, const _int line, _p2& p2)
{
   if (tks.isEmpty()) {
      result = std::make_unique<C_Error>(p2);
      return true;
   }

   _genptr<_num> num;

   if (parse::parse(p2, tks, num)) {
      result = std::make_unique<C_ErrorWithExitCode>(num, p2);
      return true;
   }
   else {
      throw SyntaxError(str(L"the argument of command '", word.getOriginString(p2),
         L"' cannot be resolved to a number"), line);
   }

   return false;
}

static _bool c_run(_comptr& result, const Token& word, const Tokens& tks, const _int line, _p2& p2)
{
   p2.contexts.closeAttributeScope();

   if (!tks.check(TI_HAS_KEYWORD_WITH)) {
      _genptr<_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_Run>(str, p2);
         return true;
      }
      else {
         commandSyntaxError(word.getOriginString(p2), line);
      }
   }

   P_DIVIDE_BY_KEYWORD(kw_With);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"right side of command '", word.getOriginString(p2), L" with' is empty"), line);
   }

   if (left.isEmpty()) {
      return c_runContextless(result, word, right, line, p2);
   }
   else {
      return c_runContextfull(result, word, left, right, line, p2);
   }

   commandSyntaxError(word.getOriginString(p2), line);
   return false;
}

static _bool c_runContextless(_comptr& result, const Token& word, const Tokens& right, const _int line, _p2& p2)
{
   if (right.check(TI_HAS_KEYWORD_WITH)) {
      return c_runContextless_with(result, word, right, line, p2);
   }
   else {
      return c_runContextless_simple(result, word, right, line, p2);
   }
}

static _bool c_runContextless_simple(_comptr& result, const Token& word, const Tokens& right, const _int line, _p2& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L" with' needs first argument here"), line);
   }

   checkFileContextExistence(str(word.getOriginString(p2), L" with"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   _genptr<_str> exec;
   if (parse::parse(p2, right, exec)) {
      if (right.getLength() == 1) {
         const Token& cf = right.first();
         if (cf.type == Token::t_Word && cf.value.word.h == p2.hashes.HASH_VAR_PERUN2) {
            result = std::make_unique<C_RunWithPerun2>(ctx, p2);
            p2.cache.loadCmdPath();
            return true;
         }
      }
      result = std::make_unique<C_RunWith>(exec, ctx, p2);
      return true;
   }
   else {
      throw SyntaxError(str(L"last argument of command '",
         word.getOriginString(p2), L" with' cannot be resolved to a string"), line);
   }

   return false;
}

static _bool c_runContextless_with(_comptr& result, const Token& word, const Tokens& right, const _int line, _p2& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2), L" with with' needs first argument here"), line);
   }

   checkFileContextExistence(str(word.getOriginString(p2), L" with with"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_With);
   Tokens& left2 = pair2.first;
   Tokens& right2 = pair2.second;

   if (left2.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L"' cannot be called with adjacent 'with' keywords"), line);
   }
   else if (right2.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L" with with' cannot be called without its last argument"), line);
   }

   _genptr<_str> exec;
   if (!parse::parse(p2, left2, exec)) {
      throw SyntaxError(str(L"second argument of command '", word.getOriginString(p2),
         L" with with' cannot be resolved to a string"), line);
   }

   if (left2.getLength() == 1) {
      const Token& cf = left2.first();
      if (cf.type == Token::t_Word && cf.value.word.h == p2.hashes.HASH_VAR_PERUN2) {
         _genptr<_str> str_;

         if (parse::parse(p2, right2, str_)) {
            result = std::make_unique<C_RunWithPerun2WithString>(str_, ctx, p2);
            p2.cache.loadCmdPath();
            return true;
         }
         else {
            _genptr<_list> list;
            if (parse::parse(p2, right2, list)) {
               result = std::make_unique<C_RunWithPerun2With>(list, ctx, p2);
               p2.cache.loadCmdPath();
               return true;
            }
            else {
               throw SyntaxError(str(L"last argument of command '",
                  word.getOriginString(p2), L" with Perun2 with' cannot be resolved to a list"), line);
            }
         }
      }
   }

   _genptr<_str> str_;
   if (parse::parse(p2, right2, str_)) {
      result = std::make_unique<C_RunWithWithString>(exec, str_, ctx, p2);
      return true;
   }
   else {
      _genptr<_list> list;
      if (parse::parse(p2, right2, list)) {
         result = std::make_unique<C_RunWithWith>(exec, list, ctx, p2);
         return true;
      }
      else {
         throw SyntaxError(str(L"last argument of command '",
            word.getOriginString(p2), L" with with' cannot be resolved to a list"), line);
      }
   }
}

static _bool c_runContextfull(_comptr& result, const Token& word, const Tokens& left, const Tokens& right, const _int line, _p2& p2)
{
   if (right.check(TI_HAS_KEYWORD_WITH)) {
      return c_runContextfull_with(result, word, left, right, line, p2);
   }
   else {
      return c_runContextfull_simple(result, word, left, right, line, p2);
   }
}

static _bool c_runContextfull_simple(_comptr& result, const Token& word, const Tokens& left,
   const Tokens& right, const _int line, _p2& p2)
{
   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   _genptr<_str> exec;
   if (!parse::parse(p2, right, exec)) {
      throw SyntaxError(str(L"last argument of command '", word.getOriginString(p2),
         L" with' cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   if (right.getLength() == 1) {
      const Token& cf = right.first();
      if (cf.type == Token::t_Word && cf.value.word.h == p2.hashes.HASH_VAR_PERUN2) {
         _comptr inner= std::make_unique<C_RunWithPerun2>(ctx.get(), p2);
         if (parseLooped(left, inner, ctx, result, p2)) {
            p2.cache.loadCmdPath();
            return true;
         }

         throw SyntaxError(str(L"first argument of command '", word.getOriginString(p2),
            L" with Perun2' cannot be resolved to a list"), line);
      }
   }

   _comptr inner= std::make_unique<C_RunWith>(exec, ctx.get(), p2);
   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   throw SyntaxError(str(L"first argument of command '", word.getOriginString(p2),
      L" with' cannot be resolved to a list"), line);
}

static _bool c_runContextfull_with(_comptr& result, const Token& word, const Tokens& left,
   const Tokens& right, const _int line, _p2& p2)
{
   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_With);
   Tokens& left2 = pair2.first;
   Tokens& right2 = pair2.second;

   if (left2.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L"' cannot be called with adjacent 'with' keywords"), line);
   }
   else if (right2.isEmpty()) {
      throw SyntaxError(str(L"command '", word.getOriginString(p2),
         L" with with' cannot be called without its last argument"), line);
   }

   _fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   _genptr<_str> exec;
   if (!parse::parse(p2, left2, exec)) {
      throw SyntaxError(str(L"second argument of command '", word.getOriginString(p2),
         L" with with' cannot be resolved to a string"), line);
   }

   _genptr<_str> lastStr;
   if (parse::parse(p2, right2, lastStr)) {
      p2.contexts.retreatFileContext();

      if (left2.getLength() == 1) {
         const Token& cf = left2.first();
         if (cf.type == Token::t_Word && cf.value.word.h == p2.hashes.HASH_VAR_PERUN2) {
            _comptr inner= std::make_unique<C_RunWithPerun2WithString>(lastStr, ctx.get(), p2);
            if (parseLooped(left, inner, ctx, result, p2)) {
               p2.cache.loadCmdPath();
               return true;
            }

            throw SyntaxError(str(L"first argument of command '", word.getOriginString(p2),
               L" with Perun2 with' cannot be resolved to a list"), line);
         }
      }

      _comptr inner= std::make_unique<C_RunWithWithString>(exec, lastStr, ctx.get(), p2);
      if (parseLooped(left, inner, ctx, result, p2)) {
         return true;
      }

      throw SyntaxError(str(L"first argument of command '", word.getOriginString(p2),
         L" with with' cannot be resolved to a list"), line);
   }
   else {
      _genptr<_list> lastList;

      if (!parse::parse(p2, right2, lastList)) {
         p2.contexts.retreatFileContext();
         throw SyntaxError(str(L"last argument of command '", word.getOriginString(p2),
            L" with with' cannot be resolved to a list"), line);
      }
      else {
         p2.contexts.retreatFileContext();

         if (left2.getLength() == 1) {
            const Token& cf = left2.first();
            if (cf.type == Token::t_Word && cf.value.word.h == p2.hashes.HASH_VAR_PERUN2) {
               _comptr inner= std::make_unique<C_RunWithPerun2With>(lastList, ctx.get(), p2);
               if (parseLooped(left, inner, ctx, result, p2)) {
                  p2.cache.loadCmdPath();
                  return true;
               }

               throw SyntaxError(str(L"first argument of command '", word.getOriginString(p2),
                  L" with Perun2 with' cannot be resolved to a list"), line);
            }
         }

         _comptr inner= std::make_unique<C_RunWithWith>(exec, lastList, ctx.get(), p2);
         if (parseLooped(left, inner, ctx, result, p2)) {
            return true;
         }

         throw SyntaxError(str(L"first argument of command '", word.getOriginString(p2),
            L" with with' cannot be resolved to a list"), line);
      }
   }

   return false;
}

static void checkUselessFlags(const Token& word, const _int line,
   const _bool force, const _bool stack, _p2& p2)
{
   if (force) {
      throw SyntaxError(str(L"keyword '", word.getOriginString(p2),
         L"' cannot be preceded by a flag 'forced'"), line);
   }

   if (stack) {
      throw SyntaxError(str(L"keyword '", word.getOriginString(p2),
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
