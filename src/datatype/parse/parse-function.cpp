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

#include "parse-function.h"
#include "../../exception.h"
#include "../../brackets.h"
#include "../function/func-bool.h"
#include "../function/func-number.h"
#include "../function/func-string.h"
#include "../function/func-time.h"
#include "../function/func-list.h"
#include "../function/func-aggr.h"
#include "../function/func-attr.h"
#include "../parse-gen.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-number.h"
#include "../generator/gen-string.h"
#include "../generator/gen-definition.h"
#include "../math.h"
#include <cwctype>


namespace perun2::func
{

static std::vector<Tokens> toFunctionArgs(const Tokens& tks)
{
   const _size start = tks.getStart() + 2;
   const _size length = tks.getLength() - 3;
   const Tokens tks2(tks.getList(), start, length, tks.getInfo());
   return tks2.splitBySymbol(CHAR_COMMA);
}

_bool boolFunction(_genptr<_bool>& result, const Tokens& tks, _p2& p2)
{
   const Token& word = tks.first();
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (word.isWord(STRINGS_FUNC_BOO_STR, p2))
   {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      return simpleBoolFunction(result, args[0], word, p2);
   }
   else if (word.isWord(STRING_EXISTINSIDE, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      checkFunctionAttribute(word, p2);

      FileContext* fctx = p2.contexts.getFileContext();
      LocationContext* lctx = p2.contexts.getLocationContext();
      fctx->attribute->setCoreCommandBase();

      _genptr<_list> list;
      if (!parse::parse(p2, args[0], list)) {
         functionArgException(1, STRING_LIST, word, p2);
      }

      if (fctx->isInside) {
         result = std::make_unique<F_Exist>(list, p2);
      }
      else {
         result = std::make_unique<F_ExistInside>(list, lctx, fctx);
      }
      return true;
   }
   else if (word.isWord(STRING_ANYINSIDE, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      checkFunctionAttribute(word, p2);

      FileContext* fctx = p2.contexts.getFileContext();
      fctx->attribute->setCoreCommandBase();

      if (fctx->isInside) {
         _defptr def;
         if (!parse::parse(p2, args[0], def)) {
            functionArgException(0, STRING_DEFINITION, word, p2);
         }

         result = std::make_unique<F_Any>(def);
         return true;
      }

      _lcptr lctx;
      p2.contexts.makeLocationContext(lctx);
      p2.contexts.addLocationContext(lctx.get());

      _defptr def;
      if (!parse::parse(p2, args[0], def)) {
         functionArgException(1, STRING_DEFINITION, word, p2);
      }

      p2.contexts.retreatLocationContext();
      result = std::make_unique<F_AnyInside>(def, lctx, fctx);
      return true;
   }
   else if (word.isWord(STRING_ANY, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_tlist> tlist;
      if (parse::parse(p2, args[0], tlist)) {
         result = std::make_unique<F_AnyList<_tim>>(tlist);
         return true;
      }

      _genptr<_nlist> nlist;
      if (parse::parse(p2, args[0], nlist)) {
         result = std::make_unique<F_AnyList<_num>>(nlist);
         return true;
      }

      _defptr def;
      if (parse::parse(p2, args[0], def)) {
         result = std::make_unique<F_Any>(def);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, args[0], list)) {
         result = std::make_unique<F_AnyList<_str>>(list);
         return true;
      }

      throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
         L"' cannot be resolved to any collection"), word.line);
   }
   else if (word.isWord(STRING_EXIST, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_list> list;
      if (!parse::parse(p2, args[0], list)) {
         functionArgException(1, STRING_LIST, word, p2);
      }

      result = std::make_unique<F_Exist>(list, p2);
      return true;
   }
   else if (word.isWord(STRING_EXISTS, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str_;
      if (!parse::parse(p2, args[0], str_)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_Exists>(str_, p2);
      return true;
   }
   else if (word.isWord(STRING_CONTAINS, p2)) {
      if (len != 2) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str_;
      if (parse::parse(p2, args[0], str_)) {
         _genptr<_str> str2;
         if (parse::parse(p2, args[1], str2)) {
            result = std::make_unique<F_ContainsStr>(str_, str2);
            return true;
         }
         else {
            functionArgException(2, STRING_STRING, word, p2);
         }
      }

      checkInOperatorCommaAmbiguity(word, args[0], p2);

      _defptr def;
      if (parse::parse(p2, args[0], def)) {
         _genptr<_str> str2;
         if (parse::parse(p2, args[1], str2)) {
            result = std::make_unique<F_ContainsDef>(def, str2, p2);
            return true;
         }
         else {
            functionArgException(2, STRING_STRING, word, p2);
         }
      }

      _genptr<_tlist> tlist;
      if (parse::parse(p2, args[0], tlist)) {
         _genptr<_tim> tim2;
         if (parse::parse(p2, args[1], tim2)) {
            result = std::make_unique<F_ContainsCol<_tim>>(tlist, tim2);
            return true;
         }
         else {
            functionArgException(2, STRING_TIME, word, p2);
         }
      }

      _genptr<_nlist> nlist;
      if (parse::parse(p2, args[0], nlist)) {
         _genptr<_num> num2;
         if (parse::parse(p2, args[1], num2)) {
            result = std::make_unique<F_ContainsCol<_num>>(nlist, num2);
            return true;
         }
         else {
            functionArgException(2, STRING_NUMBER, word, p2);
         }
      }

      _genptr<_list> list;
      if (parse::parse(p2, args[0], list)) {
         _genptr<_str> str2;
         if (parse::parse(p2, args[1], str2)) {
            result = std::make_unique<F_ContainsCol<_str>>(list, str2);
            return true;
         }
         else {
            functionArgException(2, STRING_STRING, word, p2);
         }
      }
      else {
         throw SyntaxError(str(L"first argument of function '", word.getOriginString(p2),
            L"' cannot be resolved to a string nor any collection"), word.line);
      }
   }
   else if (word.isWord(STRING_EXISTSINSIDE, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      checkFunctionAttribute(word, p2);

      FileContext* fctx = p2.contexts.getFileContext();
      LocationContext* lctx = p2.contexts.getLocationContext();
      fctx->attribute->setCoreCommandBase();

      _genptr<_str> str_;
      if (!parse::parse(p2, args[0], str_)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      if (fctx->isInside) {
         result = std::make_unique<F_Exists>(str_, p2);
      }
      else {
         result = std::make_unique<F_ExistsInside>(str_, lctx, fctx);
      }
      return true;
   }
   else if (word.isWord(STRING_STARTSWITH, p2)) {
      if (len != 2) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[0], str)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      if (args[1].getLength() == 1) {
         const Token& f = args[1].first();
         switch (f.type) {
            case Token::t_Quotation: {
               const _str os = f.getOriginString(p2);

               switch (os.size()) {
                  case 0: {
                     result = std::make_unique<gen::Constant<_bool>>(true);
                     break;
                  }
                  case 1: {
                     const _char ch = os[0];
                     result = std::make_unique<F_StartsWithChar>(str, ch);
                     break;
                  }
                  default: {
                     result = std::make_unique<F_StartsWithConst>(str, os);
                     break;
                  }
               }

               return true;
            }
            case Token::t_Number: {
               const _str conv = f.value.num.n.toString();
               switch (conv.size()) {
                  case 1: {
                     const _char ch = conv[0];
                     result = std::make_unique<F_StartsWithChar>(str, ch);
                     break;
                  }
                  default: {
                     result = std::make_unique<F_StartsWithConst>(str, conv);
                     break;
                  }
               }

               return true;
            }
         }
      }

      _genptr<_str> str2;
      if (parse::parse(p2, args[1], str2)) {
         result = std::make_unique<F_StartsWith>(str, str2);
         return true;
      }
      else {
         functionArgException(2, STRING_STRING, word, p2);
      }
   }
   else if (word.isWord(STRING_ENDSWITH, p2)) {
      if (len != 2) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[0], str)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      if (args[1].getLength() == 1) {
         const Token& f = args[1].first();
         switch (f.type) {
            case Token::t_Quotation: {
               const _str os = f.getOriginString(p2);

               switch (os.size()) {
                  case 0: {
                     result = std::make_unique<gen::Constant<_bool>>(true);
                     break;
                  }
                  case 1: {
                     const _char ch = os[0];
                     result = std::make_unique<F_EndsWithChar>(str, ch);
                     break;
                  }
                  default: {
                     result = std::make_unique<F_EndsWithConst>(str, os);
                     break;
                  }
               }

               return true;
            }
            case Token::t_Number: {
               const _str conv = f.value.num.n.toString();
               switch (conv.size()) {
                  case 1: {
                     const _char ch = conv[0];
                     result = std::make_unique<F_EndsWithChar>(str, ch);
                     break;
                  }
                  default: {
                     result = std::make_unique<F_EndsWithConst>(str, conv);
                     break;
                  }
               }

               return true;
            }
         }
      }

      _genptr<_str> str2;
      if (!parse::parse(p2, args[1], str2)) {
         functionArgException(2, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_EndsWith>(str, str2);
      return true;
   }
   else if (word.isWord(STRING_FINDTEXT, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      checkFunctionAttribute(word, p2);

      FileContext* ctx = p2.contexts.getFileContext();
      ctx->attribute->setCoreCommandBase();

      _genptr<_str> str_;
      if (!parse::parse(p2, args[0], str_)) {
         functionArgException(0, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_FindText>(str_, ctx);
      return true;
   }

   return false;
};

_bool simpleBoolFunction(_genptr<_bool>& result, const Tokens& tks, const Token& word, _p2& p2)
{
   _genptr<_str> arg1;
   if (!parse::parse(p2, tks, arg1)) {
      functionArgException(1, STRING_STRING, word, p2);
   }

   if (word.isWord(STRING_ISLOWER, p2))
      result = std::make_unique<F_IsLower>(arg1);
   else if (word.isWord(STRING_ISUPPER, p2))
      result = std::make_unique<F_IsUpper>(arg1);
   else if (word.isWord(STRING_ISNUMBER, p2))
      result = std::make_unique<F_IsNumber>(arg1);
   else if (word.isWord(STRING_ISLETTER, p2))
      result = std::make_unique<F_IsLetter>(arg1);
   else if (word.isWord(STRING_ISDIGIT, p2))
      result = std::make_unique<F_IsDigit>(arg1);
   else if (word.isWord(STRING_ISBINARY, p2))
      result = std::make_unique<F_IsBinary>(arg1);
   else if (word.isWord(STRING_ISHEX, p2))
      result = std::make_unique<F_IsHex>(arg1);
   else
      return false;

   return true;
}

_bool numberFunction(_genptr<_num>& result, const Tokens& tks, _p2& p2)
{
   const Token& word = tks.first();
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (word.isWord(STRINGS_FUNC_NUM_NUM, p2)) {
      if (len != 1)
         functionArgNumberException(len, word, p2);

      return simpleNumberFunction(result, args[0], word, p2);
   }
   else if (word.isWord(STRING_LENGTH, p2)) {
      if (len != 1)
         functionArgNumberException(len, word, p2);

      _genptr<_str> arg1;
      if (parse::parse(p2, args[0], arg1)) {
         result = std::make_unique<F_Length>(arg1);
         return true;
      }
      else {
         throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
            L"' cannot be resolved to a string. "
            L"If you want to count elements in a collection, then use function 'count' instead"), word.line);
      }
   }
   else if (word.isWord(STRING_FROMBINARY, p2) || word.isWord(STRING_FROMHEX, p2)) {
      if (len != 1)
         functionArgNumberException(len, word, p2);

      _genptr<_str> arg1;

      if (parse::parse(p2, args[0], arg1)) {
         if (word.isWord(STRING_FROMBINARY, p2))
            result = std::make_unique<F_FromBinary>(arg1);
         else
            result = std::make_unique<F_FromHex>(arg1);

         return true;
      }
      else {
         functionArgException(1, STRING_STRING, word, p2);
      }
   }
   else if (word.isWord(STRING_SIZE, p2)) {
      if (len != 1) {
         checkInOperatorCommaAmbiguity(word, args[0], p2);
         functionArgNumberException(len, word, p2);
      }

      _defptr def;
      if (parse::parse(p2, args[0], def)) {
         result = std::make_unique<F_SizeDefinition>(def, p2);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, args[0], list)) {
         result = std::make_unique<F_SizeList>(list, p2);
         return true;
      }
      else {
         throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
            L"' cannot be resolved to a collection"), word.line);
      }
   }
   else if (word.isWord(STRING_NUMBER, p2)) {
      if (len != 1)
         functionArgNumberException(len, word, p2);

      _genptr<_str> arg1;
      if (parse::parse(p2, args[0], arg1)) {
         result = std::make_unique<F_Number>(arg1);
         return true;
      }
      else {
         throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
            L"' cannot be resolved to a string"), word.line);
      }
   }
   else if (word.isWord(STRING_COUNTINSIDE, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      checkFunctionAttribute(word, p2);

      FileContext* fctx = p2.contexts.getFileContext();
      fctx->attribute->setCoreCommandBase();

      if (fctx->isInside) {
         _defptr def;
         if (!parse::parse(p2, args[0], def)) {
            functionArgException(0, STRING_DEFINITION, word, p2);
         }

         result = std::make_unique<F_Count>(def, p2);
         return true;
      }

      _lcptr lctx;
      p2.contexts.makeLocationContext(lctx);
      p2.contexts.addLocationContext(lctx.get());

      _defptr def;
      if (!parse::parse(p2, args[0], def)) {
         functionArgException(0, STRING_DEFINITION, word, p2);
      }

      p2.contexts.retreatLocationContext();
      result = std::make_unique<F_CountInside>(def, lctx, fctx, p2);
      return true;
   }
   else if (word.isWord(STRING_COUNT, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_tlist> tlist;
      if (parse::parse(p2, args[0], tlist)) {
         result = std::make_unique<F_CountList<_tim>>(tlist);
         return true;
      }

      _genptr<_nlist> nlist;
      if (parse::parse(p2, args[0], nlist)) {
         result = std::make_unique<F_CountList<_num>>(nlist);
         return true;
      }

      _defptr def;
      if (parse::parse(p2, args[0], def)) {
         result = std::make_unique<F_Count>(def, p2);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, args[0], list)) {
         result = std::make_unique<F_CountList<_str>>(list);
         return true;
      }

      throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
         L"' cannot be resolved to any collection"), word.line);
   }
   else if (word.isWord(STRING_POWER, p2)) {
      if (len != 2)
         functionArgNumberException(len, word, p2);

      _genptr<_num> arg1;
      if (!parse::parse(p2, args[0], arg1)) {
         functionArgException(1, STRING_NUMBER, word, p2);
      }

      _genptr<_num> arg2;
      if (!parse::parse(p2, args[1], arg2)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      result = std::make_unique<F_Power>(arg1, arg2);
      return true;
   }
   else if (word.isWord(STRING_SHIFTMONTH, p2)) {
      if (len != 2)
         functionArgNumberException(len, word, p2);

      _genptr<_num> arg2;
      if (!parse::parse(p2, args[1], arg2)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      _genptr<_tim> tim;
      if (parse::parse(p2, args[0], tim)) {
         result = std::make_unique<F_ShiftMonth_Time>(tim, arg2);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(p2, args[0], num)) {
         result = std::make_unique<F_ShiftMonth_Number>(num, arg2);
         return true;
      }

      throw SyntaxError(str(L"first argument of function '", word.getOriginString(p2),
         L"' cannot be resolved to a time nor a number"), word.line);
   }
   else if (word.isWord(STRING_SHIFTWEEKDAY, p2)) {
      if (len != 2)
         functionArgNumberException(len, word, p2);

      _genptr<_num> arg2;
      if (!parse::parse(p2, args[1], arg2)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      _genptr<_tim> tim;
      if (parse::parse(p2, args[0], tim)) {
         result = std::make_unique<F_ShiftWeekDay_Time>(tim, arg2);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(p2, args[0], num)) {
         result = std::make_unique<F_ShiftWeekDay_Number>(num, arg2);
         return true;
      }

      throw SyntaxError(str(L"first argument of function '", word.getOriginString(p2),
         L"' cannot be resolved to a time nor a number"), word.line);
   }
   else if (word.isWord(STRINGS_AGGRFUNC, p2)) {
      if (len == 0) {
         throw SyntaxError(str(L"aggregate function '", word.getOriginString(p2),
            L"' needs at least one argument"), word.line);
      }

      return aggrFunction(result, args, word, p2);
   }
   else if (word.isWord(STRING_FIRST, p2) || word.isWord(STRING_LAST, p2)) {
      if (len == 0) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str_;
      if (parse::parse(p2, args[0], str_)) {
         throw SyntaxError(str(L"function '", word.getOriginString(p2),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _genptr<_nlist> nlist;
      if (parse::parse(p2, args[0], nlist)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], p2);
            functionArgNumberException(len, word, p2);
         }

         if (word.isWord(STRING_FIRST, p2)) {
            result = std::make_unique<F_First<_num>>(nlist);
         }
         else {
            result = std::make_unique<F_Last<_num>>(nlist);
         }

         return true;
      }
   }
   else if (word.isWord(STRING_RANDOM, p2)) {
      if (len > 1) {
         functionArgNumberException(len, word, p2);
      }

      if (len == 0) {
         result = std::make_unique<F_Random>(p2);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(p2, args[0], num)) {
         p2.math.markAnyRandomDouble();
         result = std::make_unique<F_RandomNumber>(num, p2);
         return true;
      }

      _genptr<_nlist> nlist;
      if (parse::parse(p2, args[0], nlist)) {
         p2.math.markAnyRandomDouble();
         result = std::make_unique<F_RandomElement<_num>>(nlist, p2);
         return true;
      }
   }

   return false;
}

static _bool simpleNumberFunction(_genptr<_num>& result, const Tokens& tks, const Token& word, _p2& p2)
{
   _genptr<_num> arg;
   if (!parse::parse(p2, tks, arg)) {
      functionArgException(1, STRING_NUMBER, word, p2);
   }

   if (word.isWord(STRING_ABSOLUTE, p2))
      result = std::make_unique<F_Absolute>(arg);
   else if (word.isWord(STRING_CEIL, p2))
      result = std::make_unique<F_Ceil>(arg);
   else if (word.isWord(STRING_FLOOR, p2))
      result = std::make_unique<F_Floor>(arg);
   else if (word.isWord(STRING_ROUND, p2))
      result = std::make_unique<F_Round>(arg);
   else if (word.isWord(STRING_SIGN, p2))
      result = std::make_unique<F_Sign>(arg);
   else if (word.isWord(STRING_SQRT, p2))
      result = std::make_unique<F_Sqrt>(arg);
   else if (word.isWord(STRING_TRUNCATE, p2))
      result = std::make_unique<F_Truncate>(arg);
   else
      return false;

   return true;
}

static _bool aggrFunction(_genptr<_num>& result, const std::vector<Tokens>& args, const Token& word, _p2& p2)
{
   std::vector<_genptr<_num>> singles;
   std::vector<_genptr<_nlist>> multis;

   const _size len = args.size();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = args[i];

      _genptr<_num> num;
      if (parse::parse(p2, tks, num)) {
         singles.push_back(std::move(num));
      }
      else {
         _genptr<_nlist> nlist;
         if (parse::parse(p2, tks, nlist)) {
            multis.push_back(std::move(nlist));
         }
         else {
            throw SyntaxError(str(ordinalNumber(i + 1),
               L" argument of aggregate function '", word.getOriginString(p2),
               L"' cannot be resolved to a number nor a numeric list"), word.line);
         }
      }
   }

   if (word.isWord(STRING_AVERAGE, p2))
      result = std::make_unique<F_Average>(singles, multis);
   else if (word.isWord(STRING_MAX, p2))
      result = std::make_unique<F_Max>(singles, multis);
   else if (word.isWord(STRING_MEDIAN, p2))
      result = std::make_unique<F_Median>(singles, multis);
   else if (word.isWord(STRING_MIN, p2))
      result = std::make_unique<F_Min>(singles, multis);
   else if (word.isWord(STRING_SUM, p2))
      result = std::make_unique<F_Sum>(singles, multis);
   else
      return false;

   return true;
}

_bool periodFunction(_genptr<_per>& result, const Tokens& tks, _p2& p2)
{
   return false;
}

_bool stringFunction(_genptr<_str>& result, const Tokens& tks, _p2& p2)
{
   const Token& word = tks.first();
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (word.isWord(STRING_AFTER, p2) || word.isWord(STRING_BEFORE, p2)) {
      if (len != 2)
         functionArgNumberException(len, word, p2);

      return stringTwoArgFunction(result, args, word, p2);
   }
   else if (word.isWord(STRINGS_FUNC_STR_STR, p2)) {
      if (len != 1)
         functionArgNumberException(len, word, p2);

      return simpleStringFunction(result, args[0], word, p2);
   }
   else if (word.isWord(STRING_REVERSED, p2)) {
      const _str sub = (word.getOriginString(p2)).substr(0, 7);
      throw SyntaxError(str(L"the proper name for this function is '", sub, L"'"), word.line);
   }
   else if (word.isWord(STRINGS_FUNC_STR_STR_NUM, p2)) {
      if (len != 2) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[0], str)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      _genptr<_num> num;
      if (!parse::parse(p2, args[1], num)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      if (word.isWord(STRING_REPEAT, p2))
         result = std::make_unique<F_Repeat>(str, num);
      else if (word.isWord(STRING_LEFT, p2))
         result = std::make_unique<F_Left>(str, num);
      else if (word.isWord(STRING_RIGHT, p2))
         result = std::make_unique<F_Right>(str, num);
      else if (word.isWord(STRING_FILL, p2))
         result = std::make_unique<F_Fill>(str, num);

      return true;
   }
   else if (word.isWord(STRING_REPLACE, p2)) {
      if (len != 3) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str1;
      if (!parse::parse(p2, args[0], str1)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      _genptr<_str> str2;
      if (!parse::parse(p2, args[1], str2)) {
         functionArgException(2, STRING_STRING, word, p2);
      }

      _genptr<_str> str3;
      if (!parse::parse(p2, args[2], str3)) {
         functionArgException(3, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_Replace>(str1, str2, str3);
      return true;
   }
   else if (word.isWord(STRING_SUBSTRING, p2)) {
      if (len < 2 || len > 3) {
         throw SyntaxError(str(L"function '", word.getOriginString(p2), L"' can only take"
            L" two or three arguments"), word.line);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[0], str)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      _genptr<_num> num;
      if (!parse::parse(p2, args[1], num)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      if (len == 2) {
         result = std::make_unique<F_Substring_2>(str, num);
         return true;
      }

      _genptr<_num> num2;
      if (!parse::parse(p2, args[2], num2)) {
         functionArgException(3, STRING_NUMBER, word, p2);
      }

      result = std::make_unique<F_Substring_3>(str, num, num2);
      return true;
   }
   else if (word.isWord(STRING_CONCATENATE, p2)) {
      if (len < 1) {
         throw SyntaxError(str(L"function '", word.getOriginString(p2),
            L"' needs at least one arguments"), word.line);
      }

      std::vector<_genptr<_str>> values;

      for (_size i = 0; i < len; i++) {
         _genptr<_str> str_;
         if (parse::parse(p2, args[i], str_)) {
            values.push_back(std::move(str_));
            continue;
         }

         _genptr<_list> list;
         if (parse::parse(p2, args[i], list)) {
            if (i != len - 1) {
               checkInOperatorCommaAmbiguity(word, args[i], p2);
            }
            values.push_back(std::make_unique<F_ConcatenateUnit>(list));
         }
         else {
            throw SyntaxError(str(ordinalNumber(i + 1), L" argument of the function '",
               word.getOriginString(p2), L"' cannot be resolved to any data type"), word.line);
         }
      }

      result = std::make_unique<F_Concatenate>(values);
      return true;
   }
   else if (word.isWord(STRING_FIRST, p2) || word.isWord(STRING_LAST, p2)) {
      if (len == 0) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str_;
      if (parse::parse(p2, args[0], str_)) {
         throw SyntaxError(str(L"function '", word.getOriginString(p2),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _defptr def;
      if (parse::parse(p2, args[0], def)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], p2);
            functionArgNumberException(len, word, p2);
         }

         if (word.isWord(STRING_FIRST, p2)) {
            result = std::make_unique<F_FirstDef>(def);
         }
         else {
            result = std::make_unique<F_LastDef>(def);
         }
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, args[0], list)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], p2);
            functionArgNumberException(len, word, p2);
         }

         if (word.isWord(STRING_FIRST, p2)) {
            result = std::make_unique<F_First<_str>>(list);
         }
         else {
            result = std::make_unique<F_Last<_str>>(list);
         }
         return true;
      }
      else {
         throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
            L"' cannot be resolved to any collection"), word.line);
      }
   }
   else if (word.isWord(STRING_PATH, p2)) {
      if (len == 0) {
         functionArgNumberException(len, word, p2);
      }
      if (len == 1) {
         _genptr<_str> arg1;
         if (!parse::parse(p2, args[0], arg1)) {
            functionArgException(1, STRING_STRING, word, p2);
         }
         result = std::make_unique<F_Path_1>(arg1, p2);
         return true;
      }

      if (len > 4) {
         std::vector<_genptr<_str>> values;

         for (_size i = 0; i < len; i++) {
            _genptr<_str> str;
            if (!parse::parse(p2, args[i], str)) {
               functionArgException(i + 1, STRING_STRING, word, p2);
            }

            values.push_back(std::move(str));
         }

         result = std::make_unique<F_Path_Multi>(values);
         return true;
      }
      else {
         _genptr<_str> str1;
         if (!parse::parse(p2, args[0], str1)) {
            functionArgException(1, STRING_STRING, word, p2);
         }

         _genptr<_str> str2;
         if (!parse::parse(p2, args[1], str2)) {
            functionArgException(2, STRING_STRING, word, p2);
         }

         if (len == 2) {
            result = std::make_unique<F_Path_2>(str1, str2);
            return true;
         }

         _genptr<_str> str3;
         if (!parse::parse(p2, args[2], str3)) {
            functionArgException(3, STRING_STRING, word, p2);
         }

         if (len == 3) {
            result = std::make_unique<F_Path_3>(str1, str2, str3);
            return true;
         }

         _genptr<_str> str4;
         if (!parse::parse(p2, args[3], str4)) {
            functionArgException(4, STRING_STRING, word, p2);
         }

         result = std::make_unique<F_Path_4>(str1, str2, str3, str4);
         return true;
      }
   }
   else if (word.isWord(STRING_STRING, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_bool> boo;
      if (parse::parse(p2, args[0], boo)) {
         result = std::make_unique<F_String_B>(boo);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(p2, args[0], num)) {
         result = std::make_unique<F_String_N>(num);
         return true;
      }

      _genptr<_tim> tim;
      if (parse::parse(p2, args[0], tim)) {
         result = std::make_unique<F_String_T>(tim);
         return true;
      }

      _genptr<_per> per;
      if (parse::parse(p2, args[0], per)) {
         result = std::make_unique<F_String_P>(per);
         return true;
      }

      throw SyntaxError(str(L"the argument of function '", word.getOriginString(p2),
        L"' cannot be resolved to any singular data type. If you want to concatenate a collection, use function 'concatenate' instead"), word.line);
   }
   else if (word.isWord(STRINGS_FUNC_STR_NUM, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_num> num;

      if (parse::parse(p2, args[0], num)) {
         if (word.isWord(STRING_ROMAN, p2))
            result = std::make_unique<F_Roman>(num);
         else if (word.isWord(STRING_BINARY, p2))
            result = std::make_unique<F_Binary>(num);
         else if (word.isWord(STRING_HEX, p2))
            result = std::make_unique<F_Hex>(num);

         return true;
      }
      else {
         functionArgException(1, STRING_NUMBER, word, p2);
      }
   }
   else if (word.isWord(STRING_MONTHNAME, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_tim> tim;
      if (parse::parse(p2, args[0], tim)) {
         result = std::make_unique<F_MonthNameFromTime>(tim);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(p2, args[0], num)) {
         result = std::make_unique<F_MonthName>(num);
         return true;
      }
      else {
         functionArgException(1, STRING_NUMBER, word, p2);
      }
   }
   else if (word.isWord(STRING_WEEKDAYNAME, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_tim> tim;
      if (parse::parse(p2, args[0], tim)) {
         result = std::make_unique<F_WeekDayNameFromTime>(tim);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(p2, args[0], num)) {
         result = std::make_unique<F_WeekDayName>(num);
         return true;
      }
      else {
         functionArgException(1, STRING_NUMBER, word, p2);
      }
   }
   else if (word.isWord(STRING_RANDOM, p2)) {
      if (len == 0) {
         return false;
      }

      _genptr<_str> str_;
      if (parse::parse(p2, args[0], str_)) {
         if (len > 1) {
            functionArgNumberException(len, word, p2);
         }
         result = std::make_unique<F_RandomChar>(str_, p2);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(p2, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], p2);
            functionArgNumberException(len, word, p2);
         }

         result = std::make_unique<F_RandomElement<_str>>(list, p2);
         return true;
      }
      else {
         throw SyntaxError(str(L"wrong arguments of function '", word.getOriginString(p2), L"'"),
            word.line);
      }
   }
   else if (word.isWord(STRING_JOIN, p2)) {
      if (len == 0) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_list> list;
      if (!parse::parse(p2, args[0], list)) {
         functionArgException(1, STRING_LIST, word, p2);
      }

      if (len == 1) {
         throw SyntaxError(str(L"function '", word.getOriginString(p2),
            L"' cannot be called with one argument. If you want to join multiple strings without a separator, use function 'concatenate' instead"), word.line);
      }

      checkInOperatorCommaAmbiguity(word, args[0], p2);

      if (len != 2) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[1], str)) {
         functionArgException(2, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_Join>(list, str);
      return true;
   }

   return false;
}

static _bool stringTwoArgFunction(_genptr<_str>& result, const std::vector<Tokens>& args, const Token& word, _p2& p2)
{
   _genptr<_str> arg1;
   if (!parse::parse(p2, args[0], arg1)) {
      functionArgException(1, STRING_STRING, word, p2);
   }

   _genptr<_str> arg2;
   if (!parse::parse(p2, args[1], arg2)) {
      functionArgException(2, STRING_STRING, word, p2);
   }

   if (word.isWord(STRING_AFTER, p2))
      result = std::make_unique<F_After>(arg1, arg2);
   else if (word.isWord(STRING_BEFORE, p2))
      result = std::make_unique<F_Before>(arg1, arg2);
   else
      return false;

   return true;
}

static _bool simpleStringFunction(_genptr<_str>& result, const Tokens& tks, const Token& word, _p2& p2)
{
   _genptr<_str> arg1;
   if (!parse::parse(p2, tks, arg1)) {
      functionArgException(1, STRING_STRING, word, p2);
   }

   if (word.isWord(STRING_DIGITS, p2))
      result = std::make_unique<F_Digits>(arg1);
   else if (word.isWord(STRING_LETTERS, p2))
      result = std::make_unique<F_Letters>(arg1);
   else if (word.isWord(STRING_LOWER, p2))
      result = std::make_unique<F_Lower>(arg1);
   else if (word.isWord(STRING_TRIM, p2))
      result = std::make_unique<F_Trim>(arg1);
   else if (word.isWord(STRING_UPPER, p2))
      result = std::make_unique<F_Upper>(arg1);
   else if (word.isWord(STRING_REVERSE, p2))
      result = std::make_unique<F_Reverse>(arg1);
   else if (word.isWord(STRING_AFTERDIGITS, p2))
      result = std::make_unique<F_AfterDigits>(arg1);
   else if (word.isWord(STRING_AFTERLETTERS, p2))
      result = std::make_unique<F_AfterLetters>(arg1);
   else if (word.isWord(STRING_BEFOREDIGITS, p2))
      result = std::make_unique<F_BeforeDigits>(arg1);
   else if (word.isWord(STRING_BEFORELETTERS, p2))
      result = std::make_unique<F_BeforeLetters>(arg1);
   else if (word.isWord(STRING_CAPITALIZE, p2))
      result = std::make_unique<F_Capitalize>(arg1);
   else if (word.isWord(STRING_PARENT, p2))
      result = std::make_unique<F_Parent>(arg1, p2);
   else
      return false;

   return true;
}

_bool timeFunction(_genptr<_tim>& result, const Tokens& tks, _p2& p2)
{
   const Token& word = tks.first();
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (word.isWord(STRINGS_FUNC_TIM_NUM, p2)) {
      if (len != 1)
         functionArgNumberException(len, word, p2);

      return simpleTimeFunction(result, args[0], word, p2);
   }

   else if (word.isWord(STRING_DATE, p2)) {
      if (len != 3)
         functionArgNumberException(len, word, p2);

      _genptr<_num> arg1;
      if (!parse::parse(p2, args[0], arg1)) {
         functionArgException(1, STRING_NUMBER, word, p2);
      }

      _genptr<_num> arg2;
      if (!parse::parse(p2, args[1], arg2)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      _genptr<_num> arg3;
      if (!parse::parse(p2, args[2], arg3)) {
         functionArgException(3, STRING_NUMBER, word, p2);
      }

      result = std::make_unique<F_Time_3>(arg1, arg2, arg3);
      return true;
   }
   else if (word.isWord(STRING_TIME, p2)) {
      if (len < 2 || len == 4 || len > 6) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_num> arg1;
      if (!parse::parse(p2, args[0], arg1)) {
         functionArgException(1, STRING_NUMBER, word, p2);
      }

      _genptr<_num> arg2;
      if (!parse::parse(p2, args[1], arg2)) {
         functionArgException(2, STRING_NUMBER, word, p2);
      }

      if (len == 2) {
         result = std::make_unique<F_Time_2>(arg1, arg2);
         return true;
      }

      _genptr<_num> arg3;
      if (!parse::parse(p2, args[2], arg3)) {
         functionArgException(3, STRING_NUMBER, word, p2);
      }

      if (len == 3) {
         result = std::make_unique<F_Time_3>(arg1, arg2, arg3);
         return true;
      }

      _genptr<_num> arg4;
      if (!parse::parse(p2, args[3], arg4)) {
         functionArgException(4, STRING_NUMBER, word, p2);
      }

      _genptr<_num> arg5;
      if (!parse::parse(p2, args[4], arg5)) {
         functionArgException(5, STRING_NUMBER, word, p2);
      }

      if (len == 5) {
         result = std::make_unique<F_Time_5>(arg1, arg2, arg3, arg4, arg5);
         return true;
      }

      _genptr<_num> arg6;
      if (!parse::parse(p2, args[5], arg6)) {
         functionArgException(6, STRING_NUMBER, word, p2);
      }

      result = std::make_unique<F_Time_6>(arg1, arg2, arg3, arg4, arg5, arg6);
      return true;
   }
   else if (word.isWord(STRING_FIRST, p2) || word.isWord(STRING_LAST, p2)) {
      if (len == 0) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str_;
      if (parse::parse(p2, args[0], str_)) {
         throw SyntaxError(str(L"function '", word.getOriginString(p2),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _genptr<_tlist> tlist;
      if (parse::parse(p2, args[0], tlist)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], p2);
            functionArgNumberException(len, word, p2);
         }

         if (word.isWord(STRING_FIRST, p2)) {
            result = std::make_unique<F_First<_tim>>(tlist);
         }
         else {
            result = std::make_unique<F_Last<_tim>>(tlist);
         }

         return true;
      }
   }
   else if (word.isWord(STRING_RANDOM, p2)) {
      if (len > 1) {
         functionArgNumberException(len, word, p2);
      }

      if (len == 0) {
         return false;
      }

      _genptr<_tlist> tlist;
      if (parse::parse(p2, args[0], tlist)) {
         result = std::make_unique<F_RandomElement<_tim>>(tlist, p2);
         return true;
      }
   }

   return false;
}

static _bool simpleTimeFunction(_genptr<_tim>& result, const Tokens& tks, const Token& word, _p2& p2)
{
   _genptr<_num> arg1;
   if (!parse::parse(p2, tks, arg1)) {
      functionArgException(1, STRING_NUMBER, word, p2);
   }

   if (word.isWord(STRING_CHRISTMAS, p2))
      result = std::make_unique<F_Christmas>(arg1);
   else if (word.isWord(STRING_EASTER, p2))
      result = std::make_unique<F_Easter>(arg1);
   else if (word.isWord(STRING_NEWYEAR, p2))
      result = std::make_unique<F_NewYear>(arg1);
   else
      return false;

   return true;
}


static void functionArgNumberException(const _int argNumber, const Token& word, _p2& p2)
{
   throw SyntaxError(str(L"function '", word.getOriginString(p2), L"' cannot be called with ",
      toStr(argNumber), L" argument", (argNumber == 1 ? _str() : L"s")), word.line);
}

static void functionArgException(const _int argNumber, const _str& typeName, const Token& word, _p2& p2)
{
   throw SyntaxError(str(ordinalNumber(argNumber), L" argument of function '",
      word.getOriginString(p2), L"' cannot be resolved to a ", typeName), word.line);
}

static _str ordinalNumber(const _int number)
{
   switch (number) {
      case 1:
         return L"first";
      case 2:
         return L"second";
      case 3:
         return L"third";
      case 4:
         return L"fourth";
      case 5:
         return L"fifth";
      default: {
         if (number >= 11 && number <= 19) {
            return str(toStr(number), L"th");
         }

         switch (number % 10) {
            case 1:
               return str(toStr(number), L"st");
            case 2:
               return str(toStr(number), L"nd");
            case 3:
               return str(toStr(number), L"rd");
            default:
               return str(toStr(number), L"th");
         }
      }
   }
}

_bool listFunction(_genptr<_list>& result, const Tokens& tks, _p2& p2)
{
   const Token& word = tks.first();
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (word.isWord(STRING_CHARACTERS, p2) || word.isWord(STRING_WORDS, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[0], str)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      if (word.isWord(STRING_CHARACTERS, p2))
         result = std::make_unique<F_Characters>(str);
      else
         result = std::make_unique<F_Words>(str);

      return true;
   }
   if (word.isWord(STRING_SPLIT, p2)) {
      if (len != 2) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str1;
      if (!parse::parse(p2, args[0], str1)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      _genptr<_str> str2;
      if (!parse::parse(p2, args[1], str2)) {
         functionArgException(2, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_Split>(str1, str2);
      return true;
   }

   throw SyntaxError(str(L"function with name '", word.getOriginString(p2),
      L"' does not exist"), word.line);
}


_bool numListFunction(_genptr<_nlist>& result, const Tokens& tks, _p2& p2)
{
   const Token& word = tks.first();
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (word.isWord(STRING_NUMBERS, p2)) {
      if (len != 1) {
         functionArgNumberException(len, word, p2);
      }

      _genptr<_str> str;
      if (!parse::parse(p2, args[0], str)) {
         functionArgException(1, STRING_STRING, word, p2);
      }

      result = std::make_unique<F_Numbers>(str);
      return true;
   }

   return false;
}


static void checkFunctionAttribute(const Token& word, _p2& p2)
{
   if (!p2.contexts.hasFileContext()) {
      throw SyntaxError(str(L"function '", word.getOriginString(p2),
         L"' can be called only within a loop iterating over strings"), word.line);
   }
}

static void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, _p2& p2)
{
   BracketsInfo bi;
   _int end = tks.getEnd();
   _int start = tks.getStart();
   _int index = -1;

   for (_int i = end; i >= start; i--)
   {
      const Token& t = tks.listAt(i);

      if (t.isKeyword(Keyword::kw_In)) {
         if (bi.isBracketFree()) {
            index = i;
            break;
         }
      }
      else {
         bi.refresh(t);
      }
   }

   if (index == -1) {
      return;
   }

   const Tokens right(tks, index + 1, tks.getLength() + start - index - 1);

   if (right.first().isSymbol(CHAR_OPENING_ROUND_BRACKET)) {
      return;
   }

   const Token& inToken = tks.listAt(index);
   throw SyntaxError(str(L"the right side of the operator '", inToken.getOriginString(p2),
      L"' used in function '", word.getOriginString(p2),
      L"' should be embraced by brackets. Comma is a function argument separator and causes ambiguity here"),
      word.line);
}

}
