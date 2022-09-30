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

#include "func-list.h"
#include <cwctype>
#include <sstream>


namespace uro::func
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
         const _char& ch = v2[0];
         _size start = 0;
         _list r;

         for (_size i = 0; i < len1; i++) {
            if (v1[i] == ch) {
               if (start == i) {
                  r.emplace_back();
               }
               else {
                  r.push_back(v1.substr(start, i - start));
               }
               start = i + 1;
            }
         }

         if (start == len1) {
            r.emplace_back();
         }
         else {
            r.push_back(v1.substr(start));
         }

         return r;
      }
      default: {
         std::size_t index = v1.find(v2);
         _list r;

         while (index != _str::npos) {
            r.push_back(v1.substr(0, index));
            v1 = v1.substr(index + len2);
            index = v1.find(v2);
         }

         r.push_back(v1);
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
                  words.push_back(value.substr(start, i - start));
               }
            }
            prevLetter = isLetter;
         }

         if (prevLetter) {
            words.push_back(value.substr(start));
         }

         return words;
      }
   }
}

inline _num F_Numbers::fromChar(const _char& ch)
{
   return _num(static_cast<_nint>(ch - L'0'));
}

_nlist F_Numbers::getValue()
{
   const _str value = arg1->getValue();
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
      default: {
         _nlist numbers;
         _bool digitSerie = false;
         _size start = 0;
         _bool hasDot = false;
         _size dotPoint = 0;
         _bool splitted = false;

         for (_size i = 0; i < len; i++) {
            const _char& ch = value[i];

            if (std::iswdigit(ch)) {
               if (!digitSerie) {
                  start = i;
                  digitSerie = true;
                  hasDot = false;
               }
            }
            else if (ch == L'.') {
               if (digitSerie) {
                  if (splitted) {
                     const _size sub = i - start + 1;
                     if (sub == 1) {
                        numbers.push_back(fromChar(value[start]));
                     }
                     else {
                        try {
                           const _nint ii = std::stoll(value.substr(start, sub));
                           numbers.emplace_back(ii);
                        }
                        catch (...) {
                           throw UroRuntimeException(str(L"number '", value.substr(start, sub),
                              L"' cannot be stored in the memory"));
                        }
                     }
                     digitSerie = false;
                  }
                  else if (hasDot) {
                     // split the number
                     const _size len1 = dotPoint - start;
                     if (len1 == 1) {
                        numbers.push_back(fromChar(value[start]));
                     }
                     else {
                        try {
                           const _nint ii = std::stoll(value.substr(start, len1));
                           numbers.emplace_back(ii);
                        }
                        catch (...) {
                           throw UroRuntimeException(str(L"number '", value.substr(start, len1),
                              L"' cannot be stored in the memory"));
                        }
                     }

                     const _size len2 = i - dotPoint - 1;
                     if (len2 == 1) {
                        numbers.push_back(fromChar(value[dotPoint + 1]));
                     }
                     else {
                        try {
                           const _nint ii = std::stoll(value.substr(dotPoint + 1, len2));
                           numbers.emplace_back(ii);
                        }
                        catch (...) {
                           throw UroRuntimeException(str(L"number '", value.substr(dotPoint + 1, len2),
                              L"' cannot be stored in the memory"));
                        }
                     }

                     splitted = true;
                     digitSerie = false;
                  }
                  else {
                     hasDot = true;
                     dotPoint = i;
                  }
               }
            }
            else {
               if (digitSerie) {
                  const _size sub = i - start + 1;
                  if (sub == 1) {
                     numbers.push_back(fromChar(value[start]));
                  }
                  else {
                     if (hasDot) {
                        std::wstringstream ss(value.substr(start, sub));
                        _ndouble n;
                        ss >> n;
                        numbers.emplace_back(n);
                     }
                     else {
                        try {
                           const _nint ii = std::stoll(value.substr(start, sub));
                           numbers.emplace_back(ii);
                        }
                        catch (...) {
                           throw UroRuntimeException(str(L"number '", value.substr(start, sub),
                              L"' cannot be stored in the memory"));
                        }
                     }
                  }

                  digitSerie = false;
               }

               splitted = false;
            }
         }

         if (digitSerie) {
            if (start == len - 1) {
               numbers.push_back(fromChar(value[len - 1]));
            }
            else {
               if (hasDot) {
                  std::wstringstream ss(value.substr(start));
                  _ndouble n;
                  ss >> n;
                  numbers.emplace_back(n);
               }
               else {
                  try {
                     const _nint ii = std::stoll(value.substr(start));
                     numbers.emplace_back(ii);
                  }
                  catch (...) {
                     throw UroRuntimeException(str(L"number '", value.substr(start),
                        L"' cannot stored in the memory"));
                  }
               }
            }
         }

         return numbers;
      }
   }
}

}
