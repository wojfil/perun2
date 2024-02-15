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

#ifndef PARSE_DEFINITION_H_INCLUDED
#define PARSE_DEFINITION_H_INCLUDED

#include "../../tokens.h"
#include "../../perun2.h"


namespace perun2::parse
{

p_bool parseDefinition(p_defptr& result, const Tokens& tks, Perun2Process& p2);

static p_bool parseDefChain(p_defptr& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseDefTernary(p_defptr& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseDefBinary(p_defptr& result, const Tokens& tks, Perun2Process& p2);
static p_bool parseDefFilter(p_defptr& result, const Tokens& tks, Perun2Process& p2);

}

#endif // PARSE_DEFINITION_H_INCLUDED
