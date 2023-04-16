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

#include "parse-unit.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-number.h"
#include "../generator/gen-string.h"
#include "../generator/gen-time.h"
#include "../generator/gen-os.h"
#include "../../os.h"
#include "parse-number.h"
#include "parse-asterisk.h"
#include "parse-var.h"


namespace perun2::parse
{

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_bool>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Keyword: {
         switch (tk.value.keyword.k) {
            case Keyword::kw_True: {
               result = std::make_unique<gen::Constant<_bool>>(true);
               return true;
            }
            case Keyword::kw_False: {
               result = std::make_unique<gen::Constant<_bool>>(false);
               return true;
            }
            default: {
               return false;
            }
         }
      }
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_num>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = std::make_unique<gen::Constant<_num>>(tk.value.num.n);
         return true;
      }
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      case Token::t_TwoWords: {
         const Hashes& hs = p2.hashes;

         if (tk.value.twoWords.h1 == hs.HASH_NOTHING) {
            throw SyntaxError(L"dot . should be preceded by a time variable name", tk.line);
         }

         _genptr<_tim> var;
         if (!makeVarRef(tk, var, p2)) {
            throw SyntaxError(str(L"time variable from expression '", tk.getOriginString(p2),
               L".", tk.getOriginString_2(p2), L"' does not exist or is unreachable here"), tk.line);
         }

         const _hash h = tk.value.twoWords.h2;

         if (h == hs.HASH_PER_YEAR || h == hs.HASH_PER_YEARS)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Years);
         else if (h == hs.HASH_PER_MONTH || h == hs.HASH_PER_MONTHS)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Months);
         else if (h == hs.HASH_PER_WEEKDAY)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Weeks);
         else if (h == hs.HASH_PER_DAY || h == hs.HASH_PER_DAYS)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Days);
         else if (h == hs.HASH_PER_HOUR || h == hs.HASH_PER_HOURS)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Hours);
         else if (h == hs.HASH_PER_MINUTE || h == hs.HASH_PER_MINUTES)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Minutes);
         else if (h == hs.HASH_PER_SECOND || h == hs.HASH_PER_SECOND)
            result = std::make_unique<gen::TimeMember>(var, Period::u_Seconds);
         else if (h == hs.HASH_PER_DATE)
            return false;
         else {
            timeVariableMemberException(tk, p2);
         }

         return true;
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_str>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = std::make_unique<gen::Constant<_str>>(tk.value.num.n.toString());
         return true;
      }
      case Token::t_Quotation: {
         result = std::make_unique<gen::Constant<_str>>(tk.getOriginString(p2));
         return true;
      }
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_nlist>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_tlist>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_list>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_tim>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      case Token::t_TwoWords: {
         if (tk.value.twoWords.h1 == p2.hashes.HASH_NOTHING) {
            throw SyntaxError(L"dot . should be preceded by a time variable name", tk.line);
         }

         _genptr<_tim> var;
         if (!makeVarRef(tk, var, p2)) {
            throw SyntaxError(str(L"time variable '", tk.getOriginString(p2), L"' does not exist"), tk.line);
         }

         if (tk.value.twoWords.h2 == p2.hashes.HASH_FUNC_DATE) {
            result = std::make_unique<gen::TimeDate>(var);
            return true;
         }
         else {
            return false;
         }
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_per>& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

_bool parseOneToken(_p2& p2, const Tokens& tks, _defptr& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      case Token::t_MultiSymbol: {
         if (tk.value.chars.ch == CHAR_ASTERISK) {
            return parseAsteriskPattern(result, str(CHAR_ASTERISK, CHAR_ASTERISK), tk.line, p2);
         }
         else {
            return false;
         }
      }
      case Token::t_Symbol: {
         if (tk.value.ch == CHAR_ASTERISK) {
            return parseAsteriskPattern(result, toStr(CHAR_ASTERISK), tk.line, p2);
         }
         else {
            return false;
         }
      }
      case Token::t_Pattern: {
         return parseAsteriskPattern(result, tk.getOriginString(p2), tk.line, p2);
      }
      default: {
         return false;
      }
   }
};

}
