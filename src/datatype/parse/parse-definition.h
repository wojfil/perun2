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

#ifndef PARSE_DEFINITION_H_INCLUDED
#define PARSE_DEFINITION_H_INCLUDED

#include "../../tokens.h"
#include "../../uroboros.h"


namespace uro::parse
{

_def* parseDefinition(const Tokens& tks, uro::Uroboros* uro);

static _bool isDefinitionChain(const Tokens& tks, uro::Uroboros* uro);
static _def* parseDefinitionChain(const Tokens& tks, uro::Uroboros* uro);
static _def* parseDefTernary(const Tokens& tks, uro::Uroboros* uro);
static _def* parseDefBinary(const Tokens& tks, uro::Uroboros* uro);

}

#endif // PARSE_DEFINITION_H_INCLUDED
