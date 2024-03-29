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

#include "gen-double-asterisk.h"
#include "../../perun2.h"


namespace perun2::gen
{

DoubleAsteriskPattern::DoubleAsteriskPattern(p_rallptr& def, Perun2Process& p2, const p_str& pat, const p_str& pref, const p_int retr)
   : WildcardComparer(pat), definition(std::move(def)), context(definition->getFileContext()), perun2(p2), prefix(pref), 
      startId(pref.size()), specialStart(hasSpecialStart()), 
      hasRetreats(retr != 0), retreat(hasRetreats ? os_doubleDotsPrefix(retr) : p_str())
{   
   minLength = this->getMinLength(pat);
};


p_bool DoubleAsteriskPattern::setAction(p_daptr& act)
{
   return this->definition->setAction(act);
}


void DoubleAsteriskPattern::reset() {
   if (!first) {
      first = true;
      definition->reset();
   }
}

p_bool DoubleAsteriskPattern::hasNext()
{
   if (this->first) {
      this->index.setToZero();
      this->first = false;
   }

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->reset();
         break;
      }

      this->value = this->startId == 0
         ? this->definition->getValue()
         : str(this->prefix, this->definition->getValue());

      if (this->matches(this->value)) {
         if (this->hasRetreats) {
            this->value = str(this->retreat, this->value);
         }

         this->context->index->value = this->index;
         this->index++;
         return true;
      }
   }

   this->first = true;
   return false;
}

p_bool DoubleAsteriskPattern::hasSpecialStart() const
{
   return patternLength >= 2
       && pattern[0] == WILDCARD_DOUBLE_ASTERISK 
       && pattern[1] == OS_SEPARATOR;
};

p_size DoubleAsteriskPattern::getMinLength(const p_str& pat) const
{
   p_size result = 0;

   for (const p_char ch : pat) {
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

Logic DoubleAsteriskPattern::checkState(const p_size n, const p_size m)
{
   if (this->charStates[n][m] != Logic::Unknown) {
      return this->charStates[n][m];
   }

   if (n == this->startId && m == this->startId) {
      this->charStates[n][m] = Logic::True;
      return Logic::True;
   }

   if (n > this->startId && m == this->startId) {
      this->charStates[n][m] = Logic::False;
      return Logic::False;
   }

   if (n == 0 && m == 2 && this->specialStart) {
      this->charStates[n][m] = Logic::True;
      return Logic::True;
   }

   Logic ans = Logic::False;

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

            ans = std::max(ans, this->checkState(n - 1, m));
         }
         break;
      }
      default: {
         if (n > 0) {
            if (os_areEqualInPath(this->pattern[m - 1], (*this->valuePtr)[n - 1])) {
               ans = std::max(ans, this->checkState(n - 1, m - 1));
            }
         }
         break;
      }
   }

   this->charStates[n][m] = ans;
   return ans;
}

}
