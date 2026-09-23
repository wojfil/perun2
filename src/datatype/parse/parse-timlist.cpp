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

#include "parse-numlist.h"
#include "parse-generic.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../../util.h"
#include "../parse-gen.h"


namespace perun2::parse
{

p_bool parseTimList(p_genptr<p_tlist>& result, const Tokens& tks, Perun2Process& p2)
{
   const p_size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseTimListed(result, tks, p2);
      }

      if (parseBinary<p_tlist>(result, tks, p2) || parseTernary<p_tlist>(result, tks, p2)) {
         return true;
      }
   }

   return false;
}

static p_bool parseTimListed(p_genptr<p_tlist>& result, const Tokens& tks, Perun2Process& p2)
{
   // look - I do not use template functions from 'parse-generic.h'
   // why? because time has a special property - comma can mean
   // a list separator or a date and clock separator within one time
   // so parsing goes a weird way

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_COMMA);

   p_genptr<p_tlist> times;
   if (parseListedTimes(times, elements, p2)) {
      result = std::move(times);
      return true;
   }

   return parseListedTimLists(result, elements, p2);
}

static p_bool parseListedTimes(p_genptr<p_tlist>& res, const std::vector<Tokens>& elements, Perun2Process& p2)
{
   const p_size len = elements.size();
   p_bool isPrev = false;
   std::vector<p_genptr<p_tim>> result;
   p_bool isConstant = true;

   for (p_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      p_genptr<p_tim> time;

      if (parse(p2, tks, time)) {
         isPrev = true;
         if (isConstant && !time->isConstant()) {
            isConstant = false;
         }
         result.push_back(std::move(time));
      }
      else {
         if (isPrev) {
            p_genptr<p_tim> last = std::move(result.back());
            result.pop_back();
            p_genptr<p_tim> time2;

            if (timeFromTwoSeqs(time2, elements[i - 1], tks, p2)) {
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

   p_genptr<p_tlist> v = std::make_unique<gen::Listed<p_tim>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<p_tlist>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}

static p_bool timeFromTwoSeqs(p_genptr<p_tim>& result, const Tokens& prev, const Tokens& curr, Perun2Process& p2)
{
   const p_int start = prev.getStart();
   const p_int length = prev.getLength() + curr.getLength() + 1;
   const Tokens tks2(curr, start, length);
   return parse(p2, tks2, result);
}

static p_bool parseListedTimLists(p_genptr<p_tlist>& res, const std::vector<Tokens>& elements, Perun2Process& p2)
{
   const p_size len = elements.size();
   p_bool isPrev = false;
   std::vector<p_genptr<p_tlist>> result;
   p_bool isConstant = true;

   for (p_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      p_genptr<p_tim> time;

      if (parse(p2, tks, time)) {
         if (isConstant && !time->isConstant()) {
            isConstant = false;
         }

         p_genptr<p_tlist> time2 = std::make_unique<gen::Cast_T_TL>(time);
         result.push_back(std::move(time2));
         isPrev = true;
      }
      else {
         if (isPrev) {
            p_genptr<p_tim> time2;

            if (timeFromTwoSeqs(time2, elements[i - 1], tks, p2)) {
               if (isConstant && !time2->isConstant()) {
                  isConstant = false;
               }

               p_genptr<p_tlist>(std::move(result.back()));
               result.pop_back();
               result.push_back(std::make_unique<gen::Cast_T_TL>(time2));
               isPrev = false;
               continue;
            }
         }

         p_genptr<p_tlist> tlist;
         if (parse(p2, tks, tlist)) {
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

   p_genptr<p_tlist> v = std::make_unique<gen::ListedLists<p_tim>>(result);

   if (isConstant) {
      res = std::make_unique<gen::Constant<p_tlist>>(v->getValue());
   }
   else {
      res = std::move(v);
   }

   return true;
}

}
