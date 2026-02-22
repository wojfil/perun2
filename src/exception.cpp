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
   return str(L"Error at line ", toStr(line), L": ", message, L".");
}

SyntaxError SyntaxError::adjacentSymbols(const p_char value, const p_int line)
{
   return SyntaxError(str(L"adjacent ", toStr(value), L" symbols"), line);
}

SyntaxError SyntaxError::asteriskPatternCannotContainDotSegments(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the asterisk pattern \"", value, 
      L"\" cannot contain dot segments in the middle of a path"), line);
}

SyntaxError SyntaxError::asteriskIsNotWildcardInLikeOperator(const p_str& value, const p_int line)
{
   p_str proper = value;
   std::replace(proper.begin(), proper.end(), CHAR_ASTERISK, gen::WILDCARD_MULTIPLE_CHARS);

   return SyntaxError(str(L"the asterisk in not a wildcard character of the Like operator. You should write \"", 
      proper,  L"\" instead"), line);
}

SyntaxError SyntaxError::bracketIsNotClosed(const p_char value, const p_int line)
{
   return SyntaxError(str(L"the bracket ", toStr(value), L" is not closed"), line);
}

SyntaxError SyntaxError::bracketShouldBeClosedBeforeCurlyBracket(const p_char value, const p_int line)
{
   return SyntaxError(str(L"the bracket ", toStr(value),
      L" should to be closed before the opening of the curly bracket {"), line);
}

SyntaxError SyntaxError::adjacentFilterKeywords(const p_str& value1, const p_str& value2, const p_int line)
{
   return SyntaxError(str(L"adjacent filter keywords \"", value1, L"\" and \"", value2, L"\""), line);
}

SyntaxError SyntaxError::dayCannotBeSmallerThanOne(const p_int line)
{
   return SyntaxError(L"the value of days cannot be smaller than 1", line);
}

SyntaxError SyntaxError::decrementationInsideExpression(const p_int line)
{
   return SyntaxError(L"the decrementation signs -- cannot appear inside an expression", line);
}

SyntaxError SyntaxError::expectedSemicolonBeforeKeyword(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"expected ; before the keyword \"", value, L"\""), line);
}

SyntaxError SyntaxError::expressionCannotEndWith(const p_char value, const p_int line)
{
   return SyntaxError(str(L"an expression cannot end with the ", toStr(value), L" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotEndWithFilterKeyword(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"an expression cannot end with the filter keyword \"", value, L"\""), line);
}

SyntaxError SyntaxError::expressionCannotStartWith(const p_char value, const p_int line)
{
   return SyntaxError(str(L"an expression cannot start with the ", toStr(value), L" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotStartWithIncrementation(const p_int line)
{
   return SyntaxError(L"an expression cannot start with the incrementation signs ++", line);
}

SyntaxError SyntaxError::expressionCannotStartWithDecrementation(const p_int line)
{
   return SyntaxError(L"an expression cannot start with the decrementation signs --", line);
}

SyntaxError SyntaxError::filterKeywordAtStart(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the filter keyword \"", value, L"\" is not preceded by a collection of values"), line);
}

SyntaxError SyntaxError::filterKeywordAtEnd(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the filter keyword \"", value, L"\" cannot stand at the end of an expression"), line);
}

SyntaxError SyntaxError::hoursOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the value of hours (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::incrementationInsideExpression(const p_int line)
{
   return SyntaxError(L"the incrementation signs ++ cannot appear inside an expression", line);
}

SyntaxError SyntaxError::insteadOfYouShouldWrite(const p_str& value1, const p_str& value2, const p_int line)
{
   return SyntaxError(str(L"instead of \"", value1, L"\", you should write \"", value2, L"\""), line);
}

SyntaxError SyntaxError::invalidAsteriskPattern(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the asterisk pattern \"", value, L"\" is invalid"), line);
}

SyntaxError SyntaxError::invalidChar(const p_char value, const p_int line)
{
   switch (value) {
      case CHAR_CARET: {
         return SyntaxError(L"you should use the keyword \"xor\" instead of the character \"^\" as a boolean operator. "
            L"If your intention was to perform exponentiation, then the function \"power()\" is the right tool", line);
      }
      case CHAR_AMPERSAND: {
         return SyntaxError(L"you should use the keyword \"and\" instead of the character \"&\" as a boolean operator", line);
      }
      case CHAR_VERTICAL_BAR: {
         return SyntaxError(L"you should use the keyword \"or\" instead of the character \"|\" as a boolean operator", line);
      }
      default: {
         return SyntaxError(str(L"the character \"", toStr(value), L"\" is not allowed in ", metadata::NAME), line);
      }
   }
}

SyntaxError SyntaxError::invalidExpression(const p_int line)
{
   return SyntaxError(L"syntax of this expression is invalid", line);
}

SyntaxError SyntaxError::invalidFunctionName(const p_int line)
{
   return SyntaxError(L"the function name is invalid", line);
}

SyntaxError SyntaxError::invalidMonthName(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"\"", value, L"\" is not a valid month name"), line);
}

SyntaxError SyntaxError::invalidNumericalExpression(const p_int line)
{
   return SyntaxError(L"the syntax of a numerical expression is invalid", line);
}

SyntaxError SyntaxError::keywordNotFound(const p_int line)
{
   return SyntaxError(L"keyword not found", line);
}

SyntaxError SyntaxError::keywordNotFollowedByBool(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"tokens after the keyword \"",value, L"\" cannot be resolved to a logical condition"), line);
}

SyntaxError SyntaxError::keywordNotFollowedByNumber(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"tokens after the keyword \"",value, L"\" cannot be resolved to a number"), line);
}

SyntaxError SyntaxError::leftSideOfOperatorIsEmpty(const p_str& operator_, const p_int line)
{
   return SyntaxError(str(L"left side of the operator \"", operator_, L"\" is empty"), line);
}

SyntaxError SyntaxError::minutesOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the value of minutes (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::missingTimeVariableMember(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"a time variable member was expected after \"", value, L"\""), line);
}

SyntaxError SyntaxError::missingLetterS(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"missing letter \"s\" at the end of the word \"", value, L"\""), line);
}

SyntaxError SyntaxError::monthHasFewerDays(const p_str& month, const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the month ", month, L" has only ", value, L" days"), line);
}

SyntaxError SyntaxError::multipleDotsInNumber(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the number \"", value, L"\" contains multiple dots"), line);
}

SyntaxError SyntaxError::multipleDotsInWord(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the word \"", value, L"\" cannot contain multiple dots"), line);
}

SyntaxError SyntaxError::negationByExclamation(const p_int line)
{
   return SyntaxError(L"you should use the keyword \"not\" instead of the character \"!\" for boolean negation", line);
}

SyntaxError SyntaxError::numberDotOnly(const p_int line)
{
   return SyntaxError(str(L"the character . is not part of any expression"), line);
}

SyntaxError SyntaxError::numberEndingWithDot(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the number declaration \"", value, 
      L"\" is not clear enough. It should be written as \"", value, L"0\""), line);
}

SyntaxError SyntaxError::numberStartingWithDot(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the number declaration \"", value, 
      L"\" is not clear enough. It should be written as \"0", value, L"\""), line);
}

SyntaxError SyntaxError::numberTooBig(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the number \"", value, L"\" is too big to be stored in the memory"), line);
}

SyntaxError SyntaxError::openedStringLteral(const p_int line)
{
   return SyntaxError(L"an opened string literal is not closed", line);
}

SyntaxError SyntaxError::operatorBetweenShouldBeFollowedByAnd(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the operator \"", value, L"\" should be followed by a keyword \"and\""), line);
}

SyntaxError SyntaxError::rightSideOfOperatorIsEmpty(const p_str& operator_, const p_int line)
{
   return SyntaxError(str(L"the right side of the operator \"", operator_, L"\" is empty"), line);
}

SyntaxError SyntaxError::secondsOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the value of seconds (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::supposedUnintentionalAsteriskPattern(const p_str& value, const p_int line)
{  
   return SyntaxError(str(L"the syntax of this expression is invalid. The value \"", 
      value, 
      L"\" is an Asterisk Pattern. "
      L"If you want to treat it like a normal string, replace apostrophes \" with backtick characters `"), line);
}

SyntaxError SyntaxError::symbolNotFound(const p_char value, const p_int line)
{
   return SyntaxError(str(L"symbol \"", toStr(value), L"\" not found"), line);
}

SyntaxError SyntaxError::syntaxOfBooleanExpressionNotValid(const p_int line)
{
   return SyntaxError(L"syntax of this boolean expression is invalid", line);
}

SyntaxError SyntaxError::quotationMarkStringLteral(const p_int line)
{
   return SyntaxError(L"you should use apostrophes \" instead of quotation marks \" for string literals", line);
}

SyntaxError SyntaxError::youShouldUseApostrophesAndWrite(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"you should use apostrophes and write \"", value, L"\" instead"), line);
}

SyntaxError SyntaxError::undefinedVarValue(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"the value of the variable \"", value, L"\" is undefined here"), line);
}

SyntaxError SyntaxError::unopenedBracketIsClosed(const p_char value, const p_int line)
{
   return SyntaxError(str(L"unopened bracket ", toStr(value), L" is closed"), line);
}

SyntaxError SyntaxError::wrongSyntax(const p_int line)
{
   return SyntaxError(L"wrong syntax. No valid command can be formed from this code", line);
}

SyntaxError SyntaxError::wrongSyntaxButProbablyAsteriskPattern(const p_int line)
{
   return SyntaxError(L"wrong syntax. You probably wanted to express an Asterisk Pattern. This notation is incorrect. "
      L"You should write it between two apostrophes like this: \"*.txt\"", line);
}


}
