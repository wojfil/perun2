/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "gen-like.h"
#include "../../hash.h"
#include <set>
#include <cwctype>


namespace uro::gen
{

LikeSet::LikeSet(const std::unordered_set<_char>& vals, const _bool neg)
   : values(vals), negated(neg) { };


_bool LikeSet::contains(const _char ch) const
{
   return this->negated
      ? this->values.find(ch) == this->values.end()
      : this->values.find(ch) != this->values.end();
}

static LikeSet makeLikeSet(const _str& pattern, _size startId, const _size endId)
{
   std::unordered_set<_char> set;
   _bool negated = false;

   if (pattern[startId] == WILDCARD_SET_EXCLUSION) {
      negated = true;
      startId++;
   }

   for (_size i = startId; i <= endId; i++) {
      if (i < endId - 1 && pattern[i + 1] == WILDCARD_SET_RANGE) {
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
   std::unordered_map<_size, LikeSet> sets;
   const _size length = pattern.size();
   _stream ss;
   _bool prevWasMulti = false;
   _size resultLength = 0;

   for (_size i = 0; i < length; i++) {
      const _char& ch = pattern[i];

      if (ch == WILDCARD_SET_START) {
         i++;

         if (i == length) {
            ss << WILDCARD_SET_START;
            result = std::make_unique<LC_Default>(ss.str(), sets);
            return;
         }

         const _size startId = i;

         while (pattern[i] != WILDCARD_SET_END) {
            i++;
            if (i == length) {
               ss << pattern.substr(startId - 1);
               result = std::make_unique<LC_Default>(ss.str(), sets);
               return;
            }
         }

         if (startId == i) {
            result = std::make_unique<LC_Constant>(false);
            return;
         }

         sets.emplace(resultLength, makeLikeSet(pattern, startId, i - 1));
         ss << WILDCARD_SET;
         resultLength++;
         prevWasMulti = false;
      }
      else {
         const _bool isMulti = (ch == WILDCARD_MULTIPLE_CHARS);
         if (!(isMulti && prevWasMulti)) {
            ss << ch;
            resultLength++;
         }
         prevWasMulti = isMulti;
      }
   }

   result = std::make_unique<LC_Default>(ss.str(), sets);
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
            case WILDCARD_MULTIPLE_CHARS: {
               result = std::make_unique<LC_Constant>(true);
               break;
            }
            case WILDCARD_ONE_CHAR: {
               result = std::make_unique<LC_ConstantLength>(1);
               break;
            }
            case WILDCARD_ONE_DIGIT: {
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

         if (fst == WILDCARD_SET_START || fst == WILDCARD_SET_END || snd == WILDCARD_SET_START || snd == WILDCARD_SET_END) {
            defaultLikeCmp(result, pattern);
            return;
         }

         switch (fst) {
            case WILDCARD_MULTIPLE_CHARS: {
               switch (snd) {
                  case WILDCARD_MULTIPLE_CHARS: {
                     result = std::make_unique<LC_Constant>(true);
                     break;
                  }
                  case WILDCARD_ONE_CHAR:
                  case WILDCARD_ONE_DIGIT: {
                     result = std::make_unique<LC_Default>(pattern);
                     break;
                  }
                  default: {
                     result = std::make_unique<LC_EndsWithChar>(pattern);
                     break;
                  }
               }
               break;
            }
            case WILDCARD_ONE_CHAR: {
               switch (snd) {
                  case WILDCARD_MULTIPLE_CHARS:
                  case WILDCARD_ONE_DIGIT: {
                     result = std::make_unique<LC_Default>(pattern);
                     break;
                  }
                  case WILDCARD_ONE_CHAR: {
                     result = std::make_unique<LC_ConstantLength>(2);
                     break;
                  }
                  default: {
                     result = std::make_unique<LC_UnderscoreStart>(pattern);
                     break;
                  }
               }
               break;
            }
            case WILDCARD_ONE_DIGIT: {
               if (snd == WILDCARD_ONE_DIGIT) {
                  result = std::make_unique<LC_OnlyDigits>(2);
               }
               else {
                  result = std::make_unique<LC_Default>(pattern);
               }
               break;
            }
            case WILDCARD_SET_EXCLUSION: {
               defaultLikeCmp(result, pattern);
               break;
            }
            default: {
               switch (snd) {
                  case WILDCARD_MULTIPLE_CHARS: {
                     result = std::make_unique<LC_StartsWithChar>(pattern);
                     break;
                  }
                  case WILDCARD_ONE_CHAR: {
                     result = std::make_unique<LC_UnderscoreEnd>(pattern);
                     break;
                  }
                  case WILDCARD_ONE_DIGIT: {
                     result = std::make_unique<LC_Default>(pattern);
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
         case WILDCARD_SET_START:
         case WILDCARD_SET_END:
         case WILDCARD_MULTIPLE_CHARS:
         case WILDCARD_SET_EXCLUSION: {
            defaultLikeCmp(result, pattern);
            return;
         }
         case WILDCARD_ONE_CHAR: {
            underscoresWithin++;
            break;
         }
         case WILDCARD_ONE_DIGIT:  {
            hashesWithin++;
            break;
         }
      }
   }

   switch (first) {
      case WILDCARD_ONE_CHAR:  {
         underscoresWithin++;
         break;
      }
      case WILDCARD_ONE_DIGIT:  {
         hashesWithin++;
         break;
      }
      case WILDCARD_SET_START:
      case WILDCARD_SET_END: {
         defaultLikeCmp(result, pattern);
         return;
      }
      case WILDCARD_MULTIPLE_CHARS:
      case WILDCARD_SET_EXCLUSION: {
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
         case WILDCARD_ONE_CHAR:  {
            underscoresWithin++;
            break;
         }
         case WILDCARD_ONE_DIGIT:  {
            hashesWithin++;
            break;
         }
         case WILDCARD_SET_START:
         case WILDCARD_SET_END:
         case WILDCARD_MULTIPLE_CHARS:
         case WILDCARD_SET_EXCLUSION: {
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

   if (underscoresWithin != 0 || hashesWithin != 0 || first == WILDCARD_ONE_DIGIT || last == WILDCARD_ONE_DIGIT) {
      defaultLikeCmp(result, pattern);
      return;
   }

   // wildcard on start and end
   switch (first) {
      case WILDCARD_MULTIPLE_CHARS: {
         switch (last) {
            case WILDCARD_MULTIPLE_CHARS: {
               result = std::make_unique<LC_Contains>(pattern);
               break;
            }
            case WILDCARD_ONE_CHAR: {
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
      case WILDCARD_ONE_CHAR: {
         switch (last) {
            case WILDCARD_MULTIPLE_CHARS: {
               result = std::make_unique<LC_UnderscorePercent>(pattern);
               break;
            }
            case WILDCARD_ONE_CHAR: {
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
            case WILDCARD_MULTIPLE_CHARS: {
               result = std::make_unique<LC_StartsWith>(pattern);
               break;
            }
            case WILDCARD_ONE_CHAR: {
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
_bool Like::getValue() 
{
   const _str pat = pattern->getValue();
   const _hash hsh = rawStringHash(pat);

   if (hsh != prevHash) {
      parseLikeCmp(comparer, pat);
      prevHash = hsh;
   }

   return comparer->compareToPattern(value->getValue());
};


LC_Default::LC_Default(const _str& pat, const std::unordered_map<_size, LikeSet>& cs)
   : pattern(pat), charSets(cs), patternLen(pat.size()), minLength(getMinLength(pat)) { };

LC_Default::LC_Default(const _str& pat)
   : pattern(pat), charSets({}), patternLen(pat.size()), minLength(getMinLength(pat)) { };


void LC_Default::clearCharStates()
{
   if (this->charStates.empty()) {
      this->charStates.emplace_back(this->patternLen + 1, LikeCharState::lcs_Unknown);
   }

   const _size prevSize = this->charStates.size() - 1;
   const _size nextSize = (*this->valuePtr).size();

   if (nextSize > prevSize) {
      this->charStates.reserve(nextSize + 1);

      for (_size i = 0; i <= prevSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), LikeCharState::lcs_Unknown);
      }

      while (this->charStates.size() < nextSize + 1) {
         this->charStates.emplace_back(this->patternLen + 1, LikeCharState::lcs_Unknown);
      }
   }
   else {
      for (_size i = 0; i <= nextSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), LikeCharState::lcs_Unknown);
      }
   }
}


LikeCharState LC_Default::checkState(const _size n, const _size m)
{
   if (this->charStates[n][m] >= LikeCharState::lcs_NotMatches) {
      return this->charStates[n][m];
   }

   if (n == 0 && m == 0) {
      this->charStates[n][m] = LikeCharState::lcs_Matches;
      return this->charStates[n][m];
   }

   if (n > 0 && m == 0) {
      this->charStates[n][m] = LikeCharState::lcs_NotMatches;
      return this->charStates[n][m];
   }

   LikeCharState ans = LikeCharState::lcs_NotMatches;

   if (this->pattern[m - 1] == WILDCARD_MULTIPLE_CHARS) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }

   if (n > 0) {
      const _char& pch = this->pattern[m - 1];

      switch (pch) {
         case WILDCARD_ONE_CHAR: {
            ans = std::max(ans, this->checkState(n - 1, m - 1));
            break;
         }
         case WILDCARD_ONE_DIGIT: {
            if (std::iswdigit((*this->valuePtr)[n - 1])) {
               ans = std::max(ans, this->checkState(n - 1, m - 1));
            }
            break;
         }
         case WILDCARD_SET: {
            if (this->charSets.at(m - 1).contains((*this->valuePtr)[n - 1])) {
               ans = std::max(ans, this->checkState(n - 1, m - 1));
            }
            break;
         }
         default: {
            if (pch == (*this->valuePtr)[n - 1]) {
               ans = std::max(ans, this->checkState(n - 1, m - 1));
            }
            break;
         }
      }
   }

   this->charStates[n][m] = ans;
   return this->charStates[n][m];
}


_size LC_Default::getMinLength(const _str& pat)
{
   _size result = 0;

   for (const _char ch : pat) {
      if (ch != WILDCARD_MULTIPLE_CHARS) {
         result++;
      }
   }

   return result;
}


_bool LC_Default::compareToPattern(const _str& value)
{
   if (value.size() < this->minLength) {
      return false;
   }

   this->valuePtr = &value;
   this->clearCharStates();
   return this->checkState(value.size(), this->patternLen) == LikeCharState::lcs_Matches;
}


LC_StartsWith::LC_StartsWith(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_bool LC_StartsWith::compareToPattern(const _str& value)
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


_bool LC_EndsWith::compareToPattern(const _str& value)
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


_bool LC_Contains::compareToPattern(const _str& value)
{
   return value.size() < length
      ? false
      : value.find(string) != _str::npos;
}


_bool LC_StartsWithChar::compareToPattern(const _str& value)
{
   return value.empty()
      ? false
      : value[0] == ch;
}


_bool LC_EndsWithChar::compareToPattern(const _str& value)
{
   const _size len = value.size();

   return len == 0
      ? false
      : value[len - 1] == ch;
}


_bool LC_ContainsChar::compareToPattern(const _str& value)
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


_bool LC_UnderscoreStart::compareToPattern(const _str& value)
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


_bool LC_UnderscoreEnd::compareToPattern(const _str& value)
{
   const _size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (_size i = 0; i < length - 1; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


_bool LC_UnderscoreStartEnd::compareToPattern(const _str& value)
{
   const _size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (_size i = 1; i < length - 1; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


_bool LC_Equals::compareToPattern(const _str& value)
{
   return value == pattern;
}


_bool LC_Constant::compareToPattern(const _str& value)
{
   return constant;
}


_bool LC_ConstantLength::compareToPattern(const _str& value)
{
   return value.size() == length;
}


LC_UnderscorePercent::LC_UnderscorePercent(const _str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


_bool LC_UnderscorePercent::compareToPattern(const _str& value)
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


_bool LC_PercentUnderscore::compareToPattern(const _str& value)
{
   const _size vlength = value.size();

   if (vlength < length) {
      return false;
   }

   const _size shift = vlength - length;

   for (_size i = vlength - length; i < vlength - 1; i++) {
      if (end[i - shift] != value[i]) {
         return false;
      }
   }

   return true;
}


_bool LC_OnlyDigits::compareToPattern(const _str& value)
{
   const _size vlength = value.size();
   if (vlength != length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      if (!std::iswdigit(value[i])) {
         return false;
      }
   }

   return true;
}


LC_Field_U::LC_Field_U(const _str& pat)
   : pattern(pat), length(pat.size()), isUnderscore(std::vector<_bool>(length))
{
   for (_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == WILDCARD_ONE_CHAR);
   }
}


_bool LC_Field_U::compareToPattern(const _str& value)
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
      isHash[i] = (pat[i] == WILDCARD_ONE_DIGIT);
   }
}


_bool LC_Field_H::compareToPattern(const _str& value)
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
   : pattern(pat), length(pat.size()) { }


_bool LC_Field_UH::compareToPattern(const _str& value)
{
   if (value.size() != length) {
      return false;
   }

   for (_size i = 0; i < length; i++) {
      switch (pattern[i]) {
         case WILDCARD_ONE_CHAR: {
            break;
         }
         case WILDCARD_ONE_DIGIT: {
            if (!std::iswdigit(value[i])) {
               return false;
            }
            break;
         }
         default: {
            if (value[i] != pattern[i]) {
               return false;
            }
            break;
         }
      }
   }

   return true;
}

}
