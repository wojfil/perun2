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

#include "exp-element.hpp"
#include "cast.hpp"
#include "generator.hpp"
#include "../exception.hpp"
#include "../tokens.hpp"
#include "../token.hpp"
#include "../perun2.hpp"
#include "../util.hpp"
#include "parse/parse-unit.hpp"
#include "parse/parse-bool.hpp"
#include "parse/parse-list.hpp"
#include "parse/parse-number.hpp"
#include "parse/parse-period.hpp"
#include "parse/parse-string.hpp"
#include "parse/parse-time.hpp"
#include "parse/parse-numlist.hpp"
#include "parse/parse-timlist.hpp"
#include "parse/parse-definition.hpp"


namespace perun2::parse
{

// build a Generator of certain data type (casting included)
// this is the API we should use everywhere
// it even has a nice short name and its function overloading works well in templates

p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_bool>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_num>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_str>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_nlist>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_tlist>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_list>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_tim>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_genptr<p_per>& result);
p_bool parse(Perun2Process& p2, const Tokens& tks, p_defptr& result);

}
