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

#include "../primitives.h"
#include "../../logic.h"


namespace perun2
{


// base struct for pattern matching
struct WildcardComparer
{
public:
   WildcardComparer() = delete;
   WildcardComparer(const p_str& pat);
   p_bool matches(const p_str& val);

protected:
   void clearCharStates();

   virtual p_size getMinLength(const p_str& pat) const = 0;
   virtual Logic checkState(const p_size n, const p_size m) = 0;

   const p_str pattern;
   const p_size patternLength;
   p_size minLength = 0;
   p_str const* valuePtr = nullptr;
   std::vector<std::vector<Logic>> charStates;
};


// this comparer uses only one wildcard: asterisk *
// it represents any sequence of characters including empty space
struct SimpleWildcardComparer : WildcardComparer
{
public:
   SimpleWildcardComparer() = delete;
   SimpleWildcardComparer(const p_str& pat);

protected:
   p_size getMinLength(const p_str& pat) const override;
   Logic checkState(const p_size n, const p_size m) override;
};


}

#endif // WILDCARD_H_INCLUDED
