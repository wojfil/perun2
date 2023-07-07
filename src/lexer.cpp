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
               else if (! isSpace(c)) {
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
               else if (! isSpace(c)) {
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
   _bool onlyDigitsAndDots = true;

   for (_size i = start; i < start + length; i++) {
      if (!std::iswdigit(code[i])) {
         if (code[i] == CHAR_DOT) {
            dots++;
         }
         else {
            onlyDigitsAndDots = false;
         }
      }
   }

   // try to parse a numeric constant: 20
   if (onlyDigitsAndDots) {
      const _str value = code.substr(start, length);
      return numberToken(code, value, start, length, NINT_ONE, NumberMode::nm_Normal, dots, line, p2);
   }

   // try to parse a size unit constant: 20mb
   if (length > 2) {
      const _nint sizeUnit = fileSizeSuffixMulti(code[start + length - 2], code[start + length - 1]);

      if (sizeUnit != NINT_MINUS_ONE) {
         _bool hasLetters = false;

         for (_size i = start; i < (length - 2); i++) {
            if (!std::iswdigit(code[i]) && code[i] != CHAR_DOT) {
               hasLetters = true;
               break;
            }
         }

         if (! hasLetters) {
            const _str value2 = code.substr(start, length - 2);
            return numberToken(code, value2, start, length, sizeUnit, NumberMode::nm_Size, dots, line, p2);
         }
      }
   }

   // try to parse a decimal constant: 20k
   if (length > 1) {
      const _nint multiplier = decimalSuffixMulti(code[start + length - 1]);

      if (multiplier != NINT_MINUS_ONE) {
         _bool hasLetters = false;

         for (_size i = start; i < (length - 1); i++) {
            if (!std::iswdigit(code[i]) && code[i] != CHAR_DOT) {
               hasLetters = true;
               break;
            }
         }

         if (! hasLetters) {
            const _str value2 = code.substr(start, length - 1);
            return numberToken(code, value2, start, length, multiplier, NumberMode::nm_Decimal, dots, line, p2);
         }
      }
   }

   if (dots > 1) {
      throw SyntaxError::multipleDotsInWord(code.substr(start, length), line);
   }

   if (dots == 0) {
      _str word = code.substr(start, length);
      toLower(word);

      auto fm = p2.keywordsData.MONTHS.find(word);
      if (fm != p2.keywordsData.MONTHS.end()) {
         return Token(_num(fm->second), line, start, length, NumberMode::nm_Month, p2);
      }

      auto fw = p2.keywordsData.WEEKDAYS.find(word);
      if (fw != p2.keywordsData.WEEKDAYS.end()) {
         return Token(_num(fw->second), line, start, length, NumberMode::nm_WeekDay, p2);
      }

      auto fk = p2.keywordsData.KEYWORDS.find(word);
      if (fk == p2.keywordsData.KEYWORDS.end()) {
         return Token(line, start, length, p2);
      }
      else {
         return Token(fk->second, line, start, length, p2);
      }
   }

   if (dots == 1) {
      _size pnt = start;
      for (_size i = start; i < start + length; i++) {
         if (code[i] == CHAR_DOT) {
            pnt = i;
         }
      }

      if (pnt == length - 1) {
         throw SyntaxError::missingTimeVariableMember(code.substr(start, length), line);
      }

      return Token(line, start, pnt - start, pnt + 1, start + length - pnt - 1, p2);
   }
   
   return Token(start, length, line, p2);
}

inline static Token numberToken(const _str& code, const _str& value, const _size start, const _size length, 
   const _nint multiplier, const NumberMode mode, const _int dots, const _int line, _p2& p2)
{
   if (dots > 1) {
      throw SyntaxError::multipleDotsInNumber(value, line);
   }

   if (dots == 0) {
      try {
         _nint integer = std::stoll(value);

         if (multiplier != NINT_ONE) {
            // look for number overflow
            // the number is multiplied by the suffix
            // and then divided back

            _nint i2 = integer * multiplier;

            if (multiplier != NINT_ZERO && i2 / multiplier != integer) {
               throw SyntaxError::numberTooBig(code.substr(start, length), line);
            }

            return Token(_num(i2), line, start, length, mode, p2);
         }

         return Token(_num(integer), line, start, length, mode, p2);
      }
      catch (...) {
         throw SyntaxError::numberTooBig(code.substr(start, length), line);
      }
   }

   try {
      _ndouble dbl = stringToDouble(value);
      dbl *= multiplier;

      // try to convert the number into integer
      // if it equals the base double value, create an integer constant instead
      const _nint integer = static_cast<_nint>(dbl);
      if (dbl == integer) {
         return Token(_num(integer), line, start, length, mode, p2);
      }

      return Token(_num(dbl), line, start, length, mode, p2);
   }
   catch (...) {
      throw SyntaxError::numberTooBig(code.substr(start, length), line);
   }     
}

inline static _nint fileSizeSuffixMulti(const _char c1, const _char c2)
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

inline static _nint decimalSuffixMulti(const _char c)
{
   switch (c) {
      case CHAR_k: 
      case CHAR_K:
         return NINT_THOUSAND;
      case CHAR_m: 
      case CHAR_M:
         return NINT_MILLION;
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

inline static _bool isSpace(const _char ch)
{
   switch (ch) {
      case CHAR_SPACE:
      case CHAR_TAB:
      case CHAR_CARRIAGE_RETURN:
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