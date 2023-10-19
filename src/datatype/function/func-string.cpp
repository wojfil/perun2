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

#include "func-string.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../math.h"
#include <cwctype>
#include <algorithm>
#include <sstream>
#include <cwctype>
#include <cmath>
#include <bitset>


namespace perun2::func
{

p_str F_After::getValue()
{
   const p_str s1 = arg1->getValue();
   if (s1.empty()) {
      return s1;
   }

   const p_str s2 = arg2->getValue();
   if (s2.empty()) {
      return s2;
   }

   if (s2.size() == 1) {
      for (p_size i = 0; i < s1.size(); i++) {
         if (s1[i] == s2[0]) {
            return i == s1.size() - 1
               ? p_str()
               : s1.substr(i + 1);
         }
      }

      return p_str();
   }
   else {
      auto pos = s1.find(s2);
      return pos == p_str::npos
         ? p_str()
         : s1.substr(pos + s2.size());
   }
}


p_str F_Before::getValue()
{
   const p_str s1 = arg1->getValue();
   if (s1.empty()) {
      return s1;
   }

   const p_str s2 = arg2->getValue();
   if (s2.empty()) {
      return s2;
   }

   if (s2.size() == 1) {
      for (p_size i = 0; i < s1.size(); i++) {
         if (s1[i] == s2[0]) {
            return i == 0
               ? p_str()
               : s1.substr(0, i);
         }
      }

      return p_str();
   }
   else {
      auto pos = s1.find(s2);
      return pos == p_str::npos
         ? p_str()
         : s1.substr(0, pos);
   }
}


p_str F_Digits::getValue()
{
   const p_str s1 = arg1->getValue();
   p_size len2 = 0;
   p_size it = 0;

   for (const p_char ch : s1) {
      if (std::iswdigit(ch)) {
         len2++;
      }
   }

   p_str s2(len2, CHAR_SPACE);

   for (const p_char ch : s1) {
      if (std::iswdigit(ch)) {
         s2[it] = ch;
         it++;
      }
   }

   return s2;
}


p_str F_Fill::getValue()
{
   const p_str base = arg1->getValue();
   const p_num n = arg2->getValue();

   if (n.state == NumberState::NaN) {
      return p_str();
   }

   const p_nint v = n.toInt();

   if (v <= NINT_ZERO) {
      return base;
   }

   const p_size min = static_cast<p_size>(v);

   return base.size() >= min
      ? base
      : str(p_str(min - base.size(), CHAR_0), base);
}


p_str F_Letters::getValue()
{
   const p_str s1 = arg1->getValue();
   p_size len2 = 0;
   p_size it = 0;

   for (const p_char ch : s1) {
      if (std::iswalpha(ch)) {
         len2++;
      }
   }

   p_str s2(len2, CHAR_SPACE);
   for (const p_char ch : s1) {
      if (std::iswalpha(ch)) {
         s2[it] = ch;
         it++;
      }
   }

   return s2;
}


p_str F_Lower::getValue()
{
   p_str value = arg1->getValue();
   if (!value.empty()) {
      toLower(value);
   }
   return value;
}


p_str F_Trim::getValue()
{
   const p_str value = arg1->getValue();
   const p_int length = value.size();

   if (length == 0) {
      return value;
   }

   p_int left;
   for (left = 0; left < length; left++) {
      if (!(std::iswspace(value[left]))) {
         break;
      }
   }

   if (left == length) {
      return p_str();
   }

   p_int right;
   for (right = length - 1; right >= 0; --right) {
      if (!(std::iswspace(value[right]))) {
         break;
      }
   }

   return right == length - 1
      ? value.substr(left)
      : value.substr(left, right - left + 1);
}


p_str F_Upper::getValue()
{
   p_str value = arg1->getValue();
   if (!value.empty()) {
      toUpper(value);
   }
   return value;
}


p_str F_Repeat::getValue()
{
   const p_num n = arg2->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   const p_nint repeats = n.toInt();
   if (repeats <= NINT_ZERO) {
      return p_str();
   }

   const p_str base = arg1->getValue();
   const p_size len = base.size();

   switch (len) {
      case 0: {
         return p_str();
      }
      case 1: {
         return p_str(repeats, base[0]);
      }
   }

   switch (repeats) {
      case 1: {
         return base;
      }
      case 2: {
         return str(base, base);
      }
      case 3: {
         return str(base, base, base);
      }
   }

   p_str result;
   result.reserve(len * repeats);

   for (p_nint i = NINT_ZERO; i < repeats; i++) {
      result += base;
   }

   return result;
}


p_str F_Reverse::getValue()
{
   p_str value = arg1->getValue();

   if (value.size() <= 1) {
      return value;
   }

   std::reverse(value.begin(), value.end());
   return value;
}


p_str F_Left::getValue()
{
   const p_num n = arg2->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   const p_nint left = n.toInt();
   if (left <= NINT_ZERO) {
      return p_str();
   }

   const p_str value = arg1->getValue();
   const p_nint length = static_cast<p_nint>(value.size());

   return left >= length
      ? value
      : value.substr(0, left);
}


p_str F_Right::getValue()
{
   const p_num n = arg2->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   const p_nint right = n.toInt();
   if (right <= NINT_ZERO) {
      return p_str();
   }

   const p_str value = arg1->getValue();
   const p_nint length = static_cast<p_nint>(value.size());

   return right >= length
      ? value
      : value.substr(length - right);
}


p_str F_Substring_2::getValue()
{
   const p_num n2 = arg2->getValue();
   if (n2.state == NumberState::NaN) {
      return p_str();
   }

   p_nint index = n2.toInt();
   const p_str value = arg1->getValue();

   if (index == NINT_ZERO) {
      return value;
   }

   const p_nint length = static_cast<p_nint>(value.size());

   if (index < NINT_ZERO) {
      index *= NINT_MINUS_ONE;

      return index >= length
         ? value
         : value.substr(length - index);
   }
   else {
      return index >= length
         ? p_str()
         : value.substr(index);
   }
}


p_str F_Substring_3::getValue()
{
   const p_num n2 = arg2->getValue();
   if (n2.state == NumberState::NaN) {
      return p_str();
   }

   const p_num n3 = arg3->getValue();
   if (n3.state == NumberState::NaN) {
      return p_str();
   }

   p_nint index = n2.toInt();
   p_nint index2 = n3.toInt();

   if (index2 <= NINT_ZERO) {
      return p_str();
   }

   const p_str value = arg1->getValue();
   const p_nint length = static_cast<p_nint>(value.size());

   if (index < NINT_ZERO) {
      index *= NINT_MINUS_ONE;

      if (index >= length) {
         const p_nint lets = length - index + index2;
         return lets <= NINT_ZERO
            ? p_str()
            : value.substr(0, lets);
      }
      else {
         const p_nint start = length - index;
         return start + index2 >= length
            ? value.substr(start)
            : value.substr(start, index2);
      }
   }
   else {
      if (index >= length) {
         return p_str();
      }

      return index + index2 >= length
         ? value.substr(index)
         : value.substr(index, index2);
   }
}


p_str F_ConcatenateUnit::getValue()
{
   const p_list values = this->arg1->getValue();

   switch (values.size()) {
      case 0: {
         return p_str();
      }
      case 1: {
         return values[0];
      }
      case 2: {
         return str(values[0], values[1]);
      }
   }

   p_stream ss;

   for (const p_str& v : values) {
      ss << v;
   }

   return ss.str();
}


p_str F_Concatenate::getValue()
{
   p_stream ss;

   for (p_genptr<p_str>& gen : values) {
      ss << gen->getValue();
   }

   return ss.str();
}


p_str F_FirstDef::getValue()
{
   if (definition->hasNext()) {
      const p_str v = definition->getValue();
      definition->reset();
      return v;
   }
   else {
      return p_str();
   }
}


p_str F_LastDef::getValue()
{
   if (!definition->hasNext()) {
      return p_str();
   }

   while (definition->hasNext()) { }
   return definition->getValue();
}


p_str F_Replace::getValue()
{
   p_str base = arg1->getValue();
   p_size len = base.size();

   if (len == 0) {
      return base;
   }

   const p_str v1 = arg2->getValue();
   const p_str v2 = arg3->getValue();

   switch (v1.size()) {
      case 0: {
         break;
      }
      case 1: {
         switch (v2.size()) {
            case 0: {
               base.erase(std::remove(base.begin(), base.end(), v1[0]), base.end());
               break;
            }
            case 1: {
               std::replace(base.begin(), base.end(), v1[0], v2[0]);
               break;
            }
            default: {
               for (p_size i = 0; i < len; i++) {
                  if (base[i] == v1[0]) {
                     if (i == 0) {
                        base = str(v2, base.substr(1));
                     }
                     else if (i == len - 1) {
                        return str(base.substr(0, len - 1), v2);
                     }
                     else {
                        base = str(base.substr(0, i), v2, base.substr(i + 1));
                     }

                     i += v2.size() - 1;
                     len += v2.size() - 1;
                  }
               }
               break;
            }
         }
         break;
      }
      default: {
         size_t i = 0;

         switch (v2.size()) {
            case 0: {
               while ((i = base.find(v1)) != p_str::npos) {
                  base.erase(i, v1.size());
               }
               break;
            }
            default: {
               while ((i = base.find(v1, i)) != p_str::npos) {
                  base.replace(i, v1.size(), v2);
                  i += v2.size();
               }
               break;
            }
         }
         break;
      }
   }

   return base;
}


p_str F_String_B::getValue()
{
   return arg1->getValue() ? toStr(CHAR_1) : toStr(CHAR_0);
}


p_str F_String_N::getValue()
{
   return arg1->getValue().toString();
}


p_str F_String_T::getValue()
{
   return arg1->getValue().toString();
}


p_str F_String_P::getValue()
{
   return arg1->getValue().toString();
}


p_str F_MonthName::getValue()
{
   const p_num n = arg1->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   return monthToString(static_cast<p_tnum>(n.toInt()));
}


p_str F_MonthNameFromTime::getValue()
{
   const p_tim t = arg1->getValue();

   switch (t.type) {
      case Time::tt_YearMonth:
      case Time::tt_Null: {
         return p_str();
      }
      default: {
         return monthToString(t.month);
      }
   }
}


p_str F_WeekDayName::getValue()
{
   const p_num n = arg1->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   return weekdayToString(static_cast<p_tnum>(n.toInt()));
}


p_str F_WeekDayNameFromTime::getValue()
{
   const p_tim t = arg1->getValue();

   switch (t.type) {
      case Time::tt_YearMonth:
      case Time::tt_Null: {
         return p_str();
      }
      default: {
         return weekdayToString(t.getWeekDay());
      }
   }
}


p_str F_AfterDigits::getValue()
{
   const p_str value = arg1->getValue();
   p_bool after = false;

   for (p_size i = 0; i < value.size(); i++) {
      if (after) {
         if (!std::iswdigit(value[i])) {
            return value.substr(i);
         }
      }
      else {
         if (std::iswdigit(value[i])) {
            after = true;
         }
      }
   }

   return p_str();
}


p_str F_AfterLetters::getValue()
{
   const p_str value = arg1->getValue();
   p_bool after = false;

   for (p_size i = 0; i < value.size(); i++) {
      if (after) {
         if (!std::iswalpha(value[i])) {
            return value.substr(i);
         }
      }
      else {
         if (std::iswalpha(value[i])) {
            after = true;
         }
      }
   }

   return p_str();
}


p_str F_BeforeDigits::getValue()
{
   const p_str value = arg1->getValue();

   for (p_size i = 0; i < value.size(); i++) {
      if (std::iswdigit(value[i])) {
         return i == 0
            ? p_str()
            : value.substr(0, i);
      }
   }

   return p_str();
}


p_str F_BeforeLetters::getValue()
{
   const p_str value = arg1->getValue();

   for (p_size i = 0; i < value.size(); i++) {
      if (std::iswalpha(value[i])) {
         return i == 0
            ? p_str()
            : value.substr(0, i);
      }
   }

   return p_str();
}


p_str F_RandomChar::getValue()
{
   const p_str value = arg1->getValue();

   switch (value.size()) {
      case 0:
      case 1: {
         return value;
      }
      default: {
         return toStr(value[math.randomInt(value.size() - 1)]);
      }
   }
}


p_str F_Capitalize::getValue()
{
   p_str value = arg1->getValue();

   if (value.empty()) {
      return value;
   }

   p_bool prevLetter = false;

   for (p_char& ch : value) {
      const p_bool isLetter = std::iswalpha(ch);

      if (isLetter) {
         if (prevLetter) {
            toLower(ch);
         }
         else {
            prevLetter = true;
            toUpper(ch);
         }
      }
      else {
         prevLetter = false;
      }
   }

   return value;
}


p_str F_Join::getValue()
{
   p_list values = arg1->getValue();

   switch (values.size()) {
      case 0: {
         return p_str();
      }
      case 1: {
         return values[0];
      }
      case 2: {
         return str(values[0], arg2->getValue(), values[1]);
      }
      default: {
         const p_str separator = arg2->getValue();

         if (separator.empty()) {
            p_stream ss;

            for (const p_str& val : values) {
               ss << val;
            }

            return ss.str();
         }
         else {
            p_stream ss;
            ss << values[0];

            for (p_size i = 1; i < values.size(); i++) {
               ss << separator;
               ss << values[i];
            }

            return ss.str();
         }
      }
   }
}


p_str F_Roman::getValue()
{
   const p_num base = arg1->getValue();
   if (base.state == NumberState::NaN) {
      return p_str();
   }
   
   p_nint number = base.toInt();

   if (number == NINT_ZERO) {
      if (base.state == NumberState::Double) {
         p_stream ss;
         appendFraction(base, ss);
         p_str result = ss.str();

         if (result.empty()) {
            result = CHAR_N;
         }
         else if (base.value.d < NDOUBLE_ZERO) {
            result = str(CHAR_MINUS, result);
         }

         return result;
      }
      else {
         return toStr(CHAR_N);
      }
   }
   else if (number >= ROMAN_MAXIMUM || number <= -ROMAN_MAXIMUM) {
      return base.toString();
   }

   p_stream ss;
   if (number < NINT_ZERO) {
      ss << CHAR_MINUS;
      number *= NINT_MINUS_ONE;
   }

   const p_bool isBig = number >= ROMAN_VINCULUM_THRESHOLD;
   p_int i = 24;

   while (number > 0) {
      p_nint div = number / ROMAN_NUMBER_LITERALS[i];
      number = number % ROMAN_NUMBER_LITERALS[i];
      while (div--) {
         ss << ((isBig && i == 12) 
            ? ROMAN_VINCULUM_THOUSAND 
            : ROMAN_STRING_LITERALS[i]);
      }
      i--;
   }

   if (base.state == NumberState::Double) {
      appendFraction(base, ss);
   }

   return ss.str();
}


inline void F_Roman::appendFraction(const p_num& base, p_stream& ss) const
{
   p_int oc = static_cast<p_int>(std::fmod(base.value.d, NDOUBLE_ONE) * NDOUBLE_TWELVE);
   if (oc < 0) {
      oc *= -1;
   }
   if (oc >= 6) {
      oc -= 6;
      ss << CHAR_S;
   }

   if (oc > 0) {
      ss << p_str(oc,  CHAR_INTERPUNCT);
   }
}


p_str F_Binary::getValue()
{
   const p_num n = arg1->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   p_nint v = n.toInt();
   p_stream ss;
   p_bool negative = false;

   if (v < 0) {
      v *= -1;
      negative = true;
   }

   ss << std::bitset<BITS_IN_NINT>(v);
   const p_str val = ss.str();

   for (p_size i = 0; i < val.size(); i++) {
      if (val[i] != CHAR_0) {
         return negative
            ? str(CHAR_MINUS, val.substr(i))
            : val.substr(i);
      }
   }

   return toStr(CHAR_0);
}

p_str F_Hex::getValue()
{
   const p_num n = arg1->getValue();
   if (n.state == NumberState::NaN) {
      return p_str();
   }

   p_nint v = n.toInt();

   if (v < NINT_ZERO) {
      v *= NINT_MINUS_ONE;
      p_stream oss;
      oss << std::hex << v;
      return str(CHAR_MINUS, oss.str());
   }
   else {
      p_stream oss;
      oss << std::hex << v;
      return oss.str();
   }
}

}
