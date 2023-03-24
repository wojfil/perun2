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

#include "tokens.h"
#include "brackets.h"
#include "exception.h"
#include "lexer.h"
#include "perun2.h"


namespace perun2
{

Tokens::Tokens(const std::vector<Token>& li, const _int st, const _int ln, const _tinfo in)
   : start(st), length(ln), end(st + ln - 1), list(li), info(in) { }

Tokens::Tokens(const std::vector<Token>& li)
   : start(0), length(static_cast<_int>(li.size())), end(this->length - 1), list(li)
{
   checkBracketsThoroughly(*this);
   this->setData();
}

Tokens::Tokens(const Tokens& tks, const _int st, const _int ln)
   : start(st), length(ln), end(st + ln - 1), list(tks.getList())
{
   this->setData();
}

void Tokens::trimLeft()
{
   if (this->length != 0) {
      this->start++;
      this->length--;
      this->setData();
   }
}

void Tokens::trimRight()
{
   if (this->length != 0) {
      this->end--;
      this->length--;
      this->setData();
   }
}

_int Tokens::getStart() const
{
   return this->start;
}

_int Tokens::getLength() const
{
   return this->length;
}

_int Tokens::getEnd() const
{
   return this->end;
}

const std::vector<Token>& Tokens::getList() const
{
   return this->list;
}

_tinfo Tokens::getInfo() const
{
   return this->info;
}

_bool Tokens::check(const _tinfo in) const
{
   return this->info & in;
}

_bool Tokens::isEmpty() const
{
   return this->length == 0;
}

const Token& Tokens::first() const
{
   return this->list[this->start];
}

const Token& Tokens::second() const
{
   return this->list[this->start + 1];
}

const Token& Tokens::penultimate() const
{
   return this->list[this->end - 1];
}

const Token& Tokens::last() const
{
   return this->list[this->end];
}

const Token& Tokens::at(const _int index) const
{
   return this->list[this->start + index];
}

const Token& Tokens::listAt(const _int index) const
{
   return this->list[index];
}

std::pair<Tokens, Tokens> Tokens::divideByKeyword(const Keyword kw) const
{
   BracketsInfo bi;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isKeyword(kw)) {
         if (bi.isBracketFree()) {
            return std::make_pair(
               Tokens(this->list, this->start, i - this->start),
               Tokens(this->list, i + 1, this->length + this->start - i - 1)
            );
         }
      }
      else {
         bi.refresh(t);
      }
   }

   throw SyntaxError::keywordNotFound(first().line);
}

std::pair<Tokens, Tokens> Tokens::divideBySymbol(const _char symbol) const
{
   BracketsInfo bi;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isSymbol(symbol)) {
         if (bi.isBracketFree()) {
            return std::make_pair(
               Tokens(this->list, this->start, i - this->start),
               Tokens(this->list, i + 1, this->length + this->start - i - 1)
            );
         }
      }
      else {
         bi.refresh(t);
      }
   }

   throw SyntaxError::symbolNotFound(symbol, first().line);
}

std::vector<Tokens> Tokens::splitBySymbol(const _char symbol) const
{
   std::vector<Tokens> result;
   BracketsInfo bi;
   _int sublen = 0;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isSymbol(symbol) && bi.isBracketFree()) {
         if (sublen == 0) {
            if (result.empty()) {
               throw SyntaxError::expressionCannotStartWith(symbol, t.line);
            }
            else {
               throw SyntaxError::adjacentSymbols(symbol, t.line);
            }
         }

         result.emplace_back(*this, i - sublen, sublen);
         sublen = 0;
      }
      else {
         bi.refresh(t);
         sublen++;
      }
   }

   if (sublen == 0) {
      if (!isEmpty() && symbol != CHAR_COMMA) {
         throw SyntaxError::expressionCannotEndWith(symbol, last().line);
      }
   }
   else {
      result.emplace_back(*this, 1 + this->end - sublen, sublen);
   }

   return result;
}

_int Tokens::countSymbols(const _char symbol) const
{
   BracketsInfo bi;
   _int count = 0;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isSymbol(symbol) && bi.isBracketFree()) {
         count++;
      }
      else {
         bi.refresh(t);
      }
   }

   return count;
}

_int Tokens::getFilterKeywordId(_p2& p2) const
{
   BracketsInfo bi;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isFilterKeyword() && bi.isBracketFree()) {
         if (i == this->start) {
            throw SyntaxError::filterKeywordAtStart(t.getOriginString(p2), t.line);
         }
         else if (i == this->start + getLength() - 1) {
            throw SyntaxError::filterKeywordAtEnd(t.getOriginString(p2), t.line);
         }

         return i;
      }
      else {
         bi.refresh(t);
      }
   }

   return -1;
}

std::vector<Tokens> Tokens::splitByFiltherKeywords(_p2& p2) const
{
   std::vector<Tokens> result;
   BracketsInfo bi;
   _int sublen = 0;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isFilterKeyword() && bi.isBracketFree()) {
         if (sublen == 0) {
            const Token& prev = this->listAt(i - 1);
            throw SyntaxError::adjacentFilterKeywords(prev.getOriginString(p2), t.getOriginString(p2), t.line);
         }

         result.emplace_back(*this, i - sublen - 1, sublen + 1);
         sublen = 0;
      }
      else {
         bi.refresh(t);
         sublen++;
      }
   }

   if (sublen == 0) {
      throw SyntaxError::expressionCannotEndWithFilterKeyword(last().getOriginString(p2), last().line);
   }
   else {
      result.emplace_back(*this, this->end - sublen, sublen + 1);
   }

   return result;
}

std::tuple<Tokens, Tokens, Tokens> Tokens::divideForTernary() const
{
   BracketsInfo bi;
   _bool hasQuestionMark = false;
   _bool loop = true;
   _int questionMarkId = -1;
   _int colonId = -1;

   for (_int i = this->start; loop && i <= this->end; i++) {
      const Token& t = this->listAt(i);
      if (t.type == Token::t_Symbol) {
         if (bi.isBracketFree()) {
            if (hasQuestionMark) {
               if (t.value.ch == CHAR_COLON) {
                  loop = false;
                  colonId = i;
               }
            }
            else {
               if (t.value.ch == CHAR_QUESTION_MARK) {
                  hasQuestionMark = true;
                  questionMarkId = i;
               }
            }
         }
         bi.refresh(t);
      }
   }

   return std::make_tuple(
      Tokens(this->list, this->start, questionMarkId - this->start),
      Tokens(this->list, questionMarkId + 1, colonId - questionMarkId - 1),
      Tokens(this->list, colonId + 1, this->length + this->start - colonId - 1)
   );
}

void Tokens::checkCommonExpressionExceptions(_p2& p2) const
{
   _bool prevExclamantion = false;

   if (first().isSymbol(CHAR_EXCLAMATION_MARK)) {
      throw SyntaxError::negationByExclamation(first().line);
   }

   if (this->length == 1) {
      const Token& f = first();
      if (f.type == Token::t_Word && (!p2.contexts.varExists(f, p2))) {
         throw SyntaxError(str(L"variable '", f.getOriginString(p2), 
            L"' does not exist or is unreachable here. Look for a typo"), f.line);
      }
   }

   if (this->list[this->start].isSymbol(CHAR_OPENING_SQUARE_BRACKET)) {
      throw SyntaxError(
         L"a collection variable was expected before [ bracket. "
         L"If your intention was to define an array, then you should write values inside a pair of round brackets () and separate them by commas", 
         this->list[this->start].line);
   }

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->list[i];

      if (prevExclamantion && !t.isSymbol(CHAR_EQUAL_SIGN)) {
         throw SyntaxError::negationByExclamation(this->list[i - 1].line);
      }

      if (t.type == Token::t_MultiSymbol) {
         switch (t.value.chars.ch) {
            case CHAR_PLUS: {
               if (i == this->start) {
                  throw SyntaxError::expressionCannotStartWithIncrementation(t.line);
               }
               else {
                  throw SyntaxError::incrementationInsideExpression(t.line);
               }
               break;
            }
            case CHAR_MINUS: {
               if (i == this->start) {
                  throw SyntaxError::expressionCannotStartWithDecrementation(t.line);
               }
               else {
                  throw SyntaxError::decrementationInsideExpression(t.line);
               }
               break;
            }
         }
      }
      else if (t.type == Token::t_Keyword && t.isExpForbiddenKeyword()) {
         throw SyntaxError::expectedSemicolonBeforeKeyword(t.getOriginString(p2), t.line);
      }

      prevExclamantion = t.isSymbol(CHAR_EXCLAMATION_MARK);
   }
}

void Tokens::setData()
{
   this->info = TI_NULL;

   while (this->length >= 2) {
      if (this->list[this->start].isSymbol(CHAR_OPENING_ROUND_BRACKET) 
       && this->list[this->end].isSymbol(CHAR_CLOSING_ROUND_BRACKET)) 
      {
         _int lvl = 0;
         _bool b = true;

         for (_int i = this->start; b && i <= this->end; i++) {
            const Token& t = this->list[i];
            if (t.type == Token::t_Symbol) {
               switch (t.value.ch) {
                  case CHAR_OPENING_ROUND_BRACKET: {
                     lvl++;
                     break;
                  }
                  case CHAR_CLOSING_ROUND_BRACKET: {
                     lvl--;
                     if (lvl == 0 && i != this->end)
                     {
                        b = false;
                     }
                     break;
                  }
               }
            }
         }

         if (b) {
            this->start++;
            this->length -= 2;
            this->end--;
         }
         else {
            break;
         }
      }
      else {
         break;
      }
   }

   _int round = 0; // level of round brackets
   _int square = 0; // level of square brackets
   _bool indepRound = false;
   _bool firstSquare = false;
   _bool indepSquare = false;
   _int firstQuestionMarkId = -1;
   _int firstColonId = -1;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->list[i];

      if (round == 0 && square == 0) {
         switch (t.type) {
            case Token::t_Symbol: {
               switch (t.value.ch) {
                  case CHAR_QUESTION_MARK: {
                     this->info |= TI_HAS_CHAR_QUESTION_MARK;
                     if (firstQuestionMarkId == -1) {
                        firstQuestionMarkId = i;
                     }
                     break;
                  }
                  case CHAR_COMMA: {
                     this->info |= TI_HAS_CHAR_COMMA;
                     break;
                  }
                  case CHAR_COLON: {
                     this->info |= TI_HAS_CHAR_COLON;
                     if (firstColonId == -1) {
                        firstColonId = i;
                     }
                     break;
                  }
                  case CHAR_PLUS: {
                     this->info |= TI_HAS_CHAR_PLUS;
                     break;
                  }
                  case CHAR_MINUS: {
                     this->info |= TI_HAS_CHAR_MINUS;
                     break;
                  }
                  case CHAR_EQUAL_SIGN: {
                     this->info |= TI_HAS_CHAR_EQUALS;
                     this->info |= TI_HAS_COMPARISON_CHAR;
                     break;
                  }
                  case CHAR_SMALLER:
                  case CHAR_GREATER:
                  case CHAR_EXCLAMATION_MARK: {
                     this->info |= TI_HAS_COMPARISON_CHAR;
                     break;
                  }
               }
               break;
            }
            case Token::t_Keyword: {
               switch (t.value.keyword.k) {
                  case Keyword::kw_In: {
                     this->info |= TI_HAS_KEYWORD_IN;
                     break;
                  }
                  case Keyword::kw_Like: {
                     this->info |= TI_HAS_KEYWORD_LIKE;
                     break;
                  }
                  case Keyword::kw_To: {
                     this->info |= TI_HAS_KEYWORD_TO;
                     break;
                  }
                  case Keyword::kw_With: {
                     this->info |= TI_HAS_KEYWORD_WITH;
                     break;
                  }
                  case Keyword::kw_As: {
                     this->info |= TI_HAS_KEYWORD_AS;
                     break;
                  }
                  default: {
                     if (t.isFilterKeyword()) {
                        this->info |= TI_HAS_FILTER_KEYWORD;
                     }
                     break;
                  }
               }
               break;
            }
         }
      }

      if (t.type == Token::t_Symbol) {
         switch (t.value.ch) {
            case CHAR_OPENING_ROUND_BRACKET:  {
               round++;
               break;
            }
            case CHAR_CLOSING_ROUND_BRACKET:  {
               round--;
               
               if (round == 0 && i != this->end) {
                  indepRound = true;
               }
               else if (round < 0) {
                  throw SyntaxError::unopenedBracketIsClosed(CHAR_OPENING_ROUND_BRACKET, t.line);
               }
               break;
            }
            case CHAR_OPENING_SQUARE_BRACKET:  {
               square++;
               
               if (firstSquare && square == 1) {
                  indepSquare = true;
               }
               break;
            }
            case CHAR_CLOSING_SQUARE_BRACKET:  {
               square--;

               if (!firstSquare && square == 0) {
                  firstSquare = true;
               }
               else if (square < 0) {
                  throw SyntaxError::unopenedBracketIsClosed(CHAR_OPENING_SQUARE_BRACKET, t.line);
               }
               break;
            }
         }
      }
      else if (t.isKeyword(Keyword::kw_Rename)) {
         this->info |= TI_EVALUATE_DEFINITIONS;
      }
   }

   if (round != 0) {
      throw SyntaxError::bracketIsNotClosed(CHAR_OPENING_ROUND_BRACKET, this->list[this->end].line);
   }
   else if (square != 0) {
      throw SyntaxError::bracketIsNotClosed(CHAR_OPENING_SQUARE_BRACKET, this->list[this->end].line);
   }

   if (indepRound) {
      this->info |= TI_HAS_INDEP_BRACKETS;
   }

   if (indepSquare) {
      this->info |= TI_HAS_INDEP_SQ_BRACKETS;
   }

   if (this->length >= 3 && !indepRound) {
      if (this->list[this->start].type == Token::t_Word 
         && this->list[this->start + 1].isSymbol(CHAR_OPENING_ROUND_BRACKET) && this->list[this->end].isSymbol(CHAR_CLOSING_ROUND_BRACKET)) 
      {
         this->info |= TI_IS_POSSIBLE_FUNCTION;
      }
   }

   if ((this->info & TI_HAS_CHAR_QUESTION_MARK) != TI_NULL) {
      if (firstQuestionMarkId == this->start) {
         throw SyntaxError(L"sign ? is preceded by empty space", this->list[firstQuestionMarkId].line);
      }

      if ((this->info & TI_HAS_CHAR_COLON) == TI_NULL) {
         if (firstQuestionMarkId == this->end) {
            throw SyntaxError(L"sign ? is followed by empty space", this->list[firstQuestionMarkId].line);
         }

         this->info |= TI_IS_POSSIBLE_BINARY;
      }
      else {
         if (firstQuestionMarkId > firstColonId) {
            throw SyntaxError(L"signs ? and : appear in reverse order", this->list[firstQuestionMarkId].line);
         }
         else if (firstColonId == this->end) {
            throw SyntaxError(L"sign : is followed by empty space", this->list[firstColonId].line);
         }
         else if (firstQuestionMarkId + 1 == firstColonId) {
            throw SyntaxError(L"empty space between signs ? and :", this->list[firstQuestionMarkId].line);
         }

         this->info |= TI_IS_POSSIBLE_TERNARY;
      }
   }

   if (!indepSquare && this->length >= 3 && this->list[this->start + 1].isSymbol(CHAR_OPENING_SQUARE_BRACKET)) {
      const Token& first = this->list[this->start];
      const Token& last = this->list[this->end];

      if (last.isSymbol(CHAR_CLOSING_SQUARE_BRACKET)) {
         if (this->length == 3) {
            throw SyntaxError(L"empty space between square brackets []", last.line);
         }
         else if (first.type != Token::t_Word) {
            throw SyntaxError(L"square brackets [] can be preceded only by a variable name", first.line);
         }

         this->info |= TI_IS_POSSIBLE_LIST_ELEM;
      }

      if (this->list[this->end - 1].isSymbol(CHAR_CLOSING_SQUARE_BRACKET)) {
         if (this->length == 3) {
            throw SyntaxError(L"empty space between square brackets []", last.line);
         }
         else if (first.type != Token::t_Word) {
            throw SyntaxError(L"square brackets [] can be preceded only by a variable name", first.line);
         }

         if (last.value.twoWords.h1 == caseSensitiveHash(EMPTY_STRING)) {
            this->info |= TI_IS_LIST_ELEM_MEMBER;
         }
      }
   }
}

}
