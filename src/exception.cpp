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

#include "exception.h"
#include "datatype/chars.h"
#include "datatype/primitives.h"


namespace uro
{

SyntaxException::SyntaxException(const _str& msg, const _int& li)
   : message(msg), line(li) { };

_str SyntaxException::getMessage() const
{
   return str(L"Error at line ", toStr(line), L": ", message, L".");
}



SyntaxException SyntaxException::filterKeywordAtStart(const _str& value, const _int& line)
{
   return SyntaxException(str(L"filter keyword '", value, L"' is not preceded by a collection of values"), line);
}

SyntaxException SyntaxException::filterKeywordAtEnd(const _str& value, const _int& line)
{
   return SyntaxException(str(L"filter keyword '", value, L"' cannot stand at the end of an expression"), line);
}

SyntaxException SyntaxException::inevitableDivisionByZero(const _int& line)
{
   return SyntaxException(L"inevitable division by zero", line);
}

SyntaxException SyntaxException::inevitableModuloByZero(const _int& line)
{
   return SyntaxException(L"inevitable modulo by zero", line);
}

SyntaxException SyntaxException::invalidAsteriskPattern(const _str& value, const _int& line)
{
   return SyntaxException(str(L"asterisk pattern '", value, L"' is not valid"), line);
}

SyntaxException SyntaxException::invalidExpression(const _int& line)
{
   return SyntaxException(L"syntax of an expression is not valid", line);
}

SyntaxException SyntaxException::invalidFunctionName(const _int& line)
{
   return SyntaxException(L"function name is not valid", line);
}
   
SyntaxException SyntaxException::invalidNumericalExpression(const _int& line)
{
   return SyntaxException(L"syntax of a numerical expression is not valid", line);
}
   
SyntaxException SyntaxException::invalidChar(const _char& value, const _int& line)
{
   switch (value) {
      case CHAR_CARET: {
         return SyntaxException(L"you should use keyword 'xor' instead of character '^' as a boolean operator. "
            L"If your intention was to perform exponentiation, then function 'power()' is the right tool", line);
      }
      case CHAR_AMPERSAND: {
         return SyntaxException(L"you should use keyword 'and' instead of character '&' as a boolean operator", line);
      }
      case CHAR_VERTICAL_BAR: {
         return SyntaxException(L"you should use keyword 'or' instead of character '|' as a boolean operator", line);
      }
      default: {
         return SyntaxException(str(L"character '", toStr(value), L"' is not allowed in Uroboros"), line);
      }
   }
}

SyntaxException SyntaxException::keywordNotFollowedByBool(const _str& value, const _int& line)
{
   return SyntaxException(str(L"tokens after keyword '",value, L"' cannot be resolved to a logical condition"), line);
}

SyntaxException SyntaxException::keywordNotFollowedByNumber(const _str& value, const _int& line)
{
   return SyntaxException(str(L"tokens after keyword '",value, L"' cannot be resolved to a number"), line);
}

SyntaxException SyntaxException::missingTimeVariableMember(const _str& value, const _int& line)
{
   return SyntaxException(str(L"a time variable member was expected after '", value, L"'"), line);
}

SyntaxException SyntaxException::multipleDotsInNumber(const _str& value, const _int& line)
{
   return SyntaxException(str(L"number '", value, L"' contains multiple dots"), line);
}

SyntaxException SyntaxException::multipleDotsInWord(const _str& value, const _int& line)
{
   return SyntaxException(str(L"word '", value, L"' cannot contain multiple dots"), line);
}

SyntaxException SyntaxException::numberTooBig(const _str& value, const _int& line)
{
   return SyntaxException(str(L"number '", value, L"' is too big to be stored in the memory"), line);
}

SyntaxException SyntaxException::openedStringLteral(const _int& line)
{
   return SyntaxException(L"an opened string literal is not closed", line);
}

SyntaxException SyntaxException::quotationMarkStringLteral(const _int& line)
{
   return SyntaxException(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
}



UroRuntimeException::UroRuntimeException(const _str& msg)
   : message(msg) { };

_str UroRuntimeException::getMessage() const
{
   return str(L"Runtime error: ", message, L".");
}


}
