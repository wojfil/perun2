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

#include "../../include/perun2/command/com-create.hpp"
#include "../../include/perun2/perun2.hpp"
#include "../../include/perun2/os/os.hpp"
#include "../../include/perun2/datatype/patterns.hpp"


namespace perun2::comm
{

void C_Create::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   if (!this->context->v_isfile->value && !this->context->v_isdirectory->value) {
      this->perun2.logger.log(L"Failed to create ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (this->context->v_exists->value) {
      if (!(forced && os_drop(this->context->v_path->value, this->context->v_isfile->value, this->perun2))) {
         if (this->context->v_isfile->value) {
            this->perun2.logger.log(L"Failed to create file ", getCCName(this->context->v_path->value));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCName(this->context->v_path->value));
         }
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   if (os_hasExtension(this->context->v_path->value)) {
      const p_bool s = os_createFile(this->context->v_path->value);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create file ", getCCName(this->context->v_path->value));
         this->context->reloadData();
      }
      else {
         this->perun2.logger.log(L"Failed to create file ", getCCName(this->context->v_path->value));
      }
   }
   else {
      const p_bool s = os_createDirectory(this->context->v_path->value);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create directory ", getCCName(this->context->v_path->value));
         this->context->reloadData();
      }
      else {
         this->perun2.logger.log(L"Failed to create directory ", getCCName(this->context->v_path->value));
      }
   }
}


void C_Create_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   if (!this->context->v_isfile->value && !this->context->v_isdirectory->value) {
      this->perun2.logger.log(L"Failed to create ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str path = this->context->v_path->value;
   const p_bool hasExt = os_hasExtension(path);
   p_bool nameChanged = false;

   if (this->context->v_exists->value) {
      nameChanged = true;
      if (hasExt) {
         const p_str ex = os_extension(path);
         const p_str base = os_stackPathBase(path);
         path = os_stackPathExt(base, ex);
      }
      else {
         path = os_stackPath(path);
      }
   }

   if (hasExt) {
      const p_bool s = os_createFile(path);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create file ", getCCName(path));
         if (nameChanged) {
            this->context->this_->value = path;
         }
         this->context->reloadData();
      }
      else {
         this->perun2.logger.log(L"Failed to create file ", getCCName(path));
      }
   }
   else {
      const p_bool s = os_createDirectory(path);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create directory ", getCCName(path));
         if (nameChanged) {
            this->context->this_->value = path;
         }
         this->context->reloadData();
      }
      else {
         this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
      }
   }
}


void C_CreateFile::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   if (!this->context->v_isfile->value && !this->context->v_isdirectory->value) {
      this->perun2.logger.log(L"Failed to create file ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (this->context->v_exists->value) {
      if (!(forced && os_drop(this->context->v_path->value, this->context->v_isfile->value, this->perun2))) {
         this->perun2.logger.log(L"Failed to create file ", getCCName(this->context->v_path->value));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_createFile(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create file ", getCCName(this->context->v_path->value));
      this->context->reloadData();
   }
   else {
      this->perun2.logger.log(L"Failed to create file ", getCCName(this->context->v_path->value));
   }
}


void C_CreateFile_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   if (!this->context->v_isfile->value && !this->context->v_isdirectory->value) {
      this->perun2.logger.log(L"Failed to create file ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str path = this->context->v_path->value;
   const p_bool hasExt = os_hasExtension(path);
   p_bool nameChanged = false;

   if (this->context->v_exists->value) {
      nameChanged = true;
      if (hasExt) {
         const p_str ex = os_extension(path);
         const p_str base = os_stackPathBase(path);
         path = os_stackPathExt(base, ex);
      }
      else {
         path = os_stackPath(path);
      }
   }

   const p_bool s = os_createFile(path);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create file ", getCCName(path));
      if (nameChanged) {
         this->context->this_->value = path;
      }
      this->context->reloadData();
   }
   else {
      this->perun2.logger.log(L"Failed to create file ", getCCName(path));
   }
}


void C_CreateDirectory::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   if (!this->context->v_isfile->value && !this->context->v_isdirectory->value) {
      this->perun2.logger.log(L"Failed to create directory ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (this->context->v_exists->value) {
      if (!(forced && os_drop(this->context->v_path->value, this->context->v_isfile->value, this->perun2))) {
         this->perun2.logger.log(L"Failed to create directory ", getCCName(this->context->v_path->value));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_createDirectory(this->context->v_path->value);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create directory ", getCCName(this->context->v_path->value));
      this->context->reloadData();
   }
   else {
      this->perun2.logger.log(L"Failed to create directory ", getCCName(this->context->v_path->value));
   }
}


void C_CreateDirectory_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   if (!this->context->v_isfile->value && !this->context->v_isdirectory->value) {
      this->perun2.logger.log(L"Failed to create directory ", getCCName(this->context->v_path->value));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str path = this->context->v_path->value;
   p_bool nameChanged = false;

   if (this->context->v_exists->value) {
      nameChanged = true;
      path = os_stackPath(path);
   }

   const p_bool s = os_createDirectory(path);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create directory ", getCCName(path));
      if (nameChanged) {
         this->context->this_->value = path;
      }
      this->context->reloadData();
   }
   else {
      this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
   }
}

// others


void C_Create_String::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str value = os_trim(element->getValue());
   const p_str& dest = this->locContext->location->value;

   if (os_isInvalid(value) || !os_directoryExists(dest)) {
      this->perun2.logger.log(L"Failed to create ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool isFile = os_hasExtension(value);
   const p_str path = os_leftJoin(dest, value);

   if (path.empty()) {
      this->perun2.logger.log(L"Failed to create ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (os_exists(path)) {
      if (!(forced && os_drop(path, this->perun2))) {
         if (isFile) {
            this->perun2.logger.log(L"Failed to create file ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
         }
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   if (isFile) {
      const p_bool s = os_createFile(path);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create file ", getCCName(path));
      }
      else {
         this->perun2.logger.log(L"Failed to create file ", getCCName(path));
      }
   }
   else {
      const p_bool s = os_createDirectory(path);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create directory ", getCCName(path));
      }
      else {
         this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
      }
   }
}


void C_CreateFile_String::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str value = os_trim(element->getValue());
   const p_str& dest = this->locContext->location->value;

   if (os_isInvalid(value) || !os_directoryExists(dest)) {
      this->perun2.logger.log(L"Failed to create file ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str path = os_leftJoin(dest, value);

   if (path.empty()) {
      this->perun2.logger.log(L"Failed to create file ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (os_exists(path)) {
      if (!(forced && os_drop(path, this->perun2))) {
         this->perun2.logger.log(L"Failed to create file ", getCCName(path));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_createFile(path);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create file ", getCCName(path));
   }
   else {
      this->perun2.logger.log(L"Failed to create file ", getCCName(path));
   }
}


void C_CreateDirectory_String::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str value = os_trim(element->getValue());
   const p_str& dest = this->locContext->location->value;

   if (os_isInvalid(value) || !os_directoryExists(dest)) {
      this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str path = os_leftJoin(dest, value);
   
   if (path.empty()) {
      this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (os_exists(path)) {
      if (!(forced && os_drop(path, this->perun2))) {
         this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
         this->perun2.contexts.success->value = false;
         return;
      }
   }

   const p_bool s = os_createDirectory(path);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create directory ", getCCName(path));
   }
   else {
      this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
   }
}


void C_Create_String_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str value = os_trim(element->getValue());
   const p_str& dest = this->locContext->location->value;

   if (os_isInvalid(value) || !os_directoryExists(dest)) {
      this->perun2.logger.log(L"Failed to create ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str path = os_leftJoin(dest, value);

   if (path.empty()) {
      this->perun2.logger.log(L"Failed to create ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = os_hasExtension(value);

   if (os_exists(path)) {
      if (hasExt) {
         const p_str ex = os_extension(path);
         const p_str base = os_stackPathBase(path);
         path = os_stackPathExt(base, ex);
      }
      else {
         path = os_stackPath(path);
      }
   }

   if (hasExt) {
      const p_bool s = os_createFile(path);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create file ", getCCName(path));
      }
      else {
         this->perun2.logger.log(L"Failed to create file ", getCCName(path));
      }
   }
   else {
      const p_bool s = os_createDirectory(path);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Create directory ", getCCName(path));
      }
      else {
         this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
      }
   }
}


void C_CreateFile_String_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str value = os_trim(element->getValue());
   const p_str& dest = this->locContext->location->value;

   if (os_isInvalid(value) || !os_directoryExists(dest)) {
      this->perun2.logger.log(L"Failed to create file ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str path = os_leftJoin(dest, value);

   if (path.empty()) {
      this->perun2.logger.log(L"Failed to create file ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_bool hasExt = os_hasExtension(value);

   if (os_exists(path)) {
      if (hasExt) {
         const p_str ex = os_extension(path);
         const p_str base = os_stackPathBase(path);
         path = os_stackPathExt(base, ex);
      }
      else {
         path = os_stackPath(path);
      }
   }

   const p_bool s = os_createFile(path);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create file ", getCCName(path));
   }
   else {
      this->perun2.logger.log(L"Failed to create file ", getCCName(path));
   }
}


void C_CreateDirectory_String_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str value = os_trim(element->getValue());
   const p_str& dest = this->locContext->location->value;

   if (os_isInvalid(value) || !os_directoryExists(dest)) {
      this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   p_str path = os_leftJoin(dest, value);

   if (path.empty()) {
      this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(value));
      this->perun2.contexts.success->value = false;
      return;
   }

   if (os_exists(path)) {
      path = os_stackPath(path);
   }

   const p_bool s = os_createDirectory(path);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Create directory ", getCCName(path));
   }
   else {
      this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
   }
}

// others multi:

void C_Create_List::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& dest = this->locContext->location->value;
   const p_list names = elements->getValue();
   const p_size len = names.size();

   if (len == 0) {
      this->perun2.contexts.success->value = true;
      return;
   }

   if (!os_directoryExists(dest)) {
      for (p_size i = 0; i < len; i++) {
         const p_str n = os_trim(names[i]);
         const p_bool isFile = os_hasExtension(n);

         if (isFile) {
            this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
         }
      }
      this->perun2.contexts.success->value = false;
      return;
   }

   p_bool success = true;

   for (p_size i = 0; i < len; i++) {
      const p_str n = os_trim(names[i]);
      const p_bool isFile = os_hasExtension(n);

      if (os_isInvalid(n)) {
         if (isFile) {
            this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
         }
         success = false;
         continue;
      }

      const p_str path = os_leftJoin(dest, n);

      if (path.empty()) {
         if (isFile) {
            this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
         }
         success = false;
         continue;
      }

      if (os_exists(path)) {
         if (!(forced && os_drop(path, this->perun2))) {
            if (isFile) {
               this->perun2.logger.log(L"Failed to create file ", getCCName(path));
            }
            else {
               this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
            }
            success = false;
            continue;
         }
      }

      if (os_hasExtension(n)) {
         const p_bool s = os_createFile(path);
         this->perun2.contexts.success->value = s;

         if (s) {
            this->perun2.logger.log(L"Create file ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create file ", getCCName(path));
            success = false;
         }
      }
      else {
         const p_bool s = os_createDirectory(path);
         this->perun2.contexts.success->value = s;

         if (s) {
            this->perun2.logger.log(L"Create directory ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
            success = false;
         }
      }
   }

   this->perun2.contexts.success->value = success;
}


void C_CreateFiles_List::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& dest = this->locContext->location->value;
   const p_list names = elements->getValue();
   const p_size len = names.size();
   if (len == 0) {
      this->perun2.contexts.success->value = true;
      return;
   }

   if (!os_directoryExists(dest)) {
      for (p_size i = 0; i < len; i++) {
         const p_str n = os_trim(names[i]);
         this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
      }
      this->perun2.contexts.success->value = false;
      return;
   }

   p_bool success = true;

   for (p_size i = 0; i < len; i++) {
      const p_str n = os_trim(names[i]);

      if (os_isInvalid(n)) {
         this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
         success = false;
      }
      else {
         const p_str path = os_leftJoin(dest, n);

         if (path.empty()) {
            this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
            success = false;
            continue;
         }

         if (os_exists(path)) {
            if (!(forced && os_drop(path, this->perun2))) {
               this->perun2.logger.log(L"Failed to create file ", getCCName(path));
               success = false;
               continue;
            }
         }

         const p_bool s = os_createFile(path);
         this->perun2.contexts.success->value = s;

         if (s) {
            this->perun2.logger.log(L"Create file ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create file ", getCCName(path));
            success = false;
         }
      }
   }

   this->perun2.contexts.success->value = success;
}


void C_CreateDirectories_List::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& dest = this->locContext->location->value;
   const p_list names = elements->getValue();
   const p_size len = names.size();
   if (len == 0) {
      this->perun2.contexts.success->value = true;
      return;
   }

   if (!os_directoryExists(dest)) {
      for (p_size i = 0; i < len; i++) {
         const p_str n = os_trim(names[i]);
         this->perun2.logger.log(L"Failed to directory ", getCCNameShort(n));
      }
      this->perun2.contexts.success->value = false;
      return;
   }

   p_bool success = true;

   for (p_size i = 0; i < len; i++) {
      const p_str n = os_trim(names[i]);

      if (os_isInvalid(n)) {
         this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
         success = false;
      }
      else {
         const p_str path = os_leftJoin(dest, n);

         if (path.empty()) {
            this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
            success = false;
            continue;
         }

         if (os_exists(path)) {
            if (!(forced && os_drop(path, this->perun2))) {
               this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
               success = false;
               continue;
            }
         }

         const p_bool s = os_createDirectory(path);
         this->perun2.contexts.success->value = s;

         if (s) {
            this->perun2.logger.log(L"Create directory ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
            success = false;
         }
      }
   }

   this->perun2.contexts.success->value = success;
}


void C_Create_List_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& dest = this->locContext->location->value;
   const p_list names = elements->getValue();
   const p_size len = names.size();
   if (len == 0) {
      this->perun2.contexts.success->value = true;
      return;
   }

   if (!os_directoryExists(dest)) {
      for (p_size i = 0; i < len; i++) {
         const p_str n = os_trim(names[i]);
         const p_bool isFile = os_hasExtension(n);

         if (isFile) {
            this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
         }
      }
      this->perun2.contexts.success->value = false;
      return;
   }

   p_bool success = true;

   for (p_size i = 0; i < len; i++) {
      const p_str n = os_trim(names[i]);

      if (os_isInvalid(n)) {
         this->perun2.logger.log(L"Failed to create ", getCCNameShort(n));
         success = false;
      }
      else {
         p_str path = os_leftJoin(dest, n);

         if (path.empty()) {
            this->perun2.logger.log(L"Failed to create ", getCCNameShort(n));
            success = false;
            continue;
         }

         const p_bool hasExt = os_hasExtension(path);

         if (os_exists(path)) {
            if (hasExt) {
               const p_str ext = os_extension(path);
               const p_str base = os_stackPathBase(path);
               path = os_stackPathExt(base, ext);
            }
            else {
               path = os_stackPath(path);
            }
         }

         if (hasExt) {
            const p_bool s = os_createFile(path);
            this->perun2.contexts.success->value = s;

            if (s) {
               this->perun2.logger.log(L"Create file ", getCCName(path));
            }
            else {
               this->perun2.logger.log(L"Failed to create file ", getCCName(path));
               success = false;
            }
         }
         else {
            const p_bool s = os_createDirectory(path);
            this->perun2.contexts.success->value = s;

            if (s) {
               this->perun2.logger.log(L"Create directory ", getCCName(path));
            }
            else {
               this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
               success = false;
            }
         }
      }
   }

   this->perun2.contexts.success->value = success;
}


void C_CreateFiles_List_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& dest = this->locContext->location->value;
   const p_list names = elements->getValue();
   const p_size len = names.size();
   if (len == 0) {
      this->perun2.contexts.success->value = true;
      return;
   }

   if (!os_directoryExists(dest)) {
      for (p_size i = 0; i < len; i++) {
         const p_str n = os_trim(names[i]);
         this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
      }
      this->perun2.contexts.success->value = false;
      return;
   }

   p_bool success = true;

   for (p_size i = 0; i < len; i++) {
      const p_str n = os_trim(names[i]);

      if (os_isInvalid(n)) {
         this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
         success = false;
      }
      else {
         p_str path = os_leftJoin(dest, n);

         if (path.empty()) {
            this->perun2.logger.log(L"Failed to create file ", getCCNameShort(n));
            success = false;
            continue;
         }

         const p_bool hasExt = os_hasExtension(path);

         if (os_exists(path)) {
            if (hasExt) {
               const p_str ext = os_extension(path);
               const p_str base = os_stackPathBase(path);
               path = os_stackPathExt(base, ext);
            }
            else {
               path = os_stackPath(path);
            }
         }

         const p_bool s = os_createFile(path);
         this->perun2.contexts.success->value = s;

         if (s) {
            this->perun2.logger.log(L"Create file ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create file ", getCCName(path));
            success = false;
         }
      }
   }

   this->perun2.contexts.success->value = success;
}


void C_CreateDirectories_List_Stack::run()
{
   P_CHECK_IF_PERUN2_IS_RUNNING;

   const p_str& dest = this->locContext->location->value;
   const p_list names = elements->getValue();
   const p_size len = names.size();
   if (len == 0) {
      this->perun2.contexts.success->value = true;
      return;
   }

   if (!os_directoryExists(dest)) {
      for (p_size i = 0; i < len; i++) {
         const p_str n = os_trim(names[i]);
         this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
      }
      this->perun2.contexts.success->value = false;
      return;
   }

   p_bool success = true;

   for (p_size i = 0; i < len; i++) {
      const p_str n = os_trim(names[i]);

      if (os_isInvalid(n)) {
         this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
         success = false;
      }
      else {
         p_str path = os_leftJoin(dest, n);

         if (path.empty()) {
            this->perun2.logger.log(L"Failed to create directory ", getCCNameShort(n));
            success = false;
            continue;
         }

         if (os_exists(path)) {
            path = os_stackPath(path);
         }

         const p_bool s = os_createDirectory(path);
         this->perun2.contexts.success->value = s;

         if (s) {
            this->perun2.logger.log(L"Create directory ", getCCName(path));
         }
         else {
            this->perun2.logger.log(L"Failed to create directory ", getCCName(path));
            success = false;
         }
      }
   }

   this->perun2.contexts.success->value = success;
}

}
