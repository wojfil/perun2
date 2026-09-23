/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../datatype/datatype.h"
#include <unordered_map>


namespace perun2
{
   struct Perun2Process;
   struct Contexts;
}

namespace perun2::comm
{

typedef uint32_t     p_agunit;

p_constexpr p_agunit AGGR_NULL =       0;
p_constexpr p_agunit AGGR_COPY =       1 << 0;
p_constexpr p_agunit AGGR_SELECT =     1 << 1;

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
   Aggregate(Perun2Process& p2);
   void set(const p_agunit v);
   void markIterationCopy(FileContext* ctx);
   void markIterationSelect(FileContext* ctx);
   void onStart();
   void onIteration();
   void onFinish();

   p_set copyPaths;
   uint32_t failedCopy = 0;
   p_set invalidCopy;

   std::unordered_map<p_str, p_set> selectPaths;
   uint32_t failedSelect = 0;
   p_set invalidSelect;

private:
   void copy();
   void select();
   inline p_bool has(const p_agunit v) const;

   Perun2Process& perun2;
   Contexts& contexts;
   p_agunit value = AGGR_NULL;
   p_bool selectFailure = false;
   
   p_bool iterationCopy = false;
   p_bool iterationSelect = false;
   FileContext* iterationContext;
};

}
