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


namespace uro::func
{

static std::vector<Tokens> toFunctionArgs(const Tokens& tks)
{
   const _size start = tks.getStart() + 2;
   const _size length = tks.getLength() - 3;
   const Tokens tks2(tks.getList(), start, length, tks.getInfo());
   return tks2.splitBySymbol(L',');
}

_bool boolFunction(_genptr<_bool>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (uro.hashes.HASH_GROUP_FUNC_BOO_STR.find(name) != uro.hashes.HASH_GROUP_FUNC_BOO_STR.end())
   {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      return simpleBoolFunction(result, args[0], word, uro);
   }
   else if (name == uro.hashes.HASH_FUNC_EXIST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         //result = std::make_unique<F_AnyDef>(F_AnyDef(def));
         result = std::make_unique<F_AnyDef>(def);
         return true;
      }
      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         result = std::make_unique<F_Exist>(list, uro);
         return true;
      }
      else {
         functionArgException(1, L"list", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_ANY) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a collection"), word.line);
      }

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_AnyDef>(def);
         return true;
      }

      _genptr<_tlist> tlist;
      if (parse::parse(uro, args[0], tlist)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_Any<_tim>>(tlist);
         return true;
      }

      _genptr<_nlist> nlist;
      if (parse::parse(uro, args[0], nlist)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_Any<_num>>(nlist);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         result = std::make_unique<F_Any<_str>>(list);
         return true;
      }
      else {
         functionArgException(1, L"list", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_ANYINSIDE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str1;
      if (parse::parse(uro, args[0], str1)) {
         throw SyntaxException(str(L"first argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a collection"), word.line);
      }

      _defptr def;
      if (!parse::parse(uro, args[0], def)) {
         functionArgException(1, L"definition", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         _genptr<_str> ts;
         uro.vars.inner.createThisRef(ts);
         result = std::make_unique<F_AnyInside>(def, ts, uro);
         return true;
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);

      if (len > 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (parse::parse(uro, args[1], str)) {
         result = std::make_unique<F_AnyInside>(def, str, uro);
         return true;
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_CONTAINS) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         _genptr<_str> str2;
         if (parse::parse(uro, args[1], str2)) {
            result = std::make_unique<F_ContainsStr>(str_, str2);
            return true;
         }
         else {
            functionArgException(2, L"string", word, uro);
         }
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         _genptr<_str> str2;
         if (parse::parse(uro, args[1], str2)) {
            result = std::make_unique<F_ContainsDef>(def, str2, uro);
            return true;
         }
         else {
            functionArgException(2, L"string", word, uro);
         }
      }

      _genptr<_tlist> tlist;
      if (parse::parse(uro, args[0], tlist)) {
         _genptr<_tim> tim2;
         if (parse::parse(uro, args[1], tim2)) {
            result = std::make_unique<F_ContainsCol<_tim>>(tlist, tim2);
            return true;
         }
         else {
            functionArgException(2, L"time", word, uro);
         }
      }

      _genptr<_nlist> nlist;
      if (parse::parse(uro, args[0], nlist)) {
         _genptr<_num> num2;
         if (parse::parse(uro, args[1], num2)) {
            result = std::make_unique<F_ContainsCol<_num>>(nlist, num2);
            return true;
         }
         else {
            functionArgException(2, L"number", word, uro);
         }
      }

      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         _genptr<_str> str2;
         if (parse::parse(uro, args[1], str2)) {
            result = std::make_unique<F_ContainsCol<_str>>(list, str2);
            return true;
         }
         else {
            functionArgException(2, L"string", word, uro);
         }
      }
      else {
         throw SyntaxException(str(L"first argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string nor any collection"), word.line);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_EXISTSINSIDE) {
      if (len == 0 || len > 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         _genptr<_str> ts;
         uro.vars.inner.createThisRef(ts);
         result = std::make_unique<F_ExistsInside>(str, ts, uro);
         return true;
      }

      _genptr<_str> str2;
      if (parse::parse(uro, args[1], str2)) {
         result = std::make_unique<F_ExistsInside>(str, str2, uro);
         return true;
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_EXISTINSIDE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         if (len == 1) {
            checkFunctionAttribute(word, uro);
            _genptr<_str> ts;
            uro.vars.inner.createThisRef(ts);
            result = std::make_unique<F_AnyInside>(def, ts, uro);
            return true;
         }
         else {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            if (len > 2) {
               functionArgNumberException(len, word, uro);
            }

            _genptr<_str> str2;
            if (parse::parse(uro, args[1], str2)) {
               result = std::make_unique<F_AnyInside>(def, str2, uro);
               return true;
            }
            else {
               functionArgException(2, L"string", word, uro);
            }
         }
      }

      _genptr<_list> list;
      if (!parse::parse(uro, args[0], list)) {
         functionArgException(1, L"list", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         _genptr<_str> ts;
         uro.vars.inner.createThisRef(ts);
         result = std::make_unique<F_ExistInside>(list, ts, uro);
         return true;
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);
      if (len > 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str2;
      if (parse::parse(uro, args[1], str2)) {
         result = std::make_unique<F_ExistInside>(list, str2, uro);
         return true;
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_STARTSWITH) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (args[1].getLength() == 1) {
         const Token& f = args[1].first();
         switch (f.type) {
            case Token::t_Quotation: {
               const _str os = f.getOriginString(uro);

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
      if (parse::parse(uro, args[1], str2)) {
         result = std::make_unique<F_StartsWith>(str, str2);
         return true;
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_ENDSWITH) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (args[1].getLength() == 1) {
         const Token& f = args[1].first();
         switch (f.type) {
            case Token::t_Quotation: {
               const _str os = f.getOriginString(uro);

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
      if (!parse::parse(uro, args[1], str2)) {
         functionArgException(2, L"string", word, uro);
      }

      result = std::make_unique<F_EndsWith>(str, str2);
      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_FIND) {
      if (len == 0 || len > 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         _genptr<_str> ts;
         uro.vars.inner.createThisRef(ts);
         result = std::make_unique<F_Find_InThis>(str, uro);
         return true;
      }

      _genptr<_str> str2;
      if (parse::parse(uro, args[1], str2)) {
         result = std::make_unique<F_Find>(str, str2, uro);
         return true;
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }

   return false;
};

_bool simpleBoolFunction(_genptr<_bool>& result, const Tokens& tks, const Token& word, Uroboros& uro)
{
   _genptr<_str> arg1;
   if (!parse::parse(uro, tks, arg1)) {
      functionArgException(1, L"string", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro.hashes.HASH_FUNC_ISLOWER)
      result = std::make_unique<F_IsLower>(arg1);
   else if (name == uro.hashes.HASH_FUNC_ISUPPER)
      result = std::make_unique<F_IsUpper>(arg1);
   else if (name == uro.hashes.HASH_FUNC_ISNUMBER)
      result = std::make_unique<F_IsNumber>(arg1);
   else if (name == uro.hashes.HASH_VAR_ARCHIVE)
      result = std::make_unique<F_Archive>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_COMPRESSED)
      result = std::make_unique<F_Compressed>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_EMPTY)
      result = std::make_unique<F_Empty>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_ENCRYPTED)
      result = std::make_unique<F_Encrypted>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_EXISTS)
      result = std::make_unique<F_Exists>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_HIDDEN)
      result = std::make_unique<F_Hidden>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_ISDIRECTORY)
      result = std::make_unique<F_IsDirectory>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_ISFILE)
      result = std::make_unique<F_IsFile>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_READONLY)
      result = std::make_unique<F_Readonly>(arg1, uro);
   else if (name == uro.hashes.HASH_FUNC_ISLETTER)
      result = std::make_unique<F_IsLetter>(arg1);
   else if (name == uro.hashes.HASH_FUNC_ISDIGIT)
      result = std::make_unique<F_IsDigit>(arg1);
   else if (name == uro.hashes.HASH_FUNC_ISBINARY)
      result = std::make_unique<F_IsBinary>(arg1);
   else if (name == uro.hashes.HASH_FUNC_ISHEX)
      result = std::make_unique<F_IsHex>(arg1);
   else
      return false;

   return true;
}

_bool numberFunction(_genptr<_num>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (uro.hashes.HASH_GROUP_FUNC_NUM_NUM.find(name) != uro.hashes.HASH_GROUP_FUNC_NUM_NUM.end()) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      return simpleNumberFunction(result, args[0], word, uro);
   }
   else if (name == uro.hashes.HASH_FUNC_LENGTH) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      _genptr<_str> arg1;
      if (parse::parse(uro, args[0], arg1)) {
         result = std::make_unique<F_Length>(arg1);
         return true;
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string. "
            L"If you want to count elements in a collection, then use function 'count' instead"), word.line);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_FROMBINARY || name == uro.hashes.HASH_FUNC_FROMHEX) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      _genptr<_str> arg1;

      if (parse::parse(uro, args[0], arg1)) {
         if (name == uro.hashes.HASH_FUNC_FROMBINARY)
            result = std::make_unique<F_FromBinary>(arg1);
         else
            result = std::make_unique<F_FromHex>(arg1);

         return true;
      }
      else {
         functionArgException(1, L"string", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_VAR_SIZE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         if (len != 1) {
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_Size>(str_, uro);
         return true;
      }

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_SizeDefinition>(def, uro);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_SizeList>(list, uro);
         return true;
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string nor a list"), word.line);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_NUMBER) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      _genptr<_str> arg1;
      if (parse::parse(uro, args[0], arg1)) {
         result = std::make_unique<F_Number>(arg1);
         return true;
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string"), word.line);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_COUNT) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         throw SyntaxException(str(L"the argument of the function '", word.getOriginString(uro),
            L"' is not a collection. If you want to measure length of a string, use function 'length' instead"), word.line);
      }

      _genptr<_tlist> tlist;
      if (parse::parse(uro, args[0], tlist)) {
         result = std::make_unique<F_Count<_tim>>(tlist);
         return true;
      }

      _genptr<_nlist> nlist;
      if (parse::parse(uro, args[0], nlist)) {
         result = std::make_unique<F_Count<_num>>(nlist);
         return true;
      }

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         result = std::make_unique<F_CountDef>(def, uro);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         result = std::make_unique<F_Count<_str>>(list);
         return true;
      }

      throw SyntaxException(str(L"the argument of the function '",
         word.getOriginString(uro), L"' cannot be resolved to any collection"), word.line);
   }
   else if (name == uro.hashes.HASH_FUNC_COUNTINSIDE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _defptr def;
      if (!parse::parse(uro, args[0], def)) {
         functionArgException(1, L"definition", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         _genptr<_str> ts;
         uro.vars.inner.createThisRef(ts);
         result = std::make_unique<F_CountInside>(def, ts, uro);
         return true;
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);
      if (len > 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (parse::parse(uro, args[1], str)) {
         result = std::make_unique<F_CountInside>(def, str, uro);
         return true;
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_POWER) {
      if (len != 2)
         functionArgNumberException(len, word, uro);

      _genptr<_num> arg1;
      if (!parse::parse(uro, args[0], arg1)) {
         functionArgException(1, L"number", word, uro);
      }

      _genptr<_num> arg2;
      if (!parse::parse(uro, args[1], arg2)) {
         functionArgException(2, L"number", word, uro);
      }

      result = std::make_unique<F_Power>(arg1, arg2);
      return true;
   }
   else if (uro.hashes.HASH_GROUP_AGGRFUNC.find(name) != uro.hashes.HASH_GROUP_AGGRFUNC.end()) {
      if (len == 0) {
         throw SyntaxException(str(L"aggregate function '", word.getOriginString(uro),
            L"' needs at least one argument"), word.line);
      }

      return aggrFunction(result, args, word, uro);
   }
   else if (name == uro.hashes.HASH_FUNC_FIRST || name == uro.hashes.HASH_FUNC_LAST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _genptr<_nlist> nlist;
      if (parse::parse(uro, args[0], nlist)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro.hashes.HASH_FUNC_FIRST) {
            result = std::make_unique<F_First<_num>>(nlist);
         }
         else {
            result = std::make_unique<F_Last<_num>>(nlist);
         }

         return true;
      }
   }
   else if (name == uro.hashes.HASH_FUNC_RANDOM) {
      if (len > 1) {
         functionArgNumberException(len, word, uro);
      }

      if (len == 0) {
         result = std::make_unique<F_Random>(uro);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(uro, args[0], num)) {
         uro.math.setAnyRandomDouble();
         result = std::make_unique<F_RandomNumber>(num, uro);
         return true;
      }

      _genptr<_nlist> nlist;
      if (parse::parse(uro, args[0], nlist)) {
         uro.math.setAnyRandomDouble();
         result = std::make_unique<F_RandomElement<_num>>(nlist, uro);
         return true;
      }
   }

   return false;
}

static _bool simpleNumberFunction(_genptr<_num>& result, const Tokens& tks, const Token& word, Uroboros& uro)
{
   _genptr<_num> arg;
   if (!parse::parse(uro, tks, arg)) {
      functionArgException(1, L"number", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro.hashes.HASH_FUNC_ABSOLUTE)
      result = std::make_unique<F_Absolute>(arg);
   else if (name == uro.hashes.HASH_FUNC_CEIL)
      result = std::make_unique<F_Ceil>(arg);
   else if (name == uro.hashes.HASH_FUNC_FLOOR)
      result = std::make_unique<F_Floor>(arg);
   else if (name == uro.hashes.HASH_FUNC_ROUND)
      result = std::make_unique<F_Round>(arg);
   else if (name == uro.hashes.HASH_FUNC_SIGN)
      result = std::make_unique<F_Sign>(arg);
   else if (name == uro.hashes.HASH_FUNC_SQRT)
      result = std::make_unique<F_Sqrt>(arg);
   else if (name == uro.hashes.HASH_FUNC_TRUNCATE)
      result = std::make_unique<F_Truncate>(arg);
   else
      return false;

   return true;
}

static _bool aggrFunction(_genptr<_num>& result, const std::vector<Tokens>& args, const Token& word, Uroboros& uro)
{
   const _size& name = word.value.word.h;

   std::vector<_genptr<_num>> singles;
   std::vector<_genptr<_nlist>> multis;

   const _size len = args.size();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = args[i];

      _genptr<_num> num;
      if (parse::parse(uro, tks, num)) {
         singles.push_back(std::move(num));
      }
      else {
         _genptr<_nlist> nlist;
         if (parse::parse(uro, tks, nlist)) {
            multis.push_back(std::move(nlist));
         }
         else {
            throw SyntaxException(str(ordinalNumber(i + 1),
               L" argument of aggregate function '", word.getOriginString(uro),
               L"' cannot be resolved to a number nor a numeric list"), word.line);
         }
      }
   }

   if (name == uro.hashes.HASH_FUNC_AVERAGE)
      result = std::make_unique<F_Average>(singles, multis);
   else if (name == uro.hashes.HASH_FUNC_MAX)
      result = std::make_unique<F_Max>(singles, multis);
   else if (name == uro.hashes.HASH_FUNC_MEDIAN)
      result = std::make_unique<F_Median>(singles, multis);
   else if (name == uro.hashes.HASH_FUNC_MIN)
      result = std::make_unique<F_Min>(singles, multis);
   else if (name == uro.hashes.HASH_FUNC_SUM)
      result = std::make_unique<F_Sum>(singles, multis);
   else
      return false;

   return true;
}

_bool periodFunction(_genptr<_per>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro.hashes.HASH_FUNC_LIFETIME) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      _genptr<_str> str;
      if (parse::parse(uro, args[0], str)) {
         result = std::make_unique<F_Lifetime>(str, uro);
         return true;
      }
      else {
         functionArgException(1, L"string", word, uro);
      }
   }

   return false;
}

_bool stringFunction(_genptr<_str>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro.hashes.HASH_FUNC_AFTER || name == uro.hashes.HASH_FUNC_BEFORE) {
      if (len != 2)
         functionArgNumberException(len, word, uro);

      return stringTwoArgFunction(result, args, word, uro);
   }
   else if (uro.hashes.HASH_GROUP_FUNC_STR_STR.find(name) != uro.hashes.HASH_GROUP_FUNC_STR_STR.end()) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      return simpleStringFunction(result, args[0], word, uro);
   }
   else if (name == uro.hashes.HASH_FUNC_REVERSED) {
      const _str sub = (word.getOriginString(uro)).substr(0, 7);
      throw SyntaxException(str(L"the proper name for this function is '", sub, L"'"), word.line);
   }
   else if (uro.hashes.HASH_GROUP_FUNC_STR_STR_NUM.find(name) != uro.hashes.HASH_GROUP_FUNC_STR_STR_NUM.end()) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      _genptr<_num> num;
      if (!parse::parse(uro, args[1], num)) {
         functionArgException(2, L"number", word, uro);
      }

      if (name == uro.hashes.HASH_FUNC_REPEAT)
         result = std::make_unique<F_Repeat>(str, num);
      else if (name == uro.hashes.HASH_FUNC_LEFT)
         result = std::make_unique<F_Left>(str, num);
      else if (name == uro.hashes.HASH_FUNC_RIGHT)
         result = std::make_unique<F_Right>(str, num);
      else if (name == uro.hashes.HASH_FUNC_FILL)
         result = std::make_unique<F_Fill>(str, num);

      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_REPLACE) {
      if (len != 3) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str1;
      if (!parse::parse(uro, args[0], str1)) {
         functionArgException(1, L"string", word, uro);
      }

      _genptr<_str> str2;
      if (!parse::parse(uro, args[1], str2)) {
         functionArgException(2, L"string", word, uro);
      }

      _genptr<_str> str3;
      if (!parse::parse(uro, args[2], str3)) {
         functionArgException(3, L"string", word, uro);
      }

      result = std::make_unique<F_Replace>(str1, str2, str3);
      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_SUBSTRING) {
      if (len < 2 || len > 3) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro), L"' can only take"
            L" two or three arguments"), word.line);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      _genptr<_num> num;
      if (!parse::parse(uro, args[1], num)) {
         functionArgException(2, L"number", word, uro);
      }

      if (len == 2) {
         result = std::make_unique<F_Substring_2>(str, num);
         return true;
      }

      _genptr<_num> num2;
      if (!parse::parse(uro, args[2], num2)) {
         functionArgException(3, L"number", word, uro);
      }

      result = std::make_unique<F_Substring_3>(str, num, num2);
      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_CONCATENATE) {
      if (len < 1) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' needs at least one arguments"), word.line);
      }

      std::vector<_genptr<_str>> values;

      for (_size i = 0; i < len; i++) {
         _genptr<_str> str_;
         if (parse::parse(uro, args[i], str_)) {
            values.push_back(std::move(str_));
            continue;
         }

         _genptr<_list> list;
         if (parse::parse(uro, args[i], list)) {
            if (i != len - 1) {
               checkInOperatorCommaAmbiguity(word, args[i], uro);
            }
            values.push_back(std::make_unique<F_ConcatenateUnit>(list));
         }
         else {
            throw SyntaxException(str(ordinalNumber(i + 1), L" argument of the function '",
               word.getOriginString(uro), L"' cannot be resolved to any data type"), word.line);
         }
      }

      result = std::make_unique<F_Concatenate>(values);
      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_FIRST || name == uro.hashes.HASH_FUNC_LAST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _defptr def;
      if (parse::parse(uro, args[0], def)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro.hashes.HASH_FUNC_FIRST) {
            result = std::make_unique<F_FirstDef>(def);
         }
         else {
            result = std::make_unique<F_LastDef>(def);
         }
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro.hashes.HASH_FUNC_FIRST) {
            result = std::make_unique<F_First<_str>>(list);
         }
         else {
            result = std::make_unique<F_Last<_str>>(list);
         }
         return true;
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to any collection"), word.line);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_PATH) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      if (len > 4) {
         std::vector<_genptr<_str>> values;

         for (_size i = 0; i < len; i++) {
            _genptr<_str> str;
            if (!parse::parse(uro, args[i], str)) {
               functionArgException(i + 1, L"string", word, uro);
            }

            values.push_back(std::move(str));
         }

         result = std::make_unique<F_Path_Multi>(values);
         return true;
      }
      else {
         _genptr<_str> str1;
         if (!parse::parse(uro, args[0], str1)) {
            functionArgException(1, L"string", word, uro);
         }

         if (len == 1) {
            result = std::make_unique<F_Path>(str1, uro);
            return true;
         }

         _genptr<_str> str2;
         if (!parse::parse(uro, args[1], str2)) {
            functionArgException(2, L"string", word, uro);
         }

         if (len == 2) {
            result = std::make_unique<F_Path_2>(str1, str2);
            return true;
         }

         _genptr<_str> str3;
         if (!parse::parse(uro, args[2], str3)) {
            functionArgException(3, L"string", word, uro);
         }

         if (len == 3) {
            result = std::make_unique<F_Path_3>(str1, str2, str3);
            return true;
         }

         _genptr<_str> str4;
         if (!parse::parse(uro, args[3], str4)) {
            functionArgException(4, L"string", word, uro);
         }

         result = std::make_unique<F_Path_4>(str1, str2, str3, str4);
         return true;
      }
   }
   else if (name == uro.hashes.HASH_FUNC_STRING) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_bool> boo;
      if (parse::parse(uro, args[0], boo)) {
         result = std::make_unique<F_String_B>(boo);
         return true;
      }

      _genptr<_num> num;
      if (parse::parse(uro, args[0], num)) {
         result = std::make_unique<F_String_N>(num);
         return true;
      }

      _genptr<_tim> tim;
      if (parse::parse(uro, args[0], tim)) {
         result = std::make_unique<F_String_T>(tim);
         return true;
      }

      _genptr<_per> per;
      if (parse::parse(uro, args[0], per)) {
         result = std::make_unique<F_String_P>(per);
         return true;
      }

      throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
        L"' cannot be resolved to any singular data type. If you want to concatenate a collection, use function 'concatenate' instead"), word.line);
   }
   else if (uro.hashes.HASH_GROUP_FUNC_STR_NUM.find(name) != uro.hashes.HASH_GROUP_FUNC_STR_NUM.end()) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_num> num;

      if (parse::parse(uro, args[0], num)) {
         if (name == uro.hashes.HASH_FUNC_ROMAN)
            result = std::make_unique<F_Roman>(num);
         else if (name == uro.hashes.HASH_FUNC_BINARY)
            result = std::make_unique<F_Binary>(num);
         else if (name == uro.hashes.HASH_FUNC_HEX)
            result = std::make_unique<F_Hex>(num);

         return true;
      }
      else {
         functionArgException(1, L"number", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_MONTHNAME) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_num> num;
      if (parse::parse(uro, args[0], num)) {
         result = std::make_unique<F_MonthName>(num);
         return true;
      }
      else {
         functionArgException(1, L"number", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_WEEKDAYNAME) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_num> num;
      if (parse::parse(uro, args[0], num)) {
         result = std::make_unique<F_WeekDayName>(num);
         return true;
      }
      else {
         functionArgException(1, L"number", word, uro);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_RANDOM) {
      if (len == 0) {
         return false;
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         if (len > 1) {
            functionArgNumberException(len, word, uro);
         }
         result = std::make_unique<F_RandomChar>(str_, uro);
         return true;
      }

      _genptr<_list> list;
      if (parse::parse(uro, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         result = std::make_unique<F_RandomElement<_str>>(list, uro);
         return true;
      }
      else {
         throw SyntaxException(str(L"wrong arguments of function '", word.getOriginString(uro), L"'"),
            word.line);
      }
   }
   else if (name == uro.hashes.HASH_FUNC_JOIN) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_list> list;
      if (!parse::parse(uro, args[0], list)) {
         functionArgException(1, L"list", word, uro);
      }

      if (len == 1) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' cannot be called with one argument. If you want to join multiple strings without a separator, use function 'concatenate' instead"), word.line);
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);

      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[1], str)) {
         functionArgException(2, L"string", word, uro);
      }

      result = std::make_unique<F_Join>(list, str);
      return true;
   }

   return false;
}

static _bool stringTwoArgFunction(_genptr<_str>& result, const std::vector<Tokens>& args, const Token& word, Uroboros& uro)
{
   _genptr<_str> arg1;
   if (!parse::parse(uro, args[0], arg1)) {
      functionArgException(1, L"string", word, uro);
   }

   _genptr<_str> arg2;
   if (!parse::parse(uro, args[1], arg2)) {
      functionArgException(2, L"string", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro.hashes.HASH_FUNC_AFTER)
      result = std::make_unique<F_After>(arg1, arg2);
   else if (name == uro.hashes.HASH_FUNC_BEFORE)
      result = std::make_unique<F_Before>(arg1, arg2);
   else
      return false;

   return true;
}

static _bool simpleStringFunction(_genptr<_str>& result, const Tokens& tks, const Token& word, Uroboros& uro)
{
   _genptr<_str> arg1;
   if (!parse::parse(uro, tks, arg1)) {
      functionArgException(1, L"string", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro.hashes.HASH_FUNC_DIGITS)
      result = std::make_unique<F_Digits>(arg1);
   else if (name == uro.hashes.HASH_FUNC_LETTERS)
      result = std::make_unique<F_Letters>(arg1);
   else if (name == uro.hashes.HASH_FUNC_LOWER)
      result = std::make_unique<F_Lower>(arg1);
   else if (name == uro.hashes.HASH_FUNC_TRIM)
      result = std::make_unique<F_Trim>(arg1);
   else if (name == uro.hashes.HASH_FUNC_UPPER)
      result = std::make_unique<F_Upper>(arg1);
   else if (name == uro.hashes.HASH_FUNC_REVERSE)
      result = std::make_unique<F_Reverse>(arg1);
   else if (name == uro.hashes.HASH_VAR_DRIVE)
      result = std::make_unique<F_Drive>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_EXTENSION)
      result = std::make_unique<F_Extension>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_FULLNAME)
      result = std::make_unique<F_Fullname>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_NAME)
      result = std::make_unique<F_Name>(arg1, uro);
   else if (name == uro.hashes.HASH_VAR_PARENT)
      result = std::make_unique<F_Parent>(arg1, uro);
   else if (name == uro.hashes.HASH_FUNC_AFTERDIGITS)
      result = std::make_unique<F_AfterDigits>(arg1);
   else if (name == uro.hashes.HASH_FUNC_AFTERLETTERS)
      result = std::make_unique<F_AfterLetters>(arg1);
   else if (name == uro.hashes.HASH_FUNC_BEFOREDIGITS)
      result = std::make_unique<F_BeforeDigits>(arg1);
   else if (name == uro.hashes.HASH_FUNC_BEFORELETTERS)
      result = std::make_unique<F_BeforeLetters>(arg1);
   else if (name == uro.hashes.HASH_FUNC_CAPITALIZE)
      result = std::make_unique<F_Capitalize>(arg1);
   else
      return false;

   return true;
}

_bool timeFunction(_genptr<_tim>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (uro.hashes.HASH_GROUP_FUNC_TIM_NUM.find(name) != uro.hashes.HASH_GROUP_FUNC_TIM_NUM.end()) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      return simpleTimeFunction(result, args[0], word, uro);
   }

   if (uro.hashes.HASH_GROUP_TIME_ATTR.find(name) != uro.hashes.HASH_GROUP_TIME_ATTR.end()) {

      if (len != 1)
         functionArgNumberException(len, word, uro);

      _genptr<_str> arg1;
      if (!parse::parse(uro, args[0], arg1)) {
         functionArgException(1, L"string", word, uro);
      }

      if (name == uro.hashes.HASH_VAR_ACCESS)
         result = std::make_unique<F_Access>(arg1, uro);
      else if (name == uro.hashes.HASH_VAR_CHANGE)
         result = std::make_unique<F_Change>(arg1, uro);
      else if (name == uro.hashes.HASH_VAR_CREATION)
         result = std::make_unique<F_Creation>(arg1, uro);
      else
         result = std::make_unique<F_Modification>(arg1, uro);

      return true;
   }

   else if (name == uro.hashes.HASH_FUNC_DATE) {
      if (len != 3)
         functionArgNumberException(len, word, uro);

      _genptr<_num> arg1;
      if (!parse::parse(uro, args[0], arg1)) {
         functionArgException(1, L"number", word, uro);
      }

      _genptr<_num> arg2;
      if (!parse::parse(uro, args[1], arg2)) {
         functionArgException(2, L"number", word, uro);
      }

      _genptr<_num> arg3;
      if (!parse::parse(uro, args[2], arg3)) {
         functionArgException(3, L"number", word, uro);
      }

      result = std::make_unique<F_Time_3>(arg1, arg2, arg3);
      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_TIME) {
      if (len < 2 || len == 4 || len > 6) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_num> arg1;
      if (!parse::parse(uro, args[0], arg1)) {
         functionArgException(1, L"number", word, uro);
      }

      _genptr<_num> arg2;
      if (!parse::parse(uro, args[1], arg2)) {
         functionArgException(2, L"number", word, uro);
      }

      if (len == 2) {
         result = std::make_unique<F_Time_2>(arg1, arg2);
         return true;
      }

      _genptr<_num> arg3;
      if (!parse::parse(uro, args[2], arg3)) {
         functionArgException(3, L"number", word, uro);
      }

      if (len == 3) {
         result = std::make_unique<F_Time_3>(arg1, arg2, arg3);
         return true;
      }

      _genptr<_num> arg4;
      if (!parse::parse(uro, args[3], arg4)) {
         functionArgException(4, L"number", word, uro);
      }

      _genptr<_num> arg5;
      if (!parse::parse(uro, args[4], arg5)) {
         functionArgException(5, L"number", word, uro);
      }

      if (len == 5) {
         result = std::make_unique<F_Time_5>(arg1, arg2, arg3, arg4, arg5);
         return true;
      }

      _genptr<_num> arg6;
      if (!parse::parse(uro, args[5], arg6)) {
         functionArgException(6, L"number", word, uro);
      }

      result = std::make_unique<F_Time_6>(arg1, arg2, arg3, arg4, arg5, arg6);
      return true;
   }
   else if (name == uro.hashes.HASH_FUNC_FIRST || name == uro.hashes.HASH_FUNC_LAST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str_;
      if (parse::parse(uro, args[0], str_)) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _genptr<_tlist> tlist;
      if (parse::parse(uro, args[0], tlist)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro.hashes.HASH_FUNC_FIRST) {
            result = std::make_unique<F_First<_tim>>(tlist);
         }
         else {
            result = std::make_unique<F_Last<_tim>>(tlist);
         }

         return true;
      }
   }
   else if (name == uro.hashes.HASH_FUNC_RANDOM) {
      if (len > 1) {
         functionArgNumberException(len, word, uro);
      }

      if (len == 0) {
         return false;
      }

      _genptr<_tlist> tlist;
      if (parse::parse(uro, args[0], tlist)) {
         result = std::make_unique<F_RandomElement<_tim>>(tlist, uro);
         return true;
      }
   }

   return false;
}

static _bool simpleTimeFunction(_genptr<_tim>& result, const Tokens& tks, const Token& word, Uroboros& uro)
{
   _genptr<_num> arg1;
   if (!parse::parse(uro, tks, arg1)) {
      functionArgException(1, L"number", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro.hashes.HASH_FUNC_CHRISTMAS)
      result = std::make_unique<F_Christmas>(arg1);
   else if (name == uro.hashes.HASH_FUNC_EASTER)
      result = std::make_unique<F_Easter>(arg1);
   else if (name == uro.hashes.HASH_FUNC_NEWYEAR)
      result = std::make_unique<F_NewYear>(arg1);
   else
      return false;

   return true;
}


static void functionArgNumberException(const _int& argNumber, const Token& word, Uroboros& uro)
{
   throw SyntaxException(str(L"function '", word.getOriginString(uro), L"' cannot be called with ",
      toStr(argNumber), L" argument", (argNumber == 1 ? EMPTY_STRING : L"s")), word.line);
}

static void functionArgException(const _int& argNumber, const _str& typeName, const Token& word, Uroboros& uro)
{
   throw SyntaxException(str(ordinalNumber(argNumber), L" argument of function '",
      word.getOriginString(uro), L"' cannot be resolved to a ", typeName), word.line);
}

static _str ordinalNumber(const _int& number)
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

_bool listFunction(_genptr<_list>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro.hashes.HASH_FUNC_CHARACTERS || name == uro.hashes.HASH_FUNC_WORDS) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (name == uro.hashes.HASH_FUNC_CHARACTERS)
         result = std::make_unique<F_Characters>(str);
      else
         result = std::make_unique<F_Words>(str);

      return true;
   }
   if (name == uro.hashes.HASH_FUNC_SPLIT) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str1;
      if (!parse::parse(uro, args[0], str1)) {
         functionArgException(1, L"string", word, uro);
      }

      _genptr<_str> str2;
      if (!parse::parse(uro, args[1], str2)) {
         functionArgException(2, L"string", word, uro);
      }

      result = std::make_unique<F_Split>(str1, str2);
      return true;
   }

   throw SyntaxException(str(L"function with name '", word.getOriginString(uro),
      L"' does not exist"), word.line);
}


_bool numListFunction(_genptr<_nlist>& result, const Tokens& tks, Uroboros& uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro.hashes.HASH_FUNC_NUMBERS) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      _genptr<_str> str;
      if (!parse::parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      result = std::make_unique<F_Numbers>(str);
      return true;
   }

   return false;
}


static void checkFunctionAttribute(const Token& word, Uroboros& uro)
{
   if (!uro.vc.anyAttribute() || uro.vars.inner.thisState != ThisState::ts_String) {
      throw SyntaxException(str(L"function '", word.getOriginString(uro),
         L"' can be called only inside an iteration loop"), word.line);
   }
}

static void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, Uroboros& uro)
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

   if (right.first().isSymbol(L'(')) {
      return;
   }

   const Token& inToken = tks.listAt(index);
   throw SyntaxException(str(L"the right side of the operator '", inToken.getOriginString(uro),
      L"' used in function '", word.getOriginString(uro),
      L"' should be embraced by brackets. Comma is a function argument separator and causes ambiguity here"),
      word.line);
}

}
