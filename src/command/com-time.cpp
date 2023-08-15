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

#include "com-time.h"
#include "../perun2.h"
#include "../os.h"
#include "../datatype/patterns.h"


namespace perun2::comm
{


void C_ReaccessTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_tim t = this->context->v_access->value;
   t.setValue(time->getValue());
   this->perun2.contexts.success->value = this->context->v_exists->value
      && os_setTime(this->context->v_path->value, this->context->v_creation->value, t, this->context->v_modification->value);

   if (this->perun2.contexts.success->value) {
      this->perun2.logger.log(L"Reaccess ", getCCName(this->context->v_path->value), L" to ", t.toString());

      if (saveChanges) {
         this->context->v_access->value = t;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to reaccess ", getCCName(this->context->v_path->value));
   }
}

void C_RechangeTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_tim t = this->context->v_change->value;
   t.setValue(time->getValue());
   this->perun2.contexts.success->value = this->context->v_exists->value
      && os_setTime(this->context->v_path->value, this->context->v_creation->value, this->context->v_access->value, t);

   if (this->perun2.contexts.success->value) {
      this->perun2.logger.log(L"Rechange ", getCCName(this->context->v_path->value), L" to ", t.toString());

      if (saveChanges) {
         this->context->v_change->value = t;
         this->context->v_modification->value = t;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to rechange ", getCCName(this->context->v_path->value));
   }
}

void C_RecreateTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_tim t = this->context->v_creation->value;
   t.setValue(time->getValue());
   this->perun2.contexts.success->value = this->context->v_exists->value
      && os_setTime(this->context->v_path->value, t, this->context->v_access->value, this->context->v_modification->value);

   if (this->perun2.contexts.success->value) {
      this->perun2.logger.log(L"Recreate ", getCCName(this->context->v_path->value),  L" to ", t.toString());

      if (saveChanges) {
         this->context->v_creation->value = t;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to recreate ", getCCName(this->context->v_path->value));
   }
}

void C_RemodifyTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_tim t = this->context->v_modification->value;
   t.setValue(time->getValue());
   this->perun2.contexts.success->value = this->context->v_exists->value
      && os_setTime(this->context->v_path->value, this->context->v_creation->value, this->context->v_access->value, t);

   if (this->perun2.contexts.success->value) {
      this->perun2.logger.log(L"Remodify ", getCCName(this->context->v_path->value), L" to ", t.toString());

      if (saveChanges) {
         this->context->v_modification->value = t;
         this->context->v_change->value = t;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to remodify ", getCCName(this->context->v_path->value));
   }
}

}
