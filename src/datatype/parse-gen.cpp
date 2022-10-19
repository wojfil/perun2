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


namespace uro::parse
{

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_bool>*& result)
{
   Generator<_bool>* boo = parseBool(tks, uro);
   if (boo == nullptr) {
      return false;
   }
   else {
      result = boo;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_num>*& result)
{
   // cast from "bool" to "Number"
   Generator<_bool>* boo = parseBool(tks, uro);
   if (boo != nullptr) {
      result = new gen::Cast_B_N(boo);
      return true;
   }

   Generator<_num>* num = parseNumber(tks, uro);
   if (num == nullptr) {
      return false;
   }
   else {
      result = num;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_str>*& result)
{
   // cast from "bool" to "string"
   Generator<_bool>* boo = parseBool(tks, uro);
   if (boo != nullptr) {
      result = new gen::Cast_B_S(boo);
      return true;
   }

   // cast from "number" to "string"
   Generator<_num>* num = parseNumber(tks, uro);
   if (num != nullptr) {
      result = new gen::Cast_N_S(num);
      return true;
   }

   // cast from "time" to "string"
   Generator<_tim>* tim = parseTime(tks, uro);
   if (tim != nullptr) {
      result = new gen::Cast_T_S(tim);
      return true;
   }

   // cast from "period" to "string"
   Generator<_per>* per = parsePeriod(tks, uro);
   if (per != nullptr) {
      result = new gen::Cast_P_S(per);
      return true;
   }

   Generator<_str>* str = parseString(tks, uro);
   if (str == nullptr) {
      return false;
   }
   else {
      result = str;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_nlist>*& result)
{
   // cast from "bool" to "numList"
   Generator<_bool>* boo = parseBool(tks, uro);
   if (boo != nullptr) {
      result = new gen::Cast_B_NL(boo);
      return true;
   }

   // cast from "Number" to "numList"
   Generator<_num>* num = parseNumber(tks, uro);
   if (num != nullptr) {
      result = new gen::Cast_N_NL(num);
      return true;
   }

   Generator<_nlist>* nlist = parseNumList(tks, uro);
   if (nlist == nullptr) {
      return false;
   }
   else {
      result = nlist;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_tlist>*& result)
{
   // cast from "Time" to "timList"
   Generator<_tim>* tim = parseTime(tks, uro);
   if (tim != nullptr) {
      result = new gen::Cast_T_TL(tim);
      return true;
   }

   Generator<_tlist>* tlist = parseTimList(tks, uro);
   if (tlist == nullptr) {
      return false;
   }
   else {
      result = tlist;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_list>*& result)
{
   // cast from "bool" to "list"
   Generator<_bool>* boo = parseBool(tks, uro);
   if (boo != nullptr) {
      result = new gen::Cast_B_L(boo);
      return true;
   }

   // cast from "number" to "list"
   Generator<_num>* num = parseNumber(tks, uro);
   if (num != nullptr) {
      result = new gen::Cast_N_L(num);
      return true;
   }

   // cast from "time" to "list"
   Generator<_tim>* tim = parseTime(tks, uro);
   if (tim != nullptr) {
      result = new gen::Cast_T_L(tim);
      return true;
   }

   // cast from "period" to "list"
   Generator<_per>* per = parsePeriod(tks, uro);
   if (per != nullptr) {
      result = new gen::Cast_P_L(per);
      return true;
   }

   // cast from "numList" to "list"
   Generator<_nlist>* nlis = parseNumList(tks, uro);
   if (nlis != nullptr) {
      result = new gen::Cast_NL_L(nlis);
      return true;
   }

   // cast from "timList" to "list"
   Generator<_tlist>* tlis = parseTimList(tks, uro);
   if (tlis != nullptr) {
      result = new gen::Cast_TL_L(tlis);
      return true;
   }

   // cast from "string" to "list"
   Generator<_str>* str = parseString(tks, uro);
   if (str != nullptr) {
      result = new gen::Cast_S_L(str);
      return true;
   }

   // cast from "definition" to "list"
   _def* def = parseDefinition(tks, uro);
   if (def != nullptr) {
      result = new gen::Cast_D_L(def, uro);
      return true;
   }

   Generator<_list>* list = parseList(tks, uro);
   if (list == nullptr) {
      return false;
   }
   else {
      result = list;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_tim>*& result)
{
   Generator<_tim>* tim = parseTime(tks, uro);
   if (tim == nullptr) {
      return false;
   }
   else {
      result = tim;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, Generator<_per>*& result)
{
   Generator<_per>* per = parsePeriod(tks, uro);
   if (per == nullptr) {
      return false;
   }
   else {
      result = per;
      return true;
   }
}

_bool parse(Uroboros& uro, const Tokens& tks, _def*& result)
{
   _def* def = parseDefinition(tks, uro);
   if (def == nullptr) {
      return false;
   }
   else {
      result = def;
      return true;
   }
}

}
