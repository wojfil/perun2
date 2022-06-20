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
std::vector<Token> tokenize(const _str& code)
{
   enum Mode {
      m_Normal = 0,
      m_Word,
      m_Quote,
      m_SingleComment,
      m_MultiComment
   }; // finite-state machine within function

   std::vector<Token> tokens;
   Mode mode = Mode::m_Normal;
   _int line = 1;
   _char prev = L' ';
   const _size len = code.length();
   _size wpos = 0, wlen = 0;

   for (_size i = 0; i < len; i++) {
      const _char& c = code[i];
      switch (mode)  {
         case Mode::m_Normal: {
            if (c == L'"') {
               throw SyntaxException(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
            }
            else if (c == L'`') {
               throw SyntaxException(L"you should use apostrophes ' instead of backtick characters ` for string literals", line);
            }
            else if (isSymbol(c)) {
               if (i != 0 && prev == L'/') {
                  if (c == L'*')  {
                     mode = Mode::m_MultiComment;
                     tokens.pop_back();
                  }
                  else if (c == L'/')  {
                     mode = Mode::m_SingleComment;
                     tokens.pop_back();
                  }
                  else  {
                     tokens.push_back(Token(c, line));
                  }
               }
               else  {
                  tokens.push_back(Token(c, line));
               }
            }
            else {
               if (isAllowedInWord(c)) {
                  wpos = i;
                  wlen = 1;
                  mode = Mode::m_Word;
               }
               else if (isNewLine(c)) {
                  line++;
               }
               else if (c == L'\'') {
                  wpos = i + 1;
                  wlen = 0;
                  mode = Mode::m_Quote;
               }
               else if (c != L' '){
                  invalidCharException(c, line);
               }
            }
            break;
         }
         case Mode::m_Word: {
            if (c == L'"') {
               throw SyntaxException(L"you should use apostrophes ' instead of quotation marks \" for string literals", line);
            }
            else if (c == L'`') {
               throw SyntaxException(L"you should use apostrophes ' instead of backtick characters ` for string literals", line);
            }
            else if (isAllowedInWord(c)) {
               wlen++;
            }
            else {
               _str word = code.substr(wpos, wlen);
               tokens.push_back(wordToken(word, line));
               wlen = 0;
               mode = Mode::m_Normal;

               if (isSymbol(c)) {
                  tokens.push_back(Token(c, line));
               }
               else if (isNewLine(c)) {
                  line++;
               }
               else if (c == L'\'') {
                  wpos = i + 1;
                  mode = Mode::m_Quote;
               }
               else if (c != L' ') {
                  invalidCharException(c, line);
               }
            }
            break;
         }
         case Mode::m_Quote: {
            if (c == L'\'') {
               const _str word = code.substr(wpos, wlen);
               tokens.push_back(Token(word, line));
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

   if (mode == m_Word && wlen != 0) {
      _str word = code.substr(wpos, wlen);
      tokens.push_back(wordToken(word, line));
   }
   else if (mode == m_Quote) {
      throw SyntaxException(L"an opened string literal is not closed", line);
   }

   return tokens;
}

static Token wordToken(_str& str, _int& line)
{
   _int dots = 0;
   _boo nums = true;

   for (const _char &c : str) {
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
               return Token(_num(std::stoll(str)), line);
            }
            catch (...) {
               bigNumberException(str, line);
            }
         }
         case 1: {
            try {
               return Token(_num(stringToDouble(str)), line);
            }
            catch (...) {
               bigNumberException(str, line);
            }
         }
         default: {
            throw SyntaxException(L"number '" + str
               + L"' contains multiple dots", line);
         }
      }
   }

   _int len = str.length();
   if (len > 2 && dots <= 1) {
      _char c1 = str.at(len - 2);
      _char c2 = str.back();
      nums = true;
      const _int n = len - 2;

      for (_int i = 0; i < n; i++) {
         _char c = str[i];
         if (!std::iswdigit(c) && c != L'.') {
            nums = false;
            break;
         }
      }

      if (nums) {
         _nint mult = getSuffixMultiplier(c1, c2);
         if (mult != -1) {
            _str str2 = str.substr(0, len - 2);
            if (dots == 0) {
               try {
                  // check for number overflow
                  // the number is multiplied by the suffix
                  // and then divided back
                  _nint i = std::stoll(str2);
                  _nint i2 = i * mult;
                  if (mult != 0 && i2 / mult != i) {
                     bigNumberException(str, line);
                  }
                  return Token(_num(i2), line, str, Token::nm_Size);
               }
               catch (...) {
                  bigNumberException(str, line);
               }
            }
            else {
               try {
                  _ndouble d = stringToDouble(str2);
                  d *= mult;

                  return Token(_num(d), line, str, Token::nm_Size);
               }
               catch (...) {
                  bigNumberException(str, line);
               }
            }
         }
      }
   }

   switch (dots) {
      case 0: {
         _str lower = str;
         toLower(lower);
         const _size hsh = rawStringHash(lower);

         auto fm = HASH_MAP_MONTHS.find(hsh);
         if (fm != HASH_MAP_MONTHS.end()) {
            return Token(_num(fm->second), line, str, Token::nm_Month);
         }

         auto fw = HASH_MAP_WEEKDAYS.find(hsh);
         if (fw != HASH_MAP_WEEKDAYS.end()) {
            return Token(_num(fw->second), line, str, Token::nm_WeekDay);
         }

         auto fk = KEYWORDS.find(lower);
         if (fk == KEYWORDS.end()) {
            return Token(hsh, line, str);
         }
         else {
            return Token(fk->second, line, str);
         }
      }
      case 1: {
         _int pnt = 0;
         for (_int i = 0; i < len; i++) {
            const _char& c = str[i];
            if (c == L'.') {
               pnt = i;
            }
         }

         if (pnt == len - 1) {
            throw SyntaxException(L"a time variable member was expected after '" + str + L"'", line);
         }

         const _str os1 = str.substr(0, pnt);
         const _str os2 = str.substr(pnt + 1);
         const _size h1 = stringHash(os1);
         const _size h2 = stringHash(os2);

         return Token(h1, h2, line, os1, os2);
      }
      default: {
         throw SyntaxException(L"word '" + str + L"' cannot contain multiple dots", line);
      }
   }

   return Token(str, line);
}

inline _ndouble stringToDouble(const _str& str)
{
   std::wstringstream ss(str);
   _ndouble n;
   ss >> n;
   return n;
}

inline void bigNumberException(const _str& str, const _int& line)
{
   throw SyntaxException(L"number '" + str + L"' is too big to be stored in the memory", line);
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
         throw SyntaxException(L"character '" + _str(1, ch) + L"' is not allowed in Uroboros", line);
      }
   }
}
