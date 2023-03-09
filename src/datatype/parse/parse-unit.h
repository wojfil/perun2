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

#ifndef PARSE_UNIT_H_INCLUDED
#define PARSE_UNIT_H_INCLUDED

#include "../../tokens.h"

namespace perun2::parse
{

_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_bool>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_num>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_str>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_nlist>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_tlist>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_list>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_tim>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _genptr<_per>& result);
_bool parseOneToken(_p2& p2, const Tokens& tks, _defptr& result);

}

#endif // PARSE_UNIT_H_INCLUDED
