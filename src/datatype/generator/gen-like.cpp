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


namespace uro::gen
{

// these two non-printable chars are used internally for the Like operator
// they symbolize:
// 1) lack of char
// 2) there is a set of characters at this position
inline constexpr _char LIKE_CHAR_NONE = L'\0';
inline constexpr _char LIKE_CHAR_SET = L'\1';


LikeSet::LikeSet(const std::unordered_set<_char>& vals, const _boo& neg)
   : values(vals), negated(neg) { };


_boo LikeSet::contains(const _char& ch) const
{
   return this->negated
      ? this->values.find(ch) == this->values.end()
      : this->values.find(ch) != this->values.end();
}

static LikeComparer* defaultLikeCmp(const _str& pattern)
{
   if (pattern.find(L'[') == _str::npos) {
      return new LC_Default_NoBrackets(pattern);
   }
   else {
      return bracketsLikeCmp(pattern);
   }
}

static LikeSet makeLikeSet(const _str& pattern, _size startId, const _size& endId)
{
    std::unordered_set<_char> set;
    _boo negated = false;

    if (pattern[startId] == L'^') {
        negated = true;
        startId++;
    }

    for (_size i = startId; i <= endId; i++) {
        if (i < endId - 1 && pattern[i + 1] == L'-') {
            const _char& left = pattern[i];
            const _char& right = pattern[i + 2];

            if (left < right) {
                for (_char ch = left; ch <= right; ch++) {
                    set.insert(ch);
                }
            }
            else {
                for (_char ch = right; ch <= left; ch++) {
                    set.insert(ch);
                }
            }

            i+= 2;
        }
        else {
            set.insert(pattern[i]);
        }
    }

    return LikeSet(set, negated);
}

static LikeComparer* bracketsLikeCmp(const _str& pattern)
{
   std::unordered_map<_int, LikeSet> sets;
   const _size length = pattern.size();
   std::wstringstream ss;
   _int id = 0;

   for (_size i = 0; i < length; i++) {
      const _char& ch = pattern[i];

      if (ch == L'[') {
         i++;

         if (i == length) {
            ss << L'[';
            return new LC_Default_WithBrackets(ss.str(), sets);
         }

         const _size startId = i;

         while (pattern[i] != L']') {
            i++;
            if (i == length) {
               ss << pattern.substr(startId - 1);
               return new LC_Default_WithBrackets(ss.str(), sets);
            }
         }

         if (startId == i) {
            return new LC_Constant(false);
         }

         sets.emplace(id, makeLikeSet(pattern, startId, i - 1));
         ss << LIKE_CHAR_SET;
      }
      else {
         ss << ch;
      }

      id++;
   }

   return new LC_Default_WithBrackets(ss.str(), sets);
}


// look for special case variants of the LIKE operator
// if one is detected
// return an optimized pattern comparer
LikeComparer* parseLikeCmp(const _str& pattern)
{
   const _size length = pattern.size();

   switch (length) {
      case 0: {
         return new LC_Constant(false);
      }
      case 1: {
         switch(pattern[0]) {
            case L'%':
               return new LC_Constant(true);
            case L'_':
               return new LC_ConstantLength(1);
            case L'#':
               return new LC_OnlyDigits(1);
            default:
               return new LC_Equals(pattern);
         }
      }
      case 2: {
         const _char& fst = pattern[0];
         const _char& snd = pattern[1];

         if (fst == L'[' || fst == L']' || snd == L'[' || snd == L']') {
            return defaultLikeCmp(pattern);
         }

         switch (fst) {
            case L'%': {
               switch (snd) {
                  case L'%':
                     return new LC_Constant(true);
                  case L'_':
                     return new LC_Default_NoBrackets(pattern);
                  case L'#':
                     return new LC_Default_NoBrackets(pattern);
                  default:
                     return new LC_EndsWithChar(pattern);
               }
            }
            case L'_': {
               switch (snd) {
                  case L'%':
                     return new LC_Default_NoBrackets(pattern);
                  case L'_':
                     return new LC_ConstantLength(2);
                  case L'#':
                     return new LC_Default_NoBrackets(pattern);
                  default:
                     return new LC_UnderscoreStart(pattern);
               }
            }
            case L'#': {
               switch (snd) {
                  case L'%':
                     return new LC_Default_NoBrackets(pattern);
                  case L'_':
                     return new LC_Default_NoBrackets(pattern);
                  case L'#':
                     return new LC_OnlyDigits(2);
                  default:
                     return new LC_Default_NoBrackets(pattern);
               }
            }
            case L'^': {
               return defaultLikeCmp(pattern);
            }
            default: {
               switch (snd) {
                  case L'%':
                     return new LC_StartsWithChar(pattern);
                  case L'_':
                     return new LC_UnderscoreEnd(pattern);
                  case L'#':
                     return new LC_Default_NoBrackets(pattern);
                  default:
                     return new LC_Equals(pattern);
               }
            }
         }
      }
   }

   // pattern length is 3 or greater

   const _char& first = pattern[0];
   const _char& last = pattern[length - 1];
   const _size limit = length - 1;

   _boo fieldFail = false;
   _int underscoresWithin = 0;
   _int hashesWithin = 0;

   for (_size i = 1; i < limit; i++) {
      switch (pattern[i]) {
         case L'[':
         case L']': {
            return bracketsLikeCmp(pattern);
         }
         case L'%':
         case L'^': {
            return defaultLikeCmp(pattern);
         }
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
      case L']': {
         return bracketsLikeCmp(pattern);
      }
      case L'%':
      case L'^': {
         fieldFail = true;
         if (underscoresWithin != 0 || hashesWithin != 0) {
            return defaultLikeCmp(pattern);
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
            return new LC_ConstantLength(length);
         }
         else if (hashesWithin == length) {
            return new LC_OnlyDigits(length);
         }

         if (underscoresWithin > 0) {
            if (hashesWithin > 0) {
                return new LC_Field_UH(pattern);
            }
            else {
                return new LC_Field_U(pattern);
            }
         }
         else {
            if (hashesWithin > 0) {
                return new LC_Field_H(pattern);
            }
            else {
                return new LC_Equals(pattern);
            }
         }
      }
   }

   if (underscoresWithin != 0 || hashesWithin != 0 || first == L'#' || last == L'#') {
      return defaultLikeCmp(pattern);
   }

   // wildcard on start and end
   switch (first) {
      case L'%': {
         switch (last) {
            case L'%':
               return new LC_Contains(pattern);
            case L'_':
               return new LC_PercentUnderscore(pattern);
            default:
               return new LC_EndsWith(pattern);
         }
      }
      case L'_': {
         switch (last) {
            case L'%':
               return new LC_UnderscorePercent(pattern);
            case L'_':
               return new LC_UnderscoreStartEnd(pattern);
            default:
               return new LC_UnderscoreStart(pattern);
         }
      }
      default: {
         switch (last) {
            case L'%':
               return new LC_StartsWith(pattern);
            case L'_':
               return new LC_UnderscoreEnd(pattern);
            default:
               return new LC_Equals(pattern);
         }
      }
   }
}


LikeConst::LikeConst(Generator<_str>* val, const _str& pattern)
  : value(val), comparer(parseLikeCmp(pattern)) { };


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

      comparer = parseLikeCmp(pat);
      prevHash = hsh;
      hasPrev = true;
   }

   return comparer->compareToPattern(value->getValue());
};


LC_Default_WithBrackets::LC_Default_WithBrackets(const _str& pat, const std::unordered_map<_int, LikeSet>& cs)
   : pattern(pat), charSets(cs), patternLen(pat.size()) { };


_boo LC_Default_WithBrackets::compareToPattern(const _str& value) const
{
   const _int vlen = value.size();
   _boo isMatch = true;
   _boo wildCardOn = false; // %
   _boo charWildCardOn = false; // _
   _boo end = false;
   _int lastWildCard = -1;
   _int id = 0;
   _char p = LIKE_CHAR_NONE;
   _size setId = 0;

   for (_int i = 0; i < vlen; i++) {
      const _char& c = value[i];
      end = (id >= this->patternLen);
      if (!end) {
         p = pattern[id];

         if (!wildCardOn && p == L'%') {
            lastWildCard = id;
            wildCardOn = true;
            while (id < this->patternLen && pattern[id] == L'%') {
               id++;
            }
            if (id >= this->patternLen) {
               p = LIKE_CHAR_NONE;
            }
            else {
               p = pattern[id];
            }
         }
         else if (p == L'_') {
            charWildCardOn = true;
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
         else if (p == LIKE_CHAR_SET) {
            if (end) {
               return false;
            }
            else if (charSets.at(id).contains(c)) {
               wildCardOn = false;
               id++;
            }
         }
         else if (p == c) {
            wildCardOn = false;
            id++;
         }
      }
      else if (charWildCardOn) {
         charWildCardOn = false;
      }
      else {
         if (p == L'#') {
            if (std::iswdigit(c)) {
               id++;
               if (id > this->patternLen) {
                  return false;
               }
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
         else if (p == LIKE_CHAR_SET) {
            if (end) {
               return false;
            }
            else if (charSets.at(id).contains(c)) {
               id++;
               if (id > this->patternLen) {
                  return false;
               }
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
         else if (p == c) {
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

   end = (id >= this->patternLen);

   if (isMatch && !end) {
      _boo onlyWildCards = true;
      for (_int i = id; i < this->patternLen; i++) {
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


LC_Default_NoBrackets::LC_Default_NoBrackets(const _str& pat)
   : pattern(pat), patternLen(pat.size()) { };


_boo LC_Default_NoBrackets::compareToPattern(const _str& value) const
{
   const _int vlen = value.size();
   _boo isMatch = true;
   _boo wildCardOn = false; // %
   _boo charWildCardOn = false; // _
   _boo end = false;
   _int lastWildCard = -1;
   _int id = 0;
   _char p = LIKE_CHAR_NONE;

   for (_int i = 0; i < vlen; i++) {
      const _char& c = value[i];
      end = (id >= this->patternLen);
      if (!end) {
         p = pattern[id];

         if (!wildCardOn && p == L'%') {
            lastWildCard = id;
            wildCardOn = true;
            while (id < this->patternLen && pattern[id] == L'%') {
               id++;
            }
            if (id >= this->patternLen) {
               p = LIKE_CHAR_NONE;
            }
            else {
               p = pattern[id];
            }
         }
         else if (p == L'_') {
            charWildCardOn = true;
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
      else {
         if (p == L'#') {
            if (std::iswdigit(c)) {
               id++;
               if (id > this->patternLen) {
                  return false;
               }
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

   end = (id >= this->patternLen);

   if (isMatch && !end) {
      _boo onlyWildCards = true;
      for (_int i = id; i < this->patternLen; i++) {
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


LC_StartsWith::LC_StartsWith(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_boo LC_StartsWith::compareToPattern(const _str& value) const
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


LC_EndsWith::LC_EndsWith(const _str& pat)
   : length(pat.size() - 1), end(pat.substr(1, length)) { };


_boo LC_EndsWith::compareToPattern(const _str& value) const
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


LC_Contains::LC_Contains(const _str& pat)
   : length(pat.size() - 2), string(pat.substr(1, length)) { };


_boo LC_Contains::compareToPattern(const _str& value) const
{
   return value.size() < length
      ? false
      : value.find(string) != _str::npos;
}


_boo LC_StartsWithChar::compareToPattern(const _str& value) const
{
   return value.empty()
      ? false
      : value[0] == ch;
}


_boo LC_EndsWithChar::compareToPattern(const _str& value) const
{
   const _size len = value.size();

   return len == 0
      ? false
      : value[len - 1] == ch;
}


_boo LC_ContainsChar::compareToPattern(const _str& value) const
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


_boo LC_UnderscoreStart::compareToPattern(const _str& value) const
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


_boo LC_UnderscoreEnd::compareToPattern(const _str& value) const
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


_boo LC_UnderscoreStartEnd::compareToPattern(const _str& value) const
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


_boo LC_Equals::compareToPattern(const _str& value) const
{
   return value == pattern;
}


_boo LC_Constant::compareToPattern(const _str& value) const
{
   return constant;
}


_boo LC_ConstantLength::compareToPattern(const _str& value) const
{
   return value.size() == length;
}


LC_UnderscorePercent::LC_UnderscorePercent(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_boo LC_UnderscorePercent::compareToPattern(const _str& value) const
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


LC_PercentUnderscore::LC_PercentUnderscore(const _str& pat)
   : length(pat.size() - 1), end(pat.substr(1, length)) { };


_boo LC_PercentUnderscore::compareToPattern(const _str& value) const
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


_boo LC_OnlyDigits::compareToPattern(const _str& value) const
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


LC_Field_U::LC_Field_U(const _str& pat)
   : pattern(pat), length(pat.size()), isUnderscore(std::vector<_boo>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == L'_');
   }
}


_boo LC_Field_U::compareToPattern(const _str& value) const
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


LC_Field_H::LC_Field_H(const _str& pat)
   : pattern(pat), length(pat.size()), isHash(std::vector<_boo>(length))
{
   for (_size i = 0; i < length; i++) {
      isHash[i] = (pat[i] == L'#');
   }
}


_boo LC_Field_H::compareToPattern(const _str& value) const
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


LC_Field_UH::LC_Field_UH(const _str& pat)
   : pattern(pat), length(pat.size()),
     isUnderscore(std::vector<_boo>(length)), isHash(std::vector<_boo>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == L'_');
      isHash[i] = (pat[i] == L'#');
   }
}


_boo LC_Field_UH::compareToPattern(const _str& value) const
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

}
