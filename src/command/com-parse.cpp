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

#include "com-parse.h"
#include "com-misc.h"
#include "../exception.h"
#include "../hash.h"
#include "com-struct.h"
#include "../datatype/parse-gen.h"
#include "../datatype/parse/parse-number.h"
#include "com-parse-kw.h"
#include "com-parse-unit.h"
#include "com-condition.h"
#include "../datatype/generator/gen-string.h"
#include "../datatype/parse/parse-number.h"


namespace uro::comm
{

_bool parseCommands(_comptr& result, const Tokens& tks, _uro& uro)
{
   _ucptr context = std::make_unique<UserVarsContext>();
   uro.contexts.addUserVarsContext(context.get());

   std::vector<_comptr> commands;
   _int sublen = 0;
   _int depth = 0;
   _int open = -1;
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol)  {
         switch(t.value.ch) {
            case CHAR_SEMICOLON: {
               if (depth == 0) {
                  if (sublen != 0) {
                     uro.conditionContext.lockLast();
                     Tokens tks2(tks, i - sublen, sublen);
                     _comptr com;
                     command(com, tks2, uro);
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

                  if (commandStruct(com, tks, sublen, i, open, uro)) {
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
      command(com, tks2, uro);
      commands.push_back(std::move(com));
   }

   if (commands.size() == 1) {
      result = std::move(commands[0]);
   }
   else {
      result = std::make_unique<CS_RawBlock>(commands, uro);
   }

   uro.contexts.retreatUserVarsContext();
   return true;
}

void checkKeywordsBeforeCurlyBrackets(const Tokens& tks, _uro& uro)
{
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (t.type == Token::t_Keyword && t.isExpForbiddenKeyword()) {
         throw SyntaxError(str(L"a command-ending semicolon ; was expected somewhere between keyword '",
            t.getOriginString(uro), L"' and curly brackets {}"), t.line);
      }
   }
}


static _bool commandStruct(_comptr& result, const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open, _uro& uro)
{
   const _int leftStart = index - sublen;
   const _int leftLen = open - leftStart;
   const _int rightStart = open + 1;
   const _int rightLen = index - rightStart;

   // build a simple block of commands
   if (leftLen == 0) {
      if (rightLen == 0) {
         return false;
      }

      _acptr context = std::make_unique<AggregateContext>(uro);
      uro.contexts.addAggregateContext(context.get());
      Tokens right(tks, rightStart, rightLen);
      const _bool b = parseCommandsAsMember(result, right, nullptr, uro);
      uro.contexts.retreatAggregateContext();
      return b;
   }

   Tokens left(tks, leftStart, leftLen);
   checkKeywordsBeforeCurlyBrackets(left, uro);

   // build "times"
   const Token& leftLast = left.last();
   if (leftLast.isKeyword(Keyword::kw_Times)) {
      left.trimRight();
      if (left.isEmpty()) {
         throw SyntaxError(str(L"keyword '", leftLast.getOriginString(uro),
            L"' is not preceded by a number"), leftLast.line);
      }

      if (rightLen == 0) {
         return false;
      }

      left.checkCommonExpressionExceptions(uro);
      _genptr<_num> num;
      if (!parse::parse(uro, left, num)) {
         throw SyntaxError(str(L"keyword '", leftLast.getOriginString(uro),
            L"' is not preceded by a valid number"), leftLast.line);
      }

      Tokens right(tks, rightStart, rightLen);

      _icptr context = std::make_unique<IndexContext>(uro);
      uro.contexts.addIndexContext(context.get());
      _comptr com;
      const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
      uro.contexts.retreatIndexContext();

      if (success) {
         result = std::make_unique<CS_Times>(num, com, context, uro);
      }

      return success;
   }

   checkNoSemicolonBeforeBrackets(left, uro);

   // build "while"
   const Token& leftFirst = left.first();
   if (leftFirst.isKeyword(Keyword::kw_While)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(uro),
            L"' is not followed by a condition"), leftFirst.line);
      }

      if (rightLen == 0) {
         throw SyntaxError(str(L"structure '", leftFirst.getOriginString(uro),
            L"' is empty. It would either never run or cause an infinite loop"), leftFirst.line);
      }

      left.checkCommonExpressionExceptions(uro);
      _genptr<_bool> boo;
      if (!parse::parse(uro, left, boo)) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(uro),
            L"' is not followed by a valid condition"), leftFirst.line);
      }

      Tokens right(tks, rightStart, rightLen);

      _icptr context = std::make_unique<IndexContext>(uro);
      uro.contexts.addIndexContext(context.get());
      _comptr com;
      const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
      uro.contexts.retreatIndexContext();

      if (success) {
         result = std::make_unique<CS_While>(boo, com, context, uro);
      }

      return success;
   }

   // build "inside"
   if (leftFirst.isKeyword(Keyword::kw_Inside)) {
      throw SyntaxError(L"not implemented", leftFirst.line);
      /*left.trimLeft();
      if (left.isEmpty() && uro.vars.inner.thisState != ThisState::ts_String) {
         throw SyntaxError(str(L"argumentless structure '", leftFirst.getOriginString(uro),
            L"' can be created only inside a loop iterating over strings"), leftFirst.line);
      }
      left.checkCommonExpressionExceptions(uro);

      if (rightLen == 0) {
         return false;
      }

      Tokens right(tks, rightStart, rightLen);

      if (parseIterationLoop(result, true, left, right, prevThisState, uro)) {
         return true;
      }

      throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(uro),
         L"' is not followed by a valid value for directories to visit"), leftFirst.line);*/
   }

   // build "if"
   if (leftFirst.isKeyword(Keyword::kw_If)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(uro), L"' is not followed by a condition"),
            leftFirst.line);
      }

      left.checkCommonExpressionExceptions(uro);
      _genptr<_bool> boo;
      if (!parse::parse(uro, left, boo)) {
         throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(uro), L"' is not followed by a valid condition"),
            leftFirst.line);
      }

      std::unique_ptr<CS_Condition> cond = std::make_unique<CS_Condition>();
      _comptr* inter = cond->getCommandPtr();

      if (rightLen == 0) {
         uro.conditionContext.addClosed(inter);
         uro.conditionContext.setMain(boo);
         result = std::move(cond);
         return true;
      }

      Tokens right(tks, rightStart, rightLen);

      _comptr com;
      if (parseCommandsAsMember(com, right, inter, uro)) {
         uro.conditionContext.setMain(com, boo);
      }

      result = std::move(cond);
      return true;
   }

   if (leftFirst.isKeyword(Keyword::kw_Else)) {
      left.trimLeft();

      if (left.isEmpty()) { // build "else"
         if (rightLen == 0) {
            uro.conditionContext.addEmptyElse(leftFirst.line);
            return false;
         }

         Tokens right(tks, rightStart, rightLen);

         _comptr com;
         const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
         uro.conditionContext.deleteLast();

         if (success) {
            uro.conditionContext.addElse(com, leftFirst.line);
         }
      }
      else { // build "else if"
         if (!left.first().isKeyword(Keyword::kw_If)) {
            throw SyntaxError(str(L"keyword '", leftFirst.getOriginString(uro),
               L"' cannot be followed by an expression"), leftFirst.line);
         }

         const Token& ifToken = left.first();
         left.trimLeft();

         if (left.isEmpty()) {
            throw SyntaxError(str(L"keywords '", leftFirst.getOriginString(uro), L" ",
               ifToken.getOriginString(uro), L"' are not followed by a condition"), leftFirst.line);
         }

         left.checkCommonExpressionExceptions(uro);
         _genptr<_bool> boo;
         if (!parse::parse(uro, left, boo)) {
            throw SyntaxError(str(L"keywords '", leftFirst.getOriginString(uro), L" ",
               ifToken.getOriginString(uro), L"' are not followed by a valid condition"), leftFirst.line);
         }

         if (rightLen == 0) {
            _comptr com = std::make_unique<C_DoNothing>();
            uro.conditionContext.addElseIf(boo, com, leftFirst.line);
            return false;
         }

         Tokens right(tks, rightStart, rightLen);
         _comptr com;
         const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
         uro.conditionContext.deleteLast();

         if (com != nullptr) {
            uro.conditionContext.addElseIf(boo, com, leftFirst.line);
         }
      }

      return false;
   }

   if (rightLen == 0) {
      return false;
   }

   left.checkCommonExpressionExceptions(uro);
   Tokens right(tks, rightStart, rightLen);

   // string loop
   _genptr<_str> str_;
   if (parse::parse(uro, left, str_)) {
      _fcptr context = std::make_unique<FileContext>(uro);
      uro.contexts.addFileContext(context.get());
      _comptr com;
      const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
      uro.contexts.retreatFileContext();

      if (success) {
         result = std::make_unique<CS_StringLoop>(str_, com, context, uro);
      }

      return success;
   }

   // definition loop
   _defptr def;
   if (parse::parse(uro, left, def)) {
      FileContext* fc = def->getFileContext();

      if (fc != nullptr) {
         uro.contexts.addFileContext(fc);
         _comptr com;
         const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
         uro.contexts.retreatFileContext();

         if (success) {
            /*if (fc->attribute->isMarkedToEvaluate()) {
               _genptr<_list> g(new gen::Cast_D_L(def, uro));
               result = std::make_unique<CS_ListLoop>(g, com, uro);
            }
            else {*/
               result = std::make_unique<CS_DefinitionLoop>(def, com, uro);
            //}
         }

         return success;
      }

      _fcptr context = std::make_unique<FileContext>(uro);
      uro.contexts.addFileContext(context.get());
      _comptr com;
      const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
      uro.contexts.retreatFileContext();

      if (success) {
         /*if (fc->attribute->isMarkedToEvaluate()) {
            _genptr<_list> g(new gen::Cast_D_L(def, uro));
            result = std::make_unique<CS_ListLoop>(g, com, uro);
         }
         else {*/
            result = std::make_unique<CS_DefinitionLoop>(def, com, context, uro);
         //}
      }

      return success;
   }

   // list loop
   _genptr<_list> list;
   if (parse::parse(uro, left, list)) {
      _fcptr context = std::make_unique<FileContext>(uro);
      uro.contexts.addFileContext(context.get());
      _comptr com;
      const _bool success = parseCommandsAsMember(com, right, nullptr, uro);
      uro.contexts.retreatFileContext();

      if (success) {
         result = std::make_unique<CS_ListLoop>(list, com, context, uro);
      }

      return success;
   }





   throw SyntaxError(L"tokens before { bracket do not form any valid syntax structure", tks.first().line);
}

/*
static _bool parseIterationLoop(_comptr& result, const _bool& isInside, const Tokens& left, const Tokens& right,
   const ThisState& prevState, _uro& uro)
{
   _comptr com;

   if (isInside && left.isEmpty()) {
      _genptr<_str> tr;
      uro.vars.inner.createThisRef(tr);
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      _attrptr attr;
      _aggrptr aggr;

      if (parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         result = std::make_unique<CS_InsideString>(tr, com, attr, aggr, hasMemory, uro);
         return true;
      }
      else {
         return false;
      }
   }

   // string loop
   _genptr<_str> str;
   if (parse::parse(uro, left, str)) {
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      _attrptr attr;
      _aggrptr aggr;

      if (parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         if (isInside) {
            result = std::make_unique<CS_InsideString>(str, com, attr, aggr, hasMemory, uro);
         }
         else {
            result = std::make_unique<CS_StringLoop>(str, com, attr, aggr, hasMemory, uro);
         }

         return true;
      }
      else {
         return false;
      }
   }

   // definition loop
   _defptr def;
   if (parse::parse(uro, left, def)) {
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      _attrptr attr;
      _aggrptr aggr;

      if (!parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         return false;
      }

      if (attr->isMarkedToEvaluate()) {
         _genptr<_list> g(new gen::Cast_D_L(def, uro));

         if (isInside) {
            result = std::make_unique<CS_InsideList>(g, com, attr, aggr, hasMemory, uro);
         }
         else {
            result = std::make_unique<CS_ListLoop>(g, com, attr, aggr, hasMemory, uro);
         }

         return true;
      }

      _fdata* fdata = def->getDataPtr();

      if (fdata == nullptr) {
         if (isInside) {
            result = std::make_unique<CS_InsideDefinition>(def, com, attr, aggr, hasMemory, uro);
         }
         else {
            result = std::make_unique<CS_DefinitionLoop>(def, com, attr, aggr, hasMemory, uro);
         }

         return true;
      }

      const _aunit aval = attr->getValue();
      _attrptr bridge(new BridgeAttribute(aval, uro, fdata));

      if (isInside) {
         result = std::make_unique<CS_InsideDefinition>(def, com, bridge, aggr, hasMemory, uro);
      }
      else {
         result = std::make_unique<CS_DefinitionLoop>(def, com, bridge, aggr, hasMemory, uro);
      }

      return true;
   }

   // list loop
   _genptr<_list> lst;
   if (parse::parse(uro, left, lst)) {
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      _attrptr attr;
      _aggrptr aggr;

      if (!parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         return false;
      }

      if (isInside) {
         result = std::make_unique<CS_InsideList>(lst, com, attr, aggr, hasMemory, uro);
      }
      else {
         result = std::make_unique<CS_ListLoop>(lst, com, attr, aggr, hasMemory, uro);
      }

      return true;
   }

   return false;
}

static _bool parseLoopBase(_comptr& result, const Tokens& rightTokens, _uro& uro,
   const ThisState& prevState, _attrptr& attr, _aggrptr& aggr, _bool& hasMemory)
{
   hasMemory = uro.vc.anyAttribute();
   attr = std::make_unique<Attribute>(uro);
   uro.vc.addAttribute(attr);
   aggr = std::make_unique<Aggregate>(uro);
   uro.vc.addAggregate(aggr);

   const _bool success = parseCommandsAsMember(result, rightTokens, nullptr, uro);

   uro.vars.inner.thisState = prevState;
   uro.vc.retreatAttribute();
   uro.vc.retreatAggregate();

   return success;
}

static _bool parseLoopBase(_comptr& result, const Tokens& rightTokens, _uro& uro,
   const ThisState& prevState, _aggrptr& aggr)
{
   aggr = std::make_unique<Aggregate>(uro);
   uro.vc.addAggregate(aggr);

   const _bool success = parseCommandsAsMember(result, rightTokens, nullptr, uro);

   uro.vars.inner.thisState = prevState;
   uro.vc.retreatAggregate();

   return success;
}*/

static _bool parseCommandsAsMember(_comptr& result, const Tokens& tks, _comptr* cond, _uro& uro)
{
   uro.conditionContext.add(cond);
   const _bool success = parseCommands(result, tks, uro);
   uro.conditionContext.deleteClosedUnits();
   return success;
}

static _bool command(_comptr& result, Tokens& tks, _uro& uro)
{
   const Token& f = tks.first();

   if (tks.getLength() == 1 && f.type == Token::t_Keyword) {
      switch (f.value.keyword.k) {
         case Keyword::kw_Break:
         case Keyword::kw_Continue: {
            if (uro.contexts.hasIndexContext()) {
               if (f.value.keyword.k == Keyword::kw_Break) {
                  result = std::make_unique<C_Break>(uro);
                  return true;
               }
               else {
                  result = std::make_unique<C_Continue>(uro);
                  return true;
               }
            }
            else {
               throw SyntaxError(str(L"command '", f.getOriginString(uro),
                  L"' can be called only inside a loop"), f.line);
            }
            break;
         }
         case Keyword::kw_Exit: {
            result = std::make_unique<C_Exit>(uro);
            return true;
         }
      }
   }

   _bool force = false;
   _bool stack = false;

   const Token& f2 = tks.first();
   if (f2.type == Token::t_Keyword) {
      switch (f2.value.keyword.k) {
         case Keyword::kw_Force: {
            tks.trimLeft();
            force = true;

            if (tks.isEmpty()) {
               throw SyntaxError(str(L"command cannot consist of only one keyword: '",
                  f2.getOriginString(uro),L"'"), f.line);
            }
            break;
         }
         case Keyword::kw_Stack: {
            tks.trimLeft();
            stack = true;

            if (tks.isEmpty()) {
               throw SyntaxError(str(L"command cannot consist of only one keyword: '",
                  f2.getOriginString(uro),L"'"), f.line);
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
            tks.checkCommonExpressionExceptions(uro);
            return keywordCommands(result, f3, tks, f.line, force, stack, uro);
         }
      }
   }

   if (force) {
      throw SyntaxError(L"only a core command can start with a keyword 'force'", f.line);
   }
   if (stack) {
      throw SyntaxError(L"only a core command can start with a keyword 'stack'", f.line);
   }

   if (commandMisc(result, tks, uro)) {
      return true;
   }
   else {
      tks.checkCommonExpressionExceptions(uro);
      return c_print(result, Token(Keyword::kw_Print, f.line, static_cast<_size>(0),
         static_cast<_size>(0), uro), tks, f.line, false, uro);
   }
}

static _bool commandMisc(_comptr& result, const Tokens& tks, _uro& uro)
{
   if (tks.check(TI_HAS_CHAR_EQUALS)) {
      std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_EQUAL_SIGN);
      Tokens& left = pair.first;
      Tokens& right = pair.second;
      right.checkCommonExpressionExceptions(uro);

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

      if (right.first().isSymbol(CHAR_EQUAL_SIGN)) {
         return false;
      }

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

               return commandVarChange(result, left, right, ch, uro);
            }
         }
      }

      if (varSquareBrackets(left)) {
         return commandVarAssign_Element(result, left, right, uro);
      }
      else {
         return commandVarAssign(result, left, right, uro);
      }
   }

   const Token& last = tks.last();

   if (last.type == Token::t_MultiSymbol &&
       (last.value.chars.ch == CHAR_PLUS || last.value.chars.ch == CHAR_MINUS))
   {
      const _bool isIncrement = last.value.chars.ch == CHAR_PLUS;
      const _str op = isIncrement ? L"incremented by one" : L"decremented by one";
      const Token& first = tks.first();

      if (first.type == Token::t_Word) {
         if (tks.getLength() == 2) {
            vars::Variable<_num>* pv_num;

            if (!uro.contexts.getVariable(first, pv_num, uro) || pv_num->isImmutable()) {
               throw SyntaxError(str(L"variable '", first.getOriginString(uro),
                  L"' cannot be ", op), first.line);
            }

            pv_num->makeNotConstant();

            if (isIncrement) {
               result = std::make_unique<VarIncrement>(*pv_num);
            }
            else {
               result = std::make_unique<VarDecrement>(*pv_num);
            }

            return true;
         }
         else {
            Tokens tks2(tks);
            tks2.trimRight();
            tks2.trimRight();

            if (varSquareBrackets(tks2)) {
               _genptr<_num> index;
               parseListElementIndex(index, tks2, uro);
               vars::Variable<_nlist>* pv_nlist;

               if (!uro.contexts.getVariable(first, pv_nlist, uro) || pv_nlist->isImmutable()) {
                  throw SyntaxError(str(L"variable '", first.getOriginString(uro),
                     L"' cannot be ", op), first.line);
               }

               throw SyntaxError(str(L"an element of variable '", first.getOriginString(uro),
                  L"' cannot be ", op, L", because collections in Uroboros2 are immutable"),
                  first.line);
            }
            else {
               throw SyntaxError(str(L"this structure cannot be ", op), first.line);
            }
         }
      }
      else if (first.type == Token::t_TwoWords && tks.getLength() == 2) {
         if (first.value.twoWords.h1 == uro.hashes.HASH_NOTHING) {
            throw SyntaxError(L"the dot . should be preceded by a time variable name", first.line);
         }

         vars::Variable<_tim>* pv_tim;

         if (!uro.contexts.getVariable(first, pv_tim, uro)) {
            throw SyntaxError(str(L"time variable from expression '", first.getOriginString(uro),
               L"' does not exist or is unreachable here"), first.line);
         }

         if (pv_tim->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
         }

         const _size& h = first.value.twoWords.h2;
         Period::PeriodUnit unit;

         if (h == uro.hashes.HASH_PER_YEAR || h == uro.hashes.HASH_PER_YEARS)
            unit = Period::u_Years;
         else if (h == uro.hashes.HASH_PER_MONTH || h == uro.hashes.HASH_PER_MONTHS)
            unit = Period::u_Months;
         else if (h == uro.hashes.HASH_PER_DAY || h == uro.hashes.HASH_PER_DAYS)
            unit = Period::u_Days;
         else if (h == uro.hashes.HASH_PER_HOUR || h == uro.hashes.HASH_PER_HOURS)
            unit = Period::u_Hours;
         else if (h == uro.hashes.HASH_PER_MINUTE || h == uro.hashes.HASH_PER_MINUTES)
            unit = Period::u_Minutes;
         else if (h == uro.hashes.HASH_PER_SECOND || h == uro.hashes.HASH_PER_SECONDS)
            unit = Period::u_Seconds;
         else if (h == uro.hashes.HASH_PER_DATE || h == uro.hashes.HASH_PER_WEEKDAY) {
            throw SyntaxError(str(L"time variable member '", first.getOriginString_2(uro),
               L"' cannot be ", op), first.line);
         }
         else {
            parse::timeVariableMemberException(first, uro);
         }

         pv_tim->makeNotConstant();

         if (isIncrement) {
            result = std::make_unique<VarTimeUnitIncrement>(*pv_tim, unit);
         }
         else {
            result = std::make_unique<VarTimeUnitDecrement>(*pv_tim, unit);
         }

         return true;
      }
      else {
         throw SyntaxError(str(L"only a variable of a singular data type can be ", op), first.line);
      }
   }

   return false;
}

static _bool commandVarChange(_comptr& result, const Tokens& left, const Tokens& right, const _char& sign, _uro& uro)
{
   const Token& first = left.first();

   if (left.getLength() > 1) {
      if (varSquareBrackets(left)) {
         vars::Variable<_list>* pv_list;
         vars::Variable<_nlist>* pv_nlist;
         vars::Variable<_tlist>* pv_tlist;

         if (uro.contexts.getVariable(first, pv_list, uro)
          || uro.contexts.getVariable(first, pv_nlist, uro)
          || uro.contexts.getVariable(first, pv_tlist, uro))
         {
            throw SyntaxError(str(L"collection variable '", first.getOriginString(uro),
               L"' is immutable, so its elements cannot be modified"), right.first().line);
         }

         vars::Variable<_str>* pv_str;

         if (uro.contexts.getVariable(first, pv_str, uro)) {
            throw SyntaxError(str(L"operation ", toStr(sign),
               L"= cannot be performed on a character from string variable"), first.line);
         }

         throw SyntaxError(str(first.getOriginString(uro),
            L" is not a collection variable, which are expected before [] brackets"), first.line);
      }
      else {
         const Token& arom = left.last();

         if (arom.type == Token::t_TwoWords && arom.value.twoWords.h1 == uro.hashes.HASH_NOTHING) {
            Tokens aro(left);
            aro.trimRight();

            if (varSquareBrackets(aro)) {
               vars::Variable<_tlist>* pv_tlist;
               if (uro.contexts.getVariable(first, pv_tlist, uro)) {
                  throw SyntaxError(str(L"operation ", toStr(sign),
                     L"= cannot be performed on a time list variable member. Collections in Uroboros2 are immutable"), first.line);
               }
            }
         }

         throw SyntaxError(str(L"operator ", toStr(sign),
            L"= should be preceded by a variable name"), first.line);
      }
   }

   if (first.type == Token::t_Word) {
      vars::Variable<_num>* pv_num;
      if (uro.contexts.getVariable(first, pv_num, uro)) {
         _genptr<_num> num;

         if (!parse::parse(uro, right, num)) {
            throw SyntaxError(str(L"right side of operator ", toStr(sign),
               L"= cannot be resolved to a number"), first.line);
         }

         if (pv_num->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
         }

         vars::Variable<_num>& var = *pv_num;
         pv_num->makeNotConstant();

         switch (sign) {
            case CHAR_PLUS: {
               result = std::make_unique<VarAdd_<_num>>(var, num);
               break;
            }
            case CHAR_MINUS: {
               result = std::make_unique<VarSubtract<_num>>(var, num);
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

      vars::Variable<_per>* pv_per;
      if (uro.contexts.getVariable(first, pv_per, uro)) {
         switch (sign) {
            case CHAR_PLUS:
            case CHAR_MINUS: {
               _genptr<_per> per;

               if (!parse::parse(uro, right, per)) {
                  throw SyntaxError(str(L"right side of operator ", toStr(sign),
                     L"= cannot be resolved to a period"), first.line);
               }

               if (pv_per->isImmutable()) {
                  throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
               }

               pv_per->makeNotConstant();
               vars::Variable<_per>& var = *pv_per;

               if (sign == CHAR_PLUS) {
                  result = std::make_unique<VarAdd_<_per>>(var, per);
               }
               else {
                  result = std::make_unique<VarSubtract<_per>>(var, per);
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

      vars::Variable<_tim>* pv_tim;
      if (uro.contexts.getVariable(first, pv_tim, uro)) {
         switch (sign) {
            case CHAR_ASTERISK:
            case CHAR_SLASH:
            case CHAR_PERCENT: {
               throw SyntaxError(str(L"operation ", toStr(sign),
                  L"= is not valid for a time variable"), first.line);
            }
         }

         _genptr<_per> per;

         if (!parse::parse(uro, right, per)) {
            throw SyntaxError(str(L"right side of operator '", first.getOriginString(uro),
               L" ", toStr(sign), L"=' cannot be resolved to a period"), first.line);
         }

         vars::Variable<_tim>& var = *pv_tim;
         pv_tim->makeNotConstant();

         if (pv_tim->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
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
         return commandVarIncrement(result, first, right, first.line, uro);
      }

      throw SyntaxError(str(L"'", first.getOriginString(uro),
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

      vars::Variable<_tim>* pv_tim;
      if (!uro.contexts.getVariable(first, pv_tim, uro)) {
         throw SyntaxError(str(L"'", first.getOriginString(uro),
            L"' is not a time variable for the ", toStr(sign), L"= operation"),
            first.line);
      }

      if (pv_tim->isImmutable()) {
         throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
      }

      _genptr<_num> num;

      if (!parse::parse(uro, right, num)) {
         throw SyntaxError(str(L"right side of operation '", first.getOriginString(uro), L".",
            first.getOriginString_2(uro), L" ", toStr(sign),
            L"=' cannot be resolved to a number"), first.line);
      }

      const _size& h = first.value.twoWords.h2;
      vars::Variable<_tim>& var = *pv_tim;
      const _bool negative = (sign == '-');
      pv_tim->makeNotConstant();

      if (h == uro.hashes.HASH_PER_YEAR || h == uro.hashes.HASH_PER_YEARS) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Years, negative);
         return true;
      }
      else if (h == uro.hashes.HASH_PER_MONTH || h == uro.hashes.HASH_PER_MONTHS) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Months, negative);
         return true;
      }
      else if (h == uro.hashes.HASH_PER_DAY || h == uro.hashes.HASH_PER_DAYS) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Days, negative);
         return true;
      }
      else if (h == uro.hashes.HASH_PER_HOUR || h == uro.hashes.HASH_PER_HOURS) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Hours, negative);
         return true;
      }
      else if (h == uro.hashes.HASH_PER_MINUTE || h == uro.hashes.HASH_PER_MINUTES) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Minutes, negative);
         return true;
      }
      else if (h == uro.hashes.HASH_PER_SECOND || h == uro.hashes.HASH_PER_SECONDS) {
         result = std::make_unique<VarTimeUnitChange>(var, num, Period::u_Seconds, negative);
         return true;
      }
      else if (h == uro.hashes.HASH_PER_DATE || h == uro.hashes.HASH_PER_WEEKDAY) {
         throw SyntaxError(str(L"value of '", first.getOriginString_2(uro),
            L"' time variable member cannot be altered"), first.line);
      }

      parse::timeVariableMemberException(first, uro);
   }
   else {
      throw SyntaxError(str(L"operator ", toStr(sign),
         L"= should be preceded by a single word, which is a variable name"),
         first.line);
   }

   return false;
}

static _bool commandVarIncrement(_comptr& result, const Token& first, const Tokens& tks, const _int& line, _uro& uro)
{
   vars::Variable<_str>* pv_str;
   if (uro.contexts.getVariable(first, pv_str, uro)) {
      _genptr<_str> str_;
      if (parse::parse(uro, tks, str_)) {
         pv_str->makeNotConstant();

         if (pv_str->isImmutable()) {
            throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
         }

         result = std::make_unique<VarAdd_<_str>>(*pv_str, str_);
         return true;
      }

      _genptr<_list> list;

      if (parse::parse(uro, tks, list)) {
         throw SyntaxError(str(L"variable '", first.getOriginString(uro),
            L"' can be incremented only by a string"), line);
      }
      else {
         throw SyntaxError(L"right side of operator '+=' cannot"
            L" be resolved to a string", line);
      }
   }

   throw SyntaxError(str(L"variable '", first.getOriginString(uro),
      L"' cannot be incremented by a value"), line);
}

template <typename T>
static _bool makeVarAlteration(_uro& uro, const Tokens& tokens, const Token& first,
   vars::Variable<T>*& varPtr, _comptr& result, const _str& dataTypeName)
{
   if (uro.contexts.getVariable(first, varPtr, uro)) {
      if (varPtr->isImmutable()) {
         throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
      }

      _genptr<T> value;
      if (parse::parse(uro, tokens, value)) {
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
         throw SyntaxError(str(L"value assigned to variable '", first.getOriginString(uro),
            L"' has to be of ", dataTypeName, L" type"), first.line);
      }
   }

   return false;
}

template <typename T>
static _bool makeVarAssignment(_comptr& result, const Token& token, _uro& uro,
   vars::Variable<T>* varPtr, _genptr<T>& valuePtr)
{
   /*vars::VarBundle<T>* bundle;
   uro.vars.takeBundlePointer(bundle);
   const _bool isConstant = !uro.vc.anyAggregate() && valuePtr->isConstant();
   return bundle->makeVariableAssignment(result, token, varPtr, valuePtr, isConstant);*/



   UserVarsContext* uvc = uro.contexts.getUserVarsContext();
   const _bool isConstant = !uro.contexts.hasAggregate() && valuePtr->isConstant();
   _varptrs<T>* allVarsOfThisType;
   uvc->userVars.takeVarsPtr(allVarsOfThisType);
   const _size& hash = token.value.word.h;
   allVarsOfThisType->insert(std::make_pair(hash, std::make_unique<vars::Variable<T>>(vars::VarType::vt_User)));

   (*allVarsOfThisType)[hash]->isConstant_ = isConstant;
   if (isConstant) {
      (*allVarsOfThisType)[hash]->value = valuePtr->getValue();
   }

   result = std::make_unique<comm::VarAssignment<T>>(*(*allVarsOfThisType)[hash], valuePtr);
   return true;

}

static _bool commandVarAssign(_comptr& result, const Tokens& left, const Tokens& right, _uro& uro)
{
   const Token& first = left.first();

   if (first.type != Token::t_Word) {
      return false;
   }

   if (left.getLength() >= 5 ) {
      if (left.last().type == Token::t_TwoWords && left.last().value.word.h == uro.hashes.HASH_NOTHING) {
         Tokens le(left);
         le.trimRight();

         if (varSquareBrackets(le)) {
            vars::Variable<_nlist>* pv_nlist;
            vars::Variable<_tlist>* pv_tlist;
            vars::Variable<_list>* pv_list;

            if (uro.contexts.getVariable(first, pv_nlist, uro) ||
                uro.contexts.getVariable(first, pv_tlist, uro) ||
                uro.contexts.getVariable(first, pv_list, uro))
            {
               throw SyntaxError(str(L"collection variable '", first.getOriginString(uro),
                  L"' is immutable, so its elements cannot me modified"), first.line);
            }
            else {
               throw SyntaxError(str(L"unknown collection variable '", first.getOriginString(uro),
                  L"'. Collection variables are immutable in Uroboros2 anyways"), first.line);
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

   if (uro.hashes.HASH_GROUP_VARS_IMMUTABLES.find(first.value.word.h)
      != uro.hashes.HASH_GROUP_VARS_IMMUTABLES.end())
   {
      throw SyntaxError(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
   }

   vars::Variable<_bool>* pv_boo = nullptr;
   if (makeVarAlteration(uro, right, first, pv_boo, result, L"bool")) {
      return true;
   }

   vars::Variable<_num>* pv_num = nullptr;
   if (makeVarAlteration(uro, right, first, pv_num, result, L"number")) {
      return true;
   }

   vars::Variable<_tim>* pv_tim = nullptr;
   if (makeVarAlteration(uro, right, first, pv_tim, result, L"time")) {
      return true;
   }

   vars::Variable<_per>* pv_per = nullptr;
   if (makeVarAlteration(uro, right, first, pv_per, result, L"period")) {
      return true;
   }

   vars::Variable<_str>* pv_str = nullptr;
   if (makeVarAlteration(uro, right, first, pv_str, result, L"string")) {
      return true;
   }

   vars::Variable<_nlist>* pv_nlist = nullptr;
   if (makeVarAlteration(uro, right, first, pv_nlist, result, L"numeric list")) {
      return true;
   }

   vars::Variable<_tlist>* pv_tlist = nullptr;
   if (makeVarAlteration(uro, right, first, pv_tlist, result, L"time list")) {
      return true;
   }

   vars::Variable<_list>* pv_list = nullptr;
   if (makeVarAlteration(uro, right, first, pv_list, result, L"list")) {
      return true;
   }

   /////
   // create a new variable
   // or "resurrect" an existing variable from somewhere else out of reach
   ////

   _genptr<_bool> boo;
   if (parse::parse(uro, right, boo)) {
      return makeVarAssignment(result, first, uro, pv_boo, boo);
   }

   _genptr<_num> num;
   if (parse::parse(uro, right, num)) {
      return makeVarAssignment(result, first, uro, pv_num, num);
   }

   _genptr<_tim> tim;
   if (parse::parse(uro, right, tim)) {
      return makeVarAssignment(result, first, uro, pv_tim, tim);
   }

   _genptr<_per> per;
   if (parse::parse(uro, right, per)) {
      return makeVarAssignment(result, first, uro, pv_per, per);
   }

   _genptr<_str> str_;
   if (parse::parse(uro, right, str_)) {
      return makeVarAssignment(result, first, uro, pv_str, str_);
   }

   _genptr<_nlist> nlist;
   if (parse::parse(uro, right, nlist)) {
      return makeVarAssignment(result, first, uro, pv_nlist, nlist);
   }

   _genptr<_tlist> tlist;
   if (parse::parse(uro, right, tlist)) {
      return makeVarAssignment(result, first, uro, pv_tlist, tlist);
   }

   _genptr<_list> list;
   if (parse::parse(uro, right, list)) {
      return makeVarAssignment(result, first, uro, pv_list, list);
   }

   throw SyntaxError(str(L"value assigned to variable '", first.getOriginString(uro),
      L"' cannot be resolved to any data type"), first.line);
}

static _bool varSquareBrackets(const Tokens& tks)
{
   const _int length = tks.getLength();
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

static _bool commandVarAssign_Element(_comptr& result, const Tokens& left,
   const Tokens& right, _uro& uro)
{
   const Token& first = left.first();

   vars::Variable<_list>* pv_list;
   vars::Variable<_nlist>* pv_nlist;
   vars::Variable<_tlist>* pv_tlist;

   if (uro.contexts.getVariable(first, pv_list, uro) ||
       uro.contexts.getVariable(first, pv_nlist, uro) ||
       uro.contexts.getVariable(first, pv_tlist, uro))
   {
      throw SyntaxError(str(L"collection variable '", first.getOriginString(uro),
         L"' is immutable, so its elements cannot me modified"), first.line);
   }

   vars::Variable<_str>* pv_str;
   if (uro.contexts.getVariable(first, pv_str, uro)) {
      if (! pv_str->isImmutable()) {
         _genptr<_num> index;
         parseListElementIndex(index, left, uro);
         _genptr<_str> str_;

         if (parse::parse(uro, right, str_)) {
            pv_str->makeNotConstant();
            result = std::make_unique<VarCharAssignment>(*pv_str, str_, index);
            return true;
         }
         else {
            throw SyntaxError(str(L"new value in character assignment of variable '",
               first.getOriginString(uro), L"' cannot be resolved to a string"), first.line);
         }
      }
      else {
         throw SyntaxError(str(L"variable '", first.getOriginString(uro),
            L"' does not exist or is unreachable here"), first.line);
      }
   }

   throw SyntaxError(str(L"variable '", first.getOriginString(uro),
      L"' was not expected before [] brackets"), first.line);
}

static _bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, _uro& uro)
{
   const _size start = tks.getStart() + 2;
   const _size length = tks.getLength() - 3;
   const Tokens tks2(tks, start, length);
   _genptr<_num> index;

   if (!parse::parse(uro, tks2, index)) {
      throw SyntaxError(
        L"content of square brackets [] cannot be resolved to a number",
        tks.second().line);
   }

   result = std::move(index);
   return true;
}

static void checkNoSemicolonBeforeBrackets(const Tokens& tks, _uro& uro)
{
   const _int end = tks.getEnd();
   const _int start = tks.getStart() + 1;
   const _bool startsWithElse = tks.listAt(start - 1).isKeyword(Keyword::kw_Else);

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Keyword) {
         switch (t.value.keyword.k) {
            case Keyword::kw_Catch:
            case Keyword::kw_While:
            case Keyword::kw_Inside: {
               throw SyntaxError(str(L"a semicolon ; is missing before keyword '",
                  t.getOriginString(uro), L"'"), t.line);
            }
            case Keyword::kw_If: {
               if (!(i == start && startsWithElse)) {
                  throw SyntaxError(str(L"a semicolon ; is missing before keyword '",
                     t.getOriginString(uro), L"'"), t.line);
               }
               break;
            }
            case Keyword::kw_Else: {
               throw SyntaxError(str(L"keyword '", t.getOriginString(uro),
                  L"' should be preceded by curly brackets {}"), t.line);
            }
         }
      }
   }
}

}
