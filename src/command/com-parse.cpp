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

#include "com-parse.h"
#include "com-misc.h"
#include "../exception.h"
#include "com-struct.h"
#include "../datatype/parse-gen.h"
#include "../datatype/parse/parse-number.h"
#include "com-parse-kw.h"
#include "com-parse-unit.h"
#include "com-condition.h"
#include "../datatype/generator/gen-string.h"
#include "../datatype/parse/parse-number.h"


namespace perun2::comm
{

p_bool parseCommands(_comptr& result, const Tokens& tks, pp_perun2& p2)
{
   _ucptr context = std::make_unique<UserVarsContext>();
   p2.contexts.addUserVarsContext(context.get());

   std::vector<_comptr> commands;
   p_int sublen = 0;
   p_int depth = 0;
   p_int open = -1;
   const p_int end = tks.getEnd();

   for (p_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol)  {
         switch(t.value.ch) {
            case CHAR_SEMICOLON: {
               if (depth == 0) {
                  if (sublen != 0) {
                     p2.conditionContext.lockLast();
                     Tokens tks2(tks, i - sublen, sublen);
                     _comptr com;
                     command(com, tks2, p2);
                     commands.push_back(std::move(com));
                     sublen = 0;
                  }
               }
               else {
                  sublen++;
               }
               break;
            }
            case CHAR_OPENING_CURLY_BRACKET: {
               if (depth == 0) {
                  open = i;
               }
               depth++;
               sublen++;
               break;
            }
            case CHAR_CLOSING_CURLY_BRACKET: {
               depth--;
               if (depth == 0) {
                  _comptr com;

                  if (commandStruct(com, tks, sublen, i, open, p2)) {
                     commands.push_back(std::move(com));
                  }
                  sublen = 0;
               }
               else {
                  sublen++;
               }
               break;
            }
            default: {
               sublen++;
               break;
            }
         }
      }
      else  {
         sublen++;
      }
   }

   if (sublen != 0) {
      Tokens tks2(tks, 1 + end - sublen, sublen);
      _comptr com;
      command(com, tks2, p2);
      commands.push_back(std::move(com));
   }

   result = std::make_unique<CS_RawBlock>(commands, context, p2);
   p2.contexts.retreatUserVarsContext();
   return true;
}

void checkKeywordsBeforeCurlyBrackets(const Tokens& tks, pp_perun2& p2)
{
   const p_int end = tks.getEnd();

   for (p_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (t.type == Token::t_Keyword && t.isExpForbiddenKeyword()) {
         throw SyntaxError(str(L"a command-ending semicolon ; was expected somewhere between keyword '",
            t.getOriginString(p2), L"' and curly brackets {}"), t.line);
      }
   }
}


static p_bool commandStruct(_comptr& result, const Tokens& tks, const p_int sublen,
   const p_int index, const p_int open, pp_perun2& p2)
{
   const p_int leftStart = index - sublen;
   const p_int leftLen = open - leftStart;
   const p_int rightStart = open + 1;
   const p_int rightLen = index - rightStart;

   // build a simple block of commands
   if (leftLen == 0) {
      if (rightLen == 0) {
         return false;
      }

      _acptr context = std::make_unique<AggregateContext>(p2);
      p2.contexts.addAggregateContext(context.get());
      Tokens right(tks, rightStart, rightLen);
      const p_bool b = parseCommandsAsMember(result, right, nullptr, p2);
      p2.contexts.retreatAggregateContext();
      return b;
   }

   Tokens left(tks, leftStart, leftLen);
   checkKeywordsBeforeCurlyBrackets(left, p2);

   // build "times"
   const Token& leftLast = left.last();
   if (leftLast.isKeyword(Keyword::kw_Times)) {
      left.trimRight();
      if (left.isEmpty()) {
         throw SyntaxError(str(L"keyword '", leftLast.getOriginString(p2),
            L"' is not preceded by a number"), leftLast.line);
      }

      if (rightLen == 0) {
         return false;
      }

      left.checkCommonExpressionExceptions(p2);
      _genptr<p_num> num;
      if (!parse::parse(p2, left, num)) {
         throw SyntaxError(str(L"keyword '", leftLast.getOriginString(p2),
            L"' is not preceded by a valid number"), leftLast.line);
      }

      Tokens right(tks, rightStart, rightLen);

      _icptr context = std::make_unique<IndexContext>(p2);
      p2.contexts.addIndexContext(context.get());
      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
      p2.contexts.retreatIndexContext();

      if (success) {
         result = std::make_unique<CS_Times>(num, com, context, p2);
      }

      return success;
   }

   checkNoSemicolonBeforeBrackets(left, p2);

   // build "while"
   const Token& leftFirst = left.first();
   if (leftFirst.isKeyword(Keyword::kw_While)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(p2),
            L"' is not followed by a condition"), leftFirst.line);
      }

      if (rightLen == 0) {
         throw SyntaxError(str(L"structure '", leftFirst.getOriginString(p2),
            L"' is empty. It would either never run or cause an infinite loop"), leftFirst.line);
      }

      left.checkCommonExpressionExceptions(p2);
      _genptr<p_bool> boo;
      if (!parse::parse(p2, left, boo)) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(p2),
            L"' is not followed by a valid condition"), leftFirst.line);
      }

      Tokens right(tks, rightStart, rightLen);

      _icptr context = std::make_unique<IndexContext>(p2);
      p2.contexts.addIndexContext(context.get());
      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
      p2.contexts.retreatIndexContext();

      if (success) {
         result = std::make_unique<CS_While>(boo, com, context, p2);
      }

      return success;
   }

   // build "inside"
   if (leftFirst.isKeyword(Keyword::kw_Inside)) {
      if (rightLen == 0) {
         return false;
      }

      left.trimLeft();
      Tokens right(tks, rightStart, rightLen);
      return parseInsideLoop(result, leftFirst, left, right, p2);
   }

   // build "if"
   if (leftFirst.isKeyword(Keyword::kw_If)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(p2), L"' is not followed by a condition"),
            leftFirst.line);
      }

      left.checkCommonExpressionExceptions(p2);
      _genptr<p_bool> boo;
      if (!parse::parse(p2, left, boo)) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(p2), L"' is not followed by a valid condition"),
            leftFirst.line);
      }

      std::unique_ptr<CS_Condition> cond = std::make_unique<CS_Condition>();
      _comptr* inter = cond->getCommandPtr();

      if (rightLen == 0) {
         p2.conditionContext.addClosed(inter);
         p2.conditionContext.setMain(boo);
         result = std::move(cond);
         return true;
      }

      Tokens right(tks, rightStart, rightLen);

      _comptr com;
      if (parseCommandsAsMember(com, right, inter, p2)) {
         p2.conditionContext.setMain(com, boo);
      }

      result = std::move(cond);
      return true;
   }

   if (leftFirst.isKeyword(Keyword::kw_Else)) {
      left.trimLeft();

      if (left.isEmpty()) { // build "else"
         if (rightLen == 0) {
            p2.conditionContext.addEmptyElse(leftFirst.line);
            return false;
         }

         Tokens right(tks, rightStart, rightLen);

         _comptr com;
         const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
         p2.conditionContext.deleteLast();

         if (success) {
            p2.conditionContext.addElse(com, leftFirst.line);
         }
      }
      else { // build "else if"
         if (!left.first().isKeyword(Keyword::kw_If)) {
            throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(p2),
               L"' cannot be followed by an expression"), leftFirst.line);
         }

         const Token& ifToken = left.first();
         left.trimLeft();

         if (left.isEmpty()) {
            throw SyntaxError(str(L"keywords '", leftFirst.getOriginString(p2), L" ",
               ifToken.getOriginString(p2), L"' are not followed by a condition"), leftFirst.line);
         }

         left.checkCommonExpressionExceptions(p2);
         _genptr<p_bool> boo;
         if (!parse::parse(p2, left, boo)) {
            throw SyntaxError(str(L"keywords '", leftFirst.getOriginString(p2), L" ",
               ifToken.getOriginString(p2), L"' are not followed by a valid condition"), leftFirst.line);
         }

         if (rightLen == 0) {
            _comptr com = std::make_unique<C_DoNothing>();
            p2.conditionContext.addElseIf(boo, com, leftFirst.line);
            return false;
         }

         Tokens right(tks, rightStart, rightLen);
         _comptr com;
         const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
         p2.conditionContext.deleteLast();

         if (success) {
            p2.conditionContext.addElseIf(boo, com, leftFirst.line);
         }
      }

      return false;
   }

   if (rightLen == 0) {
      return false;
   }

   left.checkCommonExpressionExceptions(p2);
   Tokens right(tks, rightStart, rightLen);
   return parseIterationLoop(result, left, right, p2);
}

static p_bool parseIterationLoop(_comptr& result, const Tokens& left, const Tokens& right, pp_perun2& p2)
{
   // string loop
   _genptr<p_str> str_;
   if (parse::parse(p2, left, str_)) {
      _fcptr context = std::make_unique<FileContext>(p2);
      p2.contexts.addFileContext(context.get());
      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
      p2.contexts.retreatFileContext();

      if (success) {
         result = std::make_unique<CS_StringLoop>(str_, com, context, p2);
      }

      return success;
   }

   // definition loop
   p_defptr def;
   if (parse::parse(p2, left, def)) {
      FileContext* fc = def->getFileContext();

      if (fc != nullptr) {
         if (right.check(TI_EVALUATE_DEFINITIONS)) {
            _fcptr nextFc = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(nextFc.get());
            _comptr com;
            const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
            p2.contexts.retreatFileContext();

            if (success) {
               _genptr<p_list> g = std::make_unique<gen::Cast_D_L>(def, p2);
               result = std::make_unique<CS_ListLoop>(g, com, nextFc, p2);
            }

            return success;
         }
         else {
            p2.contexts.addFileContext(fc);
            _comptr com;
            const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
            p2.contexts.retreatFileContext();

            if (success) {
               result = std::make_unique<CS_ContextlessLoop>(def, com, p2);
            }

            return success;
         }
      }

      _fcptr ctx = std::make_unique<FileContext>(p2);
      p2.contexts.addFileContext(ctx.get());
      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
      p2.contexts.retreatFileContext();

      if (success) {
         if (right.check(TI_EVALUATE_DEFINITIONS)) {
            _genptr<p_list> g = std::make_unique<gen::Cast_D_L>(def, p2);
            result = std::make_unique<CS_ListLoop>(g, com, ctx, p2);
         }
         else {
            result = std::make_unique<CS_DefinitionLoop>(def, com, ctx, p2);
         }
      }

      return success;
   }

   // list loop
   _genptr<p_list> list;
   if (parse::parse(p2, left, list)) {
      _fcptr context = std::make_unique<FileContext>(p2);
      p2.contexts.addFileContext(context.get());
      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);
      p2.contexts.retreatFileContext();

      if (success) {
         result = std::make_unique<CS_ListLoop>(list, com, context, p2);
      }

      return success;
   }

   throw SyntaxError(L"tokens before { bracket do not form any valid syntax structure", left.first().line);
}

static p_bool parseInsideLoop(_comptr& result, const Token& keyword, const Tokens& left, const Tokens& right, pp_perun2& p2)
{
   // inside { }
   if (left.isEmpty()) {
      if (!p2.contexts.hasFileContext()) {
         throw SyntaxError(str(L"argumentless structure '", keyword.getOriginString(p2),
            L"' can be declared only within an iteration loop"), keyword.line);
      }

      FileContext* fc = p2.contexts.getFileContext();

      if (fc->isInside) {
         throw SyntaxError(str(L"argumentless structure '", keyword.getOriginString(p2),
            L"' is forbidden here. We already operate inside a directory, so it cannot be visited again"), keyword.line);
      }

      fc->attribute->setCoreCommandBase();
      const p_bool prevInside = fc->isInside;
      fc->isInside = true;

      _lcptr locContext;
      p2.contexts.makeLocationContext(locContext);
      p2.contexts.addLocationContext(locContext.get());

      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);

      p2.contexts.retreatLocationContext();
      fc->isInside = prevInside;

      if (success) {
         result = std::make_unique<CS_InsideThis>(com, locContext, fc, p2);
      }

      return success;
   }

   // inside string { }
   _genptr<p_str> str_;
   if (parse::parse(p2, left, str_)) {
      _fcptr context = std::make_unique<FileContext>(p2);
      context->attribute->setCoreCommandBase();
      context->isInside = true;
      p2.contexts.addFileContext(context.get());

      _lcptr locContext;
      p2.contexts.makeLocationContext(locContext);
      p2.contexts.addLocationContext(locContext.get());

      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);

      p2.contexts.retreatFileContext();
      p2.contexts.retreatLocationContext();

      if (success) {
         result = std::make_unique<CS_InsideString>(str_, com, locContext, context, p2);
      }

      return success;
   }

   // inside definition { }
   p_defptr def;
   if (parse::parse(p2, left, def)) {
      FileContext* fc = def->getFileContext();

      if (fc != nullptr) {
         if (right.check(TI_EVALUATE_DEFINITIONS)) {
            _fcptr nextFc = std::make_unique<FileContext>(p2);
            nextFc->attribute->setCoreCommandBase();
            nextFc->isInside = true;
            p2.contexts.addFileContext(nextFc.get());

            _lcptr locContext;
            p2.contexts.makeLocationContext(locContext);
            p2.contexts.addLocationContext(locContext.get());

            _comptr com;
            const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);

            p2.contexts.retreatFileContext();
            p2.contexts.retreatLocationContext();

            if (success) {
               _genptr<p_list> g = std::make_unique<gen::Cast_D_L>(def, p2);
               result = std::make_unique<CS_InsideList>(g, com, locContext, nextFc, p2);
            }

            return success;
         }
         else {
            fc->attribute->setCoreCommandBase();
            fc->isInside = true;
            p2.contexts.addFileContext(fc);

            _lcptr locContext;
            p2.contexts.makeLocationContext(locContext);
            p2.contexts.addLocationContext(locContext.get());

            _comptr com;
            const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);

            p2.contexts.retreatFileContext();
            p2.contexts.retreatLocationContext();

            if (success) {
               result = std::make_unique<CS_InsideContextless>(def, com, locContext, p2);
            }

            return success;
         }
      }

      _fcptr ctx = std::make_unique<FileContext>(p2);
      ctx->attribute->setCoreCommandBase();
      ctx->isInside = true;
      p2.contexts.addFileContext(ctx.get());

      _lcptr locContext;
      p2.contexts.makeLocationContext(locContext);
      p2.contexts.addLocationContext(locContext.get());

      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);

      p2.contexts.retreatFileContext();
      p2.contexts.retreatLocationContext();

      if (success) {
         if (right.check(TI_EVALUATE_DEFINITIONS)) {
            _genptr<p_list> g = std::make_unique<gen::Cast_D_L>(def, p2);
            result = std::make_unique<CS_InsideList>(g, com, locContext, ctx, p2);
         }
         else {
            result = std::make_unique<CS_InsideDefinition>(def, com, locContext, ctx, p2);
         }
      }

      return success;
   }

   // inside list { }
   _genptr<p_list> list;
   if (parse::parse(p2, left, list)) {
      _fcptr context = std::make_unique<FileContext>(p2);
      context->attribute->setCoreCommandBase();
      context->isInside = true;
      p2.contexts.addFileContext(context.get());

      _lcptr locContext;
      p2.contexts.makeLocationContext(locContext);
      p2.contexts.addLocationContext(locContext.get());

      _comptr com;
      const p_bool success = parseCommandsAsMember(com, right, nullptr, p2);

      p2.contexts.retreatFileContext();
      p2.contexts.retreatLocationContext();

      if (success) {
         result = std::make_unique<CS_InsideList>(list, com, locContext, context, p2);
      }

      return success;
   }

   throw SyntaxError(L"tokens before { bracket do not form any valid syntax structure", left.first().line);
}

static p_bool parseCommandsAsMember(_comptr& result, const Tokens& tks, _comptr* cond, pp_perun2& p2)
{
   p2.conditionContext.add(cond);
   const p_bool success = parseCommands(result, tks, p2);
   p2.conditionContext.deleteClosedUnits();
   return success;
}

static p_bool command(_comptr& result, Tokens& tks, pp_perun2& p2)
{
   const Token& f = tks.first();

   if (tks.getLength() == 1 && f.type == Token::t_Keyword) {
      switch (f.value.keyword.k) {
         case Keyword::kw_Break:
         case Keyword::kw_Continue: {
            if (p2.contexts.hasIndexContext()) {
               if (f.value.keyword.k == Keyword::kw_Break) {
                  result = std::make_unique<C_Break>(p2);
                  return true;
               }
               else {
                  result = std::make_unique<C_Continue>(p2);
                  return true;
               }
            }
            else {
               throw SyntaxError(str(L"command '", f.getOriginString(p2),
                  L"' can be called only inside a loop"), f.line);
            }
            break;
         }
         case Keyword::kw_Exit: {
            result = std::make_unique<C_Exit>(p2);
            return true;
         }
      }
   }

   p_bool force = false;
   p_bool stack = false;

   const Token& f2 = tks.first();
   if (f2.type == Token::t_Keyword) {
      switch (f2.value.keyword.k) {
         case Keyword::kw_Force: {
            tks.trimLeft();
            force = true;

            if (tks.isEmpty()) {
               throw SyntaxError(str(L"command cannot consist of only one keyword: '",
                  f2.getOriginString(p2),L"'"), f.line);
            }
            break;
         }
         case Keyword::kw_Stack: {
            tks.trimLeft();
            stack = true;

            if (tks.isEmpty()) {
               throw SyntaxError(str(L"command cannot consist of only one keyword: '",
                  f2.getOriginString(p2),L"'"), f.line);
            }
            break;
         }
      }
   }

   const Token& f3 = tks.first();
   if (f3.type == Token::t_Keyword) {
      switch (f3.value.keyword.k) {
         case Keyword::kw_Not:
         case Keyword::kw_True:
         case Keyword::kw_False: {
            break;
         }
         default: {
            tks.trimLeft();
            tks.checkCommonExpressionExceptions(p2);
            return keywordCommands(result, f3, tks, f.line, force, stack, p2);
         }
      }
   }

   if (force) {
      throw SyntaxError(L"only a core command can start with a keyword 'force'", f.line);
   }
   if (stack) {
      throw SyntaxError(L"only a core command can start with a keyword 'stack'", f.line);
   }

   if (commandMisc(result, tks, p2)) {
      return true;
   }
   else {
      tks.checkCommonExpressionExceptions(p2);
      return c_print(result, Token(Keyword::kw_Print, f.line, static_cast<p_size>(0),
         static_cast<p_size>(0), p2), tks, f.line, false, p2);
   }
}

static p_bool commandMisc(_comptr& result, const Tokens& tks, pp_perun2& p2)
{
   if (tks.check(TI_HAS_CHAR_EQUALS)) {
      std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_EQUAL_SIGN);
      Tokens& left = pair.first;
      Tokens& right = pair.second;

      if (left.isEmpty()) {
         if (right.isEmpty()) {
            throw SyntaxError(
               L"left and right side of the = operator are empty",
               tks.first().line);
         }
         else {
            throw SyntaxError(L"left side of the = operator is empty",
               tks.first().line);
         }
      }
      if (right.isEmpty()) {
         throw SyntaxError(L"right side of the = operator is empty",
            tks.last().line);
      }

      if (right.first().isSymbol(CHAR_EQUAL_SIGN) || left.check(TI_HAS_FILTER_KEYWORD)) {
         return false;
      }

      right.checkCommonExpressionExceptions(p2);

      const Token& leftLast = left.last();
      if (leftLast.type == Token::t_Symbol) {
         const char& ch = leftLast.value.ch;
         switch (ch) {
            case CHAR_PLUS:
            case CHAR_MINUS:
            case CHAR_ASTERISK:
            case CHAR_SLASH:
            case CHAR_PERCENT: {
               left.trimRight();
               if (left.isEmpty()) {
                  throw SyntaxError(str(L"left side of the ", toStr(ch),
                     L"= operator is empty"), tks.last().line);
               }

               return commandVarChange(result, left, right, ch, p2);
            }
         }
      }

      if (varSquareBrackets(left)) {
         return commandVarAssign_Element(result, left, right, p2);
      }
      else {
         return commandVarAssign(result, left, right, p2);
      }
   }

   const Token& last = tks.last();

   if (last.type == Token::t_MultiSymbol &&
       (last.value.chars.ch == CHAR_PLUS || last.value.chars.ch == CHAR_MINUS))
   {
      const p_bool isIncrement = last.value.chars.ch == CHAR_PLUS;
      const p_str op = isIncrement ? L"incremented by one" : L"decremented by one";
      const Token& first = tks.first();

      if (first.type == Token::t_Word) {
         if (tks.getLength() == 2) {
            Variable<p_num>* pvp_num;

            if (!p2.contexts.getVar(first, pvp_num, p2) || pvp_num->isImmutable()) {
               throw SyntaxError(str(L"variable '", first.getOriginString(p2),
                  L"' cannot be ", op), first.line);
            }

            pvp_num->makeNotConstant();

            if (isIncrement) {
               result = std::make_unique<VarIncrement>(*pvp_num);
            }
            else {
               result = std::make_unique<VarDecrement>(*pvp_num);
            }

            return true;
         }
         else {
            Tokens tks2(tks);
            tks2.trimRight();
            tks2.trimRight();

            if (varSquareBrackets(tks2)) {
               _genptr<p_num> index;
               parseListElementIndex(index, tks2, p2);
               Variable<p_nlist>* pvp_nlist;

               if (!p2.contexts.getVar(first, pvp_nlist, p2) || pvp_nlist->isImmutable()) {
                  throw SyntaxError(str(L"variable '", first.getOriginString(p2),
                     L"' cannot be ", op), first.line);
               }

               throw SyntaxError(str(L"an element of variable '", first.getOriginString(p2),
                  L"' cannot be ", op, L", because collections in Perun2 are immutable"),
                  first.line);
            }
            else {
               throw SyntaxError(str(L"this structure cannot be ", op), first.line);
            }
         }
      }
      else if (first.type == Token::t_TwoWords && tks.getLength() == 2) {
         if (first.isFirstWord(EMPTY_STRING, p2)) {
            throw SyntaxError(L"the dot . should be preceded by a time variable name", first.line);
         } 

         Variable<p_tim>* pvp_tim;

         if (!p2.contexts.getVar(first, pvp_tim, p2)) {
            throw SyntaxError(str(L"time variable from expression '", first.getOriginString(p2),
               L"' does not exist or is unreachable here"), first.line);
         }

         if (pvp_tim->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
         }

         Period::PeriodUnit unit;

         if (first.isSecondWord(STRING_YEAR, p2) || first.isSecondWord(STRING_YEARS, p2))
            unit = Period::u_Years;
         else if (first.isSecondWord(STRING_MONTH, p2) || first.isSecondWord(STRING_MONTHS, p2))
            unit = Period::u_Months;
         else if (first.isSecondWord(STRING_DAY, p2) || first.isSecondWord(STRING_DAYS, p2))
            unit = Period::u_Days;
         else if (first.isSecondWord(STRING_HOUR, p2) || first.isSecondWord(STRING_HOURS, p2))
            unit = Period::u_Hours;
         else if (first.isSecondWord(STRING_MINUTE, p2) || first.isSecondWord(STRING_MINUTES, p2))
            unit = Period::u_Minutes;
         else if (first.isSecondWord(STRING_SECOND, p2) || first.isSecondWord(STRING_SECONDS, p2))
            unit = Period::u_Seconds;
         else if (first.isSecondWord(STRING_DATE, p2) || first.isSecondWord(STRING_WEEKDAY, p2)) {
            throw SyntaxError(str(L"time variable member '", first.getOriginString_2(p2),
               L"' cannot be ", op), first.line);
         }
         else {
            parse::timeVariableMemberException(first, p2);
         }

         pvp_tim->makeNotConstant();

         if (isIncrement) {
            result = std::make_unique<VarTimeUnitIncrement>(*pvp_tim, unit);
         }
         else {
            result = std::make_unique<VarTimeUnitDecrement>(*pvp_tim, unit);
         }

         return true;
      }
      else {
         throw SyntaxError(str(L"only a variable of a singular data type can be ", op), first.line);
      }
   }

   return false;
}

static p_bool commandVarChange(_comptr& result, const Tokens& left, const Tokens& right, const p_char sign, pp_perun2& p2)
{
   const Token& first = left.first();

   if (left.getLength() > 1) {
      if (varSquareBrackets(left)) {
         Variable<p_list>* pvp_list;
         Variable<p_nlist>* pvp_nlist;
         Variable<p_tlist>* pvp_tlist;

         if (p2.contexts.getVar(first, pvp_list, p2)
          || p2.contexts.getVar(first, pvp_nlist, p2)
          || p2.contexts.getVar(first, pvp_tlist, p2))
         {
            throw SyntaxError(str(L"collection variable '", first.getOriginString(p2),
               L"' is immutable, so its elements cannot be modified"), right.first().line);
         }

         Variable<p_str>* pvp_str;

         if (p2.contexts.getVar(first, pvp_str, p2)) {
            throw SyntaxError(str(L"operation ", toStr(sign),
               L"= cannot be performed on a character from string variable"), first.line);
         }

         throw SyntaxError(str(first.getOriginString(p2),
            L" is not a collection variable, which are expected before [] brackets"), first.line);
      }
      else {
         const Token& arom = left.last();

         if (arom.type == Token::t_TwoWords && arom.isFirstWord(EMPTY_STRING, p2)) {
            Tokens aro(left);
            aro.trimRight();

            if (varSquareBrackets(aro)) {
               Variable<p_tlist>* pvp_tlist;
               if (p2.contexts.getVar(first, pvp_tlist, p2)) {
                  throw SyntaxError(str(L"operation ", toStr(sign),
                     L"= cannot be performed on a time list variable member. Collections in Perun2 are immutable"), first.line);
               }
            }
         }

         throw SyntaxError(str(L"operator ", toStr(sign),
            L"= should be preceded by a variable name"), first.line);
      }
   }

   if (first.type == Token::t_Word) {
      Variable<p_num>* pvp_num;
      if (p2.contexts.getVar(first, pvp_num, p2)) {
         _genptr<p_num> num;

         if (!parse::parse(p2, right, num)) {
            throw SyntaxError(str(L"right side of operator ", toStr(sign),
               L"= cannot be resolved to a number"), first.line);
         }

         if (pvp_num->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
         }

         Variable<p_num>& var = *pvp_num;
         pvp_num->makeNotConstant();

         switch (sign) {
            case CHAR_PLUS: {
               result = std::make_unique<VarAdd_<p_num>>(var, num);
               break;
            }
            case CHAR_MINUS: {
               result = std::make_unique<VarSubtract<p_num>>(var, num);
               break;
            }
            case CHAR_ASTERISK: {
               result = std::make_unique<VarNumMultiply>(var, num);
               break;
            }
            case CHAR_SLASH: {
               result = std::make_unique<VarNumDivide>(var, num);
               break;
            }
            case CHAR_PERCENT: {
               result = std::make_unique<VarModulo>(var, num);
               break;
            }
         }

         return true;
      }

      Variable<p_per>* pvp_per;
      if (p2.contexts.getVar(first, pvp_per, p2)) {
         switch (sign) {
            case CHAR_PLUS:
            case CHAR_MINUS: {
               _genptr<p_per> per;

               if (!parse::parse(p2, right, per)) {
                  throw SyntaxError(str(L"right side of operator ", toStr(sign),
                     L"= cannot be resolved to a period"), first.line);
               }

               if (pvp_per->isImmutable()) {
                  throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
               }

               pvp_per->makeNotConstant();
               Variable<p_per>& var = *pvp_per;

               if (sign == CHAR_PLUS) {
                  result = std::make_unique<VarAdd_<p_per>>(var, per);
               }
               else {
                  result = std::make_unique<VarSubtract<p_per>>(var, per);
               }

               return true;
            }
            case CHAR_ASTERISK:
            case CHAR_SLASH:
            case CHAR_PERCENT: {
               throw SyntaxError(str(L"operation ",  toStr(sign),
                  L"= cannot be performed on a period"), first.line);
            }
         }
      }

      Variable<p_tim>* pvp_tim;
      if (p2.contexts.getVar(first, pvp_tim, p2)) {
         switch (sign) {
            case CHAR_ASTERISK:
            case CHAR_SLASH:
            case CHAR_PERCENT: {
               throw SyntaxError(str(L"operation ", toStr(sign),
                  L"= is not valid for a time variable"), first.line);
            }
         }

         _genptr<p_per> per;

         if (!parse::parse(p2, right, per)) {
            throw SyntaxError(str(L"right side of operator '", first.getOriginString(p2),
               L" ", toStr(sign), L"=' cannot be resolved to a period"), first.line);
         }

         Variable<p_tim>& var = *pvp_tim;
         pvp_tim->makeNotConstant();

         if (pvp_tim->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
         }

         if (sign == CHAR_PLUS) {
            result = std::make_unique<VarTimeAdd>(var, per);
         }
         else {
            result = std::make_unique<VarTimeSubtract>(var, per);
         }

         return true;
      }

      if (sign == CHAR_PLUS) {
         return commandVarIncrement(result, first, right, first.line, p2);
      }

      throw SyntaxError(str(L"'", first.getOriginString(p2),
         L"' is neither a numeric, time, nor period variable"), first.line);
   }
   else if (first.type == Token::t_TwoWords) {
      switch (sign) {
         case CHAR_ASTERISK:
         case CHAR_SLASH:
         case CHAR_PERCENT: {
            throw SyntaxError(str(L"operation ", toStr(sign),
               L"= is not valid for a time variable"), first.line);
         }
      }

      Variable<p_tim>* pvp_tim;
      if (!p2.contexts.getVar(first, pvp_tim, p2)) {
         throw SyntaxError(str(L"'", first.getOriginString(p2),
            L"' is not a time variable for the ", toStr(sign), L"= operation"),
            first.line);
      }

      if (pvp_tim->isImmutable()) {
         throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
      }

      _genptr<p_num> num;

      if (!parse::parse(p2, right, num)) {
         throw SyntaxError(str(L"right side of operation '", first.getOriginString(p2), L".",
            first.getOriginString_2(p2), L" ", toStr(sign),
            L"=' cannot be resolved to a number"), first.line);
      }

      Variable<p_tim>& var = *pvp_tim;
      const p_bool negative = (sign == CHAR_MINUS);
      pvp_tim->makeNotConstant();

      if (first.isSecondWord(STRING_YEAR, p2) || first.isSecondWord(STRING_YEARS, p2)) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Years, negative);
         return true;
      }
      else if (first.isSecondWord(STRING_MONTH, p2) || first.isSecondWord(STRING_MONTHS, p2)) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Months, negative);
         return true;
      }
      else if (first.isSecondWord(STRING_DAY, p2) || first.isSecondWord(STRING_DAYS, p2)) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Days, negative);
         return true;
      }
      else if (first.isSecondWord(STRING_HOUR, p2) || first.isSecondWord(STRING_HOURS, p2)) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Hours, negative);
         return true;
      }
      else if (first.isSecondWord(STRING_MINUTE, p2) || first.isSecondWord(STRING_MINUTES, p2)) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Minutes, negative);
         return true;
      }
      else if (first.isSecondWord(STRING_SECOND, p2) || first.isSecondWord(STRING_SECONDS, p2)) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Seconds, negative);
         return true;
      }
      else if (first.isSecondWord(STRING_DATE, p2) || first.isSecondWord(STRING_WEEKDAY, p2)) {
         throw SyntaxError(str(L"value of '", first.getOriginString_2(p2),
            L"' time variable member cannot be altered"), first.line);
      }

      parse::timeVariableMemberException(first, p2);
   }
   else {
      throw SyntaxError(str(L"operator ", toStr(sign),
         L"= should be preceded by a single word, which is a variable name"),
         first.line);
   }

   return false;
}

static p_bool commandVarIncrement(_comptr& result, const Token& first, const Tokens& tks, const p_int line, pp_perun2& p2)
{
   Variable<p_str>* pvp_str;
   if (p2.contexts.getVar(first, pvp_str, p2)) {
      _genptr<p_str> str_;
      if (parse::parse(p2, tks, str_)) {
         pvp_str->makeNotConstant();

         if (pvp_str->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
         }

         result = std::make_unique<VarAdd_<p_str>>(*pvp_str, str_);
         return true;
      }

      _genptr<p_list> list;

      if (parse::parse(p2, tks, list)) {
         throw SyntaxError(str(L"variable '", first.getOriginString(p2),
            L"' can be incremented only by a string"), line);
      }
      else {
         throw SyntaxError(L"right side of operator '+=' cannot"
            L" be resolved to a string", line);
      }
   }

   throw SyntaxError(str(L"variable '", first.getOriginString(p2),
      L"' cannot be incremented by a value"), line);
}

template <typename T>
static p_bool makeVarAlteration(pp_perun2& p2, const Tokens& tokens, const Token& first,
   Variable<T>*& varPtr, _comptr& result, const p_str& dataTypeName)
{
   if (p2.contexts.getVar(first, varPtr, p2)) {
      if (varPtr->isImmutable()) {
         throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
      }

      _genptr<T> value;
      if (parse::parse(p2, tokens, value)) {
         if (!value->isConstant()) {
            varPtr->makeNotConstant();
         }
         if (varPtr->isConstant()) {
            varPtr->value = value->getValue();
         }
         result = std::make_unique<VarAssignment<T>>(*varPtr, value);
         return true;
      }
      else {
         throw SyntaxError(str(L"value assigned to variable '", first.getOriginString(p2),
            L"' has to be of ", dataTypeName, L" type"), first.line);
      }
   }

   return false;
}

template <typename T>
static void makeVarAssignment(_comptr& result, const Token& token, pp_perun2& p2,
   Variable<T>* varPtr, _genptr<T>& valuePtr)
{
   UserVarsContext* uvc = p2.contexts.getUserVarsContext();
   const p_bool isConstant = !p2.contexts.hasAggregate() && valuePtr->isConstant();
   _varptrs<T>* allVarsOfThisType;
   uvc->userVars.takeVarsPtr(allVarsOfThisType);
   p_str name = token.toLowerString(p2);
   allVarsOfThisType->insert(std::make_pair(name, std::make_unique<Variable<T>>(VarType::vt_User)));

   (*allVarsOfThisType)[name]->isConstant_ = isConstant;
   if (isConstant) {
      (*allVarsOfThisType)[name]->value = valuePtr->getValue();
   }

   result = std::make_unique<comm::VarAssignment<T>>(*(*allVarsOfThisType)[name], valuePtr);
}

static p_bool commandVarAssign(_comptr& result, const Tokens& left, const Tokens& right, pp_perun2& p2)
{
   const Token& first = left.first();

   if (first.type != Token::t_Word) {
      return false;
   }

   if (left.getLength() >= 5 ) {
      if (left.last().type == Token::t_TwoWords && left.last().isFirstWord(EMPTY_STRING, p2)) {
         Tokens le(left);
         le.trimRight();

         if (varSquareBrackets(le)) {
            Variable<p_nlist>* pvp_nlist;
            Variable<p_tlist>* pvp_tlist;
            Variable<p_list>* pvp_list;

            if (p2.contexts.getVar(first, pvp_nlist, p2) ||
                p2.contexts.getVar(first, pvp_tlist, p2) ||
                p2.contexts.getVar(first, pvp_list, p2))
            {
               throw SyntaxError(str(L"collection variable '", first.getOriginString(p2),
                  L"' is immutable, so its elements cannot me modified"), first.line);
            }
            else {
               throw SyntaxError(str(L"unknown collection variable '", first.getOriginString(p2),
                  L"'. Collection variables are immutable in Perun2 anyways"), first.line);
            }
         }
      }
   }

   if (left.getLength() != 1) {
      return false;
   }

   /////
   // assign value to an existing variable
   ////

   if (first.isWord(STRINGS_VARS_IMMUTABLES, p2)) {
      throw SyntaxError(str(L"variable '", first.getOriginString(p2), L"' is immutable"), first.line);
   }

   Variable<p_bool>* pv_boo = nullptr;
   if (makeVarAlteration(p2, right, first, pv_boo, result, STRING_BOOL)) {
      return true;
   }

   Variable<p_num>* pvp_num = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_num, result, STRING_NUMBER)) {
      return true;
   }

   Variable<p_tim>* pvp_tim = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_tim, result, STRING_TIME)) {
      return true;
   }

   Variable<p_per>* pvp_per = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_per, result, STRING_PERIOD)) {
      return true;
   }

   Variable<p_str>* pvp_str = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_str, result, STRING_STRING)) {
      return true;
   }

   Variable<p_nlist>* pvp_nlist = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_nlist, result, STRING_NUMERIC_LIST)) {
      return true;
   }

   Variable<p_tlist>* pvp_tlist = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_tlist, result, STRING_TIME_LIST)) {
      return true;
   }

   Variable<p_list>* pvp_list = nullptr;
   if (makeVarAlteration(p2, right, first, pvp_list, result, STRING_LIST)) {
      return true;
   }

   /////
   // create a new variable
   ////

   const p_size wordLength = first.value.word.os.length;
   if (wordLength != 1) {
      throw SyntaxError(str(L"name '", first.getOriginString(p2),
         L"' is not valid for a new variable. In Perun2, user defined variables can be only one-character long"), first.line);
   }

   _genptr<p_bool> boo;
   if (parse::parse(p2, right, boo)) {
      makeVarAssignment(result, first, p2, pv_boo, boo);
      return true;
   }

   _genptr<p_num> num;
   if (parse::parse(p2, right, num)) {
      makeVarAssignment(result, first, p2, pvp_num, num);
      return true;
   }

   _genptr<p_tim> tim;
   if (parse::parse(p2, right, tim)) {
      makeVarAssignment(result, first, p2, pvp_tim, tim);
      return true;
   }

   _genptr<p_per> per;
   if (parse::parse(p2, right, per)) {
      makeVarAssignment(result, first, p2, pvp_per, per);
      return true;
   }

   _genptr<p_str> str_;
   if (parse::parse(p2, right, str_)) {
      makeVarAssignment(result, first, p2, pvp_str, str_);
      return true;
   }

   _genptr<p_nlist> nlist;
   if (parse::parse(p2, right, nlist)) {
      makeVarAssignment(result, first, p2, pvp_nlist, nlist);
      return true;
   }

   _genptr<p_tlist> tlist;
   if (parse::parse(p2, right, tlist)) {
      makeVarAssignment(result, first, p2, pvp_tlist, tlist);
      return true;
   }

   _genptr<p_list> list;
   if (parse::parse(p2, right, list)) {
      makeVarAssignment(result, first, p2, pvp_list, list);
      return true;
   }

   throw SyntaxError(str(L"value assigned to variable '", first.getOriginString(p2),
      L"' cannot be resolved to any data type"), first.line);
}

static p_bool varSquareBrackets(const Tokens& tks)
{
   const p_int length = tks.getLength();
   if (length < 3 || !tks.second().isSymbol(CHAR_OPENING_SQUARE_BRACKET)
      || !tks.last().isSymbol(CHAR_CLOSING_SQUARE_BRACKET))
   {
      return false;
   }

   if (tks.first().type != Token::t_Word) {
      throw SyntaxError(L"brackets [] have to be preceded by a collection variable name",
         tks.first().line);
   }

   if (length == 3) {
      throw SyntaxError(L"empty space between [] brackets",
         tks.second().line);
   }

   return true;
}

static p_bool commandVarAssign_Element(_comptr& result, const Tokens& left,
   const Tokens& right, pp_perun2& p2)
{
   const Token& first = left.first();

   Variable<p_list>* pvp_list;
   Variable<p_nlist>* pvp_nlist;
   Variable<p_tlist>* pvp_tlist;

   if (p2.contexts.getVar(first, pvp_list, p2) ||
       p2.contexts.getVar(first, pvp_nlist, p2) ||
       p2.contexts.getVar(first, pvp_tlist, p2))
   {
      throw SyntaxError(str(L"collection variable '", first.getOriginString(p2),
         L"' is immutable, so its elements cannot me modified"), first.line);
   }

   Variable<p_str>* pvp_str;
   if (p2.contexts.getVar(first, pvp_str, p2)) {
      if (! pvp_str->isImmutable()) {
         _genptr<p_num> index;
         parseListElementIndex(index, left, p2);
         _genptr<p_str> str_;

         if (parse::parse(p2, right, str_)) {
            pvp_str->makeNotConstant();
            result = std::make_unique<VarCharAssignment>(*pvp_str, str_, index);
            return true;
         }
         else {
            throw SyntaxError(str(L"new value in character assignment of variable '",
               first.getOriginString(p2), L"' cannot be resolved to a string"), first.line);
         }
      }
      else {
         throw SyntaxError(str(L"variable '", first.getOriginString(p2),
            L"' does not exist or is unreachable here"), first.line);
      }
   }

   throw SyntaxError(str(L"variable '", first.getOriginString(p2),
      L"' was not expected before [] brackets"), first.line);
}

static p_bool parseListElementIndex(_genptr<p_num>& result, const Tokens& tks, pp_perun2& p2)
{
   const p_size start = tks.getStart() + 2;
   const p_size length = tks.getLength() - 3;
   const Tokens tks2(tks, start, length);
   _genptr<p_num> index;

   if (!parse::parse(p2, tks2, index)) {
      throw SyntaxError(
        L"content of square brackets [] cannot be resolved to a number",
        tks.second().line);
   }

   result = std::move(index);
   return true;
}

static void checkNoSemicolonBeforeBrackets(const Tokens& tks, pp_perun2& p2)
{
   const p_int end = tks.getEnd();
   const p_int start = tks.getStart() + 1;
   const p_bool startsWithElse = tks.listAt(start - 1).isKeyword(Keyword::kw_Else);

   for (p_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Keyword) {
         switch (t.value.keyword.k) {
            case Keyword::kw_While:
            case Keyword::kw_Inside: {
               throw SyntaxError(str(L"a semicolon ; is missing before keyword '",
                  t.getOriginString(p2), L"'"), t.line);
            }
            case Keyword::kw_If: {
               if (!(i == start && startsWithElse)) {
                  throw SyntaxError(str(L"a semicolon ; is missing before keyword '",
                     t.getOriginString(p2), L"'"), t.line);
               }
               break;
            }
            case Keyword::kw_Else: {
               throw SyntaxError(str(L"keyword '", t.getOriginString(p2),
                  L"' should be preceded by curly brackets {}"), t.line);
            }
         }
      }
   }
}

}
