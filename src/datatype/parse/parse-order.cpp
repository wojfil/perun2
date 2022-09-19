#include "parse-order.h"


namespace uro::parse
{

void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, uro::Uroboros* uro)
{
   if (state == ThisState::ts_String) {
      uro->vc.retreatAttribute();
   }

   uro->vars.inner.thisState = prevState;
}

void setSingleOrderFilter(Attribute* attr, const _boo& hasMemory, _def*& result,
   gen::OrderIndices* indices, gen::Order* order, uro::Uroboros* uro)
{
   result = new gen::OrderBy_Definition(result, attr, hasMemory, indices, order, uro);
}

}
