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

#ifndef INCR_CONSTR_H
#define INCR_CONSTR_H

#include "datatype.h"
#include "generator.h"


namespace perun2
{


enum IC_State
{
   True,
   False,
   Unknown
};


struct IncrementalConstraint
{
public:
   IncrementalConstraint() = delete;
   IncrementalConstraint(p_genptr<p_num>& lg);

   void loadLimit();
   void increment(const p_num val);
   virtual IC_State getState() = 0;

protected:
   p_genptr<p_num> limitGen;
   p_num value;
   p_num limit;
};


struct IC_Equals : IncrementalConstraint
{
public:  
   IC_Equals() = delete;
   IC_Equals(p_genptr<p_num>& lg);
   IC_State getState() override;
};


struct IC_NotEquals : IncrementalConstraint
{
public:
   IC_NotEquals() = delete;
   IC_NotEquals(p_genptr<p_num>& lg);
   IC_State getState() override;
};

// ...



}


#endif /* INCR_CONSTR_H */
