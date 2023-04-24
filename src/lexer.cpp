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

#include "lexer.h"
#include "exception.h"
#include "brackets.h"
#include "hash.h"
#include <cwctype>
#include <iostream>
#include <sstream>


namespace perun2
{

// transform source code into a list of tokens
// meanwhile, omit comments
// both // singleline
// and /* multiline */
std::vector<Token> tokenize(const _str& code, _p2& p2)
{
   enum Mode {
      m_Normal = 0,
      m_Word,
      m_ALiteral, // apostrophe string literal
      m_BLiteral, // backtick string literal
      m_SingleComment,
      m_MultiComment
   }; // finite-state machine within function

   std::vector<Token> tokens;
   Mode mode = Mode::m_Normal;
   _int line = 1;
   _char prev = CHAR_SPACE;
   const _size len = code.length();
   _size wpos = 0;
   _size wlen = 0;
   _bool prevSymbol = false;

   for (_size i = 0; i < len; i++) {
      const _char c = code[i];

      switch (mode)  {
         case Mode::m_Normal: {
            if (c == CHAR_QUOTATION_MARK) {
               throw SyntaxError::quotationMarkStringLteral(line);
            }
            else if (isSymbol(c)) {
               if (i != 0 && prev == CHAR_SLASH) {
                  if (c == CHAR_ASTERISK)  {
                     mode = Mode::m_MultiComment;
                     tokens.pop_back();
                     prevSymbol = false;
                  }
                  else if (c == CHAR_SLASH)  {
                     mode = Mode::m_SingleComment;
                     tokens.pop_back();
                     prevSymbol = false;
                  }
                  else {
                     if (prevSymbol) {
                        if (isDoubleChar(c) && tokens.back().value.ch == c) {
                           tokens.pop_back();
                           tokens.emplace_back(c, 2, line, p2);
                           prevSymbol = false;
                        }
                        else {
                           tokens.emplace_back(c, line, p2);
                        }
                     }
                     else {
                        tokens.emplace_back(c, line, p2);
                        prevSymbol = true;
                     }
                  }
               }
               else {
                  if (prevSymbol) {
                     if (isDoubleChar(c) && tokens.back().value.ch == c) {
                        tokens.pop_back();
                        tokens.emplace_back(c, 2, line, p2);
                        prevSymbol = false;
                     }
                     else {
                        tokens.emplace_back(c, line, p2);
                     }
                  }
                  else {
                     tokens.emplace_back(c, line, p2);
                     prevSymbol = true;
                  }
               }
            }
            else {
               if (isAllowedInWord(c)) {
                  wpos = i;
                  wlen = 1;
                  mode = Mode::m_Word;
                  prevSymbol = false;
               }
               else if (isNewLine(c)) {
                  line++;
               }
               else if (c == CHAR_APOSTROPHE) {
                  wpos = i + 1;
                  wlen = 0;
                  mode = Mode::m_ALiteral;
                  prevSymbol = false;
               }
               else if (c == CHAR_BACKTICK) {
                  wpos = i + 1;
                  wlen = 0;
                  mode = Mode::m_BLiteral;
                  prevSymbol = false;
               }
               else if (c != CHAR_SPACE) {
                  throw SyntaxError::invalidChar(c, line);
               }
            }
            break;
         }
         case Mode::m_Word: {
            if (c == CHAR_QUOTATION_MARK) {
               throw SyntaxError::quotationMarkStringLteral(line);
            }
            else if (isAllowedInWord(c)) {
               wlen++;
            }
            else {
               tokens.push_back(wordToken(code, wpos, wlen, line, p2));
               wlen = 0;
               mode = Mode::m_Normal;

               if (isSymbol(c)) {
                  tokens.emplace_back(c, line, p2);
                  prevSymbol = true;
               }
               else if (isNewLine(c)) {
                  line++;
               }
               else if (c == CHAR_APOSTROPHE) {
                  wpos = i + 1;
                  mode = Mode::m_ALiteral;
               }
               else if (c == CHAR_BACKTICK) {
                  wpos = i + 1;
                  mode = Mode::m_BLiteral;
               }
               else if (c != CHAR_SPACE) {
                  throw SyntaxError::invalidChar(c, line);
               }
            }
            break;
         }
         case Mode::m_ALiteral: {
            if (c == CHAR_APOSTROPHE) {
               _int asteriskId = -1;

               for (_size j = wpos; j < wpos + wlen; j++) {
                  if (code[j] == CHAR_ASTERISK) {
                     asteriskId = static_cast<_int>(j);
                     break;
                  }
               }

               if (asteriskId == -1) {
                  tokens.emplace_back(wpos, wlen, line, p2);
               }
               else {
                  tokens.emplace_back(wpos, wlen, asteriskId, line, p2);
               }

               wpos = i;
               wlen = 0;
               mode = Mode::m_Normal;
            }
            else {
               wlen++;
               if (isNewLine(c)) {
                  line++;
               }
            }
            break;
         }
         case Mode::m_BLiteral: {
            if (c == CHAR_BACKTICK) {
               tokens.emplace_back(wpos, wlen, line, p2);
               wpos = i;
               wlen = 0;
               mode = Mode::m_Normal;
            }
            else {
               wlen++;
               if (isNewLine(c)) {
                  line++;
               }
            }
            break;
         }
         case Mode::m_SingleComment: {
            if (isNewLine(c)) {
               line++;
               mode = Mode::m_Normal;
            }
            break;
         }
         case Mode::m_MultiComment: {
            if (isNewLine(c)){
               line++;
            }
            else if (prev == CHAR_ASTERISK && c == CHAR_SLASH) {
               mode = Mode::m_Normal;
            }
            break;
         }
      }

      prev = c;
   }

   switch (mode) {
      case m_Word: {
         if (wlen != 0) {
            tokens.push_back(wordToken(code, wpos, wlen, line, p2));
         }
         break;
      }
      case m_ALiteral:
      case m_BLiteral: {
         throw SyntaxError::openedStringLteral(line);
      }
   }

   return tokens;
}

static Token wordToken(const _str& code, const _size start, const _size length, const _int line, _p2& p2)
{
   _int dots = 0;
   _bool nums = true;

   for (_size i = start; i < start + length; i++) {
      if (!std::iswdigit(code[i])) {
         if (code[i] == CHAR_DOT) {
            dots++;
         }
         else {
            nums = false;
         }
      }
   }

   if (nums) {
      const _str value = code.substr(start, length);
      switch (dots) {
         case 0: {
            try {
               return Token(_num(std::stoll(value)), line, start, length, NumberMode::nm_Normal, p2);
            }
            catch (...) {
               throw SyntaxError::numberTooBig(code.substr(start, length), line);
            }
         }
         case 1: {
            try {
               return Token(_num(stringToDouble(value)), line, start, length, NumberMode::nm_Normal, p2);
            }
            catch (...) {
               throw SyntaxError::numberTooBig(code.substr(start, length), line);
            }
         }
         default: {
            throw SyntaxError::multipleDotsInNumber(value, line);
         }
      }
   }

   if (length > 2 && dots <= 1) {
      _char c1 = code[start + length - 2];
      _char c2 = code[start + length - 1];
      nums = true;
      const _int n = length - 2;

      for (_size i = start; i < n; i++) {
         if (!std::iswdigit(code[i]) && code[i] != CHAR_DOT) {
            nums = false;
            break;
         }
      }

      if (nums) {
         _nint mult = getSuffixMultiplier(c1, c2);
         if (mult != NINT_MINUS_ONE) {
            _str value2 = code.substr(start, length - 2);
            if (dots == 0) {
               try {
                  // check for number overflow
                  // the number is multiplied by the suffix
                  // and then divided back
                  _nint i = std::stoll(value2);
                  _nint i2 = i * mult;
                  if (mult != NINT_ZERO && i2 / mult != i) {
                     throw SyntaxError::numberTooBig(code.substr(start, length), line);
                  }
                  return Token(_num(i2), line, start, length, NumberMode::nm_Size, p2);
               }
               catch (...) {
                  throw SyntaxError::numberTooBig(code.substr(start, length), line);
               }
            }
            else {
               try {
                  _ndouble d = stringToDouble(value2);
                  d *= mult;

                  return Token(_num(d), line, start, length, NumberMode::nm_Size, p2);
               }
               catch (...) {
                  throw SyntaxError::numberTooBig(code.substr(start, length), line);
               }
            }
         }
      }
   }

   switch (dots) {
      case 0: {
         _str word = code.substr(start, length);
         const _hash hsh = caseInsensitiveHash(word);

         auto fm = p2.hashes.HASH_MAP_MONTHS.find(hsh);
         if (fm != p2.hashes.HASH_MAP_MONTHS.end()) {
            return Token(_num(fm->second), line, start, length, NumberMode::nm_Month, p2);
         }

         auto fw = p2.hashes.HASH_MAP_WEEKDAYS.find(hsh);
         if (fw != p2.hashes.HASH_MAP_WEEKDAYS.end()) {
            return Token(_num(fw->second), line, start, length, NumberMode::nm_WeekDay, p2);
         }

         auto fk = p2.keywordsData.KEYWORDS.find(hsh);
         if (fk == p2.keywordsData.KEYWORDS.end()) {
            return Token(hsh, line, start, length, p2);
         }
         else {
            return Token(fk->second, line, start, length, p2);
         }
      }
      case 1: {
         _int pnt = start;
         for (_int i = start; i < start + length; i++) {
            if (code[i] == CHAR_DOT) {
               pnt = i;
            }
         }

         if (pnt == length - 1) {
            throw SyntaxError::missingTimeVariableMember(code.substr(start, length), line);
         }

         _str os1 = code.substr(start, pnt - start);
         _str os2 = code.substr(pnt + 1, start + length - pnt - 1);
         const _hash h1 = caseInsensitiveHash(os1);
         const _hash h2 = caseInsensitiveHash(os2);

         return Token(h1, h2, line, start, pnt - start, pnt + 1, start + length - pnt - 1, p2);
      }
      default: {
         throw SyntaxError::multipleDotsInWord(code.substr(start, length), line);
      }
   }

   return Token(start, length, line, p2);
}

inline static _nint getSuffixMultiplier(const _char c1, const _char c2)
{
   if (!(c2 == CHAR_b || c2 == CHAR_B)) {
      return NINT_MINUS_ONE;
   }

   switch (c1) {
      case CHAR_k: 
      case CHAR_K:
         return SIZE_UNIT_KB;
      case CHAR_m: 
      case CHAR_M:
         return SIZE_UNIT_MB;
      case CHAR_g:
      case CHAR_G:
         return SIZE_UNIT_GB;
      case CHAR_t:
      case CHAR_T:
         return SIZE_UNIT_TB;
      case CHAR_p:
      case CHAR_P:
         return SIZE_UNIT_PB;
      default:
         return NINT_MINUS_ONE;
   }
}

inline static _bool isSymbol(const _char ch)
{
   switch (ch) {
      case CHAR_COMMA:
      case CHAR_EXCLAMATION_MARK:
      case CHAR_EQUAL_SIGN:
      case CHAR_OPENING_ROUND_BRACKET:
      case CHAR_CLOSING_ROUND_BRACKET:
      case CHAR_OPENING_CURLY_BRACKET:
      case CHAR_CLOSING_CURLY_BRACKET:
      case CHAR_OPENING_SQUARE_BRACKET:
      case CHAR_CLOSING_SQUARE_BRACKET:
      case CHAR_COLON:
      case CHAR_SEMICOLON:
      case CHAR_MINUS:
      case CHAR_PLUS:
      case CHAR_ASTERISK:
      case CHAR_PERCENT:
      case CHAR_SLASH:
      case CHAR_SMALLER:
      case CHAR_GREATER:
      case CHAR_QUESTION_MARK:
         return true;
      default:
         return false;
   }
}

inline static _bool isNewLine(const _char ch)
{
   return ch == CHAR_NEW_LINE;
}

inline static _bool isAllowedInWord(const _char ch)
{
   return std::iswalpha(ch) || std::iswdigit(ch) || ch == CHAR_DOT;
}

inline static _bool isDoubleChar(const _char ch)
{
   switch (ch) {
      case CHAR_PLUS:
      case CHAR_MINUS:
      case CHAR_ASTERISK:
         return true;
      default:
         return false;
   }
}

}