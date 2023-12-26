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

#include "parse-gen.h"
#include "parse/parse-numlist.h"
#include "parse/parse-timlist.h"
#include "../lexer.h"
#include "../brackets.h"
#include "parse/parse-function.h"


namespace perun2::parse
{

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_bool>& result)
{
   return parseBool(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_num>& result)
{
   // cast from "bool" to "Number"
   p_genptr<p_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_N>(boo);
      return true;
   }

   return parseNumber(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_str>& result)
{
   // cast from "bool" to "string"
   p_genptr<p_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_S>(boo);
      return true;
   }

   // cast from "number" to "string"
   p_genptr<p_num> num;
   if (parseNumber(num, tks, p2)) {
      result = std::make_unique<gen::Cast_N_S>(num);
      return true;
   }

   // cast from "time" to "string"
   p_genptr<p_tim> tim;
   if (parseTime(tim, tks, p2)) {
      result = std::make_unique<gen::Cast_T_S>(tim);
      return true;
   }

   // cast from "period" to "string"
   p_genptr<p_per> per;
   if (parsePeriod(per, tks, p2)) {
      result = std::make_unique<gen::Cast_P_S>(per);
      return true;
   }

   return parseString(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_nlist>& result)
{
   // cast from "bool" to "numList"
   p_genptr<p_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_NL>(boo);
      return true;
   }

   // cast from "Number" to "numList"
   p_genptr<p_num> num;
   if (parseNumber(num, tks, p2)) {
      result = std::make_unique<gen::Cast_N_NL>(num);
      return true;
   }

   return parseNumList(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_tlist>& result)
{
   // cast from "Time" to "timList"
   p_genptr<p_tim> tim;
   if (parseTime(tim, tks, p2)) {
      result = std::make_unique<gen::Cast_T_TL>(tim);
      return true;
   }

   return parseTimList(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_list>& result)
{
   // cast from "bool" to "list"
   p_genptr<p_bool> boo;
   if (parseBool(boo, tks, p2)) {
      result = std::make_unique<gen::Cast_B_L>(boo);
      return true;
   }

   // cast from "number" to "list"
   p_genptr<p_num> num;
   if (parseNumber(num, tks, p2)) {
      result = std::make_unique<gen::Cast_N_L>(num);
      return true;
   }

   // cast from "time" to "list"
   p_genptr<p_tim> tim;
   if (parseTime(tim, tks, p2)) {
      result = std::make_unique<gen::Cast_T_L>(tim);
      return true;
   }

   // cast from "period" to "list"
   p_genptr<p_per> per;
   if (parsePeriod(per, tks, p2)) {
      result = std::make_unique<gen::Cast_P_L>(per);
      return true;
   }

   // cast from "numList" to "list"
   p_genptr<p_nlist> nlis;
   if (parseNumList(nlis, tks, p2)) {
      result = std::make_unique<gen::Cast_NL_L>(nlis);
      return true;
   }

   // cast from "timList" to "list"
   p_genptr<p_tlist> tlis;
   if (parseTimList(tlis, tks, p2)) {
      result = std::make_unique<gen::Cast_TL_L>(tlis);
      return true;
   }

   // cast from "string" to "list"
   p_genptr<p_str> str;
   if (parseString(str, tks, p2)) {
      result = std::make_unique<gen::Cast_S_L>(str);
      return true;
   }

   // cast from "definition" to "list"
   p_defptr def;
   if (parseDefinition(def, tks, p2)) {
      result = std::make_unique<gen::Cast_D_L>(def, p2);
      return true;
   }

   return parseList(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_tim>& result)
{
   return parseTime(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_genptr<p_per>& result)
{
   return parsePeriod(result, tks, p2);
}

p_bool parse(p_perun2& p2, const Tokens& tks, p_defptr& result)
{
   return parseDefinition(result, tks, p2);
}

}
