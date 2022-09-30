/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#include "gen-pattern.h"
#include "gen-string.h"
#include "../../os.h"


namespace uro::gen
{

_bool PatternParser::parse(const _str& pattern, const _int& asteriskId, _def*& result) const
{
   const _str trimmed = os_trim(pattern);

   if (trimmed == L"*") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), ELEM_ALL, OS_SEPARATOR_ASTERISK);
      return true;
   }
   else if (trimmed == L"**") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), ELEM_RECURSIVE_ALL, OS_SEPARATOR_ASTERISK);
      return true;
   }

   const _size length = trimmed.size();

   if (asteriskId == length - 1) {
      const _str p = str(OS_SEPARATOR_STRING, pattern.substr(0, length - 1), L"*");
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), ELEM_ALL, p);
      return true;
   }

   // todo
   return false;
};

}
