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

#include "../../../include/perun2/datatype/parse/parse-unit.hpp"
#include "../../../include/perun2/datatype/generator/gen-generic.hpp"
#include "../../../include/perun2/datatype/generator/gen-number.hpp"
#include "../../../include/perun2/datatype/generator/gen-string.hpp"
#include "../../../include/perun2/datatype/generator/gen-time.hpp"
#include "../../../include/perun2/datatype/generator/gen-os.hpp"
#include "../../../include/perun2/os/os.hpp"
#include "../../../include/perun2/datatype/parse/parse-number.hpp"
#include "../../../include/perun2/datatype/parse/parse-asterisk.hpp"
#include "../../../include/perun2/datatype/parse/parse-var.hpp"


namespace perun2::parse
{

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_bool>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Keyword: {
         switch (tk.value.keyword) {
            case Keyword::kw_True: {
               result = std::make_unique<gen::Constant<p_bool>>(true);
               return true;
            }
            case Keyword::kw_False: {
               result = std::make_unique<gen::Constant<p_bool>>(false);
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

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_num>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = std::make_unique<gen::Constant<p_num>>(tk.value.number.value);
         return true;
      }
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      case Token::t_TwoWords: {
         if (tk.isFirstWord(EMPTY_STRING)) {
            throw SyntaxError(L"the dot . should be preceded by a time variable", tk.line);
         }

         p_genptr<p_tim> var;
         if (! makeVarRef(tk, var, p2)) {
            throw SyntaxError(str(L"the time variable from expression \"", tk.origin,
               L".", tk.origin2, L"\" does not exist or is unreachable here"), tk.line);
         }

         if (tk.isSecondWord(STRING_YEAR) || tk.isSecondWord(STRING_YEARS)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Years);
         }
         else if (tk.isSecondWord(STRING_MONTH) || tk.isSecondWord(STRING_MONTHS)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Months);
         }
         else if (tk.isSecondWord(STRING_WEEKDAY)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Weeks);
         }
         else if (tk.isSecondWord(STRING_DAY) || tk.isSecondWord(STRING_DAYS)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Days);
         }
         else if (tk.isSecondWord(STRING_HOUR) || tk.isSecondWord(STRING_HOURS)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Hours);
         }
         else if (tk.isSecondWord(STRING_MINUTE) || tk.isSecondWord(STRING_MINUTES)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Minutes);
         }
         else if (tk.isSecondWord(STRING_SECOND) || tk.isSecondWord(STRING_SECOND)) {
            result = std::make_unique<gen::TimeMember>(var, Period::u_Seconds);
         }
         else if (tk.isSecondWord(STRING_DATE)) {
            return false;
         }
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

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_str>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Number: {
         result = std::make_unique<gen::Constant<p_str>>(tk.value.number.value.toString());
         return true;
      }
      case Token::t_Quotation: {
         result = std::make_unique<gen::Constant<p_str>>(tk.origin);
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

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_nlist>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_tlist>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_list>& result)
{
   const Token& tk = tks.first();
   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_tim>& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      case Token::t_TwoWords: {
         if (tk.isFirstWord(EMPTY_STRING)) {
            throw SyntaxError(L"the dot . should be preceded by a time variable", tk.line);
         }

         p_genptr<p_tim> var;
         if (!makeVarRef(tk, var, p2)) {
            throw SyntaxError(str(L"the time variable \"", tk.origin, L"\" does not exist"), tk.line);
         }

         if (tk.isSecondWord(STRING_DATE)) {
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

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_genptr<p_per>& result)
{
   const Token& tk = tks.first();

   return tk.type == Token::t_Word && makeVarRef(tk, result, p2);
};

p_bool parseOneToken(Perun2Process& p2, const Tokens& tks, p_defptr& result)
{
   const Token& tk = tks.first();

   switch (tk.type) {
      case Token::t_Word: {
         return makeVarRef(tk, result, p2);
      }
      case Token::t_MultiSymbol: {
         if (tk.value.repeatedChars.value == CHAR_ASTERISK) {
            return parseAsteriskPattern(result, str(CHAR_ASTERISK, CHAR_ASTERISK), tk.line, p2);
         }
         else {
            return false;
         }
      }
      case Token::t_Symbol: {
         if (tk.value.singleChar == CHAR_ASTERISK) {
            return parseAsteriskPattern(result, toStr(CHAR_ASTERISK), tk.line, p2);
         }
         else {
            return false;
         }
      }
      case Token::t_Pattern: {
         return parseAsteriskPattern(result, tk.origin, tk.line, p2);
      }
      default: {
         return false;
      }
   }
};

}
