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

#include "tokens.h"
#include "brackets.h"
#include "exception.h"
#include "lexer.h"
#include "uroboros.h"


namespace uro
{

Tokens::Tokens(const Tokens& tks)
   : list(tks.list), start(tks.getStart()), length(tks.getLength()),
     end(tks.getEnd()), guardian(new ParseGuardian()) { };

Tokens::Tokens(const std::vector<Token>* li)
   : list(li), length(li->size()), start(0),
     end(this->length - 1), guardian(new ParseGuardian()) { };

Tokens::Tokens(const std::vector<Token>* li, const _int& ln)
   : list(li), length(ln), start(0),
     end(this->length - 1), guardian(new ParseGuardian()) { };

Tokens::Tokens(const std::vector<Token>* li, const _int& st, const _int& ln)
   : list(li), length(ln), start(st),
     end(this->start + this->length - 1), guardian(new ParseGuardian()) { };

Tokens::~Tokens()
{
   delete guardian;
};

_int Tokens::getStart() const
{
   return start;
};

_int Tokens::getLength() const
{
   return length;
};

_int Tokens::getEnd() const
{
   return end;
};

_boo Tokens::isEmpty() const
{
   return length == 0;
};

const Token& Tokens::first() const
{
   return (*list)[start];
};

const Token& Tokens::second() const
{
   return (*list)[start + 1];
};

const Token& Tokens::penultimate() const
{
   return (*list)[end - 1];
};

const Token& Tokens::last() const
{
   return (*list)[end];
};

const Token& Tokens::at(const _int index) const
{
   return (*list)[start + index];
};

const Token& Tokens::listAt(const _int index) const
{
   return (*list)[index];
};

void Tokens::trimLeft()
{
   if (length != 0) {
      start++;
      length--;
   }
};

void Tokens::expandLeft()
{
   start--;
   length++;
};

void Tokens::trimRight()
{
   if (length != 0) {
      end--;
      length--;
   }
};

void Tokens::trimBoth()
{
   if (length >= 2) {
      start++;
      end--;
      length -= 2;
   }
};

void Tokens::trimFunction()
{
   if (length >= 3) {
      start += 2;
      end--;
      length -= 3;
   }
};

void Tokens::setRange(const _int& st, const _int& ln)
{
   start = st;
   length = ln;
   end = start + length - 1;
}

_boo Tokens::containsChar(const _char& ch) const
{
   BracketsInfo bi;

   for (_int i = start; i <= end; i++){
      const Token& t = listAt(i);
      if (t.type != Token::t_Symbol) {
         continue;
      }
      if (t.value.ch == ch && bi.isBracketFree()) {
         return true;
      }
      bi.refresh(t);
   }

   return false;
}

_boo Tokens::containsSymbol(const _pg_unit& unit) const
{
   if (guardian->knows(unit)) {
      return guardian->protects(unit);
   }

   const _boo contains = containsChar(guardian->unitToChar(unit));
   guardian->set(unit, contains);

   return contains;
}

_boo Tokens::containsKeyword(const Keyword& kw) const
{
   BracketsInfo bi;

   for (_int i = start; i <= end; i++){
      const Token& t = listAt(i);
      if (t.type == Token::t_Keyword && t.value.keyword.k == kw && bi.isBracketFree()) {
         return true;
      }
      bi.refresh(t);
   }

   return false;
}

_boo Tokens::containsKeyword(const _pg_unit& unit) const
{
   if (guardian->knows(unit)) {
      return guardian->protects(unit);
   }

   const _boo contains = containsKeyword(guardian->unitToKeyword(unit));
   guardian->set(unit, contains);

   return contains;
}

_boo Tokens::containsComparisonSymbol() const
{
   if (guardian->knows(PG_COMPARISON_CHAR)) {
      return guardian->protects(PG_COMPARISON_CHAR);
   }

   BracketsInfo bi;
   _boo result = false;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);

      if (t.type == Token::t_Symbol && bi.isBracketFree()) {
         switch (t.value.ch) {
            case L'<':
            case L'>':
            case L'!':
            case L'=': {
               result = true;
               goto cs_exit;
               break;
            }
         }
      }

      bi.refresh(t);
   }

cs_exit:

   guardian->set(PG_COMPARISON_CHAR, result);
   return result;
}

_boo Tokens::containsFilterKeyword() const
{
   if (guardian->knows(PG_FILTER_KEYWORD)) {
      return guardian->protects(PG_FILTER_KEYWORD);
   }

   BracketsInfo bi;
   _boo result = false;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);
      if (t.isFiltherKeyword() && bi.isBracketFree()) {
         result = true;
         break;
      }
      bi.refresh(t);
   }

   guardian->set(PG_FILTER_KEYWORD, result);
   return result;
}

// independent are:  () () ()
// not independent:  ( () () )
_boo Tokens::hasIndependentBrackets() const
{
   if (guardian->knows(PG_INDEP_BRACKETS)) {
      return guardian->protects(PG_INDEP_BRACKETS);
   }

   _int lvl = 0;
   _boo result = false;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);
      if (t.type == Token::t_Symbol){
         switch(t.value.ch) {
            case L'(': {
               lvl++;
               break;
            }
            case L')': {
               lvl--;
               if (lvl == 0 && i != end) {
                  result = true;
                  goto ib_exit;
               }
               break;
            }
         }
      }
   }

ib_exit:

   guardian->set(PG_INDEP_BRACKETS, result);
   return result;
}

// independent are:  [] [] []
// not independent:  [ [] [] ]
_boo Tokens::hasIndependentSquareBrackets() const
{
   if (guardian->knows(PG_INDEP_SQ_BRACKETS)) {
      return guardian->protects(PG_INDEP_SQ_BRACKETS);
   }

   _boo first = false;
   _boo result = false;
   _int lvl = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);
      if (t.type == Token::t_Symbol){
         switch(t.value.ch) {
            case L'[': {
               lvl++;
               if (first && lvl == 1) {
                  result = true;
                  goto isb_exit;
               }
               break;
            }
            case L']': {
               lvl--;
               if (!first && lvl == 0) {
                  first = true;
               }
               break;
            }
         }
      }
   }

isb_exit:

   guardian->set(PG_INDEP_SQ_BRACKETS, result);
   return result;
}

_boo Tokens::isPossibleFunction() const
{
   if (guardian->knows(PG_POSSIBLE_FUNCTION)) {
      return guardian->protects(PG_POSSIBLE_FUNCTION);
   }

   if (length < 3) {
      guardian->set(PG_POSSIBLE_FUNCTION, false);
      return false;
   }

   const Token& f = first();
   const Token& s = second();
   const Token& l = last();

   const _boo result = f.type == Token::t_Word
       && s.type == Token::t_Symbol
       && l.type == Token::t_Symbol
       && s.value.ch == L'('
       && l.value.ch == L')'
       && !hasIndependentBrackets();

   guardian->set(PG_POSSIBLE_FUNCTION, result);
   return result;
}

_boo Tokens::isPossibleListElement() const
{
   if (length <= 2 || !second().isSymbol(L'[')
      || !last().isSymbol(L']') || hasIndependentSquareBrackets())
   {
      guardian->set(PG_POSSIBLE_LIST_ELEM, false);
      return false;
   }

   if (length == 3) {
      throw SyntaxException(L"empty space between square brackets []",
         last().line);
   }

   if (first().type != Token::t_Word) {
      throw SyntaxException(
         L"square brackets [] can be preceded only by a variable name",
         first().line);
   }

   guardian->set(PG_POSSIBLE_LIST_ELEM, true);
   return true;
}

_boo Tokens::isPossibleBinary() const
{
   if (guardian->knows(PG_POSSIBLE_BINARY)) {
      return guardian->protects(PG_POSSIBLE_BINARY);
   }

   if (!containsSymbol(PG_CHAR_QUESTION_MARK) || containsSymbol(PG_CHAR_COLON)) {
      guardian->set(PG_POSSIBLE_BINARY, false);
      return false;
   }

   BracketsInfo bi;
   _int i;

   for (i = start; i <= end; i++){
      const Token& t = listAt(i);

      if (bi.isBracketFree() && t.type == Token::t_Symbol && t.value.ch == L'?') {
         break;
      }

      bi.refresh(t);
   }

   if (i == start) {
      throw SyntaxException(L"sign ? is preceded by empty space",
         (*list)[i].line);
   }
   else if (i == end) {
      throw SyntaxException(L"sign ? is followed by empty space",
         (*list)[i].line);
   }

   guardian->set(PG_POSSIBLE_BINARY, true);
   return true;
}

_boo Tokens::isPossibleTernary() const
{
   if (guardian->knows(PG_POSSIBLE_TERNARY)) {
      return guardian->protects(PG_POSSIBLE_TERNARY);
   }

   if (!containsSymbol(PG_CHAR_QUESTION_MARK) || !containsSymbol(PG_CHAR_COLON)) {
      guardian->set(PG_POSSIBLE_TERNARY, false);
      return false;
   }

   BracketsInfo bi;
   _boo loop = true;
   _int percentId = -1;
   _int colonId = -1;

   for (_int i = start; loop && i <= end; i++) {
      const Token& t = listAt(i);

      if (bi.isBracketFree() && t.type == Token::t_Symbol) {
         switch (t.value.ch) {
            case L'?': {
               if (percentId == -1) {
                  percentId = i;
                  if (colonId != -1) {
                     loop = false;
                  }
               }
               else {
                  guardian->set(PG_POSSIBLE_TERNARY, false);
                  return false;
               }
               break;
            }
            case L':': {
               if (colonId == -1) {
                  colonId = i;
                  if (percentId != -1) {
                     loop = false;
                  }
               }
               break;
            }
         }
      }

      bi.refresh(t);
   }

   if (percentId > colonId) {
      throw SyntaxException(L"signs ? and : appear in reverse order",
         (*list)[percentId].line);
   }
   else if (percentId == start) {
      throw SyntaxException(L"sign ? is preceded by empty space",
         (*list)[percentId].line);
   }
   else if (colonId == end) {
      throw SyntaxException(L"sign : is followed by empty space",
         (*list)[colonId].line);
   }
   else if (percentId + 1 == colonId) {
      throw SyntaxException(L"empty space between signs ? and :",
         (*list)[percentId].line);
   }

   guardian->set(PG_POSSIBLE_TERNARY, true);
   return true;
}

_boo Tokens::isPossibleListElementMember(Uroboros* uro) const
{
   if (length <= 3 || !second().isSymbol(L'[')
      || !penultimate().isSymbol(L']')
      || hasIndependentSquareBrackets())
   {
      guardian->set(PG_LIST_ELEM_MEMBER, false);
      return false;
   }

   if (length == 3) {
      throw SyntaxException(L"empty space between square brackets []",
         last().line);
   }

   const Token& lst = last();

   if (lst.type != Token::t_TwoWords) {
      guardian->set(PG_LIST_ELEM_MEMBER, false);
      return false;
   }

   if (lst.value.twoWords.h1 != uro->hashes.HASH_NOTHING) {
      throw SyntaxException(L"square brackets [] should be followed by a time variable member",
         lst.line);
   }

   if (first().type != Token::t_Word) {
      throw SyntaxException(L"square brackets [] can be preceded only by a variable name",
         first().line);
   }

   guardian->set(PG_LIST_ELEM_MEMBER, true);
   return true;
}

void Tokens::divideByKeyword(const Keyword& kw, Tokens& left, Tokens& right) const
{
   BracketsInfo bi;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);

      if (t.isKeyword(kw)) {
         if (bi.isBracketFree()) {
            left.setRange(start, i - start);
            right.setRange(i + 1, length + start - i - 1);
            return;
         }
      }
      else {
         bi.refresh(t);
      }
   }
}

void Tokens::divideBySymbol(const _char& symbol, Tokens& left, Tokens& right) const
{
   BracketsInfo bi;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);

      if (t.isSymbol(symbol)) {
         if (bi.isBracketFree()) {
            left.setRange(start, i - start);
            right.setRange(i + 1, length + start - i - 1);
            return;
         }
      }
      else {
         bi.refresh(t);
      }
   }
}

void Tokens::splitBySymbol(const _char& symbol, std::vector<Tokens>& result) const
{
   BracketsInfo bi;
   _int sublen = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);

      if (t.isSymbol(symbol) && bi.isBracketFree()) {
         if (sublen == 0) {
            if (result.empty()) {
               throw SyntaxException(str(L"expression cannot start with a ", charStr(symbol), L" symbol"), t.line);
            }
            else {
               throw SyntaxException(str(L"adjacent ", charStr(symbol), L" symbols"), t.line);
            }
         }

         result.push_back(Tokens(list, i - sublen, sublen));
         sublen = 0;
      }
      else {
         bi.refresh(t);
         sublen++;
      }
   }

   if (sublen == 0) {
      if (!isEmpty()) {
         throw SyntaxException(str(L"expression cannot end with a ", charStr(symbol), L" symbol"), last().line);
      }
   }
   else {
      result.push_back(Tokens(list, 1 + end - sublen, sublen));
   }
}

_int Tokens::countSymbols(const _char& symbol) const
{
   BracketsInfo bi;
   _int count = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);

      if (t.isSymbol(symbol) && bi.isBracketFree()) {
         count++;
      }
      else {
         bi.refresh(t);
      }
   }

   return count;
}

void Tokens::splitByFiltherKeywords(std::vector<Tokens>& result, Uroboros* uro) const
{
   BracketsInfo bi;
   _int sublen = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = listAt(i);

      if (t.isFiltherKeyword() && bi.isBracketFree()) {
         if (sublen == 0) {
            const Token& prev = listAt(i - 1);

            throw SyntaxException(str(L"adjacent filter keywords '", prev.getOriginString(uro),
               L"' and '", t.getOriginString(uro), L"'"), t.line);
         }

         result.push_back(Tokens(list, i - sublen - 1, sublen + 1));
         sublen = 0;
      }
      else {
         bi.refresh(t);
         sublen++;
      }
   }

   if (sublen == 0) {
      throw SyntaxException(str(L"expression cannot end with a filter keyword '",
         last().getOriginString(uro), L"'"), last().line);
   }
   else {
      result.push_back(Tokens(list, end - sublen, sublen + 1));
   }
}

void Tokens::divideForTernary(Tokens& condition, Tokens& left, Tokens& right) const
{
   BracketsInfo bi;
   _boo hasPercent = false;
   _boo loop = true;
   _int percentId = -1;
   _int colonId = -1;

   for (_int i = start; loop && i <= end; i++) {
      const Token& t = listAt(i);
      if (t.type == Token::t_Symbol) {
         if (bi.isBracketFree()) {
            if (hasPercent) {
               if (t.value.ch == L':') {
                  loop = false;
                  colonId = i;
               }
            }
            else {
               if (t.value.ch == L'?') {
                  hasPercent = true;
                  percentId = i;
               }
            }
         }
         bi.refresh(t);
      }
   }

   condition.setRange(start, percentId - start);
   left.setRange(percentId + 1, colonId - percentId - 1);
   right.setRange(colonId + 1, length + start - colonId - 1);
}

}
