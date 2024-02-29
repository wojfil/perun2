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

#include "resemblance.h"
#include "raw.h"
#include "../../logger.h"
#include <limits>


namespace perun2::gen
{

ResemblesConst::ResemblesConst(p_genptr<p_str>& val, const p_str& pat)
   : value(std::move(val)), pattern(pat)
{
   prepareForResemblance(this->pattern);
};


p_bool ResemblesConst::getValue()
{
   p_str v = this->value->getValue();
   prepareForResemblance(v);
   return str_resemblance(v, this->pattern) >= RESEMBLANCE_RATIO;
};


Resembles::Resembles(p_genptr<p_str>& val, p_genptr<p_str>& pat)
   : value(std::move(val)), pattern(std::move(pat)) { };


p_bool Resembles::getValue()
{
   p_str v = this->value->getValue();
   p_str p = this->pattern->getValue();

   prepareForResemblance(v);
   prepareForResemblance(p);

   return str_resemblance(v, p) >= RESEMBLANCE_RATIO;
};


void prepareForResemblance(p_str& value)
{
   str_toLower(value);
   str_toRaw(value);
}


p_ndouble str_resemblance(const p_str& value, const p_str& pattern)
{
   if (pattern.empty()) {
      return NDOUBLE_ONE;
   }

   if (value.empty()) {
      return NDOUBLE_ZERO;
   }

   p_int minimum = std::numeric_limits<p_int>::max();

   for (p_size i = 0; i < value.size(); i++) 
   {
      const p_str chunk = value.substr(i);
      const p_int next = multiDamerauLevenshteinDistance(chunk, pattern);
      if (next < minimum) {
         minimum = next;
      }
   }
   
   return NDOUBLE_ONE - (static_cast<p_ndouble>(minimum) / static_cast<p_ndouble>(pattern.size()));
}


static p_int minOfThree(p_int a, p_int b, p_int c)
{
   return std::min(std::min(a, b), c);
}


static p_int multiDamerauLevenshteinDistance(const p_str& str1, const p_str& str2)
{
   const p_size len1 = str1.length();
   const p_size len2 = str2.length();

   std::vector<std::vector<p_int>> dp(len1 + 1, std::vector<p_int>(len2 + 1, 0));

   for (p_size i = 0; i <= len1; i++) {
      dp[i][0] = i;
   }

   for (p_size j = 0; j <= len2; j++) {
      dp[0][j] = j;
   }

   for (p_size i = 1; i <= len1; i++) {
      for (p_size j = 1; j <= len2; j++) {
         dp[i][j] = minOfThree(
            dp[i - 1][j] + 1,
            dp[i][j - 1] + 1,
            dp[i - 1][j - 1] + (str1[i - 1] == str2[j - 1] ? 0 : 1)
         );

         if (i > 1 && j > 1 && str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1]) {
            dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + 1);
         }
      }
   }

   p_int min = dp[0][len2];

   for (p_size i = 1; i <= len1; i++) {
      p_int nv = dp[i][len2];
      if (nv < min) {
         min = nv;
      }
   }

   return min;
}


static p_int resemblanceMistakesAllowed(const p_str& pattern)
{
   return static_cast<p_int>(pattern.size()) / RESEMBLANCE_MISTAKES_ALLOWED;
}

}
