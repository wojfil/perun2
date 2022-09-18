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

// Token represents the smallest syntax unit of the language


// if Token is a number, it can be
// 1) normal number   : 50, 0.34, -45.231, ...
// 2) file size       : 12mb, 100gb, 10kb, ...
// 3) month           : april, july, ...
// 4) day of the week : monday, sunday, ...
enum NumberMode
{
   nm_Normal = 0,
   nm_Size,
   nm_Month,
   nm_WeekDay
};


// this struct contains information
// about representation of certain token in the source code string
// this is just an index of starting character
// and length of the character sequence
struct OriginStringInfo
{
public:
   OriginStringInfo() = delete;
   OriginStringInfo(const _size& ind, const _size& len);

   const _size index;
   const _size length;
};


#define _osi OriginStringInfo


union TokenValue
{
   // single symbol
   _char ch;

   // the same symbol repeated multiple times in a row
   struct
   {
      _char ch;
      _int am; // amount
   } chars;

   // number literal (123), file size (123mb) or time constant (June)
   struct
   {
      _num n;
      _osi os; // os = original appearance of token in the source code
      NumberMode nm;
   } num;

   // string literal
   _osi str;

   // pattern
   struct
   {
      _osi os;
      _int id; // index of first asterisk
   } pattern;

   // word - variable name, function name
   struct
   {
      _size h;  // h  = hash of string
      _osi os;
   } word;

   // keyword - important syntax element (print, if, copy...)
   struct
   {
      Keyword k;
      _osi os;
   } keyword;

   // two words - time variable member (creation.year)
   struct
   {
      _size h1;
      _size h2;
      _osi os1;
      _osi os2;
   } twoWords;

   // constructors:
   TokenValue() = delete;
   TokenValue(const _char& ch);
   TokenValue(const _char& ch, const _int& am);
   TokenValue(const _num& n, const _size& os_id, const _size& os_len, const NumberMode& nm);
   TokenValue(const _size& os_id, const _size& os_len);
   TokenValue(const _size& os_id, const _size& os_len, const _int& id);
   TokenValue(const _size& h, const _size& os_id, const _size& os_len);
   TokenValue(const Keyword& k, const _size& os_id, const _size& os_len);
   TokenValue(const _size& h1, const _size& h2, const _size& os_id1, 
      const _size& os_len1, const _size& os_id2, const _size& os_len2);
};


struct Token
{
public:

   enum Type
   {
      t_Symbol = 0,
      t_MultiSymbol,
      t_Number,
      t_Word,
      t_Keyword,
      t_Quotation,
      t_Pattern,
      t_TwoWords
   };

   // constructors:
   Token() = delete;
   Token(const _char& v, const _int& li, Uroboros* uro);
   Token(const _char& v, const _int& am, const _int& li, Uroboros* uro);
   Token(const _num& v, const _int& li, const _size& os_id, const _size& os_len,
      const NumberMode& nm, Uroboros* uro);
   Token(const _size& os_id, const _size& os_len, const _int& li, Uroboros* uro);
   Token(const _size& os_id, const _size& os_len, const _int& id, const _int& li, Uroboros* uro);
   Token(const _size& v, const _int& li, const _size& os_id, const _size& os_len, Uroboros* uro);
   Token(const Keyword& v, const _int& li, const _size& os_id, const _size& os_len, Uroboros* uro);
   Token(const _size& v1, const _size& v2, const _int& li, const _size& os_id1, const _size& os_len1,
      const _size& os_id2, const _size& os_len2, Uroboros* uro);

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
   _str getOriginString(Uroboros* uro) const;
   _str getOriginString_2(Uroboros* uro) const;

private:
   _str getCodeSubstr(const _osi& osi, Uroboros* uro) const;

};


#endif /* TOKEN_H */
