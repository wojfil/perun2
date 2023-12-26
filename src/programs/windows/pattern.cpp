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

#include "pattern.h"
#include "../../datatype/text/chars.h"


namespace perun2::prog
{

   
ProgramPatternComparer::ProgramPatternComparer(const p_str& pat)
   : WildcardComparer(pat) { };


p_size ProgramPatternComparer::getMinLength(const p_str& pat) const
{
   p_size result = 0;

   for (const p_char ch : pat) {
      if (ch != CHAR_ASTERISK && ch != CHAR_HASH) {
         result++;
      }
   }

   return result;
}


Logic ProgramPatternComparer::checkState(const p_size n, const p_size m)
{
   if (this->charStates[n][m] != Logic::Unknown) {
      return this->charStates[n][m];
   }

   if (n == 0 && m == 0) {
      this->charStates[n][m] = Logic::True;
      return this->charStates[n][m];
   }

   if (n > 0 && m == 0) {
      this->charStates[n][m] = Logic::False;
      return this->charStates[n][m];
   }

   Logic ans = Logic::False;

   if (this->pattern[m - 1] == CHAR_ASTERISK) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         ans = std::max(ans, this->checkState(n - 1, m));
      }
   }
   else if (this->pattern[m - 1] == CHAR_HASH) {
      ans = std::max(ans, this->checkState(n, m - 1));
      if (n > 0) {
         switch ((*this->valuePtr)[n - 1]) {
            case CHAR_DOT:
            case CHAR_0:
            case CHAR_1:
            case CHAR_2:
            case CHAR_3:
            case CHAR_4:
            case CHAR_5:
            case CHAR_6:
            case CHAR_7:
            case CHAR_8:
            case CHAR_9: {
               ans = std::max(ans, this->checkState(n - 1, m));
               break;
            }
         }
      }
   }
   else if (n > 0 && charsEqualInsensitive(this->pattern[m - 1], (*this->valuePtr)[n - 1])) {
      ans = std::max(ans, this->checkState(n - 1, m - 1));
   }

   this->charStates[n][m] = ans;
   return ans;
}


}
