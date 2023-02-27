/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "order.h"
#include "../util.h"
#include "../datatype/patterns.h"


namespace uro::gen
{

OrderIndices::OrderIndices() noexcept
   : values(new _size[0]()) { }

OrderIndices::~OrderIndices() noexcept
{
   delete[] this->values;
}

void OrderIndices::prepare(const _size& length)
{
   delete[] this->values;
   this->values = new _size[length]();
}

OrderBy_Definition::OrderBy_Definition(_defptr& bas, FileContext* ctx, _fcptr& nextCtx, _indptr& inds, _ordptr& ord, _uro& uro)
   : OrderBy(inds, ord), fileContext(ctx), base(std::move(bas)), uroboros(uro), nextContext(std::move(nextCtx))
{
   this->resultPtr = &this->result;
}

FileContext* OrderBy_Definition::getFileContext()  
{
   return this->nextContext.get(); 
};

void OrderBy_Definition::reset()
{
   this->result.clear();
   this->order->clearValues();
   this->index = NINT_ZERO;

   if (!this->first) {
      this->base->reset();
      this->first = true;
   }
}

_bool OrderBy_Definition::hasNext()
{
   if (this->first) {
      this->reset();

      while (this->base->hasNext()) {
         if (this->uroboros.state != State::s_Running) {
            this->base->reset();
            return false;
         }

         this->value = this->base->getValue();
         this->result.emplace_back(this->value);
         this->order->addValues();
      }

      this->length = this->result.size();

      if (this->length == 0) {
         return false;
      }

      this->indices->prepare(this->length);

      for (_size i = 0; i < this->length; i++) {
         this->indices->values[i] = i;
      }

      this->quicksort(0, this->length - 1);
      this->first = false;
   }

   if (this->index == this->length) {
      this->first = true;
      return false;
   }
   else {
      this->value = this->result[this->index];
      this->nextContext->loadData(this->value);
      this->nextContext->index->value.value.i = static_cast<_nint>(this->index);
      this->index++;
      return true;
   }
}

}
