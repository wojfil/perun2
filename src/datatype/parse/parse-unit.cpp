#include "parse-unit.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-number.h"
#include "../generator/gen-string.h"
#include "../generator/gen-time.h"
#include "../generator/gen-os.h"
#include "parse-number.h"


_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_boo>*& result)
{
   switch (tk.type) {
      case Token::t_Keyword: {
         switch (tk.value.keyword.k) {
            case Keyword::kw_True: {
               result = new Constant<_boo>(true);
               return true;
            }
            case Keyword::kw_False: {
               result = new Constant<_boo>(false);
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

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_num>*& result)
{
   switch (tk.type) {
      case Token::t_Number: {
         result = new Constant<_num>(tk.value.num.n);
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
            throw SyntaxException(str(L"time variable from expression '", *tk.value.twoWords.os1,
               L".", *tk.value.twoWords.os2, L"' does not exist or is unreachable here"), tk.line);
         }

         const _size& h = tk.value.twoWords.h2;

         if (h == hs.HASH_PER_YEAR || h == hs.HASH_PER_YEARS)
            result = new TimeYears(var);
         else if (h == hs.HASH_PER_MONTH || h == hs.HASH_PER_MONTHS)
            result = new TimeMonths(var);
         else if (h == hs.HASH_PER_WEEKDAY)
            result = new TimeWeekDay(var);
         else if (h == hs.HASH_PER_DAY || h == hs.HASH_PER_DAYS)
            result = new TimeDays(var);
         else if (h == hs.HASH_PER_HOUR || h == hs.HASH_PER_HOURS)
            result = new TimeHours(var);
         else if (h == hs.HASH_PER_MINUTE || h == hs.HASH_PER_MINUTES)
            result = new TimeMinutes(var);
         else if (h == hs.HASH_PER_SECOND || h == hs.HASH_PER_SECOND)
            result = new TimeSeconds(var);
         else if (h == hs.HASH_PER_DATE)
            return false;
         else {
            timeVariableMemberException(tk);
         }

         return true;
      }
      default: {
         return false;
      }
   }
};

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_str>*& result)
{
   switch (tk.type) {
      case Token::t_Number: {
         result = new Constant<_str>(tk.value.num.n.toString());
         return true;
      }
      case Token::t_Quotation: {
         result = new Constant<_str>(*tk.value.str);
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

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_nlist>*& result)
{
   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_tlist>*& result)
{
   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_list>*& result)
{
   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_tim>*& result)
{
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
            throw SyntaxException(str(L"time variable '", *tk.value.twoWords.os1,
               L"' does not exist"), tk.line);
         }

         if (tk.value.twoWords.h2 == uro->hashes.HASH_FUNC_DATE) {
            result = new TimeDate(var);
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

_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_per>*& result)
{
   return tk.type == Token::t_Word
      && uro->vars.getVarValue(tk, result);
};

_boo parseOneToken(Uroboros* uro, const Token& tk, _def*& result)
{
   switch (tk.type) {
      case Token::t_Word: {
         return uro->vars.getVarValue(tk, result);
      }
      /*case Token::t_Symbol: {
         if (tk.value.ch == L'*') {
            result = new ElementsAtLocation(new LocationReference(uro), ELEM_ALL, uro);
            return true;
         }
         else {
            return false;
         }
      }*/
      default: {
         return false;
      }
   }
};
