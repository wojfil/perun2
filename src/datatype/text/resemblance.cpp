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


namespace perun2
{

ResemblesConst::ResemblesConst(p_genptr<p_str>& val, const p_str& pat)
   : value(std::move(val)), pattern(pat)
{
   resemblancePreparation(this->pattern);
};


p_bool ResemblesConst::getValue()
{
   p_str v = this->value->getValue();
   resemblancePreparation(v);
   return str_resemblance(v, this->pattern) >= RESEMBLANCE_RATIO;
};


Resembles::Resembles(p_genptr<p_str>& val, p_genptr<p_str>& pat)
   : value(std::move(val)), pattern(std::move(pat)), prevPattern(pattern->getValue()) { };


p_bool Resembles::getValue()
{
   p_str v = this->value->getValue();
   p_str p = this->pattern->getValue();

   resemblancePreparation(v);
   resemblancePreparation(p);

   return str_resemblance(v, p) >= RESEMBLANCE_RATIO;
};


void resemblancePreparation(p_str& value)
{
   str_toLower(value);
   str_toRaw(value);
}


p_ndouble str_resemblance(const p_str& value, const p_str& pattern)
{
   return NDOUBLE_ZERO;
}


static p_int minOfThree(p_int a, p_int b, p_int c)
{
   return std::min(std::min(a, b), c);
}


static p_int damerauLevenshteinDistance(const p_str& str1, const p_str& str2)
{
   const p_int m = str1.length();
   const p_int n = str2.length();

   std::vector<std::vector<p_int>> dp(m + 1, std::vector<p_int>(n + 1, 0));

   for (p_int i = 0; i <= m; ++i) {
      for (p_int j = 0; j <= n; ++j) {
         if (i == 0) {
            dp[i][j] = j;
         }
         else if (j == 0) {
            dp[i][j] = i;
         }
         else {
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
   }

   return dp[m][n];
}


}
