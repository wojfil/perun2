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
#include <iomanip>


_str F_After::getValue()
{
   const _str s1 = arg1->getValue();
   if (s1 == L"")
      return L"";

   const _str s2 = arg2->getValue();
   if (s2 == L"")
      return L"";

   const _size len2 = s2.size();

   if (len2 == 1) {
      const _size len1 = s1.size();
      const _char& sign = s2[0];

      for (_size i = 0; i < len1; i++) {
         if (s1[i] == sign) {
            return i == len1 - 1
               ? L""
               : s1.substr(i + 1);
         }
      }

      return L"";
   }
   else {
      auto pos = s1.find(s2);
      return pos == std::wstring::npos
         ? L""
         : s1.substr(pos + s2.size());
   }
}


_str F_Before::getValue()
{
   const _str s1 = arg1->getValue();
   if (s1 == L"")
      return L"";

   const _str s2 = arg2->getValue();
   if (s2 == L"")
      return L"";

   const _size len2 = s2.size();

   if (len2 == 1) {
      const _size len1 = s1.size();
      const _char& sign = s2[0];

      for (_size i = 0; i < len1; i++) {
         if (s1[i] == sign) {
            return i == 0
               ? L""
               : s1.substr(0, i);
         }
      }

      return L"";
   }
   else {
      auto pos = s1.find(s2);
      return pos == std::wstring::npos
         ? L""
         : s1.substr(0, pos);
   }
}


_str F_Digits::getValue()
{
   const _str s1 = arg1->getValue();
   const _size len1 = s1.size();
   _size len2 = 0;
   _size it = 0;

   for (_size i = 0; i < len1; i++) {
      if (std::iswdigit(s1[i])) {
         len2++;
      }
   }

   _str s2(len2, L' ');
   for (_size i = 0; i < len1; i++) {
      const _char& ch = s1[i];
      if (std::iswdigit(ch)) {
         s2[it] = ch;
         it++;
      }
   }

   return s2;
}


_str F_Fill::getValue()
{
   const _str base = arg1->getValue();
   const _size len = base.size();
   const _size min = arg2->getValue().toInt();

   return len >= min
      ? base
      : str(_str(min - len, '0'), base);
}


_str F_Letters::getValue()
{
   const _str s1 = arg1->getValue();
   const _size len1 = s1.size();
   _size len2 = 0;
   _size it = 0;

   for (_size i = 0; i < len1; i++) {
      if (std::iswalpha(s1[i])) {
         len2++;
      }
   }

   _str s2(len2, L' ');
   for (_size i = 0; i < len1; i++) {
      const _char& ch = s1[i];
      if (std::iswalpha(ch)) {
         s2[it] = ch;
         it++;
      }
   }

   return s2;
}


_str F_Lower::getValue()
{
   _str value = arg1->getValue();
   if (!value.empty()) {
      toLower(value);
   }
   return value;
}


_str F_Trim::getValue()
{
   const _str value = arg1->getValue();
   const _int length = value.size();

   if (length == 0) {
      return value;
   }

   _int left;
   for (left = 0; left < length; left++) {
      if (!(std::iswspace(value[left]))) {
         break;
      }
   }

   if (left == length - 1) {
      return L"";
   }

   _int right;
   for (right = length - 1; right >= 0; --right) {
      if (!(std::iswspace(value[right]))) {
         break;
      }
   }

   return right == length - 1
      ? value.substr(left)
      : value.substr(left, right - left + 1);
}


_str F_Upper::getValue()
{
   _str value = arg1->getValue();
   if (!value.empty()) {
      toUpper(value);
   }
   return value;
}


_str F_Repeat::getValue()
{
   const _nint repeats = arg2->getValue().toInt();
   if (repeats <= 0LL) {
      return L"";
   }

   const _str base = arg1->getValue();
   const _size len = base.size();

   switch (len) {
      case 0: {
         return L"";
      }
      case 1: {
         return _str(repeats, base[0]);
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

   _str result;
   result.reserve(len * repeats);

   for (_nint i = 0LL; i < repeats; i++) {
      result += base;
   }

   return result;
}


_str F_Reverse::getValue()
{
   _str value = arg1->getValue();

   if (value.size() <= 1) {
      return value;
   }

   std::reverse(value.begin(), value.end());
   return value;
}


_str F_Left::getValue()
{
   const _nint left = arg2->getValue().toInt();
   if (left <= 0LL) {
      return L"";
   }

   const _str value = arg1->getValue();
   const _nint length = (_nint)value.size();

   return left >= length
      ? value
      : value.substr(0, left);
}


_str F_Right::getValue()
{
   const _nint right = arg2->getValue().toInt();
   if (right <= 0LL) {
      return L"";
   }

   const _str value = arg1->getValue();
   const _nint length = (_nint)value.size();

   return right >= length
      ? value
      : value.substr(length - right);
}


_str F_Substring_2::getValue()
{
   _nint index = arg2->getValue().toInt();
   const _str value = arg1->getValue();

   if (index == 0LL) {
      return value;
   }

   const _nint length = (_nint)value.size();

   if (index < 0LL) {
      index *= -1;

      return index >= length
         ? value
         : value.substr(length - index);
   }
   else {
      return index >= length
         ? L""
         : value.substr(index);
   }
}


_str F_Substring_3::getValue()
{
   const _str value = arg1->getValue();
   _nint index = arg2->getValue().toInt();
   _nint index2 = arg3->getValue().toInt();
   const _nint length = (_nint)value.size();

   if (index2 <= 0LL) {
      return L"";
   }

   if (index < 0LL) {
      index *= -1LL;

      if (index >= length) {
         const _nint lets = length - index + index2;
         return lets <= 0LL
            ? L""
            : value.substr(0, lets);
      }
      else {
         const _nint start = length - index;
         return start + index2 >= length
            ? value.substr(start)
            : value.substr(start, index2);
      }
   }
   else {
      if (index >= length) {
         return L"";
      }

      return index + index2 >= length
         ? value.substr(index)
         : value.substr(index, index2);
   }
}


_str F_ConcatenateUnit::getValue()
{
   const _list values = this->arg1->getValue();
   const _size length = values.size();

   switch (length) {
      case 0: {
         return L"";
      }
      case 1: {
         return values[0];
      }
      case 2: {
         return str(values[0], values[1]);
      }
   }

   std::wstringstream ss;

   for (_size i = 0; i < length; i++) {
      ss << values[i];
   }

   return ss.str();
}


F_Concatenate::~F_Concatenate()
{
   deleteVectorPtr(values);
}

_str F_Concatenate::getValue()
{
   std::wstringstream ss;

   for (_size i = 0; i < length; i++) {
      ss << (*values)[i]->getValue();
   }

   return ss.str();
}


_str F_FirstDef::getValue()
{
   if (definition->hasNext()) {
      const _str v = definition->getValue();
      definition->reset();
      return v;
   }
   else {
      return L"";
   }
}


_str F_LastDef::getValue()
{
   if (!definition->hasNext()) {
      return L"";
   }

   while (definition->hasNext()) { }
   return definition->getValue();
}


_str F_Replace::getValue()
{
   _str base = arg1->getValue();
   _size len = base.size();

   if (len == 0) {
      return base;
   }

   const _str v1 = arg2->getValue();
   const _str v2 = arg3->getValue();
   const _size len1 = v1.size();
   const _size len2 = v2.size();

   switch (len1) {
      case 0: {
         break;
      }
      case 1: {
         const _char& ch = v1[0];
         switch (len2) {
            case 0: {
               base.erase(std::remove(base.begin(), base.end(), ch), base.end());
               break;
            }
            case 1: {
               std::replace(base.begin(), base.end(), ch, v2[0]);
               break;
            }
            default: {
               for (_size i = 0; i < len; i++) {
                  if (base[i] == ch) {
                     if (i == 0) {
                        base = str(v2, base.substr(1));
                     }
                     else if (i == len - 1) {
                        return str(base.substr(0, len - 1), v2);
                     }
                     else {
                        base = str(base.substr(0, i), v2, base.substr(i + 1));
                     }

                     i += len2 - 1;
                     len += len2 - 1;
                  }
               }
               break;
            }
         }
         break;
      }
      default: {
         size_t i = 0;

         switch (len2) {
            case 0: {
               while ((i = base.find(v1)) != _str::npos) {
                  base.erase(i, len1);
               }
               break;
            }
            default: {
               while ((i = base.find(v1, i)) != _str::npos) {
                  base.replace(i, len1, v2);
                  i += len2;
               }
               break;
            }
         }
         break;
      }
   }

   return base;
}


_str F_String_B::getValue()
{
   return arg1->getValue() ? L"1" : L"0";
}


_str F_String_N::getValue()
{
   return arg1->getValue().toString();
}


_str F_String_T::getValue()
{
   return arg1->getValue().toString();
}


_str F_String_P::getValue()
{
   return arg1->getValue().toString();
}


_str F_MonthName::getValue()
{
   switch (arg1->getValue().toInt()) {
      case 1LL:
         return L"January";
      case 2LL:
         return L"February";
      case 3LL:
         return L"March";
      case 4LL:
         return L"April";
      case 5LL:
         return L"May";
      case 6LL:
         return L"June";
      case 7LL:
         return L"July";
      case 8LL:
         return L"August";
      case 9LL:
         return L"September";
      case 10LL:
         return L"October";
      case 11LL:
         return L"November";
      case 12LL:
         return L"December";
      default:
         return L"";
   }
}


_str F_WeekDayName::getValue()
{
   switch (arg1->getValue().toInt()) {
      case 1LL:
         return L"Monday";
      case 2LL:
         return L"Tuesday";
      case 3LL:
         return L"Wednesday";
      case 4LL:
         return L"Thursday";
      case 5LL:
         return L"Friday";
      case 6LL:
         return L"Saturday";
      case 7LL:
         return L"Sunday";
      default:
         return L"";
   }
}


_str F_AfterDigits::getValue()
{
   const _str value = arg1->getValue();
   const _size len = value.size();
   _boo after = false;

   for (_size i = 0; i < len; i++) {
      if (after) {
         switch (value[i]) {
            case L'0': case L'1':
            case L'2': case L'3':
            case L'4': case L'5':
            case L'6': case L'7':
            case L'8': case L'9':
            {
               break;
            }
            default: {
               return value.substr(i);
            }
         }
      }
      else {
         switch (value[i]) {
            case L'0': case L'1':
            case L'2': case L'3':
            case L'4': case L'5':
            case L'6': case L'7':
            case L'8': case L'9':
            {
               after = true;
               break;
            }
         }
      }
   }

   return L"";
}


_str F_AfterLetters::getValue()
{
   const _str value = arg1->getValue();
   const _size len = value.size();
   _boo after = false;

   for (_size i = 0; i < len; i++) {
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

   return L"";
}


_str F_BeforeDigits::getValue()
{
   const _str value = arg1->getValue();
   const _size len = value.size();

   for (_size i = 0; i < len; i++) {
      switch (value[i]) {
         case L'0': case L'1':
         case L'2': case L'3':
         case L'4': case L'5':
         case L'6': case L'7':
         case L'8': case L'9':
         {
            return i == 0
               ? L""
               : value.substr(0, i);
         }
      }
   }

   return L"";
}


_str F_BeforeLetters::getValue()
{
   const _str value = arg1->getValue();
   const _size len = value.size();

   for (_size i = 0; i < len; i++) {
      if (std::iswalpha(value[i])) {
         return i == 0
            ? L""
            : value.substr(0, i);
      }
   }

   return L"";
}


_str F_RandomChar::getValue()
{
   const _str value = arg1->getValue();
   const _size len = value.size();

   switch (len) {
      case 0: {
         return L"";
      }
      case 1: {
         return value;
      }
      default: {
         return _str(1, value[g_math->randomInt(len - 1)]);
      }
   }
}


_str F_Capitalize::getValue()
{
   _str value = arg1->getValue();
   const _size len = value.size();

   if (len == 0) {
      return value;
   }

   _boo prevLetter = false;

   for (_size i = 0; i < len; i++) {
      const _char& ch = value[i];
      const _boo isLetter = std::iswalpha(ch);

      if (isLetter) {
         if (prevLetter) {
            toLowerChar(value[i]);
         }
         else {
            prevLetter = true;
            toUpperChar(value[i]);
         }
      }
      else {
         prevLetter = false;
      }
   }

   return value;
}


_str F_Join::getValue()
{
   _list values = arg1->getValue();
   const _size length = values.size();

   switch (length) {
      case 0: {
         return L"";
      }
      case 1: {
         return values[0];
      }
      case 2: {
         return str(values[0], arg2->getValue(), values[1]);
      }
      default: {
         const _str separator = arg2->getValue();

         if (separator == L"") {
            std::wstringstream ss;

            for (_size i = 0; i < length; i++) {
               ss << values[i];
            }

            return ss.str();
         }
         else {
            std::wstringstream ss;
            ss << values[0];

            for (_size i = 0; i < length; i++) {
               ss << separator;
               ss << values[i];
            }

            return ss.str();
         }
      }
   }
}


_str F_Roman::getValue()
{
   const _num base = arg1->getValue();
   _nint number = base.isDouble
      ? (_nint)base.toInt()
      : base.value.i;

   if (number == 0LL) {
      return L"N";
   }
   else if (number >= 5000000LL || number <= -5000000LL) {
      return base.toString();
   }

   std::wstringstream ss;
   if (number < 0LL) {
      ss << L"-";
      number *= -1LL;
   }

   static const _nint num[] = { 1LL, 4LL, 5LL, 9LL, 10LL, 40LL, 50LL, 90LL, 100LL, 400LL, 500LL, 900LL, 1000LL,
      4000LL, 5000LL, 9000LL, 10000LL, 40000LL, 50000LL, 90000LL, 100000LL, 400000LL, 500000LL, 900000LL, 1000000LL
   };
   static const _str sym[] = { L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M",
      (L"I" L"̅" L"V" L"̅"), (L"V" L"̅"), (L"I" L"̅" L"X" L"̅"), (L"X" L"̅"),
      (L"X" L"̅" L"L" L"̅"), (L"L" L"̅"), (L"X" L"̅" L"C" L"̅"),
      (L"C" L"̅"), (L"C" L"̅" L"D" L"̅"), (L"D" L"̅"),
      (L"C" L"̅" L"M" L"̅"), (L"M" L"̅")
   };

   const _boo isBig = number > 3999LL;
   _int i = 24;

   while (number > 0) {
      _nint div = number / num[i];
      number = number % num[i];
      while (div--) {
         ss << ((isBig && i == 12) ? (L"I" L"̅") : sym[i]);
      }
      i--;
   }

   if (base.isDouble) {
      _int oc = (_int)(std::fmod(base.value.d, 1.0L) * 12L);
      if (oc < 0) {
         oc *= -1;
      }
      if (oc >= 6) {
         oc -= 6;
         ss << L'S';
      }
      for (_int i = 0; i < oc; i++) {
         ss << L'·';
      }
   }

   return ss.str();
}


_str F_Binary::getValue()
{
   _nint v = arg1->getValue().toInt();
   std::wstringstream ss;
   _boo negative = false;

   if (v < 0) {
      v *= -1;
      negative = true;
   }

   ss << std::bitset<64>(v);
   const _str str_ = ss.str();
   const _size len = str_.size();

   for (_size i = 0; i < len; i++) {
      if (str_[i] != L'0') {
         return negative
            ? str(L"-", str_.substr(i))
            : str_.substr(i);
      }
   }

   return L"0";
}

_str F_Hex::getValue()
{
   _nint v = arg1->getValue().toInt();

   if (v < 0) {
      v *= -1;

      std::wostringstream oss;
      oss << std::hex << v;
      return str(L"-", oss.str());
   }

   std::wostringstream oss;
   oss << std::hex << v;
   return oss.str();
}

