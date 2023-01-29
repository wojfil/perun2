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

DoubleAsteriskPattern::DoubleAsteriskPattern(_rallptr& def, _uro& uro, const _size& dpth, const _size& start)
   : definition(std::move(def)), inner(uro.vars.inner), uroboros(uro),
     defaultDepth(dpth), startId(start) { };


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
         this->inner.index.value = index;
         index++;
         return true;
      }
   }

   first = true;
   return false;
}

_bool DoubleAsteriskPattern::matchesPattern()
{
   // todo
   return true;
}


}
