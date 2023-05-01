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

#include "func-list.h"
#include "../../lexer.h"
#include <cwctype>


namespace perun2::func
{

inline _list toChars(const _str& value)
{
   const _size len = value.size();
   _list r;
   r.reserve(len);

   for (_size i = 0; i < len; i++) {
      r.emplace_back(1, value[i]);
   }

   return r;
}


_list F_Characters::getValue()
{
   const _str v = arg1->getValue();
   return v.empty()
      ? _list()
      : toChars(v);
}


_list F_Split::getValue()
{
   _str v1 = arg1->getValue();
   const _size len1 = v1.size();

   if (len1 == 0) {
      return _list();
   }

   const _str v2 = arg2->getValue();
   const _size len2 = v2.size();

   switch (len2) {
      case 0: {
         return toChars(v1);
      }
      case 1: {
         _size start = 0;
         _list r;

         for (_size i = 0; i < len1; i++) {
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

         if (start == len1) {
            r.emplace_back();
         }
         else {
            r.emplace_back(v1.substr(start));
         }

         return r;
      }
      default: {
         _size index = v1.find(v2);
         _list r;

         while (index != _str::npos) {
            r.emplace_back(v1.substr(0, index));
            v1 = v1.substr(index + len2);
            index = v1.find(v2);
         }

         r.emplace_back(v1);
         return r;
      }
   }
}


_list F_Words::getValue()
{
   _str value = arg1->getValue();
   const _size len = value.size();

   switch (len) {
      case 0: {
         return _list();
      }
      case 1: {
         return std::iswalpha(value[0]) ? _list{value} : _list();
      }
      case 2: {
         if (std::iswalpha(value[0])) {
            if (std::iswalpha(value[1])) {
               return _list{value};
            }
            else {
               value.pop_back();
               return _list{value};
            }
         }
         else {
            if (std::iswalpha(value[1])) {
               value.erase(value.begin());
               return _list{value};
            }
            else {
               return _list();
            }
         }
      }
      default: {
         _list words;
         _bool prevLetter = false;
         _size start = 0;

         for (_size i = 0; i < len; i++) {
            const _bool isLetter = std::iswalpha(value[i]);
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

inline _nint F_Numbers::fromChar(const _char ch)
{
   return static_cast<_nint>(ch - CHAR_0);
}

_nlist F_Numbers::getValue()
{
   _str value = arg1->getValue();
   const _size len = value.size();

   switch (len) {
      case 0: {
         return _nlist();
      }
      case 1: {
         return std::iswdigit(value[0])
            ? _nlist{fromChar(value[0])}
            : _nlist();
      }
      case 2: {
         if (std::iswdigit(value[0])) {
            return _nlist {std::iswdigit(value[1])
               ? (NINT_TEN * fromChar(value[0]) + fromChar(value[1]))
               : fromChar(value[0])
            };
         }
         else {
            return std::iswdigit(value[1])
               ? _nlist{fromChar(value[1])}
               : _nlist();
         }
      }
      default: {
         _nlist numbers;
         _bool prevDigit = false;
         _size start = 0;

         for (_size i = 0; i < len; i++) {
            const _bool isDigit = std::iswdigit(value[i]);
            if (isDigit) {
               if (!prevDigit) {
                  start = i;
               }
            }
            else {
               if (prevDigit) {
                  try {
                     const _nint ii = std::stoll(value.substr(start, i - start));
                     numbers.emplace_back(ii);
                  }
                  catch(...) {
                     throw RuntimeError(str(L"number '", value.substr(start, i - start),
                        L"' cannot be stored in the memory"));
                  }
               }
            }
            prevDigit = isDigit;
         }

         if (prevDigit) {
            try {
               const _nint ii = std::stoll(value.substr(start));
               numbers.emplace_back(ii);
            }
            catch(...) {
               throw RuntimeError(str(L"number '", value.substr(start),
                  L"' cannot be stored in the memory"));
            }
         }

         return numbers;
      }
   }
}

}
