#include "parse-order.h"


void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, Uroboros* uro)
{
   if (state == ThisState::ts_String) {
      uro->vc.retreatAttribute();
   }

   uro->vars.inner.thisState = prevState;
}
