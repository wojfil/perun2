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

#include "token.h"
#include "hash.h"


Token::Token(const _char& v, const _int& li)
{
   line = li;
   type = t_Symbol;
   value.c = v;
};

Token::Token(const _num& v, const _int& li)
{
   line = li;
   type = t_Number;
   value.n = v;
}

Token::Token(const _num& v, const _int& li, const _str& os, const NumberMode& nm)
{
   line = li;
   type = t_Number;
   value.n = v;
   originString = os;
   mode = nm;
}

Token::Token(const _str& v, const _int& li)
{
   line = li;
   type = t_Quotation;
   value.sl = v;
}

Token::Token(const _size& v, const _int& li, const _str& os)
{
   line = li;
   type = t_Word;
   value.h1 = v;
   originString = os;
}

Token::Token(const Keyword& v, const _int& li, const _str& os)
{
   line = li;
   type = t_Keyword;
   value.k = v;
   originString = os;
}

Token::Token(const _size& v1, const _size& v2, const _int& li, const _str& os1, const _str& os2)
{
   line = li;
   type = t_TwoWords;
   value.h1 = v1;
   value.h2 = v2;
   originString = os1;
   originString2 = os2;
}



_boo Token::isSymbol(const _char& ch) const
{
   return type == t_Symbol && value.c == ch;
}
_boo Token::isKeyword(const Keyword& kw) const
{
   return type == t_Keyword && value.k == kw;
}

_boo Token::isLogicConstant() const
{
   return type == t_Keyword
      && (value.k == Keyword::kw_True || value.k == Keyword::kw_False);
}

_boo operator== (const Token& t1, const Token& t2)
{
   if (t1.type != t2.type) {
      return false;
   }
   else {
      switch (t1.type) {
         case Token::t_Number:
            return t1.value.n == t2.value.n;
         case Token::t_Keyword:
            return t1.value.k == t2.value.k;
         case Token::t_Quotation:
            return t1.value.sl == t2.value.sl;
         case Token::t_Symbol:
            return t1.value.c == t2.value.c;
         case Token::t_TwoWords:
            return t1.value.h1 == t2.value.h1
               && t1.value.h2 == t2.value.h2;
         case Token::t_Word:
            return t1.value.h1 == t2.value.h1;
         default:
            return false;
      }
   }
}

_boo Token::isCommandKeyword() const
{
   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.k) {
      case Keyword::kw_Copy:
      case Keyword::kw_Create:
      case Keyword::kw_Delete:
      case Keyword::kw_Download:
      case Keyword::kw_Drop:
      case Keyword::kw_Hide:
      case Keyword::kw_Lock:
      case Keyword::kw_Move:
      case Keyword::kw_Open:
      case Keyword::kw_Print:
      case Keyword::kw_Run:
      case Keyword::kw_Reaccess:
      case Keyword::kw_Rechange:
      case Keyword::kw_Recreate:
      case Keyword::kw_Remodify:
      case Keyword::kw_Rename:
      case Keyword::kw_Sleep:
      case Keyword::kw_Select:
      case Keyword::kw_Unhide:
      case Keyword::kw_Unlock:
      case Keyword::kw_Exit:
      case Keyword::kw_Error:
      case Keyword::kw_Break:
      case Keyword::kw_Continue:
      case Keyword::kw_Process:
         return true;
      default:
         return false;
   }
}

_boo Token::isFiltherKeyword() const
{
   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.k) {
      case Keyword::kw_Every:
      case Keyword::kw_Limit:
      case Keyword::kw_Order:
      case Keyword::kw_Skip:
      case Keyword::kw_Where:
         return true;
      default:
         return false;
   }
}

_boo Token::isWeekDay() const
{
   return type == Token::t_Number && mode == Token::nm_WeekDay;
}

_boo Token::isMonth() const
{
   return type == Token::t_Number && mode == Token::nm_Month;
}
