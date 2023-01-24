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

#include "gen-string.h"
#include "../../var/var-runtime.h"
#include "../../os.h"


namespace uro::gen
{

_str ConcatString::getValue()
{
   _stream ss;
   for (_size i = 0; i < length; i++) {
      ss << value[i]->getValue();
   }
   return ss.str();
}

_str StringBinary::getValue()
{
   return condition->getValue()
      ? value->getValue()
      : EMPTY_STRING;
}

_str LocationReference::getValue()
{
   return this->inner.location.value;
}

_str RelativeLocation::getValue()
{
   return str(this->inner.location.value, OS_SEPARATOR_STRING, this->value->getValue());
}

_str CharAtIndex::getValue()
{
   const _str v = value->getValue();
   const _size len = v.size();

   if (len == 0) {
      return v;
   }

   const _nint n = index->getValue().toInt();

   return n >= 0 && n < len
      ? _str(1, v[n])
      : EMPTY_STRING;
}

_str DefinitionElement::getValue()
{
   _nint n = index->getValue().toInt();

   if (n < NINT_ZERO) {
      return EMPTY_STRING;
   }

   while (this->definition->hasNext()) {
      if (this->uroboros.state != State::s_Running) {
         this->definition.reset();
         return EMPTY_STRING;
      }

      if (n == NINT_ZERO) {
         const _str result = this->definition->getValue();
         this->definition.reset();
         return result;
      }

      n--;
   }

   return EMPTY_STRING;
}

}
