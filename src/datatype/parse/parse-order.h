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

#ifndef PARSE_ORDER_H_INCLUDED
#define PARSE_ORDER_H_INCLUDED


#include "../../perun2.h"
#include "../../tokens.h"
#include "../generator/gen-definition.h"
#include "../parse-gen.h"


namespace perun2::parse
{

_bool parseOrder(gen::_ordptr& resultOrder, gen::_indptr& indices, Tokens& tks, const Token& keyword, _p2& p2);

template <typename T>
void setOrderUnit(gen::_ordptr& order, _genptr<T>& value, const _bool desc, gen::_indptr& indices)
{
   if (order) {
      gen::_ordptr prev = std::move(order);
      order = std::make_unique<gen::OrderUnit_Middle<T>>(value, desc, prev, indices);
   }
   else {
      order = std::make_unique<gen::OrderUnit_Final<T>>(value, desc, indices);
   }
}

}

#endif // PARSE_ORDER_H_INCLUDED
