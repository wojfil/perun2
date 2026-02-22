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

#include "../../include/perun2/context/ctx-index.hpp"

namespace perun2
{

   IndexContext::IndexContext(Perun2Process& p2)
      : AggregateContext(p2),
        index(std::make_unique<Variable<p_num>>(VarType::vt_Special)) { };


   void IndexContext::resetIndex()
   {
      this->index->value.value.i = NINT_ZERO;
   }

   void IndexContext::incrementIndex()
   {
      this->index->value.value.i++;
   }

}
