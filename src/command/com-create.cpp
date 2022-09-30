/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#include "com-create.h"
#include "../uroboros.h"
#include "../print.h"
#include "../os.h"


namespace uro::comm
{

void C_Create::run()
{
   if (this->uroboros->state == State::s_Running) {
      if (!this->inner->isfile.value && !this->inner->isdirectory.value) {
         commandLog(this->uroboros, L"Failed to create ", getCCName(this->inner->path.value));
         this->inner->success.value = false;
         return;
      }

      if (this->inner->exists.value) {
         if (!(forced && os_drop(this->inner->path.value, this->inner->isfile.value, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to create ", getCCName(this->inner->path.value));
            this->inner->success.value = false;
            return;
         }
      }

      if (os_hasExtension(this->inner->path.value)) {
         const _bool s = os_createFile(this->inner->path.value);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create file ", getCCName(this->inner->path.value));
            this->attribute->run();
         }
         else {
            commandLog(this->uroboros, L"Failed to create file ", getCCName(this->inner->path.value));
         }
      }
      else {
         const _bool s = os_createDirectory(this->inner->path.value);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create directory ", getCCName(this->inner->path.value));
            this->attribute->run();
         }
         else {
            commandLog(this->uroboros, L"Failed to create directory ", getCCName(this->inner->path.value));
         }
      }
   }
}


void C_Create_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      if (!this->inner->isfile.value && !this->inner->isdirectory.value) {
         commandLog(this->uroboros, L"Failed to create ", getCCName(this->inner->path.value));
         this->inner->success.value = false;
         return;
      }

      _str path = this->inner->path.value;
      const _bool hasExt = os_hasExtension(path);
      _bool nameChanged = false;

      if (this->inner->exists.value) {
         nameChanged = true;
         if (hasExt) {
            const _str ex = os_extension(path);
            const _str base = os_stackPathBase(path);
            path = os_stackPathExt(base, ex);
         }
         else {
            path = os_stackPath(path);
         }
      }

      if (hasExt) {
         const _bool s = os_createFile(path);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create file ", getCCName(path));
            if (nameChanged) {
               this->inner->this_s.value = path;
            }
            this->attribute->run();
         }
         else {
            commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
         }
      }
      else {
         const _bool s = os_createDirectory(path);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create directory ", getCCName(path));
            if (nameChanged) {
               this->inner->this_s.value = path;
            }
            this->attribute->run();
         }
         else {
            commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
         }
      }
   }
}


void C_CreateFile::run()
{
   if (this->uroboros->state == State::s_Running) {
      if (!this->inner->isfile.value && !this->inner->isdirectory.value) {
         commandLog(this->uroboros, L"Failed to create file ", getCCName(this->inner->path.value));
         this->inner->success.value = false;
         return;
      }

      if (this->inner->exists.value) {
         if (!(forced && os_drop(this->inner->path.value, this->inner->isfile.value, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to create file ", getCCName(this->inner->path.value));
            this->inner->success.value = false;
            return;
         }
      }

      const _bool s = os_createFile(this->inner->path.value);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create file ", getCCName(this->inner->path.value));
         this->attribute->run();
      }
      else {
         commandLog(this->uroboros, L"Failed to create file ", getCCName(this->inner->path.value));
      }
   }
}


void C_CreateFile_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      if (!this->inner->isfile.value && !this->inner->isdirectory.value) {
         commandLog(this->uroboros, L"Failed to create file ", getCCName(this->inner->path.value));
         this->inner->success.value = false;
         return;
      }

      _str path = this->inner->path.value;
      const _bool hasExt = os_hasExtension(path);
      _bool nameChanged = false;

      if (this->inner->exists.value) {
         nameChanged = true;
         if (hasExt) {
            const _str ex = os_extension(path);
            const _str base = os_stackPathBase(path);
            path = os_stackPathExt(base, ex);
         }
         else {
            path = os_stackPath(path);
         }
      }

      const _bool s = os_createFile(path);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create file ", getCCName(path));
         if (nameChanged) {
            this->inner->this_s.value = path;
         }
         this->attribute->run();
      }
      else {
         commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
      }
   }
}


void C_CreateDirectory::run()
{
   if (this->uroboros->state == State::s_Running) {
      if (!this->inner->isfile.value && !this->inner->isdirectory.value) {
         commandLog(this->uroboros, L"Failed to create directory ", getCCName(this->inner->path.value));
         this->inner->success.value = false;
         return;
      }

      if (this->inner->exists.value) {
         if (!(forced && os_drop(this->inner->path.value, this->inner->isfile.value, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to create directory ", getCCName(this->inner->path.value));
            this->inner->success.value = false;
            return;
         }
      }

      const _bool s = os_createDirectory(this->inner->path.value);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create directory ", getCCName(this->inner->path.value));
         this->attribute->run();
      }
      else {
         commandLog(this->uroboros, L"Failed to create directory ", getCCName(this->inner->path.value));
      }
   }
}


void C_CreateDirectory_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      if (!this->inner->isfile.value && !this->inner->isdirectory.value) {
         commandLog(this->uroboros, L"Failed to create directory ", getCCName(this->inner->path.value));
         this->inner->success.value = false;
         return;
      }

      _str path = this->inner->path.value;
      _bool nameChanged = false;

      if (this->inner->exists.value) {
         nameChanged = true;
         path = os_stackPath(path);
      }

      const _bool s = os_createDirectory(path);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create directory ", getCCName(path));
         if (nameChanged) {
            this->inner->this_s.value = path;
         }
         this->attribute->run();
      }
      else {
         commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
      }
   }
}

// others


void C_Create_String::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = this->inner->location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to create ", getCCNameShort(value));
         this->inner->success.value = false;
         return;
      }

      const _str path = os_join(dest, value);

      if (os_exists(path)) {
         if (!(forced && os_drop(path, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to create ", getCCName(path));
            this->inner->success.value = false;
            return;
         }
      }

      if (os_hasExtension(value)) {
         const _bool s = os_createFile(path);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create file ", getCCName(path));
         }
         else {
            commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
         }
      }
      else {
         const _bool s = os_createDirectory(path);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create directory ", getCCName(path));
         }
         else {
            commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
         }
      }
   }
}


void C_CreateFile_String::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = this->inner->location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to create file ", getCCNameShort(value));
         this->inner->success.value = false;
         return;
      }

      const _str path = os_join(dest, value);

      if (os_exists(path)) {
         if (!(forced && os_drop(path, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
            this->inner->success.value = false;
            return;
         }
      }

      const _bool s = os_createFile(path);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create file ", getCCName(path));
      }
      else {
         commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
      }
   }
}


void C_CreateDirectory_String::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = this->inner->location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to create directory ", getCCNameShort(value));
         this->inner->success.value = false;
         return;
      }

      const _str path = os_join(dest, value);

      if (os_exists(path)) {
         if (!(forced && os_drop(path, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
            this->inner->success.value = false;
            return;
         }
      }

      const _bool s = os_createDirectory(path);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create directory ", getCCName(path));
      }
      else {
         commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
      }
   }
}


void C_Create_String_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = this->inner->location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to create ", getCCNameShort(value));
         this->inner->success.value = false;
         return;
      }

      _str path = os_join(dest, value);
      const _bool hasExt = os_hasExtension(value);

      if (os_exists(path)) {
         if (hasExt) {
            const _str ex = os_extension(path);
            const _str base = os_stackPathBase(path);
            path = os_stackPathExt(base, ex);
         }
         else {
            path = os_stackPath(path);
         }
      }

      if (hasExt) {
         const _bool s = os_createFile(path);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create file ", getCCName(path));
         }
         else {
            commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
         }
      }
      else {
         const _bool s = os_createDirectory(path);
         this->inner->success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Create directory ", getCCName(path));
         }
         else {
            commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
         }
      }
   }
}


void C_CreateFile_String_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = this->inner->location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to create file ", getCCNameShort(value));
         this->inner->success.value = false;
         return;
      }

      _str path = os_join(dest, value);
      const _bool hasExt = os_hasExtension(value);

      if (os_exists(path)) {
         if (hasExt) {
            const _str ex = os_extension(path);
            const _str base = os_stackPathBase(path);
            path = os_stackPathExt(base, ex);
         }
         else {
            path = os_stackPath(path);
         }
      }

      const _bool s = os_createFile(path);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create file ", getCCName(path));
      }
      else {
         commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
      }
   }
}


void C_CreateDirectory_String_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = this->inner->location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to create directory ", getCCNameShort(value));
         this->inner->success.value = false;
         return;
      }

      _str path = os_join(dest, value);

      if (os_exists(path)) {
         path = os_stackPath(path);
      }

      const _bool s = os_createDirectory(path);
      this->inner->success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Create directory ", getCCName(path));
      }
      else {
         commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
      }
   }
}

// others multi:

void C_Create_List::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str& dest = this->inner->location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner->success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to create ", getCCNameShort(names[i]));
         }
         this->inner->success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(this->uroboros, L"Failed to create ", getCCNameShort(n));
            success = false;
         }
         else {
            const _str path = os_join(dest, n);

            if (os_exists(path)) {
               if (!(forced && os_drop(path, this->uroboros))) {
                  commandLog(this->uroboros, L"Failed to create ", getCCName(path));
                  success = false;
                  continue;
               }
            }

            if (os_hasExtension(n)) {
               const _bool s = os_createFile(path);
               this->inner->success.value = s;

               if (s) {
                  commandLog(this->uroboros, L"Create file ", getCCName(path));
               }
               else {
                  commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
                  success = false;
               }
            }
            else {
               const _bool s = os_createDirectory(path);
               this->inner->success.value = s;

               if (s) {
                  commandLog(this->uroboros, L"Create directory ", getCCName(path));
               }
               else {
                  commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
                  success = false;
               }
            }
         }
      }

      this->inner->success.value = success;
   }
}


void C_CreateFiles_List::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str& dest = this->inner->location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner->success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to create file ", getCCNameShort(names[i]));
         }
         this->inner->success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(this->uroboros, L"Failed to create file ", getCCNameShort(n));
            success = false;
         }
         else {
            const _str path = os_join(dest, n);

            if (os_exists(path)) {
               if (!(forced && os_drop(path, this->uroboros))) {
                  commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
                  success = false;
                  continue;
               }
            }

            const _bool s = os_createFile(path);
            this->inner->success.value = s;

            if (s) {
               commandLog(this->uroboros, L"Create file ", getCCName(path));
            }
            else {
               commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
               success = false;
            }
         }
      }

      this->inner->success.value = success;
   }
}


void C_CreateDirectories_List::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str& dest = this->inner->location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner->success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to directory ", getCCNameShort(names[i]));
         }
         this->inner->success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(this->uroboros, L"Failed to create directory ", getCCNameShort(n));
            success = false;
         }
         else {
            const _str path = os_join(dest, n);

            if (os_exists(path)) {
               if (!(forced && os_drop(path, this->uroboros))) {
                  commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
                  success = false;
                  continue;
               }
            }

            const _bool s = os_createDirectory(path);
            this->inner->success.value = s;

            if (s) {
               commandLog(this->uroboros, L"Create directory ", getCCName(path));
            }
            else {
               commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
               success = false;
            }
         }
      }

      this->inner->success.value = success;
   }
}


void C_Create_List_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str& dest = this->inner->location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner->success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to create ", getCCNameShort(names[i]));
         }
         this->inner->success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(this->uroboros, L"Failed to create ", getCCNameShort(n));
            success = false;
         }
         else {
            _str path = os_join(dest, n);
            const _bool hasExt = os_hasExtension(path);

            if (os_exists(path)) {
               if (hasExt) {
                  const _str ext = os_extension(path);
                  const _str base = os_stackPathBase(path);
                  path = os_stackPathExt(base, ext);
               }
               else {
                  path = os_stackPath(path);
               }
            }

            if (hasExt) {
               const _bool s = os_createFile(path);
               this->inner->success.value = s;

               if (s) {
                  commandLog(this->uroboros, L"Create file ", getCCName(path));
               }
               else {
                  commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
                  success = false;
               }
            }
            else {
               const _bool s = os_createDirectory(path);
               this->inner->success.value = s;

               if (s) {
                  commandLog(this->uroboros, L"Create directory ", getCCName(path));
               }
               else {
                  commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
                  success = false;
               }
            }
         }
      }

      this->inner->success.value = success;
   }
}


void C_CreateFiles_List_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str& dest = this->inner->location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner->success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to create file ", getCCNameShort(names[i]));
         }
         this->inner->success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(this->uroboros, L"Failed to create file ", getCCNameShort(n));
            success = false;
         }
         else {
            _str path = os_join(dest, n);
            const _bool hasExt = os_hasExtension(path);

            if (os_exists(path)) {
               if (hasExt) {
                  const _str ext = os_extension(path);
                  const _str base = os_stackPathBase(path);
                  path = os_stackPathExt(base, ext);
               }
               else {
                  path = os_stackPath(path);
               }
            }

            const _bool s = os_createFile(path);
            this->inner->success.value = s;

            if (s) {
               commandLog(this->uroboros, L"Create file ", getCCName(path));
            }
            else {
               commandLog(this->uroboros, L"Failed to create file ", getCCName(path));
               success = false;
            }
         }
      }

      this->inner->success.value = success;
   }
}


void C_CreateDirectories_List_Stack::run()
{
   if (this->uroboros->state == State::s_Running) {
      const _str& dest = this->inner->location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner->success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to directory ", getCCNameShort(names[i]));
         }
         this->inner->success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(this->uroboros, L"Failed to create directory ", getCCNameShort(n));
            success = false;
         }
         else {
            _str path = os_join(dest, n);

            if (os_exists(path)) {
               path = os_stackPath(path);
            }

            const _bool s = os_createDirectory(path);
            this->inner->success.value = s;

            if (s) {
               commandLog(this->uroboros, L"Create directory ", getCCName(path));
            }
            else {
               commandLog(this->uroboros, L"Failed to create directory ", getCCName(path));
               success = false;
            }
         }
      }

      this->inner->success.value = success;
   }
}

}