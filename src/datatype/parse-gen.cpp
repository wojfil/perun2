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

#include "parse-gen.h"
#include "parse/parse-numlist.h"
#include "parse/parse-timlist.h"
#include "../lexer.h"
#include "../brackets.h"
#include "../hash.h"
#include "function.h"


void negationByExclamationException(const _int& line)
{
   throw SyntaxException(L"you should use keyword 'not' instead of character '!' for boolean negation", line);
}

Tokens prepareForGen(const Tokens& tks, Uroboros* uro)
{
   // check if all opened brackets are closed within the sequence
   checkBrackets(tks);

   // look for a special kind of syntax errors: forbidden keywords
   checkKeywords(tks, uro);

   // create a modificable copy of the sequence
   Tokens tks2(tks);

   // trim encircling brackets as long as
   // they are here and the sequence is long enough
   while (tks2.getLength() >= 2) {
      const Token& f = tks2.first();
      const Token& l = tks2.last();

      if (f.type == Token::t_Symbol && f.type == Token::t_Symbol
         && f.value.ch == L'(' && l.value.ch == L')')
      {
         _int lvl = 0;
         _boo b = true;
         const _int end = tks2.getEnd();

         for (_int i = tks2.getStart(); b && i <= end; i++) {
            const Token& t = tks2.listAt(i);
            if (t.type == Token::t_Symbol) {
               switch (t.value.ch) {
                  case L'(': {
                     lvl++;
                     break;
                  }
                  case L')': {
                     lvl--;
                     if (lvl == 0 && i != tks2.getEnd())
                     {
                        b = false;
                     }
                     break;
                  }
               }
            }
         }

         if (b) {
            tks2.trimBoth();
         }
         else {
            break;
         }
      }
      else {
         break;
      }
   }

   // look for a common error - somebody calls a non-existent variable
   if (tks2.getLength() == 1) {
      const Token& f = tks2.first();
      if (f.type == Token::t_Word) {
         if (f.value.word.h == uro->hashes.HASH_VAR_DEVICE) {
            throw SyntaxException(str(L"variable '", f.getOriginString(uro),
               L"' is reserved for future use. Current version of Uroboros does not support it"), f.line);
         }

         if (f.value.word.h != uro->hashes.HASH_VAR_THIS && !uro->vars.variableExists(f)) {
            throw SyntaxException(str(L"variable '", f.getOriginString(uro),
               L"' does not exist or is unreachable here"), f.line);
         }
      }
   }

   // look for adjacent ++ and -- inside an expression
   const _int start = tks2.getStart();
   const _int end = tks2.getEnd() - 1;
   _boo prevExclamantion = false;

   if (tks2.first().isSymbol(L'!')) {
      negationByExclamationException(tks2.first().line);
   }

   for (_int i = start; i <= end; i++) {
      const Token& t = tks2.listAt(i);

      if (prevExclamantion && !t.isSymbol(L'=')) {
         negationByExclamationException(tks2.listAt(i - 1).line);
      }

      if (t.type == Token::t_MultiSymbol) {
         switch (t.value.chars.ch) {
            case L'+': {
               if (i == start) {
                  throw SyntaxException(
                     L"expression cannot start with incrementation signs ++", t.line);
               }
               else {
                  throw SyntaxException(
                     L"incrementation signs ++ cannot appear inside an expression", t.line);
               }
               break;
            }
            case L'-': {
               if (i == start) {
                  throw SyntaxException(
                     L"expression cannot start with decrementation signs --", t.line);
               }
               else {
                  throw SyntaxException(
                     L"decrementation signs -- cannot appear inside an expression", t.line);
               }
               break;
            }
         }
      }

      prevExclamantion = t.isSymbol(L'!');
   }

   return tks2;
}

void checkKeywords(const Tokens& tks, Uroboros* uro)
{
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (t.type == Token::t_Keyword && isExpForbiddenKeyword(t)) {
         throw SyntaxException(str(L"expected ; before keyword '", t.getOriginString(uro), L"'"), t.line);
      }
   }
}

_boo isExpForbiddenKeyword(const Token& tk)
{
   if (tk.isCommandKeyword()) {
      return true;
   }
   switch (tk.value.keyword.k) {
      case Keyword::kw_Force:
      case Keyword::kw_Stack:
         return true;
      default:
         return false;
   }
}


_boo parse(Uroboros* uro, const Tokens& tns, Generator<_boo>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // data type "bool" does not contain any default casting
   // so let us go straight to parsing

   Generator<_boo>* boo = parseBool(tns2, uro);
   if (boo == nullptr) {
      return false;
   }
   else {
      result = boo;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_num>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // cast from "bool" to "Number"
   Generator<_boo>* boo = parseBool(tns2, uro);
   if (boo != nullptr) {
      result = new Cast_B_N(boo);
      return true;
   }

   Generator<_num>* num = parseNumber(tns2, uro);
   if (num == nullptr) {
      return false;
   }
   else {
      result = num;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_str>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // cast from "bool" to "string"
   Generator<_boo>* boo = parseBool(tns2, uro);
   if (boo != nullptr) {
      result = new Cast_B_S(boo);
      return true;
   }

   // cast from "number" to "string"
   Generator<_num>* num = parseNumber(tns2, uro);
   if (num != nullptr) {
      result = new Cast_N_S(num);
      return true;
   }

   // cast from "time" to "string"
   Generator<_tim>* tim = parseTime(tns2, uro);
   if (tim != nullptr) {
      result = new Cast_T_S(tim);
      return true;
   }

   // cast from "period" to "string"
   Generator<_per>* per = parsePeriod(tns2, uro);
   if (per != nullptr) {
      result = new Cast_P_S(per);
      return true;
   }

   Generator<_str>* str = parseString(tns2, uro);
   if (str == nullptr) {
      return false;
   }
   else {
      result = str;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_nlist>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // cast from "bool" to "numList"
   Generator<_boo>* boo = parseBool(tns2, uro);
   if (boo != nullptr) {
      result = new Cast_B_NL(boo);
      return true;
   }

   // cast from "Number" to "numList"
   Generator<_num>* num = parseNumber(tns2, uro);
   if (num != nullptr) {
      result = new Cast_N_NL(num);
      return true;
   }

   Generator<_nlist>* nlist = parseNumList(tns2, uro);
   if (nlist == nullptr) {
      return false;
   }
   else {
      result = nlist;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_tlist>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // cast from "Time" to "timList"
   Generator<_tim>* tim = parseTime(tns2, uro);
   if (tim != nullptr) {
      result = new Cast_T_TL(tim);
      return true;
   }

   Generator<_tlist>* tlist = parseTimList(tns2, uro);
   if (tlist == nullptr) {
      return false;
   }
   else {
      result = tlist;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_list>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // cast from "bool" to "list"
   Generator<_boo>* boo = parseBool(tns2, uro);
   if (boo != nullptr) {
      result = new Cast_B_L(boo);
      return true;
   }

   // cast from "number" to "list"
   Generator<_num>* num = parseNumber(tns2, uro);
   if (num != nullptr) {
      result = new Cast_N_L(num);
      return true;
   }

   // cast from "time" to "list"
   Generator<_tim>* tim = parseTime(tns2, uro);
   if (tim != nullptr) {
      result = new Cast_T_L(tim);
      return true;
   }

   // cast from "period" to "list"
   Generator<_per>* per = parsePeriod(tns2, uro);
   if (per != nullptr) {
      result = new Cast_P_L(per);
      return true;
   }

   // cast from "numList" to "list"
   Generator<_nlist>* nlis = parseNumList(tns2, uro);
   if (nlis != nullptr) {
      result = new Cast_NL_L(nlis);
      return true;
   }

   // cast from "timList" to "list"
   Generator<_tlist>* tlis = parseTimList(tns2, uro);
   if (tlis != nullptr) {
      result = new Cast_TL_L(tlis);
      return true;
   }

   // cast from "string" to "list"
   Generator<_str>* str = parseString(tns2, uro);
   if (str != nullptr) {
      result = new Cast_S_L(str);
      return true;
   }

   // cast from "definition" to "list"
   _def* def = parseDefinition(tns2, uro);
   if (def != nullptr) {
      result = new Cast_D_L(def, uro);
      return true;
   }

   Generator<_list>* list = parseList(tns2, uro);
   if (list == nullptr) {
      return false;
   }
   else {
      result = list;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_tim>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // data type "time" does not contain any default casting
   // so let us go straight to parsing

   Generator<_tim>* tim = parseTime(tns2, uro);
   if (tim == nullptr) {
      return false;
   }
   else {
      result = tim;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_per>*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // data type "period" does not contain any default casting
   // so let us go straight to parsing

   Generator<_per>* per = parsePeriod(tns2, uro);
   if (per == nullptr) {
      return false;
   }
   else {
      result = per;
      return true;
   }
}

_boo parse(Uroboros* uro, const Tokens& tns, _def*& result)
{
   const Tokens tns2 = prepareForGen(tns, uro);

   // data type "period" does not contain any default casting
   // so let us go straight to parsing

   _def* def = parseDefinition(tns2, uro);
   if (def == nullptr) {
      return false;
   }
   else {
      result = def;
      return true;
   }
}
