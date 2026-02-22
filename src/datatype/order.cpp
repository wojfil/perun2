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

#include "../../include/perun2/datatype/order.hpp"
#include "../../include/perun2/util.hpp"
#include "../../include/perun2/datatype/patterns.hpp"


namespace perun2::gen
{

void OrderIndices::prepare(const p_size length)
{
   this->values = std::make_unique<p_size[]>(length);
}

OrderBy::OrderBy(p_indptr& inds, p_ordptr& ord)
   : indices(std::move(inds)), order(std::move(ord)) { };

void OrderBy::quicksort(p_int start, p_int end)
{
   std::unique_ptr<p_int[]> stack = std::make_unique<p_int[]>(end - start + 1);
   p_int top = -1;
  
   stack[++top] = start;
   stack[++top] = end;
  
   while (top >= 0) {
      end = stack[top--];
      start = stack[top--];
      p_int pivot = partition(start, end);
  
      if (pivot - 1 > start) {
         stack[++top] = start;
         stack[++top] = pivot - 1;
      }
  
      if (pivot + 1 < end) {
         stack[++top] = pivot + 1;
         stack[++top] = end;
      }
   }
}

p_int OrderBy::partition(const p_int start, const p_int end)
{
   p_int i = start - 1;

   for (p_int j = start; j <= end - 1; j++) {
      if (this->order->matchesSwap(j, end)) {
         i++;
         std::swap(this->indices->values[i], this->indices->values[j]);
         std::iter_swap(this->resultPtr->begin() + i, this->resultPtr->begin() + j);
      }
   }

   const p_int ip = i + 1;
   std::swap(this->indices->values[ip], this->indices->values[end]);
   std::iter_swap(this->resultPtr->begin() + ip, this->resultPtr->begin() + end);
   return ip;
}

OrderBy_List::OrderBy_List(p_genptr<p_list>& bas, p_fcptr& ctx, p_indptr& inds, p_ordptr& ord, Perun2Process& p2)
      : OrderBy(inds, ord), context(std::move(ctx)), base(std::move(bas)) { };

p_list OrderBy_List::getValue()
{
   p_list result = this->base->getValue();
   const p_size length = result.size();

   if (length == 0) {
      return result;
   }

   this->resultPtr = &result;
   this->indices->prepare(length);
   this->order->clearValues(length);
   this->context->resetIndex();

   for (p_size i = 0; i < length; i++) {
      this->context->loadData(result[i]);
      this->indices->values[i] = i;
      this->order->addValues();
      this->context->incrementIndex();
   }

   this->quicksort(0, length - 1);
   return result;
}

OrderBy_Definition::OrderBy_Definition(p_defptr& bas, FileContext* ctx, p_fcptr& nextCtx, p_indptr& inds, p_ordptr& ord, Perun2Process& p2)
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

p_bool OrderBy_Definition::hasNext()
{
   if (this->first) {
      this->reset();

      while (this->base->hasNext()) {
         if (this->perun2.isNotRunning()) {
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

      for (p_size i = 0; i < this->length; i++) {
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
      this->nextContext->index->value.value.i = static_cast<p_nint>(this->index);
      this->index++;
      return true;
   }
}

}
