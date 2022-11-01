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

Command* parseCommands(const Tokens& tks, Uroboros& uro)
{
   std::vector<Command*> commands;
   _int sublen = 0;
   _int depth = 0;
   _int open = -1;
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol)  {
         switch(t.value.ch) {
            case L';': {
               if (depth == 0) {
                  if (sublen != 0) {
                     uro.conditionContext.lockLast();
                     Tokens tks2(tks, i - sublen, sublen);
                     commands.push_back(command(tks2, uro));
                     sublen = 0;
                  }
               }
               else {
                  sublen++;
               }
               break;
            }
            case L'{': {
               if (depth == 0) {
                  open = i;
               }
               depth++;
               sublen++;
               break;
            }
            case L'}': {
               depth--;
               if (depth == 0) {
                  Command* com = commandStruct(tks, sublen, i, open, uro);
                  if (com != nullptr) {
                     commands.push_back(com);
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
      commands.push_back(command(tks2, uro));
   }

   if (commands.size() == 1) {
      return commands[0];
   }
   else {
      return new CS_Block(commands, uro);
   }
}

void checkKeywordsBeforeCurlyBrackets(const Tokens& tks, Uroboros& uro)
{
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (t.type == Token::t_Keyword && t.isExpForbiddenKeyword()) {
         throw SyntaxException(str(L"a command-ending semicolon ; was expected somewhere between keyword '",
            t.getOriginString(uro), L"' and curly brackets {}"), t.line);
      }
   }
}

// can return nullptr
static Command* commandStruct(const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open, Uroboros& uro)
{
   const _int leftStart = index - sublen;
   const _int leftLen = open - leftStart;
   const _int rightStart = open + 1;
   const _int rightLen = index - rightStart;
   Command* com = nullptr;

   // build simple block of commands
   if (leftLen == 0) {
      if (rightLen == 0) {
         return nullptr;
      }

      Tokens right(tks, rightStart, rightLen);
      com = parseCommandsAsMember(right, nullptr, uro);
      return com;
   }

   Tokens left(tks, leftStart, leftLen);
   checkKeywordsBeforeCurlyBrackets(left, uro);

   // build "times"
   const Token& leftLast = left.last();
   if (leftLast.isKeyword(Keyword::kw_Times)) {
      left.trimRight();
      if (left.isEmpty()) {
         throw SyntaxException(str(L"keyword '", leftLast.getOriginString(uro),
            L"' is not preceded by a number"), leftLast.line);
      }

      if (rightLen == 0) {
         return nullptr;
      }

      left.checkCommonExpressionExceptions(uro);
      _genptr<_num> num;
      if (!parse::parse(uro, left, num)) {
         throw SyntaxException(str(L"keyword '", leftLast.getOriginString(uro),
            L"' is not preceded by a valid number"), leftLast.line);
      }

      Tokens right(tks, rightStart, rightLen);

      Aggregate* aggr = new Aggregate(uro);
      uro.vc.addAggregate(aggr);
      com = parseCommandsAsMember(right, nullptr, uro);
      uro.vc.retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_Times(num, com, aggr, uro);
   }

   checkNoSemicolonBeforeBrackets(left, uro);

   // build "while"
   const Token& leftFirst = left.first();
   if (leftFirst.isKeyword(Keyword::kw_While)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxException(str(L"keyword '", leftFirst.getOriginString(uro),
            L"' is not followed by a condition"), leftFirst.line);
      }

      if (rightLen == 0) {
         throw SyntaxException(str(L"structure '", leftFirst.getOriginString(uro),
            L"' is empty. It would either never run or cause an infinite loop"), leftFirst.line);
      }

      left.checkCommonExpressionExceptions(uro);
      _genptr<_bool> boo;
      if (!parse::parse(uro, left, boo)) {
         throw SyntaxException(str(L"keyword '", leftFirst.getOriginString(uro),
            L"' is not followed by a valid condition"), leftFirst.line);
      }

      Tokens right(tks, rightStart, rightLen);

      Aggregate* aggr = new Aggregate(uro);
      uro.vc.addAggregate(aggr);
      com = parseCommandsAsMember(right, nullptr, uro);
      uro.vc.retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_While(boo, com, aggr, uro);
   }

   const ThisState prevThisState = uro.vars.inner.thisState;

   // build "inside"
   if (leftFirst.isKeyword(Keyword::kw_Inside)) {
      left.trimLeft();
      if (left.isEmpty() && uro.vars.inner.thisState != ThisState::ts_String) {
         throw SyntaxException(str(L"argumentless structure '", leftFirst.getOriginString(uro),
            L"' can be created only inside a loop iterating over strings"), leftFirst.line);
      }
      left.checkCommonExpressionExceptions(uro);

      if (rightLen == 0) {
         return nullptr;
      }

      Tokens right(tks, rightStart, rightLen);
      Command* c = parseIterationLoop(true, left, right, prevThisState, uro);

      if (c != nullptr) {
         return c;
      }

      throw SyntaxException(str(L"keyword '", leftFirst.getOriginString(uro), L"' is not followed by a valid "
         L"declaration of string or list"), leftFirst.line);
   }

   // build "if"
   if (leftFirst.isKeyword(Keyword::kw_If)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxException(str(L"keyword '", leftFirst.getOriginString(uro), L"' is not followed by a condition"),
            leftFirst.line);
      }

      left.checkCommonExpressionExceptions(uro);
      _genptr<_bool> boo;
      if (!parse::parse(uro, left, boo)) {
         throw SyntaxException(str(L"keyword '", leftFirst.getOriginString(uro), L"' is not followed by a valid condition"),
            leftFirst.line);
      }

      CS_Condition* cond = new CS_Condition();

      if (rightLen == 0) {
         cond->setMain(boo);
         uro.conditionContext.addClosed(cond);
         return cond;
      }

      Tokens right(tks, rightStart, rightLen);
      com = parseCommandsAsMember(right, cond, uro);

      if (com != nullptr) {
         cond->setMain(com, boo);
      }

      return cond;
   }

   if (leftFirst.isKeyword(Keyword::kw_Else)) {
      left.trimLeft();

      if (left.isEmpty()) { // build "else"
         if (rightLen == 0) {
            uro.conditionContext.addEmptyElse(leftFirst.line);
            return nullptr;
         }

         Tokens right(tks, rightStart, rightLen);
         com = parseCommandsAsMember(right, nullptr, uro);
         uro.conditionContext.deleteLast();

         if (com != nullptr) {
            uro.conditionContext.addElse(com, leftFirst.line);
         }
      }
      else { // build "else if"
         if (!left.first().isKeyword(Keyword::kw_If)) {
            throw SyntaxException(str(L"keyword '", leftFirst.getOriginString(uro),
               L"' cannot be followed by an expression"), leftFirst.line);
         }

         const Token& ifToken = left.first();
         left.trimLeft();

         if (left.isEmpty()) {
            throw SyntaxException(str(L"keywords '", leftFirst.getOriginString(uro), L" ",
               ifToken.getOriginString(uro), L"' are not followed by a condition"), leftFirst.line);
         }

         left.checkCommonExpressionExceptions(uro);
         _genptr<_bool> boo;
         if (!parse::parse(uro, left, boo)) {
            throw SyntaxException(str(L"keywords '", leftFirst.getOriginString(uro), L" ",
               ifToken.getOriginString(uro), L"' are not followed by a valid condition"), leftFirst.line);
         }

         if (rightLen == 0) {
            com = new C_DoNothing();
            uro.conditionContext.addElseIf(boo, com, leftFirst.line);
            return nullptr;
         }

         Tokens right(tks, rightStart, rightLen);
         com = parseCommandsAsMember(right, nullptr, uro);
         uro.conditionContext.deleteLast();

         if (com != nullptr) {
            uro.conditionContext.addElseIf(boo, com, leftFirst.line);
         }
      }

      return nullptr;
   }

   if (rightLen == 0) {
      return nullptr;
   }

   left.checkCommonExpressionExceptions(uro);
   Tokens right(tks, rightStart, rightLen);

   // time list loop
   _genptr<_tlist> tlist;
   if (parse::parse(uro, left, tlist)) {
      uro.vars.inner.thisState = ThisState::ts_Time;
      Aggregate* aggr;

      return parseLoopBase(com, right, uro, prevThisState, aggr)
         ? new CS_TimeLoop(tlist, com, aggr, uro)
         : nullptr;
   }

   // number list loop
   _genptr<_nlist> nlist;
   if (parse::parse(uro, left, nlist)) {
      uro.vars.inner.thisState = ThisState::ts_Number;
      Aggregate* aggr;

      return parseLoopBase(com, right, uro, prevThisState, aggr)
         ? new CS_NumberLoop(nlist, com, aggr, uro)
         : nullptr;
   }

   Command* c = parseIterationLoop(false, left, right, prevThisState, uro);
   if (c != nullptr) {
      return c;
   }

   throw SyntaxException(L"tokens before { bracket do not form any valid syntax structure", tks.first().line);
}

static Command* parseIterationLoop(const _bool& isInside, const Tokens& left, const Tokens& right,
   const ThisState& prevState, Uroboros& uro)
{
   Command* com = nullptr;

   if (isInside && left.isEmpty()) {
      _genptr<_str> tr;
      uro.vars.inner.createThisRef(tr);
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      if (parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         return new CS_InsideString(tr, com, attr, aggr, hasMemory, uro);
      }
      else {
         return nullptr;
      }
   }

   // string loop
   _genptr<_str> str;
   if (parse::parse(uro, left, str)) {
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      if (parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         if (isInside) {
            return new CS_InsideString(str, com, attr, aggr, hasMemory, uro);
         }
         else {
            return new CS_StringLoop(str, com, attr, aggr, hasMemory, uro);
         }
      }
      else {
         return nullptr;
      }
   }

   // definition loop
   _defptr def;
   if (parse::parse(uro, left, def)) {
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      if (!parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         return nullptr;
      }

      if (attr->isMarkedToEvaluate()) {
         _genptr<_list> g(new gen::Cast_D_L(def, uro));

         if (isInside) {
            return new CS_InsideList(g, com, attr, aggr, hasMemory, uro);
         }
         else {
            return new CS_ListLoop(g, com, attr, aggr, hasMemory, uro);
         }
      }
      else {
         _fdata* fdata = def->getDataPtr();

         if (fdata == nullptr) {
            if (isInside) {
               return new CS_InsideDefinition(def, com, attr, aggr, hasMemory, uro);
            }
            else {
               return new CS_DefinitionLoop(def, com, attr, aggr, hasMemory, uro);
            }
         }
         else {
            const _aunit aval = attr->getValue();
            delete attr;

            if (isInside) {
               return new CS_InsideDefinition(def, com,
                  new BridgeAttribute(aval, uro, fdata), aggr, hasMemory, uro);
            }
            else {
               return new CS_DefinitionLoop(def, com,
                  new BridgeAttribute(aval, uro, fdata), aggr, hasMemory, uro);
            }
         }
      }
   }

   // list loop
   _genptr<_list> lst;
   if (parse::parse(uro, left, lst)) {
      uro.vars.inner.thisState = ThisState::ts_String;
      _bool hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      if (!parseLoopBase(com, right, uro, prevState, attr, aggr, hasMemory)) {
         return nullptr;
      }

      if (isInside) {
         return new CS_InsideList(lst, com, attr, aggr, hasMemory, uro);
      }
      else {
         return new CS_ListLoop(lst, com, attr, aggr, hasMemory, uro);
      }
   }

   return nullptr;
}

static _bool parseLoopBase(Command*& com, const Tokens& rightTokens, Uroboros& uro,
   const ThisState& prevState, Attribute*& attr, Aggregate*& aggr, _bool& hasMemory)
{
   hasMemory = uro.vc.anyAttribute();
   attr = new Attribute(uro);
   uro.vc.addAttribute(attr);
   aggr = new Aggregate(uro);
   uro.vc.addAggregate(aggr);

   com = parseCommandsAsMember(rightTokens, nullptr, uro);

   uro.vars.inner.thisState = prevState;
   uro.vc.retreatAttribute();
   uro.vc.retreatAggregate();

   const _bool success = com != nullptr;
   if (!success) {
      delete attr;
      delete aggr;
   }

   return success;
}

static _bool parseLoopBase(Command*& com, const Tokens& rightTokens, Uroboros& uro,
   const ThisState& prevState, Aggregate*& aggr)
{
   aggr = new Aggregate(uro);
   uro.vc.addAggregate(aggr);

   com = parseCommandsAsMember(rightTokens, nullptr, uro);

   uro.vars.inner.thisState = prevState;
   uro.vc.retreatAggregate();

   const _bool success = com != nullptr;
   if (!success) {
      delete aggr;
   }

   return success;
}

static Command* parseCommandsAsMember(const Tokens& tks, CS_Condition* cond, Uroboros& uro)
{
   uro.conditionContext.add(cond);
   uro.vars.varsLevelUp();
   Command* com = parseCommands(tks, uro);
   uro.vars.varsLevelDown();
   uro.conditionContext.deleteClosedUnits();
   return com;
}

static Command* command(Tokens& tks, Uroboros& uro)
{
   const Token& f = tks.first();

   if (tks.getLength() == 1 && f.type == Token::t_Keyword) {
      switch (f.value.keyword.k) {
         case Keyword::kw_Break:
         case Keyword::kw_Continue: {
            if (uro.vc.anyAggregate()) {
               if (f.value.keyword.k == Keyword::kw_Break) {
                  return new C_Break(uro);
               }
               else {
                  return new C_Continue(uro);
               }
            }
            else {
               throw SyntaxException(str(L"command '", f.getOriginString(uro),
                  L"' can be called only inside a loop"), f.line);
            }
            break;
         }
         case Keyword::kw_Exit: {
            return new C_Exit(uro);
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
               throw SyntaxException(str(L"command cannot consist of only one keyword: '",
                  f2.getOriginString(uro),L"'"), f.line);
            }
            break;
         }
         case Keyword::kw_Stack: {
            tks.trimLeft();
            stack = true;

            if (tks.isEmpty()) {
               throw SyntaxException(str(L"command cannot consist of only one keyword: '",
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
            return keywordCommands(f3, tks, f.line, force, stack, uro);
         }
      }
   }

   if (force) {
      throw SyntaxException(L"only a core command can start with a keyword 'force'", f.line);
   }
   if (stack) {
      throw SyntaxException(L"only a core command can start with a keyword 'stack'", f.line);
   }

   Command* misc = commandMisc(tks, uro);

   if (misc == nullptr) {
      tks.checkCommonExpressionExceptions(uro);
      return c_print(Token(Keyword::kw_Print, f.line, static_cast<_size>(0), static_cast<_size>(0), uro), tks, f.line, false, uro);
   }
   else {
      return misc;
   }
}

static Command* commandMisc(const Tokens& tks, Uroboros& uro)
{
   if (tks.check(TI_HAS_CHAR_EQUALS)) {
      std::pair<Tokens, Tokens> pair = tks.divideBySymbol(L'=');
      Tokens& left = pair.first;
      Tokens& right = pair.second;
      right.checkCommonExpressionExceptions(uro);
      
      if (left.isEmpty()) {
         if (right.isEmpty()) {
            throw SyntaxException(
               L"left and right side of the = operator are empty",
               tks.first().line);
         }
         else {
            throw SyntaxException(L"left side of the = operator is empty",
               tks.first().line);
         }
      }
      if (right.isEmpty()) {
         throw SyntaxException(L"right side of the = operator is empty",
            tks.last().line);
      }

      if (right.first().isSymbol(L'=')) {
         return nullptr;
      }

      const Token& leftLast = left.last();
      if (leftLast.type == Token::t_Symbol) {
         const char& ch = leftLast.value.ch;
         switch (ch) {
            case L'+':
            case L'-':
            case L'*':
            case L'/':
            case L'%': {
               left.trimRight();
               if (left.isEmpty()) {
                  throw SyntaxException(str(L"left side of the ", toStr(ch),
                     L"= operator is empty"), tks.last().line);
               }

               return commandVarChange(left, right, ch, uro);
            }
         }
      }

      if (varSquareBrackets(left)) {
         return commandVarAssign_Element(left, right, uro);
      }
      else {
         return commandVarAssign(left, right, uro);
      }
   }

   const Token& last = tks.last();

   if (last.type == Token::t_MultiSymbol &&
       (last.value.chars.ch == L'+' || last.value.chars.ch == L'-'))
   {
      const _bool isIncrement = last.value.chars.ch == L'+';

      const _str op = isIncrement
         ? L"incremented by one"
         : L"decremented by one";
      const Token& first = tks.first();

      if (first.type == Token::t_Word) {
         if (tks.getLength() == 2) {
            vars::ParseVariable<_num>* pv_num;

            if (!uro.vars.getVarPtr(first, pv_num)) {
               throw SyntaxException(str(L"variable '", first.getOriginString(uro),
                  L"' cannot be ", op), first.line);
            }

            pv_num->makeNotConstant();

            if (isIncrement) {
               return new VarIncrement(pv_num->getVarPtr());
            }
            else {
               return new VarDecrement(pv_num->getVarPtr());
            }
         }
         else {
            Tokens tks2(tks);
            tks2.trimRight();
            tks2.trimRight();

            if (varSquareBrackets(tks2)) {
               _genptr<_num> index;
               parseListElementIndex(index, tks2, uro);
               vars::ParseVariable<_nlist>* pv_nlist;

               if (!uro.vars.getVarPtr(first, pv_nlist)) {
                  throw SyntaxException(str(L"variable '", first.getOriginString(uro),
                     L"' cannot be ", op), first.line);
               }

               throw SyntaxException(str(L"an element of variable '", first.getOriginString(uro),
                  L"' cannot be ", op, L", because collections in Uroboros are immutable"),
                  first.line);
            }
            else {
               throw SyntaxException(str(L"this structure cannot be ", op), first.line);
            }
         }
      }
      else if (first.type == Token::t_TwoWords && tks.getLength() == 2) {
         if (first.value.twoWords.h1 == uro.hashes.HASH_NOTHING) {
            throw SyntaxException(L"the dot . should be preceded by a time variable name", first.line);
         }

         vars::ParseVariable<_tim>* pv_tim;

         if (!uro.vars.getVarPtr(first, pv_tim)) {
            throw SyntaxException(str(L"time variable from expression '", first.getOriginString(uro),
               L"' does not exist or is unreachable here"), first.line);
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
            throw SyntaxException(str(L"time variable member '", first.getOriginString_2(uro),
               L"' cannot be ", op), first.line);
         }
         else {
            parse::timeVariableMemberException(first, uro);
         }

         pv_tim->makeNotConstant();

         if (isIncrement) {
            return new VarTimeUnitIncrement(pv_tim->getVarPtr(), unit);
         }
         else {
            return new VarTimeUnitDecrement(pv_tim->getVarPtr(), unit);
         }
      }
      else {
         throw SyntaxException(str(L"only a variable of a singular data type can be ", op), first.line);
      }
   }

   return nullptr;
}

static Command* commandVarChange(const Tokens& left, const Tokens& right,
   const _char& sign, Uroboros& uro)
{
   const Token& first = left.first();

   if (left.getLength() > 1) {
      if (varSquareBrackets(left)) {
         vars::ParseVariable<_list>* pv_list;
         vars::ParseVariable<_nlist>* pv_nlist;
         vars::ParseVariable<_tlist>* pv_tlist;

         if (uro.vars.getVarPtr(first, pv_list)
          || uro.vars.getVarPtr(first, pv_nlist)
          || uro.vars.getVarPtr(first, pv_tlist))
         {
            throw SyntaxException(str(L"collection variable '", first.getOriginString(uro),
               L"' is immutable, so its elements cannot be modified"), right.first().line);
         }

         vars::ParseVariable<_str>* pv_str;

         if (uro.vars.getVarPtr(first, pv_str)) {
            throw SyntaxException(str(L"operation ", toStr(sign),
               L"= cannot be performed on a character from string variable"), first.line);
         }

         throw SyntaxException(str(first.getOriginString(uro),
            L" is not a collection variable, which are expected before [] brackets"), first.line);
      }
      else {
         const Token& arom = left.last();

         if (arom.type == Token::t_TwoWords && arom.value.twoWords.h1 == uro.hashes.HASH_NOTHING) {
            Tokens aro(left);
            aro.trimRight();

            if (varSquareBrackets(aro)) {
               vars::ParseVariable<_tlist>* pv_tlist;
               if (uro.vars.getVarPtr(first, pv_tlist)) {
                  throw SyntaxException(str(L"operation ", toStr(sign),
                     L"= cannot be performed on a time list variable member. Collections in Uroboros are immutable"), first.line);
               }
            }
         }

         throw SyntaxException(str(L"operator ", toStr(sign),
            L"= should be preceded by a variable name"), first.line);
      }
   }

   if (first.type == Token::t_Word) {
      vars::ParseVariable<_num>* pv_num;
      if (uro.vars.getVarPtr(first, pv_num)) {
         _genptr<_num> num;

         if (!parse::parse(uro, right, num)) {
            throw SyntaxException(str(L"right side of operator ", toStr(sign),
               L"= cannot be resolved to a number"), first.line);
         }

         vars::Variable<_num>* var = pv_num->getVarPtr();
         pv_num->makeNotConstant();

         switch (sign) {
            case L'+':
               return new VarAdd_<_num>(var, num);
            case L'-':
               return new VarSubtract<_num>(var, num);
            case L'*':
               return new VarNumMultiply(var, num);
            case L'/':
               return new VarNumDivide(var, num);
            case L'%':
               return new VarModulo(var, num);
         }
      }

      vars::ParseVariable<_per>* pv_per;
      if (uro.vars.getVarPtr(first, pv_per)) {
         vars::Variable<_per>* var = pv_per->getVarPtr();

         switch (sign) {
            case L'+':
            case L'-': {
               _genptr<_per> per;

               if (!parse::parse(uro, right, per)) {
                  throw SyntaxException(str(L"right side of operator ", toStr(sign),
                     L"= cannot be resolved to a period"), first.line);
               }

               pv_per->makeNotConstant();

               if (sign == L'+')
                  return new VarAdd_<_per>(var, per);
               else
                  return new VarSubtract<_per>(var, per);
            }
            case L'*':
            case L'/':
            case L'%': {
               throw SyntaxException(str(L"operation ",  toStr(sign),
                  L"= cannot be performed on a period"), first.line);
            }
         }
      }

      vars::ParseVariable<_tim>* pv_tim;
      if (uro.vars.getVarPtr(first, pv_tim)) {
         switch (sign) {
            case L'*':
            case L'/':
            case L'%': {
               throw SyntaxException(str(L"operation ", toStr(sign),
                  L"= is not valid for a time variable"), first.line);
            }
         }

         vars::Variable<_tim>* var = pv_tim->getVarPtr();
         _genptr<_per> per;

         if (!parse::parse(uro, right, per)) {
            throw SyntaxException(str(L"right side of operator '", first.getOriginString(uro),
               L" ", toStr(sign), L"=' cannot be resolved to a period"), first.line);
         }

         pv_tim->makeNotConstant();

         if (sign == L'+') {
            return new VarTimeAdd(var, per);
         }
         else {
            return new VarTimeSubtract(var, per);
         }
      }

      if (sign == L'+') {
         return commandVarIncrement(first, right, first.line, uro);
      }

      throw SyntaxException(str(L"'", first.getOriginString(uro),
         L"' is neither a numeric, time, nor period variable"), first.line);
   }
   else if (first.type == Token::t_TwoWords) {
      switch (sign) {
         case L'*':
         case L'/':
         case L'%': {
            throw SyntaxException(str(L"operation ", toStr(sign),
               L"= is not valid for a time variable"), first.line);
         }
      }

      vars::ParseVariable<_tim>* pv_tim;
      if (!uro.vars.getVarPtr(first, pv_tim)) {
         throw SyntaxException(str(L"'", first.getOriginString(uro),
            L"' is not a time variable for ", toStr(sign), L"= operation"),
            first.line);
      }

      _genptr<_num> num;

      if (!parse::parse(uro, right, num)) {
         throw SyntaxException(str(L"right side of operation '", first.getOriginString(uro), L".",
            first.getOriginString_2(uro), L" ", toStr(sign),
            L"=' cannot be resolved to a number"), first.line);
      }

      const _size& h = first.value.twoWords.h2;
      vars::Variable<_tim>* var = pv_tim->getVarPtr();
      const bool negative = (sign == '-');
      pv_tim->makeNotConstant();

      if (h == uro.hashes.HASH_PER_YEAR || h == uro.hashes.HASH_PER_YEARS)
         return new VarTimeUnitChange(var, num, Period::u_Years, negative);
      else if (h == uro.hashes.HASH_PER_MONTH || h == uro.hashes.HASH_PER_MONTHS)
         return new VarTimeUnitChange(var, num, Period::u_Months, negative);
      else if (h == uro.hashes.HASH_PER_DAY || h == uro.hashes.HASH_PER_DAYS)
         return new VarTimeUnitChange(var, num, Period::u_Days, negative);
      else if (h == uro.hashes.HASH_PER_HOUR || h == uro.hashes.HASH_PER_HOURS)
         return new VarTimeUnitChange(var, num, Period::u_Hours, negative);
      else if (h == uro.hashes.HASH_PER_MINUTE || h == uro.hashes.HASH_PER_MINUTES)
         return new VarTimeUnitChange(var, num, Period::u_Minutes, negative);
      else if (h == uro.hashes.HASH_PER_SECOND || h == uro.hashes.HASH_PER_SECONDS)
         return new VarTimeUnitChange(var, num, Period::u_Seconds, negative);
      else if (h == uro.hashes.HASH_PER_DATE || h == uro.hashes.HASH_PER_WEEKDAY) {
         throw SyntaxException(str(L"value of '", first.getOriginString_2(uro),
            L"' time variable member cannot be altered"), first.line);
      }

      parse::timeVariableMemberException(first, uro);
   }
   else {
      throw SyntaxException(str(L"operator ", toStr(sign),
         L"= has to be preceded by a single word, which is a variable name"),
         first.line);
   }

   return nullptr;
}

static Command* commandVarIncrement(const Token& first, const Tokens& tks,
   const _int& line, Uroboros& uro)
{
   vars::ParseVariable<_str>* pv_str;
   if (uro.vars.getVarPtr(first, pv_str)) {
      vars::Variable<_str>* var = pv_str->getVarPtr();

      _genptr<_str> str_;
      if (parse::parse(uro, tks, str_)) {
         pv_str->makeNotConstant();
         return new VarAdd_<_str>(var, str_);
      }

      _genptr<_list> list;

      if (parse::parse(uro, tks, list)) {
         throw SyntaxException(str(L"variable '", first.getOriginString(uro),
            L"' can be incremented only by a string"), line);
      }
      else {
         throw SyntaxException(L"right side of operator '+=' cannot"
            L" be resolved to a string", line);
      }
   }

   throw SyntaxException(str(L"variable '", first.getOriginString(uro),
      L"' cannot be incremented by a value"), line);
}

template <typename T>
static _bool makeVarAlteration(Uroboros& uro, const Tokens& tokens, const Token& first,
   vars::ParseVariable<T>*& varPtr, Command*& result, const _str& dataTypeName)
{
   if (uro.vars.getVarPtr(first, varPtr) && varPtr->isReachable()) {
      _genptr<T> value;
      if (parse::parse(uro, tokens, value)) {
         if (!value->isConstant()) {
            varPtr->makeNotConstant();
         }
         result = new VarAssignment<T>(varPtr->getVarPtr(), value);
         if (varPtr->var.isConstant()) {
            varPtr->var.value = value->getValue();
         }
         return true;
      }
      else {
         throw SyntaxException(str(L"value assigned to variable '", first.getOriginString(uro),
            L"' has to be of ", dataTypeName, L" type"), first.line);
      }
   }

   return false;
}

template <typename T>
static Command* makeVarAssignment(const Token& token, Uroboros& uro,
   vars::ParseVariable<T>* varPtr, _genptr<T>& valuePtr)
{
   vars::VarBundle<T>* bundle;
   uro.vars.takeBundlePointer(bundle);
   const _bool isConstant = !uro.vc.anyAggregate() && valuePtr->isConstant();
   return bundle->makeVariableAssignment(token, varPtr, valuePtr, isConstant);
}

static Command* commandVarAssign(const Tokens& left, const Tokens& right, Uroboros& uro)
{
   const Token& first = left.first();

   if (first.type != Token::t_Word) {
      return nullptr;
   }

   if (left.getLength() >= 5 ) {
      if (left.last().type == Token::t_TwoWords && left.last().value.word.h == uro.hashes.HASH_NOTHING) {
         Tokens le(left);
         le.trimRight();

         if (varSquareBrackets(le)) {
            vars::ParseVariable<_nlist>* pv_nlist;
            vars::ParseVariable<_tlist>* pv_tlist;
            vars::ParseVariable<_list>* pv_list;

            if (uro.vars.getVarPtr(first, pv_nlist) ||
                uro.vars.getVarPtr(first, pv_tlist) ||
                uro.vars.getVarPtr(first, pv_list))
            {
               throw SyntaxException(str(L"collection variable '", first.getOriginString(uro),
                  L"' is immutable, so its elements cannot me modified"), first.line);
            }
            else {
               throw SyntaxException(str(L"unknown collection variable '", first.getOriginString(uro),
                  L"'. Collection variables are immutable in Uroboros anyways"), first.line);
            }
         }
      }
   }

   if (left.getLength() != 1) {
      return nullptr;
   }

   /////
   // assign value to an existing variable
   ////

   if (uro.hashes.HASH_GROUP_INNERVAR.find(first.value.word.h) != uro.hashes.HASH_GROUP_INNERVAR.end()) {
      throw SyntaxException(str(L"variable '", first.getOriginString(uro), L"' is immutable"), first.line);
   }

   Command* varAlteration;

   vars::ParseVariable<_bool>* pv_boo = nullptr;
   if (makeVarAlteration(uro, right, first, pv_boo, varAlteration, L"bool")) {
      return varAlteration;
   }

   vars::ParseVariable<_num>* pv_num = nullptr;
   if (makeVarAlteration(uro, right, first, pv_num, varAlteration, L"number")) {
      return varAlteration;
   }

   vars::ParseVariable<_tim>* pv_tim = nullptr;
   if (makeVarAlteration(uro, right, first, pv_tim, varAlteration, L"time")) {
      return varAlteration;
   }

   vars::ParseVariable<_per>* pv_per = nullptr;
   if (makeVarAlteration(uro, right, first, pv_per, varAlteration, L"period")) {
      return varAlteration;
   }

   vars::ParseVariable<_str>* pv_str = nullptr;
   if (makeVarAlteration(uro, right, first, pv_str, varAlteration, L"string")) {
      return varAlteration;
   }

   vars::ParseVariable<_nlist>* pv_nlist = nullptr;
   if (makeVarAlteration(uro, right, first, pv_nlist, varAlteration, L"numeric list")) {
      return varAlteration;
   }

   vars::ParseVariable<_tlist>* pv_tlist = nullptr;
   if (makeVarAlteration(uro, right, first, pv_tlist, varAlteration, L"time list")) {
      return varAlteration;
   }

   vars::ParseVariable<_list>* pv_list = nullptr;
   if (makeVarAlteration(uro, right, first, pv_list, varAlteration, L"list")) {
      return varAlteration;
   }

   /////
   // create a new variable
   // or "resurrect" an existing variable from somewhere else out of reach
   ////

   _genptr<_bool> boo;
   if (parse::parse(uro, right, boo)) {
      return makeVarAssignment(first, uro, pv_boo, boo);
   }

   _genptr<_num> num;
   if (parse::parse(uro, right, num)) {
      return makeVarAssignment(first, uro, pv_num, num);
   }

   _genptr<_tim> tim;
   if (parse::parse(uro, right, tim)) {
      return makeVarAssignment(first, uro, pv_tim, tim);
   }

   _genptr<_per> per;
   if (parse::parse(uro, right, per)) {
      return makeVarAssignment(first, uro, pv_per, per);
   }

   _genptr<_str> str_;
   if (parse::parse(uro, right, str_)) {
      return makeVarAssignment(first, uro, pv_str, str_);
   }

   _genptr<_nlist> nlist;
   if (parse::parse(uro, right, nlist)) {
      return makeVarAssignment(first, uro, pv_nlist, nlist);
   }

   _genptr<_tlist> tlist;
   if (parse::parse(uro, right, tlist)) {
      return makeVarAssignment(first, uro, pv_tlist, tlist);
   }

   _genptr<_list> list;
   if (parse::parse(uro, right, list)) {
      return makeVarAssignment(first, uro, pv_list, list);
   }

   throw SyntaxException(str(L"value assigned to variable '", first.getOriginString(uro),
      L"' cannot be resolved to any data type"), first.line);
}

static bool varSquareBrackets(const Tokens& tks)
{
   const _int length = tks.getLength();
   if (length < 3 || !tks.second().isSymbol(L'[') || !tks.last().isSymbol(L']')) {
      return false;
   }

   if (tks.first().type != Token::t_Word) {
      throw SyntaxException(L"brackets [] have to be preceded by a collection variable name",
         tks.first().line);
   }

   if (length == 3) {
      throw SyntaxException(L"empty space between [] brackets",
         tks.second().line);
   }

   return true;
}

static Command* commandVarAssign_Element(const Tokens& left,
   const Tokens& right, Uroboros& uro)
{
   const Token& first = left.first();

   vars::ParseVariable<_list>* pv_list;
   vars::ParseVariable<_nlist>* pv_nlist;
   vars::ParseVariable<_tlist>* pv_tlist;

   if (uro.vars.getVarPtr(first, pv_list) ||
       uro.vars.getVarPtr(first, pv_nlist) ||
       uro.vars.getVarPtr(first, pv_tlist))
   {
      throw SyntaxException(str(L"collection variable '", first.getOriginString(uro),
         L"' is immutable, so its elements cannot me modified"), first.line);
   }

   vars::ParseVariable<_str>* pv_str;
   if (uro.vars.getVarPtr(first, pv_str)) {
      if (pv_str->isReachable()) {
         _genptr<_num> index;
         parseListElementIndex(index, left, uro);
         _genptr<_str> str_;

         if (parse::parse(uro, right, str_)) {
            pv_str->makeNotConstant();
            return new VarCharAssignment(pv_str->getVarPtr(), str_, index);
         }
         else {
            throw SyntaxException(str(L"new value in character assignment of variable '",
               first.getOriginString(uro), L"' cannot be resolved to a string"), first.line);
         }
      }
      else {
         throw SyntaxException(str(L"variable '", first.getOriginString(uro),
            L"' is unreachable here"), first.line);
      }
   }

   throw SyntaxException(str(L"variable '", first.getOriginString(uro),
      L"' was not expected before [] brackets"), first.line);
}

static _bool parseListElementIndex(_genptr<_num>& result, const Tokens& tks, Uroboros& uro)
{
   const _size start = tks.getStart() + 2;
   const _size length = tks.getLength() - 3;
   const Tokens tks2(tks, start, length);
   _genptr<_num> index;

   if (!parse::parse(uro, tks2, index)) {
      throw SyntaxException(
        L"content of square brackets [] cannot be resolved to a number",
        tks.second().line);
   }

   result = std::move(index);
   return true;
}

static void checkNoSemicolonBeforeBrackets(const Tokens& tks, Uroboros& uro)
{
   const _int end = tks.getEnd();
   const _int start = tks.getStart() + 1;
   const _bool startsWithElse = tks.listAt(start - 1).isKeyword(Keyword::kw_Else);

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Keyword) {
         switch (t.value.keyword.k) {
            case Keyword::kw_While:
            case Keyword::kw_Inside: {
               throw SyntaxException(str(L"a semicolon ; is missing before keyword '",
                  t.getOriginString(uro), L"'"), t.line);
            }
            case Keyword::kw_If: {
               if (!(i == start && startsWithElse)) {
                  throw SyntaxException(str(L"a semicolon ; is missing before keyword '",
                     t.getOriginString(uro), L"'"), t.line);
               }
               break;
            }
            case Keyword::kw_Else: {
               throw SyntaxException(str(L"keyword '", t.getOriginString(uro),
                  L"' should be preceded by curly brackets {}"), t.line);
            }
         }
      }
   }
}

}
