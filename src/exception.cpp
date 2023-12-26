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
   return SyntaxError(str(L"asterisk pattern '", value, 
      L"' cannot contain dot segments in the middle of a path"), line);
}

SyntaxError SyntaxError::bracketIsNotClosed(const p_char value, const p_int line)
{
   return SyntaxError(str(L"bracket ", toStr(value), L" is not closed"), line);
}

SyntaxError SyntaxError::bracketShouldBeClosedBeforeCurlyBracket(const p_char value, const p_int line)
{
   return SyntaxError(str(L"bracket ", toStr(value),
      L" should to be closed before the opening of curly bracket {"), line);
}

SyntaxError SyntaxError::adjacentFilterKeywords(const p_str& value1, const p_str& value2, const p_int line)
{
   return SyntaxError(str(L"adjacent filter keywords '", value1, L"' and '", value2, L"'"), line);
}

SyntaxError SyntaxError::dayCannotBeSmallerThanOne(const p_int line)
{
   return SyntaxError(L"day cannot be smaller than 1", line);
}

SyntaxError SyntaxError::decrementationInsideExpression(const p_int line)
{
   return SyntaxError(L"decrementation signs -- cannot appear inside an expression", line);
}

SyntaxError SyntaxError::expectedSemicolonBeforeKeyword(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"expected ; before keyword '", value, L"'"), line);
}

SyntaxError SyntaxError::expressionCannotEndWith(const p_char value, const p_int line)
{
   return SyntaxError(str(L"expression cannot end with a ", toStr(value), L" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotEndWithFilterKeyword(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"expression cannot end with a filter keyword '", value, L"'"), line);
}

SyntaxError SyntaxError::expressionCannotStartWith(const p_char value, const p_int line)
{
   return SyntaxError(str(L"expression cannot start with a ", toStr(value), L" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotStartWithIncrementation(const p_int line)
{
   return SyntaxError(L"expression cannot start with incrementation signs ++", line);
}

SyntaxError SyntaxError::expressionCannotStartWithDecrementation(const p_int line)
{
   return SyntaxError(L"expression cannot start with decrementation signs --", line);
}

SyntaxError SyntaxError::filterKeywordAtStart(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"filter keyword '", value, L"' is not preceded by a collection of values"), line);
}

SyntaxError SyntaxError::filterKeywordAtEnd(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"filter keyword '", value, L"' cannot stand at the end of an expression"), line);
}

SyntaxError SyntaxError::hoursOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"value of hours (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::incrementationInsideExpression(const p_int line)
{
   return SyntaxError(L"incrementation signs ++ cannot appear inside an expression", line);
}

SyntaxError SyntaxError::insteadOfYouShouldWrite(const p_str& value1, const p_str& value2, const p_int line)
{
   return SyntaxError(str(L"instead of '", value1, L"', you should write '", value2, L"'"), line);
}

SyntaxError SyntaxError::invalidAsteriskPattern(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"asterisk pattern '", value, L"' is not valid"), line);
}

SyntaxError SyntaxError::invalidChar(const p_char value, const p_int line)
{
   switch (value) {
      case CHAR_CARET: {
         return SyntaxError(L"you should use keyword 'xor' instead of character '^' as a boolean operator. "
            L"If your intention was to perform exponentiation, then function 'power()' is the right tool", line);
      }
      case CHAR_AMPERSAND: {
         return SyntaxError(L"you should use keyword 'and' instead of character '&' as a boolean operator", line);
      }
      case CHAR_VERTICAL_BAR: {
         return SyntaxError(L"you should use keyword 'or' instead of character '|' as a boolean operator", line);
      }
      default: {
         return SyntaxError(str(L"character '", toStr(value), L"' is not allowed in ", metadata::NAME), line);
      }
   }
}

SyntaxError SyntaxError::invalidExpression(const p_int line)
{
   return SyntaxError(L"syntax of an expression is not valid", line);
}

SyntaxError SyntaxError::invalidFunctionName(const p_int line)
{
   return SyntaxError(L"function name is not valid", line);
}

SyntaxError SyntaxError::invalidMonthName(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"'", value, L"' is not a valid month name"), line);
}

SyntaxError SyntaxError::invalidNumericalExpression(const p_int line)
{
   return SyntaxError(L"syntax of a numerical expression is not valid", line);
}

SyntaxError SyntaxError::keywordNotFound(const p_int line)
{
   return SyntaxError(L"keyword not found", line);
}

SyntaxError SyntaxError::keywordNotFollowedByBool(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"tokens after keyword '",value, L"' cannot be resolved to a logical condition"), line);
}

SyntaxError SyntaxError::keywordNotFollowedByNumber(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"tokens after keyword '",value, L"' cannot be resolved to a number"), line);
}

SyntaxError SyntaxError::leftSideOfOperatorIsEmpty(const p_str& operator_, const p_int line)
{
   return SyntaxError(str(L"left side of operator '", operator_, L"' is empty"), line);
}

SyntaxError SyntaxError::minutesOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"value of minutes (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::missingTimeVariableMember(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"a time variable member was expected after '", value, L"'"), line);
}

SyntaxError SyntaxError::missingLetterS(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"missing letter 's' at the end of the word '", value, L"'"), line);
}

SyntaxError SyntaxError::monthHasFewerDays(const p_str& month, const p_str& value, const p_int line)
{
   return SyntaxError(str(L"month ", month, L" has only ", value, L" days"), line);
}

SyntaxError SyntaxError::multipleDotsInNumber(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"number '", value, L"' contains multiple dots"), line);
}

SyntaxError SyntaxError::multipleDotsInWord(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"word '", value, L"' cannot contain multiple dots"), line);
}

SyntaxError SyntaxError::negationByExclamation(const p_int line)
{
   return SyntaxError(L"you should use keyword 'not' instead of character '!' for boolean negation", line);
}

SyntaxError SyntaxError::numberTooBig(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"number '", value, L"' is too big to be stored in the memory"), line);
}

SyntaxError SyntaxError::openedStringLteral(const p_int line)
{
   return SyntaxError(L"an opened string literal is not closed", line);
}

SyntaxError SyntaxError::rightSideOfOperatorIsEmpty(const p_str& operator_, const p_int line)
{
   return SyntaxError(str(L"right side of operator '", operator_, L"' is empty"), line);
}

SyntaxError SyntaxError::secondsOutOfRange(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"value of seconds (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::supposedUnintentionalAsteriskPattern(const p_str& value, const p_int line)
{  
   return SyntaxError(str(L"syntax of an expression is not valid. Value '", 
      value, 
      L"' is an Asterisk Pattern. "
      L"If you want to treat it like a normal string, replace apostrophes ' with backtick characters `"), line);
}

SyntaxError SyntaxError::symbolNotFound(const p_char value, const p_int line)
{
   return SyntaxError(str(L"symbol '", toStr(value), L"' not found"), line);
}

SyntaxError SyntaxError::syntaxOfBooleanExpressionNotValid(const p_int line)
{
   return SyntaxError(L"syntax of a boolean expression is not valid", line);
}

SyntaxError SyntaxError::quotationMarkStringLteral(const p_int line)
{
   return SyntaxError(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
}

SyntaxError SyntaxError::youShouldUseApostrophesAndWrite(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"you should use apostrophes and write '", value, L"' instead"), line);
}

SyntaxError SyntaxError::undefinedVarValue(const p_str& value, const p_int line)
{
   return SyntaxError(str(L"value of variable '", value, L"' is undefined here"), line);
}

SyntaxError SyntaxError::unopenedBracketIsClosed(const p_char value, const p_int line)
{
   return SyntaxError(str(L"unopened bracket ", toStr(value), L" is closed"), line);
}

SyntaxError SyntaxError::wrongSyntax(const p_int line)
{
   return SyntaxError(L"wrong syntax. No command can be formed of this code", line);
}

SyntaxError SyntaxError::wrongSyntaxButProbablyAsteriskPattern(const p_int line)
{
   return SyntaxError(L"wrong syntax. You probably wanted to express an Asterisk Pattern. This notation is incorrect. "
      L"You should write it between two apostrophes like this: '*.txt'", line);
}


}
