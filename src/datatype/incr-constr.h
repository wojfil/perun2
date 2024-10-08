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

#include "comparison.h"
#include "datatype.h"
#include "generator.h"
#include "../logic.h"


namespace perun2
{


// what is Incremental Constraint?
// look at the code below
//
//      select directories
//        where size > 10mb
//
// we can introduce an obvious optimization
// instead of calculating the exact size of every directory and comparing this value to 10 megabytes
// we read sizes of files sequentially and every step check if condition is alredy satisfied
// if the size already exceedes 10mb, we can escape early
// there is no need to read more
// this is just an example
// Incremental Constraints have more use cases


struct IncrementalConstraint
{
public:
   IncrementalConstraint() = delete;
   IncrementalConstraint(p_genptr<p_num>& lg, const CompType ct);

   void loadLimit();
   void setValueToZero();
   void setValue(const p_num& val);
   void increment(const p_num& val);
   void incrementByOne();

   Logic getState() const;
   p_bool getFinalResult() const;
   p_bool limitIsNaN() const;

private:
   p_genptr<p_num> limitGen;
   p_num value;
   p_num limit;
   const CompType comparisonType;
};


}
