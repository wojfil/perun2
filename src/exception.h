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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "datatype/primitives.h"


namespace perun2
{

class SyntaxError
{
public:
   SyntaxError() = delete;
   SyntaxError(const _str& msg, const _int li);
   _str getMessage() const;

   static SyntaxError adjacentSymbols(const _char value, const _int line);
   static SyntaxError adjacentFilterKeywords(const _str& value1, const _str& value2, const _int line);
   static SyntaxError bracketIsNotClosed(const _char value, const _int line);
   static SyntaxError bracketShouldBeClosedBeforeCurlyBracket(const _char value, const _int line);
   static SyntaxError dayCannotBeSmallerThanOne(const _int line);
   static SyntaxError decrementationInsideExpression(const _int line);
   static SyntaxError expectedSemicolonBeforeKeyword(const _str& value, const _int line);
   static SyntaxError expressionCannotEndWith(const _char value, const _int line);
   static SyntaxError expressionCannotEndWithFilterKeyword(const _str& value, const _int line);
   static SyntaxError expressionCannotStartWith(const _char value, const _int line);
   static SyntaxError expressionCannotStartWithIncrementation(const _int line);
   static SyntaxError expressionCannotStartWithDecrementation(const _int line);
   static SyntaxError filterKeywordAtStart(const _str& value, const _int line);
   static SyntaxError filterKeywordAtEnd(const _str& value, const _int line);
   static SyntaxError hoursOutOfRange(const _str& value, const _int line);
   static SyntaxError incrementationInsideExpression(const _int line);
   static SyntaxError inevitableDivisionByZero(const _int line);
   static SyntaxError inevitableModuloByZero(const _int line);
   static SyntaxError insteadOfYouShouldWrite(const _str& value1, const _str& value2, const _int line);
   static SyntaxError invalidAsteriskPattern(const _str& value, const _int line);
   static SyntaxError invalidChar(const _char value, const _int line);
   static SyntaxError invalidExpression(const _int line);
   static SyntaxError invalidFunctionName(const _int line);
   static SyntaxError invalidMonthName(const _str& value, const _int line);
   static SyntaxError invalidNumericalExpression(const _int line);
   static SyntaxError keywordNotFound(const _int line);
   static SyntaxError keywordNotFollowedByBool(const _str& value, const _int line);
   static SyntaxError keywordNotFollowedByNumber(const _str& value, const _int line);
   static SyntaxError minutesOutOfRange(const _str& value, const _int line);
   static SyntaxError missingTimeVariableMember(const _str& value, const _int line);
   static SyntaxError missingLetterS(const _str& value, const _int line);
   static SyntaxError monthHasFewerDays(const _str& month, const _str& value, const _int line);
   static SyntaxError multipleDotsInNumber(const _str& value, const _int line);
   static SyntaxError multipleDotsInWord(const _str& value, const _int line);
   static SyntaxError negationByExclamation(const _int line);
   static SyntaxError numberTooBig(const _str& value, const _int line);
   static SyntaxError openedStringLteral(const _int line);
   static SyntaxError secondsOutOfRange(const _str& value, const _int line);
   static SyntaxError symbolNotFound(const _char value, const _int line);
   static SyntaxError quotationMarkStringLteral(const _int line);
   static SyntaxError undefinedVarValue(const _str& value, const _int line);
   static SyntaxError unopenedBracketIsClosed(const _char value, const _int line);
   static SyntaxError wrongSyntax(const _int line);

private:
   const _str message;
   const _int line;
};

class RuntimeError
{
public:
   RuntimeError() = delete;
   RuntimeError(const _str& msg);
   _str getMessage() const;

   static RuntimeError divisionByZero();
   static RuntimeError moduloByZero();
   static RuntimeError numberTooBig(const _str& value);
   static RuntimeError squareRootOfNegativeNumber(const _str& value);
   static RuntimeError wrongResultOfExponentiation();

private:
   const _str message;
};

}

#endif /* EXCEPTION_H */
