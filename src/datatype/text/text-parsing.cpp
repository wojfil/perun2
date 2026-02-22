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

#include "../../../include/perun2/datatype/text/text-parsing.hpp"
#include "../../../include/perun2/datatype/text/strings.hpp"


namespace perun2
{


p_str str_beforeLastComma(const p_str& value)
{
   for (p_int i = value.length() - 1; i >= 0; i--) {
      switch (value[i]) {
         case CHAR_SLASH:
         case CHAR_BACKSLASH: {
            return value;
         }
         case CHAR_COMMA: {
            return value.substr(0, i);
         }
      }

   }

   return value;
};


p_str str_firstArg(const p_str& value)
{
   if (value.empty()) {
      return value;
   }

   p_bool started = false;
   p_size start = 0;

   for (p_size i = 0; i < value.size(); i++) {
      if (value[i] == CHAR_QUOTATION_MARK) {
         if (started) {
            return value.substr(start + 1, i - start - 1);
         }
         else {
            start = i;
            started = true;
         }
      }
   }

   return EMPTY_STRING;
};


}
