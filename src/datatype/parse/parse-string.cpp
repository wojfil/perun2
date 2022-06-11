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
#include "../cast.h"
#include "../function.h"
#include "../generator/gen-string.h"
#include "../generator/gen-number.h"
#include "../generator/gen-time.h"
#include "../generator/gen-period.h"
#include "../generator/gen-generic.h"


Generator<_str>* parseString(const Tokens& tks)
{
   const _size len = tks.getLength();
   if (len == 1) {
      const Token& f = tks.first();

      switch(f.type) {
         case Token::t_Quotation: {
            return new Constant<_str>(f.value.sl);
         }
         case Token::t_Word: {
            return getVarValueStr(f);
         }
         default: {
            return nullptr;
         }
      }
   }

   if (isPossibleFunction(tks)) {
      Generator<_str>* func = stringFunction(tks);
      if (func != nullptr) {
         return func;
      }
   }
   else if (tks.containsSymbol(L'+')) {
      Generator<_str>* str = parseStringConcat(tks);
      if (str != nullptr) {
         return str;
      }
   }


   if (isPossibleListElement(tks)) {
      Generator<_num>* num = parseListElementIndex(tks);
      const Token& f = tks.first();
      Generator<_list>* list = getVarValueList(f);
      if (list == nullptr) {
         Generator<_str>* str = getVarValueStr(f);
         if (str == nullptr) {
            _def* def = getVarValueDef(f);
            if (def == nullptr) {
               delete num;
            }
            else {
               return new DefinitionElement(def, num);
            }
         }
         else {
            return new CharAtIndex(str, num);
         }
      }
      else {
         return new ListElement<_str>(list, num);
      }
   }

   if (isPossibleBinary(tks)) {
      Generator<_str>* bin = parseStringBinary(tks);
      if (bin != nullptr) {
         return bin;
      }
   }

   if (isPossibleTernary(tks)) {
      Generator<_str>* tern = parseStringTernary(tks);
      if (tern != nullptr) {
         return tern;
      }
   }

   return nullptr;
}


static Generator<_str>* parseStringBinary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   tks.divideBySymbol(L'?', tks1, tks2);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_str>* value = stringGenerator(tks2);
   if (value == nullptr) {
      delete condition;
      return nullptr;
   }

   return new Binary<_str>(condition, value);
}

static Generator<_str>* parseStringTernary(const Tokens& tks)
{
   Tokens tks1(tks);
   Tokens tks2(tks);
   Tokens tks3(tks);
   tks.divideForTernary(tks1, tks2, tks3);

   Generator<_boo>* condition = boolGenerator(tks1);
   if (condition == nullptr) {
      return nullptr;
   }

   Generator<_str>* left = stringGenerator(tks2);
   if (left == nullptr) {
      delete condition;
      return nullptr;
   }

   Generator<_str>* right = stringGenerator(tks3);
   if (right == nullptr) {
      delete condition;
      delete left;
      return nullptr;
   }

   return new Ternary<_str>(condition, left, right);
}

Generator<_str>* parseStringConcat(const Tokens& tks)
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
   std::vector<Generator<_str>*>* result =
      new std::vector<Generator<_str>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _boo parsed = false;

      switch (prevType) {
         case pt_String: {
            prevNum = numberGenerator(tks);
            if (prevNum != nullptr) {
               prevType = pt_Number;
               parsed = true;
            }
            else {
               prevTim = timeGenerator(tks);
               if (prevTim != nullptr) {
                  prevType = pt_Time;
                  parsed = true;
               }
               else {
                  prevPer = periodGenerator(tks);
                  if (prevPer != nullptr) {
                     prevType = pt_Period;
                     parsed = true;
                  }
               }
            }
            break;
         }
         case pt_Number: {
            Generator<_num>* num = numberGenerator(tks);
            if (num != nullptr) {
               Generator<_num>* add = new Addition(prevNum, num);
               prevNum = add;
               parsed = true;
            }
            else {
               result->push_back(new Cast_N_S(prevNum));
               prevNum = nullptr;
               prevTim = timeGenerator(tks);
               if (prevTim != nullptr) {
                  prevType = pt_Time;
                  parsed = true;
               }
               else {
                  prevPer = periodGenerator(tks);
                  if (prevPer != nullptr) {
                     prevType = pt_Period;
                     parsed = true;
                  }
               }
            }
            break;
         }
         case pt_Time: {
            Generator<_per>* per = periodGenerator(tks);
            if (per != nullptr) {
               Generator<_tim>* incr = new IncreasedTime(prevTim, per);
               prevTim = incr;
               parsed = true;
            }
            else {
               result->push_back(new Cast_T_S(prevTim));
               prevTim = timeGenerator(tks);
               if (prevTim != nullptr) {
                  prevType = pt_Time;
                  parsed = true;
               }
               else {
                  prevNum = numberGenerator(tks);
                  if (prevNum != nullptr) {
                     prevType = pt_Number;
                     parsed = true;
                  }
               }
            }
            break;
         }
         case pt_Period: {
            Generator<_per>* per = periodGenerator(tks);
            if (per != nullptr) {
               Generator<_per>* add = new PeriodAddition(prevPer, per);
               prevPer = add;
               parsed = true;
            }
            else {
               result->push_back(new Cast_P_S(prevPer));
               prevPer = nullptr;
               prevNum = numberGenerator(tks);
               if (prevNum != nullptr) {
                  prevType = pt_Number;
                  parsed = true;
               }
               else {
                  prevTim = timeGenerator(tks);
                  if (prevTim != nullptr) {
                     prevType = pt_Time;
                     parsed = true;
                  }
               }
            }
            break;
         }
      }

      if (!parsed) {
         prevType = pt_String;
         Generator<_str>* str = stringGenerator(tks);
         if (str == nullptr) {
            // parsing failed
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
         else {
            result->push_back(str);
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

   Generator<_str>* s = new ConcatString(result);
   return s;
}
