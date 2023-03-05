/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "token.h"
#include "uroboros.h"


namespace uro
{

OriginStringInfo::OriginStringInfo(const _size ind, const _size len)
   : index(ind), length(len) { };

TokenValue::TokenValue(const _char ch)
   : ch(ch) { };

TokenValue::TokenValue(const _char ch, const _int am)
   : chars({ ch, am }) { };

TokenValue::TokenValue(const _num& n, const _size os_id, const _size os_len, const NumberMode& nm)
   : num({ n, _osi(os_id, os_len), nm }) { };
TokenValue::TokenValue(const _size os_id, const _size os_len)
   : str(_osi(os_id, os_len)) { };

TokenValue::TokenValue(const _size os_id, const _size os_len, const _int id)
   : pattern({ _osi(os_id, os_len), id }) { };

TokenValue::TokenValue(const _hash h, const _size os_id, const _size os_len)
   : word({ h, _osi(os_id, os_len) }) { };

TokenValue::TokenValue(const Keyword& k, const _size os_id, const _size os_len)
   : keyword({ k, _osi(os_id, os_len) }) { };

TokenValue::TokenValue(const _hash h1, const _hash h2, const _size os_id1,
   const _size os_len1, const _size os_id2, const _size os_len2)
   : twoWords({ h1, h2, _osi(os_id1, os_len1), _osi(os_id2, os_len2) }) { };

Token::Token(const _char v, const _int li, _uro& uro)
   : line(li), type(t_Symbol), value(v) { };

Token::Token(const _char v, const _int am, const _int li, _uro& uro)
   : line(li), type(t_MultiSymbol), value(v, am) { };

Token::Token(const _num& v, const _int li, const _size os_id, const _size os_len,
   const NumberMode& nm, _uro& uro)
   : line(li), type(t_Number), value(v, os_id, os_len, nm) { };

Token::Token(const _size os_id, const _size os_len, const _int li, _uro& uro)
   : line(li), type(t_Quotation), value(os_id, os_len) { };

Token::Token(const _size os_id, const _size os_len, const _int id, const _int li, _uro& uro)
   : line(li), type(t_Pattern), value(os_id, os_len, id) { };

Token::Token(const _hash v, const _int li, const _size os_id, const _size os_len, _uro& uro)
   : line(li), type(t_Word), value(v, os_id, os_len) { };

Token::Token(const Keyword& v, const _int li, const _size os_id, const _size os_len, _uro& uro)
   : line(li), type(t_Keyword), value(v, os_id, os_len) { };

Token::Token(const _hash v1, const _hash v2, const _int li, const _size os_id1, const _size os_len1,
   const _size os_id2, const _size os_len2, _uro& uro)
   : line(li), type(t_TwoWords), value(v1, v2, os_id1, os_len1, os_id2, os_len2) { };

_bool Token::isSymbol(const _char ch) const
{
   return type == t_Symbol && value.ch == ch;
}

_bool Token::isKeyword(const Keyword& kw) const
{
   return type == t_Keyword && value.keyword.k == kw;
}

// this keyword is a binary operator and can be preceded by a Not keyword
_bool Token::isNegatableKeywordOperator() const
{
   if (type != t_Keyword) {
      return false;
   }

   switch (value.keyword.k) {
      case Keyword::kw_In:
      case Keyword::kw_Like:
         return true;
      default:
         return false;
   }
}

_bool Token::isLogicConstant() const
{
   return type == t_Keyword
      && (value.keyword.k == Keyword::kw_True || value.keyword.k == Keyword::kw_False);
}

_bool Token::isCommandKeyword() const
{
   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.keyword.k) {
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
         return true;
      default:
         return false;
   }
}

_bool Token::isFilterKeyword() const
{
   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.keyword.k) {
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

_bool Token::isExpForbiddenKeyword() const
{
   if (isCommandKeyword()) {
      return true;
   }

   if (type != Token::t_Keyword) {
      return false;
   }

   switch (value.keyword.k) {
      case Keyword::kw_Force:
      case Keyword::kw_Stack:
         return true;
      default:
         return false;
   }
}

_bool Token::isWeekDay() const
{
   return type == Token::t_Number && value.num.nm == NumberMode::nm_WeekDay;
}

_bool Token::isMonth() const
{
   return type == Token::t_Number && value.num.nm == NumberMode::nm_Month;
}

_str Token::getOriginString(_uro& uro) const
{
   switch (type) {
      case Token::Type::t_Symbol: {
         return toStr(value.ch);
      }
      case Token::Type::t_MultiSymbol: {
         return _str(value.chars.am, value.chars.ch);
      }
      case Token::Type::t_Number: {
         return getCodeSubstr(value.num.os, uro);
      }
      case Token::Type::t_Word: {
         return getCodeSubstr(value.word.os, uro);
      }
      case Token::Type::t_Keyword: {
         return getCodeSubstr(value.keyword.os, uro);
      }
      case Token::Type::t_Quotation: {
         return getCodeSubstr(value.str, uro);
      }
      case Token::Type::t_Pattern: {
         return getCodeSubstr(value.pattern.os, uro);
      }
      case Token::Type::t_TwoWords: {
         return getCodeSubstr(value.twoWords.os1, uro);
      }
      default: {
         return EMPTY_STRING;
      }
   }
}

_str Token::getOriginString_2(_uro& uro) const
{
   return type == Token::Type::t_TwoWords
      ? getCodeSubstr(value.twoWords.os2, uro)
      : EMPTY_STRING;
}

_str Token::getCodeSubstr(const _osi& osi, _uro& uro) const
{
   return uro.arguments.getCode().substr(osi.index, osi.length);
}

}
