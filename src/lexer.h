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

std::vector<Token> tokenize(const p_str &code, pp_perun2& p2);
static Token wordToken(const p_str& code, const p_size start, const p_size length, const p_int line, pp_perun2& p2);
inline static Token numberToken(const p_str& code, const p_str& value, const p_size start, const p_size length, 
   const p_nint multiplier, const NumberMode mode, const p_int dots, const p_int line, pp_perun2& p2);
inline static p_bool isSymbol(const p_char ch);
inline static p_bool isSpace(const p_char ch);
inline static p_bool isNewLine(const p_char ch);
inline static p_bool isAllowedInWord(const p_char ch);
inline static p_bool isDoubleChar(const p_char ch);
inline static p_nint fileSizeSuffixMulti(const p_char c1, const p_char c2);
inline static p_nint decimalSuffixMulti(const p_char c);

}

#endif /* LEXER_H */
