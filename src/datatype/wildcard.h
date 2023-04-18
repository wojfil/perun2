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

#ifndef WILDCARD_H_INCLUDED
#define WILDCARD_H_INCLUDED

#include "primitives.h"


namespace perun2
{


enum WildcardCharState
{
   // these numbers are important
   wcs_Unknown = -1,
   wcs_NotMatches = 0,
   wcs_Matches = 1
};


// base struct for pattern matching
struct WildcardComparer
{
public:
   WildcardComparer() = delete;
   WildcardComparer(const _str& pat);
   _bool matches(const _str& val);

protected:
   void clearCharStates();

   virtual _size getMinLength(const _str& pat) const = 0;
   virtual WildcardCharState checkState(const _size n, const _size m) = 0;

   const _str pattern;
   const _size patternLength;
   _size minLength = 0;
   _str const* valuePtr = nullptr;
   std::vector<std::vector<WildcardCharState>> charStates;
};


// this comparer uses only one wildcard: asterisk *
// it represents any sequence of characters including empty space
struct SimpleWildcardComparer : WildcardComparer
{
public:
   SimpleWildcardComparer() = delete;
   SimpleWildcardComparer(const _str& pat);

protected:
   _size getMinLength(const _str& pat) const override;
   WildcardCharState checkState(const _size n, const _size m) override;
};


}

#endif // WILDCARD_H_INCLUDED
