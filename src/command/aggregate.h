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

#ifndef AGGR_H
#define AGGR_H

#include "../datatype/datatype.h"
#include <set>
#include <map>

inline constexpr _uint32 AGGR_NULL =      0b000;
inline constexpr _uint32 AGGR_COPY =      0b001;
inline constexpr _uint32 AGGR_SELECT =    0b010;

// commands Select and Copy are different than others
// they have to be performed on multiple files and directories at once
// what happens when we call them in a loop?
// if we select files one by one, then the result will be selection of the one last file
// this is what Aggregate is for - it stores files and directories
// and performs the command once at the loop exit


struct Uroboros;
struct InnerVariables;

struct Aggregate
{
public:
   Aggregate(Uroboros* uro);
   void set(const _uint32& v);
   void run();

   std::set<_str> copyPaths;
   std::map<_str, std::set<_str>> selectPaths;
   _uint32 failedCopy;
   _uint32 failedSelect;
   std::set<_str> invalidCopy;
   std::set<_str> invalidSelect;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   _uint32 value;
   inline _boo has(const _uint32& v) const;
};

#endif /* AGGR_H */
