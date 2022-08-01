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

#include "lexer.h"
#include "exception.h"
#include "brackets.h"
#include "hash.h"
#include <cwctype>
#include <iostream>
#include <sstream>


// transform source code into a list of tokens
// meanwhile, omit comments
// both // singleline
// and /* multiline */
std::vector<Token> tokenize(const _str& code, Uroboros* uro)
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
   _char prev = L' ';
   const _size len = code.length();
   _size wpos = 0, wlen = 0;
   _boo prevSymbol = false;

   for (_size i = 0; i < len; i++) {
      const _char& c = code[i];
      switch (mode)  {
         case Mode::m_Normal: {
            if (c == L'"') {
               throw SyntaxException(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
            }
            else if (isSymbol(c)) {
               if (i != 0 && prev == L'/') {
                  if (c == L'*')  {
                     mode = Mode::m_MultiComment;
                     tokens.pop_back();
                     prevSymbol = false;
                  }
                  else if (c == L'/')  {
                     mode = Mode::m_SingleComment;
                     tokens.pop_back();
                     prevSymbol = false;
                  }
                  else {
                     if (prevSymbol) {
                        if (isDoubleChar(c) && tokens.back().value.ch == c) {
                           tokens.pop_back();
                           tokens.push_back(Token(c, 2, line, uro));
                           prevSymbol = false;
                        }
                        else {
                           tokens.push_back(Token(c, line, uro));
                        }
                     }
                     else {
                        tokens.push_back(Token(c, line, uro));
                        prevSymbol = true;
                     }
                  }
               }
               else {
                  if (prevSymbol) {
                     if (isDoubleChar(c) && tokens.back().value.ch == c) {
                        tokens.pop_back();
                        tokens.push_back(Token(c, 2, line, uro));
                        prevSymbol = false;
                     }
                     else {
                        tokens.push_back(Token(c, line, uro));
                     }
                  }
                  else {
                     tokens.push_back(Token(c, line, uro));
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
               else if (c == L'\'') {
                  wpos = i + 1;
                  wlen = 0;
                  mode = Mode::m_ALiteral;
                  prevSymbol = false;
               }
               else if (c == L'`') {
                  wpos = i + 1;
                  wlen = 0;
                  mode = Mode::m_BLiteral;
                  prevSymbol = false;
               }
               else if (c != L' ') {
                  invalidCharException(c, line);
               }
            }
            break;
         }
         case Mode::m_Word: {
            if (c == L'"') {
               throw SyntaxException(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
            }
            else if (isAllowedInWord(c)) {
               wlen++;
            }
            else {
               const _str word = code.substr(wpos, wlen);
               tokens.push_back(wordToken(word, line, uro));
               wlen = 0;
               mode = Mode::m_Normal;

               if (isSymbol(c)) {
                  tokens.push_back(Token(c, line, uro));
                  prevSymbol = true;
               }
               else if (isNewLine(c)) {
                  line++;
               }
               else if (c == L'\'') {
                  wpos = i + 1;
                  mode = Mode::m_ALiteral;
               }
               else if (c == L'`') {
                  wpos = i + 1;
                  mode = Mode::m_BLiteral;
               }
               else if (c != L' ') {
                  invalidCharException(c, line);
               }
            }
            break;
         }
         case Mode::m_ALiteral: {
            if (c == L'\'') {
               const _str word = code.substr(wpos, wlen);
               const _int asteriskId = word.find(L'*');

               if (asteriskId == _str::npos) {
                  tokens.push_back(Token(word, line, uro));
               }
               else {
                  tokens.push_back(Token(word, asteriskId, line, uro));
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
            if (c == L'`') {
               const _str word = code.substr(wpos, wlen);
               tokens.push_back(Token(word, line, uro));
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
            else if (prev == L'*' && c == L'/') {
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
            const _str word = code.substr(wpos, wlen);
            tokens.push_back(wordToken(word, line, uro));
         }
         break;
      }
      case m_ALiteral:
      case m_BLiteral: {
         throw SyntaxException(L"an opened string literal is not closed", line);
      }
   }

   return tokens;
}

static Token wordToken(const _str& value, const _int& line, Uroboros* uro)
{
   _int dots = 0;
   _boo nums = true;

   for (const _char &c : value) {
      if (!std::iswdigit(c)) {
         if (c == L'.') {
            dots++;
         }
         else {
            nums = false;
         }
      }
   }

   if (nums) {
      switch (dots) {
         case 0: {
            try {
               return Token(_num(std::stoll(value)), line, uro);
            }
            catch (...) {
               bigNumberException(value, line);
            }
         }
         case 1: {
            try {
               return Token(_num(stringToDouble(value)), line, uro);
            }
            catch (...) {
               bigNumberException(value, line);
            }
         }
         default: {
            throw SyntaxException(str(L"number '", value, L"' contains multiple dots"), line);
         }
      }
   }

   _int len = value.length();
   if (len > 2 && dots <= 1) {
      _char c1 = value.at(len - 2);
      _char c2 = value.back();
      nums = true;
      const _int n = len - 2;

      for (_int i = 0; i < n; i++) {
         _char c = value[i];
         if (!std::iswdigit(c) && c != L'.') {
            nums = false;
            break;
         }
      }

      if (nums) {
         _nint mult = getSuffixMultiplier(c1, c2);
         if (mult != -1) {
            _str value2 = value.substr(0, len - 2);
            if (dots == 0) {
               try {
                  // check for number overflow
                  // the number is multiplied by the suffix
                  // and then divided back
                  _nint i = std::stoll(value2);
                  _nint i2 = i * mult;
                  if (mult != 0 && i2 / mult != i) {
                     bigNumberException(value, line);
                  }
                  return Token(_num(i2), line, value, NumberMode::nm_Size, uro);
               }
               catch (...) {
                  bigNumberException(value, line);
               }
            }
            else {
               try {
                  _ndouble d = stringToDouble(value2);
                  d *= mult;

                  return Token(_num(d), line, value, NumberMode::nm_Size, uro);
               }
               catch (...) {
                  bigNumberException(value, line);
               }
            }
         }
      }
   }

   switch (dots) {
      case 0: {
         _str lower = value;
         toLower(lower);
         const _size hsh = rawStringHash(lower);

         auto fm = uro->hashes.HASH_MAP_MONTHS.find(hsh);
         if (fm != uro->hashes.HASH_MAP_MONTHS.end()) {
            return Token(_num(fm->second), line, value, NumberMode::nm_Month, uro);
         }

         auto fw = uro->hashes.HASH_MAP_WEEKDAYS.find(hsh);
         if (fw != uro->hashes.HASH_MAP_WEEKDAYS.end()) {
            return Token(_num(fw->second), line, value, NumberMode::nm_WeekDay, uro);
         }

         auto fk = uro->keywordsData.KEYWORDS.find(lower);
         if (fk == uro->keywordsData.KEYWORDS.end()) {
            return Token(hsh, line, value, uro);
         }
         else {
            return Token(fk->second, line, value, uro);
         }
      }
      case 1: {
         _int pnt = 0;
         for (_int i = 0; i < len; i++) {
            const _char& c = value[i];
            if (c == L'.') {
               pnt = i;
            }
         }

         if (pnt == len - 1) {
            throw SyntaxException(str(L"a time variable member was expected after '", value, L"'"), line);
         }

         const _str os1 = value.substr(0, pnt);
         const _str os2 = value.substr(pnt + 1);
         const _size h1 = stringHash(os1);
         const _size h2 = stringHash(os2);

         return Token(h1, h2, line, os1, os2, uro);
      }
      default: {
         throw SyntaxException(str(L"word '", value, L"' cannot contain multiple dots"), line);
      }
   }

   return Token(value, line, uro);
}

inline _ndouble stringToDouble(const _str& value)
{
   std::wstringstream ss(value);
   _ndouble n;
   ss >> n;
   return n;
}

inline void bigNumberException(const _str& value, const _int& line)
{
   throw SyntaxException(str(L"number '", value, L"' is too big to be stored in the memory"), line);
}

_nint getSuffixMultiplier(const _char& c1, const _char& c2)
{
   if (!(c2 == L'b' || c2 == L'B')) {
      return -1LL;
   }

   switch (c1) {
      case L'k': case L'K':
         return 1024LL;
      case L'm': case L'M':
         return 1048576LL;
      case L'g': case L'G':
         return 1073741824LL;
      case L't': case L'T':
         return 1099511627776LL;
      case L'p': case L'P':
         return 114349209288704LL;
      default:
         return -1LL;
   }
}

inline _boo isSymbol(const _char& ch)
{
   switch (ch) {
      case L',':
      case L'!':
      case L'=':
      case L'(':
      case L')':
      case L'{':
      case L'}':
      case L'[':
      case L']':
      case L':':
      case L';':
      case L'-':
      case L'+':
      case L'*':
      case L'%':
      case L'/':
      case L'<':
      case L'>':
      case L'?':
         return true;
      default:
         return false;
   }
}

_boo isNewLine(const _char& ch)
{
   return ch == L'\n';
}

inline _boo isAllowedInWord(const _char& ch)
{
   if (std::iswalpha(ch) || std::iswdigit(ch)) {
      return true;
   }

   switch (ch) {
      case L'.':
      case L'_':
         return true;
      default:
         return false;
   }
}

inline _boo isDoubleChar(const _char& ch)
{
   switch (ch) {
      case L'+':
      case L'-':
      case L'*':
         return true;
      default:
         return false;
   }
}

void invalidCharException(const _char& ch, const _int& line)
{
   switch (ch) {
      case L'^': {
         throw SyntaxException(L"you should use keyword 'xor' instead of character '^' as a boolean operator. "
            L"If your intention was to perform exponentiation, then function 'power()' is the right tool", line);
      }
      case L'&': {
         throw SyntaxException(L"you should use keyword 'and' instead of character '&' as a boolean operator", line);
      }
      case L'|': {
         throw SyntaxException(L"you should use keyword 'or' instead of character '|' as a boolean operator", line);
      }
      default: {
         throw SyntaxException(str(L"character '", charStr(ch), L"' is not allowed in Uroboros"), line);
      }
   }
}
