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

#include "com-core.h"
#include "../uroboros.h"
#include "../os.h"


////
//   simple commands:
////

void C_Delete::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_delete(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Delete " + getCCName(g_path.value));
         if (hasAttribute) {
            os_loadAttributes(*attribute);
         }
      }
      else {
         print(L"Failed to delete " + getCCName(g_path.value));
      }
   }
}

void C_Drop::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_drop(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Drop " + getCCName(g_path.value));
         if (hasAttribute) {
            os_loadAttributes(*attribute);
         }
      }
      else {
         print(L"Failed to drop " + getCCName(g_path.value));
      }
   }
}

void C_Hide::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_hide(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Hide " + getCCName(g_path.value));
         if (saveChanges) {
            g_hidden.value = true;
         }
      }
      else {
         print(L"Failed to hide " + getCCName(g_path.value));
      }
   }
}

void C_Lock::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_lock(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Lock " + getCCName(g_path.value));
         if (saveChanges) {
            g_readonly.value = true;
         }
      }
      else {
         print(L"Failed to lock " + getCCName(g_path.value));
      }
   }
}

void C_Open::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_open(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Open " + getCCName(g_path.value));
      }
      else {
         print(L"Failed to open " + getCCName(g_path.value));
      }
   }
}

void C_Unlock::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_unlock(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Unlock " + getCCName(g_path.value));
         if (saveChanges) {
            g_readonly.value = false;
         }
      }
      else {
         print(L"Failed to unlock " + getCCName(g_path.value));
      }
   }
}

void C_Unhide::run()
{
   if (g_running) {
      const _boo s = g_exists.value && os_unhide(g_path.value);
      g_success.value = s;

      if (s) {
         print(L"Unhide " + getCCName(g_path.value));
         if (saveChanges) {
            g_hidden.value = false;
         }
      }
      else {
         print(L"Failed to unhide " + getCCName(g_path.value));
      }
   }
}

void C_OpenWith::run()
{
   if (g_running) {
      const _str pro = os_trim(program->getValue());

      if (!g_exists.value || pro.empty()) {
         print(L"Failed to open " + getCCName(g_path.value) + L" with " + getCCNameShort(pro));
         g_success.value = false;
         return;
      }

      const _str proPath = os_join(g_location.value, pro);
      if (os_exists(proPath)) {
         const _boo s = os_openWith(proPath, os_quoteEmbraced(g_path.value));
         g_success.value = s;

         if (s) {
            print(L"Open " + getCCName(g_path.value) + L" with " + getCCName(proPath));
         }
         else {
            print(L"Failed to open " + getCCName(g_path.value) + L" with " + getCCName(proPath));
         }
      }
      else {
         const _str com = pro + L" " + os_quoteEmbraced(g_path.value);
         const _boo s = os_openAsCommand(com);
         g_success.value = s;

         if (s) {
            print(L"Open " + getCCName(g_path.value) + L" with '" + pro + L"'");
         }
         else {
            print(L"Failed to open " + getCCName(g_path.value) + L" with '" + pro + L"'");
         }
      }
   }
}

void C_ReaccessTo::run()
{
   if (g_running) {
      _tim t = g_access.value;
      t.setValue(time->getValue());
      const _boo s = g_exists.value && os_reaccessTo(g_path.value, t);
      g_success.value = s;

      if (s) {
         print(L"Reaccess " + getCCName(g_path.value) +
            L" to " + t.toString());

         if (saveChanges) {
            g_access.value = t;
         }
      }
      else {
         print(L"Failed to reaccess " + getCCName(g_path.value));
      }
   }
}

void C_RechangeTo::run()
{
   if (g_running) {
      _tim t = g_change.value;
      t.setValue(time->getValue());
      const _boo s = g_exists.value && os_rechangeTo(g_path.value, t);
      g_success.value = s;

      if (s) {
         print(L"Rechange " + getCCName(g_path.value) +
            L" to " + t.toString());

         if (saveChanges) {
            g_change.value = t;
         }
      }
      else {
         print(L"Failed to rechange " + getCCName(g_path.value));
      }
   }
}

void C_RecreateTo::run()
{
   if (g_running) {
      _tim t = g_creation.value;
      t.setValue(time->getValue());
      const _boo s = g_exists.value && os_recreateTo(g_path.value, t);
      g_success.value = s;

      if (s) {
         print(L"Recreate " + getCCName(g_path.value) +
            L" to " + t.toString());

         if (saveChanges) {
            g_creation.value = t;
         }
      }
      else {
         print(L"Failed to recreate " + getCCName(g_path.value));
      }
   }
}

void C_RemodifyTo::run()
{
   if (g_running) {
      _tim t = g_modification.value;
      t.setValue(time->getValue());
      const _boo s = g_exists.value && os_remodifyTo(g_path.value, t);
      g_success.value = s;

      if (s) {
         print(L"Remodify " + getCCName(g_path.value) +
            L" to " + t.toString());

         if (saveChanges) {
            g_modification.value = t;
         }
      }
      else {
         print(L"Failed to remodify " + getCCName(g_path.value));
      }
   }
}

void C_RenameTo::run()
{
   if (g_running) {
      _str n = os_fullname(os_trim(name->getValue()));

      if (!g_exists.value || n.empty() || os_isInvaild(n)
          || !os_hasParentDirectory(g_path.value) || os_isAbsolute(n)) {

         print(L"Failed to rename " + getCCName(g_path.value));
         g_success.value = false;
         return;
      }

      const _boo hasExt = g_isfile.value && os_hasExtension(g_path.value);

      if (!extensionless && hasExt && !os_hasExtension(n)) {
         n += L"." + os_extension(g_path.value);
      }

      const _str base = os_parent(g_path.value);
      const _str newPath = base + OS_SEPARATOR + n;

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to rename " + getCCName(g_path.value));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_moveTo(g_path.value, newPath);
      g_success.value = s;

      if (s) {
         print(L"Rename " + getCCName(g_path.value) + L" to '" + n + L"'");

         if (saveChanges) {
            g_fullname.value = n;
            g_name.value = hasExt ? os_name(newPath) : n;
            g_path.value = newPath;
            g_this_s.value = newPath;
         }
      }
      else {
         print(L"Failed to rename " + getCCName(g_path.value));
      }
   }
}

void C_RenameTo_Stack::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str n = os_fullname(os_trim(name->getValue()));
      if (!g_exists.value || n.empty() || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath) || os_isAbsolute(n)) {

         print(L"Failed to rename " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);
      const _str parent = os_parent(oldPath);
      _str newPath;
      _boo s;

      if (hasExt) {
         _str ex;
         if (os_hasExtension(n)) {
            ex = os_extension(n);
         }
         else {
            ex = os_extension(oldPath);
            if (!extensionless) {
               n += L"." + ex;
            }
         }

         newPath = parent + OS_SEPARATOR + n;

         if (os_exists(newPath)) {
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ex);
         }

         s = os_moveTo(oldPath, newPath);
         n = os_fullname(newPath);

         if (s && saveChanges) {
            g_extension.value = ex;
         }
      }
      else {
         newPath = parent + OS_SEPARATOR + n;
         const _str newPath = os_stackPath(newPath);
         s = os_moveTo(oldPath, newPath);
      }

      g_success.value = s;

      if (s) {
         print(L"Rename " + getCCName(g_path.value) + L" to '" + n + L"'");

         if (saveChanges) {
            g_fullname.value = n;
            g_name.value = hasExt ? os_name(n) : n;
            g_path.value = newPath;
            g_this_s.value = newPath;
         }
      }
      else {
         print(L"Failed to rename " + getCCName(g_path.value));
      }
   }
}

void C_MoveTo::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str n = os_trim(location->getValue());

      if (!g_exists.value || n.empty() || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath)) {

         print(L"Failed to move " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _str newLoc = os_join(g_location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to move " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      const _str newPath = newLoc + OS_SEPARATOR + fulln;
      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to move " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_moveTo(oldPath, newPath);
      g_success.value = s;

      if (s) {
         print(L"Move " + getCCName(oldPath) + L" to " + getCCName(newLoc));

         if (hasAttribute) {
            g_this_s.value = newPath;
            os_loadAttributes(*attribute);
         }
      }
      else {
         print(L"Failed to move " + getCCName(oldPath));
      }
   }
}

void C_MoveTo_Stack::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str n = os_trim(location->getValue());

      if (!g_exists.value || n.empty() || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath)) {

         print(L"Failed to move " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      _str newLoc = os_join(g_location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to move " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      _str newPath = newLoc + OS_SEPARATOR + fulln;
      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (hasExt) {
            const _str ext = os_extension(newPath);
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ext);
         }
         else {
            newPath = os_stackPath(newPath);
         }
      }

      const _boo s = os_moveTo(oldPath, newPath);
      g_success.value = s;

      if (s) {
         print(L"Move " + getCCName(oldPath) + L" to " + getCCName(newLoc));

         if (hasAttribute) {
            g_this_s.value = newPath;
            os_loadAttributes(*attribute);
         }
      }
      else {
         print(L"Failed to move " + getCCName(oldPath));
      }
   }
}

void C_MoveToAs::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!g_exists.value || fulln.empty() || os_isInvaild(fulln)
           || os_isInvaild(loc) || loc.empty() || !os_hasParentDirectory(oldPath)) {

         print(L"Failed to move " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);

      if (!extensionless && hasExt && !os_hasExtension(fulln)) {
         fulln += L"." + os_extension(oldPath);
      }

      const _str newLoc = os_join(g_location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to move " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      _str newPath = newLoc + OS_SEPARATOR + fulln;

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to move " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_moveTo(oldPath, newPath);
      g_success.value = s;

      if (s) {
         print(L"Move " + getCCName(oldPath) + L" to " + getCCName(newLoc) + L" as '" + fulln + L"'");

         if (hasAttribute) {
            g_this_s.value = newPath;
            os_loadAttributes(*attribute);
         }
      }
      else {
         print(L"Failed to move " + getCCName(oldPath));
      }
   }
}

void C_MoveToAs_Stack::run()
{
   if (g_running) {
      const _str& oldPath = g_path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!g_exists.value || fulln.empty() || os_isInvaild(fulln)
          || loc.empty() || os_isInvaild(loc) || !os_hasParentDirectory(oldPath)) {

         print(L"Failed to move " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _str newLoc = os_join(g_location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to move " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);
      _str newPath;
      _boo s;

      if (hasExt) {
         _str ex;
         if (!os_hasExtension(fulln)) {
            ex = os_extension(fulln);
         }
         else {
            ex = os_extension(oldPath);
            if (!extensionless) {
               fulln += L"." + ex;
            }
         }

         newPath = newLoc + OS_SEPARATOR + fulln;
         if (os_exists(newPath)) {
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ex);
            fulln = os_fullname(newPath);
         }

         s = os_moveTo(oldPath, newPath);
      }
      else {
         newPath = newLoc + OS_SEPARATOR + fulln;
         if (os_exists(newPath)) {
            newPath = os_stackPath(newPath);
            fulln = os_fullname(newPath);
         }
         s = os_moveTo(oldPath, newPath);
      }

      g_success.value = s;

      if (s) {
         print(L"Move " + getCCName(oldPath) + L" to " + getCCName(newLoc) + L" as '" + fulln + L"'");

         if (hasAttribute) {
            g_this_s.value = newPath;
            os_loadAttributes(*attribute);
         }
      }
      else {
         print(L"Failed to move " + getCCName(oldPath));
      }
   }
}

void C_DownloadFrom_String::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _str name = os_trim(element->getValue());
      const _str src = os_trim(location->getValue());

      if (name.empty() || src.empty() || os_isInvaild(name)
           || os_isInvaild(src) || !os_directoryExists(dest)) {

         print(L"Failed to download " + getCCName(name));
         g_success.value = false;
         return;
      }

      const _str srcLoc = os_join(dest, src);
      const _str oldPath = srcLoc + OS_SEPARATOR + name;

      if (!os_exists(oldPath)) {
         print(L"Failed to download " + getCCName(name));
         g_success.value = false;
         return;
      }

      const _str newPath = dest + OS_SEPARATOR + name;

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to download " + getCCName(name));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));
      g_success.value = s;

      if (s) {
         print(L"Download " + getCCName(name) + L" from " + getCCName(srcLoc));
      }
      else {
         print(L"Failed to download " + getCCName(name));
      }
   }
}

void C_DownloadFrom_List::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (src.empty() || os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         for (_size i = 0; i < len; i++) {
            print(L"Failed to download " + getCCNameShort(os_trim(names[i])));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str& n = os_trim(names[i]);
         const _str oldPath = srcLoc + OS_SEPARATOR + n;

         if (n.empty() || os_isInvaild(n) || !os_exists(oldPath) || os_isAbsolute(n)) {
            print(L"Failed to download " + getCCName(oldPath));
            success = false;
         }
         else {
            const _str newPath = dest + OS_SEPARATOR + n;

            if (os_exists(newPath)) {
               if (!(forced && os_drop(newPath))) {
                  print(L"Failed to download " + getCCName(oldPath));
                  success = false;
                  continue;
               }
            }

            const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));

            if (s) {
               print(L"Download " + getCCName(oldPath) + L" from " + getCCName(srcLoc));
            }
            else {
               print(L"Failed to download " + getCCName(oldPath));
               success = false;
            }
         }
      }

      g_success.value = success;
   }
}

void C_DownloadFrom_Definition::run()
{
   if (g_running) {
      const _str dest = g_location.value;
      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(g_location.value, src);

      if (src.empty() || os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         print(L"Failed to download");
         g_success.value = false;
         return;
      }

      _boo success = true;
      g_location.value = srcLoc;

      while (g_running && elements->hasNext()) {
         const _str n = os_trim(elements->getValue());
         const _str oldPath = srcLoc + OS_SEPARATOR + n;

         if (n.empty() || os_isInvaild(n) || !os_exists(oldPath) || os_isAbsolute(n)) {
            print(L"Failed to download " + getCCNameShort(n));
            success = false;
         }
         else {
            const _str newPath = dest + OS_SEPARATOR + n;

            if (os_exists(newPath)) {
               if (!(forced && os_drop(newPath))) {
                  print(L"Failed to download " + getCCName(oldPath));
                  success = false;
                  continue;
               }
            }

            const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));

            if (s) {
               print(L"Download " + getCCName(oldPath) + L" from " + getCCName(srcLoc));
            }
            else {
               print(L"Failed to download " + getCCName(oldPath));
               success = false;
            }
         }
      }

      g_success.value = success;
      g_location.value = dest;
   }
}

void C_DownloadFrom_String_Stack::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _str name = os_trim(element->getValue());
      const _str src = os_trim(location->getValue());

      if (name.empty() || src.empty() || os_isInvaild(name)
          || os_isInvaild(src) || !os_directoryExists(dest)) {

         print(L"Failed to download " + getCCNameShort(name));
         g_success.value = false;
         return;
      }

      const _str srcLoc = os_join(dest, src);
      const _str oldPath = srcLoc + OS_SEPARATOR + name;

      if (!os_exists(oldPath)) {
         print(L"Failed to download " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      _str newPath = dest + OS_SEPARATOR + name;

      if (os_exists(newPath)) {
         if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
            const _str ext = os_extension(newPath);
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ext);
         }
         else {
            newPath = os_stackPath(newPath);
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));
      g_success.value = s;

      if (s) {
         print(L"Download " + getCCName(oldPath) + L" from " + getCCName(srcLoc));
      }
      else {
         print(L"Failed to download " + getCCName(oldPath));
      }
   }
}

void C_DownloadFrom_List_Stack::run()
{
   if (g_running) {
      const _str& dest = g_location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         g_success.value = true;
         return;
      }

      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (src.empty() || os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         for (_size i = 0; i < len; i++) {
            print(L"Failed to download " + getCCNameShort(os_trim(names[i])));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);
         const _str oldPath = srcLoc + OS_SEPARATOR + n;

         if (n.empty() || os_isInvaild(n) || os_isAbsolute(n) || !os_exists(oldPath)) {
            print(L"Failed to download " + getCCNameShort(n));
            success = false;
         }
         else {
            _str newPath = dest + OS_SEPARATOR + n;

            if (os_exists(newPath)) {
               if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
                  const _str ext = os_extension(newPath);
                  const _str base = os_stackPathBase(newPath);
                  newPath = os_stackPathExt(base, ext);
               }
               else {
                  newPath = os_stackPath(newPath);
               }
            }

            const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));

            if (s) {
               print(L"Download " + getCCName(oldPath) + L" from " + getCCName(srcLoc));
            }
            else {
               print(L"Failed to download " + getCCName(oldPath));
               success = false;
            }
         }
      }

      g_success.value = success;
   }
}

void C_DownloadFrom_Definition_Stack::run()
{
   if (g_running) {
      const _str dest = g_location.value;
      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (src.empty() || os_isInvaild(src) || !os_directoryExists(dest)
          || !os_directoryExists(srcLoc)) {

         print(L"Failed to download");
         g_success.value = false;
         return;
      }

      _boo success = true;
      const _str f = os_fullname(srcLoc);
      g_location.value = srcLoc;

      while (elements->hasNext()) {
         if (!g_running) {
            elements->reset();
            break;
         }
         const _str n = os_trim(elements->getValue());
         const _str oldPath = srcLoc + OS_SEPARATOR + n;

         if (n.empty() || os_isInvaild(n) || os_isAbsolute(n) || !os_exists(oldPath)) {
            print(L"Failed to download " + getCCNameShort(n));
            success = false;
         }
         else {
            _str newPath = dest + OS_SEPARATOR + n;

            if (os_exists(newPath)) {
               if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
                  const _str ext = os_extension(newPath);
                  const _str base = os_stackPathBase(newPath);
                  newPath = os_stackPathExt(base, ext);
               }
               else {
                  newPath = os_stackPath(newPath);

                  if (!g_running) {
                     elements->reset();
                     break;
                  }
               }
            }

            const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));

            if (s) {
               print(L"Download " + getCCName(oldPath) + L" from " + getCCName(srcLoc));
            }
            else {
               print(L"Failed to download " + getCCName(oldPath));
               success = false;
            }
         }
      }

      g_success.value = success;
      g_location.value = dest;
   }
}

void C_Download_String::run()
{
   if (g_running) {
      const _str oldElement = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (oldElement.empty() || os_isInvaild(oldElement) || !os_directoryExists(dest) || !os_isPath(oldElement))
      {
         print(L"Failed to download " + getCCNameShort(oldElement));
         g_success.value = false;
         return;
      }

      const _str oldPath = os_join(dest, oldElement);

      if (!os_exists(oldPath)) {
         print(L"Failed to download " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _str newPath = dest + OS_SEPARATOR + os_fullname(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to download " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));
      g_success.value = s;

      if (s) {
         print(L"Download " + getCCName(oldPath));
      }
      else {
         print(L"Failed to download " + getCCName(oldPath));
      }
   }
}

void C_Download_List::run()
{
   if (g_running) {
      _list oldPaths = elements->getValue();
      const _size len = oldPaths.size();

      if (len == 0) {
         g_success.value = true;
         return;
      }

      const _str& dest = g_location.value;

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            const _str p = os_trim(oldPaths[i]);
            print(L"Failed to download " + getCCNameShort(p));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; g_running && i < len; i++) {
         const _str n = os_trim(oldPaths[i]);
         const _str oldPath = os_join(dest, n);

         if (n.empty() || os_isInvaild(n) || !os_isPath(n) || !os_exists(oldPath)) {
            print(L"Failed to download " + getCCName(oldPath));
            success = false;
            continue;
         }

         const _str newPath = dest + OS_SEPARATOR + os_fullname(oldPath);

         if (os_exists(newPath)) {
            if (!(forced && os_drop(newPath))) {
               print(L"Failed to download " + getCCName(oldPath));
               success = false;
               continue;
            }
         }

         const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));

         if (s) {
            print(L"Download " + getCCName(oldPath));
         }
         else {
            print(L"Failed to download " + getCCName(oldPath));
            success = false;
         }
      }

      g_success.value = success;
   }
}

void C_Download_String_Stack::run()
{
   if (g_running) {
      const _str oldElement = os_trim(element->getValue());
      const _str& dest = g_location.value;

      if (oldElement.empty() || os_isInvaild(oldElement) || !os_isPath(oldElement) || !os_directoryExists(dest))
      {
         print(L"Failed to download " + getCCNameShort(oldElement));
         g_success.value = false;
         return;
      }

      const _str oldPath = os_join(dest, oldElement);

      if (!os_exists(oldPath)) {
         print(L"Failed to download " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      _str newPath = dest + OS_SEPARATOR + os_fullname(oldPath);

      if (os_exists(newPath)) {
         if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
            const _str ext = os_extension(newPath);
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ext);
         }
         else {
            newPath = os_stackPath(newPath);
         }
         if (!g_running) {
            return;
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));
      g_success.value = s;

      if (s) {
         print(L"Download " + getCCName(oldPath));
      }
      else {
         print(L"Failed to download " + getCCName(oldPath));
      }
   }
}

void C_Download_List_Stack::run()
{
   if (g_running) {
      _list oldPaths = elements->getValue();
      const _size len = oldPaths.size();

      if (len == 0) {
         g_success.value = true;
         return;
      }

      const _str& dest = g_location.value;

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            const _str p = os_trim(oldPaths[i]);
            print(L"Failed to download " + getCCNameShort(p));
         }
         g_success.value = false;
         return;
      }

      _boo success = true;

      for (_size i = 0; g_running && i < len; i++) {
         const _str oldPath = os_trim(oldPaths[i]);

         if (oldPath.empty() || os_isInvaild(oldPath) || !os_isPath(oldPath) || !os_exists(oldPath)) {
            print(L"Failed to download " + getCCName(oldPath));
            success = false;
            continue;
         }

         _str newPath = dest + OS_SEPARATOR + os_fullname(oldPath);

         if (os_exists(newPath)) {
            if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
               const _str ext = os_extension(newPath);
               const _str base = os_stackPathBase(newPath);
               newPath = os_stackPathExt(base, ext);
            }
            else {
               newPath = os_stackPath(newPath);
            }
            if (!g_running) {
               break;
            }
         }

         const _boo s = os_copyTo(oldPath, newPath, os_isFile(oldPath));

         if (s) {
            print(L"Download " + getCCName(oldPath));
         }
         else {
            print(L"Failed to download " + getCCName(oldPath));
            success = false;
         }
      }

      g_success.value = success;
   }
}

void C_CopyTo::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str n = os_trim(location->getValue());

      if (!g_exists.value || n.empty() || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         print(L"Failed to copy " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _str newLoc = os_join(g_location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to copy " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      const _str newPath = newLoc + OS_SEPARATOR + fulln;
      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to copy " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, g_isfile.value);
      g_success.value = s;

      if (s) {
         print(L"Copy " + getCCName(oldPath) + L" to " + getCCName(newLoc));
      }
      else {
         print(L"Failed to copy " + getCCName(oldPath));
      }
   }
}

void C_CopyTo_Stack::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str n = os_trim(location->getValue());

      if (!g_exists.value || n.empty() || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         print(L"Failed to copy " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      _str newLoc = os_join(g_location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to copy " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      _str newPath = newLoc + OS_SEPARATOR + fulln;
      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (hasExt) {
            const _str ext = os_extension(newPath);
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ext);
         }
         else {
            newPath = os_stackPath(newPath);
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, g_isfile.value);
      g_success.value = s;

      if (s) {
         print(L"Copy " + getCCName(oldPath) + L" to " + getCCName(newLoc));
      }
      else {
         print(L"Failed to copy " + getCCName(oldPath));
      }
   }
}

void C_CopyToAs::run()
{
   if (g_running) {
      _str& oldPath = g_path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!g_exists.value || fulln.empty() || loc.empty()
          || os_isInvaild(fulln) || os_isInvaild(loc) || !os_hasParentDirectory(oldPath))
      {
         print(L"Failed to copy " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);

      if (!extensionless && hasExt && !os_hasExtension(fulln)) {
         fulln += L"." + os_extension(oldPath);
      }

      const _str newLoc = os_join(g_location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to copy " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      _str newPath = newLoc + OS_SEPARATOR + fulln;

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath))) {
            print(L"Failed to copy " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, g_isfile.value);
      g_success.value = s;

      if (s) {
         print(L"Copy " + getCCName(oldPath) + L" to " + getCCName(newLoc) + L" as '" + fulln + L"'");
      }
      else {
         print(L"Failed to copy " + getCCName(oldPath));
      }
   }
}

void C_CopyToAs_Stack::run()
{
   if (g_running) {
      const _str& oldPath = g_path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!g_exists.value || fulln.empty() || loc.empty()
          || os_isInvaild(fulln) || os_isInvaild(loc) || !os_hasParentDirectory(oldPath))
      {
         print(L"Failed to copy " + getCCName(oldPath));
         g_success.value = false;
         return;
      }

      const _str newLoc = os_join(g_location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            print(L"Failed to copy " + getCCName(oldPath));
            g_success.value = false;
            return;
         }
      }

      const _boo hasExt = g_isfile.value && os_hasExtension(oldPath);
      _str newPath;

      if (hasExt) {
         _str ex;
         if (!os_hasExtension(fulln)) {
            ex = os_extension(fulln);
         }
         else {
            ex = os_extension(oldPath);
            if (!extensionless) {
               fulln += L"." + ex;
            }
         }

         newPath = newLoc + OS_SEPARATOR + fulln;
         if (os_exists(newPath)) {
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ex);
            fulln = os_fullname(newPath);
         }
      }
      else {
         newPath = newLoc + OS_SEPARATOR + fulln;
         if (os_exists(newPath)) {
            newPath = os_stackPath(newPath);
            fulln = os_fullname(newPath);
         }
      }

      const _boo s = os_copyTo(oldPath, newPath, g_isfile.value);
      g_success.value = s;

      if (s) {
         print(L"Copy " + getCCName(oldPath) + L" to " + getCCName(newLoc) + L" as '" + fulln + L"'");
      }
      else {
         print(L"Failed to copy " + getCCName(oldPath));
      }
   }
}

_str getCCName(const _str& path)
{
   if (path.empty()) {
      return L"nothing";
   }

   return os_hasParentDirectory(path) || path.size() != 2
      ? (L"'" + os_fullname(path) + L"'")
      : (L"'" + path + L"\\'");
}

_str getCCNameShort(const _str& path)
{
   if (path.empty()) {
      return L"nothing";
   }

   const _str f = os_fullname(path);
   if (f.empty()) {
      return L"nothing";
   }

   return f.size() == 2 && f[1] == L':' && os_isDriveLetter(f[0])
      ? (L"'" + f + L"\\'")
      : (L"'" + f + L"'");
}

