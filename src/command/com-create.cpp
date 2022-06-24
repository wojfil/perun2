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
#include "../os.h"


void C_Create::run()
{
   if (g_running) {
      if (!g_isfile.value && !g_isdirectory.value) {
         commandLog(L"Failed to create " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      if (g_exists.value) {
         if (!(forced && os_drop(g_path.value, g_isfile.value))) {
            commandLog(L"Failed to create " + getCCName(g_path.value));
            g_success.value = false;
            return;
         }
      }

      if (os_hasExtension(g_path.value)) {
         const _boo s = os_createFile(g_path.value);
         g_success.value = s;

         if (s) {
            commandLog(L"Create file " + getCCName(g_path.value));
            os_loadAttributes(*attribute);
         }
         else {
            commandLog(L"Failed to create file " + getCCName(g_path.value));
         }
      }
      else {
         const _boo s = os_createDirectory(g_path.value);
         g_success.value = s;

         if (s) {
            commandLog(L"Create directory " + getCCName(g_path.value));
            os_loadAttributes(*attribute);
         }
         else {
            commandLog(L"Failed to create directory " + getCCName(g_path.value));
         }
      }
   }
}


void C_Create_Stack::run()
{
   if (g_running) {
      if (!g_isfile.value && !g_isdirectory.value) {
         commandLog(L"Failed to create " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      _str path = g_path.value;
      const _boo hasExt = os_hasExtension(path);
      _boo nameChanged = false;

      if (g_exists.value) {
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
         const _boo s = os_createFile(path);
         g_success.value = s;

         if (s) {
            commandLog(L"Create file " + getCCName(path));
            if (nameChanged) {
               g_this_s.value = path;
            }
            os_loadAttributes(*attribute);
         }
         else {
            commandLog(L"Failed to create file " + getCCName(path));
         }
      }
      else {
         const _boo s = os_createDirectory(path);
         g_success.value = s;

         if (s) {
            commandLog(L"Create directory " + getCCName(path));
            if (nameChanged) {
               g_this_s.value = path;
            }
            os_loadAttributes(*attribute);
         }
         else {
            commandLog(L"Failed to create directory " + getCCName(path));
         }
      }
   }
}


void C_CreateFile::run()
{
   if (g_running) {
      if (!g_isfile.value && !g_isdirectory.value) {
         commandLog(L"Failed to create file " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      if (g_exists.value) {
         if (!(forced && os_drop(g_path.value, g_isfile.value))) {
            commandLog(L"Failed to create file " + getCCName(g_path.value));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_createFile(g_path.value);
      g_success.value = s;

      if (s) {
         commandLog(L"Create file " + getCCName(g_path.value));
         os_loadAttributes(*attribute);
      }
      else {
         commandLog(L"Failed to create file " + getCCName(g_path.value));
      }
   }
}


void C_CreateFile_Stack::run()
{
   if (g_running) {
      if (!g_isfile.value && !g_isdirectory.value) {
         commandLog(L"Failed to create file " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      _str path = g_path.value;
      const _boo hasExt = os_hasExtension(path);
      _boo nameChanged = false;

      if (g_exists.value) {
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

      const _boo s = os_createFile(path);
      g_success.value = s;

      if (s) {
         commandLog(L"Create file " + getCCName(path));
         if (nameChanged) {
            g_this_s.value = path;
         }
         os_loadAttributes(*attribute);
      }
      else {
         commandLog(L"Failed to create file " + getCCName(path));
      }
   }
}


void C_CreateDirectory::run()
{
   if (g_running) {
      if (!g_isfile.value && !g_isdirectory.value) {
         commandLog(L"Failed to create directory " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      if (g_exists.value) {
         if (!(forced && os_drop(g_path.value, g_isfile.value))) {
            commandLog(L"Failed to create directory " + getCCName(g_path.value));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_createDirectory(g_path.value);
      g_success.value = s;

      if (s) {
         commandLog(L"Create directory " + getCCName(g_path.value));
         os_loadAttributes(*attribute);
      }
      else {
         commandLog(L"Failed to create directory " + getCCName(g_path.value));
      }
   }
}


void C_CreateDirectory_Stack::run()
{
   if (g_running) {
      if (!g_isfile.value && !g_isdirectory.value) {
         commandLog(L"Failed to create directory " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      _str path = g_path.value;
      _boo nameChanged = false;

      if (g_exists.value) {
         nameChanged = true;
         path = os_stackPath(path);
      }

      const _boo s = os_createDirectory(path);
      g_success.value = s;

      if (s) {
         commandLog(L"Create directory " + getCCName(path));
         if (nameChanged) {
            g_this_s.value = path;
         }
         os_loadAttributes(*attribute);
      }
      else {
         commandLog(L"Failed to create directory " + getCCName(path));
      }
   }
}

// others


void C_Create_String::run()
{
   if (g_running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(L"Failed to create " + getCCNameShort(value));
         g_success.value = false;
         return;
      }

      const _str path = os_join(dest, value);

      if (os_exists(path)) {
         if (!(forced && os_drop(path))) {
            commandLog(L"Failed to create " + getCCName(path));
            g_success.value = false;
            return;
         }
      }

      if (os_hasExtension(value)) {
         const _boo s = os_createFile(path);
         g_success.value = s;

         if (s) {
            commandLog(L"Create file " + getCCName(path));
         }
         else {
            commandLog(L"Failed to create file " + getCCName(path));
         }
      }
      else {
         const _boo s = os_createDirectory(path);
         g_success.value = s;

         if (s) {
            commandLog(L"Create directory " + getCCName(path));
         }
         else {
            commandLog(L"Failed to create directory " + getCCName(path));
         }
      }
   }
}


void C_CreateFile_String::run()
{
   if (g_running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(L"Failed to create file " + getCCNameShort(value));
         g_success.value = false;
         return;
      }

      const _str path = os_join(dest, value);

      if (os_exists(path)) {
         if (!(forced && os_drop(path))) {
            commandLog(L"Failed to create file " + getCCName(path));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_createFile(path);
      g_success.value = s;

      if (s) {
         commandLog(L"Create file " + getCCName(path));
      }
      else {
         commandLog(L"Failed to create file " + getCCName(path));
      }
   }
}


void C_CreateDirectory_String::run()
{
   if (g_running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(L"Failed to create directory " + getCCNameShort(value));
         g_success.value = false;
         return;
      }

      const _str path = os_join(dest, value);

      if (os_exists(path)) {
         if (!(forced && os_drop(path))) {
            commandLog(L"Failed to create directory " + getCCName(path));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_createDirectory(path);
      g_success.value = s;

      if (s) {
         commandLog(L"Create directory " + getCCName(path));
      }
      else {
         commandLog(L"Failed to create directory " + getCCName(path));
      }
   }
}


void C_Create_String_Stack::run()
{
   if (g_running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(L"Failed to create " + getCCNameShort(value));
         g_success.value = false;
         return;
      }

      _str path = os_join(dest, value);
      const _boo hasExt = os_hasExtension(value);

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
         const _boo s = os_createFile(path);
         g_success.value = s;

         if (s) {
            commandLog(L"Create file " + getCCName(path));
         }
         else {
            commandLog(L"Failed to create file " + getCCName(path));
         }
      }
      else {
         const _boo s = os_createDirectory(path);
         g_success.value = s;

         if (s) {
            commandLog(L"Create directory " + getCCName(path));
         }
         else {
            commandLog(L"Failed to create directory " + getCCName(path));
         }
      }
   }
}


void C_CreateFile_String_Stack::run()
{
   if (g_running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(L"Failed to create file " + getCCNameShort(value));
         g_success.value = false;
         return;
      }

      _str path = os_join(dest, value);
      const _boo hasExt = os_hasExtension(value);

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

      const _boo s = os_createFile(path);
      g_success.value = s;

      if (s) {
         commandLog(L"Create file " + getCCName(path));
      }
      else {
         commandLog(L"Failed to create file " + getCCName(path));
      }
   }
}


void C_CreateDirectory_String_Stack::run()
{
   if (g_running) {
      const _str value = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (os_isInvaild(value) || !os_directoryExists(dest)) {
         commandLog(L"Failed to create directory " + getCCNameShort(value));
         g_success.value = false;
         return;
      }

      _str path = os_join(dest, value);

      if (os_exists(path)) {
         path = os_stackPath(path);
      }

      const _boo s = os_createDirectory(path);
      g_success.value = s;

      if (s) {
         commandLog(L"Create directory " + getCCName(path));
      }
      else {
         commandLog(L"Failed to create directory " + getCCName(path));
      }
   }
}

// others multi:

void C_Create_List::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(L"Failed to create " + getCCNameShort(names[i]));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(L"Failed to create " + getCCNameShort(n));
            success = false;
         }
         else {
            const _str path = os_join(dest, n);

            if (os_exists(path)) {
               if (!(forced && os_drop(path))) {
                  commandLog(L"Failed to create " + getCCName(path));
                  success = false;
                  continue;
               }
            }

            if (os_hasExtension(n)) {
               const _boo s = os_createFile(path);
               g_success.value = s;

               if (s) {
                  commandLog(L"Create file " + getCCName(path));
               }
               else {
                  commandLog(L"Failed to create file " + getCCName(path));
                  success = false;
               }
            }
            else {
               const _boo s = os_createDirectory(path);
               g_success.value = s;

               if (s) {
                  commandLog(L"Create directory " + getCCName(path));
               }
               else {
                  commandLog(L"Failed to create directory " + getCCName(path));
                  success = false;
               }
            }
         }
      }

      g_success.value = success;
   }
}


void C_CreateFiles_List::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(L"Failed to create file " + getCCNameShort(names[i]));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(L"Failed to create file " + getCCNameShort(n));
            success = false;
         }
         else {
            const _str path = os_join(dest, n);

            if (os_exists(path)) {
               if (!(forced && os_drop(path))) {
                  commandLog(L"Failed to create file " + getCCName(path));
                  success = false;
                  continue;
               }
            }

            const _boo s = os_createFile(path);
            g_success.value = s;

            if (s) {
               commandLog(L"Create file " + getCCName(path));
            }
            else {
               commandLog(L"Failed to create file " + getCCName(path));
               success = false;
            }
         }
      }

      g_success.value = success;
   }
}


void C_CreateDirectories_List::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(L"Failed to directory " + getCCNameShort(names[i]));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(L"Failed to create directory " + getCCNameShort(n));
            success = false;
         }
         else {
            const _str path = os_join(dest, n);

            if (os_exists(path)) {
               if (!(forced && os_drop(path))) {
                  commandLog(L"Failed to create directory " + getCCName(path));
                  success = false;
                  continue;
               }
            }

            const _boo s = os_createDirectory(path);
            g_success.value = s;

            if (s) {
               commandLog(L"Create directory " + getCCName(path));
            }
            else {
               commandLog(L"Failed to create directory " + getCCName(path));
               success = false;
            }
         }
      }

      g_success.value = success;
   }
}


void C_Create_List_Stack::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(L"Failed to create " + getCCNameShort(names[i]));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(L"Failed to create " + getCCNameShort(n));
            success = false;
         }
         else {
            _str path = os_join(dest, n);
            const _boo hasExt = os_hasExtension(path);

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
               const _boo s = os_createFile(path);
               g_success.value = s;

               if (s) {
                  commandLog(L"Create file " + getCCName(path));
               }
               else {
                  commandLog(L"Failed to create file " + getCCName(path));
                  success = false;
               }
            }
            else {
               const _boo s = os_createDirectory(path);
               g_success.value = s;

               if (s) {
                  commandLog(L"Create directory " + getCCName(path));
               }
               else {
                  commandLog(L"Failed to create directory " + getCCName(path));
                  success = false;
               }
            }
         }
      }

      g_success.value = success;
   }
}


void C_CreateFiles_List_Stack::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(L"Failed to create file " + getCCNameShort(names[i]));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(L"Failed to create file " + getCCNameShort(n));
            success = false;
         }
         else {
            _str path = os_join(dest, n);
            const _boo hasExt = os_hasExtension(path);

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

            const _boo s = os_createFile(path);
            g_success.value = s;

            if (s) {
               commandLog(L"Create file " + getCCName(path));
            }
            else {
               commandLog(L"Failed to create file " + getCCName(path));
               success = false;
            }
         }
      }

      g_success.value = success;
   }
}


void C_CreateDirectories_List_Stack::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            commandLog(L"Failed to directory " + getCCNameShort(names[i]));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);

         if (os_isInvaild(n)) {
            commandLog(L"Failed to create directory " + getCCNameShort(n));
            success = false;
         }
         else {
            _str path = os_join(dest, n);

            if (os_exists(path)) {
               path = os_stackPath(path);
            }

            const _boo s = os_createDirectory(path);
            g_success.value = s;

            if (s) {
               commandLog(L"Create directory " + getCCName(path));
            }
            else {
               commandLog(L"Failed to create directory " + getCCName(path));
               success = false;
            }
         }
      }

      g_success.value = success;
   }
}
