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

#include "../include/perun2/token.hpp"
#include "../include/perun2/perun2.hpp"


namespace perun2
{

TokenValue::TokenValue()
   : singleChar() { };

TokenValue::TokenValue(const p_char ch)
   : singleChar(ch) { };

TokenValue::TokenValue(const p_char ch, const p_int am)
   : repeatedChars({ ch, am }) { };

TokenValue::TokenValue(const p_num& n, const NumberMode nm)
   : number({ n, nm }) { };

TokenValue::TokenValue(const Keyword k)
   : keyword(k) { };



Token::Token(const p_char v, const p_int li, const p_str& o)
   : type(t_Symbol), value(v), line(li), origin(o), origin2(), lowercase(str_lowercased(o)) { };

Token::Token(const p_char v, const p_int am, const p_int li, const p_str& o)
   : type(t_MultiSymbol), value(v, am), line(li), origin(o), origin2(), lowercase(str_lowercased(o)) { };

Token::Token(const p_num& v, const NumberMode nm, const p_int li, const p_str& o)
   : type(t_Number), value(v, nm), line(li), origin(o), origin2(), lowercase(str_lowercased(o)) { };

Token::Token(const Type type, const p_int li, const p_str& o)
   : type(type), value(), line(li), origin(o), origin2(), lowercase(str_lowercased(o)) { };

Token::Token(const Keyword v, const p_int li, const p_str& o)
   : type(t_Keyword), value(v), line(li), origin(o), origin2(), lowercase(str_lowercased(o)) { };

Token::Token(const p_int li, const p_str& o, const p_str& o2)
   : type(t_TwoWords), value(), line(li), origin(o), origin2(o2), 
     lowercase(str_lowercased(o)), lowercase2(str_lowercased(o2)) { };



p_bool Token::isSymbol(const p_char ch) const
{
   return type == t_Symbol && value.singleChar == ch;
}

p_bool Token::isKeyword(const Keyword kw) const
{
   return type == t_Keyword && value.keyword == kw;
}

p_bool Token::isWord(const p_char (&word)[]) const
{
   return type == t_Word && p_str(word) == lowercase;
}

p_bool Token::isWord(const std::vector<p_str>& words) const
{
   if (type != t_Word) {
      return false;
   }

   for (const p_str& w : words) {
      if (w == lowercase) {
         return true;
      }
   }

   return false;
}

p_bool Token::isFirstWord(const p_char (&word)[]) const
{
   return type == t_TwoWords && p_str(word) == lowercase;
}

p_bool Token::isFirstWord(const std::vector<p_str>& words) const
{
   if (type != t_TwoWords) {
      return false;
   }

   for (const p_str& w : words) {
      if (w == lowercase) {
         return true;
      }
   }

   return false;
}

p_bool Token::isSecondWord(const p_char (&word)[]) const
{
   return type == t_TwoWords && p_str(word) == lowercase2;
}

p_bool Token::isSecondWord(const std::vector<p_str>& words) const
{
   if (type != t_TwoWords) {
      return false;
   }

   for (const p_str& w : words) {
      if (w == lowercase2) {
         return true;
      }
   }

   return false;
}

p_bool Token::isVariable(const p_char (&word)[]) const
{
   switch (this->type) {
      case Token::t_Word: {
         return isWord(word);
      }
      case Token::t_TwoWords: {
         return isFirstWord(word);
      }
      default: {
         return false;
      }
   }
}

p_bool Token::isVariable(const std::vector<p_str>& words) const
{
   switch (this->type) {
      case Token::t_Word: {
         return isWord(words);
      }
      case Token::t_TwoWords: {
         return isFirstWord(words);
      }
      default: {
         return false;
      }
   }
}

// this keyword is a binary operator and can be preceded by a Not keyword
p_bool Token::isNegatableKeywordOperator() const
{
   if (type != t_Keyword) {
      return false;
   }

   switch (value.keyword) {
      case Keyword::kw_In:
      case Keyword::kw_Like:
      case Keyword::kw_Resembles:
      case Keyword::kw_Between:
      case Keyword::kw_Regexp:
         return true;
      default:
         return false;
   }
}

p_bool Token::isLogicConstant() const
{
   return type == t_Keyword
      && (value.keyword == Keyword::kw_True || value.keyword == Keyword::kw_False);
}

p_bool Token::isCommandKeyword() const
{
   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.keyword) {
      case Keyword::kw_Copy:
      case Keyword::kw_Create:
      case Keyword::kw_CreateFile:
      case Keyword::kw_CreateDirectory:
      case Keyword::kw_CreateFiles:
      case Keyword::kw_CreateDirectories:
      case Keyword::kw_Delete:
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
      case Keyword::kw_Popup:
      case Keyword::kw_Python:
      case Keyword::kw_Python3:
      case Keyword::kw_Execute:
         return true;
      default:
         return false;
   }
}

p_bool Token::isFilterKeyword() const
{
   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.keyword) {
      case Keyword::kw_Every:
      case Keyword::kw_Final:
      case Keyword::kw_Limit:
      case Keyword::kw_Order:
      case Keyword::kw_Skip:
      case Keyword::kw_Where:
         return true;
      default:
         return false;
   }
}

p_bool Token::isExpForbiddenKeyword() const
{
   if (isCommandKeyword()) {
      return true;
   }

   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.keyword) {
      case Keyword::kw_Force:
      case Keyword::kw_Stack:
         return true;
      default:
         return false;
   }
}

p_bool Token::isWeekDay() const
{
   return type == Token::t_Number && value.number.mode == NumberMode::nm_WeekDay;
}

p_bool Token::isMonth() const
{
   return type == Token::t_Number && value.number.mode == NumberMode::nm_Month;
}

p_bool Token::isOne() const
{
   return type == Token::t_Number && value.number.value.isOne();
}

p_bool Token::isIntegerLiteral() const
{
   return type == Token::t_Number && value.number.value.state == NumberState::Int;
}

p_bool Token::isTimeAttribute() const
{
   return this->type == Token::t_Word && isWord(STRINGS_TIME_ATTR);
}

p_str Token::toLowerString() const
{
   return this->type == Token::t_TwoWords
      ? str(this->lowercase, L".", this->lowercase2) : this->lowercase;
}

p_str Token::getTotalOrigin() const
{
   return this->type == Token::t_TwoWords
      ? str(this->origin, L".", this->origin2) : this->origin;
}


}
