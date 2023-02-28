/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parse-numlist.h"
#include "parse-generic.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../../util.h"
#include "../parse-gen.h"


namespace uro::parse
{

_bool parseTimList(_genptr<_tlist>& result, const Tokens& tks, _uro& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseTimListed(result, tks, uro);
      }

      if (parseBinary<_tlist>(result, tks, uro) || parseTernary<_tlist>(result, tks, uro)) {
         return true;
      }
   }

   return false;
}

static _bool parseTimListed(_genptr<_tlist>& result, const Tokens& tks, _uro& uro)
{
   // look - I do not use template functions from 'parse-generic.h'
   // why? because time has a special property - comma can mean
   // a list separator or a date and clock separator within one time
   // so parsing goes a weird way

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_COMMA);

   _genptr<_tlist> times;
   if (parseListedTimes(times, elements, uro)) {
      result = std::move(times);
      return true;
   }

   return parseListedTimLists(result, elements, uro);
}

static _bool parseListedTimes(_genptr<_tlist>& res, const std::vector<Tokens>& elements, _uro& uro)
{
   const _size len = elements.size();
   _bool isPrev = false;
   std::vector<_genptr<_tim>> result;
   _bool isConstant = true;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _genptr<_tim> time;

      if (parse(uro, tks, time)) {
         isPrev = true;
         if (isConstant && !time->isConstant()) {
            isConstant = false;
         }
         result.push_back(std::move(time));
      }
      else {
         if (isPrev) {
            _genptr<_tim> last = std::move(result.back());
            result.pop_back();
            _genptr<_tim> time2;

            if (timeFromTwoSeqs(time2, elements[i - 1], tks, uro)) {
               isPrev = false;
               if (isConstant && !time2->isConstant()) {
                  isConstant = false;
               }
               result.push_back(std::move(time2));
            }
            else {
               return false;
            }
         }
         else {
            return false;
         }
      }
   }

   _genptr<_tlist> v = std::make_unique<gen::Listed<_tim>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<_tlist>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}

static _bool timeFromTwoSeqs(_genptr<_tim>& result, const Tokens& prev, const Tokens& curr, _uro& uro)
{
   const _int start = prev.getStart();
   const _int length = prev.getLength() + curr.getLength() + 1;
   const Tokens tks2(curr, start, length);
   return parse(uro, tks2, result);
}

static _bool parseListedTimLists(_genptr<_tlist>& res, const std::vector<Tokens>& elements, _uro& uro)
{
   const _size len = elements.size();
   _bool isPrev = false;
   std::vector<_genptr<_tlist>> result;
   _bool isConstant = true;

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      _genptr<_tim> time;

      if (parse(uro, tks, time)) {
         if (isConstant && !time->isConstant()) {
            isConstant = false;
         }

         _genptr<_tlist> time2 = std::make_unique<gen::Cast_T_TL>(time);
         result.push_back(std::move(time2));
         isPrev = true;
      }
      else {
         if (isPrev) {
            _genptr<_tim> time2;

            if (timeFromTwoSeqs(time2, elements[i - 1], tks, uro)) {
               if (isConstant && !time2->isConstant()) {
                  isConstant = false;
               }

               _genptr<_tlist>(std::move(result.back()));
               result.pop_back();
               result.push_back(std::make_unique<gen::Cast_T_TL>(time2));
               isPrev = false;
               continue;
            }
         }

         _genptr<_tlist> tlist;
         if (parse(uro, tks, tlist)) {
            if (isConstant && !tlist->isConstant()) {
               isConstant = false;
            }

            result.push_back(std::move(tlist));
            isPrev = false;
         }
         else {
            return false;
         }
      }
   }

   _genptr<_tlist> v = std::make_unique<gen::ListedLists<_tim>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<_tlist>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}

}
