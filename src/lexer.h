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

#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include "perun2.h"

namespace perun2
{

std::vector<Token> tokenize(const _str &code, _p2& p2);
static Token wordToken(const _str& code, const _size start, const _size length, const _int line, _p2& p2);
inline static _bool isSymbol(const _char ch);
inline static _bool isSpace(const _char ch);
inline static _bool isNewLine(const _char ch);
inline static _bool isAllowedInWord(const _char ch);
inline static _bool isDoubleChar(const _char ch);
inline static _nint fileSizeSuffixMulti(const _char c1, const _char c2);
inline static _nint decimalSuffixMulti(const _char c);

}

#endif /* LEXER_H */
