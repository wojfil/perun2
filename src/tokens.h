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

#ifndef TOKENS_H
#define TOKENS_H

#include "token.h"
#include <vector>


namespace perun2
{

// Tokens' info about certain sequence of tokens
// this info is generated only once at the sequence creation
// and then read multiple times during parsing
typedef uint64_t p_tinfo;

p_constexpr p_tinfo TI_NULL =                    0b00000000000000000000000;
p_constexpr p_tinfo TI_HAS_COMPARISON_CHAR =     0b00000000000000000000001;
p_constexpr p_tinfo TI_HAS_FILTER_KEYWORD =      0b00000000000000000000010;
p_constexpr p_tinfo TI_HAS_INDEP_BRACKETS =      0b00000000000000000000100;
p_constexpr p_tinfo TI_HAS_INDEP_SQ_BRACKETS =   0b00000000000000000001000;
p_constexpr p_tinfo TI_HAS_CHAR_QUESTION_MARK =  0b00000000000000000010000;
p_constexpr p_tinfo TI_HAS_CHAR_COMMA =          0b00000000000000000100000;
p_constexpr p_tinfo TI_HAS_CHAR_COLON =          0b00000000000000001000000;
p_constexpr p_tinfo TI_HAS_CHAR_PLUS =           0b00000000000000010000000;
p_constexpr p_tinfo TI_HAS_CHAR_MINUS =          0b00000000000000100000000;
p_constexpr p_tinfo TI_HAS_CHAR_EQUALS =         0b00000000000001000000000;
p_constexpr p_tinfo TI_IS_POSSIBLE_FUNCTION =    0b00000000000010000000000;
p_constexpr p_tinfo TI_HAS_KEYWORD_IN =          0b00000000000100000000000;
p_constexpr p_tinfo TI_HAS_KEYWORD_LIKE =        0b00000000001000000000000;
p_constexpr p_tinfo TI_HAS_KEYWORD_TO =          0b00000000010000000000000;
p_constexpr p_tinfo TI_HAS_KEYWORD_WITH =        0b00000000100000000000000;
p_constexpr p_tinfo TI_HAS_KEYWORD_AS =          0b00000001000000000000000;
p_constexpr p_tinfo TI_IS_POSSIBLE_LIST_ELEM =   0b00000010000000000000000;
p_constexpr p_tinfo TI_IS_POSSIBLE_BINARY =      0b00000100000000000000000;
p_constexpr p_tinfo TI_IS_POSSIBLE_TERNARY =     0b00001000000000000000000;
p_constexpr p_tinfo TI_IS_LIST_ELEM_MEMBER =     0b00010000000000000000000;
p_constexpr p_tinfo TI_EVALUATE_DEFINITIONS =    0b00100000000000000000000;


struct p_perun2;

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
   p_int getFilterKeywordId(p_perun2& p2) const;
   std::vector<Tokens> splitByFiltherKeywords(p_perun2& p2) const;
   std::tuple<Tokens, Tokens, Tokens> divideForTernary() const;

   void checkCommonExpressionExceptions(p_perun2& p2) const;

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
