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

#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include "uroboros.h"

namespace uro
{

std::vector<Token> tokenize(const _str &code, Uroboros& uro);
static Token wordToken(const _str& code, const _size& start, const _size& length, const _int& line, Uroboros& uro);
inline _bool isSymbol(const _char& ch);
_bool isNewLine(const _char& ch);
inline _bool isAllowedInWord(const _char& ch);
inline _bool isDoubleChar(const _char& ch);
inline void bigNumberException(const _str& code, const _size& start, const _size& length, const _int& line);
inline _ndouble stringToDouble(const _str& value);
_nint getSuffixMultiplier(const _char& c1, const _char& c2);
void invalidCharException(const _char& ch, const _int& line);

}

#endif /* LEXER_H */
