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


Generator<_str>* parseString(const Tokens& tks, Uroboros* uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      Generator<_str>* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }

   if (tks.isPossibleFunction()) {
      Generator<_str>* func = stringFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }
   else if (tks.containsSymbol(PG_CHAR_PLUS)) {
      Generator<_str>* str = parseStringConcat(tks, uro);
      if (str != nullptr) {
         return str;
      }
   }

   if (isPossibleListElement(tks)) {
      Generator<_num>* num = parseListElementIndex(tks, uro);
      const Token& f = tks.first();
      Generator<_list>* list;

      if (uro->vars.getVarValue(f, list)) {
         return new ListElement<_str>(list, num);
      }
      else {
         Generator<_str>* str;

         if (uro->vars.getVarValue(f, str)) {
            return new CharAtIndex(str, num);
         }
         else {
            _def* def;

            if (uro->vars.getVarValue(f, def)) {
               return new DefinitionElement(def, num);
            }
            else {
               delete num;
            }
         }
      }
   }

   Generator<_str>* bin = parseBinary<_str>(tks, uro);
   if (bin != nullptr) {
      return bin;
   }

   Generator<_str>* tern = parseTernary<_str>(tks, uro);
   if (tern != nullptr) {
      return tern;
   }

   return nullptr;
}

template <typename T>
void concatParseOutcome(_boo& parsed, _boo& allConstants, Generator<T>* recentValue)
{
   parsed = true;
   allConstants &= recentValue->isConstant();
}

// parse string cocatenation (by + is separator)
// if adjacent elements are numbers or periods, sum them
// if a time is followed by a period, then shift the time
// all these elements are casted into strings finally
Generator<_str>* parseStringConcat(const Tokens& tks, Uroboros* uro)
{
   enum PrevType {
      pt_String = 0,
      pt_Number,
      pt_Time,
      pt_Period
   };

   std::vector<Tokens> elements;
   tks.splitBySymbol(L'+', elements);

   PrevType prevType = pt_String;
   Generator<_num>* prevNum = nullptr;
   Generator<_tim>* prevTim = nullptr;
   Generator<_per>* prevPer = nullptr;

   const _size len = elements.size();
   _boo allConstants = true;
   std::vector<Generator<_str>*>* result = new std::vector<Generator<_str>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _boo parsed = false;

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
            Generator<_num>* num;
            if (parse(uro, tks, num)) {
               Generator<_num>* add = new Addition(prevNum, num);
               prevNum = add;
               concatParseOutcome(parsed, allConstants, num);
            }
            else {
               result->push_back(new Cast_N_S(prevNum));
               prevNum = nullptr;
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
            Generator<_per>* per;
            if (parse(uro, tks, per)) {
               Generator<_tim>* incr = new IncreasedTime(prevTim, per);
               prevTim = incr;
               concatParseOutcome(parsed, allConstants, per);
            }
            else {
               result->push_back(new Cast_T_S(prevTim));
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
            Generator<_per>* per;
            if (parse(uro, tks, per)) {
               Generator<_per>* add = new PeriodAddition(prevPer, per);
               prevPer = add;
               concatParseOutcome(parsed, allConstants, per);
            }
            else {
               result->push_back(new Cast_P_S(prevPer));
               prevPer = nullptr;
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
         Generator<_str>* str;

         if (parse(uro, tks, str)) {
            result->push_back(str);
            allConstants &= str->isConstant();
         }
         else {
            // parsing has failed
            // so free memory
            deleteVectorPtr(result);

            switch (prevType) {
               case pt_Number: {
                  delete prevNum;
                  break;
               }
               case pt_Time: {
                  delete prevTim;
                  break;
               }
               case pt_Period: {
                  delete prevPer;
                  break;
               }
            }

            return nullptr;
         }
      }
   }

   switch (prevType) {
      case pt_Number: {
         result->push_back(new Cast_N_S(prevNum));
         break;
      }
      case pt_Time: {
         result->push_back(new Cast_T_S(prevTim));
         break;
      }
      case pt_Period: {
         result->push_back(new Cast_P_S(prevPer));
         break;
      }
   }

   Generator<_str>* concat = new ConcatString(result);

   if (allConstants) {
      // if all units of string concatenation are constant
      // just transform the whole structure into one constant value
      const _str cnst = concat->getValue();
      delete concat;
      return new Constant<_str>(cnst);
   }
   else {
      return concat;
   }
}
