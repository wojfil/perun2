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

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "datatype/datatype.h"
#include "keyword.h"

struct Uroboros;

enum NumberMode
{
   nm_Normal = 0,
   nm_Size,
   nm_Month,
   nm_WeekDay
};

union TokenValue
{
   // single symbol
   _char ch;

   // number literal (123), file size (123mb) or time constant (June)
   struct
   {
      _num n;
      _str* os;
      NumberMode nm;
   } num;

   // string literal
   _str* str;

   // pattern
   struct
   {
      _str* str;
      _int id; // index of first asterisk
   } pattern;

   // word - variable name, function name
   struct
   {
      _size h;  // h  = hash of string
      _str* os; // os = original appearance of token in the source code
   } word;

   // keyword - important syntax element (print, if, copy...)
   struct
   {
      Keyword k;
      _str* os;
   } keyword;

   // two words - time variable member (creation.year)
   struct
   {
      _size h1;
      _size h2;
      _str* os1;
      _str* os2;
   } twoWords;

   // constructors:
   TokenValue(const _char& ch) : ch(ch) {};
   TokenValue(const _num& n, _str* os, const NumberMode& nm)
      : num({ n, os, nm }) {};
   TokenValue(_str* str) : str(str) {};
   TokenValue(_str* str, const _int& id) : pattern({ str, id }) {};
   TokenValue(const _size& h, _str* os) : word({ h, os }) {};
   TokenValue(const Keyword& k, _str* os) : keyword({ k, os }) {};
   TokenValue(const _size& h1, const _size& h2, _str* os1, _str* os2)
      : twoWords({ h1, h2, os1, os2 }) {};
};


struct Token
{
public:

   enum Type
   {
      t_Symbol = 0,
      t_Number,
      t_Word,
      t_Keyword,
      t_Quotation,
      t_Pattern,
      t_TwoWords
   };

   // constructors:
   Token(const _char& v, const _int& li, Uroboros* uro);
   Token(const _num& v, const _int& li, Uroboros* uro);
   Token(const _num& v, const _int& li, const _str& os,
      const NumberMode& nm, Uroboros* uro);
   Token(const _str& v, const _int& li, Uroboros* uro);
   Token(const _str& v, const _int& id, const _int& li, Uroboros* uro);
   Token(const _size& v, const _int& li, const _str& os, Uroboros* uro);
   Token(const Keyword& v, const _int& li, const _str& os, Uroboros* uro);
   Token(const _size& v1, const _size& v2, const _int& li, const _str& os1,
      const _str& os2, Uroboros* uro);

   // members:
   const Type type;
   const _int line;
   const TokenValue value;

   // methods:
   _boo isCommandKeyword() const;
   _boo isFiltherKeyword() const;
   _boo isSymbol(const _char& ch) const;
   _boo isKeyword(const Keyword& kw) const;
   _boo isLogicConstant() const;
   _boo isWeekDay() const;
   _boo isMonth() const;
};


#endif /* TOKEN_H */
