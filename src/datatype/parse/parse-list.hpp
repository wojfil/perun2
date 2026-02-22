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

#pragma once

#include "../../tokens.hpp"
#include "../../perun2.hpp"


namespace perun2::parse
{

p_bool parseList(p_genptr<p_list>& result, const Tokens& tks, Perun2Process& p2);

static p_bool parseListFilter(p_genptr<p_list>& result, const Tokens& tks, Perun2Process& p2);

}
