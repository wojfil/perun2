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
#include "../print.h"
#include "../os.h"


namespace uro::comm
{

////
//   simple commands:
////

void C_Delete::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_delete(this->inner.path.value);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Delete ", getCCName(this->inner.path.value));
         if (hasAttribute) {
            this->attribute->run();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to delete ", getCCName(this->inner.path.value));
      }
   }
}

void C_Drop::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_drop(this->inner.path.value, this->inner.isfile.value, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Drop ", getCCName(this->inner.path.value));
         if (hasAttribute) {
            this->attribute->run();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to drop ", getCCName(this->inner.path.value));
      }
   }
}

void C_Hide::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_hide(this->inner.path.value);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Hide ", getCCName(this->inner.path.value));
         if (saveChanges) {
            this->inner.hidden.value = true;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to hide ", getCCName(this->inner.path.value));
      }
   }
}

void C_Lock::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_lock(this->inner.path.value);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Lock ", getCCName(this->inner.path.value));
         if (saveChanges) {
            this->inner.readonly.value = true;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to lock ", getCCName(this->inner.path.value));
      }
   }
}

void C_Open::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_open(this->inner.path.value);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Open ", getCCName(this->inner.path.value));
      }
      else {
         commandLog(this->uroboros, L"Failed to open ", getCCName(this->inner.path.value));
      }
   }
}

void C_Unlock::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_unlock(this->inner.path.value);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Unlock ", getCCName(this->inner.path.value));
         if (saveChanges) {
            this->inner.readonly.value = false;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to unlock ", getCCName(this->inner.path.value));
      }
   }
}

void C_Unhide::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->inner.exists.value && os_unhide(this->inner.path.value);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Unhide ", getCCName(this->inner.path.value));
         if (saveChanges) {
            this->inner.hidden.value = false;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to unhide ", getCCName(this->inner.path.value));
      }
   }
}

void C_OpenWith::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str pro = os_trim(program->getValue());

      if (!this->inner.exists.value || pro.empty()) {
         commandLog(this->uroboros, L"Failed to open ", getCCName(this->inner.path.value), L" with ", getCCNameShort(pro));
         this->inner.success.value = false;
         return;
      }

      const _str proPath = os_join(this->inner.location.value, pro);
      if (os_exists(proPath)) {
         const _bool s = os_openWith(proPath, os_quoteEmbraced(this->inner.path.value));
         this->inner.success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Open ", getCCName(this->inner.path.value), L" with ", getCCName(proPath));
         }
         else {
            commandLog(this->uroboros, L"Failed to open ", getCCName(this->inner.path.value), L" with ", getCCName(proPath));
         }
      }
      else {
         if (!os_hasParentDirectory(this->inner.path.value)) {
            commandLog(this->uroboros, L"Failed to open ", getCCName(this->inner.path.value), L" with '", pro, L"'");
            this->inner.success.value = false;
            return;
         }

         const _str com = str(pro, L" ", os_quoteEmbraced(this->inner.path.value));
         const _bool s = os_process(com, os_parent(this->inner.path.value));
         this->inner.success.value = s;

         if (s) {
            commandLog(this->uroboros, L"Open ", getCCName(this->inner.path.value), L" with '", pro, L"'");
         }
         else {
            commandLog(this->uroboros, L"Failed to open ", getCCName(this->inner.path.value), L" with '", pro, L"'");
         }
      }
   }
}

void C_ReaccessTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->inner.access.value;
      t.setValue(time->getValue());
      this->inner.success.value = this->inner.exists.value
         && os_setTime(this->inner.path.value, this->inner.creation.value, t, this->inner.modification.value);

      if (this->inner.success.value) {
         commandLog(this->uroboros, L"Reaccess ", getCCName(this->inner.path.value), L" to ", t.toString());

         if (saveChanges) {
            this->inner.access.value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to reaccess ", getCCName(this->inner.path.value));
      }
   }
}

void C_RechangeTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->inner.change.value;
      t.setValue(time->getValue());
      this->inner.success.value = this->inner.exists.value
         && os_setTime(this->inner.path.value, this->inner.creation.value, this->inner.access.value, t);

      if (this->inner.success.value) {
         commandLog(this->uroboros, L"Rechange ", getCCName(this->inner.path.value), L" to ", t.toString());

         if (saveChanges) {
            this->inner.change.value = t;
            this->inner.modification.value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to rechange ", getCCName(this->inner.path.value));
      }
   }
}

void C_RecreateTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->inner.creation.value;
      t.setValue(time->getValue());
      this->inner.success.value = this->inner.exists.value
         && os_setTime(this->inner.path.value, t, this->inner.access.value, this->inner.modification.value);

      if (this->inner.success.value) {
         commandLog(this->uroboros, L"Recreate ", getCCName(this->inner.path.value),  L" to ", t.toString());

         if (saveChanges) {
            this->inner.creation.value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to recreate ", getCCName(this->inner.path.value));
      }
   }
}

void C_RemodifyTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->inner.modification.value;
      t.setValue(time->getValue());
      this->inner.success.value = this->inner.exists.value
         && os_setTime(this->inner.path.value, this->inner.creation.value, this->inner.access.value, t);

      if (this->inner.success.value) {
         commandLog(this->uroboros, L"Remodify ", getCCName(this->inner.path.value), L" to ", t.toString());

         if (saveChanges) {
            this->inner.modification.value = t;
            this->inner.change.value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to remodify ", getCCName(this->inner.path.value));
      }
   }
}

void C_RenameTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str n = os_fullname(os_trim(name->getValue()));

      if (!this->inner.exists.value || os_isInvaild(n)
          || !os_hasParentDirectory(this->inner.path.value) || os_isAbsolute(n)) {

         commandLog(this->uroboros, L"Failed to rename ", getCCName(this->inner.path.value));
         this->inner.success.value = false;
         return;
      }

      const _bool hasExt = this->inner.isfile.value && os_hasExtension(this->inner.path.value);

      if (!extensionless && hasExt && !os_hasExtension(n)) {
         n += str(L".", os_extension(this->inner.path.value));
      }

      const _str base = os_parent(this->inner.path.value);
      const _str newPath = str(base, OS_SEPARATOR_STRING, n);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to rename ", getCCName(this->inner.path.value));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_moveTo(this->inner.path.value, newPath);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Rename ", getCCName(this->inner.path.value), L" to '", n, L"'");

         if (saveChanges) {
            this->inner.fullname.value = n;
            this->inner.name.value = hasExt ? os_name(newPath) : n;
            this->inner.path.value = newPath;
            this->inner.this_s.value = newPath;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to rename ", getCCName(this->inner.path.value));
      }
   }
}

void C_RenameTo_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str n = os_fullname(os_trim(name->getValue()));
      if (!this->inner.exists.value || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath) || os_isAbsolute(n)) {

         commandLog(this->uroboros, L"Failed to rename ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);
      const _str parent = os_parent(oldPath);
      _str newPath;
      _bool s;

      if (hasExt) {
         _str ex;
         if (os_hasExtension(n)) {
            ex = os_extension(n);
         }
         else {
            ex = os_extension(oldPath);
            if (!extensionless) {
               n += str(L".", ex);
            }
         }

         newPath = str(parent, OS_SEPARATOR_STRING, n);

         if (os_exists(newPath)) {
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ex);
         }

         s = os_moveTo(oldPath, newPath);
         n = os_fullname(newPath);

         if (s && saveChanges) {
            this->inner.extension.value = ex;
         }
      }
      else {
         newPath = str(parent, OS_SEPARATOR_STRING, n);
         newPath = os_stackPath(newPath);
         s = os_moveTo(oldPath, newPath);
      }

      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Rename ", getCCName(this->inner.path.value), L" to '", n, L"'");

         if (saveChanges) {
            this->inner.fullname.value = n;
            this->inner.name.value = hasExt ? os_name(n) : n;
            this->inner.path.value = newPath;
            this->inner.this_s.value = newPath;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to rename ", getCCName(this->inner.path.value));
      }
   }
}

void C_MoveTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str n = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath)) {

         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _str newLoc = os_join(this->inner.location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      const _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_moveTo(oldPath, newPath);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc));

         if (hasAttribute) {
            this->inner.this_s.value = newPath;
            this->attribute->run();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
      }
   }
}

void C_MoveTo_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str n = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      _str newLoc = os_join(this->inner.location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);

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

      const _bool s = os_moveTo(oldPath, newPath);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc));

         if (hasAttribute) {
            this->inner.this_s.value = newPath;
            this->attribute->run();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
      }
   }
}

void C_MoveToAs::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(fulln)
           || os_isInvaild(loc) || !os_hasParentDirectory(oldPath)) {

         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);

      if (!extensionless && hasExt && !os_hasExtension(fulln)) {
         fulln += str(L".", os_extension(oldPath));
      }

      const _str newLoc = os_join(this->inner.location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_moveTo(oldPath, newPath);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as '", fulln, L"'");

         if (hasAttribute) {
            this->inner.this_s.value = newPath;
            this->attribute->run();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
      }
   }
}

void C_MoveToAs_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& oldPath = this->inner.path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(fulln)
          || os_isInvaild(loc) || !os_hasParentDirectory(oldPath)) {

         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _str newLoc = os_join(this->inner.location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);
      _str newPath;
      _bool s;

      if (hasExt) {
         _str ex;
         if (!os_hasExtension(fulln)) {
            ex = os_extension(fulln);
         }
         else {
            ex = os_extension(oldPath);
            if (!extensionless) {
               fulln += str(L".", ex);
            }
         }

         newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
         if (os_exists(newPath)) {
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ex);
            fulln = os_fullname(newPath);
         }

         s = os_moveTo(oldPath, newPath);
      }
      else {
         newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
         if (os_exists(newPath)) {
            newPath = os_stackPath(newPath);
            fulln = os_fullname(newPath);
         }
         s = os_moveTo(oldPath, newPath);
      }

      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as '", fulln, L"'");

         if (hasAttribute) {
            this->inner.this_s.value = newPath;
            this->attribute->run();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
      }
   }
}

void C_DownloadFrom_String::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& dest = this->inner.location.value;
      const _str name = os_trim(element->getValue());
      const _str src = os_trim(location->getValue());

      if (os_isInvaild(name) || os_isInvaild(src) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(name));
         this->inner.success.value = false;
         return;
      }

      const _str srcLoc = os_join(dest, src);
      const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, name);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(name));
         this->inner.success.value = false;
         return;
      }

      const _str newPath = str(dest, OS_SEPARATOR_STRING, name);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(name));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Download ", getCCName(name), L" from ", getCCName(srcLoc));
      }
      else {
         commandLog(this->uroboros, L"Failed to download ", getCCName(name));
      }
   }
}

void C_DownloadFrom_List::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& dest = this->inner.location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner.success.value = true;
         return;
      }

      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(os_trim(names[i])));
         }
         this->inner.success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str& n = os_trim(names[i]);
         const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, n);

         if (os_isInvaild(n) || !os_exists(oldPath) || os_isAbsolute(n)) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            success = false;
         }
         else {
            const _str newPath = str(dest, OS_SEPARATOR_STRING, n);

            if (os_exists(newPath)) {
               if (!(forced && os_drop(newPath, this->uroboros))) {
                  commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
                  success = false;
                  continue;
               }
            }

            const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);

            if (s) {
               commandLog(this->uroboros, L"Download ", getCCName(oldPath), L" from ", getCCName(srcLoc));
            }
            else {
               commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
               success = false;
            }
         }
      }

      this->inner.success.value = success;
   }
}

void C_DownloadFrom_Definition::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str dest = this->inner.location.value;
      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(this->inner.location.value, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         commandLog(this->uroboros, L"Failed to download");
         this->inner.success.value = false;
         return;
      }

      _bool success = true;
      this->inner.location.value = srcLoc;

      while (this->uroboros.state == State::s_Running && elements->hasNext()) {
         const _str n = os_trim(elements->getValue());
         const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, n);

         if (os_isInvaild(n) || !os_exists(oldPath) || os_isAbsolute(n)) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(n));
            success = false;
         }
         else {
            const _str newPath = str(dest, OS_SEPARATOR_STRING, n);

            if (os_exists(newPath)) {
               if (!(forced && os_drop(newPath, this->uroboros))) {
                  commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
                  success = false;
                  continue;
               }
            }

            const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);

            if (s) {
               commandLog(this->uroboros, L"Download ", getCCName(oldPath), L" from ", getCCName(srcLoc));
            }
            else {
               commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
               success = false;
            }
         }
      }

      this->inner.success.value = success;
      this->inner.location.value = dest;
   }
}

void C_DownloadFrom_String_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& dest = this->inner.location.value;
      const _str name = os_trim(element->getValue());
      const _str src = os_trim(location->getValue());

      if (os_isInvaild(name) || os_isInvaild(src) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to download ", getCCNameShort(name));
         this->inner.success.value = false;
         return;
      }

      const _str srcLoc = os_join(dest, src);
      const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, name);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      _str newPath = str(dest, OS_SEPARATOR_STRING, name);

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

      const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Download ", getCCName(oldPath), L" from ", getCCName(srcLoc));
      }
      else {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
      }
   }
}

void C_DownloadFrom_List_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& dest = this->inner.location.value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->inner.success.value = true;
         return;
      }

      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(os_trim(names[i])));
         }
         this->inner.success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; i < len; i++) {
         const _str n = os_trim(names[i]);
         const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, n);

         if (os_isInvaild(n) || os_isAbsolute(n) || !os_exists(oldPath)) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(n));
            success = false;
         }
         else {
            _str newPath = str(dest, OS_SEPARATOR_STRING, n);

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

            const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);

            if (s) {
               commandLog(this->uroboros, L"Download ", getCCName(oldPath), L" from ", getCCName(srcLoc));
            }
            else {
               commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
               success = false;
            }
         }
      }

      this->inner.success.value = success;
   }
}

void C_DownloadFrom_Definition_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str dest = this->inner.location.value;
      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         commandLog(this->uroboros, L"Failed to download");
         this->inner.success.value = false;
         return;
      }

      _bool success = true;
      const _str f = os_fullname(srcLoc);
      this->inner.location.value = srcLoc;

      while (elements->hasNext()) {
         if (!this->uroboros.state == State::s_Running) {
            elements->reset();
            break;
         }
         const _str n = os_trim(elements->getValue());
         const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, n);

         if (os_isInvaild(n) || os_isAbsolute(n) || !os_exists(oldPath)) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(n));
            success = false;
         }
         else {
            _str newPath = str(dest, OS_SEPARATOR_STRING, n);

            if (os_exists(newPath)) {
               if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
                  const _str ext = os_extension(newPath);
                  const _str base = os_stackPathBase(newPath);
                  newPath = os_stackPathExt(base, ext);
               }
               else {
                  newPath = os_stackPath(newPath);

                  if (!this->uroboros.state == State::s_Running) {
                     elements->reset();
                     break;
                  }
               }
            }

            const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);

            if (s) {
               commandLog(this->uroboros, L"Download ", getCCName(oldPath), L" from ", getCCName(srcLoc));
            }
            else {
               commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
               success = false;
            }
         }
      }

      this->inner.success.value = success;
      this->inner.location.value = dest;
   }
}

void C_Download_String::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str oldElement = os_trim(element->getValue());
      const _str& dest = this->inner.location.value;

      if (os_isInvaild(oldElement) || !os_directoryExists(dest) || !os_isPath(oldElement))
      {
         commandLog(this->uroboros, L"Failed to download ", getCCNameShort(oldElement));
         this->inner.success.value = false;
         return;
      }

      const _str oldPath = os_join(dest, oldElement);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _str newPath = str(dest, OS_SEPARATOR_STRING, os_fullname(oldPath));

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Download ", getCCName(oldPath));
      }
      else {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
      }
   }
}

void C_Download_List::run()
{
   if (this->uroboros.state == State::s_Running) {
      _list oldPaths = elements->getValue();
      const _size len = oldPaths.size();

      if (len == 0) {
         this->inner.success.value = true;
         return;
      }

      const _str& dest = this->inner.location.value;

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            const _str p = os_trim(oldPaths[i]);
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(p));
         }
         this->inner.success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; this->uroboros.state == State::s_Running && i < len; i++) {
         const _str n = os_trim(oldPaths[i]);
         const _str oldPath = os_join(dest, n);

         if (os_isInvaild(n) || !os_isPath(n) || !os_exists(oldPath)) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            success = false;
            continue;
         }

         const _str newPath = str(dest, OS_SEPARATOR_STRING, os_fullname(oldPath));

         if (os_exists(newPath)) {
            if (!(forced && os_drop(newPath, this->uroboros))) {
               commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
               success = false;
               continue;
            }
         }

         const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);

         if (s) {
            commandLog(this->uroboros, L"Download ", getCCName(oldPath));
         }
         else {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            success = false;
         }
      }

      this->inner.success.value = success;
   }
}

void C_Download_String_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str oldElement = os_trim(element->getValue());
      const _str& dest = this->inner.location.value;

      if (os_isInvaild(oldElement) || !os_isPath(oldElement) || !os_directoryExists(dest))
      {
         commandLog(this->uroboros, L"Failed to download ", getCCNameShort(oldElement));
         this->inner.success.value = false;
         return;
      }

      const _str oldPath = os_join(dest, oldElement);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      _str newPath = str(dest, OS_SEPARATOR_STRING, os_fullname(oldPath));

      if (os_exists(newPath)) {
         if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
            const _str ext = os_extension(newPath);
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ext);
         }
         else {
            newPath = os_stackPath(newPath);
         }
         if (!this->uroboros.state == State::s_Running) {
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Download ", getCCName(oldPath));
      }
      else {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
      }
   }
}

void C_Download_List_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      _list oldPaths = elements->getValue();
      const _size len = oldPaths.size();

      if (len == 0) {
         this->inner.success.value = true;
         return;
      }

      const _str& dest = this->inner.location.value;

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            const _str p = os_trim(oldPaths[i]);
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(p));
         }
         this->inner.success.value = false;
         return;
      }

      _bool success = true;

      for (_size i = 0; this->uroboros.state == State::s_Running && i < len; i++) {
         const _str oldPath = os_trim(oldPaths[i]);

         if (os_isInvaild(oldPath) || !os_isPath(oldPath) || !os_exists(oldPath)) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            success = false;
            continue;
         }

         _str newPath = str(dest, OS_SEPARATOR_STRING, os_fullname(oldPath));

         if (os_exists(newPath)) {
            if (os_isFile(oldPath) && os_hasExtension(oldPath)) {
               const _str ext = os_extension(newPath);
               const _str base = os_stackPathBase(newPath);
               newPath = os_stackPathExt(base, ext);
            }
            else {
               newPath = os_stackPath(newPath);
            }
            if (!this->uroboros.state == State::s_Running) {
               break;
            }
         }

         const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);

         if (s) {
            commandLog(this->uroboros, L"Download ", getCCName(oldPath));
         }
         else {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            success = false;
         }
      }

      this->inner.success.value = success;
   }
}

void C_CopyTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str n = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _str newLoc = os_join(this->inner.location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      const _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, this->inner.isfile.value, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc));
      }
      else {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
      }
   }
}

void C_CopyTo_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str n = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      _str newLoc = os_join(this->inner.location.value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);

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

      const _bool s = os_copyTo(oldPath, newPath, this->inner.isfile.value, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc));
      }
      else {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
      }
   }
}

void C_CopyToAs::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->inner.path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(fulln) || os_isInvaild(loc) || !os_hasParentDirectory(oldPath))
      {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);

      if (!extensionless && hasExt && !os_hasExtension(fulln)) {
         fulln += str(L".", os_extension(oldPath));
      }

      const _str newLoc = os_join(this->inner.location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, this->inner.isfile.value, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as '", fulln, L"'");
      }
      else {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
      }
   }
}

void C_CopyToAs_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& oldPath = this->inner.path.value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->inner.exists.value || os_isInvaild(fulln) || os_isInvaild(loc) || !os_hasParentDirectory(oldPath))
      {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->inner.success.value = false;
         return;
      }

      const _str newLoc = os_join(this->inner.location.value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->inner.success.value = false;
            return;
         }
      }

      const _bool hasExt = this->inner.isfile.value && os_hasExtension(oldPath);
      _str newPath;

      if (hasExt) {
         _str ex;
         if (!os_hasExtension(fulln)) {
            ex = os_extension(fulln);
         }
         else {
            ex = os_extension(oldPath);
            if (!extensionless) {
               fulln += str(L".", ex);
            }
         }

         newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
         if (os_exists(newPath)) {
            const _str base = os_stackPathBase(newPath);
            newPath = os_stackPathExt(base, ex);
            fulln = os_fullname(newPath);
         }
      }
      else {
         newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
         if (os_exists(newPath)) {
            newPath = os_stackPath(newPath);
            fulln = os_fullname(newPath);
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, this->inner.isfile.value, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Copy ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as '", fulln, L"'");
      }
      else {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
      }
   }
}

_str getCCName(const _str& path)
{
   if (path.empty()) {
      return L"nothing";
   }

   return os_hasParentDirectory(path) || path.size() != 2
      ? str(L"'", os_fullname(path), L"'")
      : str(L"'", path, OS_SEPARATOR_APOSTROPHE);
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
      ? str(L"'", f, OS_SEPARATOR_APOSTROPHE)
      : str(L"'", f, L"'");
}

}
