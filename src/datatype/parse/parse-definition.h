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

#ifndef PARSE-DEFINITION_H_INCLUDED
#define PARSE-DEFINITION_H_INCLUDED

#include "../../tokens.h"
#include "../../uroboros.h"


_def* parseDefinition(const Tokens& tks, Uroboros* uro);

static _boo isDefinitionChain(const Tokens& tks, Uroboros* uro);
static _def* parseDefinitionChain(const Tokens& tks, Uroboros* uro);

#endif // PARSE-DEFINITION_H_INCLUDED
