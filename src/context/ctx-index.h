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

#ifndef CTX_INDEX_H_INCLUDED
#define CTX_INDEX_H_INCLUDED

#include "ctx-aggr.h"
#include "ctx-vars.h"


namespace perun2
{

   struct IndexContext : AggregateContext
   {
   public:
      IndexContext() = delete;
      IndexContext(Perun2Process& p2);
      void resetIndex();
      void incrementIndex();

      p_varptr<p_num> index;
   };


   typedef std::unique_ptr<IndexContext>           p_icptr;
   
}

#endif // CTX_INDEX_H_INCLUDED
