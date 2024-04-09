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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "datatype/primitives.h"


namespace perun2
{

class SyntaxError
{
public:
   SyntaxError() = delete;
   SyntaxError(const p_str& msg, const p_int li);
   p_str getMessage() const;

   static SyntaxError adjacentSymbols(const p_char value, const p_int line);
   static SyntaxError adjacentFilterKeywords(const p_str& value1, const p_str& value2, const p_int line);
   static SyntaxError asteriskPatternCannotContainDotSegments(const p_str& value, const p_int line);
   static SyntaxError asteriskIsNotWildcardInLikeOperator(const p_str& value, const p_int line);
   static SyntaxError bracketIsNotClosed(const p_char value, const p_int line);
   static SyntaxError bracketShouldBeClosedBeforeCurlyBracket(const p_char value, const p_int line);
   static SyntaxError dayCannotBeSmallerThanOne(const p_int line);
   static SyntaxError decrementationInsideExpression(const p_int line);
   static SyntaxError expectedSemicolonBeforeKeyword(const p_str& value, const p_int line);
   static SyntaxError expressionCannotEndWith(const p_char value, const p_int line);
   static SyntaxError expressionCannotEndWithFilterKeyword(const p_str& value, const p_int line);
   static SyntaxError expressionCannotStartWith(const p_char value, const p_int line);
   static SyntaxError expressionCannotStartWithIncrementation(const p_int line);
   static SyntaxError expressionCannotStartWithDecrementation(const p_int line);
   static SyntaxError filterKeywordAtStart(const p_str& value, const p_int line);
   static SyntaxError filterKeywordAtEnd(const p_str& value, const p_int line);
   static SyntaxError hoursOutOfRange(const p_str& value, const p_int line);
   static SyntaxError incrementationInsideExpression(const p_int line);
   static SyntaxError insteadOfYouShouldWrite(const p_str& value1, const p_str& value2, const p_int line);
   static SyntaxError invalidAsteriskPattern(const p_str& value, const p_int line);
   static SyntaxError invalidChar(const p_char value, const p_int line);
   static SyntaxError invalidExpression(const p_int line);
   static SyntaxError invalidFunctionName(const p_int line);
   static SyntaxError invalidMonthName(const p_str& value, const p_int line);
   static SyntaxError invalidNumericalExpression(const p_int line);
   static SyntaxError keywordNotFound(const p_int line);
   static SyntaxError keywordNotFollowedByBool(const p_str& value, const p_int line);
   static SyntaxError keywordNotFollowedByNumber(const p_str& value, const p_int line);
   static SyntaxError leftSideOfOperatorIsEmpty(const p_str& operator_, const p_int line);
   static SyntaxError minutesOutOfRange(const p_str& value, const p_int line);
   static SyntaxError missingTimeVariableMember(const p_str& value, const p_int line);
   static SyntaxError missingLetterS(const p_str& value, const p_int line);
   static SyntaxError monthHasFewerDays(const p_str& month, const p_str& value, const p_int line);
   static SyntaxError multipleDotsInNumber(const p_str& value, const p_int line);
   static SyntaxError multipleDotsInWord(const p_str& value, const p_int line);
   static SyntaxError negationByExclamation(const p_int line);
   static SyntaxError numberTooBig(const p_str& value, const p_int line);
   static SyntaxError openedStringLteral(const p_int line);
   static SyntaxError operatorBetweenShouldBeFolowedByAnd(const p_str& value, const p_int line);
   static SyntaxError rightSideOfOperatorIsEmpty(const p_str& operator_, const p_int line);
   static SyntaxError secondsOutOfRange(const p_str& value, const p_int line);
   static SyntaxError supposedUnintentionalAsteriskPattern(const p_str& value, const p_int line);
   static SyntaxError symbolNotFound(const p_char value, const p_int line);
   static SyntaxError syntaxOfBooleanExpressionNotValid(const p_int line);
   static SyntaxError quotationMarkStringLteral(const p_int line);
   static SyntaxError youShouldUseApostrophesAndWrite(const p_str& value, const p_int line);
   static SyntaxError undefinedVarValue(const p_str& value, const p_int line);
   static SyntaxError unopenedBracketIsClosed(const p_char value, const p_int line);
   static SyntaxError wrongSyntax(const p_int line);
   static SyntaxError wrongSyntaxButProbablyAsteriskPattern(const p_int line);
   // to be continued
   // all errors from the source code will finally end up here

private:
   const p_str message;
   const p_int line;
};

}

#endif /* EXCEPTION_H */
