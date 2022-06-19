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



Command* parseCommands(const Tokens& tks)
{
   std::vector<Command*> commands;
   _int sublen = 0;
   _int depth = 0;
   _int open = -1;
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol)  {
         switch(t.value.c) {
            case L';': {
               if (depth == 0) {
                  if (sublen != 0) {
                     lockLastIf();
                     Tokens tks2(tks.list, i - sublen, sublen);
                     commands.push_back(command(tks2));
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
                  Command* com = commandStruct(tks, sublen, i, open);
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
      commands.push_back(command(tks2));
   }

   if (commands.size() == 1) {
      return commands[0];
   }
   else {
      return new CS_Block(commands);
   }
}

// can return nullptr
static Command* commandStruct(const Tokens& tks, const _int& sublen,
   const _int& index, const _int& open)
{
   const _int leftStart = index - sublen;
   const _int leftLen = open - leftStart;
   const _int rightStart = open + 1;
   const _int rightLen = index - rightStart;

   // build simple block of commands
   if (leftLen == 0) {
      if (rightLen == 0) {
         return nullptr;
      }

      Tokens tks2(tks.list, rightStart, rightLen);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(tks2);
      afterCommandStruct();
      return com;
   }

   // build "times"
   Tokens left(tks.list, leftStart, leftLen);
   const Token& leftLast = left.last();
   if (leftLast.isKeyword(Keyword::kw_Times)) {
      left.trimRight();
      if (left.isEmpty()) {
         throw SyntaxException(L"keyword '"+ leftLast.originString + L"' is not preceded by a number",
            leftLast.line);
      }

      if (rightLen == 0) {
         return nullptr;
      }

      Generator<_num>* num;
      if (!parse(left, num)) {
         throw SyntaxException(
            L"keyword '" + leftLast.originString + L"' is not preceded by a valid number",
            leftLast.line);
      }

      Tokens right(tks.list, rightStart, rightLen);

      Aggregate* aggr = new Aggregate();
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_Times(num, com, aggr);
   }

   checkNoSemicolonBeforeBrackets(left);

   // build "while"
   const Token& leftFirst = left.first();
   if (leftFirst.isKeyword(Keyword::kw_While)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxException(L"keyword '" + leftFirst.originString + L"' is not followed by a condition",
            leftFirst.line);
      }

      if (rightLen == 0) {
         throw SyntaxException(L"structure '" + leftFirst.originString + L"' is empty. It would either never run or cause an infinite loop", leftFirst.line);
      }

      Generator<_boo>* boo;
      if (parse(left, boo)) {
         throw SyntaxException(
            L"keyword '" + leftFirst.originString + L"' is not followed by a valid condition",
            leftFirst.line);
      }

      Tokens right(tks.list, rightStart, rightLen);

      Aggregate* aggr = new Aggregate();
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_While(boo, com, aggr);
   }

   const ThisState prevThisState = g_thisstate;

   // build "inside"
   if (leftFirst.isKeyword(Keyword::kw_Inside)) {
      left.trimLeft();
      if (left.isEmpty() && g_thisstate != ThisState::ts_String) {
         throw SyntaxException(L"argumentless structure '" + leftFirst.originString + L"' can be created only inside a loop iterating over strings", leftFirst.line);
      }

      if (rightLen == 0) {
         //throw SyntaxException(L"structure '" + leftFirst.originString + L"' is empty", leftFirst.line);
         return nullptr;
      }

      g_thisstate = ThisState::ts_String;
      Tokens right(tks.list, rightStart, rightLen);
      const _boo hasMemory = anyAttribute();
      Attribute* attr = new Attribute();
      Aggregate* aggr = new Aggregate();
      addAttribute(attr);
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      g_thisstate = prevThisState;
      retreatAttribute();
      retreatAggregate();

      if (com == nullptr) {
         return nullptr;
      }

      if (left.isEmpty()) {
         Generator<_str>* tr = new ThisReference();
         return new CS_InsideString(tr, com, attr, aggr, hasMemory);
      }

      Generator<_str>* str;
      if (parse(left, str)) {
         return new CS_InsideString(str, com, attr, aggr, hasMemory);
      }

      _def* def;
      if (parse(left, def)) {
         if (attr->markToEvaluate) {
            return new CS_InsideList(new Cast_D_L(def), com, attr, aggr, hasMemory);
         }
         else {
            return new CS_InsideDefinition(def, com, attr, aggr, hasMemory);
         }
      }

      Generator<_list>* list;
      if (parse(left, list)) {
         throw SyntaxException(L"keyword '" + leftFirst.originString + L"' is not followed by a valid "
            L"declaration of string or list",
            leftFirst.line);
      }
      else {
         return new CS_InsideList(list, com, attr, aggr, hasMemory);
      }
   }

   // build "if"
   if (leftFirst.isKeyword(Keyword::kw_If)) {
      left.trimLeft();
      if (left.isEmpty()) {
         throw SyntaxException(L"keyword '" + leftFirst.originString + L"' is not followed by a condition",
            leftFirst.line);
      }

      Generator<_boo>* boo;
      if (parse(left, boo)) {
         throw SyntaxException(
            L"keyword '" + leftFirst.originString + L"' is not followed by a valid condition",
            leftFirst.line);
      }

      CS_If* cif = new CS_If(boo);

      if (rightLen == 0) {
         beforeCommandStruct(cif);
         afterCommandStruct();
         return cif;
      }

      Tokens right(tks.list, rightStart, rightLen);

      beforeCommandStruct(cif);
         Command* com = parseCommands(right);
      afterCommandStruct();

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

         varsLevelUp();
            Command* com = parseCommands(right);
         varsLevelDown();

         if (com != nullptr) {
            setElse(com, leftFirst.line);
         }
      }
      else {
         if (!left.first().isKeyword(Keyword::kw_If)) {
            throw SyntaxException(
               L"keyword '" + leftFirst.originString + L"' cannot be followed by an expression",
               leftFirst.line);
         }
         const Token& ifToken = left.first();
         left.trimLeft();

         if (left.isEmpty()) {
            throw SyntaxException(
               L"keywords '" + leftFirst.originString + L" " + ifToken.originString + L"' are not followed by a condition",
               leftFirst.line);
         }

         Generator<_boo>* boo;
         if (parse(left, boo)) {
            throw SyntaxException(
            L"keywords '" + leftFirst.originString + L" " + ifToken.originString + L"' are not followed by a valid condition",
               leftFirst.line);
         }

         if (rightLen == 0) {
            addElseIf(boo, new C_DoNothing(), leftFirst.line);
            return nullptr;
         }

         Tokens right(tks.list, rightStart, rightLen);

         varsLevelUp();
            Command* com = parseCommands(right);
         varsLevelDown();

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
   if (parse(left, tlist)) {
      g_thisstate = ThisState::ts_Time;
      Aggregate* aggr = new Aggregate();
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      g_thisstate = prevThisState;
      retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_TimeLoop(tlist, com, aggr);
   }

   // number list loop
   Generator<_nlist>* nlist;
   if (parse(left, nlist)) {
      g_thisstate = ThisState::ts_Number;
      Aggregate* aggr = new Aggregate();
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      g_thisstate = prevThisState;
      retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_NumberLoop(nlist, com, aggr);
   }

   // string loop
   Generator<_str>* str;
   if (parse(left, str)) {
      const _boo hasMemory = anyAttribute();
      g_thisstate = ThisState::ts_String;
      Attribute* attr = new Attribute();
      Aggregate* aggr = new Aggregate();
      addAttribute(attr);
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      g_thisstate = prevThisState;
      retreatAttribute();
      retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_StringLoop(str, com, attr, aggr, hasMemory);
   }

   // definition loop
   _def* def;
   if (parse(left, def)) {
      const _boo hasMemory = anyAttribute();
      g_thisstate = ThisState::ts_String;
      Attribute* attr = new Attribute();
      Aggregate* aggr = new Aggregate();
      addAttribute(attr);
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      g_thisstate = prevThisState;
      retreatAttribute();
      retreatAggregate();

      if (com == nullptr) {
         return nullptr;
      }

      if (attr->markToEvaluate) {
         return new CS_ListLoop(new Cast_D_L(def), com, attr, aggr, hasMemory);
      }
      else {
         return new CS_DefinitionLoop(def, com, attr, aggr, hasMemory);
      }
   }

   // list loop
   Generator<_list>* lst;
   if (parse(left, lst)) {
      const _boo hasMemory = anyAttribute();
      g_thisstate = ThisState::ts_String;
      Attribute* attr = new Attribute();
      Aggregate* aggr = new Aggregate();
      addAttribute(attr);
      addAggregate(aggr);
      beforeCommandStruct(nullptr);
         Command* com = parseCommands(right);
      afterCommandStruct();
      g_thisstate = prevThisState;
      retreatAttribute();
      retreatAggregate();

      return com == nullptr
         ? nullptr
         : new CS_ListLoop(lst, com, attr, aggr, hasMemory);
   }

   throw SyntaxException(L"wrong syntax before { bracket", tks.first().line);
}

static void beforeCommandStruct(CS_If* pntr)
{
   addIfParseUnit(pntr);
   varsLevelUp();
}

static void afterCommandStruct()
{
   retreatIfParseUnit();
   varsLevelDown();
}

static Command* command(Tokens& tks)
{
   const Token& f = tks.first();

   if (tks.getLength() == 1 && f.type == Token::t_Keyword) {
      switch (f.value.k) {
         case Keyword::kw_Break:
         case Keyword::kw_Continue: {
            if (g_aggrs.empty()) {
               throw SyntaxException(L"command '" + f.originString
                  + L"' can be called only inside a loop", f.line);
            }
            else {
               if (f.value.k == Keyword::kw_Break) {
                  return new C_Break();
               }
               else {
                  return new C_Continue();
               }
            }
            break;
         }
         case Keyword::kw_Exit: {
            return new C_Exit();
         }
      }
   }

   _boo force = false;
   _boo stack = false;

   const Token& f2 = tks.first();
   if (f2.type == Token::t_Keyword) {
      switch (f2.value.k) {
         case Keyword::kw_Force: {
            tks.trimLeft();
            force = true;

            if (tks.isEmpty()) {
               throw SyntaxException(L"command cannot consist of only one keyword: '"
                  + f2.originString + L"'", f.line);
            }
            break;
         }
         case Keyword::kw_Stack: {
            tks.trimLeft();
            stack = true;

            if (tks.isEmpty()) {
               throw SyntaxException(L"command cannot consist of only one keyword: '"
                  + f2.originString + L"'", f.line);
            }
            break;
         }
      }
   }

   const Token& f3 = tks.first();
   if (f3.type == Token::t_Keyword) {
      switch (f3.value.k) {
         case Keyword::kw_Not:
         case Keyword::kw_True:
         case Keyword::kw_False: {
            break;
         }
         default: {
            tks.trimLeft();
            return keywordCommands(f3, tks, f.line, force, stack);
         }
      }
   }

   if (force) {
      throw SyntaxException(
         L"only a core command can start with a keyword 'force'", f.line);
   }
   if (stack) {
      throw SyntaxException(
         L"only a core command can start with a keyword 'stack'", f.line);
   }

   Command* misc = commandMisc(tks);

   if (misc == nullptr) {
      Token pf(Keyword::kw_Print, f.line, L"print");
      return c_print(pf, tks, f.line, false);
   }
   else {
      return misc;
   }
}

static Command* commandMisc(const Tokens& tks)
{
   if (tks.containsSymbol(L'=')) {
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
         const char& ch = leftLast.value.c;
         switch (ch) {
            case L'+':
            case L'-':
            case L'*':
            case L'/':
            case L'%': {
               left.trimRight();
               if (left.isEmpty()) {
                  throw SyntaxException(L"left side of the " + _str(1, ch)
                     + L"= operator is empty", tks.last().line);
               }

               return commandVarChange(left, right, ch);
            }
         }
      }

      if (varSquareBrackets(left)) {
         return commandVarAssign_Element(left, right);
      }
      else {
         return commandVarAssign(left, right);
      }
   }

   if (tks.penultimate().type == Token::t_Symbol
      && tks.last().type == Token::t_Symbol) {

      const _char& c1 = tks.penultimate().value.c;
      const _char& c2 = tks.last().value.c;
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

               if (!getVarPtr(first, pv_num)) {
                  throw SyntaxException(L"variable '" + first.originString +
                     L"' cannot be " + op, first.line);
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
                  Generator<_num>* index = parseListElementIndex(tks2);
                  ParseVariable<_nlist>* pv_nlist;

                  if (!getVarPtr(first, pv_nlist)) {
                     throw SyntaxException(L"variable '" + first.originString +
                        L"' cannot be " + op, first.line);
                  }

                  throw SyntaxException(L"an element of variable '" + first.originString +
                     L"' cannot be " + op + L", because collections in Uroboros are immutable",
                     first.line);
               }
               else {
                  throw SyntaxException(L"this structure cannot be " + op, first.line);
               }
            }
         }
         else if (first.type == Token::t_TwoWords) {
            if (first.value.h1 == HASH_NOTHING) {
               throw SyntaxException(L"the dot . should be preceded by a time variable name", first.line);
            }

            ParseVariable<_tim>* pv_tim;

            if (!getVarPtr(first, pv_tim)) {
               throw SyntaxException(L"time variable from expression '" + first.originString
                  + L"' does not exist or is unreachable here", first.line);
            }

            const _size& h = first.value.h2;
            Period::PeriodUnit unit;

            if (h == HASH_PER_YEAR || h == HASH_PER_YEARS)
               unit = Period::u_Years;
            else if (h == HASH_PER_MONTH || h == HASH_PER_MONTHS)
               unit = Period::u_Months;
            else if (h == HASH_PER_DAY || h == HASH_PER_DAYS)
               unit = Period::u_Days;
            else if (h == HASH_PER_HOUR || h == HASH_PER_HOURS)
               unit = Period::u_Hours;
            else if (h == HASH_PER_MINUTE || h == HASH_PER_MINUTES)
               unit = Period::u_Minutes;
            else if (h == HASH_PER_SECOND || h == HASH_PER_SECONDS)
               unit = Period::u_Seconds;
            else if (h == HASH_PER_DATE || h == HASH_PER_WEEKDAY) {
               throw SyntaxException(L"time variable member '" + first.originString2 + L"' cannot be " + op, first.line);
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
            throw SyntaxException(L"only a variable of a singular value type can be " + op, first.line);
         }
      }
   }

   return nullptr;
}

static Command* commandVarChange(const Tokens& left, const Tokens& right,
   const _char& sign)
{
   const Token& first = left.first();

   if (left.getLength() > 1) {
      if (varSquareBrackets(left)) {
         ParseVariable<_list>* pv_list;
         ParseVariable<_nlist>* pv_nlist;
         ParseVariable<_tlist>* pv_tlist;

         if (getVarPtr(first, pv_list) || getVarPtr(first, pv_nlist) || getVarPtr(first, pv_tlist)) {
            throw SyntaxException(L"collection variable '" + first.originString +
               L"' is immutable, so its elements cannot be modified", right.first().line);
         }

         ParseVariable<_str>* pv_str;

         if (getVarPtr(first, pv_str)) {
            throw SyntaxException(L"operation " + _str(1, sign)
               + L"= cannot be performed on a character from string variable", first.line);
         }

         throw SyntaxException(first.originString + L" is not a collection variable, which are expected before [] brackets", first.line);
      }
      else {
         const Token& arom = left.last();

         if (arom.type == Token::t_TwoWords && arom.value.h1 == HASH_NOTHING) {
            Tokens aro(left);
            aro.trimRight();

            if (varSquareBrackets(aro)) {
               ParseVariable<_tlist>* pv_tlist;
               if (getVarPtr(first, pv_tlist)) {
                  throw SyntaxException(L"operation " + _str(1, sign)
                     + L"= cannot be performed on a time list variable member. Collections in Uroboros are immutable", first.line);
               }
            }
         }

         throw SyntaxException(L"operator " + _str(1, sign) +
            L"= should be preceded by a variable name", first.line);
      }
   }

   if (first.type == Token::t_Word) {
      ParseVariable<_num>* pv_num;
      if (getVarPtr(first, pv_num)) {
         Generator<_num>* num;

         if (!parse(right, num)) {
            throw SyntaxException(L"right side of operator " + _str(1, sign)
               + L"= cannot be resolved to a number", first.line);
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
      if (getVarPtr(first, pv_per)) {
         Variable<_per>* var = pv_per->getVarPtr();

         switch (sign) {
            case L'+':
            case L'-': {
               Generator<_per>* per;

               if (!parse(right, per)) {
                  throw SyntaxException(L"right side of operator " + _str(1, sign)
                     + L"= cannot be resolved to a period", first.line);
               }

               if (sign == L'+')
                  return new VarAdd_<_per>(var, per);
               else
                  return new VarSubtract<_per>(var, per);
            }
            case L'*':
            case L'/':
            case L'%': {
               throw SyntaxException(L"operation " + _str(1, sign) +
                  L"= cannot be performed on a period", first.line);
            }
         }
      }

      ParseVariable<_tim>* pv_tim;
      if (getVarPtr(first, pv_tim)) {
         switch (sign) {
            case L'*':
            case L'/':
            case L'%': {
               throw SyntaxException(L"operation " + _str(1, sign) +
                  L"= is not valid for a time variable", first.line);
            }
         }

         Variable<_tim>* var = pv_tim->getVarPtr();
         Generator<_per>* per;

         if (!parse(right, per)) {
            throw SyntaxException(L"right side of operator '" + first.originString + L" " +
               _str(1, sign) + L"=' cannot be resolved to a period", first.line);
         }

         if (sign == L'+') {
            return new VarTimeAdd(var, per);
         }
         else {
            return new VarTimeSubtract(var, per);
         }
      }

      if (sign == L'+') {
         return commandVarIncrement(first, right, first.line);
      }

      throw SyntaxException(L"'" + first.originString +
         L"' is neither a numeric, time, nor period variable", first.line);
   }
   else if (first.type == Token::t_TwoWords) {
      switch (sign) {
         case L'*':
         case L'/':
         case L'%': {
            throw SyntaxException(L"operation " + _str(1, sign) +
               L"= is not valid for a time variable", first.line);
         }
      }

      ParseVariable<_tim>* pv_tim;
      if (!getVarPtr(first, pv_tim)) {
         const _size id = first.originString.find_last_of(L".");

         throw SyntaxException(L"'" + first.originString.substr(0, id) +
            L"' is not a time variable for " + _str(1, sign) + L"= operation",
            first.line);
      }

      Generator<_num>* num;

      if (!parse(right, num)) {
         throw SyntaxException(L"right side of operation '" + first.originString + L"." +
            first.originString2 + L" " + _str(1, sign) +
            L"=' cannot be resolved to a number", first.line);
      }

      const _size& h = first.value.h2;
      Variable<_tim>* var = pv_tim->getVarPtr();
      const bool negative = (sign == '-');

      if (h == HASH_PER_YEAR || h == HASH_PER_YEARS)
         return new VarTimeUnitChange(var, num, Period::u_Years, negative);
      else if (h == HASH_PER_MONTH || h == HASH_PER_MONTHS)
         return new VarTimeUnitChange(var, num, Period::u_Months, negative);
      else if (h == HASH_PER_DAY || h == HASH_PER_DAYS)
         return new VarTimeUnitChange(var, num, Period::u_Days, negative);
      else if (h == HASH_PER_HOUR || h == HASH_PER_HOURS)
         return new VarTimeUnitChange(var, num, Period::u_Hours, negative);
      else if (h == HASH_PER_MINUTE || h == HASH_PER_MINUTES)
         return new VarTimeUnitChange(var, num, Period::u_Minutes, negative);
      else if (h == HASH_PER_SECOND || h == HASH_PER_SECONDS)
         return new VarTimeUnitChange(var, num, Period::u_Seconds, negative);
      else if (h == HASH_PER_DATE || h == HASH_PER_WEEKDAY) {
         throw SyntaxException(L"value of '" + first.originString2 +
            L"' time variable member cannot be altered", first.line);
      }

      timeVariableMemberException(first);
   }
   else {
      throw SyntaxException(L"operator " + _str(1, sign) +
         L"= has to be preceded by a single word, which is a variable name",
         first.line);
   }

   return nullptr;
}

static Command* commandVarIncrement(const Token& first, const Tokens& tks,
   const _int& line)
{
   ParseVariable<_str>* pv_str;
   if (getVarPtr(first, pv_str)) {
      Variable<_str>* var = pv_str->getVarPtr();

      Generator<_str>* str;
      if (parse(tks, str)) {
         return new VarAdd_<_str>(var, str);
      }

      Generator<_list>* list;
      if (parse(tks, list)) {
         delete list;
         throw SyntaxException(L"variable '" + first.originString +
            L"' can be incremented only by a string", line);
      }
      else {
         throw SyntaxException(L"right side of operator '+=' cannot"
            L" be resolved to a string", line);
      }
   }

   throw SyntaxException(L"variable '" + first.originString +
      L"' cannot be incremented by a value", line);
}



static Command* commandVarAssign(const Tokens& left, const Tokens& right)
{
   const Token& first = left.first();
   const _str& origin = first.originString;

   if (first.type != Token::t_Word) {
      return nullptr;
   }

   if (left.getLength() >= 5 ) {
      if (left.last().type == Token::t_TwoWords && left.last().value.h1 == HASH_NOTHING) {
         Tokens le(left);
         le.trimRight();

         if (varSquareBrackets(le)) {
            ParseVariable<_nlist>* pv_nlist;
            ParseVariable<_tlist>* pv_tlist;
            ParseVariable<_list>* pv_list;

            if (getVarPtr(first, pv_nlist) || getVarPtr(first, pv_tlist) || getVarPtr(first, pv_list)) {
               throw SyntaxException(L"collection variable '" + origin +
                  L"' is immutable, so its elements cannot me modified", first.line);
            }
            else {
               throw SyntaxException(L"unknown collection variable '" + origin +
                  L"'. Collection variables are immutable in Uroboros anyways", first.line);
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

   if (HASH_GROUP_INNERVAR.find(first.value.h1) != HASH_GROUP_INNERVAR.end()) {
      throw SyntaxException(L"variable '" + first.originString + L"' is immutable", first.line);
   }

   ParseVariable<_boo>* pv_boo = nullptr;
   if (getVarPtr(first, pv_boo) && pv_boo->isReachable()) {
      Generator<_boo>* boo;
      if (parse(right, boo)) {
         return new VarAssignment<_boo>(pv_boo->getVarPtr(), boo);
      }
      else {
         varAssignException(origin, L"bool", right);
      }
   }

   ParseVariable<_num>* pv_num = nullptr;
   if (getVarPtr(first, pv_num) && pv_num->isReachable()) {
      Generator<_num>* num;
      if (parse(right, num)) {
         return new VarAssignment<_num>(pv_num->getVarPtr(), num);
      }
      else {
         varAssignException(origin, L"number", right);
      }
   }

   ParseVariable<_tim>* pv_tim = nullptr;
   if (getVarPtr(first, pv_tim) && pv_tim->isReachable()) {
      Generator<_tim>* tim;
      if (parse(right, tim)) {
         return new VarAssignment<_tim>(pv_tim->getVarPtr(), tim);
      }
      else {
         varAssignException(origin, L"time", right);
      }
   }

   ParseVariable<_per>* pv_per = nullptr;
   if (getVarPtr(first, pv_per) && pv_per->isReachable()) {
      Generator<_per>* per;
      if (parse(right, per)) {
         return new VarAssignment<_per>(pv_per->getVarPtr(), per);
      }
      else {
         varAssignException(origin, L"period", right);
      }
   }

   ParseVariable<_str>* pv_str = nullptr;
   if (getVarPtr(first, pv_str) && pv_str->isReachable()) {
      Generator<_str>* str;
      if (parse(right, str)) {
         return new VarAssignment<_str>(pv_str->getVarPtr(), str);
      }
      else {
         varAssignException(origin, L"string", right);
      }
   }

   ParseVariable<_nlist>* pv_nlist = nullptr;
   if (getVarPtr(first, pv_nlist) && pv_nlist->isReachable()) {
      Generator<_nlist>* nlist;
      if (parse(right, nlist)) {
         return new VarAssignment<_nlist>(pv_nlist->getVarPtr(), nlist);
      }
      else {
         varAssignException(origin, L"numeric list", right);
      }
   }

   ParseVariable<_tlist>* pv_tlist = nullptr;
   if (getVarPtr(first, pv_tlist) && pv_tlist->isReachable()) {
      Generator<_tlist>* tlist;
      if (parse(right, tlist)) {
         return new VarAssignment<_tlist>(pv_tlist->getVarPtr(), tlist);
      }
      else {
         varAssignException(origin, L"time list", right);
      }
   }

   ParseVariable<_list>* pv_list = nullptr;
   if (getVarPtr(first, pv_list) && pv_list->isReachable()) {
      Generator<_list>* list;
      if (parse(right, list)) {
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
   if (parse(right, boo)) {
      if (pv_boo == nullptr) {
         g_var_boo.insert(std::make_pair(first.value.h1, ParseVariable<_boo>()));
         return new VarAssignment<_boo>(g_var_boo[first.value.h1].getVarPtr(), boo);
      }
      else {
         pv_boo->resurrect();
         return new VarAssignment<_boo>(pv_boo->getVarPtr(), boo);
      }
   }

   Generator<_num>* num;
   if (parse(right, num)) {
      if (pv_num == nullptr) {
         g_var_num.insert(std::make_pair(first.value.h1, ParseVariable<_num>()));
         return new VarAssignment<_num>(g_var_num[first.value.h1].getVarPtr(), num);
      }
      else {
         pv_num->resurrect();
         return new VarAssignment<_num>(pv_num->getVarPtr(), num);
      }
   }

   Generator<_tim>* tim;
   if (parse(right, tim)) {
      if (pv_tim == nullptr) {
         g_var_tim.insert(std::make_pair(first.value.h1, ParseVariable<_tim>()));
         return new VarAssignment<_tim>(g_var_tim[first.value.h1].getVarPtr(), tim);
      }
      else {
         pv_tim->resurrect();
         return new VarAssignment<_tim>(pv_tim->getVarPtr(), tim);
      }
   }

   Generator<_per>* per;
   if (parse(right, per)) {
      if (pv_per == nullptr) {
         g_var_per.insert(std::make_pair(first.value.h1, ParseVariable<_per>()));
         return new VarAssignment<_per>(g_var_per[first.value.h1].getVarPtr(), per);
      }
      else {
         pv_per->resurrect();
         return new VarAssignment<_per>(pv_per->getVarPtr(), per);
      }
   }

   Generator<_str>* str;
   if (parse(right, str)) {
      if (pv_str == nullptr) {
         g_var_str.insert(std::make_pair(first.value.h1, ParseVariable<_str>()));
         return new VarAssignment<_str>(g_var_str[first.value.h1].getVarPtr(), str);
      }
      else {
         pv_str->resurrect();
         return new VarAssignment<_str>(pv_str->getVarPtr(), str);
      }
   }

   Generator<_nlist>* nlist;
   if (parse(right, nlist)) {
      if (pv_nlist == nullptr) {
         g_var_nlist.insert(std::make_pair(first.value.h1, ParseVariable<_nlist>()));
         return new VarAssignment<_nlist>(g_var_nlist[first.value.h1].getVarPtr(), nlist);
      }
      else {
         pv_nlist->resurrect();
         return new VarAssignment<_nlist>(pv_nlist->getVarPtr(), nlist);
      }
   }

   Generator<_tlist>* tlist;
   if (parse(right, tlist)) {
      if (pv_tlist == nullptr) {
         g_var_tlist.insert(std::make_pair(first.value.h1, ParseVariable<_tlist>()));
         return new VarAssignment<_tlist>(g_var_tlist[first.value.h1].getVarPtr(), tlist);
      }
      else {
         pv_tlist->resurrect();
         return new VarAssignment<_tlist>(pv_tlist->getVarPtr(), tlist);
      }
   }

   Generator<_list>* list;
   if (parse(right, list)) {
      if (pv_list == nullptr) {
         g_var_list.insert(std::make_pair(first.value.h1, ParseVariable<_list>()));
         return new VarAssignment<_list>(g_var_list[first.value.h1].getVarPtr(), list);
      }
      else {
         pv_list->resurrect();
         return new VarAssignment<_list>(pv_list->getVarPtr(), list);
      }
   }

   throw SyntaxException(L"wrong declaration of variable '" + origin + L"'", first.line);
}

static void varAssignException(const _str& name, const _str& type, const Tokens& tks)
{
   throw SyntaxException(L"value assigned to variable '" + name
      + L"' has to be of " + type + L" type", tks.first().line);
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
   const Tokens& right)
{
   const Token& first = left.first();

   ParseVariable<_list>* pv_list;
   ParseVariable<_nlist>* pv_nlist;
   ParseVariable<_tlist>* pv_tlist;

   if (getVarPtr(first, pv_list) || getVarPtr(first, pv_nlist) || getVarPtr(first, pv_tlist)) {
      throw SyntaxException(L"collection variable '" + first.originString +
         L"' is immutable, so its elements cannot me modified", first.line);
   }

   ParseVariable<_str>* pv_str;
   if (getVarPtr(first, pv_str)) {
      if (pv_str->isReachable()) {
         Generator<_num>* index = parseListElementIndex(left);
         Generator<_str>* str;

         if (parse(right, str)) {
            return new VarCharAssignment(pv_str->getVarPtr(), str, index);
         }
         else {
            throw SyntaxException(L"new value in character assignment of variable '" + first.originString +
               L"' cannot be resolved to a string", first.line);
         }
      }
      else {
         throw SyntaxException(L"variable '" + first.originString +
            L"' is unreachable here", first.line);
      }
   }

   throw SyntaxException(L"variable '" + first.originString
      + L"' was not expected before [] brackets", first.line);
}

static Generator<_num>* parseListElementIndex(const Tokens& tks)
{
   Tokens left2(tks);
   left2.trimBoth();
   left2.trimLeft();
   Generator<_num>* index;

   if (!parse(left2, index)) {
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
         switch (t.value.k) {
            case Keyword::kw_While:
            case Keyword::kw_Inside: {
               throw SyntaxException(
                  L"a semicolon ; is missing before keyword '" + t.originString + L"'", t.line);
            }
            case Keyword::kw_If: {
               if (!(i == start && startsWithElse)) {
                  throw SyntaxException(
                     L"a semicolon ; is missing before keyword '" + t.originString + L"'", t.line);
               }
               break;
            }
            case Keyword::kw_Else: {
               throw SyntaxException(
                  L"keyword '" + t.originString + L"' should be preceded by curly brackets {}", t.line);
            }
         }
      }
   }
}
