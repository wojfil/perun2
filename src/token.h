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

#ifndef TOKEN_H
#define TOKEN_H

#include <unordered_set>
#include "datatype/datatype.h"
#include "keyword.h"


namespace perun2
{

struct p_perun2;

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


// representation of certain token in the source code string
// index of starting character + length of the character sequence
struct OriginStringInfo
{
public:
   OriginStringInfo() = delete;
   OriginStringInfo(const p_size ind, const p_size len);

   const p_size index;
   const p_size length;
};


typedef OriginStringInfo p_osi;


// the value of token can be...
union TokenValue
{
   // single symbol
   p_char ch;

   // the same symbol repeated multiple times in a row
   struct
   {
      p_char ch;
      p_int am; // amount
   } chars;

   // number literal (123), file size (123mb) or time constant (June)
   struct
   {
      p_num n;
      p_osi os; // os = original appearance of token in the source code
      NumberMode nm;
   } num;

   // string literal
   p_osi str;

   // pattern
   struct
   {
      p_osi os;
      p_int id; // index of first asterisk
   } pattern;

   // word - variable name, function name
   struct
   {
      p_osi os;
   } word;

   // keyword - important syntax element (print, if, copy...)
   struct
   {
      Keyword k;
      p_osi os;
   } keyword;

   // two words - time variable member (creation.year)
   struct
   {
      p_osi os1;
      p_osi os2;
   } twoWords;

   // constructors:
   TokenValue() = delete;
   TokenValue(const p_char ch);
   TokenValue(const p_char ch, const p_int am);
   TokenValue(const p_num& n, const p_size os_id, const p_size os_len, const NumberMode nm);
   TokenValue(const p_size os_id, const p_size os_len);
   TokenValue(const p_size os_id, const p_size os_len, const p_int id);
   TokenValue(const p_size os_id, const p_size os_len, const p_size len2);
   TokenValue(const Keyword k, const p_size os_id, const p_size os_len);
   TokenValue(const p_size os_id1, const p_size os_len1, const p_size os_id2, const p_size os_len2);
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
   Token(const p_char v, const p_int li, p_perun2& p2);
   Token(const p_char v, const p_int am, const p_int li, p_perun2& p2);
   Token(const p_num& v, const p_int li, const p_size os_id, const p_size os_len, const NumberMode nm, p_perun2& p2);
   Token(const p_size os_id, const p_size os_len, const p_int li, p_perun2& p2);
   Token(const p_size os_id, const p_size os_len, const p_int id, const p_int li, p_perun2& p2);
   Token(const p_int li, const p_size os_id, const p_size os_len, p_perun2& p2);
   Token(const Keyword v, const p_int li, const p_size os_id, const p_size os_len, p_perun2& p2);
   Token(const p_int li, const p_size os_id1, const p_size os_len1, const p_size os_id2, const p_size os_len2, p_perun2& p2);

   p_bool isCommandKeyword() const;
   p_bool isFilterKeyword() const;
   p_bool isExpForbiddenKeyword() const;
   p_bool isSymbol(const p_char ch) const;
   p_bool isKeyword(const Keyword kw) const;

   // is single word (CREATION)
   p_bool isWord(const p_char (&word)[], p_perun2& p2) const;
   p_bool isWord(const std::vector<p_str>& words, p_perun2& p2) const;

   // is first word of two (CREATION.year)
   p_bool isFirstWord(const p_char (&word)[], p_perun2& p2) const;
   p_bool isFirstWord(const std::vector<p_str>& words, p_perun2& p2) const;

   // is second word of two (creation.YEAR)
   p_bool isSecondWord(const p_char (&word)[], p_perun2& p2) const;
   p_bool isSecondWord(const std::vector<p_str>& words, p_perun2& p2) const;

   // is single word (CREATION) OR is first word of two (CREATION.year)
   p_bool isVariable(const p_char (&word)[], p_perun2& p2) const;
   p_bool isVariable(const std::vector<p_str>& words, p_perun2& p2) const;

   p_bool isNegatableKeywordOperator() const;
   p_bool isLogicConstant() const;
   p_bool isWeekDay() const;
   p_bool isMonth() const;
   p_bool isOne() const;
   p_bool isTimeAttribute(p_perun2& p2) const;
   p_str getOriginString(p_perun2& p2) const;
   p_str getOriginString_2(p_perun2& p2) const;
   p_str toLowerString(p_perun2& p2) const;

   const Type type;
   const p_int line;
   const TokenValue value;
   
private:
   p_str getCodeSubstr(const p_osi& osi, p_perun2& p2) const;
   p_bool isCodeSubstr(const p_char (&word)[], const p_osi& osi, p_perun2& p2) const;
   p_bool isCodeSubstr(const p_str& word, const p_osi& osi, p_perun2& p2) const;

};


}

#endif /* TOKEN_H */
