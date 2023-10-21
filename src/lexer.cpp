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
std::vector<Token> tokenize(const p_str& code, p_perun2& p2)
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
   p_int line = 1;
   p_char prev = CHAR_SPACE;
   const p_size len = code.length();
   p_size wpos = 0;
   p_size wlen = 0;
   p_bool prevSymbol = false;
   p_bool prevReset = false;

   for (p_size i = 0; i < len; i++) {
      const p_char c = code[i];

      switch (mode)  {
         case Mode::m_Normal: {
            if (c == CHAR_QUOTATION_MARK) {
               throw SyntaxError::quotationMarkStringLteral(line);
            }

            if (isSymbol(c)) {
               if (i != 0 && prev == CHAR_SLASH) {
                  if (c == CHAR_ASTERISK)  {
                     mode = Mode::m_MultiComment;
                     tokens.pop_back();
                     prevSymbol = false;
                     prevReset = true;
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

            if (isAllowedInWord(c)) {
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
               p_int asteriskId = -1;

               for (p_size j = wpos; j < wpos + wlen; j++) {
                  if (code[j] == CHAR_ASTERISK) {
                     asteriskId = static_cast<p_int>(j);
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
               prevReset = true;
            }
            break;
         }
      }

      if (prevReset) {
         prevReset = false;
         prev = CHAR_NULL;
      }
      else {
         prev = c;
      }
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

static Token wordToken(const p_str& code, const p_size start, const p_size length, const p_int line, p_perun2& p2)
{
   p_int dots = 0;
   p_bool onlyDigitsAndDots = true;

   for (p_size i = start; i < start + length; i++) {
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
      const p_str value = code.substr(start, length);
      return numberToken(code, value, start, length, NINT_ONE, NumberMode::nm_Normal, dots, line, p2);
   }

   // try to parse a size unit constant: 20mb
   if (length > 2) {
      const p_nint sizeUnit = fileSizeSuffixMulti(code[start + length - 2], code[start + length - 1]);

      if (sizeUnit != NINT_MINUS_ONE) {
         p_bool hasLetters = false;

         for (p_size i = start; i < (start + length - 2); i++) {
            if (!std::iswdigit(code[i]) && code[i] != CHAR_DOT) {
               hasLetters = true;
               break;
            }
         }

         if (! hasLetters) {
            const p_str value2 = code.substr(start, length - 2);
            return numberToken(code, value2, start, length, sizeUnit, NumberMode::nm_Size, dots, line, p2);
         }
      }
   }

   // try to parse a decimal constant: 20k
   if (length > 1) {
      const p_nint multiplier = decimalSuffixMulti(code[start + length - 1]);

      if (multiplier != NINT_MINUS_ONE) {
         p_bool hasLetters = false;

         for (p_size i = start; i < (start + length - 1); i++) {
            if (!std::iswdigit(code[i]) && code[i] != CHAR_DOT) {
               hasLetters = true;
               break;
            }
         }

         if (! hasLetters) {
            const p_str value2 = code.substr(start, length - 1);
            return numberToken(code, value2, start, length, multiplier, NumberMode::nm_Decimal, dots, line, p2);
         }
      }
   }

   // try to parse K infix: 2k23
   if (dots == 0) {
      p_int kid = -1;

      for (p_size i = start; i < start + length; i++) {
         const p_char ch = code[i];

         if (ch == CHAR_k || ch == CHAR_K) {
            if (kid == -1) {
               kid = i;
            }
            else {
               kid = -1;
               break;
            }
         }
         else if (!std::iswdigit(ch)) {
            kid = -1;
            break;
         }
      }

      const p_int min = static_cast<p_int>(start + length) - 4;
      const p_int max = static_cast<p_int>(start + length) - 1;

      if (kid != -1 && kid > static_cast<p_int>(start) && kid < max && kid >= min) {
         p_nint first;
         p_nint second;
         const p_str firstString = code.substr(start, kid - start);
         const p_str secondString = code.substr(kid + 1, start + length - kid - 1);

         try {
            first = std::stoll(firstString);
         }
         catch (...) {
            throw SyntaxError::numberTooBig(firstString, line);
         }

         try {
            second = std::stoll(secondString);
         }
         catch (...) {
            throw SyntaxError::numberTooBig(secondString, line);
         }

         return Token(p_num(first * NINT_THOUSAND + second), line, start, length, NumberMode::nm_Infix, p2);
      }
   }

   if (dots > 1) {
      throw SyntaxError::multipleDotsInWord(code.substr(start, length), line);
   }

   if (dots == 0) {
      p_str word = code.substr(start, length);
      toLower(word);

      auto fm = p2.keywordsData.MONTHS.find(word);
      if (fm != p2.keywordsData.MONTHS.end()) {
         return Token(p_num(fm->second), line, start, length, NumberMode::nm_Month, p2);
      }

      auto fw = p2.keywordsData.WEEKDAYS.find(word);
      if (fw != p2.keywordsData.WEEKDAYS.end()) {
         return Token(p_num(fw->second), line, start, length, NumberMode::nm_WeekDay, p2);
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
      p_size pnt = start;
      for (p_size i = start; i < start + length; i++) {
         if (code[i] == CHAR_DOT) {
            pnt = i;
            break;
         }
      }

      if (pnt == start + length - 1) {
         throw SyntaxError::missingTimeVariableMember(code.substr(start, length), line);
      }

      return Token(line, start, pnt - start, pnt + 1, start + length - pnt - 1, p2);
   }
   
   return Token(start, length, line, p2);
}

inline static Token numberToken(const p_str& code, const p_str& value, const p_size start, const p_size length, 
   const p_nint multiplier, const NumberMode mode, const p_int dots, const p_int line, p_perun2& p2)
{
   if (dots > 1) {
      throw SyntaxError::multipleDotsInNumber(value, line);
   }

   if (dots == 0) {
      try {
         p_nint integer = std::stoll(value);

         if (multiplier != NINT_ONE) {
            // look for number overflow
            // the number is multiplied by the suffix
            // and then divided back

            p_nint i2 = integer * multiplier;

            if (multiplier != NINT_ZERO && i2 / multiplier != integer) {
               throw SyntaxError::numberTooBig(code.substr(start, length), line);
            }

            return Token(p_num(i2), line, start, length, mode, p2);
         }

         return Token(p_num(integer), line, start, length, mode, p2);
      }
      catch (...) {
         throw SyntaxError::numberTooBig(code.substr(start, length), line);
      }
   }

   try {
      p_ndouble dbl = stringToDouble(value);
      dbl *= multiplier;

      // try to convert the number into integer
      // if it equals the base double value, create an integer constant instead
      const p_ndouble trunc = std::trunc(dbl);
      if (trunc == dbl) {
         return Token(p_num(static_cast<p_nint>(trunc)), line, start, length, mode, p2);
      }

      return Token(p_num(dbl), line, start, length, mode, p2);
   }
   catch (...) {
      throw SyntaxError::numberTooBig(code.substr(start, length), line);
   }     
}

inline static p_nint fileSizeSuffixMulti(const p_char ch1, const p_char ch2)
{
   if (!(ch2 == CHAR_b || ch2 == CHAR_B)) {
      return NINT_MINUS_ONE;
   }

   switch (ch1) {
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

inline static p_nint decimalSuffixMulti(const p_char ch)
{
   switch (ch) {
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

inline static p_bool isSymbol(const p_char ch)
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

inline static p_bool isSpace(const p_char ch)
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

inline static p_bool isNewLine(const p_char ch)
{
   return ch == CHAR_NEW_LINE;
}

inline static p_bool isAllowedInWord(const p_char ch)
{
   return std::iswalpha(ch) || std::iswdigit(ch) || ch == CHAR_DOT;
}

inline static p_bool isDoubleChar(const p_char ch)
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