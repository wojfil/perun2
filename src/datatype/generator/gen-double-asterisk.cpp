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

DoubleAsteriskPattern::DoubleAsteriskPattern(_rallptr& def, _uro& uro, const _str& pat, const _size& dpth, const _size& start)
   : definition(std::move(def)), context(definition->getFileContext()), uroboros(uro), defaultDepth(dpth), startId(start),
      pattern(pat), patternLength(pat.size()) { };


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
      value = definition->getValue();

      if (this->matchesPattern()) {
         this->context->index->value = index;
         this->context->v_depth->value -= static_cast<_nint>(defaultDepth);
         index++;
         return true;
      }
   }

   first = true;
   return false;
}

_bool DoubleAsteriskPattern::matchesPattern()
{
   this->valuePtr = &value;
   this->clearCharStates();
   return this->checkState(value.size(), this->patternLength) == CharState::cs_Matches;
}

void DoubleAsteriskPattern::clearCharStates()
{
   if (this->charStates.empty()) {
      this->charStates.emplace_back(this->patternLength + 1, CharState::cs_Unknown);
   }

   const _size prevSize = this->charStates.size() - 1;
   const _size nextSize = (*this->valuePtr).size();

   if (nextSize > prevSize) {
      this->charStates.reserve(nextSize + 1);

      for (_size i = 0; i <= prevSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), CharState::cs_Unknown);
      }

      while (this->charStates.size() < nextSize + 1) {
         this->charStates.emplace_back(this->patternLength + 1, CharState::cs_Unknown);
      }
   }
   else {
      for (_size i = 0; i <= nextSize; i++) {
         std::fill(this->charStates[i].begin(), this->charStates[i].end(), CharState::cs_Unknown);
      }
   }
}

CharState DoubleAsteriskPattern::checkState(const _size& n, const _size& m)
{
   if (this->charStates[n][m] >= CharState::cs_NotMatches) {
      return this->charStates[n][m];
   }

   if (n == 0 && m == 0) {
      this->charStates[n][m] = CharState::cs_Matches;
      return this->charStates[n][m];
   }

   if (n > 0 && m == 0) {
      this->charStates[n][m] = CharState::cs_NotMatches;
      return this->charStates[n][m];
   }

   CharState ans = CharState::cs_NotMatches;

   if (this->pattern[m - 1] == WILDCARD_DOUBLE_ASTERISK) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }
   else if (this->pattern[m - 1] == WILDCARD_SINGLE_ASTERISK) { // todo add separator as exception
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }

   if (n > 0 && charsEqualInsensitive(this->pattern[m - 1], (*this->valuePtr)[n - 1])) {
      ans = std::max(ans, this->checkState(n - 1, m - 1));
   }

   this->charStates[n][m] = ans;
   return this->charStates[n][m];
}










}
