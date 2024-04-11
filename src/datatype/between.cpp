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

#include "between.h"


namespace perun2::gen
{


BetweenTimes::BetweenTimes(p_genptr<p_tim>& val, p_genptr<p_tim>& b1, p_genptr<p_tim>& b2)
   : value(std::move(val)), bound1(std::move(b1)), bound2(std::move(b2)) { };


p_bool BetweenTimes::getValue() 
{
   const p_tim b1 = bound1->getValue();
   const p_tim b2 = bound2->getValue();

   if (! b1.isComparableWith(b2)) {
      return false;
   }

   const p_tim v = value->getValue();

   if (! v.isComparableWith(b1)) {
      return false;
   }

   if (b1 < b2) {
      return v >= b1 && v <= b2;
   }

   return v >= b2 && v <= b1;
};


BetweenNumbers::BetweenNumbers(p_genptr<p_num>& val, p_genptr<p_num>& b1, p_genptr<p_num>& b2)
   : value(std::move(val)), bound1(std::move(b1)), bound2(std::move(b2)) { };


p_bool BetweenNumbers::getValue() 
{
   const p_num v = value->getValue();

   if (v.isNaN()) {
      return false;
   }

   const p_num b1 = bound1->getValue();

   if (b1.isNaN()) {
      return false;
   }

   const p_num b2 = bound2->getValue();

   if (b2.isNaN()) {
      return false;
   }

   if (b1 < b2) {
      return v >= b1 && v <= b2;
   }

   return v >= b2 && v <= b1;
};


BetweenNumbersHalfConst::BetweenNumbersHalfConst(p_genptr<p_num>& val, p_genptr<p_num>& b1, const p_num& b2)
   : value(std::move(val)), bound1(std::move(b1)), bound2(b2) { };


p_bool BetweenNumbersHalfConst::getValue() 
{
   const p_num v = value->getValue();

   if (v.isNaN()) {
      return false;
   }

   const p_num b1 = bound1->getValue();

   if (b1.isNaN()) {
      return false;
   }

   if (b1 < bound2) {
      return v >= b1 && v <= bound2;
   }

   return v >= bound2 && v <= b1;
};


BetweenTimesHalfConst::BetweenTimesHalfConst(p_genptr<p_tim>& val, p_genptr<p_tim>& b1, const p_tim& b2)
   : value(std::move(val)), bound1(std::move(b1)), bound2(b2) { };


p_bool BetweenTimesHalfConst::getValue() 
{
   const p_tim v = value->getValue();
   
   if (! v.isComparableWith(bound2)) {
      return false;
   }

   const p_tim b1 = bound1->getValue();

   if (! v.isComparableWith(b1)) {
      return false;
   }

   if (b1 < bound2) {
      return v >= b1 && v <= bound2;
   }

   return v >= bound2 && v <= b1;
};




}
