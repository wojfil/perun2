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

#include "parse-gen.h"
#include "parse/parse-numlist.h"
#include "parse/parse-timlist.h"
#include "../lexer.h"
#include "../brackets.h"
#include "parse/parse-function.h"


namespace perun2::parse
{

_bool parse(_p2& p2, const Tokens& tks, _genptr<_bool>& result)
{
   return parseBool(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_num>& result)
{
   // cast from "bool" to "Number"
   _genptr<_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_N>(boo);
      return true;
   }

   return parseNumber(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_str>& result)
{
   // cast from "bool" to "string"
   _genptr<_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_S>(boo);
      return true;
   }

   // cast from "number" to "string"
   _genptr<_num> num;
   if (parseNumber(num, tks, p2)) {
      result = std::make_unique<gen::Cast_N_S>(num);
      return true;
   }

   // cast from "time" to "string"
   _genptr<_tim> tim;
   if (parseTime(tim, tks, p2)) {
      result = std::make_unique<gen::Cast_T_S>(tim);
      return true;
   }

   // cast from "period" to "string"
   _genptr<_per> per;
   if (parsePeriod(per, tks, p2)) {
      result = std::make_unique<gen::Cast_P_S>(per);
      return true;
   }

   return parseString(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_nlist>& result)
{
   // cast from "bool" to "numList"
   _genptr<_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_NL>(boo);
      return true;
   }

   // cast from "Number" to "numList"
   _genptr<_num> num;
   if (parseNumber(num, tks, p2)) {
      result = std::make_unique<gen::Cast_N_NL>(num);
      return true;
   }

   return parseNumList(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_tlist>& result)
{
   // cast from "Time" to "timList"
   _genptr<_tim> tim;
   if (parseTime(tim, tks, p2)) {
      result = std::make_unique<gen::Cast_T_TL>(tim);
      return true;
   }

   return parseTimList(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_list>& result)
{
   // cast from "bool" to "list"
   _genptr<_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_L>(boo);
      return true;
   }

   // cast from "number" to "list"
   _genptr<_num> num;
   if (parseNumber(num, tks, p2)) {
      result = std::make_unique<gen::Cast_N_L>(num);
      return true;
   }

   // cast from "time" to "list"
   _genptr<_tim> tim;
   if (parseTime(tim, tks, p2)) {
      result = std::make_unique<gen::Cast_T_L>(tim);
      return true;
   }

   // cast from "period" to "list"
   _genptr<_per> per;
   if (parsePeriod(per, tks, p2)) {
      result = std::make_unique<gen::Cast_P_L>(per);
      return true;
   }

   // cast from "numList" to "list"
   _genptr<_nlist> nlis;
   if (parseNumList(nlis, tks, p2)) {
      result = std::make_unique<gen::Cast_NL_L>(nlis);
      return true;
   }

   // cast from "timList" to "list"
   _genptr<_tlist> tlis;
   if (parseTimList(tlis, tks, p2)) {
      result = std::make_unique<gen::Cast_TL_L>(tlis);
      return true;
   }

   // cast from "string" to "list"
   _genptr<_str> str;
   if (parseString(str, tks, p2)) {
      result = std::make_unique<gen::Cast_S_L>(str);
      return true;
   }

   // cast from "definition" to "list"
   _defptr def;
   if (parseDefinition(def, tks, p2)) {
      result = std::make_unique<gen::Cast_D_L>(def, p2);
      return true;
   }

   return parseList(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_tim>& result)
{
   return parseTime(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _genptr<_per>& result)
{
   return parsePeriod(result, tks, p2);
}

_bool parse(_p2& p2, const Tokens& tks, _defptr& result)
{
   return parseDefinition(result, tks, p2);
}

}
