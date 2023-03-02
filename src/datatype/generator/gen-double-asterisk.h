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

#ifndef GEN_DOUBLE_ASTERISK_H_INCLUDED
#define GEN_DOUBLE_ASTERISK_H_INCLUDED

#include "gen-os.h"
#include <memory>



namespace uro::gen
{

// these wildcards are used only internally
// they should be characters restricted for a file name
// so no collision happens
inline constexpr _char WILDCARD_SINGLE_ASTERISK = CHAR_SMALLER;
inline constexpr _char WILDCARD_DOUBLE_ASTERISK = CHAR_GREATER;

typedef std::unique_ptr<RecursiveAll> _rallptr;

enum CharState
{
   // these numbers are important
   cs_Unknown = -1,
   cs_NotMatches = 0,
   cs_Matches = 1
};


struct DoubleAsteriskPattern : Definition
{
public:
   DoubleAsteriskPattern(_rallptr& def, _uro& uro, const _str& pat, const _size dpth, const _size start);

   _bool hasNext() override;
   void reset() override;

private:
   _bool matchesPattern();
   void clearCharStates();
   CharState checkState(const _size n, const _size m);

   const _str pattern;
   const _size patternLength;
   const _size defaultDepth;
   const _size startId;
   _rallptr definition;
   FileContext* context;
   _uro& uroboros;
   _bool first = true;
   _num index = NINT_ZERO;

   std::vector<std::vector<CharState>> charStates;
   _str const* valuePtr = nullptr;

};


}

#endif // GEN_DOUBLE_ASTERISK_H_INCLUDED
