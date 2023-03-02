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

#ifndef TOKENS_H
#define TOKENS_H

#include "token.h"
#include <vector>


namespace uro
{

// Tokens info about certain sequence of tokens
// this info is generate once at the sequence creation
// and then used multiple times during parsing
typedef _uint64 _tinfo;

inline constexpr _tinfo TI_NULL =                    0b00000000000000000000000;
inline constexpr _tinfo TI_HAS_COMPARISON_CHAR =     0b00000000000000000000001;
inline constexpr _tinfo TI_HAS_FILTER_KEYWORD =      0b00000000000000000000010;
inline constexpr _tinfo TI_HAS_INDEP_BRACKETS =      0b00000000000000000000100;
inline constexpr _tinfo TI_HAS_INDEP_SQ_BRACKETS =   0b00000000000000000001000;
inline constexpr _tinfo TI_HAS_CHAR_QUESTION_MARK =  0b00000000000000000010000;
inline constexpr _tinfo TI_HAS_CHAR_COMMA =          0b00000000000000000100000;
inline constexpr _tinfo TI_HAS_CHAR_COLON =          0b00000000000000001000000;
inline constexpr _tinfo TI_HAS_CHAR_PLUS =           0b00000000000000010000000;
inline constexpr _tinfo TI_HAS_CHAR_MINUS =          0b00000000000000100000000;
inline constexpr _tinfo TI_HAS_CHAR_EQUALS =         0b00000000000001000000000;
inline constexpr _tinfo TI_IS_POSSIBLE_FUNCTION =    0b00000000000010000000000;
inline constexpr _tinfo TI_HAS_KEYWORD_IN =          0b00000000000100000000000;
inline constexpr _tinfo TI_HAS_KEYWORD_LIKE =        0b00000000001000000000000;
inline constexpr _tinfo TI_HAS_KEYWORD_TO =          0b00000000010000000000000;
inline constexpr _tinfo TI_HAS_KEYWORD_WITH =        0b00000000100000000000000;
inline constexpr _tinfo TI_HAS_KEYWORD_AS =          0b00000001000000000000000;
inline constexpr _tinfo TI_HAS_KEYWORD_FROM =        0b00000010000000000000000;
inline constexpr _tinfo TI_IS_POSSIBLE_LIST_ELEM =   0b00000100000000000000000;
inline constexpr _tinfo TI_IS_POSSIBLE_BINARY =      0b00001000000000000000000;
inline constexpr _tinfo TI_IS_POSSIBLE_TERNARY =     0b00010000000000000000000;
inline constexpr _tinfo TI_IS_LIST_ELEM_MEMBER =     0b00100000000000000000000;


struct _uro;

struct Tokens
{
public:
   Tokens() = delete;
   Tokens(const std::vector<Token>& li, const _int st, const _int ln, const _tinfo& in);
   Tokens(const std::vector<Token>& li);
   Tokens(const Tokens& tks, const _int st, const _int ln);

   void trimLeft();
   void trimRight();

   _int getStart() const;
   _int getLength() const;
   _int getEnd() const;
   _int getEmbracement() const;
   const std::vector<Token>& getList() const;
   _tinfo getInfo() const;
   _bool check(const _tinfo& in) const;
   _bool isEmpty() const;
   const Token& first() const;
   const Token& second() const;
   const Token& penultimate() const;
   const Token& last() const;
   const Token& at(const _int index) const;
   const Token& listAt(const _int index) const;

   std::pair<Tokens, Tokens> divideByKeyword(const Keyword& kw) const;
   std::pair<Tokens, Tokens> divideBySymbol(const _char& symbol) const;
   std::vector<Tokens> splitBySymbol(const _char& symbol) const;
   _int countSymbols(const _char& symbol) const;
   _int getFilterKeywordId(_uro& uro) const;
   std::vector<Tokens> splitByFiltherKeywords(_uro& uro) const;
   std::tuple<Tokens, Tokens, Tokens> divideForTernary() const;

   void checkCommonExpressionExceptions(_uro& uro) const;

private:
   _int start;
   _int length;
   _int end;
   const std::vector<Token>& list;
   _tinfo info = TI_NULL;

   void setData();
};

}

#endif /* TOKENS_H */
