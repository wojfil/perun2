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

#include "parse-time.h"
#include "parse-generic.h"
#include "../function.h"
#include "../generator/gen-time.h"
#include "../generator/gen-period.h"
#include "../generator/gen-generic.h"
#include "../../brackets.h"
#include "../../util.h"
#include "../../hash.h"
#include "../parse-gen.h"


Generator<_tim>* parseTime(const Tokens& tks, Uroboros* uro)
{
   const _size len = tks.getLength();

   if (tks.first().isSymbol(L'-')) {
      return nullptr;
   }

   if (len == 1) {
      Generator<_tim>* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }
   else if (len >= 2) {
      Generator<_tim>* cnst = parseTimeConst(tks);
      if (cnst != nullptr) {
         return cnst;
      }

      const _boo hasPluses = tks.containsSymbol(PGCS_PLUS);
      const _boo hasMinuses = tks.containsSymbol(PGCS_MINUS);


      if (hasMinuses || hasPluses) {
         Generator<_per>* per;
         if (parse(uro, tks, per)) {
            delete per;
            return nullptr;
         }
         else {
            Generator<_tim>* exp = parseTimeExp(tks, uro);
            if (exp != nullptr) {
               return exp;
            }
         }
      }
   }

   if (isPossibleFunction(tks)) {
      Generator<_tim>* func = timeFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }

   Generator<_tim>* el = parseCollectionElement<_tim>(tks, uro);
   if (el != nullptr) {
      return el;
   }

   if (isPossibleListElementMember(tks, uro)) {
      Tokens tksm(tks);
      tksm.trimRight();

      Generator<_num>* num = parseListElementIndex(tksm, uro);
      const Token& f = tks.first();
      Generator<_tlist>* tlist;
      if (uro->vars.getVarValue(f, tlist)) {
         const Token& last = tks.last();

         if (last.value.twoWords.h2 == uro->hashes.HASH_FUNC_DATE)
            return new TimeDateAtIndex(tlist, num);
         else
            return nullptr;
      }
      else {
         delete num;
      }
   }

   Generator<_tim>* tern = parseTernary<_tim>(tks, uro);
   if (tern != nullptr) {
      return tern;
   }

   return nullptr;
}

Generator<_tim>* parseTimeConst(const Tokens& tks)
{
   // tt_YearMonth:
   const _size len = tks.getLength();
   const Token& first = tks.first();
   const Token& second = tks.second();

   if (len == 2) {
      if (second.type != Token::t_Number) {
         return nullptr;
      }

      if (first.type == Token::t_Word) {
         throw SyntaxException(str(L"'", *first.value.word.os,
            L"' is not a valid month name"), first.line);
      }

      if (first.type != Token::t_Number || first.value.num.nm != NumberMode::nm_Month) {
         return nullptr;
      }

      const _tnum month = static_cast<_tnum>(first.value.num.n.toInt());
      const _tnum year = tokenToTimeNumber(second);
      return new Constant<_tim>(_tim(month, year));
   }

   // tt_Date:
   const Token& third = tks.at(2);

   if (first.type != Token::t_Number || third.type != Token::t_Number) {
      return nullptr;
   }

   if (second.type == Token::t_Word) {
      throw SyntaxException(str(L"'", *second.value.word.os,
         L"' is not a valid month name"), second.line);
   }

   if (second.type != Token::t_Number || second.value.num.nm != NumberMode::nm_Month) {
      return nullptr;
   }

   const _tnum day = tokenToTimeNumber(first);
   const _tnum month = static_cast<_tnum>(second.value.num.n.toInt());
   const _tnum year = tokenToTimeNumber(third);
   checkDayCorrectness(day, month, year, first);

   if (len == 3) {
      return new Constant<_tim>(_tim(day, month, year));
   }

   // tt_ShortClock
   if (!(len == 7 || len == 9)){
      return nullptr;
   }

   if (!tks.at(3).isSymbol(L',') || !tks.at(5).isSymbol(L':') ||
       tks.at(4).type != Token::t_Number || tks.at(6).type != Token::t_Number){
      return nullptr;
   }

   const _tnum hour = tokenToTimeNumber(tks.at(4));
   const _tnum minute = tokenToTimeNumber(tks.at(6));

   if (hour < 0 || hour >= 24) {
      clockUnitException(L"hours", hour, tks.at(4));
   }

   if (minute < 0 || minute >= 60) {
      clockUnitException(L"minutes", minute, tks.at(6));
   }

   if (len == 7) {
      return new Constant<_tim>(_tim(day, month, year, hour, minute));
   }

   // tt_Clock:
   if (!tks.at(7).isSymbol(L':') || tks.at(8).type != Token::t_Number) {
      return nullptr;
   }

   const _tnum secs = tokenToTimeNumber(tks.at(8));

   if (secs < 0 || secs >= 60) {
      clockUnitException(L"seconds", secs, tks.at(8));
   }

   return new Constant<_tim>(_tim(day, month, year, hour, minute, secs));
}

static _tnum tokenToTimeNumber(const Token& tk)
{
   return toTimeNumber(tk.value.num.n);
}

static void checkDayCorrectness(const _tnum& day, const _tnum& month,
   const _tnum& year, const Token& tk)
{
   if (day < 1) {
      throw SyntaxException(L"day cannot be smaller than 1", tk.line);
   }

   const _tnum expected = daysInMonth(month, year);
   if (day > expected) {
      throw SyntaxException(str(L"month ", monthToString(month), L" has only ",
         toStr(expected), L" days"), tk.line);
   }
}

static void clockUnitException(const _str& unit, const _tnum& value,
   const Token& tk)
{
   throw SyntaxException(str(L"value of ", unit, L" (", toStr(value),
      L") went out of range"), tk.line);
}

static Generator<_tim>* parseTimeExp(const Tokens& tks, Uroboros* uro)
{
   Generator<_tim>* prevTim = nullptr;
   Generator<_tim>* time = nullptr;
   BracketsInfo bi;
   const _int start = tks.getStart();
   const _int end = tks.getEnd();
   _int sublen = 0;
   _boo subtract = false;
   _boo prevSubtract = false;
   _int numReserve = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol) {
         switch (t.value.ch) {
            case L'+': {
               if (bi.isBracketFree()) {
                  if (sublen == 0) {
                     if (time == nullptr) {
                        throw SyntaxException(L"expression cannot start with +",
                           t.line);
                     }
                     else {
                        delete time;
                        throw SyntaxException(L"adjacent + symbols", t.line);
                     }
                  }

                  const Tokens tks2(tks.list, i - sublen, sublen);
                  if (!timeExpUnit(sublen, subtract, prevSubtract,
                     prevTim, time, tks2, numReserve, uro)) {

                     deleteTwo(prevTim, time);
                     return nullptr;
                  }
                  if (numReserve == 0) {
                     prevSubtract = subtract;
                     subtract = false;
                  }
               }
               else {
                  sublen++;
               }
               break;
            }
            case L'-': {
               if (bi.isBracketFree() && sublen != 0) {
                  const Tokens tks2(tks.list, i - sublen, sublen);
                  if (!timeExpUnit(sublen, subtract, prevSubtract,
                     prevTim, time, tks2, numReserve, uro)) {

                     deleteTwo(prevTim, time);
                     return nullptr;
                  }

                  if (numReserve == 0) {
                     prevSubtract = subtract;
                     subtract = true;
                  }
               }
               else {
                  sublen++;
               }
               break;
            }
            case L'(': case L')':
            case L'[': case L']': {
               bi.refresh(t);
               sublen++;
               break;
            }
            default: {
               sublen++;
               break;
            }
         }
      }
      else {
         sublen++;
      }
   }

   if (sublen == 0) {
      deleteTwo(prevTim, time);

      if (subtract)
         throw SyntaxException(L"expression cannot end with -", tks.last().line);
      else
         throw SyntaxException(L"expression cannot end with +", tks.last().line);
   }

   const Tokens tks2(tks.list, 1 + end - sublen, sublen);
   if (!timeExpUnit(sublen, subtract, prevSubtract, prevTim, time, tks2,
      numReserve, uro) || numReserve != 0 || prevTim != nullptr)
   {
      deleteTwo(prevTim, time);
      return nullptr;
   }

   return time;
}

static _boo timeExpUnit(_int& sublen, const _boo& subtract, _boo& prevSubtract,
   Generator<_tim>*& prevTim, Generator<_tim>*& time, const Tokens& tks,
   _int& numReserve, Uroboros* uro)
{
   Generator<_tim>* tim;
   if (parse(uro, tks, tim)) {
      if (numReserve != 0) {
         delete tim;
         return false;
      }

      if (time == nullptr) {
         time = tim;
      }
      else if (prevTim == nullptr) {
         prevTim = tim;
      }
      else {
         if (subtract) {
            Generator<_per>* diff = new TimeDifference(prevTim, tim);
            prevTim = nullptr;

            if (time == nullptr) {
               delete diff;
               return false;
            }
            else {
               Generator<_tim>* prev = time;
               if (prevSubtract) {
                  time = new DecreasedTime(prev, diff);
               }
               else {
                  time = new IncreasedTime(prev, diff);
               }
            }
         }
         else {
            return false;
         }
      }
      sublen = 0;
      numReserve = 0;
      return true;
   }

   if (prevTim != nullptr) {
      return false;
   }

   if (time == nullptr) {
      return false;
   }

   Generator<_num>* num;
   if (parse(uro, tks, num)) {
      if (numReserve == 0 && subtract) {
         numReserve = 1;
      }
      numReserve += sublen + 1;
      sublen = 0;
      delete num;
      return true;
   }

   if (numReserve == 0) {
      Generator<_per>* per;
      if (!parse(uro, tks, per)) {
         return false;
      }

      Generator<_tim>* prev = time;
      if (subtract) {
         time = new DecreasedTime(prev, per);
      }
      else {
         time = new IncreasedTime(prev, per);
      }
   }
   else {
      const _int start = tks.getStart() - numReserve;
      const _int length = tks.getLength() + numReserve;
      Tokens tks2(tks.list, start, length);

      Generator<_per>* per;
      if (!parse(uro, tks2, per)) {
         return false;
      }

      Generator<_tim>* prev = time;
      time = new IncreasedTime(prev, per);
   }

   sublen = 0;
   numReserve = 0;
   return true;
}
