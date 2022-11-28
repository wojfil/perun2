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


LikeSet::LikeSet(const std::unordered_set<_char>& vals, const _bool& neg)
   : values(vals), negated(neg) { };


_bool LikeSet::contains(const _char& ch) const
{
   return this->negated
      ? this->values.find(ch) == this->values.end()
      : this->values.find(ch) != this->values.end();
}

static LikeSet makeLikeSet(const _str& pattern, _size startId, const _size& endId)
{
   std::unordered_set<_char> set;
   _bool negated = false;

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

static void defaultLikeCmp(_likeptr& result, const _str& pattern)
{
   if (pattern.find(L'[') == _str::npos) {
      result = std::make_unique<LC_Default_NoBrackets>(pattern);
   }
   else {
      bracketsLikeCmp(result, pattern);
   }
}
static void bracketsLikeCmp(_likeptr& result, const _str& pattern)
{
   std::unordered_map<_int, LikeSet> sets;
   const _size length = pattern.size();
   _stream ss;
   _int id = 0;

   for (_size i = 0; i < length; i++) {
      const _char& ch = pattern[i];

      if (ch == L'[') {
         i++;

         if (i == length) {
            ss << L'[';
            result = std::make_unique<LC_Default_WithBrackets>(ss.str(), sets);
            return;
         }

         const _size startId = i;

         while (pattern[i] != L']') {
            i++;
            if (i == length) {
               ss << pattern.substr(startId - 1);
               result = std::make_unique<LC_Default_WithBrackets>(ss.str(), sets);
               return;
            }
         }

         if (startId == i) {
            result = std::make_unique<LC_Constant>(false);
            return;
         }

         sets.emplace(id, makeLikeSet(pattern, startId, i - 1));
         ss << LIKE_CHAR_SET;
      }
      else {
         ss << ch;
      }

      id++;
   }

   result = std::make_unique<LC_Default_WithBrackets>(ss.str(), sets);
}


// look for special case variants of the LIKE operator
// if one is detected
// return an optimized pattern comparer
void parseLikeCmp(_likeptr& result, const _str& pattern)
{
   const _size length = pattern.size();

   switch (length) {
      case 0: {
         result = std::make_unique<LC_Constant>(false);
         return;
      }
      case 1: {
         switch(pattern[0]) {
            case L'%': {
               result = std::make_unique<LC_Constant>(true);
               break;
            }
            case L'_': {
               result = std::make_unique<LC_ConstantLength>(1);
               break;
            }
            case L'#': {
               result = std::make_unique<LC_OnlyDigits>(1);
               break;
            }
            default: {
               result = std::make_unique<LC_Equals>(pattern);
               break;
            }
         }
         return;
      }
      case 2: {
         const _char& fst = pattern[0];
         const _char& snd = pattern[1];

         if (fst == L'[' || fst == L']' || snd == L'[' || snd == L']') {
            defaultLikeCmp(result, pattern);
            return;
         }

         switch (fst) {
            case L'%': {
               switch (snd) {
                  case L'%': {
                     result = std::make_unique<LC_Constant>(true);
                     break;
                  }
                  case L'_': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  case L'#': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  default: {
                     result = std::make_unique<LC_EndsWithChar>(pattern);
                     break;
                  }
               }
               break;
            }
            case L'_': {
               switch (snd) {
                  case L'%': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  case L'_': {
                     result = std::make_unique<LC_ConstantLength>(2);
                     break;
                  }
                  case L'#': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  default: {
                     result = std::make_unique<LC_UnderscoreStart>(pattern);
                     break;
                  }
               }
               break;
            }
            case L'#': {
               switch (snd) {
                  case L'%': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  case L'_': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  case L'#': {
                     result = std::make_unique<LC_OnlyDigits>(2);
                     break;
                  }
                  default: {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
               }
               break;
            }
            case L'^': {
               defaultLikeCmp(result, pattern);
               break;
            }
            default: {
               switch (snd) {
                  case L'%': {
                     result = std::make_unique<LC_StartsWithChar>(pattern);
                     break;
                  }
                  case L'_': {
                     result = std::make_unique<LC_UnderscoreEnd>(pattern);
                     break;
                  }
                  case L'#': {
                     result = std::make_unique<LC_Default_NoBrackets>(pattern);
                     break;
                  }
                  default: {
                     result = std::make_unique<LC_Equals>(pattern);
                     break;
                  }
               }
               break;
            }
         }
         return;
      }
   }

   // pattern length is 3 or greater

   const _char& first = pattern[0];
   const _char& last = pattern[length - 1];
   const _size limit = length - 1;

   _bool fieldFail = false;
   _int underscoresWithin = 0;
   _int hashesWithin = 0;

   for (_size i = 1; i < limit; i++) {
      switch (pattern[i]) {
         case L'[':
         case L']': {
            bracketsLikeCmp(result, pattern);
            return;
         }
         case L'%':
         case L'^': {
            defaultLikeCmp(result, pattern);
            return;
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
         bracketsLikeCmp(result, pattern);
         return;
      }
      case L'%':
      case L'^': {
         fieldFail = true;
         if (underscoresWithin != 0 || hashesWithin != 0) {
            defaultLikeCmp(result, pattern);
            return;
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
            result = std::make_unique<LC_ConstantLength>(length);
            return;
         }
         else if (hashesWithin == length) {
            result = std::make_unique<LC_OnlyDigits>(length);
            return;
         }

         if (underscoresWithin > 0) {
            if (hashesWithin > 0) {
               result = std::make_unique<LC_Field_UH>(pattern);
            }
            else {
               result = std::make_unique<LC_Field_U>(pattern);
            }

         }
         else {
            if (hashesWithin > 0) {
               result = std::make_unique<LC_Field_H>(pattern);
            }
            else {
               result = std::make_unique<LC_Equals>(pattern);
            }
         }

         return;
      }
   }

   if (underscoresWithin != 0 || hashesWithin != 0 || first == L'#' || last == L'#') {
      defaultLikeCmp(result, pattern);
      return;
   }

   // wildcard on start and end
   switch (first) {
      case L'%': {
         switch (last) {
            case L'%': {
               result = std::make_unique<LC_Contains>(pattern);
               break;
            }
            case L'_': {
               result = std::make_unique<LC_PercentUnderscore>(pattern);
               break;
            }
            default: {
               result = std::make_unique<LC_EndsWith>(pattern);
               break;
            }
         }
         break;
      }
      case L'_': {
         switch (last) {
            case L'%': {
               result = std::make_unique<LC_UnderscorePercent>(pattern);
               break;
            }
            case L'_': {
               result = std::make_unique<LC_UnderscoreStartEnd>(pattern);
               break;
            }
            default: {
               result = std::make_unique<LC_UnderscoreStart>(pattern);
               break;
            }
         }
         break;
      }
      default: {
         switch (last) {
            case L'%': {
               result = std::make_unique<LC_StartsWith>(pattern);
               break;
            }
            case L'_': {
               result = std::make_unique<LC_UnderscoreEnd>(pattern);
               break;
            }
            default: {
               result = std::make_unique<LC_Equals>(pattern);
               break;
            }
         }
         break;
      }
   }
}


LikeConst::LikeConst(_genptr<_str>& val, const _str& pattern)
  : value(std::move(val))
{ 
   parseLikeCmp(comparer, pattern);
};


_bool LikeConst::getValue() 
{
   return comparer->compareToPattern(value->getValue());
};


Like::Like(_genptr<_str>& val, _genptr<_str>& pat)
   : value(std::move(val)), pattern(std::move(pat)) { };


// if the pattern of the operator LIKE is not a string literal
// we have to generate a new pattern string for every its call
// usually, the pattern does not change
// so, for every call, generate a pattern string and its hash
// if the hash is the same as hash from the previously used pattern
// then just use previous pattern comparer
_bool Like::getValue() {
   const _str pat = pattern->getValue();
   const _size hsh = rawStringHash(pat);

   if (hsh != prevHash) {
      parseLikeCmp(comparer, pat);
      prevHash = hsh;
   }

   return comparer->compareToPattern(value->getValue());
};


LC_Default_WithBrackets::LC_Default_WithBrackets(const _str& pat, const std::unordered_map<_int, LikeSet>& cs)
   : pattern(pat), charSets(cs), patternLen(pat.size()) { };


_bool LC_Default_WithBrackets::compareToPattern(const _str& value) const
{
   const _int vlen = value.size();
   _bool isMatch = true;
   _bool wildCardOn = false; // %
   _bool charWildCardOn = false; // _
   _bool end = false;
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
      _bool onlyWildCards = true;
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


_bool LC_Default_NoBrackets::compareToPattern(const _str& value) const
{
   const _int vlen = value.size();
   _bool isMatch = true;
   _bool wildCardOn = false; // %
   _bool charWildCardOn = false; // _
   _bool end = false;
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
      _bool onlyWildCards = true;
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


_bool LC_StartsWith::compareToPattern(const _str& value) const
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


_bool LC_EndsWith::compareToPattern(const _str& value) const
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


_bool LC_Contains::compareToPattern(const _str& value) const
{
   return value.size() < length
      ? false
      : value.find(string) != _str::npos;
}


_bool LC_StartsWithChar::compareToPattern(const _str& value) const
{
   return value.empty()
      ? false
      : value[0] == ch;
}


_bool LC_EndsWithChar::compareToPattern(const _str& value) const
{
   const _size len = value.size();

   return len == 0
      ? false
      : value[len - 1] == ch;
}


_bool LC_ContainsChar::compareToPattern(const _str& value) const
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


_bool LC_UnderscoreStart::compareToPattern(const _str& value) const
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


_bool LC_UnderscoreEnd::compareToPattern(const _str& value) const
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


_bool LC_UnderscoreStartEnd::compareToPattern(const _str& value) const
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


_bool LC_Equals::compareToPattern(const _str& value) const
{
   return value == pattern;
}


_bool LC_Constant::compareToPattern(const _str& value) const
{
   return constant;
}


_bool LC_ConstantLength::compareToPattern(const _str& value) const
{
   return value.size() == length;
}


LC_UnderscorePercent::LC_UnderscorePercent(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_bool LC_UnderscorePercent::compareToPattern(const _str& value) const
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


_bool LC_PercentUnderscore::compareToPattern(const _str& value) const
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


_bool LC_OnlyDigits::compareToPattern(const _str& value) const
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
   : pattern(pat), length(pat.size()), isUnderscore(std::vector<_bool>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == L'_');
   }
}


_bool LC_Field_U::compareToPattern(const _str& value) const
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
   : pattern(pat), length(pat.size()), isHash(std::vector<_bool>(length))
{
   for (_size i = 0; i < length; i++) {
      isHash[i] = (pat[i] == L'#');
   }
}


_bool LC_Field_H::compareToPattern(const _str& value) const
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
     isUnderscore(std::vector<_bool>(length)), isHash(std::vector<_bool>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == L'_');
      isHash[i] = (pat[i] == L'#');
   }
}


_bool LC_Field_UH::compareToPattern(const _str& value) const
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
