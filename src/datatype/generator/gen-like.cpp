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

#include "gen-like.h"
#include "../../hash.h"
#include <set>
#include <cwctype>


_boo correctLikePattern(const _str& pattern)
{
   if (pattern.empty()) {
      return false;
   }

   const _size len = pattern.size();
   _int level = 0;

   for (_size i = 0; i < len; i++) {
      switch (pattern[i]) {
         case L'[': {
            level++;
            break;
         }
         case L']': {
            level--;
            if (level < 0) {
               return false;
            }
            break;
         }
         case L'^': {
            if (level == 0)
               return false;
         }
      }
   }

   return level == 0;
}


// look for special case variants of the LIKE operator
// if one is detected
// return an optimized pattern comparer
LikeComparer* parseLikeComparer(const _str& pattern)
{
   const _size length = pattern.size();

   switch (length) {
      case 0: {
         return new LikeComparer_Constant(false);
      }
      case 1: {
         switch(pattern[0]) {
            case L'%':
               return new LikeComparer_Constant(true);
            case L'_':
               return new LikeComparer_ConstantLength(1);
            case L'#':
               return new LikeComparer_OnlyDigits(1);
            case L'[':
            case L']':
            case L'^':
               return new LikeComparer_Constant(false);
            default:
               return new LikeComparer_Equals(pattern);
         }
      }
      case 2: {
         const _char& fst = pattern[0];
         const _char& snd = pattern[1];

         if (fst == L'[' || fst == L']' || snd == L'[' || snd == L']') {
            return new LikeComparer_Constant(false);
         }

         switch (fst) {
            case L'%': {
               switch (snd) {
                  case L'%':
                     return new LikeComparer_Constant(true);
                  case L'_':
                     return new LikeComparer_Default(pattern);
                  case L'^':
                     return new LikeComparer_Constant(false);
                  case L'#':
                     return new LikeComparer_Default(pattern);
                  default:
                     return new LikeComparer_EndsWithChar(pattern);
               }
            }
            case L'_': {
               switch (snd) {
                  case L'%':
                     return new LikeComparer_Default(pattern);
                  case L'_':
                     return new LikeComparer_ConstantLength(2);
                  case L'^':
                     return new LikeComparer_Constant(false);
                  case L'#':
                     return new LikeComparer_Default(pattern);
                  default:
                     return new LikeComparer_UnderscoreStart(pattern);
               }
            }
            case L'#': {
               switch (snd) {
                  case L'%':
                     return new LikeComparer_Default(pattern);
                  case L'_':
                     return new LikeComparer_Default(pattern);
                  case L'^':
                     return new LikeComparer_Constant(false);
                  case L'#':
                     return new LikeComparer_OnlyDigits(2);
                  default:
                     return new LikeComparer_Default(pattern);
               }
            }
            case L'^': {
               return new LikeComparer_Constant(false);
            }
            default: {
               switch (snd) {
                  case L'%':
                     return new LikeComparer_StartsWithChar(pattern);
                  case L'_':
                     return new LikeComparer_UnderscoreEnd(pattern);
                  case L'^':
                     return new LikeComparer_Constant(false);
                  case L'#':
                     return new LikeComparer_Default(pattern);
                  default:
                     return new LikeComparer_Equals(pattern);
               }
            }
         }
      }
   }

   // pattern length is 3 or greater

   if (!correctLikePattern(pattern)) {
      return new LikeComparer_Constant(false);
   }

   const _char& first = pattern[0];
   const _char& last = pattern[length - 1];
   const _size limit = length - 1;

   _boo fieldFail = false;
   _int underscoresWithin = 0;
   _int hashesWithin = 0;

   for (_size i = 1; i < limit; i++) {
      switch (pattern[i]) {
         case L'[':
         case L']':
         case L'%':
         case L'^':
            return new LikeComparer_Default(pattern);
         case L'_': {
            underscoresWithin++;
            break;
         }
         case L'#':  {
            hashesWithin++;
            break;
         }
      }
   }

   if (first == L'[' && last == L']') {
      _int minusId = -1;

      for (_int i = 1; i < limit; i++) {
         if (pattern[i] == L'-') {
            minusId = i;
            break;
         }
      }

      switch (minusId) {
         case -1: {
            return new LikeComparer_SingleSet(pattern);
         }
         case 1: {
            if (length == 3) {
               return new LikeComparer_Constant(false);
            }
            else {
               const _str p = pattern.substr(2, 1);
               return new LikeComparer_Equals(p);
            }
         }
         case 2: {
            if (length == 4) {
               const _str p = pattern.substr(1, 1);
               return new LikeComparer_Equals(p);
            }
            else {
               return new LikeComparer_SingleRange(pattern);
            }
         }
         default: {
            if (minusId == length - 2) {
               const _str p = pattern.substr(minusId - 1, 1);
               return new LikeComparer_Equals(p);
            }
            else {
               const _str p2 = pattern.substr(minusId - 2);
               return new LikeComparer_SingleRange(p2);
            }
         }
      }
   }

   switch (first) {
      case L'_':  {
         underscoresWithin++;
         break;
      }
      case L'#':  {
         hashesWithin++;
         break;
      }
      case L'[':
      case L']':
      case L'%':
      case L'^': {
         fieldFail = true;
         if (underscoresWithin != 0 || hashesWithin != 0) {
            return new LikeComparer_Default(pattern);
         }
         break;
      }
   }

   if (!fieldFail) {
      switch (last) {
         case L'_':  {
            underscoresWithin++;
            break;
         }
         case L'#':  {
            hashesWithin++;
            break;
         }
         case L'[':
         case L']':
         case L'%':
         case L'^': {
            fieldFail = true;
            break;
         }
      }

      if (!fieldFail) {
         if (underscoresWithin == length) {
            return new LikeComparer_ConstantLength(length);
         }
         else if (hashesWithin == length) {
            return new LikeComparer_OnlyDigits(length);
         }

         if (underscoresWithin > 0) {
            if (hashesWithin > 0) {
                return new LikeComparer_Field_UH(pattern);
            }
            else {
                return new LikeComparer_Field_U(pattern);
            }
         }
         else {
            if (hashesWithin > 0) {
                return new LikeComparer_Field_H(pattern);
            }
            else {
                return new LikeComparer_Equals(pattern);
            }
         }
      }
   }

   if (underscoresWithin != 0 || hashesWithin != 0 || first == L'#' || last == L'#') {

      return new LikeComparer_Default(pattern);
   }

   // wildcard on start and end
   switch (first) {
      case L'%': {
         switch (last) {
            case L'%':
               return new LikeComparer_Contains(pattern);
            case L'_':
               return new LikeComparer_PercentUnderscore(pattern);
            default:
               return new LikeComparer_EndsWith(pattern);
         }
      }
      case L'_': {
         switch (last) {
            case L'%':
               return new LikeComparer_UnderscorePercent(pattern);
            case L'_':
               return new LikeComparer_UnderscoreStartEnd(pattern);
            default:
               return new LikeComparer_UnderscoreStart(pattern);
         }
      }
      default: {
         switch (last) {
            case L'%':
               return new LikeComparer_StartsWith(pattern);
            case L'_':
               return new LikeComparer_UnderscoreEnd(pattern);
            default:
               return new LikeComparer_Equals(pattern);
         }
      }
   }
}

LikeConst::LikeConst(Generator<_str>* val, const _str pattern)
{
   value = val;
   comparer = parseLikeComparer(pattern);
}

_boo LikeConst::getValue() {
   return comparer->compareToPattern(value->getValue());
};

// if the pattern of the operator LIKE is not a string literal
// we have to generate a new pattern string for every its call
// usually, the pattern does not change
// so, for every call, generate a pattern string and its hash
// if the hash is the same as hash from the previously used pattern
// then just use previous pattern comparer
_boo Like::getValue() {
   const _str pat = pattern->getValue();
   const _size hsh = rawStringHash(pat);

   if ((!hasPrev) || hsh != prevHash) {
      if (hasPrev) {
         delete comparer;
      }

      comparer = parseLikeComparer(pat);
      prevHash = hsh;
      hasPrev = true;
   }

   return comparer->compareToPattern(value->getValue());
};


_boo LikeComparer_Default::compareToPattern(const _str& value) const
{
   const _int vlen = value.size();
   const _int plen = pattern.size();
   _boo isMatch = true;
   _boo wildCardOn = false; // %
   _boo charWildCardOn = false; // _
   _boo charSetOn = false;
   _boo notCharSetOn = false;
   _boo end = false;
   _int lastWildCard = -1;
   _int id = 0;
   std::set<_char> set;
   _char p = L'\0';

   for (_int i = 0; i < vlen; i++) {
      const _char& c = value[i];
      end = (id >= plen);
      if (!end) {
         p = pattern[id];

         if (!wildCardOn && p == L'%') {
            lastWildCard = id;
            wildCardOn = true;
            while (id < plen && pattern[id] == L'%') {
               id++;
            }
            if (id >= plen) {
               p = L'\0';
            }
            else {
               p = pattern[id];
            }
         }
         else if (p == L'_') {
            charWildCardOn = true;
            id++;
         }
         else if (p == L'[') {
            if (pattern[++id] == L'^') {
               notCharSetOn = true;
               id++;
            }
            else {
               charSetOn = true;
            }

            set.clear();

            if (pattern[id + 1] == L'-' && pattern[id + 3] == L']') {
               _char start = pattern[id];
               id += 2;
               _char end = pattern[id];
               if (start <= end) {
                  for (_char ci = start; ci <= end; ci++) {
                     set.insert(ci);
                  }
               }
               else {
                  for (_char ci = end; ci <= start; ci++) {
                     set.insert(ci);
                  }
               }
               id++;
            }

            while (id < plen && pattern[id] != L']') {
               set.insert(pattern[id]);
               id++;
            }
            id++;
         }
      }

      if (wildCardOn) {
         if (p == L'#') {
            if (std::iswdigit(c)) {
               wildCardOn = false;
               id++;
            }
         }
         else if (c == p) {
            wildCardOn = false;
            id++;
         }
      }
      else if (charWildCardOn) {
         charWildCardOn = false;
      }
      else if (charSetOn || notCharSetOn) {
         const _boo _charMatch = set.find(c) != set.end();

         if ((notCharSetOn && _charMatch) || (charSetOn && !_charMatch)) {
            if (lastWildCard >= 0) {
               id = lastWildCard;
            }
            else {
               isMatch = false;
               break;
            }
         }
         notCharSetOn = charSetOn = false;
      }
      else {
         if (p == L'#') {
            if (std::iswdigit(c)) {
               id++;
            }
            else {
               if (lastWildCard >= 0) {
                  id = lastWildCard;
               }
               else {
                  isMatch = false;
                  break;
               }
            }
         }
         else if (c == p) {
            id++;
         }
         else {
            if (lastWildCard >= 0) {
               id = lastWildCard;
            }
            else {
               isMatch = false;
               break;
            }
         }
      }
   }

   end = (id >= plen);

   if (isMatch && !end) {
      _boo onlyWildCards = true;
      for (_int i = id; i < plen; i++) {
         if (pattern[i] != L'%') {
            onlyWildCards = false;
            break;
         }
      }
      if (onlyWildCards) {
         end = true;
      }
   }

   return isMatch && end;
}


LikeComparer_StartsWith::LikeComparer_StartsWith(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_boo LikeComparer_StartsWith::compareToPattern(const _str& value) const
{
   if (value.size() < length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      if (start[i] != value[i]) {
         return false;
      }
   }

   return true;
}


LikeComparer_EndsWith::LikeComparer_EndsWith(const _str& pat)
   : length(pat.size() - 1), end(pat.substr(1, length)) { };


_boo LikeComparer_EndsWith::compareToPattern(const _str& value) const
{
   const _size vlength = value.size();

   if (vlength < length) {
      return false;
   }

   const _size shift = vlength - length;

   for (_size i = vlength - length; i < vlength; i++) {
      if (end[i - shift] != value[i]) {
         return false;
      }
   }

   return true;
}


LikeComparer_Contains::LikeComparer_Contains(const _str& pat)
   : length(pat.size() - 2), string(pat.substr(1, length)) { };


_boo LikeComparer_Contains::compareToPattern(const _str& value) const
{
   return value.size() < length
      ? false
      : value.find(string) != _str::npos;
}


_boo LikeComparer_StartsWithChar::compareToPattern(const _str& value) const
{
   return value.empty()
      ? false
      : value[0] == ch;
}


_boo LikeComparer_EndsWithChar::compareToPattern(const _str& value) const
{
   const _size len = value.size();

   return len == 0
      ? false
      : value[len - 1] == ch;
}


_boo LikeComparer_ContainsChar::compareToPattern(const _str& value) const
{
   const _size len = value.size();

   if (len == 0) {
      return false;
   }

   for (_size i = 0; i < len; i++) {
      if (value[i] == ch) {
         return true;
      }
   }

   return false;
}


_boo LikeComparer_UnderscoreStart::compareToPattern(const _str& value) const
{
   const _size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (_size i = 1; i < length; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


_boo LikeComparer_UnderscoreEnd::compareToPattern(const _str& value) const
{
   const _size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (_size i = 0; i < lengthMinusOne; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


_boo LikeComparer_UnderscoreStartEnd::compareToPattern(const _str& value) const
{
   const _size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (_size i = 1; i < lengthMinusOne; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


_boo LikeComparer_Equals::compareToPattern(const _str& value) const
{
   return value == pattern;
}


_boo LikeComparer_Constant::compareToPattern(const _str& value) const
{
   return constant;
}


_boo LikeComparer_ConstantLength::compareToPattern(const _str& value) const
{
   return value.size() == length;
}


LikeComparer_UnderscorePercent::LikeComparer_UnderscorePercent(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_boo LikeComparer_UnderscorePercent::compareToPattern(const _str& value) const
{
   if (value.size() < length) {
      return false;
   }

   for (_size i = 1; i < length; i++) {
      if (start[i] != value[i]) {
         return false;
      }
   }

   return true;
}


LikeComparer_PercentUnderscore::LikeComparer_PercentUnderscore(const _str& pat)
   : length(pat.size() - 1), end(pat.substr(1, length)) { };


_boo LikeComparer_PercentUnderscore::compareToPattern(const _str& value) const
{
   const _size vlength = value.size();

   if (vlength < length) {
      return false;
   }

   const _size vlengthMinusOne = vlength - 1;
   const _size shift = vlength - length;

   for (_size i = vlength - length; i < vlengthMinusOne; i++) {
      if (end[i - shift] != value[i]) {
         return false;
      }
   }

   return true;
}


_boo LikeComparer_OnlyDigits::compareToPattern(const _str& value) const
{
   const _size vlength = value.size();
   if (vlength != length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      if (!std::iswdigit(value[i]))
         return false;
   }

   return true;
}


LikeComparer_Field_U::LikeComparer_Field_U(const _str& pat)
   : pattern(pat), length(pat.size()), isUnderscore(std::vector<_boo>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == L'_');
   }
}


_boo LikeComparer_Field_U::compareToPattern(const _str& value) const
{
   if (value.size() != length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      if (!isUnderscore[i] && value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


LikeComparer_Field_H::LikeComparer_Field_H(const _str& pat)
   : pattern(pat), length(pat.size()), isHash(std::vector<_boo>(length))
{
   for (_size i = 0; i < length; i++) {
      isHash[i] = (pat[i] == L'#');
   }
}


_boo LikeComparer_Field_H::compareToPattern(const _str& value) const
{
   if (value.size() != length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      if (isHash[i]) {
         if (!std::iswdigit(value[i])) {
            return false;
         }
      }
      else if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


LikeComparer_Field_UH::LikeComparer_Field_UH(const _str& pat)
   : pattern(pat), length(pat.size()),
     isUnderscore(std::vector<_boo>(length)), isHash(std::vector<_boo>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == L'_');
      isHash[i] = (pat[i] == L'#');
   }
}


_boo LikeComparer_Field_UH::compareToPattern(const _str& value) const
{
   if (value.size() != length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      if (!isUnderscore[i]) {
         if (isHash[i]) {
            if (!std::iswdigit(value[i])) {
               return false;
            }
         }
         else if (value[i] != pattern[i]) {
            return false;
         }
      }
   }

   return true;
}


LikeComparer_SingleSet::LikeComparer_SingleSet(const _str& pat)
{
   const _size limit = pat.size() - 1;
   for (_size i = 1; i < limit; i++) {
      chars.insert(pat[i]);
   }
}


_boo LikeComparer_SingleSet::compareToPattern(const _str& value) const
{
   return value.size() == 1
     && chars.find(value[0]) != chars.end();
}


LikeComparer_SingleRange::LikeComparer_SingleRange(const _str& pat)
{
   const _char& ch1 = pat[1];
   const _char& ch2 = pat[3];

   if (ch1 < ch2) {
      first = ch1;
      last = ch2;
   }
   else {
      first = ch2;
      last = ch1;
   }
}


_boo LikeComparer_SingleRange::compareToPattern(const _str& value) const
{
   if (value.size() != 1) {
      return false;
   }

   const _char& ch = value[0];
   return ch >= first && ch <= last;
}

