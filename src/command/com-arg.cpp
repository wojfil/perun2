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

#include "com-arg.h"
#include "..\os.h"
#include "..\perun2.h"


namespace perun2::comm
{

IterationLoop::IterationLoop(_comptr& com, _fcptr& ctx, p_perun2& p2)
   : command(std::move(com)), context(std::move(ctx)), perun2(p2) { };

CS_StringComArg::CS_StringComArg(_genptr<p_str>& str, _comptr& com, _fcptr& ctx, p_perun2& p2)
   : IterationLoop(com, ctx, p2), string(std::move(str)) { };

CS_ListComArg::CS_ListComArg(_genptr<p_list>& li, _comptr& com, _fcptr& ctx, p_perun2& p2)
   : IterationLoop(com, ctx, p2), list(std::move(li)) { };

CS_DefinitionComArg::CS_DefinitionComArg(p_defptr& def, _comptr& com, _fcptr& ctx, p_perun2& p2)
   : IterationLoop(com, ctx, p2), definition(std::move(def)) { };

void CS_StringComArg::run()
{
   if (this->perun2.isRunning()) {
      this->context->resetIndex();
      this->context->loadData(this->string->getValue());
      this->command->run();
   }
}


void CS_ListComArg::run()
{
   const p_list values = list->getValue();
   const p_num length = p_num(static_cast<p_nint>(values.size()));

   if (length.value.i == NINT_ZERO) {
      return;
   }

   p_num index;
   this->context->resetIndex();

   while (this->perun2.isRunning() && index != length) {
      this->context->loadData(values[index.value.i]);
      this->command->run();
      index++;
      this->context->index->value = index;
   }
}


void CS_DefinitionComArg::run()
{
   p_num index;
   this->context->resetIndex();

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
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
