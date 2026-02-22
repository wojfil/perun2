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

#include "../../../include/perun2/datatype/parse/parse-string.hpp"
#include "../../../include/perun2/datatype/parse/parse-generic.hpp"
#include "../../../include/perun2/datatype/parse/parse-var.hpp"
#include "../../../include/perun2/datatype/cast.hpp"
#include "../../../include/perun2/datatype/parse/parse-function.hpp"
#include "../../../include/perun2/datatype/generator/gen-string.hpp"
#include "../../../include/perun2/datatype/generator/gen-number.hpp"
#include "../../../include/perun2/datatype/generator/gen-time.hpp"
#include "../../../include/perun2/datatype/generator/gen-period.hpp"
#include "../../../include/perun2/datatype/generator/gen-generic.hpp"
#include "../../../include/perun2/datatype/parse-gen.hpp"


namespace perun2::parse
{

p_bool parseString(p_genptr<p_str>& result, const Tokens& tks, Perun2Process& p2)
{
   const p_size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return false;
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::stringFunction(result, tks, p2);
   }
   else if (tks.check(TI_HAS_CHAR_PLUS)) {
      p_genptr<p_str> str;
      if (parseStringConcat(str, tks, p2)) {
         result = std::move(str);
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_LIST_ELEM)) {
      p_genptr<p_num> num;
      parseListElementIndex(num, tks, p2);
      const Token& f = tks.first();
      p_genptr<p_list> list;

      if (makeVarRef(f, list, p2)) {
         result = std::make_unique<gen::ListElement<p_str>>(list, num);
         return true;
      }
      else {
         p_genptr<p_str> str;

         if (makeVarRef(f, str, p2)) {
            result = std::make_unique<gen::CharAtIndex>(str, num);
            return true;
         }
         else {
            p_defptr def;

            if (makeVarRef(f, def, p2)) {
               result = std::make_unique<gen::DefinitionElement>(def, num, p2);
               return true;
            }
         }
      }
   }

   return parseBinary<p_str>(result, tks, p2) || parseTernary<p_str>(result, tks, p2);
}

template <typename T>
void concatParseOutcome(p_bool& parsed, p_bool& allConstants, p_genptr<T>& recentValue)
{
   parsed = true;
   allConstants &= recentValue->isConstant();
}

// parse string concatenation (by the + separator)
// if adjacent elements are numbers or periods, sum them
// if a time is followed by a period, then shift the time
// all these elements are casted into strings finally
p_bool parseStringConcat(p_genptr<p_str>& res, const Tokens& tks, Perun2Process& p2)
{
   enum PrevType {
      pt_String = 0,
      pt_Number,
      pt_Time,
      pt_Period
   };

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_PLUS);

   PrevType prevType = pt_String;
   p_genptr<p_num> prevNum;
   p_genptr<p_tim> prevTim;
   p_genptr<p_per> prevPer;

   p_bool allConstants = true;
   std::vector<p_genptr<p_str>> result;

   for (const Tokens& tks2 : elements) {
      p_bool parsed = false;

      switch (prevType) {
         case pt_String: {
            if (parse(p2, tks2, prevNum)) {
               prevType = pt_Number;
               concatParseOutcome(parsed, allConstants, prevNum);
            }
            else {
               if (parse(p2, tks2, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
               else if (parse(p2, tks2, prevPer)) {
                  prevType = pt_Period;
                  concatParseOutcome(parsed, allConstants, prevPer);
               }
            }
            break;
         }
         case pt_Number: {
            p_genptr<p_num> num;
            if (parse(p2, tks2, num)) {
               p_genptr<p_num> pn = std::move(prevNum);
               concatParseOutcome(parsed, allConstants, num);
               prevNum = std::make_unique<gen::Addition>(pn, num);
            }
            else {
               result.push_back(std::make_unique<gen::Cast_N_S>(prevNum));
               if (parse(p2, tks2, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
               else if (parse(p2, tks2, prevPer)) {
                  prevType = pt_Period;
                  concatParseOutcome(parsed, allConstants, prevPer);
               }
            }
            break;
         }
         case pt_Time: {
            p_genptr<p_per> per;
            if (parse(p2, tks2, per)) {
               p_genptr<p_tim> pt = std::move(prevTim);
               concatParseOutcome(parsed, allConstants, per);
               prevTim = std::make_unique<gen::IncreasedTime>(pt, per);
            }
            else {
               result.push_back(std::make_unique<gen::Cast_T_S>(prevTim));
               if (parse(p2, tks2, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
               else if (parse(p2, tks2, prevNum)) {
                  prevType = pt_Number;
                  concatParseOutcome(parsed, allConstants, prevNum);
               }
            }
            break;
         }
         case pt_Period: {
            p_genptr<p_per> per;
            if (parse(p2, tks2, per)) {
               p_genptr<p_per> pp = std::move(prevPer);
               concatParseOutcome(parsed, allConstants, per);
               prevPer = std::make_unique<gen::PeriodAddition>(pp, per);
            }
            else {
               result.push_back(std::make_unique<gen::Cast_P_S>(prevPer));
               if (parse(p2, tks2, prevNum)) {
                  prevType = pt_Number;
                  concatParseOutcome(parsed, allConstants, prevNum);
               }
               else if (parse(p2, tks2, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
            }
            break;
         }
      }

      if (!parsed) {
         prevType = pt_String;
         p_genptr<p_str> str;

         if (parse(p2, tks2, str)) {
            allConstants &= str->isConstant();
            result.push_back(std::move(str));
         }
         else {
            return false;
         }
      }
   }

   switch (prevType) {
      case pt_Number: {
         result.push_back(std::make_unique<gen::Cast_N_S>(prevNum));
         break;
      }
      case pt_Time: {
         result.push_back(std::make_unique<gen::Cast_T_S>(prevTim));
         break;
      }
      case pt_Period: {
         result.push_back(std::make_unique<gen::Cast_P_S>(prevPer));
         break;
      }
   }

   p_genptr<p_str> concat;

   switch (result.size()) {
      case 1: {
         concat = std::move(result[0]);
         break;
      }
      case 2: {
         concat = std::make_unique<gen::ConcatString_2>(result[0], result[1]);
         break;
      }
      case 3: {
         concat = std::make_unique<gen::ConcatString_3>(result[0], result[1], result[2]);
         break;
      }
      case 4: {
         concat = std::make_unique<gen::ConcatString_4>(result[0], result[1], result[2], result[3]);
         break;
      }
      case 5: {
         concat = std::make_unique<gen::ConcatString_5>(result[0], result[1], result[2], result[3], result[4]);
         break;
      }
      case 6: {
         concat = std::make_unique<gen::ConcatString_6>(result[0], result[1], result[2], result[3], result[4], result[5]);
         break;
      }
      default: {
         concat = std::make_unique<gen::ConcatString_Multi>(result);
         break;
      }
   }

   if (allConstants) {
      // if all units of string concatenation are constant values
      // just transform the whole structure into one constant
      const p_str cnst = concat->getValue();
      res = std::make_unique<gen::Constant<p_str>>(cnst);
   }
   else {
      res = std::move(concat);
   }

   return true;
}

}
