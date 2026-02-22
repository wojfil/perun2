/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <unordered_set>
#include "datatype/datatype.h"
#include "keyword.h"


namespace perun2
{

struct Perun2Process;

// Token represents the smallest syntax unit of the language

// if Token is a number, it can be
// 1) normal number    : 50, 0.34, -45.231, ...
// 2) file size        : 12mb, 100gb, 10kb, ...
// 3) month            : april, july, ...
// 4) day of the week  : monday, sunday, ...
// 5) decimal constant : 20k, 100m ..
// 6) infix constant   : 2k23, 2k20 ..
enum NumberMode
{
   nm_Normal = 0,
   nm_Size,
   nm_Month,
   nm_WeekDay,
   nm_Decimal,
   nm_Infix
};


union TokenValue
{
   // single symbol
   p_char singleChar;

   // the same symbol repeated multiple times in a row
   struct
   {
      p_char value;
      p_int amount;
   } repeatedChars;

   // number literal (123), file size (123mb) or time constant (June)
   struct
   {
      p_num value;
      NumberMode mode;
   } number;

   // keyword - important syntax element (print, if, copy...)
   Keyword keyword;


   // constructors:
   TokenValue();
   TokenValue(const p_char ch);
   TokenValue(const p_char ch, const p_int am);
   TokenValue(const p_num& n, const NumberMode nm);
   TokenValue(const Keyword k);
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

   Token() = delete;
   Token(const p_char v, const p_int li, const p_str& o);
   Token(const p_char v, const p_int am, const p_int li, const p_str& o);
   Token(const p_num& v, const NumberMode nm, const p_int li, const p_str& o);
   Token(const Type type, const p_int li, const p_str& o);
   Token(const Keyword v, const p_int li, const p_str& o);
   Token(const p_int li, const p_str& o, const p_str& o2);

   p_bool isCommandKeyword() const;
   p_bool isFilterKeyword() const;
   p_bool isExpForbiddenKeyword() const;
   p_bool isSymbol(const p_char ch) const;
   p_bool isKeyword(const Keyword kw) const;

   // is single word (CREATION)
   p_bool isWord(const p_char (&word)[]) const;
   p_bool isWord(const std::vector<p_str>& words) const;

   // is first word of two (CREATION.year)
   p_bool isFirstWord(const p_char (&word)[]) const;
   p_bool isFirstWord(const std::vector<p_str>& words) const;

   // is second word of two (creation.YEAR)
   p_bool isSecondWord(const p_char (&word)[]) const;
   p_bool isSecondWord(const std::vector<p_str>& words) const;

   // is single word (CREATION) OR is first word of two (CREATION.year)
   p_bool isVariable(const p_char (&word)[]) const;
   p_bool isVariable(const std::vector<p_str>& words) const;

   p_bool isNegatableKeywordOperator() const;
   p_bool isLogicConstant() const;
   p_bool isWeekDay() const;
   p_bool isMonth() const;
   p_bool isOne() const;
   p_bool isIntegerLiteral() const;
   p_bool isTimeAttribute() const;
   p_str toLowerString() const;

   const Type type;
   const TokenValue value;
   const p_int line;
   
   const p_str origin;
   const p_str lowercase;
   const p_str origin2;
   const p_str lowercase2;
   
private:
   p_str getTotalOrigin() const;

};


}
