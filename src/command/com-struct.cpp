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

#include "com-struct.h"
#include "..\os.h"
#include "..\perun2.h"


namespace perun2::comm
{


void CS_RawBlock::run()
{
   for (p_size i = 0; this->perun2.isRunning() && i < this->length; i++) {
      (this->commands[i])->run();
   }
}


void CS_Block::run()
{
   this->context->aggregate.onStart();

   for (p_size i = 0; this->perun2.isRunning() && i < this->length; i++) {
      (this->commands[i])->run();
      this->context->aggregate.onIteration();
   }

   this->context->aggregate.onFinish();
}


void CS_Times::run()
{
   const p_num n = this->times->getValue();

   if (n.state == NumberState::NaN) {
      return;
   }

   p_nint repeats = n.toInt();
   if (repeats <= NINT_ZERO) {
      return;
   }

   this->context->aggregate.onStart();
   this->context->resetIndex();

   while (this->perun2.isRunning() && repeats != NINT_ZERO) {
      this->command->run();
      this->context->incrementIndex();
      repeats--;

      P_CHECK_LOOP_BREAK;
      this->context->aggregate.onIteration();
   }

   this->context->aggregate.onFinish();
}


void CS_While::run()
{
   this->context->aggregate.onStart();
   this->context->resetIndex();

   while (this->perun2.isRunning() && this->condition->getValue()) {
      this->command->run();
      this->context->incrementIndex();

      P_CHECK_LOOP_BREAK;
      this->context->aggregate.onIteration();
   }

   this->context->aggregate.onFinish();
}


void CS_StringLoop::run()
{
   if (this->perun2.isRunning()) {
      this->context->aggregate.onStart();
      this->context->resetIndex();
      this->context->loadData(this->string->getValue());
      this->command->run();

      P_CHECK_SOFT_LOOP_BREAK;

      this->context->aggregate.onIteration();
      this->context->aggregate.onFinish();
   }
}


void CS_DefinitionLoop::run()
{
   p_num index = NINT_ZERO;
   this->context->resetIndex();
   this->context->aggregate.onStart();

   while (this->definition->hasNext()) {
      if (!this->perun2.isRunning()) {
         this->definition->reset();
         break;
      }

      this->context->loadData(this->definition->getValue());
      this->command->run();

      index++;
      this->context->index->value = index;

      P_CHECK_LOOP_BREAK;
      this->context->aggregate.onIteration();
   }

   this->context->aggregate.onFinish();
}


CS_ContextlessLoop::CS_ContextlessLoop(p_defptr& def, p_comptr& com, p_perun2& p2)
   : definition(std::move(def)), command(std::move(com)), perun2(p2),
     context(this->definition->getFileContext()) { };


void CS_ContextlessLoop::run()
{
   this->context->aggregate.onStart();

   while (this->definition->hasNext()) {
      if (!this->perun2.isRunning()) {
         this->definition->reset();
         break;
      }

      this->command->run();
      P_CHECK_LOOP_BREAK;
      this->context->aggregate.onIteration();
   }

   this->context->aggregate.onFinish();
}


void CS_ListLoop::run()
{
   const p_list values = this->list->getValue();
   const p_num length = static_cast<p_nint>(values.size());

   if (length == 0) {
      return;
   }

   p_num index = NINT_ZERO;
   this->context->aggregate.onStart();
   this->context->resetIndex();

   while (this->perun2.isRunning() && index != length) {
      this->context->loadData(values[static_cast<p_size>(index.value.i)]);
      this->command->run();
      index++;
      this->context->index->value = index;

      P_CHECK_LOOP_BREAK;
      this->context->aggregate.onIteration();
   }

   this->context->aggregate.onFinish();
}


void CS_InsideThis::run()
{
   if (this->perun2.isRunning()) {
      if (this->fileContext->v_exists->value && this->fileContext->v_isdirectory->value) {

         this->locContext->loadData(this->fileContext->trimmed);
         this->command->run();

         P_CHECK_SOFT_LOOP_BREAK;
      }
   }
}


void CS_InsideString::run()
{
   if (this->perun2.isRunning()) {
      this->context->resetIndex();
      this->context->loadData(this->string->getValue());

      if (this->context->v_exists->value && this->context->v_isdirectory->value) {
         this->context->aggregate.onStart();
         this->locContext->loadData(this->context->trimmed);
         this->command->run();

         P_CHECK_SOFT_LOOP_BREAK;

         this->context->aggregate.onIteration();
         this->context->aggregate.onFinish();
      }
   }
}


void CS_InsideDefinition::run()
{
   p_num index = NINT_ZERO;
   this->context->aggregate.onStart();
   this->context->resetIndex();

   while (definition->hasNext()) {
      if (!this->perun2.isRunning()) {
         this->definition->reset();
         break;
      }

      this->context->loadData(this->definition->getValue());

      if (this->context->v_exists->value && this->context->v_isdirectory->value) {
         this->locContext->loadData(this->context->trimmed);
         this->command->run();

         index++;
         this->context->index->value = index;

         P_CHECK_LOOP_BREAK;
         this->context->aggregate.onIteration();
      }
   }

   this->context->aggregate.onFinish();
}


CS_InsideContextless::CS_InsideContextless(p_defptr& def, p_comptr& com, p_lcptr& lctx, p_perun2& p2)
   : definition(std::move(def)), command(std::move(com)), locContext(std::move(lctx)), perun2(p2),
     fileContext(this->definition->getFileContext()) { };


void CS_InsideContextless::run()
{
   this->fileContext->aggregate.onStart();

   while (definition->hasNext()) {
      if (!this->perun2.isRunning()) {
         this->definition->reset();
         break;
      }

      if (this->fileContext->v_exists->value && this->fileContext->v_isdirectory->value) {
         this->locContext->loadData(os_trim(this->definition->getValue()));
         this->command->run();

         P_CHECK_LOOP_BREAK;
         this->fileContext->aggregate.onIteration();
      }
   }

   this->fileContext->aggregate.onFinish();
}


void CS_InsideList::run()
{
   const p_list values = this->list->getValue();
   const p_num length = p_num(static_cast<p_nint>(values.size()));

   if (length.value.i == 0) {
      return;
   }

   p_num index = NINT_ZERO;
   p_nint outIndex = NINT_ZERO;
   this->context->aggregate.onStart();
   this->context->resetIndex();

   while (this->perun2.isRunning() && index != length) {
      this->context->loadData(values[static_cast<p_size>(index.value.i)]);
      if (this->context->v_exists->value && this->context->v_isdirectory->value) {
         this->locContext->loadData(this->context->trimmed);
         this->command->run();

         outIndex++;
         this->context->index->value = outIndex;

         P_CHECK_LOOP_BREAK;
         this->context->aggregate.onIteration();
      }

      index++;
   }

   this->context->aggregate.onFinish();
}

}
