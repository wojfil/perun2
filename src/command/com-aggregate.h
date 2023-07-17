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

#ifndef COM_AGGREGATE_H_INCLUDED
#define COM_AGGREGATE_H_INCLUDED

#include "../datatype/datatype.h"
#include <unordered_map>


namespace perun2
{
   struct p_perun2;
   struct Contexts;
}

namespace perun2::comm
{

typedef uint32_t _agunit;

_constexpr _agunit AGGR_NULL =      0b000;
_constexpr _agunit AGGR_COPY =      0b001;
_constexpr _agunit AGGR_SELECT =    0b010;

// commands Select and Copy are different from others
// they have to be performed on multiple files and directories at once
// we cannot simply call them one by one
// what happens when we call them in a loop?
// if we select files one by one, then the result will be selection of the one last file
// this is what Aggregate is for - it stores files and directories
// and performs the command once at the loop exit

struct Aggregate
{
public:
   Aggregate() = delete;
   Aggregate(p_perun2& p2);
   void set (const _agunit v);
   void run();

   _set copyPaths;
   std::unordered_map<_str, _set> selectPaths;
   uint32_t failedCopy = 0;
   uint32_t failedSelect = 0;
   _set invalidCopy;
   _set invalidSelect;

private:
   void copy();
   void select();
   inline _bool has(const _agunit v) const;

   p_perun2& perun2;
   Contexts& contexts;
   _agunit value = AGGR_NULL;
   _bool selectFailure = false;
};

}

#endif /* COM_AGGREGATE_H_INCLUDED */
