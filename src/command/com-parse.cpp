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


Command* parseCommands(const Tokens& tks, Uroboros* uro)
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
                     lockLastIf();
                     Tokens tks2(tks.list, i - sublen, sublen);
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
      Tokens tks2(tks.list, 1 + end - sublen, sublen);
      commands.push_back(command(tks2, uro));
   }

   if (commands.size() == 1) {
      return commands[0];
   }
   else {
      return new CS_Block(commands, uro);
   }
}

// can return nullptr
static Command* commandStruct(const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open, Uroboros* uro)
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

      Tokens tks2(tks.list, rightStart, rightLen);
      beforeCommandStruct(nullptr, uro);
         com = parseCommands(tks2, uro);
      afterCommandStruct(uro);
      return com;
   }

   // build "times"
   Tokens left(tks.list, leftStart, leftLen);
   const Token& leftLast = left.last();
   if (leftLast.isKeyword(Keyword::kw_Times)) {
      left.trimRight();
      if (left.isEmpty()) {
         throw SyntaxException(str(L"keyword '", *leftLast.value.keyword.os,
            L"' is not preceded by a number"), leftLast.line);
      }

      if (rightLen == 0) {
         return nullptr;
      }

      Generator<_num>* num;
      if (!parse(uro, left, num)) {
         throw SyntaxException(str(L"keyword '", *leftLast.value.keyword.os,
            L"' is not preceded by a valid number"), leftLast.line);
      }

      Tokens right(tks.list, rightStart, rightLen);

      Aggregate* aggr = new Aggregate(uro);
      uro->vc.addAggregate(aggr);
      beforeCommandStruct(nullptr, uro);
         com = parseCommands(right, uro);
      afterCommandStruct(uro);
      uro->vc.retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_Times(num, com, aggr, uro);
   }

   checkNoSemicolonBeforeBrackets(left);

   // build "while"
   const Token& leftFirst = left.first();
   if (leftFirst.isKeyword(Keyword::kw_While)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxException(str(L"keyword '", *leftFirst.value.keyword.os,
            L"' is not followed by a condition"), leftFirst.line);
      }

      if (rightLen == 0) {
         throw SyntaxException(str(L"structure '", *leftFirst.value.keyword.os,
            L"' is empty. It would either never run or cause an infinite loop"), leftFirst.line);
      }

      Generator<_boo>* boo;
      if (parse(uro, left, boo)) {
         throw SyntaxException(str(L"keyword '", *leftFirst.value.keyword.os,
            L"' is not followed by a valid condition"), leftFirst.line);
      }

      Tokens right(tks.list, rightStart, rightLen);

      Aggregate* aggr = new Aggregate(uro);
      uro->vc.addAggregate(aggr);
      beforeCommandStruct(nullptr, uro);
         com = parseCommands(right, uro);
      afterCommandStruct(uro);
      uro->vc.retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_While(boo, com, aggr, uro);
   }

   const ThisState prevThisState = uro->vars.inner.thisState;

   // build "inside"
   if (leftFirst.isKeyword(Keyword::kw_Inside)) {
      left.trimLeft();
      if (left.isEmpty() && uro->vars.inner.thisState != ThisState::ts_String) {
         throw SyntaxException(str(L"argumentless structure '", *leftFirst.value.keyword.os,
            L"' can be created only inside a loop iterating over strings"), leftFirst.line);
      }

      if (rightLen == 0) {
         return nullptr;
      }

      uro->vars.inner.thisState = ThisState::ts_String;
      Tokens right(tks.list, rightStart, rightLen);
      const _boo hasMemory = uro->vc.anyAttribute();
      Attribute* attr = new Attribute(uro);
      Aggregate* aggr = new Aggregate(uro);
      uro->vc.addAttribute(attr);
      uro->vc.addAggregate(aggr);
      beforeCommandStruct(nullptr, uro);
         com = parseCommands(right, uro);
      afterCommandStruct(uro);
      uro->vars.inner.thisState = prevThisState;
      uro->vc.retreatAttribute();
      uro->vc.retreatAggregate();

      if (com == nullptr) {
         return nullptr;
      }

      if (left.isEmpty()) {
         Variable<_str>* tr;
         uro->vars.inner.createThisReference(tr);
         return new CS_InsideString(tr, com, attr, aggr, hasMemory, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, left, str_)) {
         return new CS_InsideString(str_, com, attr, aggr, hasMemory, uro);
      }

      _def* def;
      if (parse(uro, left, def)) {
         if (attr->markToEvaluate) {
            return new CS_InsideList(new Cast_D_L(def, uro), com, attr, aggr, hasMemory, uro);
         }
         else {
            return new CS_InsideDefinition(def, com, attr, aggr, hasMemory, uro);
         }
      }

      Generator<_list>* list;
      if (parse(uro, left, list)) {
         throw SyntaxException(str(L"keyword '", *leftFirst.value.keyword.os, L"' is not followed by a valid "
            L"declaration of string or list"), leftFirst.line);
      }
      else {
         return new CS_InsideList(list, com, attr, aggr, hasMemory, uro);
      }
   }

   // build "if"
   if (leftFirst.isKeyword(Keyword::kw_If)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxException(str(L"keyword '", *leftFirst.value.keyword.os, L"' is not followed by a condition"),
            leftFirst.line);
      }

      Generator<_boo>* boo;
      if (parse(uro, left, boo)) {
         throw SyntaxException(str(L"keyword '", *leftFirst.value.keyword.os, L"' is not followed by a valid condition"),
            leftFirst.line);
      }

      CS_If* cif = new CS_If(boo, uro);

      if (rightLen == 0) {
         beforeCommandStruct(cif, uro);
         afterCommandStruct(uro);
         return cif;
      }

      Tokens right(tks.list, rightStart, rightLen);

      beforeCommandStruct(cif, uro);
         com = parseCommands(right, uro);
      afterCommandStruct(uro);

      if (com != nullptr) {
         cif->setMain(com);
      }

      return cif;
   }

   // build "else" and "else if"
   if (leftFirst.isKeyword(Keyword::kw_Else)) {
      left.trimLeft();

      if (left.isEmpty()) {
         if (rightLen == 0) {
            setEmptyElse(leftFirst.line);
            return nullptr;
         }

         Tokens right(tks.list, rightStart, rightLen);

         uro->vars.varsLevelUp();
            com = parseCommands(right, uro);
         uro->vars.varsLevelDown();

         if (com != nullptr) {
            setElse(com, leftFirst.line);
         }
      }
      else {
         if (!left.first().isKeyword(Keyword::kw_If)) {
            throw SyntaxException(str(L"keyword '", *leftFirst.value.keyword.os,
               L"' cannot be followed by an expression"), leftFirst.line);
         }
         const Token& ifToken = left.first();
         left.trimLeft();

         if (left.isEmpty()) {
            throw SyntaxException(str(L"keywords '", *leftFirst.value.keyword.os, L" ",
               *ifToken.value.keyword.os, L"' are not followed by a condition"), leftFirst.line);
         }

         Generator<_boo>* boo;
         if (parse(uro, left, boo)) {
            throw SyntaxException(str(L"keywords '", *leftFirst.value.keyword.os, L" ", 
               *ifToken.value.keyword.os, L"' are not followed by a valid condition"), leftFirst.line);
         }

         if (rightLen == 0) {
            addElseIf(boo, new C_DoNothing(), leftFirst.line);
            return nullptr;
         }

         Tokens right(tks.list, rightStart, rightLen);

         uro->vars.varsLevelUp();
            com = parseCommands(right, uro);
         uro->vars.varsLevelDown();

         if (com != nullptr) {
            addElseIf(boo, com, leftFirst.line);
         }
      }

      return nullptr;
   }

   if (rightLen == 0) {
      return nullptr;
   }

   Tokens right(tks.list, rightStart, rightLen);

   // time list loop
   Generator<_tlist>* tlist;
   if (parse(uro, left, tlist)) {
      uro->vars.inner.thisState = ThisState::ts_Time;
      Aggregate* aggr;

      return parseLoopBase(com, right, uro, prevThisState, aggr)
         ? new CS_TimeLoop(tlist, com, aggr, uro)
         : nullptr;
   }

   // number list loop
   Generator<_nlist>* nlist;
   if (parse(uro, left, nlist)) {
      uro->vars.inner.thisState = ThisState::ts_Number;
      Aggregate* aggr;

      return parseLoopBase(com, right, uro, prevThisState, aggr)
         ? new CS_NumberLoop(nlist, com, aggr, uro)
         : nullptr;
   }


   // string loop
   Generator<_str>* str;
   if (parse(uro, left, str)) {
      uro->vars.inner.thisState = ThisState::ts_String;
      _boo hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      return parseLoopBase(com, right, uro, prevThisState, attr, aggr, hasMemory)
         ? new CS_StringLoop(str, com, attr, aggr, hasMemory, uro)
         : nullptr;
   }

   // definition loop
   _def* def;
   if (parse(uro, left, def)) {
      uro->vars.inner.thisState = ThisState::ts_String;
      _boo hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      if (!parseLoopBase(com, right, uro, prevThisState, attr, aggr, hasMemory)) {
         return nullptr;
      }

      if (attr->markToEvaluate) {
         return new CS_ListLoop(new Cast_D_L(def, uro), com, attr, aggr, hasMemory, uro);
      }
      else {
         return new CS_DefinitionLoop(def, com, attr, aggr, hasMemory, uro);
      }
   }

   // list loop
   Generator<_list>* lst;
   if (parse(uro, left, lst)) {
      uro->vars.inner.thisState = ThisState::ts_String;
      _boo hasMemory;
      Attribute* attr;
      Aggregate* aggr;

      return parseLoopBase(com, right, uro, prevThisState, attr, aggr, hasMemory)
         ? new CS_ListLoop(lst, com, attr, aggr, hasMemory, uro)
         : nullptr;
   }

   throw SyntaxException(L"tokens before { bracket do not form and valid syntax structure", tks.first().line);
}

static _boo parseLoopBase(Command*& com, const Tokens& rightTokens, Uroboros* uro,
   const ThisState& prevState, Attribute*& attr, Aggregate*& aggr, _boo& hasMemory)
{
   hasMemory = uro->vc.anyAttribute();
   attr = new Attribute(uro);
   uro->vc.addAttribute(attr);
   aggr = new Aggregate(uro);
   uro->vc.addAggregate(aggr);

   beforeCommandStruct(nullptr, uro);
      com = parseCommands(rightTokens, uro);
   afterCommandStruct(uro);

   uro->vars.inner.thisState = prevState;
   uro->vc.retreatAttribute();
   uro->vc.retreatAggregate();

   const _boo success = com != nullptr;
   if (!success) {
      delete attr;
      delete aggr;
   }

   return success;
}

static _boo parseLoopBase(Command*& com, const Tokens& rightTokens, Uroboros* uro,
   const ThisState& prevState, Aggregate*& aggr)
{
   aggr = new Aggregate(uro);
   uro->vc.addAggregate(aggr);

   beforeCommandStruct(nullptr, uro);
      com = parseCommands(rightTokens, uro);
   afterCommandStruct(uro);

   uro->vars.inner.thisState = prevState;
   uro->vc.retreatAggregate();

   const _boo success = com != nullptr;
   if (!success) {
      delete aggr;
   }

   return success;
}

static void beforeCommandStruct(CS_If* pntr, Uroboros* uro)
{
   addIfParseUnit(pntr);
   uro->vars.varsLevelUp();
}

static void afterCommandStruct(Uroboros* uro)
{
   retreatIfParseUnit();
   uro->vars.varsLevelDown();
}

static Command* command(Tokens& tks, Uroboros* uro)
{
   const Token& f = tks.first();

   if (tks.getLength() == 1 && f.type == Token::t_Keyword) {
      switch (f.value.keyword.k) {
         case Keyword::kw_Break:
         case Keyword::kw_Continue: {
            if (uro->vc.anyAggregate()) {
               if (f.value.keyword.k == Keyword::kw_Break) {
                  return new C_Break(uro);
               }
               else {
                  return new C_Continue(uro);
               }
            }
            else {
               throw SyntaxException(str(L"command '", *f.value.keyword.os,
                  L"' can be called only inside a loop"), f.line);
            }
            break;
         }
         case Keyword::kw_Exit: {
            return new C_Exit(uro);
         }
      }
   }

   _boo force = false;
   _boo stack = false;

   const Token& f2 = tks.first();
   if (f2.type == Token::t_Keyword) {
      switch (f2.value.keyword.k) {
         case Keyword::kw_Force: {
            tks.trimLeft();
            force = true;

            if (tks.isEmpty()) {
               throw SyntaxException(str(L"command cannot consist of only one keyword: '",
                  *f2.value.keyword.os,L"'"), f.line);
            }
            break;
         }
         case Keyword::kw_Stack: {
            tks.trimLeft();
            stack = true;

            if (tks.isEmpty()) {
               throw SyntaxException(str(L"command cannot consist of only one keyword: '",
                  *f2.value.keyword.os,L"'"), f.line);
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
      Token pf(Keyword::kw_Print, f.line, L"print", uro);
      return c_print(pf, tks, f.line, false, uro);
   }
   else {
      return misc;
   }
}

static Command* commandMisc(const Tokens& tks, Uroboros* uro)
{
   if (tks.containsSymbol(PGCS_EQUALS)) {
      Tokens left(tks);
      Tokens right(tks);
      tks.divideBySymbol(L'=', left, right);

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
                  throw SyntaxException(str(L"left side of the ", charStr(ch),
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

   if (tks.penultimate().type == Token::t_Symbol
      && tks.last().type == Token::t_Symbol) {

      const _char& c1 = tks.penultimate().value.ch;
      const _char& c2 = tks.last().value.ch;
      const _boo isIncrement = (c1 == L'+' && c2 == L'+');
      const _boo isDecrement = (c1 == L'-' && c2 == L'-');

      if (isIncrement || isDecrement) {
         const _str op = isIncrement
            ? L"incremented by one"
            : L"decremented by one";
         const Token& first = tks.first();

         if (first.type == Token::t_Word) {
            if (tks.getLength() == 3) {
               ParseVariable<_num>* pv_num;

               if (!uro->vars.getVarPtr(first, pv_num)) {
                  throw SyntaxException(str(L"variable '", *first.value.word.os,
                     L"' cannot be ", op), first.line);
               }

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
                  Generator<_num>* index = parseListElementIndex(tks2, uro);
                  ParseVariable<_nlist>* pv_nlist;

                  if (!uro->vars.getVarPtr(first, pv_nlist)) {
                     throw SyntaxException(str(L"variable '", *first.value.word.os,
                        L"' cannot be ", op), first.line);
                  }

                  throw SyntaxException(str(L"an element of variable '", *first.value.word.os,
                     L"' cannot be ", op, L", because collections in Uroboros are immutable"),
                     first.line);
               }
               else {
                  throw SyntaxException(str(L"this structure cannot be ", op), first.line);
               }
            }
         }
         else if (first.type == Token::t_TwoWords) {
            if (first.value.twoWords.h1 == uro->hashes.HASH_NOTHING) {
               throw SyntaxException(L"the dot . should be preceded by a time variable name", first.line);
            }

            ParseVariable<_tim>* pv_tim;

            if (!uro->vars.getVarPtr(first, pv_tim)) {
               throw SyntaxException(str(L"time variable from expression '", *first.value.twoWords.os1,
                  L"' does not exist or is unreachable here"), first.line);
            }

            const _size& h = first.value.twoWords.h2;
            Period::PeriodUnit unit;

            if (h == uro->hashes.HASH_PER_YEAR || h == uro->hashes.HASH_PER_YEARS)
               unit = Period::u_Years;
            else if (h == uro->hashes.HASH_PER_MONTH || h == uro->hashes.HASH_PER_MONTHS)
               unit = Period::u_Months;
            else if (h == uro->hashes.HASH_PER_DAY || h == uro->hashes.HASH_PER_DAYS)
               unit = Period::u_Days;
            else if (h == uro->hashes.HASH_PER_HOUR || h == uro->hashes.HASH_PER_HOURS)
               unit = Period::u_Hours;
            else if (h == uro->hashes.HASH_PER_MINUTE || h == uro->hashes.HASH_PER_MINUTES)
               unit = Period::u_Minutes;
            else if (h == uro->hashes.HASH_PER_SECOND || h == uro->hashes.HASH_PER_SECONDS)
               unit = Period::u_Seconds;
            else if (h == uro->hashes.HASH_PER_DATE || h == uro->hashes.HASH_PER_WEEKDAY) {
               throw SyntaxException(str(L"time variable member '", *first.value.twoWords.os2,
                  L"' cannot be ", op), first.line);
            }
            else {
               timeVariableMemberException(first);
            }

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
   }

   return nullptr;
}

static Command* commandVarChange(const Tokens& left, const Tokens& right,
   const _char& sign, Uroboros* uro)
{
   const Token& first = left.first();

   if (left.getLength() > 1) {
      if (varSquareBrackets(left)) {
         ParseVariable<_list>* pv_list;
         ParseVariable<_nlist>* pv_nlist;
         ParseVariable<_tlist>* pv_tlist;

         if (uro->vars.getVarPtr(first, pv_list)
          || uro->vars.getVarPtr(first, pv_nlist)
          || uro->vars.getVarPtr(first, pv_tlist))
         {
            throw SyntaxException(str(L"collection variable '", *first.value.word.os,
               L"' is immutable, so its elements cannot be modified"), right.first().line);
         }

         ParseVariable<_str>* pv_str;

         if (uro->vars.getVarPtr(first, pv_str)) {
            throw SyntaxException(str(L"operation ", charStr(sign),
               L"= cannot be performed on a character from string variable"), first.line);
         }

         throw SyntaxException(str(*first.value.word.os,
            L" is not a collection variable, which are expected before [] brackets"), first.line);
      }
      else {
         const Token& arom = left.last();

         if (arom.type == Token::t_TwoWords && arom.value.twoWords.h1 == uro->hashes.HASH_NOTHING) {
            Tokens aro(left);
            aro.trimRight();

            if (varSquareBrackets(aro)) {
               ParseVariable<_tlist>* pv_tlist;
               if (uro->vars.getVarPtr(first, pv_tlist)) {
                  throw SyntaxException(str(L"operation ", charStr(sign),
                     L"= cannot be performed on a time list variable member. Collections in Uroboros are immutable"), first.line);
               }
            }
         }

         throw SyntaxException(str(L"operator ", charStr(sign),
            L"= should be preceded by a variable name"), first.line);
      }
   }

   if (first.type == Token::t_Word) {
      ParseVariable<_num>* pv_num;
      if (uro->vars.getVarPtr(first, pv_num)) {
         Generator<_num>* num;

         if (!parse(uro, right, num)) {
            throw SyntaxException(str(L"right side of operator ", charStr(sign),
               L"= cannot be resolved to a number"), first.line);
         }

         Variable<_num>* var = pv_num->getVarPtr();

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

      ParseVariable<_per>* pv_per;
      if (uro->vars.getVarPtr(first, pv_per)) {
         Variable<_per>* var = pv_per->getVarPtr();

         switch (sign) {
            case L'+':
            case L'-': {
               Generator<_per>* per;

               if (!parse(uro, right, per)) {
                  throw SyntaxException(str(L"right side of operator ", charStr(sign),
                     L"= cannot be resolved to a period"), first.line);
               }

               if (sign == L'+')
                  return new VarAdd_<_per>(var, per);
               else
                  return new VarSubtract<_per>(var, per);
            }
            case L'*':
            case L'/':
            case L'%': {
               throw SyntaxException(str(L"operation ",  charStr(sign),
                  L"= cannot be performed on a period"), first.line);
            }
         }
      }

      ParseVariable<_tim>* pv_tim;
      if (uro->vars.getVarPtr(first, pv_tim)) {
         switch (sign) {
            case L'*':
            case L'/':
            case L'%': {
               throw SyntaxException(str(L"operation ", charStr(sign),
                  L"= is not valid for a time variable"), first.line);
            }
         }

         Variable<_tim>* var = pv_tim->getVarPtr();
         Generator<_per>* per;

         if (!parse(uro, right, per)) {
            throw SyntaxException(str(L"right side of operator '", *first.value.word.os, 
               L" ", charStr(sign), L"=' cannot be resolved to a period"), first.line);
         }

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

      throw SyntaxException(str(L"'", *first.value.word.os,
         L"' is neither a numeric, time, nor period variable"), first.line);
   }
   else if (first.type == Token::t_TwoWords) {
      switch (sign) {
         case L'*':
         case L'/':
         case L'%': {
            throw SyntaxException(str(L"operation ", charStr(sign),
               L"= is not valid for a time variable"), first.line);
         }
      }

      ParseVariable<_tim>* pv_tim;
      if (!uro->vars.getVarPtr(first, pv_tim)) {
         throw SyntaxException(str(L"'", *first.value.twoWords.os1,
            L"' is not a time variable for ", charStr(sign), L"= operation"),
            first.line);
      }

      Generator<_num>* num;

      if (!parse(uro, right, num)) {
         throw SyntaxException(str(L"right side of operation '", *first.value.twoWords.os1, L".",
            *first.value.twoWords.os2, L" ", charStr(sign),
            L"=' cannot be resolved to a number"), first.line);
      }

      const _size& h = first.value.twoWords.h2;
      Variable<_tim>* var = pv_tim->getVarPtr();
      const bool negative = (sign == '-');

      if (h == uro->hashes.HASH_PER_YEAR || h == uro->hashes.HASH_PER_YEARS)
         return new VarTimeUnitChange(var, num, Period::u_Years, negative);
      else if (h == uro->hashes.HASH_PER_MONTH || h == uro->hashes.HASH_PER_MONTHS)
         return new VarTimeUnitChange(var, num, Period::u_Months, negative);
      else if (h == uro->hashes.HASH_PER_DAY || h == uro->hashes.HASH_PER_DAYS)
         return new VarTimeUnitChange(var, num, Period::u_Days, negative);
      else if (h == uro->hashes.HASH_PER_HOUR || h == uro->hashes.HASH_PER_HOURS)
         return new VarTimeUnitChange(var, num, Period::u_Hours, negative);
      else if (h == uro->hashes.HASH_PER_MINUTE || h == uro->hashes.HASH_PER_MINUTES)
         return new VarTimeUnitChange(var, num, Period::u_Minutes, negative);
      else if (h == uro->hashes.HASH_PER_SECOND || h == uro->hashes.HASH_PER_SECONDS)
         return new VarTimeUnitChange(var, num, Period::u_Seconds, negative);
      else if (h == uro->hashes.HASH_PER_DATE || h == uro->hashes.HASH_PER_WEEKDAY) {
         throw SyntaxException(str(L"value of '", *first.value.twoWords.os2,
            L"' time variable member cannot be altered"), first.line);
      }

      timeVariableMemberException(first);
   }
   else {
      throw SyntaxException(str(L"operator ", charStr(sign),
         L"= has to be preceded by a single word, which is a variable name"),
         first.line);
   }

   return nullptr;
}

static Command* commandVarIncrement(const Token& first, const Tokens& tks,
   const _int& line, Uroboros* uro)
{
   ParseVariable<_str>* pv_str;
   if (uro->vars.getVarPtr(first, pv_str)) {
      Variable<_str>* var = pv_str->getVarPtr();

      Generator<_str>* str_;
      if (parse(uro, tks, str_)) {
         return new VarAdd_<_str>(var, str_);
      }

      Generator<_list>* list;
      if (parse(uro, tks, list)) {
         delete list;
         throw SyntaxException(str(L"variable '", *first.value.twoWords.os1,
            L"' can be incremented only by a string"), line);
      }
      else {
         throw SyntaxException(L"right side of operator '+=' cannot"
            L" be resolved to a string", line);
      }
   }

   throw SyntaxException(str(L"variable '", *first.value.twoWords.os1,
      L"' cannot be incremented by a value"), line);
}



static Command* commandVarAssign(const Tokens& left, const Tokens& right, Uroboros* uro)
{
   const Token& first = left.first();
   const _str& origin = *first.value.word.os;

   if (first.type != Token::t_Word) {
      return nullptr;
   }

   if (left.getLength() >= 5 ) {
      if (left.last().type == Token::t_TwoWords && left.last().value.word.h == uro->hashes.HASH_NOTHING) {
         Tokens le(left);
         le.trimRight();

         if (varSquareBrackets(le)) {
            ParseVariable<_nlist>* pv_nlist;
            ParseVariable<_tlist>* pv_tlist;
            ParseVariable<_list>* pv_list;

            if (uro->vars.getVarPtr(first, pv_nlist) ||
                uro->vars.getVarPtr(first, pv_tlist) ||
                uro->vars.getVarPtr(first, pv_list))
            {
               throw SyntaxException(str(L"collection variable '", origin,
                  L"' is immutable, so its elements cannot me modified"), first.line);
            }
            else {
               throw SyntaxException(str(L"unknown collection variable '", origin,
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

   if (uro->hashes.HASH_GROUP_INNERVAR.find(first.value.word.h) != uro->hashes.HASH_GROUP_INNERVAR.end()) {
      throw SyntaxException(str(L"variable '", origin, L"' is immutable"), first.line);
   }

   ParseVariable<_boo>* pv_boo = nullptr;
   if (uro->vars.getVarPtr(first, pv_boo) && pv_boo->isReachable()) {
      Generator<_boo>* boo;
      if (parse(uro, right, boo)) {
         return new VarAssignment<_boo>(pv_boo->getVarPtr(), boo);
      }
      else {
         varAssignException(origin, L"bool", right);
      }
   }

   ParseVariable<_num>* pv_num = nullptr;
   if (uro->vars.getVarPtr(first, pv_num) && pv_num->isReachable()) {
      Generator<_num>* num;
      if (parse(uro, right, num)) {
         return new VarAssignment<_num>(pv_num->getVarPtr(), num);
      }
      else {
         varAssignException(origin, L"number", right);
      }
   }

   ParseVariable<_tim>* pv_tim = nullptr;
   if (uro->vars.getVarPtr(first, pv_tim) && pv_tim->isReachable()) {
      Generator<_tim>* tim;
      if (parse(uro, right, tim)) {
         return new VarAssignment<_tim>(pv_tim->getVarPtr(), tim);
      }
      else {
         varAssignException(origin, L"time", right);
      }
   }

   ParseVariable<_per>* pv_per = nullptr;
   if (uro->vars.getVarPtr(first, pv_per) && pv_per->isReachable()) {
      Generator<_per>* per;
      if (parse(uro, right, per)) {
         return new VarAssignment<_per>(pv_per->getVarPtr(), per);
      }
      else {
         varAssignException(origin, L"period", right);
      }
   }

   ParseVariable<_str>* pv_str = nullptr;
   if (uro->vars.getVarPtr(first, pv_str) && pv_str->isReachable()) {
      Generator<_str>* str;
      if (parse(uro, right, str)) {
         return new VarAssignment<_str>(pv_str->getVarPtr(), str);
      }
      else {
         varAssignException(origin, L"string", right);
      }
   }

   ParseVariable<_nlist>* pv_nlist = nullptr;
   if (uro->vars.getVarPtr(first, pv_nlist) && pv_nlist->isReachable()) {
      Generator<_nlist>* nlist;
      if (parse(uro, right, nlist)) {
         return new VarAssignment<_nlist>(pv_nlist->getVarPtr(), nlist);
      }
      else {
         varAssignException(origin, L"numeric list", right);
      }
   }

   ParseVariable<_tlist>* pv_tlist = nullptr;
   if (uro->vars.getVarPtr(first, pv_tlist) && pv_tlist->isReachable()) {
      Generator<_tlist>* tlist;
      if (parse(uro, right, tlist)) {
         return new VarAssignment<_tlist>(pv_tlist->getVarPtr(), tlist);
      }
      else {
         varAssignException(origin, L"time list", right);
      }
   }

   ParseVariable<_list>* pv_list = nullptr;
   if (uro->vars.getVarPtr(first, pv_list) && pv_list->isReachable()) {
      Generator<_list>* list;
      if (parse(uro, right, list)) {
         return new VarAssignment<_list>(pv_list->getVarPtr(), list);
      }
      else {
         varAssignException(origin, L"list", right);
      }
   }

   /////
   // create a new variable
   // or "resurrect" an existing variable from somewhere else out of reach
   ////

   Generator<_boo>* boo;
   if (parse(uro, right, boo)) {
      if (pv_boo == nullptr) {
         uro->vars.var_boo.insert(std::make_pair(first.value.word.h, ParseVariable<_boo>()));
         return new VarAssignment<_boo>(uro->vars.var_boo[first.value.word.h].getVarPtr(), boo);
      }
      else {
         pv_boo->resurrect();
         return new VarAssignment<_boo>(pv_boo->getVarPtr(), boo);
      }
   }

   Generator<_num>* num;
   if (parse(uro, right, num)) {
      if (pv_num == nullptr) {
         uro->vars.var_num.insert(std::make_pair(first.value.word.h, ParseVariable<_num>()));
         return new VarAssignment<_num>(uro->vars.var_num[first.value.word.h].getVarPtr(), num);
      }
      else {
         pv_num->resurrect();
         return new VarAssignment<_num>(pv_num->getVarPtr(), num);
      }
   }

   Generator<_tim>* tim;
   if (parse(uro, right, tim)) {
      if (pv_tim == nullptr) {
         uro->vars.var_tim.insert(std::make_pair(first.value.word.h, ParseVariable<_tim>()));
         return new VarAssignment<_tim>(uro->vars.var_tim[first.value.word.h].getVarPtr(), tim);
      }
      else {
         pv_tim->resurrect();
         return new VarAssignment<_tim>(pv_tim->getVarPtr(), tim);
      }
   }

   Generator<_per>* per;
   if (parse(uro, right, per)) {
      if (pv_per == nullptr) {
         uro->vars.var_per.insert(std::make_pair(first.value.word.h, ParseVariable<_per>()));
         return new VarAssignment<_per>(uro->vars.var_per[first.value.word.h].getVarPtr(), per);
      }
      else {
         pv_per->resurrect();
         return new VarAssignment<_per>(pv_per->getVarPtr(), per);
      }
   }

   Generator<_str>* str_;
   if (parse(uro, right, str_)) {
      if (pv_str == nullptr) {
         uro->vars.var_str.insert(std::make_pair(first.value.word.h, ParseVariable<_str>()));
         return new VarAssignment<_str>(uro->vars.var_str[first.value.word.h].getVarPtr(), str_);
      }
      else {
         pv_str->resurrect();
         return new VarAssignment<_str>(pv_str->getVarPtr(), str_);
      }
   }

   Generator<_nlist>* nlist;
   if (parse(uro, right, nlist)) {
      if (pv_nlist == nullptr) {
         uro->vars.var_nlist.insert(std::make_pair(first.value.word.h, ParseVariable<_nlist>()));
         return new VarAssignment<_nlist>(uro->vars.var_nlist[first.value.word.h].getVarPtr(), nlist);
      }
      else {
         pv_nlist->resurrect();
         return new VarAssignment<_nlist>(pv_nlist->getVarPtr(), nlist);
      }
   }

   Generator<_tlist>* tlist;
   if (parse(uro, right, tlist)) {
      if (pv_tlist == nullptr) {
         uro->vars.var_tlist.insert(std::make_pair(first.value.word.h, ParseVariable<_tlist>()));
         return new VarAssignment<_tlist>(uro->vars.var_tlist[first.value.word.h].getVarPtr(), tlist);
      }
      else {
         pv_tlist->resurrect();
         return new VarAssignment<_tlist>(pv_tlist->getVarPtr(), tlist);
      }
   }

   Generator<_list>* list;
   if (parse(uro, right, list)) {
      if (pv_list == nullptr) {
         uro->vars.var_list.insert(std::make_pair(first.value.word.h, ParseVariable<_list>()));
         return new VarAssignment<_list>(uro->vars.var_list[first.value.word.h].getVarPtr(), list);
      }
      else {
         pv_list->resurrect();
         return new VarAssignment<_list>(pv_list->getVarPtr(), list);
      }
   }

   throw SyntaxException(str(L"wrong declaration of variable '", origin, L"'"), first.line);
}

static void varAssignException(const _str& name, const _str& type, const Tokens& tks)
{
   throw SyntaxException(str(L"value assigned to variable '", name,
      L"' has to be of ", type, L" type"), tks.first().line);
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
   const Tokens& right, Uroboros* uro)
{
   const Token& first = left.first();

   ParseVariable<_list>* pv_list;
   ParseVariable<_nlist>* pv_nlist;
   ParseVariable<_tlist>* pv_tlist;

   if (uro->vars.getVarPtr(first, pv_list) ||
       uro->vars.getVarPtr(first, pv_nlist) ||
       uro->vars.getVarPtr(first, pv_tlist))
   {
      throw SyntaxException(str(L"collection variable '", *first.value.word.os,
         L"' is immutable, so its elements cannot me modified"), first.line);
   }

   ParseVariable<_str>* pv_str;
   if (uro->vars.getVarPtr(first, pv_str)) {
      if (pv_str->isReachable()) {
         Generator<_num>* index = parseListElementIndex(left, uro);
         Generator<_str>* str_;

         if (parse(uro, right, str_)) {
            return new VarCharAssignment(pv_str->getVarPtr(), str_, index);
         }
         else {
            throw SyntaxException(str(L"new value in character assignment of variable '",
               *first.value.word.os, L"' cannot be resolved to a string"), first.line);
         }
      }
      else {
         throw SyntaxException(str(L"variable '", *first.value.word.os,
            L"' is unreachable here"), first.line);
      }
   }

   throw SyntaxException(str(L"variable '", *first.value.word.os,
      L"' was not expected before [] brackets"), first.line);
}

static Generator<_num>* parseListElementIndex(const Tokens& tks, Uroboros* uro)
{
   Tokens left2(tks);
   left2.trimBoth();
   left2.trimLeft();
   Generator<_num>* index;

   if (!parse(uro, left2, index)) {
      throw SyntaxException(
        L"content of square brackets [] cannot be resolved to a number",
        tks.second().line);
   }

   return index;
}

static void checkNoSemicolonBeforeBrackets(const Tokens& tks)
{
   const _int end = tks.getEnd();
   const _int start = tks.getStart() + 1;
   const _boo startsWithElse = tks.listAt(start - 1).isKeyword(Keyword::kw_Else);

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Keyword) {
         switch (t.value.keyword.k) {
            case Keyword::kw_While:
            case Keyword::kw_Inside: {
               throw SyntaxException(str(L"a semicolon ; is missing before keyword '",
                  *t.value.keyword.os, L"'"), t.line);
            }
            case Keyword::kw_If: {
               if (!(i == start && startsWithElse)) {
                  throw SyntaxException(str(L"a semicolon ; is missing before keyword '",
                     *t.value.keyword.os, L"'"), t.line);
               }
               break;
            }
            case Keyword::kw_Else: {
               throw SyntaxException(str(L"keyword '", *t.value.keyword.os,
                  L"' should be preceded by curly brackets {}"), t.line);
            }
         }
      }
   }
}
