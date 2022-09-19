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

#include "gen-list.h"
#include "../../uroboros.h"
#include "../../util.h"


namespace uro::gen
{

Join_StrStr::~Join_StrStr() {
   delete left;
   delete right;
}

Join_StrList::~Join_StrList() {
   delete left;
   delete right;
}

Join_ListStr::~Join_ListStr() {
   delete left;
   delete right;
}

Join_ListList::~Join_ListList() {
   delete left;
   delete right;
}

_list Join_StrStr::getValue()
{
   _list v;
   v.reserve(2);
   v.push_back(left->getValue());
   v.push_back(right->getValue());
   return v;
}

_list Join_StrList::getValue()
{
   _list v = right->getValue();
   v.insert(v.begin(), left->getValue());
   return v;
}

_list Join_ListStr::getValue()
{
   _list v = left->getValue();
   v.push_back(right->getValue());
   return v;
}

_list Join_ListList::getValue()
{
   _list v = left->getValue();
   appendVector(v, right->getValue());
   return v;
}

}
