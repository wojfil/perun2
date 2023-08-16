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

#include "com-renameto.h"
#include "../perun2.h"
#include "../os.h"
#include "../datatype/patterns.h"


namespace perun2::comm
{

void C_RenameTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str n = os_fullname(os_trim(name->getValue()));

   if (!this->context->v_exists->value || os_isInvalid(n)
         || !os_hasParentDirectory(this->context->v_path->value) || os_isAbsolute(n)) {

      this->perun2.logger.log(L"Failed to rename ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(this->context->v_path->value);

   if (!extensionless && hasExt && !os_hasExtension(n)) {
      n += str(CHAR_DOT, os_extension(this->context->v_path->value));
   }

   const p_str base = os_parent(this->context->v_path->value);
   const p_str newPath = str(base, OS_SEPARATOR, n);

   if (os_exists(newPath)) {
      if (!(forced && os_drop(newPath, this->perun2))) {
         this->perun2.logger.log(L"Failed to rename ", getCCName(this->context->v_path->value));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_moveTo(this->context->v_path->value, newPath);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Rename ", getCCName(this->context->v_path->value), L" to '", n, L"'");

      if (saveChanges) {
         this->context->v_fullname->value = n;
         this->context->v_name->value = hasExt ? os_name(newPath) : n;
         this->context->v_path->value = newPath;
         this->context->this_->value = newPath;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to rename ", getCCName(this->context->v_path->value));
   }
}

void C_RenameTo_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   p_str n = os_fullname(os_trim(name->getValue()));
   if (!this->context->v_exists->value || os_isInvalid(n)
         || !os_hasParentDirectory(oldPath) || os_isAbsolute(n)) {

      this->perun2.logger.log(L"Failed to rename ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);
   const p_str parent = os_parent(oldPath);
   p_str newPath;
   p_str ex;

   if (hasExt) {
      if (os_hasExtension(n)) {
         ex = os_extension(n);
      }
      else {
         ex = os_extension(oldPath);
         if (!extensionless) {
            n += str(CHAR_DOT, ex);
         }
      }

      newPath = str(parent, OS_SEPARATOR, n);

      if (os_exists(newPath)) {
         const p_str base = os_stackPathBase(newPath);
         newPath = os_stackPathExt(base, ex);
      }
   }
   else {
      newPath = str(parent, OS_SEPARATOR, n);
      newPath = os_stackPath(newPath);
   }

   const p_bool s = os_moveTo(oldPath, newPath);
   
   if (hasExt) {
      n = os_fullname(newPath);

      if (s && saveChanges) {
         this->context->v_extension->value = ex;
      }
   }

   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Rename ", getCCName(this->context->v_path->value), L" to '", n, L"'");

      if (saveChanges) {
         this->context->v_fullname->value = n;
         this->context->v_name->value = hasExt ? os_name(n) : n;
         this->context->v_path->value = newPath;
         this->context->this_->value = newPath;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to rename ", getCCName(this->context->v_path->value));
   }
}

}
