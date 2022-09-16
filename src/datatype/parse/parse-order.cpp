#include "parse-order.h"


void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, Uroboros* uro)
{
   if (state == ThisState::ts_String) {
      uro->vc.retreatAttribute();
   }

   uro->vars.inner.thisState = prevState;
}

void setSingleOrderFilter(Attribute* attr, const _boo& hasMemory, _def*& result,
   OrderIndices* indices, Order* order, Uroboros* uro)
{
   result = new OrderBy_Definition(result, attr, hasMemory, indices, order, uro);
}
