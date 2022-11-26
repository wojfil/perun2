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

#include "parse-unit.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-number.h"
#include "../generator/gen-string.h"
#include "../generator/gen-time.h"
#include "../generator/gen-os.h"
#include "../../os.h"
#include "parse-number.h"
#include "parse-asterisk.h"


namespace uro::parse
{

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_bool>& result)
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
         return uro.vars.getVarValue(tk, result);
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_num>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = std::make_unique<gen::Constant<_num>>(tk.value.num.n);
         return true;
      }
      case Token::t_Word: {
         return uro.vars.getVarValue(tk, result);
      }
      case Token::t_TwoWords: {
         const Hashes& hs = uro.hashes;

         if (tk.value.twoWords.h1 == hs.HASH_NOTHING) {
            throw SyntaxException(L"dot . should be preceded by a time variable name", tk.line);
         }

         _genptr<_tim> var;
         if (!uro.vars.getVarValue(tk, var)) {
            throw SyntaxException(str(L"time variable from expression '", tk.getOriginString(uro),
               L".", tk.getOriginString_2(uro), L"' does not exist or is unreachable here"), tk.line);
         }

         const _size& h = tk.value.twoWords.h2;

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
            timeVariableMemberException(tk, uro);
         }

         return true;
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_str>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = std::make_unique<gen::Constant<_str>>(tk.value.num.n.toString());
         return true;
      }
      case Token::t_Quotation: {
         result = std::make_unique<gen::Constant<_str>>(tk.getOriginString(uro));
         return true;
      }
      case Token::t_Word: {
         return uro.vars.getVarValue(tk, result);
      }
      default: {
         return false;
      }
   }
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_nlist>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && uro.vars.getVarValue(tk, result);
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_tlist>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && uro.vars.getVarValue(tk, result);
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_list>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && uro.vars.getVarValue(tk, result);
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_tim>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return uro.vars.getVarValue(tk, result);
      }
      case Token::t_TwoWords: {
         if (tk.value.twoWords.h1 == uro.hashes.HASH_NOTHING) {
            throw SyntaxException(L"dot . should be preceded by a time variable name", tk.line);
         }

         _genptr<_tim> var;
         if (!uro.vars.getVarValue(tk, var)) {
            throw SyntaxException(str(L"time variable '", tk.getOriginString(uro), L"' does not exist"), tk.line);
         }

         if (tk.value.twoWords.h2 == uro.hashes.HASH_FUNC_DATE) {
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

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _genptr<_per>& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word && uro.vars.getVarValue(tk, result);
};

_bool parseOneToken(Uroboros& uro, const Tokens& tks, _defptr& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return uro.vars.getVarValue(tk, result);
      }
      case Token::t_MultiSymbol: {
         if (tk.value.chars.ch == L'*') {
            return parseAsteriskPattern(result, L"**", tk.line, uro);
         }
         else {
            return false;
         }
      }
      case Token::t_Symbol: {
         if (tk.value.ch == L'*') {
            return parseAsteriskPattern(result, L"*", tk.line, uro);
         }
         else {
            return false;
         }
      }
      case Token::t_Pattern: {
         return parseAsteriskPattern(result, tk.getOriginString(uro), tk.line, uro);
      }
      default: {
         return false;
      }
   }
};

}
