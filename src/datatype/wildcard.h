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

#ifndef WILDCARD_H_INCLUDED
#define WILDCARD_H_INCLUDED

#include "primitives.h"


namespace uro
{


enum WildcardCharState
{
   // these numbers are important
   wcs_Unknown = -1,
   wcs_NotMatches = 0,
   wcs_Matches = 1
};


struct WildcardComparer
{
public:
    WildcardComparer(const _str& pat);


   void clearCharStates();

    virtual _size getMinLength(const _str& pat) const;
    virtual _bool matches(const _str& value);

protected:


   const _str pattern;
   const _size patternLen;
   const _size minLength;

   _str const* valuePtr = nullptr;
   std::vector<std::vector<WildcardCharState>> charStates;


};




}

#endif // WILDCARD_H_INCLUDED
