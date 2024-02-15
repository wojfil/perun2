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

#ifndef TOKENS_H
#define TOKENS_H

#include "token.h"
#include <vector>


namespace perun2
{

// Tokens' info about certain sequence of tokens
// this info is generated only once at the sequence creation
// and then read multiple times during parsing
typedef uint64_t     p_tinfo;

p_constexpr p_tinfo TI_NULL =                       0;
p_constexpr p_tinfo TI_HAS_COMPARISON_CHAR =        1 << 0;
p_constexpr p_tinfo TI_HAS_FILTER_KEYWORD =         1 << 1;
p_constexpr p_tinfo TI_HAS_INDEP_BRACKETS =         1 << 2;
p_constexpr p_tinfo TI_HAS_INDEP_SQ_BRACKETS =      1 << 3;
p_constexpr p_tinfo TI_HAS_CHAR_QUESTION_MARK =     1 << 4;
p_constexpr p_tinfo TI_HAS_CHAR_COMMA =             1 << 5;
p_constexpr p_tinfo TI_HAS_CHAR_COLON =             1 << 6;
p_constexpr p_tinfo TI_HAS_CHAR_PLUS =              1 << 7;
p_constexpr p_tinfo TI_HAS_CHAR_MINUS =             1 << 8;
p_constexpr p_tinfo TI_HAS_CHAR_EQUALS =            1 << 9;
p_constexpr p_tinfo TI_IS_POSSIBLE_FUNCTION =       1 << 10;
p_constexpr p_tinfo TI_HAS_KEYWORD_IN =             1 << 11;
p_constexpr p_tinfo TI_HAS_KEYWORD_LIKE =           1 << 12;
p_constexpr p_tinfo TI_HAS_KEYWORD_RESEMBLES =      1 << 13;
p_constexpr p_tinfo TI_HAS_KEYWORD_TO =             1 << 14;
p_constexpr p_tinfo TI_HAS_KEYWORD_WITH =           1 << 15;
p_constexpr p_tinfo TI_HAS_KEYWORD_AS =             1 << 16;
p_constexpr p_tinfo TI_IS_POSSIBLE_LIST_ELEM =      1 << 17;
p_constexpr p_tinfo TI_IS_POSSIBLE_BINARY =         1 << 18;
p_constexpr p_tinfo TI_IS_POSSIBLE_TERNARY =        1 << 19;
p_constexpr p_tinfo TI_IS_LIST_ELEM_MEMBER =        1 << 20;
p_constexpr p_tinfo TI_EVALUATE_DEFINITIONS =       1 << 21;
p_constexpr p_tinfo TI_HAS_CHAR_PERCENT =           1 << 22;


struct Perun2Process;

// this is a subsequence of vector of tokens
// it has a reference to the origin vector, some indexes and info
struct Tokens
{
public:
   Tokens() = delete;
   Tokens(const std::vector<Token>& li, const p_int st, const p_int ln, const p_tinfo in);
   Tokens(const std::vector<Token>& li);
   Tokens(const Tokens& tks, const p_int st, const p_int ln);

   // shorten this sequence by one
   // remove the first or the last token
   void trimLeft();
   void trimRight();

   p_int getStart() const;
   p_int getLength() const;
   p_int getEnd() const;
   p_int getEmbracement() const;
   const std::vector<Token>& getList() const;
   p_tinfo getInfo() const;
   p_bool check(const p_tinfo in) const;
   p_bool hasBinaryBoolKeyword() const;
   p_bool isEmpty() const;
   const Token& first() const;
   const Token& second() const;
   const Token& penultimate() const;
   const Token& last() const;
   const Token& at(const p_int index) const;
   const Token& listAt(const p_int index) const;

   std::pair<Tokens, Tokens> divideByKeyword(const Keyword kw) const;
   std::pair<Tokens, Tokens> divideBySymbol(const p_char symbol) const;
   std::vector<Tokens> splitBySymbol(const p_char symbol) const;
   p_int countSymbols(const p_char symbol) const;
   p_int getFilterKeywordId(Perun2Process& p2) const;
   std::vector<Tokens> splitByFiltherKeywords(Perun2Process& p2) const;
   std::tuple<Tokens, Tokens, Tokens> divideForTernary() const;

   void checkCommonExpressionExceptions(Perun2Process& p2) const;

private:
   p_int start;
   p_int length;
   p_int end;
   const std::vector<Token>& list;
   p_tinfo info = TI_NULL;

   void setData();
};

}

#endif /* TOKENS_H */
