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

#include "order-limit-one.h"

namespace perun2::gen
{

   OrderByLimitOne::OrderByLimitOne(_defptr& bas, FileContext* ctx, _fcptr& nextCtx, _loptr& lo, p_perun2& p2)
      : base(std::move(bas)), fileContext(ctx), nextContext(std::move(nextCtx)), limitOne(std::move(lo)), perun2(p2) { };


   FileContext* OrderByLimitOne::getFileContext()
   {
      return this->nextContext.get(); 
   }


   void OrderByLimitOne::reset() 
   { 
      this->valueTaken = false;
   }

   p_bool OrderByLimitOne::hasNext()
   {
      if (this->valueTaken) {
         this->valueTaken = false;
         return false;
      }

      if (this->base->hasNext()) {
         this->value = this->base->getValue();
         this->limitOne->loadData();
      }
      else {
         return false;
      }

      while (this->base->hasNext()) {
         if (this->perun2.isNotRunning()) {
            this->base->reset();
            return false;
         }

         if (this->limitOne->nextElementIsBetter()) {
            this->value = this->base->getValue();
         }
      }

      this->valueTaken = true;
      this->nextContext->loadData(this->value);
      this->nextContext->index->value.setToZero();
      return true;
   }
}
