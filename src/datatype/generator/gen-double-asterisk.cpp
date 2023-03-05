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

#include "gen-double-asterisk.h"
#include "../../uroboros.h"


namespace uro::gen
{

DoubleAsteriskPattern::DoubleAsteriskPattern(_rallptr& def, _uro& uro, const _str& pat, const _str& pref)
   : WildcardComparer(pat), definition(std::move(def)), context(definition->getFileContext()), uroboros(uro), preffix(pref), 
      startId(pref.size()), specialStart(hasSpecialStart()) 
{   
   minLength = this->getMinLength(pat);
};


void DoubleAsteriskPattern::reset() {
   if (!first) {
      first = true;
      definition->reset();
   }
}

_bool DoubleAsteriskPattern::hasNext()
{
   if (first) {
      index.setToZero();
      first = false;
   }

   while (definition->hasNext() && this->uroboros.state == State::s_Running) {
      value = this->startId == 0
         ? definition->getValue()
         : str(this->preffix, definition->getValue());

      if (this->matches(value)) {
         this->context->index->value = index;
         index++;
         return true;
      }
   }

   first = true;
   return false;
}

_bool DoubleAsteriskPattern::hasSpecialStart() const
{
   return patternLength >= 2
       && pattern[0] == WILDCARD_DOUBLE_ASTERISK 
       && pattern[1] == OS_SEPARATOR;
};

_size DoubleAsteriskPattern::getMinLength(const _str& pat) const
{
   _size result = 0;

   for (const _char& ch : pat) {
      switch (ch) {
         case WILDCARD_SINGLE_ASTERISK:
         case WILDCARD_DOUBLE_ASTERISK: {
            break;
         }
         default: {
            result++;
            break;
         }
      }
   }

   return result;
}

WildcardCharState DoubleAsteriskPattern::checkState(const _size n, const _size m)
{
   if (this->charStates[n][m] >= WildcardCharState::wcs_NotMatches) {
      return this->charStates[n][m];
   }

   if (n == this->startId && m == this->startId) {
      this->charStates[n][m] = WildcardCharState::wcs_Matches;
      return this->charStates[n][m];
   }

   if (n > this->startId && m == this->startId) {
      this->charStates[n][m] = WildcardCharState::wcs_NotMatches;
      return this->charStates[n][m];
   }

   if (n == 0 && m == 2 && this->specialStart) {
      this->charStates[n][m] = WildcardCharState::wcs_Matches;
      return this->charStates[n][m];
   }

   WildcardCharState ans = WildcardCharState::wcs_NotMatches;

   switch (this->pattern[m - 1]) {
      case WILDCARD_SINGLE_ASTERISK: {
         ans = std::max(ans, this->checkState(n, m - 1));
         if (n > 0 && (*this->valuePtr)[n - 1] != OS_SEPARATOR) {
            ans = std::max(ans, this->checkState(n - 1, m));
         }
         break;
      }
      case WILDCARD_DOUBLE_ASTERISK: {
         ans = std::max(ans, this->checkState(n, m - 1));
         if (n > 0) {
            if (m >= 2 && this->pattern[m - 2] == OS_SEPARATOR && m < this->patternLength && this->pattern[m] == OS_SEPARATOR) {
               ans = std::max(ans, this->checkState(n, m - 2));
            }
            else {
               ans = std::max(ans, this->checkState(n - 1, m));
            }
         }
         break;
      }
      default: {
         if (n > 0) {
            if (this->pattern[m - 1] == (*this->valuePtr)[n - 1]) {
               ans = std::max(ans, this->checkState(n - 1, m - 1));
            }
         }
         break;
      }
   }

   this->charStates[n][m] = ans;
   return this->charStates[n][m];
}

}
