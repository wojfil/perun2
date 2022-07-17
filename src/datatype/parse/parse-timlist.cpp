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

#include "parse-numlist.h"
#include "parse-generic.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-timlist.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../../util.h"
#include "../parse-gen.h"


Generator<_tlist>* parseTimList(const Tokens& tks, Uroboros* uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      Generator<_tlist>* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }

   Generator<_tlist>* filter = parseFilter<Generator<_tlist>*, _tim>(tks, ThisState::ts_Time, uro);
   if (filter != nullptr) {
      return filter;
   }

   if (len >= 3) {
      if (tks.containsSymbol(PGCS_COMMA)) {
         Generator<_tlist>* listed = parseTimListed(tks, uro);
         if (listed != nullptr) {
            return listed;
         }
      }

      Generator<_tlist>* bin = parseBinary<_tlist>(tks, uro);
      if (bin != nullptr) {
         return bin;
      }

      Generator<_tlist>* tern = parseTernary<_tlist>(tks, uro);
      if (tern != nullptr) {
         return tern;
      }
   }

   return nullptr;
}

static Generator<_tlist>* parseTimListed(const Tokens& tks, Uroboros* uro)
{
   std::vector<Tokens> elements;
   tks.splitBySymbol(L',', elements);

   Generator<_tlist>* cnst = parseTimListConst(elements);
   if (cnst != nullptr) {
      return cnst;
   }

   // look - I do not use template functions from 'parse-generic.h'
   // why? because time has a special property - comma can mean
   // a list separator, or a date and clock separator within one time
   // so parsing goes a weird way
   Generator<_tlist>* times = parseListedTimes(elements, uro);
   if (times != nullptr) {
      return times;
   }

   Generator<_tlist>* lists = parseListedTimLists(elements, uro);
   if (lists != nullptr) {
      return lists;
   }

   return nullptr;
}

Generator<_tlist>* parseTimListConst(const std::vector<Tokens>& elements)
{
   const _size length = elements.size();
   std::vector<Generator<_tim>*> gens;

   for (_size i = 0; i < length; i++) {
      Generator<_tim>* time = parseTimeConst(elements[i]);
      if (time == nullptr) {
         if (i != 0) {
            deleteVector(gens);
         }
         return nullptr;
      }
      else {
         gens.push_back(time);
      }
   }

   _tlist times(length);
   for (_size i = 0; i < length; i++) {
      times[i] = (gens[i])->getValue();
   }

   return new Constant<_tlist>(times);
}

static Generator<_tlist>* parseListedTimes(const std::vector<Tokens>& elements, Uroboros* uro)
{
   const _size len = elements.size();
   _boo isPrev = false;
   std::vector<Generator<_tim>*>* result = new std::vector<Generator<_tim>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<_tim>* time;

      if (parse(uro, tks, time)) {
         isPrev = true;
         result->push_back(time);
      }
      else {
         if (isPrev) {
            Generator<_tim>*& last = result->back();
            result->pop_back();
            delete last;
            Generator<_tim>* time2 = timeFromTwoSeqs(elements[i - 1], tks, uro);

            if (time2 == nullptr) {
               deleteVectorPtr(result);
               return nullptr;
            }
            else {
               result->push_back(time2);
               isPrev = false;
            }
         }
         else {
            deleteVectorPtr(result);
            return nullptr;
         }
      }
   }

   return new Listed<_tim>(result);
}

static Generator<_tim>* timeFromTwoSeqs(const Tokens& prev, const Tokens& curr, Uroboros* uro)
{
   const _int start = prev.getStart();
   const _int length = prev.getLength() + curr.getLength() + 1;
   const Tokens tks2(curr.list, start, length);
   Generator<_tim>* tim;
   return parse(uro, tks2, tim) ? tim : nullptr;
}

static Generator<_tlist>* parseListedTimLists(const std::vector<Tokens>& elements, Uroboros* uro)
{
   const _size len = elements.size();
   _boo isPrev = false;
   std::vector<Generator<_tlist>*>* result = new std::vector<Generator<_tlist>*>();

   for (_size i = 0; i < len; i++) {
      const Tokens& tks = elements[i];
      Generator<_tim>* time;

      if (parse(uro, tks, time)) {
         result->push_back(new Cast_T_TL(time));
         isPrev = true;
      }
      else {
         if (isPrev) {
            Generator<_tim>* time2 = timeFromTwoSeqs(elements[i - 1], tks, uro);

            if (time2 != nullptr) {
               Generator<_tlist>*& last = result->back();
               result->pop_back();
               delete last;
               result->push_back(new Cast_T_TL(time2));
               isPrev = false;
               continue;
            }
         }

         Generator<_tlist>* tlist;
         if (parse(uro, tks, tlist)) {
            result->push_back(tlist);
            isPrev = false;
         }
         else {
            deleteVectorPtr(result);
            return nullptr;
         }
      }
   }

   return new ListedLists<_tim>(result);
}
