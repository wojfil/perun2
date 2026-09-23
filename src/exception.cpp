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

#include "exception.h"
#include "metadata.h"
#include "datatype/datatype.h"
#include "datatype/text/like.h"
#include <algorithm>


namespace perun2
{

SyntaxError::SyntaxError(const p_str& msg, const p_int li)
   : message(msg), line(li) { };

p_str SyntaxError::getMessage() const
{
   return str(U"Error at line ", intToString(line), U": ", message, U".");
}

SyntaxError SyntaxError::adjacentSymbols(const p_char value, const p_int line)
{
   return SyntaxError(str(U"adjacent ", intToString(value), U" symbols"), line);
}

SyntaxError SyntaxError::asteriskPatternCannotContainDotSegments(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the asterisk pattern \"", value, 
      U"\" cannot contain dot segments in the middle of a path"), line);
}

SyntaxError SyntaxError::asteriskIsNotWildcardInLikeOperator(const p_str& value, const p_int line)
{
   p_str proper = value;
   std::replace(proper.begin(), proper.end(), CHAR_ASTERISK, gen::WILDCARD_MULTIPLE_CHARS);

   return SyntaxError(str(U"the asterisk in not a wildcard character of the Like operator. You should write \"", 
      proper,  U"\" instead"), line);
}

SyntaxError SyntaxError::bracketIsNotClosed(const p_char value, const p_int line)
{
   return SyntaxError(str(U"the bracket ", intToString(value), U" is not closed"), line);
}

SyntaxError SyntaxError::bracketShouldBeClosedBeforeCurlyBracket(const p_char value, const p_int line)
{
   return SyntaxError(str(U"the bracket ", intToString(value),
      U" should to be closed before the opening of the curly bracket {"), line);
}

SyntaxError SyntaxError::adjacentFilterKeywords(const p_str& value1, const p_str& value2, const p_int line)
{
   return SyntaxError(str(U"adjacent filter keywords \"", value1, U"\" and \"", value2, U"\""), line);
}

SyntaxError SyntaxError::dayCannotBeSmallerThanOne(const p_int line)
{
   return SyntaxError(U"the value of days cannot be smaller than 1", line);
}

SyntaxError SyntaxError::decrementationInsideExpression(const p_int line)
{
   return SyntaxError(U"the decrementation signs -- cannot appear inside an expression", line);
}

SyntaxError SyntaxError::expectedSemicolonBeforeKeyword(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"expected ; before the keyword \"", value, U"\""), line);
}

SyntaxError SyntaxError::expressionCannotEndWith(const p_char value, const p_int line)
{
   return SyntaxError(str(U"an expression cannot end with the ", intToString(value), U" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotEndWithFilterKeyword(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"an expression cannot end with the filter keyword \"", value, U"\""), line);
}

SyntaxError SyntaxError::expressionCannotStartWith(const p_char value, const p_int line)
{
   return SyntaxError(str(U"an expression cannot start with the ", intToString(value), U" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotStartWithIncrementation(const p_int line)
{
   return SyntaxError(U"an expression cannot start with the incrementation signs ++", line);
}

SyntaxError SyntaxError::expressionCannotStartWithDecrementation(const p_int line)
{
   return SyntaxError(U"an expression cannot start with the decrementation signs --", line);
}

SyntaxError SyntaxError::filterKeywordAtStart(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the filter keyword \"", value, U"\" is not preceded by a collection of values"), line);
}

SyntaxError SyntaxError::filterKeywordAtEnd(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the filter keyword \"", value, U"\" cannot stand at the end of an expression"), line);
}

SyntaxError SyntaxError::hoursOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the value of hours (", value, U") went out of range"), line);
}

SyntaxError SyntaxError::incrementationInsideExpression(const p_int line)
{
   return SyntaxError(U"the incrementation signs ++ cannot appear inside an expression", line);
}

SyntaxError SyntaxError::insteadOfYouShouldWrite(const p_str& value1, const p_str& value2, const p_int line)
{
   return SyntaxError(str(U"instead of \"", value1, U"\", you should write \"", value2, U"\""), line);
}

SyntaxError SyntaxError::invalidAsteriskPattern(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the asterisk pattern \"", value, U"\" is invalid"), line);
}

SyntaxError SyntaxError::invalidChar(const p_char value, const p_int line)
{
   switch (value) {
      case CHAR_CARET: {
         return SyntaxError(U"you should use the keyword \"xor\" instead of the character \"^\" as a boolean operator. "
            U"If your intention was to perform exponentiation, then the function \"power()\" is the right tool", line);
      }
      case CHAR_AMPERSAND: {
         return SyntaxError(U"you should use the keyword \"and\" instead of the character \"&\" as a boolean operator", line);
      }
      case CHAR_VERTICAL_BAR: {
         return SyntaxError(U"you should use the keyword \"or\" instead of the character \"|\" as a boolean operator", line);
      }
      default: {
         return SyntaxError(str(U"the character \"", charToString(value), U"\" is not allowed in ", metadata::NAME), line);
      }
   }
}

SyntaxError SyntaxError::invalidExpression(const p_int line)
{
   return SyntaxError(U"syntax of this expression is invalid", line);
}

SyntaxError SyntaxError::invalidFunctionName(const p_int line)
{
   return SyntaxError(U"the function name is invalid", line);
}

SyntaxError SyntaxError::invalidMonthName(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"\"", value, U"\" is not a valid month name"), line);
}

SyntaxError SyntaxError::invalidNumericalExpression(const p_int line)
{
   return SyntaxError(U"the syntax of a numerical expression is invalid", line);
}

SyntaxError SyntaxError::keywordNotFound(const p_int line)
{
   return SyntaxError(U"keyword not found", line);
}

SyntaxError SyntaxError::keywordNotFollowedByBool(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"tokens after the keyword \"",value, U"\" cannot be resolved to a logical condition"), line);
}

SyntaxError SyntaxError::keywordNotFollowedByNumber(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"tokens after the keyword \"",value, U"\" cannot be resolved to a number"), line);
}

SyntaxError SyntaxError::leftSideOfOperatorIsEmpty(const p_str& operator_, const p_int line)
{
   return SyntaxError(str(U"left side of the operator \"", operator_, U"\" is empty"), line);
}

SyntaxError SyntaxError::minutesOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the value of minutes (", value, U") went out of range"), line);
}

SyntaxError SyntaxError::missingTimeVariableMember(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"a time variable member was expected after \"", value, U"\""), line);
}

SyntaxError SyntaxError::missingLetterS(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"missing letter \"s\" at the end of the word \"", value, U"\""), line);
}

SyntaxError SyntaxError::monthHasFewerDays(const p_str& month, const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the month ", month, U" has only ", value, U" days"), line);
}

SyntaxError SyntaxError::multipleDotsInNumber(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the number \"", value, U"\" contains multiple dots"), line);
}

SyntaxError SyntaxError::multipleDotsInWord(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the word \"", value, U"\" cannot contain multiple dots"), line);
}

SyntaxError SyntaxError::negationByExclamation(const p_int line)
{
   return SyntaxError(U"you should use the keyword \"not\" instead of the character \"!\" for boolean negation", line);
}

SyntaxError SyntaxError::numberTooBig(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the number \"", value, U"\" is too big to be stored in the memory"), line);
}

SyntaxError SyntaxError::openedStringLteral(const p_int line)
{
   return SyntaxError(U"an opened string literal is not closed", line);
}

SyntaxError SyntaxError::operatorBetweenShouldBeFollowedByAnd(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the operator \"", value, U"\" should be followed by a keyword \"and\""), line);
}

SyntaxError SyntaxError::rightSideOfOperatorIsEmpty(const p_str& operator_, const p_int line)
{
   return SyntaxError(str(U"the right side of the operator \"", operator_, U"\" is empty"), line);
}

SyntaxError SyntaxError::secondsOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the value of seconds (", value, U") went out of range"), line);
}

SyntaxError SyntaxError::supposedUnintentionalAsteriskPattern(const p_str& value, const p_int line)
{  
   return SyntaxError(str(U"the syntax of this expression is invalid. The value \"", 
      value, 
      U"\" is an Asterisk Pattern. "
      U"If you want to treat it like a normal string, replace apostrophes \" with backtick characters `"), line);
}

SyntaxError SyntaxError::symbolNotFound(const p_char value, const p_int line)
{
   return SyntaxError(str(U"symbol \"", charToString(value), U"\" not found"), line);
}

SyntaxError SyntaxError::syntaxOfBooleanExpressionNotValid(const p_int line)
{
   return SyntaxError(U"syntax of this boolean expression is invalid", line);
}

SyntaxError SyntaxError::quotationMarkStringLteral(const p_int line)
{
   return SyntaxError(U"you should use apostrophes \" instead of quotation marks \" for string literals", line);
}

SyntaxError SyntaxError::youShouldUseApostrophesAndWrite(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"you should use apostrophes and write \"", value, U"\" instead"), line);
}

SyntaxError SyntaxError::undefinedVarValue(const p_str& value, const p_int line)
{
   return SyntaxError(str(U"the value of the variable \"", value, U"\" is undefined here"), line);
}

SyntaxError SyntaxError::unopenedBracketIsClosed(const p_char value, const p_int line)
{
   return SyntaxError(str(U"unopened bracket ", charToString(value), U" is closed"), line);
}

SyntaxError SyntaxError::wrongSyntax(const p_int line)
{
   return SyntaxError(U"wrong syntax. No valid command can be formed from this code", line);
}

SyntaxError SyntaxError::wrongSyntaxButProbablyAsteriskPattern(const p_int line)
{
   return SyntaxError(U"wrong syntax. You probably wanted to express an Asterisk Pattern. This notation is incorrect. "
      U"You should write it between two apostrophes like this: \"*.txt\"", line);
}


}
