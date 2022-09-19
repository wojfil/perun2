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

#include "gen-string.h"
#include <sstream>
#include "../../var/var-runtime.h"


namespace uro::gen
{

_str ConcatString::getValue()
{
   std::wstringstream ss;
   for (_size i = 0; i < length; i++) {
      ss << (*value)[i]->getValue();
   }
   return ss.str();
};


_str StringBinary::getValue()
{
   return condition->getValue()
      ? value->getValue()
      : L"";
};

_str LocationReference::getValue()
{
   return this->inner->location.value;
}

_str CharAtIndex::getValue()
{
   const _str v = value->getValue();
   const _size len = v.size();

   if (len == 0) {
      return L"";
   }

   const _nint n = index->getValue().toInt();

   return n >= 0 && n < len
      ? _str(1, v[n])
      : L"";
}

_str DefinitionElement::getValue()
{
   _nint n = index->getValue().toInt();

   if (n < 0LL) {
      return L"";
   }

   while (definition->hasNext()) {
      if (n == 0LL) {
         const _str result = definition->getValue();
         definition->reset();
         return result;
      }

      n--;
   }

   return L"";
}

}
