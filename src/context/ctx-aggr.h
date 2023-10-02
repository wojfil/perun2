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

#ifndef CTX_AGGR_H_INCLUDED
#define CTX_AGGR_H_INCLUDED

#include "../datatype/datatype.h"
#include "../command/com-aggregate.h"


namespace perun2
{
   struct p_perun2;

   struct AggregateContext
   {
      AggregateContext() = delete;
      AggregateContext(p_perun2& p2);
      void runAggregate();

      comm::Aggregate aggregate;
   };


   typedef std::unique_ptr<AggregateContext>       p_acptr;

}

#endif // CTX_AGGR_H_INCLUDED
