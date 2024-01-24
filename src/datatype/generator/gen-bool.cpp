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

#include "gen-bool.h"


namespace perun2::gen
{

Not::Not(p_genptr<p_bool>& val) 
   : UnaryOperation<p_bool>(val) { };

And::And(p_genptr<p_bool>& val1, p_genptr<p_bool>& val2)
   : BinaryOperation<p_bool>(val1, val2) { };

Or::Or(p_genptr<p_bool>& val1, p_genptr<p_bool>& val2)
   : BinaryOperation<p_bool>(val1, val2) { };

Xor::Xor(p_genptr<p_bool>& val1, p_genptr<p_bool>& val2)
   : BinaryOperation<p_bool>(val1, val2) { };

p_bool Not::getValue() 
{
   return !value->getValue();
}

p_bool And::getValue() 
{
   return value1->getValue() && value2->getValue();
}

p_bool Or::getValue() 
{
   return value1->getValue() || value2->getValue();
}

p_bool Xor::getValue() 
{
   return value1->getValue() ^ value2->getValue();
}

InConstTimeList::InConstTimeList(p_genptr<p_tim>& val, const p_tlist& li)
   : value(std::move(val)), list(li) { };

p_bool InConstTimeList::getValue() 
{
   const p_tim v = value->getValue();

   for (const p_tim& t : list) {
      if (v == t) {
         return true;
      }
   }

   return false;
}

}
