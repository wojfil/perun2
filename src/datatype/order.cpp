#include "order.h"



OrderBase_Definition::~OrderBase_Definition()
{
   delete base;
}

std::vector<_str> OrderBase_Definition::getValue()
{
   this->volatileDepth = false;
   this->depths.clear();
   _list strings;

   while (this->base->hasNext()) {
      if (!this->uroboros->running) {
         this->base->reset();
         break;
      }

      if (!this->volatileDepth && !this->inner->depth.value.isZero()) {
         this->volatileDepth = true;
      }

      this->depths.push_back(this->inner->depth.value);
      strings.push_back(this->base->getValue());
   }

   return strings;
}

_boo OrderBase_Definition::hasVolatileDepth() const
{
   return this->volatileDepth;
}

_numi OrderBase_Definition::getDepth(const _size& index) const
{
   return this->depths[index];
}

void OrderBase_Definition::shuffleDepth(const std::vector<_size>& indices, const _size& length)
{
   shuffleByIndices(this->depths, indices, length);
}

void OrderIndices::clearValues(const _size& length)
{
   clearAndReserve(this->values, length);
}
