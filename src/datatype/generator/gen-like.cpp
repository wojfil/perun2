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

#include "gen-like.h"
#include "../text/strings.h"
#include <set>


namespace perun2::gen
{

LikeSet::LikeSet(const std::unordered_set<p_char>& vals, const p_bool neg)
   : values(vals), negated(neg) { };


p_bool LikeSet::contains(const p_char ch) const
{
   return this->negated
      ? this->values.find(ch) == this->values.end()
      : this->values.find(ch) != this->values.end();
}

static LikeSet makeLikeSet(const p_str& pattern, p_size startId, const p_size endId)
{
   std::unordered_set<p_char> set;
   p_bool negated = false;

   if (pattern[startId] == WILDCARD_SET_EXCLUSION) {
      negated = true;
      startId++;
   }

   for (p_size i = startId; i <= endId; i++) {
      if (i < endId - 1 && pattern[i + 1] == WILDCARD_SET_RANGE) {
         const p_char left = pattern[i];
         const p_char right = pattern[i + 2];

         if (left < right) {
            for (p_char ch = left; ch <= right; ch++) {
               set.insert(ch);
            }
         }
         else {
            for (p_char ch = right; ch <= left; ch++) {
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

static void defaultLikeCmp(p_likeptr& result, const p_str& pattern)
{
   std::unordered_map<p_size, LikeSet> sets;
   const p_size length = pattern.size();
   p_stream ss;
   p_bool prevWasMulti = false;
   p_size resultLength = 0;

   for (p_size i = 0; i < length; i++) {
      const p_char ch = pattern[i];

      if (ch == WILDCARD_SET_START) {
         i++;

         if (i == length) {
            ss << WILDCARD_SET_START;
            result = std::make_unique<LC_Default>(ss.str(), sets);
            return;
         }

         const p_size startId = i;

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
         const p_bool isMulti = (ch == WILDCARD_MULTIPLE_CHARS);
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
void parseLikeCmp(p_likeptr& result, const p_str& pattern)
{
   const p_size length = pattern.size();

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
         const p_char fst = pattern[0];
         const p_char snd = pattern[1];

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

   const p_char first = pattern[0];
   const p_char last = pattern[length - 1];
   const p_size limit = length - 1;

   p_bool fieldFail = false;
   p_size underscoresWithin = 0;
   p_size hashesWithin = 0;

   for (p_size i = 1; i < limit; i++) {
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


LikeConst::LikeConst(p_genptr<p_str>& val, const p_str& pattern)
  : value(std::move(val))
{ 
   parseLikeCmp(comparer, pattern);
};


p_bool LikeConst::getValue() 
{
   return comparer->compareToPattern(value->getValue());
};


Like::Like(p_genptr<p_str>& val, p_genptr<p_str>& pat)
   : value(std::move(val)), pattern(std::move(pat)), prevPattern(pattern->getValue()) { };


// if the pattern of the operator LIKE is not a constan value (string literal, etc.)
// we have to generate a new pattern string for every its call
// usually, the pattern does not change
// so, for every call, generate a pattern string
// if the same as the previous one
// then use previously created pattern comparer
p_bool Like::getValue() 
{
   const p_str pat = pattern->getValue();

   if (pat != prevPattern) {
      parseLikeCmp(comparer, pat);
      prevPattern = pat;
   }

   return comparer->compareToPattern(value->getValue());
};


LC_Default::LC_Default(const p_str& pat, const std::unordered_map<p_size, LikeSet>& cs)
   : WildcardComparer(pat), charSets(cs) 
{ 
   minLength = this->getMinLength(pat);
};

LC_Default::LC_Default(const p_str& pat)
   : WildcardComparer(pat) 
{ 
   minLength = this->getMinLength(pat);
};


Logic LC_Default::checkState(const p_size n, const p_size m)
{
   if (this->charStates[n][m] != Logic::Unknown) {
      return this->charStates[n][m];
   }

   if (n == 0 && m == 0) {
      this->charStates[n][m] = Logic::True;
      return Logic::True;
   }

   if (n > 0 && m == 0) {
      this->charStates[n][m] = Logic::False;
      return Logic::False;
   }

   Logic ans = Logic::False;

   if (this->pattern[m - 1] == WILDCARD_MULTIPLE_CHARS) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }

   if (n > 0) {
      const p_char pch = this->pattern[m - 1];

      switch (pch) {
         case WILDCARD_ONE_CHAR: {
            ans = std::max(ans, this->checkState(n - 1, m - 1));
            break;
         }
         case WILDCARD_ONE_DIGIT: {
            if (char_isDigit((*this->valuePtr)[n - 1])) {
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
   return ans;
}


p_size LC_Default::getMinLength(const p_str& pat) const
{
   p_size result = 0;

   for (const p_char ch : pat) {
      if (ch != WILDCARD_MULTIPLE_CHARS) {
         result++;
      }
   }

   return result;
}


p_bool LC_Default::compareToPattern(const p_str& value)
{
   return this->matches(value);
}


LC_StartsWith::LC_StartsWith(const p_str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


p_bool LC_StartsWith::compareToPattern(const p_str& value)
{
   if (value.size() < length) {
      return false;
   }

   for (p_size i = 0; i < length; i++) {
      if (start[i] != value[i]) {
         return false;
      }
   }

   return true;
}


LC_EndsWith::LC_EndsWith(const p_str& pat)
   : length(pat.size() - 1), end(pat.substr(1, length)) { };


p_bool LC_EndsWith::compareToPattern(const p_str& value)
{
   const p_size vlength = value.size();

   if (vlength < length) {
      return false;
   }

   const p_size shift = vlength - length;

   for (p_size i = vlength - length; i < vlength; i++) {
      if (end[i - shift] != value[i]) {
         return false;
      }
   }

   return true;
}


LC_Contains::LC_Contains(const p_str& pat)
   : length(pat.size() - 2), string(pat.substr(1, length)) { };


p_bool LC_Contains::compareToPattern(const p_str& value)
{
   return value.size() < length
      ? false
      : value.find(string) != p_str::npos;
}


p_bool LC_StartsWithChar::compareToPattern(const p_str& value)
{
   return value.empty()
      ? false
      : value[0] == this->ch;
}


p_bool LC_EndsWithChar::compareToPattern(const p_str& value)
{
   return value.empty()
      ? false
      : value[value.size() - 1] == this->ch;
}


p_bool LC_ContainsChar::compareToPattern(const p_str& value)
{
   return value.find(this->ch) != p_str::npos;
}


p_bool LC_UnderscoreStart::compareToPattern(const p_str& value)
{
   const p_size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (p_size i = 1; i < length; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


p_bool LC_UnderscoreEnd::compareToPattern(const p_str& value)
{
   const p_size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (p_size i = 0; i < length - 1; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


p_bool LC_UnderscoreStartEnd::compareToPattern(const p_str& value)
{
   const p_size vlength = value.size();

   if (vlength != length) {
      return false;
   }

   for (p_size i = 1; i < length - 1; i++) {
      if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


p_bool LC_Equals::compareToPattern(const p_str& value)
{
   return value == pattern;
}


p_bool LC_Constant::compareToPattern(const p_str& value)
{
   return constant;
}


p_bool LC_ConstantLength::compareToPattern(const p_str& value)
{
   return value.size() == length;
}


LC_UnderscorePercent::LC_UnderscorePercent(const p_str& pat)
   : length(pat.size() - 1), start(pat.substr(0, length)) { };


p_bool LC_UnderscorePercent::compareToPattern(const p_str& value)
{
   if (value.size() < length) {
      return false;
   }

   for (p_size i = 1; i < length; i++) {
      if (start[i] != value[i]) {
         return false;
      }
   }

   return true;
}


LC_PercentUnderscore::LC_PercentUnderscore(const p_str& pat)
   : length(pat.size() - 1), end(pat.substr(1, length)) { };


p_bool LC_PercentUnderscore::compareToPattern(const p_str& value)
{
   const p_size vlength = value.size();

   if (vlength < length) {
      return false;
   }

   const p_size shift = vlength - length;

   for (p_size i = vlength - length; i < vlength - 1; i++) {
      if (end[i - shift] != value[i]) {
         return false;
      }
   }

   return true;
}


p_bool LC_OnlyDigits::compareToPattern(const p_str& value)
{
   const p_size vlength = value.size();
   if (vlength != length) {
      return false;
   }

   for (p_size i = 0; i < length; i++) {
      if (!char_isDigit(value[i])) {
         return false;
      }
   }

   return true;
}


LC_Field_U::LC_Field_U(const p_str& pat)
   : pattern(pat), length(pat.size()), isUnderscore(std::vector<p_bool>(length))
{
   for (p_size i = 0; i < length; i++) {
      isUnderscore[i] = (pat[i] == WILDCARD_ONE_CHAR);
   }
}


p_bool LC_Field_U::compareToPattern(const p_str& value)
{
   if (value.size() != length) {
      return false;
   }

   for (p_size i = 0; i < length; i++) {
      if (!isUnderscore[i] && value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


LC_Field_H::LC_Field_H(const p_str& pat)
   : pattern(pat), length(pat.size()), isHash(std::vector<p_bool>(length))
{
   for (p_size i = 0; i < length; i++) {
      isHash[i] = (pat[i] == WILDCARD_ONE_DIGIT);
   }
}


p_bool LC_Field_H::compareToPattern(const p_str& value)
{
   if (value.size() != length) {
      return false;
   }

   for (p_size i = 0; i < length; i++) {
      if (isHash[i]) {
         if (!char_isDigit(value[i])) {
            return false;
         }
      }
      else if (value[i] != pattern[i]) {
         return false;
      }
   }

   return true;
}


LC_Field_UH::LC_Field_UH(const p_str& pat)
   : pattern(pat), length(pat.size()) { }


p_bool LC_Field_UH::compareToPattern(const p_str& value)
{
   if (value.size() != length) {
      return false;
   }

   for (p_size i = 0; i < length; i++) {
      switch (pattern[i]) {
         case WILDCARD_ONE_CHAR: {
            break;
         }
         case WILDCARD_ONE_DIGIT: {
            if (!char_isDigit(value[i])) {
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
