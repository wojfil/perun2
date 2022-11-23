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

Tokens::Tokens(const std::vector<Token>& li, const _int& st, const _int& ln, const _tinfo& in)
   : start(st), length(ln), end(st + ln - 1), list(li), info(in) { }

Tokens::Tokens(const std::vector<Token>& li)
   : start(0), length(static_cast<_int>(li.size())), end(this->length - 1), list(li)
{
   checkBracketsThoroughly(*this);
   this->setData();
}

Tokens::Tokens(const Tokens& tks, const _int& st, const _int& ln)
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

_bool Tokens::check(const _tinfo& in) const
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

std::pair<Tokens, Tokens> Tokens::divideByKeyword(const Keyword& kw) const
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

   throw new SyntaxException(L"keyword not found", first().line);
}

std::pair<Tokens, Tokens> Tokens::divideBySymbol(const _char& symbol) const
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

   throw new SyntaxException(str(L"symbol '", toStr(symbol), L"' not found"), first().line);
}

std::vector<Tokens> Tokens::splitBySymbol(const _char& symbol) const
{
   std::vector<Tokens> result;
   BracketsInfo bi;
   _int sublen = 0;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isSymbol(symbol) && bi.isBracketFree()) {
         if (sublen == 0) {
            if (result.empty()) {
               throw SyntaxException(str(L"expression cannot this->start with a ", toStr(symbol), L" symbol"), t.line);
            }
            else {
               throw SyntaxException(str(L"adjacent ", toStr(symbol), L" symbols"), t.line);
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
      if (!isEmpty()) {
         throw SyntaxException(str(L"expression cannot this->end with a ", toStr(symbol), L" symbol"), last().line);
      }
   }
   else {
      result.emplace_back(*this, 1 + this->end - sublen, sublen);
   }

   return result;
}

_int Tokens::countSymbols(const _char& symbol) const
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

_int Tokens::getFilterKeywordId() const
{
   BracketsInfo bi;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isFilterKeyword() && bi.isBracketFree()) {
         return i;
      }
   }

   return -1;
}

std::vector<Tokens> Tokens::splitByFiltherKeywords(Uroboros& uro) const
{
   std::vector<Tokens> result;
   BracketsInfo bi;
   _int sublen = 0;

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->listAt(i);

      if (t.isFilterKeyword() && bi.isBracketFree()) {
         if (sublen == 0) {
            const Token& prev = this->listAt(i - 1);

            throw SyntaxException(str(L"adjacent filter keywords '", prev.getOriginString(uro),
               L"' and '", t.getOriginString(uro), L"'"), t.line);
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
      throw SyntaxException(str(L"expression cannot end with a filter keyword '",
         last().getOriginString(uro), L"'"), last().line);
   }
   else {
      result.emplace_back(*this, this->end - sublen, sublen + 1);
   }

   return result;
}

std::tuple<Tokens, Tokens, Tokens> Tokens::divideForTernary() const
{
   BracketsInfo bi;
   _bool hasPercent = false;
   _bool loop = true;
   _int percentId = -1;
   _int colonId = -1;

   for (_int i = this->start; loop && i <= this->end; i++) {
      const Token& t = this->listAt(i);
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

   return std::make_tuple(
      Tokens(this->list, this->start, percentId - this->start),
      Tokens(this->list, percentId + 1, colonId - percentId - 1),
      Tokens(this->list, colonId + 1, this->length + this->start - colonId - 1)
   );
}

void negationByExclamationException(const _int& line)
{
   throw SyntaxException(L"you should use keyword 'not' instead of character '!' for boolean negation", line);
}

void Tokens::checkCommonExpressionExceptions(Uroboros& uro) const
{
   _bool prevExclamantion = false;

   if (first().isSymbol(L'!')) {
      negationByExclamationException(first().line);
   }

   if (this->length == 1) {
      const Token& f = first();
      if (f.type == Token::t_Word) {
         if (f.value.word.h == uro.hashes.HASH_VAR_DEVICE) {
            throw SyntaxException(str(L"variable '", f.getOriginString(uro),
               L"' is reserved for future use. Your current version of Uroboros does not support it"), f.line);
         }

         if (f.value.word.h != uro.hashes.HASH_VAR_THIS && !uro.vars.variableExists(f)) {
            if (!(this->start > 0 && this->list[this->start - 1].isKeyword(Keyword::kw_Create)
            && (f.value.word.h == uro.hashes.HASH_VAR_FILE || f.value.word.h == uro.hashes.HASH_VAR_DIRECTORY))) {
               throw SyntaxException(str(L"variable '", f.getOriginString(uro), L"' does not exist or is unreachable here"), f.line);
            }
         }
      }
   }

   if (this->list[this->start].isSymbol(L'[')) {
      throw SyntaxException(
         L"a collection variable was expected before [ bracket. "
         L"If your intention was to define an array, then you should write values inside a pair of round brackets () and separate them by commas", 
         this->list[this->start].line);
   }

   for (_int i = this->start; i <= this->end; i++) {
      const Token& t = this->list[i];

      if (prevExclamantion && !t.isSymbol(L'=')) {
         negationByExclamationException(this->list[i - 1].line);
      }

      if (t.type == Token::t_MultiSymbol) {
         switch (t.value.chars.ch) {
            case L'+': {
               if (i == this->start) {
                  throw SyntaxException(L"expression cannot start with incrementation signs ++", t.line);
               }
               else {
                  throw SyntaxException(L"incrementation signs ++ cannot appear inside an expression", t.line);
               }
               break;
            }
            case L'-': {
               if (i == this->start) {
                  throw SyntaxException(L"expression cannot start with decrementation signs --", t.line);
               }
               else {
                  throw SyntaxException(L"decrementation signs -- cannot appear inside an expression", t.line);
               }
               break;
            }
         }
      }
      else if (t.type == Token::t_Keyword && t.isExpForbiddenKeyword()) {
         throw SyntaxException(str(L"expected ; before keyword '", t.getOriginString(uro), L"'"), t.line);
      }

      prevExclamantion = t.isSymbol(L'!');
   }
}

void Tokens::setData()
{
   this->info = TI_NULL;

   while (this->length >= 2) {
      if (this->list[this->start].isSymbol(L'(') && this->list[this->end].isSymbol(L')')) {
         _int lvl = 0;
         _bool b = true;

         for (_int i = this->start; b && i <= this->end; i++) {
            const Token& t = this->list[i];
            if (t.type == Token::t_Symbol) {
               switch (t.value.ch) {
                  case L'(': {
                     lvl++;
                     break;
                  }
                  case L')': {
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
                  case L'?': {
                     this->info |= TI_HAS_CHAR_QUESTION_MARK;
                     if (firstQuestionMarkId == -1) {
                        firstQuestionMarkId = i;
                     }
                     break;
                  }
                  case L',': {
                     this->info |= TI_HAS_CHAR_COMMA;
                     break;
                  }
                  case L':': {
                     this->info |= TI_HAS_CHAR_COLON;
                     if (firstColonId == -1) {
                        firstColonId = i;
                     }
                     break;
                  }
                  case L'+': {
                     this->info |= TI_HAS_CHAR_PLUS;
                     break;
                  }
                  case L'-': {
                     this->info |= TI_HAS_CHAR_MINUS;
                     break;
                  }
                  case L'=': {
                     this->info |= TI_HAS_CHAR_EQUALS;
                     this->info |= TI_HAS_COMPARISON_CHAR;
                     break;
                  }
                  case L'<':
                  case L'>':
                  case L'!': {
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
                  case Keyword::kw_From: {
                     this->info |= TI_HAS_KEYWORD_FROM;
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
            case '(':  {
               round++;
               break;
            }
            case ')':  {
               round--;
               
               if (round == 0 && i != this->end) {
                  indepRound = true;
               }
               else if (round < 0) {
                  throw SyntaxException(L"unopened bracket ( is closed", t.line);
               }
               break;
            }
            case '[':  {
               square++;
               
               if (firstSquare && square == 1) {
                  indepSquare = true;
               }
               break;
            }
            case ']':  {
               square--;

               if (!firstSquare && square == 0) {
                  firstSquare = true;
               }
               else if (square < 0) {
                  throw SyntaxException(L"unopened bracket [ is closed", t.line);
               }
               break;
            }
         }
      }
   }

   if (round != 0) {
      throw SyntaxException(L"bracket ( is not closed", this->list[this->end].line);
   }
   else if (square != 0) {
      throw SyntaxException(L"bracket [ is not closed", this->list[this->end].line);
   }

   if (indepRound) {
      this->info |= TI_HAS_INDEP_BRACKETS;
   }

   if (indepSquare) {
      this->info |= TI_HAS_INDEP_SQ_BRACKETS;
   }

   if (this->length >= 3 && !indepRound) {
      if (this->list[this->start].type == Token::t_Word && this->list[this->start + 1].isSymbol(L'(') && this->list[this->end].isSymbol(L')')) {
         this->info |= TI_IS_POSSIBLE_FUNCTION;
      }
   }

   if ((this->info & TI_HAS_CHAR_QUESTION_MARK) != TI_NULL) {
      if (firstQuestionMarkId == this->start) {
         throw SyntaxException(L"sign ? is preceded by empty space", this->list[firstQuestionMarkId].line);
      }

      if ((this->info & TI_HAS_CHAR_COLON) == TI_NULL) {
         if (firstQuestionMarkId == this->end) {
            throw SyntaxException(L"sign ? is followed by empty space", this->list[firstQuestionMarkId].line);
         }

         this->info |= TI_IS_POSSIBLE_BINARY;
      }
      else {
         if (firstQuestionMarkId > firstColonId) {
            throw SyntaxException(L"signs ? and : appear in reverse order", this->list[firstQuestionMarkId].line);
         }
         else if (firstColonId == this->end) {
            throw SyntaxException(L"sign : is followed by empty space", this->list[firstColonId].line);
         }
         else if (firstQuestionMarkId + 1 == firstColonId) {
            throw SyntaxException(L"empty space between signs ? and :", this->list[firstQuestionMarkId].line);
         }

         this->info |= TI_IS_POSSIBLE_TERNARY;
      }
   }

   if (!indepSquare && this->length >= 3 && this->list[this->start + 1].isSymbol(L'[')) {
      const Token& first = this->list[this->start];
      const Token& last = this->list[this->end];

      if (last.isSymbol(L']')) {
         if (this->length == 3) {
            throw SyntaxException(L"empty space between square brackets []", last.line);
         }
         else if (first.type != Token::t_Word) {
            throw SyntaxException(L"square brackets [] can be preceded only by a variable name", first.line);
         }

         this->info |= TI_IS_POSSIBLE_LIST_ELEM;
      }

      if (this->list[this->end - 1].isSymbol(L']')) {
         if (this->length == 3) {
            throw SyntaxException(L"empty space between square brackets []", last.line);
         }
         else if (first.type != Token::t_Word) {
            throw SyntaxException(L"square brackets [] can be preceded only by a variable name", first.line);
         }

         if (last.value.twoWords.h1 == rawStringHash(EMPTY_STRING)) {
            this->info |= TI_IS_LIST_ELEM_MEMBER;
         }
      }
   }
}

}
