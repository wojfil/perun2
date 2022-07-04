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

#ifndef PARSE_GEN_H
#define PARSE_GEN_H

#include "exp-element.h"
#include "cast.h"
#include "generator.h"
#include "../exception.h"
#include "../tokens.h"
#include "../token.h"
#include "../uroboros.h"
#include "../util.h"
#include "../var/var-runtime.h"
#include "parse/parse-bool.h"
#include "parse/parse-list.h"
#include "parse/parse-number.h"
#include "parse/parse-numlist.h"
#include "parse/parse-timlist.h"
#include "parse/parse-period.h"
#include "parse/parse-string.h"
#include "parse/parse-time.h"
#include "parse/parse-definition.h"

Tokens prepareForGen(const Tokens& tks, Uroboros* uro);
void checkKeywords(const Tokens& tks);
_boo isExpForbiddenKeyword(const Token& tk);

_boo parse(Uroboros* uro, const Tokens& tns, Generator<_boo>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, Generator<_num>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, Generator<_str>*& result);
_boo parse(Uroboros* uro, const Tokens& tns,Generator<_nlist>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, Generator<_tlist>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, Generator<_list>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, Generator<_tim>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, Generator<_per>*& result);
_boo parse(Uroboros* uro, const Tokens& tns, _def*& result);

#endif /* PARSE_GEN_H */
