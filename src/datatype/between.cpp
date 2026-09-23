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


BetweenNumbers::BetweenNumbers(p_genptr<p_num>& val, p_genptr<p_num>& b1, p_genptr<p_num>& b2, const bool neg)
   : value(std::move(val)), bound1(std::move(b1)), bound2(std::move(b2)), negated(neg) { };


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
      const p_bool r = (v >= b1 && v <= b2);
      return negated ? !r : r;
   }

   const p_bool r = (v >= b2 && v <= b1);
   return negated ? !r : r;
};


BetweenNumbersHalfConst::BetweenNumbersHalfConst(p_genptr<p_num>& val, p_genptr<p_num>& b1, const p_num& b2, const bool neg)
   : value(std::move(val)), bound1(std::move(b1)), bound2(b2), negated(neg) { };


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
      const p_bool r = (v >= b1 && v <= bound2);
      return negated ? !r : r;
   }

   const p_bool r = (v >= bound2 && v <= b1);
   return negated ? !r : r;
};


BetweenNumbersConst::BetweenNumbersConst(p_genptr<p_num>& val, const p_num& b1, const p_num& b2, const bool neg)
   : value(std::move(val)), bound1((b1 < b2) ? b1 : b2), bound2((b1 > b2) ? b1 : b2), negated(neg) { };


p_bool BetweenNumbersConst::getValue()
{
   const p_num v = value->getValue();

   if (v.isNaN()) {
      return false;
   }

   const p_bool r = (v >= bound1 && v <= bound2);
   return negated ? !r : r;
}


BetweenTimes::BetweenTimes(p_genptr<p_tim>& val, p_genptr<p_tim>& b1, p_genptr<p_tim>& b2, const bool neg)
   : value(std::move(val)), bound1(std::move(b1)), bound2(std::move(b2)), negated(neg) { };


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
      const p_bool r = (v >= b1 && v <= b2);
      return negated ? !r : r;
   }

   const p_bool r = (v >= b2 && v <= b1);
   return negated ? !r : r;
};


BetweenTimesHalfConst::BetweenTimesHalfConst(p_genptr<p_tim>& val, p_genptr<p_tim>& b1, const p_tim& b2, const bool neg)
   : value(std::move(val)), bound1(std::move(b1)), bound2(b2), negated(neg) { };


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
      const p_bool r = (v >= b1 && v <= bound2);
      return negated ? !r : r;
   }

   const p_bool r = (v >= bound2 && v <= b1);
   return negated ? !r : r;
};


BetweenTimesConst::BetweenTimesConst(p_genptr<p_tim>& val, const p_tim& b1, const p_tim& b2, const bool neg)
   : value(std::move(val)), bound1((b1 < b2) ? b1 : b2), bound2((b1 > b2) ? b1 : b2), negated(neg) { };


p_bool BetweenTimesConst::getValue()
{
   const p_tim v = value->getValue();
   
   if (! v.isComparableWith(bound1) || ! v.isComparableWith(bound2)) {
      return false;
   }

   const p_bool r = (v >= bound1 && v <= bound2);
   return negated ? !r : r;
}


}
