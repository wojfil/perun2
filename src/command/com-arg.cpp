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

#include "com-arg.h"
#include "..\os.h"
#include "..\uroboros.h"


namespace uro::comm
{

IterationLoop::IterationLoop(_comptr& com, _fcptr& ctx, _uro& uro)
   : command(std::move(com)), context(std::move(ctx)), uroboros(uro) { };

CS_StringComArg::CS_StringComArg(_genptr<_str>& str, _comptr& com, _fcptr& ctx, _uro& uro)
   : IterationLoop(com, ctx, uro), string(std::move(str)) { };

CS_ListComArg::CS_ListComArg(_genptr<_list>& li, _comptr& com, _fcptr& ctx, _uro& uro)
   : IterationLoop(com, ctx, uro), list(std::move(li)) { };

CS_DefinitionComArg::CS_DefinitionComArg(_defptr& def, _comptr& com, _fcptr& ctx, _uro& uro)
   : IterationLoop(com, ctx, uro), definition(std::move(def)) { };

void CS_StringComArg::run()
{
   if (this->uroboros.state == State::s_Running) {
      this->context->resetIndexAndDepth();
      this->context->loadData(this->string->getValue());
      this->command->run();
   }
}


void CS_ListComArg::run()
{
   const _list values = list->getValue();
   const _num length = _num(static_cast<_nint>(values.size()));

   if (length.value.i == NINT_ZERO) {
      return;
   }

   _num index;
   this->context->resetIndexAndDepth();

   while (this->uroboros.state == State::s_Running && index != length) {
      this->context->loadData(values[index.value.i]);
      this->command->run();
      index++;
      this->context->index->value = index;
   }
}


void CS_DefinitionComArg::run()
{
   _num index;
   this->context->resetIndexAndDepth();

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      this->context->loadData(this->definition->getValue());
      this->command->run();
      index++;
      this->context->index->value = index;
   }
}

}
