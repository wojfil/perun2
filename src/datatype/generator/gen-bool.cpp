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

#include "gen-bool.h"


namespace perun2::gen
{

Not::Not(_genptr<_bool>& val) 
   : UnaryOperation<_bool>(val) { };

And::And(_genptr<_bool>& val1, _genptr<_bool>& val2)
   : BinaryOperation<_bool>(val1, val2) { };

Or::Or(_genptr<_bool>& val1, _genptr<_bool>& val2)
   : BinaryOperation<_bool>(val1, val2) { };

Xor::Xor(_genptr<_bool>& val1, _genptr<_bool>& val2)
   : BinaryOperation<_bool>(val1, val2) { };

_bool Not::getValue() 
{
   return !value->getValue();
}

_bool And::getValue() 
{
   return value1->getValue() && value2->getValue();
}

_bool Or::getValue() 
{
   return value1->getValue() || value2->getValue();
}

_bool Xor::getValue() 
{
   return value1->getValue() ^ value2->getValue();
}

InConstTimeList::InConstTimeList(_genptr<_tim>& val, const _tlist& li)
   : value(std::move(val)), list(sortedAndUniqueTimeList(li)) { };

_bool InConstTimeList::getValue() 
{
   return std::binary_search(list.begin(), list.end(), value->getValue());
}

}
