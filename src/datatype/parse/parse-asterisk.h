/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSE_ASTERISK_H_INCLUDED
#define PARSE_ASTERISK_H_INCLUDED

#include "../generator/gen-os.h"


namespace uro::parse
{

inline constexpr _uint32 ASTERISK_INFO_NULL =             0b0000000;
inline constexpr _uint32 ASTERISK_INFO_VALID =            0b0000001;
inline constexpr _uint32 ASTERISK_INFO_ONE_ASTERISK =     0b0000010;
inline constexpr _uint32 ASTERISK_INFO_DOUBLE_ASTERISK =  0b0000100;
inline constexpr _uint32 ASTERISK_INFO_IS_ABSOLUTE =      0b0001000;


struct AsteriskUnit
{
public:
   AsteriskUnit() = delete;
   AsteriskUnit(const _str& ast, const _str& suf) : asteriskPart(ast), suffixPart(suf) { };
   AsteriskUnit(const _str& ast) : asteriskPart(ast) { };

   const _str asteriskPart;
   const _str suffixPart;
};


_bool parseAsteriskPattern(_defptr& result, const _str& originPattern, const _int& line, _uro& uro);
void addAsteriskPatternUnit(_str& asteriskPart, _str& suffixPart, const _str& part,
   const _bool hasAsterisk, std::vector<AsteriskUnit>& units);
_bool parseDoubleAsterisk(_defptr& result, _genptr<_str>& base, const _str& pattern, 
   const _size& start, const _bool isAbsolute, _uro& uro);

}

#endif // PARSE_ASTERISK_H_INCLUDED
