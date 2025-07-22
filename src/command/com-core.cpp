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

#include "com-core.h"
#include "../perun2.h"
#include "../os/os.h"
#include "../datatype/patterns.h"


namespace perun2::comm
{

CoreCommand::CoreCommand(FileContext* ctx, Perun2Process& p2)
   : saveChanges(true), context(ctx), perun2(p2), locationContext(p2.contexts.getLocationContext()) { };
   
CoreCommand::CoreCommand(const p_bool save, FileContext* ctx, Perun2Process& p2)
   : saveChanges(save), context(ctx), perun2(p2), locationContext(p2.contexts.getLocationContext()) { };


////
//   simple commands:
////

void C_Delete::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_delete(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Delete ", getCCName(this->context->v_path->value));
      this->context->reloadData();
   }
   else {
      this->perun2.logger.log(L"Failed to delete ", getCCName(this->context->v_path->value));
   }
}

void C_Drop::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_drop(this->context->v_path->value, this->context->v_isfile->value, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Drop ", getCCName(this->context->v_path->value));
      if (saveChanges) {
         this->context->reloadData();
      }
   }
   else {
      this->perun2.logger.log(L"Failed to drop ", getCCName(this->context->v_path->value));
   }
}

void C_Hide::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_hide(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Hide ", getCCName(this->context->v_path->value));
      if (saveChanges) {
         this->context->v_hidden->value = true;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to hide ", getCCName(this->context->v_path->value));
   }
}

void C_Lock::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_lock(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Lock ", getCCName(this->context->v_path->value));
      if (saveChanges) {
         this->context->v_readonly->value = true;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to lock ", getCCName(this->context->v_path->value));
   }
}

void C_Open::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_open(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Open ", getCCName(this->context->v_path->value));
   }
   else {
      this->perun2.logger.log(L"Failed to open ", getCCName(this->context->v_path->value));
   }
}

void C_Unlock::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_unlock(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Unlock ", getCCName(this->context->v_path->value));
      if (saveChanges) {
         this->context->v_readonly->value = false;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to unlock ", getCCName(this->context->v_path->value));
   }
}

void C_Unhide::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_bool s = this->context->v_exists->value && os_unhide(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Unhide ", getCCName(this->context->v_path->value));
      if (saveChanges) {
         this->context->v_hidden->value = false;
      }
   }
   else {
      this->perun2.logger.log(L"Failed to unhide ", getCCName(this->context->v_path->value));
   }
}

void C_OpenWith::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str pro = os_trim(program->getValue());

   if (!this->context->v_exists->value || pro.empty()) {
      this->perun2.logger.log(L"Failed to open ", getCCName(this->context->v_path->value), L" with ", getCCNameShort(pro));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str proPath = os_leftJoin(this->locationContext->location->value, pro);

   if (proPath.empty()) {
      this->perun2.logger.log(L"Failed to open ", getCCName(this->context->v_path->value), L" with ", getCCNameShort(pro));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasParent = os_hasParentDirectory(this->context->v_path->value);
   const p_str p = hasParent 
      ? this->context->v_path->value
      : str(this->context->v_path->value, OS_SEPARATOR);
   
   const p_str com = str(
      os_fileExists(proPath) ? os_quoteEmbraced(proPath) : pro, 
      CHAR_SPACE, 
      os_quoteEmbraced(p));

   const p_str loc = hasParent
      ? os_parent(this->context->v_path->value) 
      : str(this->context->v_path->value, OS_SEPARATOR);

   const p_bool s = os_openAsCommand(com, loc);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Open ", getCCName(this->context->v_path->value), L" with ", getCCName(proPath));
   }
   else {
      this->perun2.logger.log(L"Failed to open ", getCCName(this->context->v_path->value), L" with ", getCCName(proPath));
   }
};

p_str getCCName(const p_str& path)
{
   if (path.empty()) {
      return STRING_NOTHING;
   }

   return os_hasParentDirectory(path) || path.size() != 2
      ? str(CHAR_QUOTATION_MARK, os_fullname(path), CHAR_QUOTATION_MARK)
      : str(CHAR_QUOTATION_MARK, path, OS_SEPARATOR, CHAR_QUOTATION_MARK);
}

p_str getCCNameShort(const p_str& path)
{
   if (path.empty()) {
      return STRING_NOTHING;
   }

   const p_str f = os_fullname(path);
   if (f.empty()) {
      return STRING_NOTHING;
   }

   return f.size() == 2 && f[1] == CHAR_COLON && os_isDriveLetter(f[0])
      ? str(CHAR_QUOTATION_MARK, f, OS_SEPARATOR, CHAR_QUOTATION_MARK)
      : str(CHAR_QUOTATION_MARK, f, CHAR_QUOTATION_MARK);
}

p_str argQuoted(const p_str& value)
{
   return str(CHAR_QUOTATION_MARK, value, CHAR_QUOTATION_MARK);
}

}
