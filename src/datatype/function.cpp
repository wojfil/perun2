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

#include "function.h"
#include "../exception.h"
#include "function/func-bool.h"
#include "function/func-number.h"
#include "function/func-string.h"
#include "function/func-time.h"
#include "function/func-list.h"
#include "function/func-aggr.h"
#include "function/func-attr.h"
#include "parse-gen.h"
#include "../util.h"
#include "generator/gen-generic.h"
#include "generator/gen-number.h"
#include "generator/gen-string.h"
#include "generator/gen-definition.h"
#include "../brackets.h"
#include "../var/var-runtime.h"
#include "math.h"
#include <cwctype>

// function parsing could have been done in a more generalized way
// on the other hand, current approach
// enables variety of precise error messages

static std::vector<Tokens> toFunctionArgs(const Tokens& tks)
{
   Tokens tks2(tks);
   tks2.trimFunction();

   std::vector<Tokens> result;
   tks2.splitBySymbol(L',', result);

   return result;
}

Generator<_boo>* boolFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (uro->hashes.HASH_GROUP_FUNC_BOO_STR.find(name) != uro->hashes.HASH_GROUP_FUNC_BOO_STR.end())
   {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      return simpleBoolFunction(args[0], word, uro);
   }
   else if (name == uro->hashes.HASH_FUNC_EXIST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _def* def;
      if (parse(uro, args[0], def)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         return new F_AnyDef(def);
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         return new F_Exist(list, uro);
      }
      else {
         functionArgException(1, L"list", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_ANY) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         delete str_;
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a collection"), word.line);
      }

      _def* def;
      if (parse(uro, args[0], def)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         return new F_AnyDef(def);
      }

      Generator<_tlist>* tlist;
      if (parse(uro, args[0], tlist)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         return new F_Any<_tim>(tlist);
      }

      Generator<_nlist>* nlist;
      if (parse(uro, args[0], nlist)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         return new F_Any<_num>(nlist);
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         return new F_Any<_str>(list);
      }
      else {
         functionArgException(1, L"list", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_ANYINSIDE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str1;
      if (parse(uro, args[0], str1)) {
         delete str1;
         throw SyntaxException(str(L"first argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a collection"), word.line);
      }

      _def* def;
      if (!parse(uro, args[0], def)) {
         functionArgException(1, L"definition", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         Generator<_str>* ts;
         uro->vars.inner.createThisRef(ts);
         return new F_AnyInside(def, ts, uro);
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);

      if (len > 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (parse(uro, args[1], str)) {
         return new F_AnyInside(def, str, uro);
      }
      else {
         delete def;
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_CONTAINS) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         Generator<_str>* str2;
         if (parse(uro, args[1], str2)) {
            return new F_ContainsStr(str_, str2);
         }
         else {
            delete str_;
            functionArgException(2, L"string", word, uro);
         }
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);

      _def* def;
      if (parse(uro, args[0], def)) {
         Generator<_str>* str2;
         if (parse(uro, args[1], str2)) {
            return new F_ContainsDef(def, str2, uro);
         }
         else {
            delete def;
            functionArgException(2, L"string", word, uro);
         }
      }

      Generator<_tlist>* tlist;
      if (parse(uro, args[0], tlist)) {
         Generator<_tim>* tim2;
         if (parse(uro, args[1], tim2)) {
            return new F_ContainsCol<_tim>(tlist, tim2);
         }
         else {
            delete tlist;
            functionArgException(2, L"time", word, uro);
         }
      }

      Generator<_nlist>* nlist;
      if (parse(uro, args[0], nlist)) {
         Generator<_num>* num2;
         if (parse(uro, args[1], num2)) {
            return new F_ContainsCol<_num>(nlist, num2);
         }
         else {
            delete nlist;
            functionArgException(2, L"number", word, uro);
         }
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
         Generator<_str>* str2;
         if (parse(uro, args[1], str2)) {
            return new F_ContainsCol<_str>(list, str2);
         }
         else {
            delete list;
            functionArgException(2, L"string", word, uro);
         }
      }
      else {
         throw SyntaxException(str(L"first argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string nor any collection"), word.line);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_EXISTSINSIDE) {
      if (len == 0 || len > 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         Generator<_str>* ts;
         uro->vars.inner.createThisRef(ts);
         return new F_ExistsInside(str, ts, uro);
      }

      Generator<_str>* str2;
      if (parse(uro, args[1], str2)) {
         return new F_ExistsInside(str, str2, uro);
      }
      else {
         delete str;
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_EXISTINSIDE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _def* def;
      if (parse(uro, args[0], def)) {
         if (len == 1) {
            checkFunctionAttribute(word, uro);
            Generator<_str>* ts;
            uro->vars.inner.createThisRef(ts);
            return new F_AnyInside(def, ts, uro);
         }
         else {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            if (len > 2) {
               functionArgNumberException(len, word, uro);
            }

            Generator<_str>* str2;
            if (parse(uro, args[1], str2)) {
               return new F_AnyInside(def, str2, uro);
            }
            else {
               functionArgException(2, L"string", word, uro);
            }
         }
      }

      Generator<_list>* list;
      if (!parse(uro, args[0], list)) {
         functionArgException(1, L"list", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         Generator<_str>* ts;
         uro->vars.inner.createThisRef(ts);
         return new F_ExistInside(list, ts, uro);
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);
      if (len > 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str2;
      if (parse(uro, args[1], str2)) {
         return new F_ExistInside(list, str2, uro);
      }
      else {
         delete list;
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_STARTSWITH) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (args[1].getLength() == 1) {
         const Token& f = args[1].first();
         switch (f.type) {
            case Token::t_Quotation: {
               const _str os = f.getOriginString(uro);

               switch (os.size()) {
                  case 0: {
                     return new Constant<_boo>(true);
                  }
                  case 1: {
                     const _char ch = os[0];
                     return new F_StartsWithChar(str, ch);
                  }
                  default: {
                     return new F_StartsWithConst(str, os);
                  }
               }
            }
            case Token::t_Number: {
               const _str conv = f.value.num.n.toString();
               switch (conv.size()) {
                  case 1: {
                     const _char ch = conv[0];
                     return new F_StartsWithChar(str, ch);
                  }
                  default: {
                     return new F_StartsWithConst(str, conv);
                  }
               }
            }
         }
      }

      Generator<_str>* str2;
      if (parse(uro, args[1], str2)) {
         return new F_StartsWith(str, str2);
      }
      else {
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_ENDSWITH) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (args[1].getLength() == 1) {
         const Token& f = args[1].first();
         switch (f.type) {
            case Token::t_Quotation: {
               const _str os = f.getOriginString(uro);

               switch (os.size()) {
                  case 0: {
                     return new Constant<_boo>(true);
                  }
                  case 1: {
                     const _char ch = os[0];
                     return new F_EndsWithChar(str, ch);
                  }
                  default: {
                     return new F_EndsWithConst(str, os);
                  }
               }
            }
            case Token::t_Number: {
               const _str conv = f.value.num.n.toString();
               switch (conv.size()) {
                  case 1: {
                     const _char ch = conv[0];
                     return new F_EndsWithChar(str, ch);
                  }
                  default: {
                     return new F_EndsWithConst(str, conv);
                  }
               }
            }
         }
      }

      Generator<_str>* str2;
      if (!parse(uro, args[1], str2)) {
         functionArgException(2, L"string", word, uro);
      }

      return new F_EndsWith(str, str2);
   }

   return nullptr;
}

static Generator<_boo>* simpleBoolFunction(const Tokens& tks, const Token& word, Uroboros* uro)
{
   Generator<_str>* arg1;
   if (!parse(uro, tks, arg1)) {
      functionArgException(1, L"string", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro->hashes.HASH_FUNC_ISLOWER)
      return new F_IsLower(arg1);
   else if (name == uro->hashes.HASH_FUNC_ISUPPER)
      return new F_IsUpper(arg1);
   else if (name == uro->hashes.HASH_FUNC_ISNUMBER)
      return new F_IsNumber(arg1);
   else if (name == uro->hashes.HASH_VAR_ARCHIVE)
      return new F_Archive(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_COMPRESSED)
      return new F_Compressed(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_EMPTY)
      return new F_Empty(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_ENCRYPTED)
      return new F_Encrypted(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_EXISTS)
      return new F_Exists(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_HIDDEN)
      return new F_Hidden(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_ISDIRECTORY)
      return new F_IsDirectory(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_ISFILE)
      return new F_IsFile(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_READONLY)
      return new F_Readonly(arg1, uro);
   else if (name == uro->hashes.HASH_FUNC_ISLETTER)
      return new F_IsLetter(arg1);
   else if (name == uro->hashes.HASH_FUNC_ISDIGIT)
      return new F_IsDigit(arg1);
   else if (name == uro->hashes.HASH_FUNC_ISBINARY)
      return new F_IsBinary(arg1);
   else if (name == uro->hashes.HASH_FUNC_ISHEX)
      return new F_IsHex(arg1);
   return nullptr;
}

Generator<_num>* numberFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (uro->hashes.HASH_GROUP_FUNC_NUM_NUM.find(name) != uro->hashes.HASH_GROUP_FUNC_NUM_NUM.end()) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      return simpleNumberFunction(args[0], word, uro);
   }
   else if (name == uro->hashes.HASH_FUNC_LENGTH) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      Generator<_str>* arg1;
      if (parse(uro, args[0], arg1)) {
         return new F_Length(arg1);
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string. "
            L"If you want to count elements in a collection, then use function 'count' instead"), word.line);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_FROMBINARY || name == uro->hashes.HASH_FUNC_FROMHEX) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      Generator<_str>* arg1;

      if (parse(uro, args[0], arg1)) {
         if (name == uro->hashes.HASH_FUNC_FROMBINARY)
            return new F_FromBinary(arg1);
         else
            return new F_FromHex(arg1);
      }
      else {
         functionArgException(1, L"string", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_VAR_SIZE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         if (len != 1) {
            functionArgNumberException(len, word, uro);
         }
         return new F_Size(str_, uro);
      }

      _def* def;
      if (parse(uro, args[0], def)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         return new F_SizeDefinition(def, uro);
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }
         return new F_SizeList(list, uro);
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string nor a list"), word.line);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_NUMBER) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      Generator<_str>* arg1;
      if (parse(uro, args[0], arg1)) {
         return new F_Number(arg1);
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to a string"), word.line);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_COUNT) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         delete str_;
         throw SyntaxException(str(L"the argument of the function '", word.getOriginString(uro),
            L"' is not a collection. If you want to measure length of a string, use function 'length' instead"), word.line);
      }

      Generator<_tlist>* tlist;
      if (parse(uro, args[0], tlist)) {
          return new F_Count<_tim>(tlist);
      }

      Generator<_nlist>* nlist;
      if (parse(uro, args[0], nlist)) {
          return new F_Count<_num>(nlist);
      }

      _def* def;
      if (parse(uro, args[0], def)) {
          return new F_CountDef(def, uro);
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
          return new F_Count<_str>(list);
      }

      throw SyntaxException(str(L"the argument of the function '",
         word.getOriginString(uro), L"' cannot be resolved to any collection"), word.line);
   }
   else if (name == uro->hashes.HASH_FUNC_COUNTINSIDE) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      _def* def;
      if (!parse(uro, args[0], def)) {
         functionArgException(1, L"definition", word, uro);
      }

      if (len == 1) {
         checkFunctionAttribute(word, uro);
         Generator<_str>* ts;
         uro->vars.inner.createThisRef(ts);
         return new F_CountInside(def, ts, uro);
      }

      checkInOperatorCommaAmbiguity(word, args[0], uro);
      if (len > 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (parse(uro, args[1], str)) {
         return new F_CountInside(def, str, uro);
      }
      else {
         delete def;
         functionArgException(2, L"string", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_POWER) {
      if (len != 2)
         functionArgNumberException(len, word, uro);

      Generator<_num>* arg1;
      if (!parse(uro, args[0], arg1)) {
         functionArgException(1, L"number", word, uro);
      }

      Generator<_num>* arg2;
      if (!parse(uro, args[1], arg2)) {
         delete arg1;
         functionArgException(2, L"number", word, uro);
      }

      return new F_Power(arg1, arg2);
   }
   else if (uro->hashes.HASH_GROUP_AGGRFUNC.find(name) != uro->hashes.HASH_GROUP_AGGRFUNC.end()) {
      if (len == 0) {
         throw SyntaxException(str(L"aggregate function '", word.getOriginString(uro),
            L"' needs at least one argument"), word.line);
      }

      return aggrFunction(args, word, uro);
   }
   else if (name == uro->hashes.HASH_FUNC_FIRST || name == uro->hashes.HASH_FUNC_LAST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         delete str_;

         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' can only take a collection of values as an argument"), word.line);
      }

      Generator<_nlist>* nlist;
      if (parse(uro, args[0], nlist)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro->hashes.HASH_FUNC_FIRST) {
            return new F_First<_num>(nlist);
         }
         else {
            return new F_Last<_num>(nlist);
         }
      }
   }
   else if (name == uro->hashes.HASH_FUNC_RANDOM) {
      if (len > 1) {
         functionArgNumberException(len, word, uro);
      }

      if (len == 0) {
         return new F_Random(uro);
      }

      Generator<_num>* num;
      if (parse(uro, args[0], num)) {
         uro->math.setAnyRandomDouble();
         return new F_RandomNumber(num, uro);
      }

      Generator<_nlist>* nlist;
      if (parse(uro, args[0], nlist)) {
         uro->math.setAnyRandomDouble();
         return new F_RandomElement<_num>(nlist, uro);
      }
   }

   return nullptr;
}

static Generator<_num>* simpleNumberFunction(const Tokens& tks, const Token& word, Uroboros* uro)
{
   Generator<_num>* arg;
   if (!parse(uro, tks, arg)) {
      functionArgException(1, L"number", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro->hashes.HASH_FUNC_ABSOLUTE)
      return new F_Absolute(arg);
   else if (name == uro->hashes.HASH_FUNC_CEIL)
      return new F_Ceil(arg);
   else if (name == uro->hashes.HASH_FUNC_FLOOR)
      return new F_Floor(arg);
   else if (name == uro->hashes.HASH_FUNC_ROUND)
      return new F_Round(arg);
   else if (name == uro->hashes.HASH_FUNC_SIGN)
      return new F_Sign(arg);
   else if (name == uro->hashes.HASH_FUNC_SQRT)
      return new F_Sqrt(arg);
   else if (name == uro->hashes.HASH_FUNC_TRUNCATE)
      return new F_Truncate(arg);
   else
      return nullptr;
}

static Generator<_num>* aggrFunction(const std::vector<Tokens>& args, const Token& word, Uroboros* uro)
{
   const _size& name = word.value.word.h;

   std::vector<Generator<_num>*>* singles = new std::vector<Generator<_num>*>();
   std::vector<Generator<_nlist>*>* multis = new std::vector<Generator<_nlist>*>();

   const _size len = args.size();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = args[i];

      Generator<_num>* num;
      if (parse(uro, tks, num)) {
         singles->push_back(num);
      }
      else {
         Generator<_nlist>* nlist;
         if (parse(uro, tks, nlist)) {
            multis->push_back(nlist);
         }
         else {
            deleteVectorPtr(singles);
            deleteVectorPtr(multis);

            throw SyntaxException(str(ordinalNumber(i + 1),
               L" argument of aggregate function '", word.getOriginString(uro),
               L"' cannot be resolved to a number nor a numeric list"), word.line);
         }
      }
   }

   if (name == uro->hashes.HASH_FUNC_AVERAGE)
      return new F_Average(singles, multis);
   else if (name == uro->hashes.HASH_FUNC_MAX)
      return new F_Max(singles, multis);
   else if (name == uro->hashes.HASH_FUNC_MEDIAN)
      return new F_Median(singles, multis);
   else if (name == uro->hashes.HASH_FUNC_MIN)
      return new F_Min(singles, multis);
   else if (name == uro->hashes.HASH_FUNC_SUM)
      return new F_Sum(singles, multis);
   else
      return nullptr;
}

Generator<_per>* periodFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro->hashes.HASH_FUNC_LIFETIME) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      Generator<_str>* str;
      if (parse(uro, args[0], str)) {
         return new F_Lifetime(str, uro);
      }
      else {
         functionArgException(1, L"string", word, uro);
      }
   }

   return nullptr;
}

Generator<_str>* stringFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro->hashes.HASH_FUNC_AFTER || name == uro->hashes.HASH_FUNC_BEFORE) {
      if (len != 2)
         functionArgNumberException(len, word, uro);

      return stringTwoArgFunction(args, word, uro);
   }
   else if (uro->hashes.HASH_GROUP_FUNC_STR_STR.find(name) != uro->hashes.HASH_GROUP_FUNC_STR_STR.end()) {

      if (len != 1)
         functionArgNumberException(len, word, uro);

      return simpleStringFunction(args[0], word, uro);
   }
   else if (name == uro->hashes.HASH_FUNC_REVERSED) {
      const _str sub = (word.getOriginString(uro)).substr(0, 7);
      throw SyntaxException(str(L"the proper name for this function is '", sub, L"'"), word.line);
   }
   else if (uro->hashes.HASH_GROUP_FUNC_STR_STR_NUM.find(name) != uro->hashes.HASH_GROUP_FUNC_STR_STR_NUM.end()) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      Generator<_num>* num;
      if (!parse(uro, args[1], num)) {
         delete str;
         functionArgException(2, L"number", word, uro);
      }

      if (name == uro->hashes.HASH_FUNC_REPEAT)
         return new F_Repeat(str, num);
      else if (name == uro->hashes.HASH_FUNC_LEFT)
         return new F_Left(str, num);
      else if (name == uro->hashes.HASH_FUNC_RIGHT)
         return new F_Right(str, num);
      else if (name == uro->hashes.HASH_FUNC_FILL)
         return new F_Fill(str, num);
   }
   else if (name == uro->hashes.HASH_FUNC_REPLACE) {
      if (len != 3) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str1;
      if (!parse(uro, args[0], str1)) {
         functionArgException(1, L"string", word, uro);
      }

      Generator<_str>* str2;
      if (!parse(uro, args[1], str2)) {
         delete str1;
         functionArgException(2, L"string", word, uro);
      }

      Generator<_str>* str3;
      if (!parse(uro, args[2], str3)) {
         delete str1;
         delete str2;
         functionArgException(3, L"string", word, uro);
      }

      return new F_Replace(str1, str2, str3);
   }
   else if (name == uro->hashes.HASH_FUNC_SUBSTRING) {
      if (len < 2 || len > 3) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro), L"' can only take "
            L" two or three arguments"), word.line);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      Generator<_num>* num;
      if (!parse(uro, args[1], num)) {
         delete str;
         functionArgException(2, L"number", word, uro);
      }

      if (len == 2) {
         return new F_Substring_2(str, num);
      }

      Generator<_num>* num2;
      if (!parse(uro, args[2], num2)) {
         delete str;
         delete num;
         functionArgException(3, L"number", word, uro);
      }

      return new F_Substring_3(str, num, num2);
   }
   else if (name == uro->hashes.HASH_FUNC_CONCATENATE) {
      if (len < 1) {
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' needs at least one arguments"), word.line);
      }

      std::vector<Generator<_str>*>* values = new std::vector<Generator<_str>*>();

      for (_size i = 0; i < len; i++) {
         Generator<_str>* str_;
         if (parse(uro, args[i], str_)) {
            values->push_back(str_);
            continue;
         }

         Generator<_list>* list;
         if (parse(uro, args[i], list)) {
            if (i != len - 1) {
               checkInOperatorCommaAmbiguity(word, args[i], uro);
            }
            values->push_back(new F_ConcatenateUnit(list));
         }
         else {
            deleteVectorPtr(values);

            throw SyntaxException(str(ordinalNumber(i + 1), L" argument of the function '",
               word.getOriginString(uro), L"' cannot be resolved to any data type"), word.line);
         }
      }

      return new F_Concatenate(values);
   }
   else if (name == uro->hashes.HASH_FUNC_FIRST || name == uro->hashes.HASH_FUNC_LAST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         delete str_;
         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' can only take a collection of values as an argument"), word.line);
      }

      _def* def;
      if (parse(uro, args[0], def)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro->hashes.HASH_FUNC_FIRST) {
            return new F_FirstDef(def);
         }
         else {
            return new F_LastDef(def);
         }
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro->hashes.HASH_FUNC_FIRST) {
            return new F_First<_str>(list);
         }
         else {
            return new F_Last<_str>(list);
         }
      }
      else {
         throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
            L"' cannot be resolved to any collection"), word.line);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_PATH) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      if (len > 4) {
         std::vector<Generator<_str>*>* values = new std::vector<Generator<_str>*>();

         for (_size i = 0; i < len; i++) {
            Generator<_str>* str;
            if (!parse(uro, args[i], str)) {
               deleteVectorPtr(values);
               functionArgException(i + 1, L"string", word, uro);
            }

            values->push_back(str);
         }

         return new F_Path_Multi(values);
      }
      else {
         Generator<_str>* str1;
         if (!parse(uro, args[0], str1)) {
            functionArgException(1, L"string", word, uro);
         }

         if (len == 1) {
            return new F_Path(str1, uro);
         }

         Generator<_str>* str2;
         if (!parse(uro, args[1], str2)) {
            delete str1;
            functionArgException(2, L"string", word, uro);
         }

         if (len == 2) {
            return new F_Path_2(str1, str2);
         }

         Generator<_str>* str3;
         if (!parse(uro, args[2], str3)) {
            delete str1;
            delete str2;
            functionArgException(3, L"string", word, uro);
         }

         if (len == 3) {
            return new F_Path_3(str1, str2, str3);
         }

         Generator<_str>* str4;
         if (!parse(uro, args[3], str4)) {
            delete str1;
            delete str2;
            delete str3;
            functionArgException(4, L"string", word, uro);
         }

         return new F_Path_4(str1, str2, str3, str4);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_STRING) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_boo>* boo;
      if (parse(uro, args[0], boo)) {
         return new F_String_B(boo);
      }

      Generator<_num>* num;
      if (parse(uro, args[0], num)) {
         return new F_String_N(num);
      }

      Generator<_tim>* tim;
      if (parse(uro, args[0], tim)) {
         return new F_String_T(tim);
      }

      Generator<_per>* per;
      if (parse(uro, args[0], per)) {
         return new F_String_P(per);
      }

      throw SyntaxException(str(L"the argument of function '", word.getOriginString(uro),
        L"' cannot be resolved to any singular data type. If you want to concatenate a collection, use function 'concatenate' instead"), word.line);
   }
   else if (uro->hashes.HASH_GROUP_FUNC_STR_NUM.find(name) != uro->hashes.HASH_GROUP_FUNC_STR_NUM.end()) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_num>* num;

      if (parse(uro, args[0], num)) {
         if (name == uro->hashes.HASH_FUNC_ROMAN)
            return new F_Roman(num);
         else if (name == uro->hashes.HASH_FUNC_BINARY)
            return new F_Binary(num);
         else if (name == uro->hashes.HASH_FUNC_HEX)
            return new F_Hex(num);
      }
      else {
         functionArgException(1, L"number", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_MONTHNAME) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_num>* num;
      if (parse(uro, args[0], num)) {
         return new F_MonthName(num);
      }
      else {
         functionArgException(1, L"number", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_WEEKDAYNAME) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_num>* num;
      if (parse(uro, args[0], num)) {
         return new F_WeekDayName(num);
      }
      else {
         functionArgException(1, L"number", word, uro);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_RANDOM) {
      if (len == 0) {
         return nullptr;
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         if (len > 1) {
            functionArgNumberException(len, word, uro);
         }
         return new F_RandomChar(str_, uro);
      }

      Generator<_list>* list;
      if (parse(uro, args[0], list)) {
         if (len > 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         return new F_RandomElement<_str>(list, uro);
      }
      else {
         throw SyntaxException(str(L"wrong arguments of function '", word.getOriginString(uro)),
            word.line);
      }
   }
   else if (name == uro->hashes.HASH_FUNC_JOIN) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_list>* list;
      if (!parse(uro, args[0], list)) {
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

      Generator<_str>* str;
      if (!parse(uro, args[1], str)) {
         functionArgException(2, L"string", word, uro);
      }

      return new F_Join(list, str);
   }

   return nullptr;
}

static Generator<_str>* stringTwoArgFunction(const std::vector<Tokens>& args, const Token& word, Uroboros* uro)
{
   Generator<_str>* arg1;
   if (!parse(uro, args[0], arg1)) {
      functionArgException(1, L"string", word, uro);
   }

   Generator<_str>* arg2;
   if (!parse(uro, args[1], arg2)) {
      delete arg1;
      functionArgException(2, L"string", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro->hashes.HASH_FUNC_AFTER)
      return new F_After(arg1, arg2);
   else if (name == uro->hashes.HASH_FUNC_BEFORE)
      return new F_Before(arg1, arg2);

   return nullptr;
}

static Generator<_str>* simpleStringFunction(const Tokens& tks, const Token& word, Uroboros* uro)
{
   Generator<_str>* arg1;
   if (!parse(uro, tks, arg1)) {
      functionArgException(1, L"string", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro->hashes.HASH_FUNC_DIGITS)
      return new F_Digits(arg1);
   else if (name == uro->hashes.HASH_FUNC_LETTERS)
      return new F_Letters(arg1);
   else if (name == uro->hashes.HASH_FUNC_LOWER)
      return new F_Lower(arg1);
   else if (name == uro->hashes.HASH_FUNC_TRIM)
      return new F_Trim(arg1);
   else if (name == uro->hashes.HASH_FUNC_UPPER)
      return new F_Upper(arg1);
   else if (name == uro->hashes.HASH_FUNC_REVERSE)
      return new F_Reverse(arg1);
   else if (name == uro->hashes.HASH_VAR_DRIVE)
      return new F_Drive(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_EXTENSION)
      return new F_Extension(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_FULLNAME)
      return new F_Fullname(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_NAME)
      return new F_Name(arg1, uro);
   else if (name == uro->hashes.HASH_VAR_PARENT)
      return new F_Parent(arg1, uro);
   else if (name == uro->hashes.HASH_FUNC_AFTERDIGITS)
      return new F_AfterDigits(arg1);
   else if (name == uro->hashes.HASH_FUNC_AFTERLETTERS)
      return new F_AfterLetters(arg1);
   else if (name == uro->hashes.HASH_FUNC_BEFOREDIGITS)
      return new F_BeforeDigits(arg1);
   else if (name == uro->hashes.HASH_FUNC_BEFORELETTERS)
      return new F_BeforeLetters(arg1);
   else if (name == uro->hashes.HASH_FUNC_CAPITALIZE)
      return new F_Capitalize(arg1);
   else
      return nullptr;
}

Generator<_tim>* timeFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (uro->hashes.HASH_GROUP_FUNC_TIM_NUM.find(name) != uro->hashes.HASH_GROUP_FUNC_TIM_NUM.end()) {
      if (len != 1)
         functionArgNumberException(len, word, uro);

      return simpleTimeFunction(args[0], word, uro);
   }

   if (uro->hashes.HASH_GROUP_TIME_ATTR.find(name) != uro->hashes.HASH_GROUP_TIME_ATTR.end()) {

      if (len != 1)
         functionArgNumberException(len, word, uro);

      Generator<_str>* arg1;
      if (!parse(uro, args[0], arg1)) {
         functionArgException(1, L"string", word, uro);
      }

      if (name == uro->hashes.HASH_VAR_ACCESS)
         return new F_Access(arg1, uro);
      else if (name == uro->hashes.HASH_VAR_CHANGE)
         return new F_Change(arg1, uro);
      else if (name == uro->hashes.HASH_VAR_CREATION)
         return new F_Creation(arg1, uro);
      else
         return new F_Modification(arg1, uro);
   }

   else if (name == uro->hashes.HASH_FUNC_DATE) {
      if (len != 3)
         functionArgNumberException(len, word, uro);

      Generator<_num>* arg1;
      if (!parse(uro, args[0], arg1)) {
         functionArgException(1, L"number", word, uro);
      }

      Generator<_num>* arg2;
      if (!parse(uro, args[1], arg2)) {
         delete arg1;
         functionArgException(2, L"number", word, uro);
      }

      Generator<_num>* arg3;
      if (!parse(uro, args[2], arg3)) {
         delete arg1;
         delete arg2;
         functionArgException(3, L"number", word, uro);
      }

      return new F_Time_3(arg1, arg2, arg3);
   }
   else if (name == uro->hashes.HASH_FUNC_TIME) {
      if (len < 2 || len == 4 || len > 6) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_num>* arg1;
      if (!parse(uro, args[0], arg1)) {
         functionArgException(1, L"number", word, uro);
      }

      Generator<_num>* arg2;
      if (!parse(uro, args[1], arg2)) {
         delete arg1;
         functionArgException(2, L"number", word, uro);
      }

      if (len == 2) {
         return new F_Time_2(arg1, arg2);
      }

      Generator<_num>* arg3;
      if (!parse(uro, args[2], arg3)) {
         delete arg1;
         delete arg2;
         functionArgException(3, L"number", word, uro);
      }

      if (len == 3) {
         return new F_Time_3(arg1, arg2, arg3);
      }

      Generator<_num>* arg4;
      if (!parse(uro, args[3], arg4)) {
         delete arg1;
         delete arg2;
         delete arg3;
         functionArgException(4, L"number", word, uro);
      }

      Generator<_num>* arg5;
      if (!parse(uro, args[4], arg5)) {
         delete arg1;
         delete arg2;
         delete arg3;
         delete arg4;
         functionArgException(5, L"number", word, uro);
      }

      if (len == 5) {
         return new F_Time_5(arg1, arg2, arg3, arg4, arg5);
      }

      Generator<_num>* arg6;
      if (!parse(uro, args[5], arg6)) {
         delete arg1;
         delete arg2;
         delete arg3;
         delete arg4;
         delete arg5;
         functionArgException(6, L"number", word, uro);
      }

      return new F_Time_6(arg1, arg2, arg3, arg4, arg5, arg6);
   }
   else if (name == uro->hashes.HASH_FUNC_FIRST || name == uro->hashes.HASH_FUNC_LAST) {
      if (len == 0) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str_;
      if (parse(uro, args[0], str_)) {
         delete str_;

         throw SyntaxException(str(L"function '", word.getOriginString(uro),
            L"' can only take a collection of values as an argument"), word.line);
      }

      Generator<_tlist>* tlist;
      if (parse(uro, args[0], tlist)) {
         if (len != 1) {
            checkInOperatorCommaAmbiguity(word, args[0], uro);
            functionArgNumberException(len, word, uro);
         }

         if (name == uro->hashes.HASH_FUNC_FIRST) {
            return new F_First<_tim>(tlist);
         }
         else {
            return new F_Last<_tim>(tlist);
         }
      }
   }
   else if (name == uro->hashes.HASH_FUNC_RANDOM) {
      if (len > 1) {
         functionArgNumberException(len, word, uro);
      }

      if (len == 0) {
         return nullptr;
      }

      Generator<_tlist>* tlist;
      if (parse(uro, args[0], tlist)) {
         return new F_RandomElement<_tim>(tlist, uro);
      }
   }

   return nullptr;
}

static Generator<_tim>* simpleTimeFunction(const Tokens& tks, const Token& word, Uroboros* uro)
{
   Generator<_num>* arg1;
   if (!parse(uro, tks, arg1)) {
      functionArgException(1, L"number", word, uro);
   }

   const _size& name = word.value.word.h;

   if (name == uro->hashes.HASH_FUNC_CHRISTMAS)
      return new F_Christmas(arg1);
   else if (name == uro->hashes.HASH_FUNC_EASTER)
      return new F_Easter(arg1);
   else if (name == uro->hashes.HASH_FUNC_NEWYEAR)
      return new F_NewYear(arg1);
   else
      return nullptr;
}


static void functionArgNumberException(const _int& argNumber, const Token& word, Uroboros* uro)
{
   throw SyntaxException(str(L"function '", word.getOriginString(uro), L"' cannot be called with ",
      toStr(argNumber), L" argument", (argNumber == 1 ? L"" : L"s")), word.line);
}

static void functionArgException(const _int& argNumber, const _str& typeName, const Token& word, Uroboros* uro)
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

Generator<_list>* listFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro->hashes.HASH_FUNC_CHARACTERS || name == uro->hashes.HASH_FUNC_WORDS) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      if (name == uro->hashes.HASH_FUNC_CHARACTERS)
         return new F_Characters(str);
      else
         return new F_Words(str);
   }
   if (name == uro->hashes.HASH_FUNC_SPLIT) {
      if (len != 2) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str1;
      if (!parse(uro, args[0], str1)) {
         functionArgException(1, L"string", word, uro);
      }

      Generator<_str>* str2;
      if (!parse(uro, args[1], str2)) {
         delete str1;
         functionArgException(2, L"string", word, uro);
      }

      return new F_Split(str1, str2);
   }

   throw SyntaxException(str(L"function with name '", word.getOriginString(uro),
      L"' does not exist"), word.line);
}


Generator<_nlist>* numListFunction(const Tokens& tks, Uroboros* uro)
{
   const Token& word = tks.first();
   const _size& name = word.value.word.h;
   const std::vector<Tokens> args = toFunctionArgs(tks);
   const _size len = args.size();

   if (name == uro->hashes.HASH_FUNC_NUMBERS) {
      if (len != 1) {
         functionArgNumberException(len, word, uro);
      }

      Generator<_str>* str;
      if (!parse(uro, args[0], str)) {
         functionArgException(1, L"string", word, uro);
      }

      return new F_Numbers(str);
   }

   return nullptr;
}


static void checkFunctionAttribute(const Token& word, Uroboros* uro)
{
   if (!uro->vc.anyAttribute() || uro->vars.inner.thisState != ThisState::ts_String) {
      throw SyntaxException(str(L"function '", word.getOriginString(uro),
         L"' can be called only inside an iteration loop"), word.line);
   }
}

static void checkInOperatorCommaAmbiguity(const Token& word, const Tokens& tks, Uroboros* uro)
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

   Tokens right(tks);
   right.setRange(index + 1, tks.getLength() + start - index - 1);

   if (right.first().isSymbol(L'(')) {
      return;
   }

   const Token& inToken = tks.listAt(index);
   throw SyntaxException(str(L"the right side of the operator '", inToken.getOriginString(uro),
      L"' used in function '", word.getOriginString(uro),
      L"' should be embraced by brackets. Comma is a function argument separator and causes ambiguity here"),
      word.line);
}
