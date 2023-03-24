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

#include "parse-period.h"
#include "parse-generic.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-period.h"
#include "../generator/gen-time.h"
#include "../../brackets.h"
#include "../../util.h"
#include "../parse/parse-function.h"
#include "../../hash.h"
#include "../parse-gen.h"


namespace perun2::parse
{

_bool parsePeriod(_genptr<_per>& result, const Tokens& tks, _p2& p2)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (tks.check(TI_HAS_CHAR_COMMA) || tks.check(TI_HAS_FILTER_KEYWORD)) {
      return false;
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::periodFunction(result, tks, p2);
   }

   const _bool startsWithMinus = tks.first().isSymbol(CHAR_MINUS);
   const _bool lastIsWord = tks.last().type == Token::t_Word;
   const _bool hasPluses =  tks.check(TI_HAS_CHAR_PLUS);

   _bool hasMinuses;
   if (startsWithMinus) {
      Tokens tks2(tks);
      tks2.trimLeft();
      hasMinuses = tks2.check(TI_HAS_CHAR_MINUS);
   }
   else {
      hasMinuses = tks.check(TI_HAS_CHAR_MINUS);
   }

   if (len >= 3 && hasPluses || hasMinuses) {
      if (parsePeriodExp(result, tks, p2)) {
         return true;
      }
   }

   if (len >= 2 && !hasPluses && !hasMinuses && lastIsWord) {
      if (len == 2) {
         if (parsePeriodConst(result, tks, false, p2)) {
            return true;
         }
      }
      else if (len == 3 && startsWithMinus) {
         Tokens tks2(tks);
         tks2.trimLeft();
         if (parsePeriodConst(result, tks2, true, p2)) {
            return true;
         }
      }

      if (!tks.check(TI_HAS_CHAR_QUESTION_MARK) && !tks.check(TI_HAS_CHAR_COLON)) {
         if (parsePeriodUnit(result, tks, p2)) {
            return true;
         }
      }
   }

   if (len >= 2 && !hasPluses && !hasMinuses && startsWithMinus) {
      Tokens tks2(tks);
      tks2.trimLeft();

      _genptr<_per> per;
      if (parse(p2, tks2, per)) {
         result = std::make_unique<gen::NegatedPeriod>(per);
         return true;
      }
   }

   if (parseBinary<_per>(result, tks, p2) || parseTernary<_per>(result, tks, p2)) {
      return true;
   }

   return false;
}

_bool parsePeriodConst(_genptr<_per>& result, const Tokens& tks, const _bool negated, _p2& p2)
{
   const Token& last = tks.last();
   const Token& first = tks.first();

   if (first.type != Token::t_Number) {
      return false;
   }

   const _hash h = last.value.word.h;
   const _num& num = first.value.num.n;

   if (p2.hashes.HASH_GROUP_PERIOD_SINGLE.find(h) != p2.hashes.HASH_GROUP_PERIOD_SINGLE.end())
   {
      const Period::PeriodUnit unit = p2.hashes.HASH_MAP_PERIOD_UNITS.find(h)->second;

      if (num.isDouble) {
         if (num.value.d == NDOUBLE_ONE) {
            result = std::make_unique<gen::Constant<_per>>(_per(negated ? TNUM_MINUS_ONE : TNUM_ONE, unit));
            return true;
         }
         else {
            throw SyntaxError::missingLetterS(last.getOriginString(p2), last.line);
         }
      }
      else {
         if (num.value.i == NINT_ONE) {
            result = std::make_unique<gen::Constant<_per>>(_per(negated ? TNUM_MINUS_ONE : TNUM_ONE, unit));
            return true;
         }
         else {
            throw SyntaxError::missingLetterS(last.getOriginString(p2), last.line);
         }
      }
   }

   if (p2.hashes.HASH_GROUP_PERIOD_MULTI.find(h) != p2.hashes.HASH_GROUP_PERIOD_MULTI.end())
   {
      const Period::PeriodUnit unit = p2.hashes.HASH_MAP_PERIOD_UNITS.find(h)->second;

      _tnum v = num.isDouble
         ? static_cast<_tnum>(num.value.d)
         : static_cast<_tnum>(num.value.i);

      if (negated) {
         v *= TNUM_MINUS_ONE;
      }

      result = std::make_unique<gen::Constant<_per>>(_per(v, unit));
      return true;
   }

   return false;
}

_bool parsePeriodUnit(_genptr<_per>& result, const Tokens& tks, _p2& p2)
{
   const _hash h = tks.last().value.word.h;
   Tokens tks2(tks);
   tks2.trimRight();

   _genptr<_num> num;
   if (!parse(p2, tks2, num)) {
      return false;
   }

   if (p2.hashes.HASH_GROUP_PERIOD_SINGLE.find(h) != p2.hashes.HASH_GROUP_PERIOD_SINGLE.end()) {
      const Token& last = tks.last();
      throw SyntaxError::missingLetterS(last.getOriginString(p2), last.line);
   }

   if (p2.hashes.HASH_GROUP_PERIOD_MULTI.find(h) != p2.hashes.HASH_GROUP_PERIOD_MULTI.end()) {
      const Period::PeriodUnit& unit = p2.hashes.HASH_MAP_PERIOD_UNITS.find(h)->second;
      result = std::make_unique<gen::PeriodUnit>(num, unit);
      return true;
   }

   return false;
}

_bool parsePeriodExp(_genptr<_per>& result, const Tokens& tks, _p2& p2)
{
   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_PLUS);
   const _size len = elements.size();
   _genptr<_per> res;

   if (!parsePeriodExpDiff(res, elements[0], p2)) {
      return false;
   }
   if (len == 1) {
      result = std::move(res);
      return true;
   }

   for (_size i = 1; i < len; i++) {
      const Tokens& el = elements[i];
      _genptr<_per> per;

      if (parsePeriodExpDiff(per, el, p2)) {
         _genptr<_per> per2 = std::move(res);
         res = std::make_unique<gen::PeriodAddition>(per2, per);
      }
      else {
         return false;
      }
   }

   result = std::move(res);
   return true;
}

_bool parsePeriodExpDiff(_genptr<_per>& result, const Tokens& tks, _p2& p2)
{
   const _int baseLen = tks.getLength();
   if (baseLen == 1) {
      return parse(p2, tks, result);
   }

   _bool minusAwaits = tks.first().isSymbol(CHAR_MINUS);

   Tokens tks2(tks);
   if (minusAwaits) {
      tks2.trimLeft();
   }

   if (!tks2.check(TI_HAS_CHAR_MINUS)) {
      _genptr<_per> gp;
      if (parse(p2, tks2, gp)) {
         if (minusAwaits) {
            result = std::make_unique<gen::NegatedPeriod>(gp);
         }
         else {
            result = std::move(gp);
         }

         return true;
      }
      else {
         return false;
      }
   }

   const std::vector<Tokens> elements = tks2.splitBySymbol(CHAR_MINUS);
   const _size len = elements.size();

   _bool isTime;
   _bool hasFirst = false;
   _genptr<_tim> time;
   _genptr<_per> res;

   if (parse(p2, elements[0], res)) {
      hasFirst = true;
      isTime = false;
      if (minusAwaits) {
         _genptr<_per> res2 = std::move(res);
         res = std::make_unique<gen::NegatedPeriod>(res2);
         minusAwaits = false;
      }
   }
   else {
      if (parse(p2, elements[0], time)) {
         isTime = true;
      }
      else {
         return false;
      }
   }

   for (_size i = 1; i < len; i++) {
      const Tokens& el = elements[i];

      if (isTime) {
         _genptr<_tim> time2;
         if (parse(p2, el, time2)) {
            _genptr<_per> per = std::make_unique<gen::TimeDifference>(time, time2);

            if (hasFirst) {
               _genptr<_per> res2 = std::move(res);
               res = std::make_unique<gen::PeriodSubtraction>(res2, per);
            }
            else {
               if (minusAwaits) {
                  res = std::make_unique<gen::NegatedPeriod>(per);
                  minusAwaits = false;
               }
               else {
                  res = std::move(per);
               }
               hasFirst = true;
            }
            time.reset();
            isTime = false;
         }
         else {
            _genptr<_per> per3;
            if (parse(p2, el, per3)) {
               _genptr<_tim> tim3 = std::make_unique<gen::DecreasedTime>(time, per3);
               time = std::move(tim3);
            }
            else {
               return false;
            }
         }
      }
      else {
         _genptr<_per> per;
         if (parse(p2, el, per)) {
            _genptr<_per> sub = std::make_unique<gen::PeriodSubtraction>(res, per);

            if (minusAwaits) {
               res = std::make_unique<gen::NegatedPeriod>(sub);
               minusAwaits = false;
            }
            else {
               res = std::move(sub);
            }
         }
         else {
            if (parse(p2, el, time)) {
               isTime = true;
            }
            else {
               return false;
            }
         }
      }
   }

   if (isTime) {
      return false;
   }

   result = std::move(res);
   return true;
}

_bool parseTimeDifference(_genptr<_per>& result, const Tokens& tks, _p2& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_MINUS);

   if (pair.second.isEmpty()) {
      throw SyntaxError::expressionCannotEndWith(CHAR_MINUS, tks.last().line);
   }

   if (pair.first.isEmpty()) {
      return false;
   }

   _genptr<_tim> tim1;
   if (parse(p2, pair.first, tim1)) {
      _genptr<_tim> tim2;
      if (parse(p2, pair.second, tim2)) {
         result = std::make_unique<gen::TimeDifference>(tim1, tim2);
         return true;
      }
   }

   _genptr<_per> per1;
   if (!parse(p2, pair.first, per1)) {
      return false;
   }

   _genptr<_per> per2;
   if (!parse(p2, pair.second, per2)) {
      return false;
   }

   result = std::make_unique<gen::PeriodSubtraction>(per1, per2);
   return true;
}

}
