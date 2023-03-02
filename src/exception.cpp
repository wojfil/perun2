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

#include "exception.h"
#include "datatype/chars.h"
#include "datatype/primitives.h"


namespace uro
{

SyntaxError::SyntaxError(const _str& msg, const _int li)
   : message(msg), line(li) { };

_str SyntaxError::getMessage() const
{
   return str(L"Error at line ", toStr(line), L": ", message, L".");
}

SyntaxError SyntaxError::adjacentSymbols(const _char& value, const _int line)
{
   return SyntaxError(str(L"adjacent ", toStr(value), L" symbols"), line);
}

SyntaxError SyntaxError::bracketIsNotClosed(const _char& value, const _int line)
{
   return SyntaxError(str(L"bracket ", toStr(value), L" is not closed"), line);
}

SyntaxError SyntaxError::bracketShouldBeClosedBeforeCurlyBracket(const _char& value, const _int line)
{
   return SyntaxError(str(L"bracket ", toStr(value), 
      L" should to be closed before the opening of curly bracket {"), line);
}

SyntaxError SyntaxError::adjacentFilterKeywords(const _str& value1, const _str& value2, const _int line)
{
   return SyntaxError(str(L"adjacent filter keywords '", value1, L"' and '", value2, L"'"), line);
}

SyntaxError SyntaxError::dayCannotBeSmallerThanOne(const _int line)
{
   return SyntaxError(L"day cannot be smaller than 1", line);
}

SyntaxError SyntaxError::decrementationInsideExpression(const _int line)
{
   return SyntaxError(L"decrementation signs -- cannot appear inside an expression", line);
}

SyntaxError SyntaxError::expectedSemicolonBeforeKeyword(const _str& value, const _int line)
{
   return SyntaxError(str(L"expected ; before keyword '", value, L"'"), line);
}

SyntaxError SyntaxError::expressionCannotEndWith(const _char& value, const _int line)
{
   return SyntaxError(str(L"expression cannot end with a ", toStr(value), L" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotEndWithFilterKeyword(const _str& value, const _int line)
{
   return SyntaxError(str(L"expression cannot end with a filter keyword '", value, L"'"), line);
}

SyntaxError SyntaxError::expressionCannotStartWith(const _char& value, const _int line)
{
   return SyntaxError(str(L"expression cannot start with a ", toStr(value), L" symbol"), line);
}

SyntaxError SyntaxError::expressionCannotStartWithIncrementation(const _int line)
{
   return SyntaxError(L"expression cannot start with incrementation signs ++", line);
}

SyntaxError SyntaxError::expressionCannotStartWithDecrementation(const _int line)
{
   return SyntaxError(L"expression cannot start with decrementation signs --", line);
}

SyntaxError SyntaxError::filterKeywordAtStart(const _str& value, const _int line)
{
   return SyntaxError(str(L"filter keyword '", value, L"' is not preceded by a collection of values"), line);
}

SyntaxError SyntaxError::filterKeywordAtEnd(const _str& value, const _int line)
{
   return SyntaxError(str(L"filter keyword '", value, L"' cannot stand at the end of an expression"), line);
}

SyntaxError SyntaxError::hoursOutOfRange(const _str& value, const _int line)
{
   return SyntaxError(str(L"value of hours (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::incrementationInsideExpression(const _int line)
{
   return SyntaxError(L"incrementation signs ++ cannot appear inside an expression", line);
}

SyntaxError SyntaxError::inevitableDivisionByZero(const _int line)
{
   return SyntaxError(L"inevitable division by zero", line);
}

SyntaxError SyntaxError::inevitableModuloByZero(const _int line)
{
   return SyntaxError(L"inevitable modulo by zero", line);
}

SyntaxError SyntaxError::invalidAsteriskPattern(const _str& value, const _int line)
{
   return SyntaxError(str(L"asterisk pattern '", value, L"' is not valid"), line);
}

SyntaxError SyntaxError::invalidChar(const _char& value, const _int line)
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
         return SyntaxError(str(L"character '", toStr(value), L"' is not allowed in Uroboros2"), line);
      }
   }
}

SyntaxError SyntaxError::invalidExpression(const _int line)
{
   return SyntaxError(L"syntax of an expression is not valid", line);
}

SyntaxError SyntaxError::invalidFunctionName(const _int line)
{
   return SyntaxError(L"function name is not valid", line);
}

SyntaxError SyntaxError::invalidMonthName(const _str& value, const _int line)
{
   return SyntaxError(str(L"'", value, L"' is not a valid month name"), line);
}

SyntaxError SyntaxError::invalidNumericalExpression(const _int line)
{
   return SyntaxError(L"syntax of a numerical expression is not valid", line);
}

SyntaxError SyntaxError::keywordNotFound(const _int line)
{
   return SyntaxError(L"keyword not found", line);
}

SyntaxError SyntaxError::keywordNotFollowedByBool(const _str& value, const _int line)
{
   return SyntaxError(str(L"tokens after keyword '",value, L"' cannot be resolved to a logical condition"), line);
}

SyntaxError SyntaxError::keywordNotFollowedByNumber(const _str& value, const _int line)
{
   return SyntaxError(str(L"tokens after keyword '",value, L"' cannot be resolved to a number"), line);
}

SyntaxError SyntaxError::minutesOutOfRange(const _str& value, const _int line)
{
   return SyntaxError(str(L"value of minutes (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::missingTimeVariableMember(const _str& value, const _int line)
{
   return SyntaxError(str(L"a time variable member was expected after '", value, L"'"), line);
}

SyntaxError SyntaxError::missingLetterS(const _str& value, const _int line)
{
   return SyntaxError(str(L"missing letter 's' at the end of the word '", value, L"'"), line);
}

SyntaxError SyntaxError::monthHasFewerDays(const _str& month, const _str& value, const _int line)
{
   return SyntaxError(str(L"month ", month, L" has only ", value, L" days"), line);
}

SyntaxError SyntaxError::multipleDotsInNumber(const _str& value, const _int line)
{
   return SyntaxError(str(L"number '", value, L"' contains multiple dots"), line);
}

SyntaxError SyntaxError::multipleDotsInWord(const _str& value, const _int line)
{
   return SyntaxError(str(L"word '", value, L"' cannot contain multiple dots"), line);
}

SyntaxError SyntaxError::negationByExclamation(const _int line)
{
   return SyntaxError(L"you should use keyword 'not' instead of character '!' for boolean negation", line);
}

SyntaxError SyntaxError::numberTooBig(const _str& value, const _int line)
{
   return SyntaxError(str(L"number '", value, L"' is too big to be stored in the memory"), line);
}

SyntaxError SyntaxError::openedStringLteral(const _int line)
{
   return SyntaxError(L"an opened string literal is not closed", line);
}

SyntaxError SyntaxError::secondsOutOfRange(const _str& value, const _int line)
{
   return SyntaxError(str(L"value of seconds (", value, L") went out of range"), line);
}

SyntaxError SyntaxError::symbolNotFound(const _char& value, const _int line)
{
   return SyntaxError(str(L"symbol '", toStr(value), L"' not found"), line);
}

SyntaxError SyntaxError::quotationMarkStringLteral(const _int line)
{
   return SyntaxError(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
}

SyntaxError SyntaxError::undefinedVarValue(const _str& value, const _int line)
{
   return SyntaxError(str(L"value of variable '", value, L"' is undefined here"), line);
}

SyntaxError SyntaxError::unopenedBracketIsClosed(const _char& value, const _int line)
{
   return SyntaxError(str(L"unopened bracket ", toStr(value), L" is closed"), line);
}

SyntaxError SyntaxError::wrongSyntax(const _int line)
{
   return SyntaxError(L"wrong syntax. No command can be formed of this code", line);
}


RuntimeError::RuntimeError(const _str& msg)
   : message(msg) { };

_str RuntimeError::getMessage() const
{
   return str(L"Runtime error: ", message, L".");
}

RuntimeError RuntimeError::divisionByZero()
{
   return RuntimeError(L"division by zero");
}

RuntimeError RuntimeError::moduloByZero()
{
   return RuntimeError(L"modulo by zero");
}

RuntimeError RuntimeError::numberTooBig(const _str& value)
{
   return RuntimeError(str(L"number '", value, L"' is too big to be stored in the memory"));
}

RuntimeError RuntimeError::squareRootOfNegativeNumber(const _str& value)
{
   return RuntimeError(str(L"square root of a negative number ", value));
}

RuntimeError RuntimeError::wrongResultOfExponentiation()
{
   return RuntimeError(L"result of exponentiation cannot be expressed");
}

}
