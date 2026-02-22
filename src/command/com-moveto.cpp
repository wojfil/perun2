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

#include "com-moveto.hpp"
#include "../perun2.hpp"
#include "../os/os.hpp"
#include "../datatype/patterns.hpp"


namespace perun2::comm
{

void changeValueOfThisAfterMoving(FileContext& context, const p_str& location, const p_str& newPath)
{
   const p_str fullname = os_fullname(newPath);
   const p_str newThis = str(location, OS_SEPARATOR, fullname);
   context.loadData(newThis);
}

void C_MoveTo::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   const p_str n = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(n)
         || !os_hasParentDirectory(oldPath)) {

      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str newLoc = os_leftJoin(this->locationContext->location->value, n);

   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
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
         this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_moveTo(oldPath, newPath);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc));

      if (this->saveChanges) {
         changeValueOfThisAfterMoving(*this->context, n, newPath);
      }
   }
   else {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
   }
}

void C_MoveTo_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   const p_str n = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(n) || !os_hasParentDirectory(oldPath)) {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str newLoc = os_leftJoin(this->locationContext->location->value, n);
   
   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
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

   const p_bool s = os_moveTo(oldPath, newPath);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc));

      if (this->saveChanges) {
         changeValueOfThisAfterMoving(*this->context, n, newPath);
      }
   }
   else {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
   }
}

void C_MoveToAs::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   p_str& oldPath = this->context->v_path->value;
   p_str fulln = os_fullname(os_trim(name->getValue()));
   const p_str loc = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(fulln)
         || os_isInvalid(loc) || !os_hasParentDirectory(oldPath)) {

      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

   if (!extensionless && hasExt && !os_hasExtension(fulln)) {
      fulln += str(CHAR_DOT, os_extension(oldPath));
   }

   const p_str newLoc = os_leftJoin(this->locationContext->location->value, loc);

   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   p_str newPath = str(newLoc, OS_SEPARATOR, fulln);

   if (os_exists(newPath)) {
      if (!(forced && !(this->context->v_isdirectory->value && os_isAncestor(oldPath, newPath))
            && os_drop(newPath, this->perun2))) 
      {
         this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_moveTo(oldPath, newPath);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as ", argQuoted(fulln));

      if (this->saveChanges) {
         changeValueOfThisAfterMoving(*this->context, loc, newPath);
      }
   }
   else {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
   }
}

void C_MoveToAs_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& oldPath = this->context->v_path->value;
   p_str fulln = os_fullname(os_trim(name->getValue()));
   const p_str loc = os_trim(location->getValue());

   if (!this->context->v_exists->value || os_isInvalid(fulln)
         || os_isInvalid(loc) || !os_hasParentDirectory(oldPath)) {

      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);
   const p_str newLoc = os_leftJoin(this->locationContext->location->value, loc);

   if (newLoc.empty()) {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (!os_directoryExists(newLoc)) {
      if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
         this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   p_str newPath;
   p_bool s;

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

   s = os_moveTo(oldPath, newPath);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as ", argQuoted(fulln));

      if (this->saveChanges) {
         changeValueOfThisAfterMoving(*this->context, loc, newPath);
      }
   }
   else {
      this->perun2.logger.log(L"Failed to move ", getCCName(oldPath));
   }
}

}
