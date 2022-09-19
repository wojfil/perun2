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


namespace uro::parse
{

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_boo>*& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Keyword: {
         switch (tk.value.keyword.k) {
            case Keyword::kw_True: {
               result = new gen::Constant<_boo>(true);
               return true;
            }
            case Keyword::kw_False: {
               result = new gen::Constant<_boo>(false);
               return true;
            }
            default: {
               return false;
            }
         }
      }
      case Token::t_Word: {
         return uro->vars.getVarValue(tk, result);
      }
      default: {
         return false;
      }
   }
};

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_num>*& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = new gen::Constant<_num>(tk.value.num.n);
         return true;
      }
      case Token::t_Word: {
         return uro->vars.getVarValue(tk, result);
      }
      case Token::t_TwoWords: {
         const Hashes& hs = uro->hashes;

         if (tk.value.twoWords.h1 == hs.HASH_NOTHING) {
            throw SyntaxException(L"dot . should be preceded by a time variable name", tk.line);
         }

         Generator<_tim>* var;
         if (!uro->vars.getVarValue(tk, var)) {
            throw SyntaxException(str(L"time variable from expression '", tk.getOriginString(uro),
               L".", tk.getOriginString_2(uro), L"' does not exist or is unreachable here"), tk.line);
         }

         const _size& h = tk.value.twoWords.h2;

         if (h == hs.HASH_PER_YEAR || h == hs.HASH_PER_YEARS)
            result = new gen::TimeYears(var);
         else if (h == hs.HASH_PER_MONTH || h == hs.HASH_PER_MONTHS)
            result = new gen::TimeMonths(var);
         else if (h == hs.HASH_PER_WEEKDAY)
            result = new gen::TimeWeekDay(var);
         else if (h == hs.HASH_PER_DAY || h == hs.HASH_PER_DAYS)
            result = new gen::TimeDays(var);
         else if (h == hs.HASH_PER_HOUR || h == hs.HASH_PER_HOURS)
            result = new gen::TimeHours(var);
         else if (h == hs.HASH_PER_MINUTE || h == hs.HASH_PER_MINUTES)
            result = new gen::TimeMinutes(var);
         else if (h == hs.HASH_PER_SECOND || h == hs.HASH_PER_SECOND)
            result = new gen::TimeSeconds(var);
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

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_str>*& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = new gen::Constant<_str>(tk.value.num.n.toString());
         return true;
      }
      case Token::t_Quotation: {
         result = new gen::Constant<_str>(tk.getOriginString(uro));
         return true;
      }
      case Token::t_Word: {
         return uro->vars.getVarValue(tk, result);
      }
      default: {
         return false;
      }
   }
};

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_nlist>*& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_tlist>*& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_list>*& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_tim>*& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return uro->vars.getVarValue(tk, result);
      }
      case Token::t_TwoWords: {
         if (tk.value.twoWords.h1 == uro->hashes.HASH_NOTHING) {
            throw SyntaxException(L"dot . should be preceded by a time variable name", tk.line);
         }

         Generator<_tim>* var;
         if (!uro->vars.getVarValue(tk, var)) {
            throw SyntaxException(str(L"time variable '", tk.getOriginString(uro), L"' does not exist"), tk.line);
         }

         if (tk.value.twoWords.h2 == uro->hashes.HASH_FUNC_DATE) {
            result = new gen::TimeDate(var);
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

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, Generator<_per>*& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(uro::Uroboros* uro, const Tokens& tks, _def*& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return uro->vars.getVarValue(tk, result);
      }
      case Token::t_MultiSymbol: {
         if (tk.value.chars.ch == L'*') {
            return uro->patternParser.parse(L"**", -1, result);
         }
         else {
            return false;
         }
      }
      case Token::t_Symbol: {
         if (tk.value.ch == L'*') {
            return uro->patternParser.parse(L"*", -1, result);
         }
         else {
            return false;
         }
      }
      case Token::t_Pattern: {
         return uro->patternParser.parse(tk.getOriginString(uro), tk.value.pattern.id, result);
      }
      default: {
         return false;
      }
   }
};

}
