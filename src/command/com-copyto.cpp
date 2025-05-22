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

#include "com-copyto.h"
#include "../perun2.h"
#include "../os/os.h"
#include "../datatype/patterns.h"


namespace perun2::comm
{

void C_CopyTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   p_str n = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(n) || !os_hasParentDirectory(oldPath)) {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str newLoc = os_leftJoin(this->locationContext->location->value, n);

   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_str fulln = os_fullname(oldPath);
   const p_str newPath = str(newLoc, OS_SEPARATOR, fulln);

   if (os_exists(newPath)) {
      if (!(forced && !(this->context->v_isdirectory->value && os_isAncestor(oldPath, newPath))
         && os_drop(newPath, this->perun2))) 
      {
         this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc));
   }
   else {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
   }
}

void C_CopyTo_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   p_str n = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(n) || !os_hasParentDirectory(oldPath)) {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str newLoc = os_leftJoin(this->locationContext->location->value, n);
   
   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_str fulln = os_fullname(oldPath);
   p_str newPath = str(newLoc, OS_SEPARATOR, fulln);
   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

   if (os_exists(newPath)) {
      if (hasExt) {
         const p_str ext = os_extension(newPath);
         const p_str base = os_stackPathBase(newPath);
         newPath = os_stackPathExt(base, ext);
      }
      else {
         newPath = os_stackPath(newPath);
      }
   }

   const p_bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc));
   }
   else {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
   }
}

void C_CopyToAs::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   p_str fulln = os_fullname(os_trim(name->getValue()));
   const p_str loc = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(fulln) || os_isInvalid(loc) || !os_hasParentDirectory(oldPath))
   {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

   if (!extensionless && hasExt && !os_hasExtension(fulln)) {
      fulln += str(CHAR_DOT, os_extension(oldPath));
   }

   const p_str newLoc = os_leftJoin(this->locationContext->location->value, loc);

   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }


   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   p_str newPath = str(newLoc, OS_SEPARATOR, fulln);

   if (os_exists(newPath)) {
      if (!(forced && !(this->context->v_isdirectory->value && os_isAncestor(oldPath, newPath))
            && os_drop(newPath, this->perun2)))
      {
         this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as ", argQuoted(fulln));
   }
   else {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
   }
}

void C_CopyToAs_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& oldPath = this->context->v_path->value;
   p_str fulln = os_fullname(os_trim(name->getValue()));
   const p_str loc = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(fulln) || os_isInvalid(loc) || !os_hasParentDirectory(oldPath))
   {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str newLoc = os_leftJoin(this->locationContext->location->value, loc);

   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   p_str newPath;

   if (hasExt) {
      p_str ex;
      if (os_hasExtension(fulln)) {
         ex = os_extension(fulln);
      }
      else {
         ex = os_extension(oldPath);
         if (!extensionless) {
            fulln += str(CHAR_DOT, ex);
         }
      }

      newPath = str(newLoc, OS_SEPARATOR, fulln);
      if (os_exists(newPath)) {
         const p_str base = os_stackPathBase(newPath);
         newPath = os_stackPathExt(base, ex);
         fulln = os_fullname(newPath);
      }
   }
   else {
      newPath = str(newLoc, OS_SEPARATOR, fulln);
      if (os_exists(newPath)) {
         newPath = os_stackPath(newPath);
         fulln = os_fullname(newPath);
      }
   }

   const p_bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as ", argQuoted(fulln));
   }
   else {
      this->perun2.logger.log(L"Failed to copy ", getCCName(oldPath));
   }
}

}
