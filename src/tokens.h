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

#ifndef TOKENS_H
#define TOKENS_H

#include "token.h"
#include <vector>

#define _pgunit _uint64

// Parse Guardians:
const _pgunit PG_NULL =         0b00000000000000000000000000000000;
const _pgunit PG_NO_COMMA =     0b00000000000000000000000000000001;
const _pgunit PG_NO _CMP =      0b00000000000000000000000000000010;

// what are parse guardians for?
// they provide memory for a sequence of tokens
// for example, if we checked once that this sequence does not contain any comma
// then we do not have to do that again - potential parsing possibilities are already narrowed

struct Tokens
{
public:

   Tokens(const Tokens& tks)
      : list(tks.list), start(tks.start), length(tks.length), end(tks.end){};
   Tokens(const std::vector<Token>* li);
   Tokens(const std::vector<Token>* li, _int ln);
   Tokens(const std::vector<Token>* li, _int st, _int ln);

   _int getStart() const;
   _int getLength() const;
   _int getEnd() const;
   _boo isEmpty() const;
   const Token& first() const;
   const Token& second() const;
   const Token& penultimate() const;
   const Token& last() const;
   const Token& at(const _int index) const;
   const Token& listAt(const _int index) const;
   void trimLeft();
   void expandLeft();
   void trimRight();
   void trimBoth();
   void trimFunction();
   void setRange(_int st, _int ln);

   _boo containsSymbol(const _char& ch) const;
   _boo containsComparisonSymbol() const;
   _boo containsKeyword(const Keyword& kw) const;
   _boo containsFilterKeyword() const;
   _boo hasIndependentBrackets() const;
   _boo hasIndependentSquareBrackets() const;
   void divideByKeyword(const Keyword& kw, Tokens& left, Tokens& right) const;
   void divideBySymbol(const _char& symbol, Tokens& left, Tokens& right) const;
   void splitBySymbol(const _char& symbol, std::vector<Tokens>& result) const;
   _int countSymbols(const _char& symbol) const;
   void splitByFiltherKeywords(std::vector<Tokens>& result) const;
   void divideForTernary(Tokens& condition, Tokens& left, Tokens& right) const;

   const std::vector<Token>* list;

private:
   _int length;
   _int start;
   _int end;
   _pgunit parseGuardian;
};

#endif /* TOKENS_H */
