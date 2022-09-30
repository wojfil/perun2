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

#include "gen-bool.h"


namespace uro::gen
{

Not::Not(Generator<_bool>* val) 
   : UnaryOperation<_bool>(val) { };

And::And(Generator<_bool>* val1, Generator<_bool>* val2)
   : BinaryOperation<_bool>(val1, val2) { };

Or::Or(Generator<_bool>* val1, Generator<_bool>* val2)
   : BinaryOperation<_bool>(val1, val2) { };

Xor::Xor(Generator<_bool>* val1, Generator<_bool>* val2)
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

InConstTimeList::InConstTimeList(Generator<_tim>* val, const _tlist& li)
   : value(val), list(sortedAndUniqueTimeList(li)) { };

InConstTimeList::~InConstTimeList() 
{
   delete value;
}

_bool InConstTimeList::getValue() 
{
   return std::binary_search(list.begin(), list.end(), value->getValue());
}

}
