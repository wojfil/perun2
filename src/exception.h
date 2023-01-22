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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "datatype/primitives.h"


namespace uro
{

class SyntaxError
{
public:
   SyntaxError(const _str& msg, const _int& li);
   _str getMessage() const;

   static SyntaxError adjacentSymbols(const _char& value, const _int& line);
   static SyntaxError dayCannotBeSmallerThanOne(const _int& line);
   static SyntaxError expressionCannotEndWith(const _char& value, const _int& line);
   static SyntaxError expressionCannotStartWith(const _char& value, const _int& line);
   static SyntaxError filterKeywordAtStart(const _str& value, const _int& line);
   static SyntaxError filterKeywordAtEnd(const _str& value, const _int& line);
   static SyntaxError hoursOutOfRange(const _str& value, const _int& line);
   static SyntaxError inevitableDivisionByZero(const _int& line);
   static SyntaxError inevitableModuloByZero(const _int& line);
   static SyntaxError invalidAsteriskPattern(const _str& value, const _int& line);
   static SyntaxError invalidChar(const _char& value, const _int& line);
   static SyntaxError invalidExpression(const _int& line);
   static SyntaxError invalidFunctionName(const _int& line);
   static SyntaxError invalidMonthName(const _str& value, const _int& line);
   static SyntaxError invalidNumericalExpression(const _int& line);
   static SyntaxError keywordNotFollowedByBool(const _str& value, const _int& line);
   static SyntaxError keywordNotFollowedByNumber(const _str& value, const _int& line);
   static SyntaxError minutesOutOfRange(const _str& value, const _int& line);
   static SyntaxError missingTimeVariableMember(const _str& value, const _int& line);
   static SyntaxError missingLetterS(const _str& value, const _int& line);
   static SyntaxError monthHasFewerDays(const _str& month, const _str& value, const _int& line);
   static SyntaxError multipleDotsInNumber(const _str& value, const _int& line);
   static SyntaxError multipleDotsInWord(const _str& value, const _int& line);
   static SyntaxError numberTooBig(const _str& value, const _int& line);
   static SyntaxError openedStringLteral(const _int& line);
   static SyntaxError secondsOutOfRange(const _str& value, const _int& line);
   static SyntaxError quotationMarkStringLteral(const _int& line);

private:
   const _str message;
   const _int line;
};

class RuntimeError
{
public:
   RuntimeError(const _str& msg);
   _str getMessage() const;

   static RuntimeError divisionByZero();
   static RuntimeError moduloByZero();

private:
   const _str message;
};

}

#endif /* EXCEPTION_H */
