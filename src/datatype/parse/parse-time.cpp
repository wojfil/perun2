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

#include "parse-time.h"
#include "parse-generic.h"
#include "parse-var.h"
#include "../parse/parse-function.h"
#include "../generator/gen-time.h"
#include "../generator/gen-period.h"
#include "../generator/gen-generic.h"
#include "../../brackets.h"
#include "../../util.h"
#include "../../hash.h"
#include "../parse-gen.h"


namespace perun2::parse
{

_bool parseTime(_genptr<_tim>& result, const Tokens& tks, _p2& p2)
{
   const _size len = tks.getLength();

   if (tks.first().isSymbol(CHAR_MINUS) || tks.check(TI_HAS_FILTER_KEYWORD)) {
      return false;
   }

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }
   else if (len >= 2) {
      if (parseTimeConst(result, tks, p2)) {
         return true;
      }

      const _bool hasPluses = tks.check(TI_HAS_CHAR_PLUS);
      const _bool hasMinuses = tks.check(TI_HAS_CHAR_MINUS);

      if (hasMinuses || hasPluses) {
         _genptr<_per> per;
         if (parse(p2, tks, per)) {
            return false;
         }
         else {
            if (parseTimeExp(result, tks, p2)) {
               return true;
            }
         }
      }
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::timeFunction(result, tks, p2);
   }

   if (parseCollectionElement<_tim>(result, tks, p2)) {
      return true;
   }

   if (tks.check(TI_IS_LIST_ELEM_MEMBER)) {
      const Tokens tksm(tks, tks.getStart(), tks.getLength() - 1);
      _genptr<_num> num;
      parseListElementIndex(num, tksm, p2);
      const Token& f = tks.first();
      _genptr<_tlist> tlist;
      if (makeVarRef(f, tlist, p2)) {
         const Token& last = tks.last();
         _genptr<_tim> tim = std::make_unique<gen::ListElement<_tim>>(tlist, num);

         if (last.value.twoWords.h2 == p2.hashes.HASH_FUNC_DATE) {
            result = std::make_unique<gen::TimeDateAtIndex>(tim);
            return true;
         }
         else
            return false;
      }
   }

   if (parseTernary<_tim>(result, tks, p2)) {
      return true;
   }

   return false;
}

_bool parseTimeConst(_genptr<_tim>& result, const Tokens& tks, _p2& p2)
{
   // tt_YearMonth:
   const _size len = tks.getLength();
   const Token& first = tks.first();
   const Token& second = tks.second();

   if (len == 2) {
      if (second.type != Token::t_Number) {
         return false;
      }

      if (first.type == Token::t_Word) {
         throw SyntaxError::invalidMonthName(first.getOriginString(p2), first.line);
      }

      if (first.type != Token::t_Number || first.value.num.nm != NumberMode::nm_Month) {
         return false;
      }

      const _tnum month = static_cast<_tnum>(first.value.num.n.toInt());
      const _tnum year = tokenToTimeNumber(second);
      result = std::make_unique<gen::Constant<_tim>>(_tim(month, year));
      return true;
   }

   // tt_Date:
   const Token& third = tks.at(2);

   if (first.type != Token::t_Number || third.type != Token::t_Number) {
      return false;
   }

   if (second.type == Token::t_Word) {
      throw SyntaxError::invalidMonthName(second.getOriginString(p2), second.line);
   }

   if (second.type != Token::t_Number || second.value.num.nm != NumberMode::nm_Month) {
      return false;
   }

   const _tnum day = tokenToTimeNumber(first);
   const _tnum month = static_cast<_tnum>(second.value.num.n.toInt());
   const _tnum year = tokenToTimeNumber(third);
   checkDayCorrectness(day, month, year, first);

   if (len == 3) {
      result = std::make_unique<gen::Constant<_tim>>(_tim(day, month, year));
      return true;
   }

   // tt_ShortClock
   if (!(len == 7 || len == 9)){
      return false;
   }

   if (!tks.at(3).isSymbol(CHAR_COMMA) || !tks.at(5).isSymbol(CHAR_COLON) ||
       tks.at(4).type != Token::t_Number || tks.at(6).type != Token::t_Number){
      return false;
   }

   const _tnum hour = tokenToTimeNumber(tks.at(4));
   const _tnum minute = tokenToTimeNumber(tks.at(6));

   if (hour < 0 || hour >= 24) {

      throw SyntaxError::hoursOutOfRange(toStr(hour), tks.at(4).line);
   }

   if (minute < 0 || minute >= 60) {
      throw SyntaxError::minutesOutOfRange(toStr(minute), tks.at(6).line);
   }

   if (len == 7) {
      result = std::make_unique<gen::Constant<_tim>>(_tim(day, month, year, hour, minute));
      return true;
   }

   // tt_Clock:
   if (!tks.at(7).isSymbol(CHAR_COLON) || tks.at(8).type != Token::t_Number) {
      return false;
   }

   const _tnum secs = tokenToTimeNumber(tks.at(8));

   if (secs < 0 || secs >= 60) {
      throw SyntaxError::secondsOutOfRange(toStr(secs), tks.at(8).line);
   }

   result = std::make_unique<gen::Constant<_tim>>(_tim(day, month, year, hour, minute, secs));
   return true;
}

static _tnum tokenToTimeNumber(const Token& tk)
{
   return toTimeNumber(tk.value.num.n);
}

static void checkDayCorrectness(const _tnum day, const _tnum month,
   const _tnum year, const Token& tk)
{
   if (day < 1) {
      throw SyntaxError::dayCannotBeSmallerThanOne(tk.line);
   }

   const _tnum expected = daysInMonth(month, year);
   if (day > expected) {
      throw SyntaxError::monthHasFewerDays(monthToString(month), toStr(expected), tk.line);
   }
}

static _bool parseTimeExp(_genptr<_tim>& result, const Tokens& tks, _p2& p2)
{
   _genptr<_tim> prevTim;
   _genptr<_tim> time;
   BracketsInfo bi;
   const _int start = tks.getStart();
   const _int end = tks.getEnd();
   _int sublen = 0;
   _bool subtract = false;
   _bool prevSubtract = false;
   _int numReserve = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol) {
         switch (t.value.ch) {
            case CHAR_PLUS: {
               if (bi.isBracketFree()) {
                  if (sublen == 0) {
                     if (time) {
                        throw SyntaxError::adjacentSymbols(CHAR_PLUS, t.line);
                     }
                     else {
                        throw SyntaxError::expressionCannotStartWith(CHAR_PLUS, t.line);
                     }
                  }

                  const Tokens tks2(tks, i - sublen, sublen);
                  if (!timeExpUnit(sublen, subtract, prevSubtract, prevTim, time, tks2, numReserve, p2)) {
                     return false;
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
            case CHAR_MINUS: {
               if (bi.isBracketFree() && sublen != 0) {
                  const Tokens tks2(tks, i - sublen, sublen);
                  if (!timeExpUnit(sublen, subtract, prevSubtract, prevTim, time, tks2, numReserve, p2)) {
                     return false;
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
            case CHAR_OPENING_ROUND_BRACKET:
            case CHAR_CLOSING_ROUND_BRACKET:
            case CHAR_OPENING_SQUARE_BRACKET:
            case CHAR_CLOSING_SQUARE_BRACKET: {
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
      throw SyntaxError::expressionCannotEndWith(subtract ? CHAR_MINUS : CHAR_PLUS, tks.last().line);
   }

   const Tokens tks2(tks, 1 + end - sublen, sublen);
   if (!timeExpUnit(sublen, subtract, prevSubtract, prevTim, time, tks2,
      numReserve, p2) || numReserve != 0 || prevTim)
   {
      return false;
   }

   result = std::move(time);
   return true;
}

static _bool timeExpUnit(_int& sublen, const _bool subtract, _bool& prevSubtract,
   _genptr<_tim>& prevTim, _genptr<_tim>& time, const Tokens& tks,
   _int& numReserve, _p2& p2)
{
   _genptr<_tim> tim;
   if (parse(p2, tks, tim)) {
      if (numReserve != 0) {
         return false;
      }

      if (!time) {
         time = std::move(tim);
      }
      else if (!prevTim) {
         prevTim = std::move(tim);
      }
      else {
         if (subtract) {
            _genptr<_per> diff = std::make_unique<gen::TimeDifference>(prevTim, tim);

            if (time) {
               _genptr<_tim> tim2 = std::move(time);
               if (prevSubtract) {
                  time = std::make_unique<gen::DecreasedTime>(tim2, diff);
               }
               else {
                  time = std::make_unique<gen::IncreasedTime>(tim2, diff);
               }
            }
            else {
               return false;
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

   if (prevTim) {
      return false;
   }

   if (!time) {
      return false;
   }

   _genptr<_num> num;
   if (parse(p2, tks, num)) {
      if (numReserve == 0 && subtract) {
         numReserve = 1;
      }
      numReserve += sublen + 1;
      sublen = 0;
      return true;
   }

   if (numReserve == 0) {
      _genptr<_per> per;
      if (!parse(p2, tks, per)) {
         return false;
      }

      if (subtract) {
         time = std::make_unique<gen::DecreasedTime>(time, per);
      }
      else {
         time = std::make_unique<gen::IncreasedTime>(time, per);
      }
   }
   else {
      const _int start = tks.getStart() - numReserve;
      const _int length = tks.getLength() + numReserve;
      const Tokens tks2(tks, start, length);

      _genptr<_per> per;
      if (!parse(p2, tks2, per)) {
         return false;
      }

      time = std::make_unique<gen::IncreasedTime>(time, per);
   }

   sublen = 0;
   numReserve = 0;
   return true;
}

}
