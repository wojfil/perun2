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

#include "order.h"
#include "../util.h"
#include "../datatype/patterns.h"


namespace perun2::gen
{

OrderIndices::OrderIndices() noexcept
   : values(new _size[0]()) { }

OrderIndices::~OrderIndices() noexcept
{
   delete[] this->values;
}

void OrderIndices::prepare(const _size length)
{
   delete[] this->values;
   this->values = new _size[length]();
}

OrderBy::OrderBy(_indptr& inds, _ordptr& ord)
   : indices(std::move(inds)), order(std::move(ord)) { };

void OrderBy::quicksort(const _int start, const _int end)
{
   if (start < end) {
      const _int i = partition(start, end);
      quicksort(start, i - 1);
      quicksort(i + 1, end);
   }
}

_int OrderBy::partition(const _int start, const _int end)
{
   _int i = start - 1;

   for (_int j = start; j <= end - 1; j++) {
      if (this->order->matchesSwap(j, end)) {
         i++;
         std::swap(this->indices->values[i], this->indices->values[j]);
         std::iter_swap(this->resultPtr->begin() + i, this->resultPtr->begin() + j);
      }
   }

   const _int ip = i + 1;
   std::swap(this->indices->values[ip], this->indices->values[end]);
   std::iter_swap(this->resultPtr->begin() + ip, this->resultPtr->begin() + end);
   return ip;
}

OrderBy_List::OrderBy_List(_genptr<_list>& bas, _fcptr& ctx, _indptr& inds, _ordptr& ord, _p2& p2)
      : OrderBy(inds, ord), context(std::move(ctx)), base(std::move(bas)) { };

_list OrderBy_List::getValue()
{
   _list result = this->base->getValue();
   const _size length = result.size();

   if (length == 0) {
      return result;
   }

   this->resultPtr = &result;
   this->indices->prepare(length);
   this->order->clearValues(length);
   this->context->resetIndex();

   for (_size i = 0; i < length; i++) {
      this->context->loadData(result[i]);
      this->indices->values[i] = i;
      this->order->addValues();
      this->context->incrementIndex();
   }

   this->quicksort(0, length - 1);
   return result;
}

OrderBy_Definition::OrderBy_Definition(_defptr& bas, FileContext* ctx, _fcptr& nextCtx, _indptr& inds, _ordptr& ord, _p2& p2)
   : OrderBy(inds, ord), fileContext(ctx), base(std::move(bas)), perun2(p2), nextContext(std::move(nextCtx))
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
         if (this->perun2.state != State::s_Running) {
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
