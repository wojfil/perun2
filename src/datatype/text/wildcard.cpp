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

#include "wildcard.hpp"
#include "chars.hpp"
#include "../../os/os.hpp"


namespace perun2
{


WildcardComparer::WildcardComparer(const p_str& pat)
   : pattern(pat), patternLength(pat.size()) { };


p_bool WildcardComparer::matches(const p_str& val)
{
   if (val.size() < this->minLength) {
      return false;
   }

   this->valuePtr = &val;
   this->clearCharStates();
   return this->checkState(val.size(), this->patternLength) == Logic::True;
}


void WildcardComparer::clearCharStates()
{
   if (this->charStates.empty()) {
      this->charStates.emplace_back(this->patternLength + 1, Logic::Unknown);
   }

   const p_size prevSize = this->charStates.size() - 1;
   const p_size nextSize = (*this->valuePtr).size();

   if (nextSize > prevSize) {
      this->charStates.reserve(nextSize + 1);

      for (p_size i = 0; i <= prevSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), Logic::Unknown);
      }

      while (this->charStates.size() < nextSize + 1) {
         this->charStates.emplace_back(this->patternLength + 1, Logic::Unknown);
      }
   }
   else {
      for (p_size i = 0; i <= nextSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), Logic::Unknown);
      }
   }
}


SimpleWildcardComparer::SimpleWildcardComparer(const p_str& pat)
   : WildcardComparer(pat) { };


p_size SimpleWildcardComparer::getMinLength(const p_str& pat) const
{
   p_size result = 0;

   for (const p_char ch : pat) {
      if (ch != CHAR_ASTERISK) {
         result++;
      }
   }

   return result;
}


Logic SimpleWildcardComparer::checkState(const p_size n, const p_size m)
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

   if (this->pattern[m - 1] == CHAR_ASTERISK) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }
   else if (n > 0 && os_areEqualInPath(this->pattern[m - 1], (*this->valuePtr)[n - 1])) {
      ans = std::max(ans, this->checkState(n - 1, m - 1));
   }

   this->charStates[n][m] = ans;
   return ans;
}

}
