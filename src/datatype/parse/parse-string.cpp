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

#include "parse-string.h"
#include "parse-generic.h"
#include "../cast.h"
#include "../function.h"
#include "../generator/gen-string.h"
#include "../generator/gen-number.h"
#include "../generator/gen-time.h"
#include "../generator/gen-period.h"
#include "../generator/gen-generic.h"
#include "../parse-gen.h"


namespace uro::parse
{

_bool parseString(_genptr<_str>& result, const Tokens& tks, Uroboros& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return false;
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::stringFunction(result, tks, uro);
   }
   else if (tks.check(TI_HAS_CHAR_PLUS)) {
      _genptr<_str> str;
      if (parseStringConcat(str, tks, uro)) {
         result = std::move(str);
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_LIST_ELEM)) {
      _genptr<_num> num;
      parseListElementIndex(num, tks, uro);
      const Token& f = tks.first();
      _genptr<_list> list;

      if (uro.vars.getVarValue(f, list)) {
         result = std::make_unique<gen::ListElement<_str>>(list, num);
         return true;
      }
      else {
         _genptr<_str> str;

         if (uro.vars.getVarValue(f, str)) {
            result = std::make_unique<gen::CharAtIndex>(str, num);
            return true;
         }
         else {
            _defptr def;

            if (uro.vars.getVarValue(f, def)) {
               result = std::make_unique<gen::DefinitionElement>(def, num);
               return true;
            }
         }
      }
   }

   if (parseBinary<_str>(result, tks, uro) || parseTernary<_str>(result, tks, uro)) {
      return true;
   }

   return false;
}

template <typename T>
void concatParseOutcome(_bool& parsed, _bool& allConstants, _genptr<T>& recentValue)
{
   parsed = true;
   allConstants &= recentValue->isConstant();
}

// parse string concatenation (by the + separator)
// if adjacent elements are numbers or periods, sum them
// if a time is followed by a period, then shift the time
// all these elements are casted into strings finally
_bool parseStringConcat(_genptr<_str>& res, const Tokens& tks, Uroboros& uro)
{
   enum PrevType {
      pt_String = 0,
      pt_Number,
      pt_Time,
      pt_Period
   };

   const std::vector<Tokens> elements = tks.splitBySymbol(L'+');

   PrevType prevType = pt_String;
   _genptr<_num> prevNum;
   _genptr<_tim> prevTim;
   _genptr<_per> prevPer;

   const _size len = elements.size();
   _bool allConstants = true;
   std::vector<_genptr<_str>> result;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _bool parsed = false;

      switch (prevType) {
         case pt_String: {
            if (parse(uro, tks, prevNum)) {
               prevType = pt_Number;
               concatParseOutcome(parsed, allConstants, prevNum);
            }
            else {
               if (parse(uro, tks, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
               else if (parse(uro, tks, prevPer)) {
                  prevType = pt_Period;
                  concatParseOutcome(parsed, allConstants, prevPer);
               }
            }
            break;
         }
         case pt_Number: {
            _genptr<_num> num;
            if (parse(uro, tks, num)) {
               _genptr<_num> pn = std::move(prevNum);
               prevNum = std::make_unique<gen::Addition>(pn, num);
               concatParseOutcome(parsed, allConstants, num);
            }
            else {
               result.push_back(std::make_unique<gen::Cast_N_S>(prevNum));
               if (parse(uro, tks, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
               else if (parse(uro, tks, prevPer)) {
                  prevType = pt_Period;
                  concatParseOutcome(parsed, allConstants, prevPer);
               }
            }
            break;
         }
         case pt_Time: {
            _genptr<_per> per;
            if (parse(uro, tks, per)) {
               _genptr<_tim> pt = std::move(prevTim);
               prevTim = std::make_unique<gen::IncreasedTime>(prevTim, per);
               concatParseOutcome(parsed, allConstants, per);
            }
            else {
               result.push_back(std::make_unique<gen::Cast_T_S>(prevTim));
               if (parse(uro, tks, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
               else if (parse(uro, tks, prevNum)) {
                  prevType = pt_Number;
                  concatParseOutcome(parsed, allConstants, prevNum);
               }
            }
            break;
         }
         case pt_Period: {
            _genptr<_per> per;
            if (parse(uro, tks, per)) {
               _genptr<_per> pp = std::move(prevPer);
               prevPer = std::make_unique<gen::PeriodAddition>(pp, per);
               concatParseOutcome(parsed, allConstants, per);
            }
            else {
               result.push_back(std::make_unique<gen::Cast_P_S>(prevPer));
               if (parse(uro, tks, prevNum)) {
                  prevType = pt_Number;
                  concatParseOutcome(parsed, allConstants, prevNum);
               }
               else if (parse(uro, tks, prevTim)) {
                  prevType = pt_Time;
                  concatParseOutcome(parsed, allConstants, prevTim);
               }
            }
            break;
         }
      }

      if (!parsed) {
         prevType = pt_String;
         _genptr<_str> str;

         if (parse(uro, tks, str)) {
            result.push_back(std::move(str));
            allConstants &= str->isConstant();
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

   _genptr<_str> concat(new gen::ConcatString(result));

   if (allConstants) {
      // if all units of string concatenation are constant values
      // just transform the whole structure into one constant
      const _str cnst = concat->getValue();
      res = std::make_unique<gen::Constant<_str>>(cnst);
   }
   else {
      res = std::move(concat);
   }

   return true;
}

}
