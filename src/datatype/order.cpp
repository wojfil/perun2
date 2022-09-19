#include "order.h"
#include "../util.h"


namespace uro::gen
{

void OrderIndices::prepare(const _size& length)
{
   langutil::clearAndReserve(this->values, length);
}

OrderBy_Definition::OrderBy_Definition(_def* bas, Attribute* attr,
   const _boo& hasMem, OrderIndices* indices, Order* ord, Uroboros* uro)
   : OrderBy<_str>(attr, indices, ord, uro), base(bas),
     uroboros(uro), inner(&uro->vars.inner),
     hasMemory(hasMem), attrMemory(AttributeMemory(attr, &uro->vars.inner))
{
   this->resultPtr = &this->result;
}

OrderBy_Definition::~OrderBy_Definition()
{
   delete this->base;
}

void OrderBy_Definition::reset()
{
   this->result.clear();
   this->depths.clear();
   this->order->clearValues();
   this->index = 0LL;

   this->hasVolatileDepth = false;

   if (!this->first) {
      this->base->reset();

      this->first = true;

      this->inner->index.value = this->prevIndex;
      this->inner->depth.value = this->prevDepth;
      this->thisReference->value = this->prevThis;

      if (this->hasMemory) {
         this->attrMemory.restore();
      }
   }
}

_boo OrderBy_Definition::hasNext()
{
   if (this->first) {
      this->reset();

      this->prevIndex = this->inner->index.value;
      this->prevDepth = this->inner->depth.value;
      this->prevThis = this->thisReference->value;

      if (this->hasMemory) {
         this->attrMemory.load();
      }

      while (this->base->hasNext()) {
         if (!this->uroboros->running) {
            this->base->reset();
            return false;
         }

         this->thisReference->value = this->base->getValue();

         if (this->hasAttribute) {
            this->attribute->run();
         }

         this->result.push_back(this->base->getValue());
         const _numi& depth = this->inner->depth.value;

         this->depths.push_back(depth.value.i);
         this->order->addValues();

         if (!this->hasVolatileDepth && depth.value.i != 0LL) {
            this->hasVolatileDepth = true;
         }
      }

      this->length = this->result.size();

      switch (this->length) {
         case 0: {
            return false;
         }
         case 1: {
            this->value = this->base->getValue();
            return true;
         }
      }

      if (!this->hasVolatileDepth) {
         this->inner->depth.value.setToZero();
      }

      this->orderIndices->prepare(this->length);

      for (_size i = 0; i < this->length; i++) {
         this->orderIndices->values.push_back(i);
      }

      this->quicksort(0, this->length - 1);
      this->first = false;
   }

   if (this->index == this->length) {
      this->first = true;
      this->inner->index.value = this->prevIndex;
      this->inner->depth.value = this->prevDepth;
      this->thisReference->value = this->prevThis;

      if (this->hasMemory) {
         this->attrMemory.restore();
      }

      return false;
   }
   else {
      this->value = this->result[this->index];
      this->thisReference->value = this->value;
      this->inner->index.value.value.i = static_cast<_nint>(this->index);
      if (this->hasVolatileDepth) {
         this->inner->depth.value.value.i = this->depths[this->orderIndices->values[this->index]];
      }
      this->index++;
      return true;
   }
}

}
