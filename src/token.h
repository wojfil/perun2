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


struct Token
{
   enum Type
   {
      t_Symbol = 0,
      t_Number,
      t_Word,
      t_Keyword,
      t_Quotation,
      t_TwoWords
   };

   enum NumberMode
   {
      nm_Normal = 0,
      nm_Size,
      nm_Month,
      nm_WeekDay
   };

   Token(const _char& v, const _int& li);
   Token(const _num& v, const _int& li);
   Token(const _num& v, const _int& li, const _str& os, const NumberMode& nm);
   Token(const _str& v, const _int& li);
   Token(const _size& v, const _int& li, const _str& os);
   Token(const Keyword& v, const _int& li, const _str& os);
   Token(const _size& v1, const _size& v2, const _int& li, const _str& os1, const _str& os2);

   _boo isCommandKeyword() const;
   _boo isFiltherKeyword() const;
   _boo isSymbol(const _char& ch) const;
   _boo isKeyword(const Keyword& kw) const;
   _boo isLogicConstant() const;
   _boo isTimeVariable() const;
   _boo isWeekDay() const;
   _boo isMonth() const;
   friend _boo operator== (const Token& t1, const Token& t2);

   Type type;
   _int line;
   struct
   {
      _char c;
      _num n;
      _str sl; // string literal
      _size h1; // hash of first word
      _size h2; // hash of second word
      Keyword k;
   } value;
   _str originString = L"";
   _str originString2 = L""; // for time variable members only
   NumberMode mode = NumberMode::nm_Normal;
};


#endif /* TOKEN_H */
