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

#include "wildcard.h"
#include "chars.h"


namespace perun2
{


WildcardComparer::WildcardComparer(const _str& pat)
   : pattern(pat), patternLength(pat.size()) { };


_bool WildcardComparer::matches(const _str& val)
{
   if (val.size() < this->minLength) {
      return false;
   }

   this->valuePtr = &val;
   this->clearCharStates();
   return this->checkState(val.size(), this->patternLength) == WildcardCharState::wcs_Matches;
}


void WildcardComparer::clearCharStates()
{
   if (this->charStates.empty()) {
      this->charStates.emplace_back(this->patternLength + 1, WildcardCharState::wcs_Unknown);
   }

   const _size prevSize = this->charStates.size() - 1;
   const _size nextSize = (*this->valuePtr).size();

   if (nextSize > prevSize) {
      this->charStates.reserve(nextSize + 1);

      for (_size i = 0; i <= prevSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), WildcardCharState::wcs_Unknown);
      }

      while (this->charStates.size() < nextSize + 1) {
         this->charStates.emplace_back(this->patternLength + 1, WildcardCharState::wcs_Unknown);
      }
   }
   else {
      for (_size i = 0; i <= nextSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), WildcardCharState::wcs_Unknown);
      }
   }
}


SimpleWildcardComparer::SimpleWildcardComparer(const _str& pat)
   : WildcardComparer(pat) { };


_size SimpleWildcardComparer::getMinLength(const _str& pat) const
{
   _size result = 0;

   for (const _char& ch : pat) {
      if (ch != CHAR_ASTERISK) {
         result++;
      }
   }

   return result;
}


WildcardCharState SimpleWildcardComparer::checkState(const _size n, const _size m)
{
   if (this->charStates[n][m] != WildcardCharState::wcs_Unknown) {
      return this->charStates[n][m];
   }

   if (n == 0 && m == 0) {
      this->charStates[n][m] = WildcardCharState::wcs_Matches;
      return this->charStates[n][m];
   }

   if (n > 0 && m == 0) {
      this->charStates[n][m] = WildcardCharState::wcs_NotMatches;
      return this->charStates[n][m];
   }

   WildcardCharState ans = WildcardCharState::wcs_NotMatches;

   if (this->pattern[m - 1] == CHAR_ASTERISK) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }
   else if (n > 0 && (this->pattern[m - 1] == (*this->valuePtr)[n - 1])) {
      ans = std::max(ans, this->checkState(n - 1, m - 1));
   }

   this->charStates[n][m] = ans;
   return this->charStates[n][m];
}

}
