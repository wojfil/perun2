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

#include "com-struct.h"
#include "..\os.h"
#include "..\uroboros.h"


namespace uro::comm
{


void CS_RawBlock::run()
{
   for (_size i = 0; this->uroboros.state == State::s_Running && i < this->length; i++) {
      (this->commands[i])->run();
   }
}


void CS_Block::run()
{
   for (_size i = 0; this->uroboros.state == State::s_Running && i < this->length; i++) {
      (this->commands[i])->run();
   }

   this->context->runAggregate();
}


void CS_Times::run()
{
   _nint repeats = this->times->getValue().toInt();
   if (repeats <= NINT_ZERO) {
      return;
   }

   this->context->resetIndex();

   while (this->uroboros.state == State::s_Running && repeats != NINT_ZERO) {
      this->command->run();
      this->context->incrementIndex();
      repeats--;

      P_CHECK_LOOP_BREAK;
   }

   this->context->runAggregate();
}


void CS_While::run()
{
   this->context->resetIndex();

   while (this->uroboros.state == State::s_Running && this->condition->getValue()) {
      this->command->run();
      this->context->incrementIndex();

      P_CHECK_LOOP_BREAK;
   }

   this->context->runAggregate();
}


void CS_StringLoop::run()
{
   if (this->uroboros.state == State::s_Running) {
      this->context->resetIndexAndDepth();
      this->context->loadData(this->string->getValue());
      this->command->run();

      P_CHECK_SOFT_LOOP_BREAK;

      this->context->runAggregate();
   }
}


void CS_DefinitionLoop::run()
{
   _num index = NINT_ZERO;
   this->context->resetIndexAndDepth();

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      this->context->loadData(this->definition->getValue());
      this->command->run();

      P_CHECK_LOOP_BREAK;

      index++;
      this->context->index->value = index;
   }

   this->context->runAggregate();
}


CS_ContextlessLoop::CS_ContextlessLoop(_defptr& def, _comptr& com, _uro& uro)
   : definition(std::move(def)), command(std::move(com)), uroboros(uro),
     context(this->definition->getFileContext()) { };


void CS_ContextlessLoop::run()
{
   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      this->command->run();
      P_CHECK_LOOP_BREAK;
   }

   this->context->runAggregate();
}


void CS_ListLoop::run()
{
   const _list values = this->list->getValue();
   const _num length = _numi(static_cast<_nint>(values.size()));

   if (length == 0) {
      return;
   }

   _num index = NINT_ZERO;
   this->context->resetIndexAndDepth();

   while (this->uroboros.state == State::s_Running && index != length) {
      this->context->loadData(values[static_cast<_size>(index.value.i)]);
      this->command->run();
      index++;
      this->context->index->value = index;

      P_CHECK_LOOP_BREAK;
   }

   this->context->runAggregate();
}


void CS_InsideThis::run()
{
   if (this->uroboros.state == State::s_Running) {
      if (this->fileContext->v_exists->value && this->fileContext->v_isdirectory->value) {

         this->locContext->loadData(this->fileContext->this_->value);
         this->command->run();

         P_CHECK_SOFT_LOOP_BREAK;
      }
   }
}


void CS_InsideString::run()
{
   if (this->uroboros.state == State::s_Running) {
      this->context->resetIndexAndDepth();
      this->context->loadData(this->string->getValue());

      if (this->context->v_exists->value && this->context->v_isdirectory->value) {
         this->locContext->loadData(this->context->this_->value);
         this->command->run();

         P_CHECK_SOFT_LOOP_BREAK;

         this->context->runAggregate();
      }
   }
}


void CS_InsideDefinition::run()
{
   _num index = NINT_ZERO;
   this->context->resetIndexAndDepth();

   while (definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      this->context->loadData(this->definition->getValue());

      if (this->context->v_exists->value && this->context->v_isdirectory->value) {
         this->locContext->loadData(this->definition->getValue());
         this->command->run();

         P_CHECK_LOOP_BREAK;

         index++;
         this->context->index->value = index;
      }
   }

   this->context->runAggregate();
}


CS_InsideContextless::CS_InsideContextless(_defptr& def, _comptr& com, _lcptr& lctx, _uro& uro)
   : definition(std::move(def)), command(std::move(com)), locContext(std::move(lctx)), uroboros(uro),
     fileContext(this->definition->getFileContext()) { };


void CS_InsideContextless::run()
{
   while (definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      if (this->fileContext->v_exists->value && this->fileContext->v_isdirectory->value) {
         this->locContext->loadData(this->definition->getValue());
         this->command->run();

         P_CHECK_LOOP_BREAK;
      }
   }

   this->fileContext->runAggregate();
}


void CS_InsideList::run()
{
   const _list values = this->list->getValue();
   const _num length = _num(static_cast<_nint>(values.size()));

   if (length.value.i == 0) {
      return;
   }

   _num index = NINT_ZERO;
   _nint outIndex = NINT_ZERO;
   this->context->resetIndexAndDepth();

   while (this->uroboros.state == State::s_Running && index != length) {
      this->context->loadData(values[static_cast<_size>(index.value.i)]);
      if (this->context->v_exists->value && this->context->v_isdirectory->value) {
         this->locContext->loadData(this->context->this_->value);
         this->command->run();

         P_CHECK_LOOP_BREAK;

         outIndex++;
         this->context->index->value = outIndex;
      }

      index++;
   }

   this->context->runAggregate();
}

}
