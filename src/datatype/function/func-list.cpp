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

#include "func-list.h"
#include "../../lexer.h"


namespace perun2::func
{

inline p_list toChars(const p_str& value)
{
   p_list r;
   r.reserve(value.size());

   for (const p_char ch : value) {
      r.emplace_back(1, ch);
   }

   return r;
}


p_list F_Characters::getValue()
{
   const p_str v = arg1->getValue();
   return v.empty()
      ? p_list()
      : toChars(v);
}


p_list F_Split::getValue()
{
   p_str v1 = arg1->getValue();

   if (v1.empty()) {
      return p_list();
   }

   const p_str v2 = arg2->getValue();

   switch (v2.size()) {
      case 0: {
         return toChars(v1);
      }
      case 1: {
         p_size start = 0;
         p_list r;

         for (p_size i = 0; i < v1.size(); i++) {
            if (v1[i] == v2[0]) {
               if (start == i) {
                  r.emplace_back();
               }
               else {
                  r.emplace_back(v1.substr(start, i - start));
               }
               start = i + 1;
            }
         }

         if (start == v1.size()) {
            r.emplace_back();
         }
         else {
            r.emplace_back(v1.substr(start));
         }

         return r;
      }
      default: {
         const p_size len2 = v2.size();
         p_size index = v1.find(v2);
         p_list r;

         while (index != p_str::npos) {
            r.emplace_back(v1.substr(0, index));
            v1 = v1.substr(index + len2);
            index = v1.find(v2);
         }

         r.emplace_back(v1);
         return r;
      }
   }
}


p_list F_Words::getValue()
{
   p_str value = arg1->getValue();
   const p_size len = value.size();

   switch (len) {
      case 0: {
         return p_list();
      }
      case 1: {
         return char_isAlpha(value[0]) ? p_list{value} : p_list();
      }
      case 2: {
         if (char_isAlpha(value[0])) {
            if (char_isAlpha(value[1])) {
               return p_list{value};
            }
            else {
               value.pop_back();
               return p_list{value};
            }
         }
         else {
            if (char_isAlpha(value[1])) {
               value.erase(value.begin());
               return p_list{value};
            }
            else {
               return p_list();
            }
         }
      }
      default: {
         p_list words;
         p_bool prevLetter = false;
         p_size start = 0;

         for (p_size i = 0; i < len; i++) {
            const p_bool isLetter = char_isAlpha(value[i]);
            if (isLetter) {
               if (!prevLetter) {
                  start = i;
               }
            }
            else {
               if (prevLetter) {
                  words.emplace_back(value.substr(start, i - start));
               }
            }
            prevLetter = isLetter;
         }

         if (prevLetter) {
            words.emplace_back(value.substr(start));
         }

         return words;
      }
   }
}

inline p_nint F_Numbers::fromChar(const p_char ch)
{
   return static_cast<p_nint>(ch - CHAR_0);
}

p_nlist F_Numbers::getValue()
{
   p_str value = arg1->getValue();
   const p_size len = value.size();

   switch (len) {
      case 0: {
         return p_nlist();
      }
      case 1: {
         return char_isDigit(value[0])
            ? p_nlist{fromChar(value[0])}
            : p_nlist();
      }
      case 2: {
         if (char_isDigit(value[0])) {
            return p_nlist {char_isDigit(value[1])
               ? (NINT_TEN * fromChar(value[0]) + fromChar(value[1]))
               : fromChar(value[0])
            };
         }
         else {
            return char_isDigit(value[1])
               ? p_nlist{fromChar(value[1])}
               : p_nlist();
         }
      }
      default: {
         p_nlist numbers;
         p_bool prevDigit = false;
         p_size start = 0;

         for (p_size i = 0; i < len; i++) {
            const p_bool isDigit = char_isDigit(value[i]);
            if (isDigit) {
               if (!prevDigit) {
                  start = i;
               }
            }
            else {
               if (prevDigit) {
                  try {
                     const p_nint ii = std::stoll(value.substr(start, i - start));
                     numbers.emplace_back(ii);
                  }
                  catch (...) { }
               }
            }
            prevDigit = isDigit;
         }

         if (prevDigit) {
            try {
               const p_nint ii = std::stoll(value.substr(start));
               numbers.emplace_back(ii);
            }
            catch (...) { }
         }

         return numbers;
      }
   }
}

}
