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

#ifndef PARSE_ASTERISK_H_INCLUDED
#define PARSE_ASTERISK_H_INCLUDED

#include "../generator/gen-os.h"


namespace perun2::parse
{

p_constexpr uint32_t ASTERISK_INFO_NULL =              0;
p_constexpr uint32_t ASTERISK_INFO_VALID =             1 << 0;
p_constexpr uint32_t ASTERISK_INFO_ONE_ASTERISK =      1 << 1;
p_constexpr uint32_t ASTERISK_INFO_DOUBLE_ASTERISK =   1 << 2;
p_constexpr uint32_t ASTERISK_INFO_IS_ABSOLUTE =       1 << 3;


struct AsteriskUnit
{
public:
   AsteriskUnit() = delete;
   AsteriskUnit(const p_str& ast, const p_str& suf) : asteriskPart(ast), suffixPart(suf) { };
   AsteriskUnit(const p_str& ast) : asteriskPart(ast) { };

   const p_str asteriskPart;
   const p_str suffixPart;
};


p_bool parseAsteriskPattern(p_defptr& result, const p_str& originPattern, const p_int line, p_perun2& p2);
void addAsteriskPatternUnit(p_str& asteriskPart, p_str& suffixPart, const p_str& part,
   const p_bool hasAsterisk, std::vector<AsteriskUnit>& units);
p_bool parseDoubleAsterisk(p_defptr& result, p_genptr<p_str>& base, const p_str& pattern, const p_str& trimmed,
   const p_size start, const PathType pathType, const p_int retreats, p_perun2& p2);
p_bool parseDefinitionSuffix(p_defptr& result, p_defptr& definition, const p_str& suffix, 
   const PathType pathType, const SegmentType segmType, const p_int retreats, p_def* previous, p_perun2& p2);

}

#endif // PARSE_ASTERISK_H_INCLUDED
