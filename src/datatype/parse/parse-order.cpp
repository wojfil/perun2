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
   OrderBase_Definition* obase = new OrderBase_Definition(result, uro);
   result = new OrderByCast(new OrderBy<_str>(
      obase, attr, indices, order, uro), attr, hasMemory, obase, uro);
}
