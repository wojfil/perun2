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
#include "guardian.h"
#include <vector>

struct Uroboros;

struct Tokens
{
public:
   Tokens(const Tokens& tks);
   Tokens(const std::vector<Token>* li);
   Tokens(const std::vector<Token>* li, const _int& ln);
   Tokens(const std::vector<Token>* li, const _int& st, const _int& ln);
   ~Tokens();

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
   void setRange(const _int& st, const _int& ln);

   _boo containsSymbol(const _pg_unit& unit) const;
   _boo containsKeyword(const _pg_unit& unit) const;
   _boo containsComparisonSymbol() const;
   _boo containsFilterKeyword() const;
   _boo hasIndependentBrackets() const;
   _boo hasIndependentSquareBrackets() const;
   _boo isPossibleFunction() const;
   _boo isPossibleListElement() const;
   _boo isPossibleBinary() const;
   _boo isPossibleTernary() const;
   _boo isPossibleListElementMember(Uroboros* uro) const;

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
   ParseGuardian* guardian;

   _boo containsChar(const _char& ch) const;
   _boo containsKeyword(const Keyword& kw) const;
};

#endif /* TOKENS_H */
