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

#include "../../include/perun2/command/com-arg.hpp"
#include "../../include/perun2/os/os.hpp"
#include "../../include/perun2/perun2.hpp"


namespace perun2::comm
{

IterationLoop::IterationLoop(p_comptr& com, p_fcptr& ctx, Perun2Process& p2)
   : command(std::move(com)), context(std::move(ctx)), perun2(p2) { };

CS_StringComArg::CS_StringComArg(p_genptr<p_str>& str, p_comptr& com, p_fcptr& ctx, Perun2Process& p2)
   : IterationLoop(com, ctx, p2), string(std::move(str)) { };

CS_ListComArg::CS_ListComArg(p_genptr<p_list>& li, p_comptr& com, p_fcptr& ctx, Perun2Process& p2)
   : IterationLoop(com, ctx, p2), list(std::move(li)) { };

CS_DefinitionComArg::CS_DefinitionComArg(p_defptr& def, p_comptr& com, p_fcptr& ctx, Perun2Process& p2)
   : IterationLoop(com, ctx, p2), definition(std::move(def)) { };

void CS_StringComArg::run()
{
   if (this->perun2.isRunning()) {
      this->context->resetIndex();
      this->context->loadData(this->string->getValue());
      this->command->run();
   }
   else {
      this->perun2.contexts.success->value = false;
   }
}


void CS_ListComArg::run()
{
   const p_list values = list->getValue();
   const p_num length = p_num(static_cast<p_nint>(values.size()));

   if (length.isZero()) {
      return;
   }

   p_num index;
   this->context->resetIndex();
   p_bool anyFailure = false;

   while (index != length) {
      if (this->perun2.isNotRunning()) {
         anyFailure = true;
         break;
      }

      this->context->loadData(values[index.value.i]);
      this->command->run();
      index++;
      this->context->index->value = index;

      if (!this->perun2.contexts.success->value) {
         anyFailure = true;
      }
   }

   if (anyFailure) {
      this->perun2.contexts.success->value = false;
   }
}


void CS_DefinitionComArg::run()
{
   p_num index;
   this->context->resetIndex();
   p_bool anyFailure = false;

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         anyFailure = true;
         this->definition->reset();
         break;
      }

      this->context->loadData(this->definition->getValue());
      this->command->run();
      index++;
      this->context->index->value = index;

      if (!this->perun2.contexts.success->value) {
         anyFailure = true;
      }
   }

   if (anyFailure) {
      this->perun2.contexts.success->value = false;
   }
}

}
