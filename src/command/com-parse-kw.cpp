/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../include/perun2/command/com-parse-kw.hpp"
#include "../../include/perun2/command/com-misc.hpp"
#include "../../include/perun2/command/com-execute.hpp"
#include "../../include/perun2/command/com-core.hpp"
#include "../../include/perun2/exception.hpp"
#include "../../include/perun2/lexer.hpp"
#include "../../include/perun2/datatype/parse-gen.hpp"
#include "../../include/perun2/command/com-struct.hpp"
#include "../../include/perun2/command/com-arg.hpp"
#include "../../include/perun2/command/com-core-aggr.hpp"
#include "../../include/perun2/command/com-create.hpp"
#include "../../include/perun2/command/com-time.hpp"
#include "../../include/perun2/command/com-renameto.hpp"
#include "../../include/perun2/command/com-copyto.hpp"
#include "../../include/perun2/command/com-moveto.hpp"
#include "../../include/perun2/command/com-def-action.hpp"
#include "../../include/perun2/python3/com-python3.hpp"
#include "../../include/perun2/datatype/generator/gen-string.hpp"
#include "../../include/perun2/datatype/patterns.hpp"


namespace perun2::comm
{

p_bool keywordCommands(p_comptr& result, const Token& word, Tokens& tks,
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   switch (word.value.keyword) {
      case Keyword::kw_Delete:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Unlock:
      case Keyword::kw_Unhide: {
         checkUselessFlags(word, line, mode, p2);
         return kwCommandSimple(result, word, tks, line, p2);
      }
      case Keyword::kw_Open: {
         checkUselessFlags(word, line, mode, p2);
         return c_open(result, word, tks, line, p2);
      }
      case Keyword::kw_Copy: {
         return c_copy(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_Create: {
         return c_create(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_CreateFile: {
         return c_createFile(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_CreateDirectory: {
         return c_createDirectory(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_CreateFiles: {
         return c_createFiles(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_CreateDirectories: {
         return c_createDirectories(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_Break:
      case Keyword::kw_Continue:
      case Keyword::kw_Exit: {
         checkUselessFlags(word, line, mode, p2);
         throw SyntaxError(str(L"the command \"", word.origin, L"\" cannot be called with an argument"), line);
      }
      case Keyword::kw_Error: {
         checkUselessFlags(word, line, mode, p2);
         return c_error(result, word, tks, line, p2);
      }
      case Keyword::kw_Move: {
         return c_moveTo(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_Print: {
         checkUselessFlags(word, line, mode, p2);
         return c_print(result, word, tks, line, true, p2);
      }
      case Keyword::kw_Reaccess:
      case Keyword::kw_Recreate:
      case Keyword::kw_Rechange:
      case Keyword::kw_Remodify: {
         checkUselessFlags(word, line, mode, p2);
         return kwCommandTime(result, word, tks, line, p2);
      }
      case Keyword::kw_Rename: {
         return c_rename(result, word, tks, line, mode, p2);
      }
      case Keyword::kw_Run: {
         checkUselessFlags(word, line, mode, p2);
         return c_run(result, word, tks, line, p2);
      }
      case Keyword::kw_Select: {
         checkUselessFlags(word, line, mode, p2);
         return c_select(result, word, tks, line, p2);
      }
      case Keyword::kw_Sleep: {
         checkUselessFlags(word, line, mode, p2);
         return c_sleep(result, word, tks, line, p2);
      }
      case Keyword::kw_Popup: {
         checkUselessFlags(word, line, mode, p2);
         return c_popup(result, word, tks, line, p2);
      }
      case Keyword::kw_Python: {
         throw SyntaxError(str(L"the command \"", word.origin, L"\" does not exist. You probably meant Python3"), line);
      }
      case Keyword::kw_Python3: {
         checkUselessFlags(word, line, mode, p2);
         return c_python3(result, word, tks, line, p2);
      }
      case Keyword::kw_Execute: {
         checkUselessFlags(word, line, mode, p2);
         return c_execute(result, word, tks, line, p2);
      }
   }

   throw SyntaxError(str(L"the command cannot start with a keyword \"", word.origin, L"\""), line);
   return false;
}

static void checkFileContextExistence(const p_str& commandName, const p_int line, Perun2Process& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"the subject of the command \"", commandName, L"\" is undefined here"), line);
   }
}

static p_bool parseLooped(const Tokens& tks, p_comptr& innerCommand, p_fcptr& ctx, p_comptr& result, Perun2Process& p2)
{
   p_genptr<p_str> str_;
   if (parse::parse(p2, tks, str_)) {
      result = std::make_unique<CS_StringComArg>(str_, innerCommand, ctx, p2);
      return true;
   }

   p_defptr def;
   if (parse::parse(p2, tks, def)) {
      result = std::make_unique<CS_DefinitionComArg>(def, innerCommand, ctx, p2);
      return true;
   }

   p_genptr<p_list> list;
   if (parse::parse(p2, tks, list)) {
      result = std::make_unique<CS_ListComArg>(list, innerCommand, ctx, p2);
      return true;
   }

   return false;
}

static void makeCoreCommandContext(p_fcptr& result, Perun2Process& p2)
{
   p_attrptr attr = std::make_unique<Attribute>();
   attr->setCoreCommandBase();
   result = std::make_unique<FileContext>(attr, p2);
}

static p_bool kwCommandSimple(p_comptr& result, const Token& word, Tokens& tks, const p_int line, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();
      return coreCommandSimple(result, word, ctx, true, p2);
   }

   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.closeAttributeScope();

   p_comptr inner;
   if (coreCommandSimple(inner, word, ctx.get(), false, p2) && parseLooped(tks, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool coreCommandSimple(p_comptr& result, const Token& word, FileContext* context, const p_bool saveChanges, Perun2Process& p2)
{
   switch (word.value.keyword) {
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

static p_bool kwCommandTime(p_comptr& result, const Token& word, Tokens& tks, const p_int line, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to\" is empty"), line);
   }

   if (!tks.check(TI_HAS_KEYWORD_TO)) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to\" does not contain a keyword \"to\""), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to\" does not contain its time argument"), line);
   }

   if (left.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setTimeCommandBase();
      p2.contexts.closeDeepAttributeScope();

      p_genptr<p_tim> tim;
      if (!parse::parse(p2, right, tim)) {
         throw SyntaxError(str(L"the time argument of the command \"", word.origin, L" to\" is invalid"), line);
      }

      if (coreCommandTime(result, word, ctx, tim, true, p2)) {
         return true;
      }

      commandSyntaxError(str(word.origin, L" to\""), line);
   }

   p_attrptr attr = std::make_unique<Attribute>();
   attr->setTimeCommandBase();
   p2.contexts.closeAttributeScope();
   p_fcptr ctx = std::make_unique<FileContext>(attr, p2);

   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_tim> tim;
   if (!parse::parse(p2, right, tim)) {
      throw SyntaxError(str(L"the time argument of the command \"", word.origin, L" to\" is invalid"), line);
   }

   p2.contexts.retreatFileContext();

   p_comptr inner;
   if (coreCommandTime(inner, word, ctx.get(), tim, false, p2) && parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool coreCommandTime(p_comptr& result, const Token& word, FileContext* context,
   p_genptr<p_tim>& time, const p_bool saveChanges, Perun2Process& p2)
{
   switch (word.value.keyword) {
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

static p_bool c_open(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();
      result = std::make_unique<C_Open>(true, ctx, p2);
      return true;
   }

   if (tks.check(TI_HAS_KEYWORD_WITH)) {
      P_DIVIDE_BY_KEYWORD(kw_With);

      if (right.isEmpty()) {
         throw SyntaxError(str(L"the command \"", word.origin,  L" with\" does not "
            L"contain its last argument"), line);
      }

      p_fcptr ctx;
      makeCoreCommandContext(ctx, p2);
      p2.contexts.addFileContext(ctx.get());

      p_genptr<p_str> prog;
      if (!parse::parse(p2, right, prog)) {
         throw SyntaxError(str(L"the last argument of the command \"", word.origin, L" with\" "
            L"cannot be resolved to a string"), line);
      }

      p2.contexts.retreatFileContext();

      if (left.isEmpty()) {
         checkFileContextExistence(str(word.origin, L" with"), line, p2);
         FileContext* ctx = p2.contexts.getFileContext();
         ctx->attribute->setCoreCommandBase();
         p2.contexts.closeDeepAttributeScope();
         result = std::make_unique<C_OpenWith>(prog, true, ctx, p2);
         return true;
      }
      else {
         p2.contexts.closeAttributeScope();
         p_comptr inner= std::make_unique<C_OpenWith>(prog, false, ctx.get(), p2);
         if (parseLooped(left, inner, ctx, result, p2)) {
            return true;
         }

         throw SyntaxError(str(L"wrong syntax of the command \"", word.origin, L" with\""), line);
      }
   }

   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.closeAttributeScope();

   p_comptr inner= std::make_unique<C_Open>(false, ctx.get(), p2);
   if (parseLooped(tks, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_select(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   if (p2.contexts.hasAggregate()) {
      Aggregate* aggr = p2.contexts.getAggregate();
      aggr->set(AGGR_SELECT);

      if (tks.isEmpty()) {
         FileContext* fc = p2.contexts.getFileContext();
         fc->attribute->setCoreCommandBase();
         fc->attribute->set(ATTR_PARENT);
         aggr->markIterationSelect(fc);
         result = std::make_unique<C_DoNothing>();
         return true;
      }

      p_genptr<p_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_AggrSelect_String>(aggr, str, p2);
         return true;
      }

      p_genptr<p_list> list;
      if (parse::parse(p2, tks, list)) {
         result = std::make_unique<C_AggrSelect_List>(aggr, list, p2);
         return true;
      }
   }
   else {
      if (tks.isEmpty()) {
         commandNoArgException(word.origin, line);
      }

      p_genptr<p_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_Select_String>(str, p2);
         return true;
      }
      
      p_defptr def;
      if (parse::parse(p2, tks, def)) {
         p_daptr action = std::make_unique<SelectDefAction>(p2);
         DefinitionAction& actionRef = *action.get();

         if (def->setAction(action)) {
            result = std::make_unique<C_SelectAsAction>(def, actionRef, p2);
            return true;
         }

         FileContext* ctx = def->getFileContext();
         if (ctx == nullptr) {
            result = std::make_unique<C_Select_Definition>(def, p2);
         }
         else {
            ctx->attribute->setCoreCommandBase();
            ctx->attribute->set(ATTR_PARENT);
            result = std::make_unique<C_Select_ContextDefinition>(def, p2, ctx);
         }

         return true;
      }

      p_genptr<p_list> list;
      if (parse::parse(p2, tks, list)) {
         result = std::make_unique<C_Select_List>(list, p2);
         return true;
      }
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_rename(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to\" is empty"), line);
   }

   if (!tks.check(TI_HAS_KEYWORD_TO)) {
      throw SyntaxError(str(L"the command \"", word.origin,  L" to\" does not contain a keyword \"to\""), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to\" ",
         L"does not contain a declaration of a new file name"), line);
   }

   p_bool extless = false;

   if (right.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      right.popLeft();
      if (right.isEmpty()) {
         throw SyntaxError(L"the keyword \"extensionless\" is not followed by a declaration of a new file name", line);
      }
   }

   if (left.isEmpty()) {
      checkFileContextExistence(str(word.origin, L" to"), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();
      //ctx->attribute->markToEvaluate();

      p_genptr<p_str> newName;
      if (!parse::parse(p2, right, newName)) {
         throw SyntaxError(str(L"the declaration of a new name in the command \"", word.origin, L" to\" is invalid"), line);
      }

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_RenameTo_Stack>(newName, true, extless, ctx, p2);
      }
      else {
         result = std::make_unique<C_RenameTo>(newName, true, mode == CoreCommandMode::ccm_Force, extless, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> newName;
   if (!parse::parse(p2, right, newName)) {
      throw SyntaxError(str(L"the declaration of a new name in the command \"", word.origin, L" to\" is invalid"), line);
   }

   p2.contexts.retreatFileContext();

   p_genptr<p_str> str_;
   if (parse::parse(p2, left, str_)) {
      p_comptr inner;

      if (mode == CoreCommandMode::ccm_Stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, ctx.get(), p2);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, mode == CoreCommandMode::ccm_Force, extless, ctx.get(), p2);
      }

      result = std::make_unique<CS_StringComArg>(str_, inner, ctx, p2);
      return true;
   }

   p_genptr<p_list> list;
   if (parse::parse(p2, left, list)) {
      p_comptr inner;

      if (mode == CoreCommandMode::ccm_Stack) {
         inner = std::make_unique<C_RenameTo_Stack>(newName, false, extless, ctx.get(), p2);
      }
      else {
         inner = std::make_unique<C_RenameTo>(newName, false, mode == CoreCommandMode::ccm_Force, extless, ctx.get(), p2);
      }

      result = std::make_unique<CS_ListComArg>(list, inner, ctx, p2);
      return true;
   }

   commandSyntaxError(str(word.origin, L" to"), line);
   return false;
}

static p_bool c_create(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_Create_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_Create>(mode == CoreCommandMode::ccm_Force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   p_genptr<p_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_Create_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_Create_String>(str_, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   p_genptr<p_list> list;
   if (parse::parse(p2, tks, list)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_Create_List_Stack>(list, p2);
      }
      else {
         result = std::make_unique<C_Create_List>(list, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}


static p_bool c_createFile(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateFile_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateFile>(mode == CoreCommandMode::ccm_Force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   p_genptr<p_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateFile_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateFile_String>(str_, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   throw SyntaxError(str(L"the argument of the command \"", word.origin, L"\" cannot be resolved to a string"), line);
}

static p_bool c_createDirectory(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateDirectory_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory>(mode == CoreCommandMode::ccm_Force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   p_genptr<p_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateDirectory_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory_String>(str_, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   throw SyntaxError(str(L"the argument of the command \"", word.origin, L"\" cannot be resolved to a string"), line);
}

static p_bool c_createFiles(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateFile_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateFile>(mode == CoreCommandMode::ccm_Force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   p_genptr<p_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateFile_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateFile_String>(str_, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   p_genptr<p_list> list;
   if (parse::parse(p2, tks, list)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateFiles_List_Stack>(list, p2);
      }
      else {
         result = std::make_unique<C_CreateFiles_List>(list, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_createDirectories(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      checkFileContextExistence(word.origin, line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();
      p2.contexts.closeDeepAttributeScope();

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateDirectory_Stack>(ctx, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory>(mode == CoreCommandMode::ccm_Force, ctx, p2);
      }

      return true;
   }

   p2.contexts.closeAttributeScope();

   p_genptr<p_str> str_;
   if (parse::parse(p2, tks, str_)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateDirectory_String_Stack>(str_, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectory_String>(str_, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   p_genptr<p_list> list;
   if (parse::parse(p2, tks, list)) {
      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CreateDirectories_List_Stack>(list, p2);
      }
      else {
         result = std::make_unique<C_CreateDirectories_List>(list, mode == CoreCommandMode::ccm_Force, p2);
      }

      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_moveTo(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to\" is empty"), line);
   }

   const p_bool hasTo = tks.check(TI_HAS_KEYWORD_TO);
   const p_bool hasAs = tks.check(TI_HAS_KEYWORD_AS);

   if (!hasTo) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L" to\" cannot be called without a keyword \"to\""), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (left.isEmpty()) {
      p2.contexts.closeDeepAttributeScope();

      return hasAs 
         ? c_moveToAsContextless(result, word, right, line, mode, p2)
         : c_moveToContextless(result, word, right, line, mode, p2);
   }

   p2.contexts.closeAttributeScope();

   return hasAs
      ? c_moveToAsContextfull(result, word, left, right, line, mode, p2)
      : c_moveToContextfull(result, word, left, right, line, mode, p2);
}

static p_bool c_moveToContextless(p_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   if (right.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L" to\" lacks a declaration of a new location"), line);
   }

   p_genptr<p_str> str_;
   if (!parse::parse(p2, right, str_)) {
      throw SyntaxError(str(L"new location in the command \"",
         word.origin, L" to\" cannot be resolved to a string"), line);
      return false;
   }
   
   checkFileContextExistence(str(word.origin, L" to"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   if (mode == CoreCommandMode::ccm_Stack) {
      result = std::make_unique<C_MoveTo_Stack>(str_, ctx, p2);
   }
   else {
      result = std::make_unique<C_MoveTo>(str_, mode == CoreCommandMode::ccm_Force, ctx, p2);
   }

   return true;
}

static p_bool c_moveToAsContextless(p_comptr& result, const Token& word, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
   Tokens& preAs = pair2.first;
   Tokens& postAs = pair2.second;

   if (preAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of new location written between "
         L"keywords \"to\" and \"as\""), line);
   }
   if (postAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of a new name written after keyword \"as\""), line);
   }

   p_bool extless = false;
   const Token& paf = postAs.first();

   if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      postAs.popLeft();
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"the keyword \"", paf.origin,
            L"\" is not followed by a declaration of a new file name"), line);
      }
   }

   p_genptr<p_str> nname;
   if (!parse::parse(p2, postAs, nname)) {
      throw SyntaxError(str(L"new name in the command \"", word.origin,
         L" to as\" cannot be resolved to a string"), line);
   }

   p_genptr<p_str> dest;
   if (!parse::parse(p2, preAs, dest)) {
      throw SyntaxError(str(L"new location in the command \"", word.origin,
         L" to\" cannot be resolved to a string"), line);
   }

   checkFileContextExistence(str(word.origin, L" to as"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   if (mode == CoreCommandMode::ccm_Stack) {
      result = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, ctx, p2);
   }
   else {
      result = std::make_unique<C_MoveToAs>(dest, nname, mode == CoreCommandMode::ccm_Force, extless, ctx, p2);
   }

   return true;
}

static p_bool c_moveToContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> dest;
   if (!parse::parse(p2, right, dest)) {
      throw SyntaxError(str(L"new location in the command \"", word.origin,
         L" to\" cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   p_comptr inner;
   if (mode == CoreCommandMode::ccm_Stack) {
      inner = std::make_unique<C_MoveTo_Stack>(dest, ctx.get(), p2);
   }
   else {
      inner = std::make_unique<C_MoveTo>(dest, mode == CoreCommandMode::ccm_Force, ctx.get(), p2);
   }

   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(str(word.origin, L" to"), line);
   return false;
}

static p_bool c_moveToAsContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   if (left.check(TI_HAS_KEYWORD_AS)) {
      throw SyntaxError(str(L"keywords \"to\" and \"as\" appear in the command \"",
         word.origin, L" to as\" in reverse order"), line);
   }

   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
   Tokens& preAs = pair2.first;
   Tokens& postAs = pair2.second;

   if (preAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of a new location written between "
         L"keywords \"to\" and \"as\""), line);
   }
   if (postAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of a new name written after keyword \"as\""), line);
   }

   p_bool extless = false;

   if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      postAs.popLeft();
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"the keyword \"", postAs.first().origin,
            L"\" is not followed by a declaration of a new file name"), line);
      }
   }

   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> nname;
   if (!parse::parse(p2, postAs, nname)) {
      throw SyntaxError(str(L"new name in the command \"", word.origin,
         L" to as\" cannot be resolved to a string"), line);
   }

   p_genptr<p_str> dest;
   if (!parse::parse(p2, preAs, dest)) {
      throw SyntaxError(str(L"new location in the command \"", word.origin,
         L" to\" cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();
   p_comptr inner;

   if (mode == CoreCommandMode::ccm_Stack) {
      inner = std::make_unique<C_MoveToAs_Stack>(dest, nname, extless, ctx.get(), p2);
   }
   else {
      inner = std::make_unique<C_MoveToAs>(dest, nname, mode == CoreCommandMode::ccm_Force, extless, ctx.get(), p2);
   }

   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(str(word.origin, L" to as"), line);
   return false;
}

static p_bool c_copy(p_comptr& result, const Token& word, const Tokens& tks, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   const p_bool hasTo = tks.check(TI_HAS_KEYWORD_TO);
   const p_bool hasAs = tks.check(TI_HAS_KEYWORD_AS);

   if (!hasTo) {
      if (hasAs) {
         throw SyntaxError(str(L"the command \"", word.origin,
            L" to as\" cannot be called without a keyword \"to\""), line);
      }

      if (mode == CoreCommandMode::ccm_Force) {
         throw SyntaxError(str(L"the command \"", word.origin,
            L"\" cannot be preceded by a flag \"forced\""), line);
      }

      if (mode == CoreCommandMode::ccm_Stack) {
         throw SyntaxError(str(L"the command \"", word.origin,
            L"\" cannot be preceded by a flag \"stack\""), line);
      }

      return c_copySimple(result, word, tks, line, p2);
   }

   P_DIVIDE_BY_KEYWORD(kw_To);

   if (left.isEmpty()) {
      p2.contexts.closeDeepAttributeScope();

      return hasAs 
         ? c_copyToAsContextless(result, word, right, line, mode, p2)
         : c_copyToContextless(result, word, right, line, mode, p2);
   }

   p2.contexts.closeAttributeScope();

   return hasAs
      ? c_copyToAsContextfull(result, word, left, right, line, mode, p2)
      : c_copyToContextfull(result, word, left, right, line, mode, p2);
}

static p_bool c_copySimple(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   if (p2.contexts.hasAggregate()) {
      Aggregate* aggr = p2.contexts.getAggregate();
      aggr->set(AGGR_COPY);

      if (tks.isEmpty()) {
         FileContext* fc = p2.contexts.getFileContext();
         fc->attribute->setCoreCommandBase();
         aggr->markIterationCopy(fc);
         result = std::make_unique<C_DoNothing>();
         return true;
      }

      p_genptr<p_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_AggrCopy_String>(aggr, str, p2);
         return true;
      }

      p_genptr<p_list> list;
      if (parse::parse(p2, tks, list)) {
         result = std::make_unique<C_AggrCopy_List>(aggr, list, p2);
         return true;
      }
   }
   else {
      if (tks.isEmpty()) {
         commandNoArgException(word.origin, line);
      }

      p_genptr<p_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_Copy_String>(str, p2);
         return true;
      }

      p_genptr<p_list> list;
      if (parse::parse(p2, tks, list)) {
         result = std::make_unique<C_Copy_List>(list, p2);
         return true;
      }
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_copyToContextless(p_comptr& result, const Token& word, const Tokens& right,
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   if (right.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L" to\" lacks a declaration of a new location"), line);
   }

   p_genptr<p_str> str_;
   if (parse::parse(p2, right, str_)) {
      checkFileContextExistence(str(word.origin, L" to"), line, p2);
      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();

      if (mode == CoreCommandMode::ccm_Stack) {
         result = std::make_unique<C_CopyTo_Stack>(str_, true, ctx, p2);
      }
      else {
         result = std::make_unique<C_CopyTo>(str_, true, mode == CoreCommandMode::ccm_Force, ctx, p2);
      }

      return true;
   }
   else {
      throw SyntaxError(str(L"new location in the command \"",
         word.origin, L" to\" cannot be resolved to a string"), line);
   }

   return false;
}

static p_bool c_copyToAsContextless(p_comptr& result, const Token& word, const Tokens& right,
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
   Tokens& preAs = pair2.first;
   Tokens& postAs = pair2.second;

   if (preAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of new location written between "
         L"keywords \"to\" and \"as\""), line);
   }
   if (postAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of new name written after the keyword \"as\""), line);
   }

   p_bool extless = false;

   if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      postAs.popLeft();
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"the keyword \"", postAs.first().origin,
            L"\" is not followed by a declaration of new file name"), line);
      }
   }

   p_genptr<p_str> nname;
   if (!parse::parse(p2, postAs, nname)) {
      throw SyntaxError(str(L"new name in the command \"", word.origin,
         L" to as\" cannot be resolved to a string"), line);
   }

   p_genptr<p_str> dest;
   if (!parse::parse(p2, preAs, dest)) {
      throw SyntaxError(str(L"new location in the command \"", word.origin,
         L" to\" cannot be resolved to a string"), line);
   }

   checkFileContextExistence(str(word.origin, L" to as"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   if (mode == CoreCommandMode::ccm_Stack) {
      result = std::make_unique<C_CopyToAs_Stack>(dest, nname, true, extless, ctx, p2);
   }
   else {
      result = std::make_unique<C_CopyToAs>(dest, nname, true, mode == CoreCommandMode::ccm_Force, extless, ctx, p2);
   }

   return true;
}

static p_bool c_copyToContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> dest;
   if (!parse::parse(p2, right, dest)) {
      throw SyntaxError(str(L"new location in the command \"", word.origin,
         L" to\" cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   p_comptr inner;
   if (mode == CoreCommandMode::ccm_Stack) {
      inner = std::make_unique<C_CopyTo_Stack>(dest, false, ctx.get(), p2);
   }
   else {
      inner = std::make_unique<C_CopyTo>(dest, false, mode == CoreCommandMode::ccm_Force, ctx.get(), p2);
   }

   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(str(word.origin, L" to"), line);
   return false;
}

static p_bool c_copyToAsContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, 
   const p_int line, const CoreCommandMode mode, Perun2Process& p2)
{
   if (left.check(TI_HAS_KEYWORD_AS)) {
      throw SyntaxError(str(L"keywords \"to\" and \"as\" appear in "
         L"the command \"", word.origin, L" to as\" in reverse order"), line);
   }

   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_As);
   Tokens& preAs = pair2.first;
   Tokens& postAs = pair2.second;

   if (preAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of new location written between "
         L"keywords \"to\" and \"as\""), line);
   }
   if (postAs.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" to as\" "
         L"does not contain a declaration of new name written after the keyword \"as\""), line);
   }

   p_bool extless = false;

   if (postAs.first().isKeyword(Keyword::kw_Extensionless)) {
      extless = true;
      postAs.popLeft();
      if (postAs.isEmpty()) {
         throw SyntaxError(str(L"the keyword \"", postAs.first().origin,
            L"\" is not followed by a declaration of new file name"), line);
      }
   }

   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> nname;
   if (!parse::parse(p2, postAs, nname)) {
      throw SyntaxError(str(L"new name in the command \"", word.origin,
         L" to as\" cannot be resolved to a string"), line);
   }

   p_genptr<p_str> dest;
   if (!parse::parse(p2, preAs, dest)) {
      throw SyntaxError(str(L"new location in the command \"", word.origin,
         L" to\" cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   p_comptr inner;
   if (mode == CoreCommandMode::ccm_Stack) {
      inner = std::make_unique<C_CopyToAs_Stack>(dest, nname, false, extless, ctx.get(), p2);
   }
   else {
      inner = std::make_unique<C_CopyToAs>(dest, nname, false, mode == CoreCommandMode::ccm_Force, extless, ctx.get(), p2);
   }

   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   commandSyntaxError(str(word.origin, L" to as"), line);
   return false;
}

void finalSyntaxError(const Tokens& tks, const Token& word, const p_int line, const p_bool directError, Perun2Process& p2)
{
   // all the possible expression parsing has failed
   // now, we have to show a Syntax Error message to the user

   const p_int length = tks.getLength();

   if (length > 1 && tks.first().isSymbol(CHAR_ASTERISK)) {
      if (length == 2) {
         const Token& second = tks.second();

         if (second.isFirstWord(EMPTY_STRING)) {
            throw SyntaxError::youShouldUseApostrophesAndWrite(
               str(CHAR_ASTERISK, CHAR_DOT, second.origin2), tks.first().line);
            return;
         }
      }

      throw SyntaxError::wrongSyntaxButProbablyAsteriskPattern(tks.first().line);
      return;
   }

   if (directError) {
      commandSyntaxError(word.origin, line);
   }
   else {
      throw SyntaxError::wrongSyntax(line);
   }
}

p_bool c_print(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, const p_bool directError, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      if (!p2.contexts.hasIterationContext()) {
         throw SyntaxError(str(L"the command \"", word.origin, L"\" needs an argument here. "
            L"The value of the variable \"this\" is undefined here"), line);
         return false;
      }

      FileContext* fc = p2.contexts.getFileContext();
      if (fc != nullptr) {
         result = std::make_unique<C_PrintThis>(p2, *fc);
         return true;
      }
      
      commandSyntaxError(word.origin, line);
   }

   p_genptr<p_str> str;
   if (parse::parse(p2, tks, str)) {
      result = std::make_unique<C_PrintSingle>(str, p2);
      return true;
   }

   p_defptr def;
   if (parse::parse(p2, tks, def)) {
      result = std::make_unique<C_PrintDefinition>(def, p2);
      return true;
   }

   p_genptr<p_list> list;
   if (parse::parse(p2, tks, list)) {
      result = std::make_unique<C_PrintList>(list, p2);
      return true;
   }

   finalSyntaxError(tks, word, line, directError, p2);
   return false;
}

static p_bool c_sleep(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   p_genptr<p_per> per;
   if (parse::parse(p2, tks, per)) {
      result = std::make_unique<C_SleepPeriod>(per, p2);
      return true;
   }

   p_genptr<p_num> num;
   if (parse::parse(p2, tks, num)) {
      result = std::make_unique<C_SleepMs>(num, p2);
      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_popup(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   p_genptr<p_str> str;
   if (parse::parse(p2, tks, str)) {
      result = std::make_unique<C_Popup>(str);
      return true;
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_error(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   if (tks.isEmpty()) {
      result = std::make_unique<C_Error>(p2);
      return true;
   }

   p_genptr<p_num> num;

   if (parse::parse(p2, tks, num)) {
      result = std::make_unique<C_ErrorWithExitCode>(num, p2);
      return true;
   }
   else {
      throw SyntaxError(str(L"the argument of the command \"", word.origin,
         L"\" cannot be resolved to a number"), line);
   }

   return false;
}

static p_bool c_run(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   p2.contexts.closeAttributeScope();

   if (!tks.check(TI_HAS_KEYWORD_WITH)) {
      p_genptr<p_str> str;
      if (parse::parse(p2, tks, str)) {
         result = std::make_unique<C_Run>(str, p2);
         return true;
      }
      else {
         commandSyntaxError(word.origin, line);
      }
   }

   P_DIVIDE_BY_KEYWORD(kw_With);

   if (right.isEmpty()) {
      throw SyntaxError(str(L"the right side of the command \"", word.origin, L" with\" is empty"), line);
   }

   if (left.isEmpty()) {
      return c_runContextless(result, word, right, line, p2);
   }
   else {
      return c_runContextfull(result, word, left, right, line, p2);
   }

   commandSyntaxError(word.origin, line);
   return false;
}

static p_bool c_runContextless(p_comptr& result, const Token& word, const Tokens& right, const p_int line, Perun2Process& p2)
{
   if (right.check(TI_HAS_KEYWORD_WITH)) {
      return c_runContextless_with(result, word, right, line, p2);
   }
   else {
      return c_runContextless_simple(result, word, right, line, p2);
   }
}

static p_bool c_runContextless_simple(p_comptr& result, const Token& word, const Tokens& right, const p_int line, Perun2Process& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L" with\" needs first argument here"), line);
   }

   checkFileContextExistence(str(word.origin, L" with"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   p_genptr<p_str> exec;
   if (parse::parse(p2, right, exec)) {
      if (right.getLength() == 1) {
         const Token& cf = right.first();
         if (cf.isWord(STRING_PERUN2)) {
            result = std::make_unique<C_RunWithPerun2>(ctx, p2);
            p2.postParseData.loadCmdPath();
            return true;
         }
      }
      result = std::make_unique<C_RunWith>(exec, ctx, p2);
      return true;
   }
   else {
      throw SyntaxError(str(L"last argument of the command \"",
         word.origin, L" with\" cannot be resolved to a string"), line);
   }

   return false;
}

static p_bool c_runContextless_with(p_comptr& result, const Token& word, const Tokens& right, const p_int line, Perun2Process& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"the command \"", word.origin, L" with with\" needs first argument here"), line);
   }

   checkFileContextExistence(str(word.origin, L" with with"), line, p2);
   FileContext* ctx = p2.contexts.getFileContext();
   ctx->attribute->setCoreCommandBase();

   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_With);
   Tokens& left2 = pair2.first;
   Tokens& right2 = pair2.second;

   if (left2.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L"\" cannot be called with adjacent \"with\" keywords"), line);
   }
   else if (right2.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L" with with\" cannot be called without its last argument"), line);
   }

   p_genptr<p_str> exec;
   if (!parse::parse(p2, left2, exec)) {
      throw SyntaxError(str(L"second argument of the command \"", word.origin,
         L" with with\" cannot be resolved to a string"), line);
   }

   if (left2.getLength() == 1) {
      const Token& cf = left2.first();
      if (cf.isWord(STRING_PERUN2)) {
         p_genptr<p_str> str_;

         if (parse::parse(p2, right2, str_)) {
            result = std::make_unique<C_RunWithPerun2WithString>(str_, ctx, p2);
            p2.postParseData.loadCmdPath();
            return true;
         }
         else {
            p_genptr<p_list> list;
            if (parse::parse(p2, right2, list)) {
               result = std::make_unique<C_RunWithPerun2With>(list, ctx, p2);
               p2.postParseData.loadCmdPath();
               return true;
            }
            else {
               throw SyntaxError(str(L"last argument of the command \"",
                  word.origin, L" with Perun2 with\" cannot be resolved to a list"), line);
            }
         }
      }
   }

   p_genptr<p_str> str_;
   if (parse::parse(p2, right2, str_)) {
      result = std::make_unique<C_RunWithWithString>(exec, str_, ctx, p2);
      return true;
   }
   else {
      p_genptr<p_list> list;
      if (parse::parse(p2, right2, list)) {
         result = std::make_unique<C_RunWithWith>(exec, list, ctx, p2);
         return true;
      }
      else {
         throw SyntaxError(str(L"last argument of the command \"",
            word.origin, L" with with\" cannot be resolved to a list"), line);
      }
   }
}

static p_bool c_runContextfull(p_comptr& result, const Token& word, const Tokens& left, const Tokens& right, const p_int line, Perun2Process& p2)
{
   if (right.check(TI_HAS_KEYWORD_WITH)) {
      return c_runContextfull_with(result, word, left, right, line, p2);
   }
   else {
      return c_runContextfull_simple(result, word, left, right, line, p2);
   }
}

static p_bool c_runContextfull_simple(p_comptr& result, const Token& word, const Tokens& left,
   const Tokens& right, const p_int line, Perun2Process& p2)
{
   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> exec;
   if (!parse::parse(p2, right, exec)) {
      throw SyntaxError(str(L"last argument of the command \"", word.origin,
         L" with\" cannot be resolved to a string"), line);
   }

   p2.contexts.retreatFileContext();

   if (right.getLength() == 1) {
      const Token& cf = right.first();
      if (cf.isWord(STRING_PERUN2)) {
         p_comptr inner= std::make_unique<C_RunWithPerun2>(ctx.get(), p2);
         if (parseLooped(left, inner, ctx, result, p2)) {
            p2.postParseData.loadCmdPath();
            return true;
         }

         throw SyntaxError(str(L"first argument of the command \"", word.origin,
            L" with Perun2\" cannot be resolved to a list"), line);
      }
   }

   p_comptr inner= std::make_unique<C_RunWith>(exec, ctx.get(), p2);
   if (parseLooped(left, inner, ctx, result, p2)) {
      return true;
   }

   throw SyntaxError(str(L"first argument of the command \"", word.origin,
      L" with\" cannot be resolved to a list"), line);
}

static p_bool c_runContextfull_with(p_comptr& result, const Token& word, const Tokens& left,
   const Tokens& right, const p_int line, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair2 = right.divideByKeyword(Keyword::kw_With);
   Tokens& left2 = pair2.first;
   Tokens& right2 = pair2.second;

   if (left2.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L"\" cannot be called with adjacent \"with\" keywords"), line);
   }
   else if (right2.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin,
         L" with with\" cannot be called without its last argument"), line);
   }

   p_fcptr ctx;
   makeCoreCommandContext(ctx, p2);
   p2.contexts.addFileContext(ctx.get());

   p_genptr<p_str> exec;
   if (!parse::parse(p2, left2, exec)) {
      throw SyntaxError(str(L"second argument of the command \"", word.origin,
         L" with with\" cannot be resolved to a string"), line);
   }

   p_genptr<p_str> lastStr;
   if (parse::parse(p2, right2, lastStr)) {
      p2.contexts.retreatFileContext();

      if (left2.getLength() == 1) {
         const Token& cf = left2.first();
         if (cf.isWord(STRING_PERUN2)) {
            p_comptr inner= std::make_unique<C_RunWithPerun2WithString>(lastStr, ctx.get(), p2);
            if (parseLooped(left, inner, ctx, result, p2)) {
               p2.postParseData.loadCmdPath();
               return true;
            }

            throw SyntaxError(str(L"first argument of the command \"", word.origin,
               L" with Perun2 with\" cannot be resolved to a list"), line);
         }
      }

      p_comptr inner= std::make_unique<C_RunWithWithString>(exec, lastStr, ctx.get(), p2);
      if (parseLooped(left, inner, ctx, result, p2)) {
         return true;
      }

      throw SyntaxError(str(L"first argument of the command \"", word.origin,
         L" with with\" cannot be resolved to a list"), line);
   }
   else {
      p_genptr<p_list> lastList;

      if (!parse::parse(p2, right2, lastList)) {
         p2.contexts.retreatFileContext();
         throw SyntaxError(str(L"last argument of the command \"", word.origin,
            L" with with\" cannot be resolved to a list"), line);
      }
      else {
         p2.contexts.retreatFileContext();

         if (left2.getLength() == 1) {
            const Token& cf = left2.first();
            if (cf.isWord(STRING_PERUN2)) {
               p_comptr inner= std::make_unique<C_RunWithPerun2With>(lastList, ctx.get(), p2);
               if (parseLooped(left, inner, ctx, result, p2)) {
                  p2.postParseData.loadCmdPath();
                  return true;
               }

               throw SyntaxError(str(L"first argument of the command \"", word.origin,
                  L" with Perun2 with\" cannot be resolved to a list"), line);
            }
         }

         p_comptr inner= std::make_unique<C_RunWithWith>(exec, lastList, ctx.get(), p2);
         if (parseLooped(left, inner, ctx, result, p2)) {
            return true;
         }

         throw SyntaxError(str(L"first argument of the command \"", word.origin,
            L" with with\" cannot be resolved to a list"), line);
      }
   }

   return false;
}

static p_str getPythonScriptName(p_genptr<p_str>& generator, const p_int line, const p_str& name) 
{
   if (! generator->isConstant()) {
      throw SyntaxError(str(L"the command \"", name, 
         L"\" needs a constant value as an argument"), line);
   }

   p_str value = generator->getValue();
   str_trim(value);

   if (value.empty()) {
      throw SyntaxError(str(L"the argument of the command \"", name, L"\" is empty"), line);
   }

   if (! os_isAbsolute(value)) {
      throw SyntaxError(str(L"the argument of the command \"", name, L"\" is not an absolute path"), line);
   }

   if (! os_fileExists(value)) {
      throw SyntaxError(str(L"the argument of the command \"", name, L"\" does not point to an existing file"), line);
   }

   return value;
}

static p_bool c_python3(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   p2.contexts.closeAttributeScope();

   if (tks.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, 
         L"\" needs an argument here"), line);
   }

   if (! tks.check(TI_HAS_KEYWORD_WITH)) {
      p_genptr<p_str> string;
      if (! parse::parse(p2, tks, string)) {
         throw SyntaxError(str(L"the argument of the command \"", word.origin, 
            L"\" cannot be resolved to a string"), line);
      }

      const p_str commandName = word.origin;
      const p_str scriptName = getPythonScriptName(string, line, commandName);

      std::unique_ptr<C_Python3> python3 = std::make_unique<C_Python3>(scriptName, p2);
      const p_str commandName2 = str(L"the command \"", word.origin, L"\"");
      python3->staticallyAnalyze(line, commandName2);

      result = std::move(python3);
      return true;
   }

   P_DIVIDE_BY_KEYWORD(kw_With);

   if (left.isEmpty()) {
      throw SyntaxError(str(L"the left side of the command \"", word.origin, L" with\" is empty"), line);
   }

   if (right.isEmpty()) {
      throw SyntaxError(str(L"the right side of the command \"", word.origin, L" with\" is empty"), line);
   }

   p_genptr<p_str> string;
   if (! parse::parse(p2, left, string)) {
      throw SyntaxError(str(L"the first argument of the command \"", word.origin, 
         L" with\" cannot be resolved to a string"), line);
   }

   const p_str commandName = str(word.origin, L" with");
   const p_str scriptName = getPythonScriptName(string, line, commandName);

   p_genptr<p_list> list;
   if (! parse::parse(p2, right, list)) {
      throw SyntaxError(str(L"the second argument of the command \"", word.origin, 
         L" with\" cannot be resolved to a list"), line);
   }

   std::unique_ptr<C_Python3With> python3With = std::make_unique<C_Python3With>(scriptName, list, p2);
   
   const p_str commandName2 = str(L"the command \"", word.origin, L" with\"");
   python3With->staticallyAnalyze(line, commandName2);
   result = std::move(python3With);
   return true;
}


static p_bool c_execute(p_comptr& result, const Token& word, const Tokens& tks, const p_int line, Perun2Process& p2)
{
   p2.contexts.closeAttributeScope();

   if (tks.isEmpty()) {
      throw SyntaxError(str(L"the command \"", word.origin, 
         L"\" needs an argument here"), line);
   }

   if (! tks.check(TI_HAS_KEYWORD_WITH)) {
      p_genptr<p_str> string;
      if (parse::parse(p2, tks, string)) {
         if (! string->isConstant()) {
            throw SyntaxError(str(L"the command \"", word.origin, 
               L"\" needs a constant value as an argument"), line);
         }

         p_str command = string->getValue();
         str_trim(command);

         if (command.empty()) {
            throw SyntaxError(str(L"the argument of the command \"", word.origin, 
               L"\" is empty"), line);
         }

         result = std::make_unique<C_Execute>(command, p2);
         return true;
      }

      if (tks.getLength() == 1 && tks.first().type == Token::t_Pattern) {
         throw SyntaxError(str(L"the argument of the command \"", word.origin, 
            L"\" contains asterisks. It should be written using backtick characters instead of regular quotes. For example: ",
            CHAR_BACKTICK, tks.first().origin, CHAR_BACKTICK
         ), line);
      }

      throw SyntaxError(str(L"the argument of the command \"", word.origin, 
         L"\" cannot be resolved to a string"), line);
   }

   P_DIVIDE_BY_KEYWORD(kw_With);

   if (left.isEmpty()) {
      throw SyntaxError(str(L"the left side of the command \"", word.origin, L" with\" is empty"), line);
   }

   if (right.isEmpty()) {
      throw SyntaxError(str(L"the right side of the command \"", word.origin, L" with\" is empty"), line);
   }

   p_genptr<p_str> string;
   if (! parse::parse(p2, left, string)) {
      if (left.getLength() == 1 && left.first().type == Token::t_Pattern) {
         throw SyntaxError(str(L"the argument of the command \"", word.origin, 
            L"\" contains asterisks. It should be written using backtick characters instead of regular quotes. For example: ",
            CHAR_BACKTICK, left.first().origin, CHAR_BACKTICK
         ), line);
      }

      throw SyntaxError(str(L"the first argument of the command \"", word.origin, 
         L" with\" cannot be resolved to a string"), line);
   }

   if (! string->isConstant()) {
      throw SyntaxError(str(L"the command \"", word.origin, 
         L" with\" needs a constant value as an argument"), line);
   }

   p_str command = string->getValue();
   str_trim(command);

   if (command.empty()) {
      throw SyntaxError(str(L"the argument of the command \"", word.origin, 
         L" with\" is empty"), line);
   }

   p_genptr<p_list> list;
   if (! parse::parse(p2, right, list)) {
      throw SyntaxError(str(L"the second argument of the command \"", word.origin, 
         L" with\" cannot be resolved to a list"), line);
   }

   result = std::make_unique<C_ExecuteWith>(command, list, p2);
   return true;
}



static void checkUselessFlags(const Token& word, const p_int line,
   const CoreCommandMode mode, Perun2Process& p2)
{
   switch (mode) {
      case CoreCommandMode::ccm_Force: {
         throw SyntaxError(str(L"the keyword \"", word.origin, L"\" cannot be preceded by a flag \"forced\""), line);
         break;
      }
      case CoreCommandMode::ccm_Stack: {
         throw SyntaxError(str(L"the keyword \"", word.origin, L"\" cannot be preceded by a flag \"stack\""), line);
         break;
      }
   }
}

static void commandSyntaxError(const p_str& name, const p_int line)
{
   throw SyntaxError(str(L"wrong syntax of the command \"", name, L"\""), line);
}

static void commandNoArgException(const p_str& name, const p_int line)
{
   throw SyntaxError(str(L"the command \"", name, L"\" requires an argument"), line);
}

}
