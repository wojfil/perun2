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

#include "wildcard.h"


namespace uro
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


}
