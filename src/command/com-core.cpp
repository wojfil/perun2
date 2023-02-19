/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
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
      const _bool s = this->context->v_exists->value && os_delete(this->context->v_path->value);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Delete ", getCCName(this->context->v_path->value));
         this->context->reloadData();
      }
      else {
         commandLog(this->uroboros, L"Failed to delete ", getCCName(this->context->v_path->value));
      }
   }
}

void C_Drop::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->context->v_exists->value && os_drop(this->context->v_path->value, this->context->v_isfile->value, this->uroboros);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Drop ", getCCName(this->context->v_path->value));
         if (saveChanges) {
            this->context->reloadData();
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to drop ", getCCName(this->context->v_path->value));
      }
   }
}

void C_Hide::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->context->v_exists->value && os_hide(this->context->v_path->value);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Hide ", getCCName(this->context->v_path->value));
         if (saveChanges) {
            this->context->v_hidden->value = true;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to hide ", getCCName(this->context->v_path->value));
      }
   }
}

void C_Lock::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->context->v_exists->value && os_lock(this->context->v_path->value);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Lock ", getCCName(this->context->v_path->value));
         if (saveChanges) {
            this->context->v_readonly->value = true;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to lock ", getCCName(this->context->v_path->value));
      }
   }
}

void C_Open::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->context->v_exists->value && os_open(this->context->v_path->value);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Open ", getCCName(this->context->v_path->value));
      }
      else {
         commandLog(this->uroboros, L"Failed to open ", getCCName(this->context->v_path->value));
      }
   }
}

void C_Unlock::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->context->v_exists->value && os_unlock(this->context->v_path->value);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Unlock ", getCCName(this->context->v_path->value));
         if (saveChanges) {
            this->context->v_readonly->value = false;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to unlock ", getCCName(this->context->v_path->value));
      }
   }
}

void C_Unhide::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _bool s = this->context->v_exists->value && os_unhide(this->context->v_path->value);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Unhide ", getCCName(this->context->v_path->value));
         if (saveChanges) {
            this->context->v_hidden->value = false;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to unhide ", getCCName(this->context->v_path->value));
      }
   }
}

void C_OpenWith::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str pro = os_trim(program->getValue());

      if (!this->context->v_exists->value || pro.empty()) {
         commandLog(this->uroboros, L"Failed to open ", getCCName(this->context->v_path->value), L" with ", getCCNameShort(pro));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str proPath = os_join(this->locationContext->location->value, pro);
      if (os_exists(proPath)) {
         const _bool s = os_openWith(proPath, os_quoteEmbraced(this->context->v_path->value));
         this->uroboros.contexts.success->value = s;

         if (s) {
            commandLog(this->uroboros, L"Open ", getCCName(this->context->v_path->value), L" with ", getCCName(proPath));
         }
         else {
            commandLog(this->uroboros, L"Failed to open ", getCCName(this->context->v_path->value), L" with ", getCCName(proPath));
         }
      }
      else {
         if (!os_hasParentDirectory(this->context->v_path->value)) {
            commandLog(this->uroboros, L"Failed to open ", getCCName(this->context->v_path->value), L" with '", pro, L"'");
            this->uroboros.contexts.success->value = false;
            return;
         }

         const _str com = str(pro, L" ", os_quoteEmbraced(this->context->v_path->value));
         const _bool s = os_process(com, os_parent(this->context->v_path->value));
         this->uroboros.contexts.success->value = s;

         if (s) {
            commandLog(this->uroboros, L"Open ", getCCName(this->context->v_path->value), L" with '", pro, L"'");
         }
         else {
            commandLog(this->uroboros, L"Failed to open ", getCCName(this->context->v_path->value), L" with '", pro, L"'");
         }
      }
   }
};

void C_ReaccessTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->context->v_access->value;
      t.setValue(time->getValue());
      this->uroboros.contexts.success->value = this->context->v_exists->value
         && os_setTime(this->context->v_path->value, this->context->v_creation->value, t, this->context->v_modification->value);

      if (this->uroboros.contexts.success->value) {
         commandLog(this->uroboros, L"Reaccess ", getCCName(this->context->v_path->value), L" to ", t.toString());

         if (saveChanges) {
            this->context->v_access->value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to reaccess ", getCCName(this->context->v_path->value));
      }
   }
}

void C_RechangeTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->context->v_change->value;
      t.setValue(time->getValue());
      this->uroboros.contexts.success->value = this->context->v_exists->value
         && os_setTime(this->context->v_path->value, this->context->v_creation->value, this->context->v_access->value, t);

      if (this->uroboros.contexts.success->value) {
         commandLog(this->uroboros, L"Rechange ", getCCName(this->context->v_path->value), L" to ", t.toString());

         if (saveChanges) {
            this->context->v_change->value = t;
            this->context->v_modification->value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to rechange ", getCCName(this->context->v_path->value));
      }
   }
}

void C_RecreateTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->context->v_creation->value;
      t.setValue(time->getValue());
      this->uroboros.contexts.success->value = this->context->v_exists->value
         && os_setTime(this->context->v_path->value, t, this->context->v_access->value, this->context->v_modification->value);

      if (this->uroboros.contexts.success->value) {
         commandLog(this->uroboros, L"Recreate ", getCCName(this->context->v_path->value),  L" to ", t.toString());

         if (saveChanges) {
            this->context->v_creation->value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to recreate ", getCCName(this->context->v_path->value));
      }
   }
}

void C_RemodifyTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _tim t = this->context->v_modification->value;
      t.setValue(time->getValue());
      this->uroboros.contexts.success->value = this->context->v_exists->value
         && os_setTime(this->context->v_path->value, this->context->v_creation->value, this->context->v_access->value, t);

      if (this->uroboros.contexts.success->value) {
         commandLog(this->uroboros, L"Remodify ", getCCName(this->context->v_path->value), L" to ", t.toString());

         if (saveChanges) {
            this->context->v_modification->value = t;
            this->context->v_change->value = t;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to remodify ", getCCName(this->context->v_path->value));
      }
   }
}

void C_RenameTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str n = os_fullname(os_trim(name->getValue()));

      if (!this->context->v_exists->value || os_isInvaild(n)
          || !os_hasParentDirectory(this->context->v_path->value) || os_isAbsolute(n)) {

         commandLog(this->uroboros, L"Failed to rename ", getCCName(this->context->v_path->value));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(this->context->v_path->value);

      if (!extensionless && hasExt && !os_hasExtension(n)) {
         n += str(L".", os_extension(this->context->v_path->value));
      }

      const _str base = os_parent(this->context->v_path->value);
      const _str newPath = str(base, OS_SEPARATOR_STRING, n);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to rename ", getCCName(this->context->v_path->value));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_moveTo(this->context->v_path->value, newPath);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Rename ", getCCName(this->context->v_path->value), L" to '", n, L"'");

         if (saveChanges) {
            this->context->v_fullname->value = n;
            this->context->v_name->value = hasExt ? os_name(newPath) : n;
            this->context->v_path->value = newPath;
            this->context->this_->value = newPath;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to rename ", getCCName(this->context->v_path->value));
      }
   }
}

void C_RenameTo_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->context->v_path->value;
      _str n = os_fullname(os_trim(name->getValue()));
      if (!this->context->v_exists->value || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath) || os_isAbsolute(n)) {

         commandLog(this->uroboros, L"Failed to rename ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);
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
            this->context->v_extension->value = ex;
         }
      }
      else {
         newPath = str(parent, OS_SEPARATOR_STRING, n);
         newPath = os_stackPath(newPath);
         s = os_moveTo(oldPath, newPath);
      }

      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Rename ", getCCName(this->context->v_path->value), L" to '", n, L"'");

         if (saveChanges) {
            this->context->v_fullname->value = n;
            this->context->v_name->value = hasExt ? os_name(n) : n;
            this->context->v_path->value = newPath;
            this->context->this_->value = newPath;
         }
      }
      else {
         commandLog(this->uroboros, L"Failed to rename ", getCCName(this->context->v_path->value));
      }
   }
}

void C_MoveTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->context->v_path->value;
      _str n = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(n)
          || !os_hasParentDirectory(oldPath)) {

         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str newLoc = os_join(this->locationContext->location->value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      const _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_moveTo(oldPath, newPath);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc));

         if (this->saveChanges) {
            this->context->loadData(newPath);
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
      _str& oldPath = this->context->v_path->value;
      _str n = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      _str newLoc = os_join(this->locationContext->location->value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

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
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc));

         if (this->saveChanges) {
            this->context->loadData(newPath);
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
      _str& oldPath = this->context->v_path->value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(fulln)
           || os_isInvaild(loc) || !os_hasParentDirectory(oldPath)) {

         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

      if (!extensionless && hasExt && !os_hasExtension(fulln)) {
         fulln += str(L".", os_extension(oldPath));
      }

      const _str newLoc = os_join(this->locationContext->location->value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_moveTo(oldPath, newPath);
      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as '", fulln, L"'");

         if (this->saveChanges) {
            this->context->loadData(newPath);
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
      const _str& oldPath = this->context->v_path->value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(fulln)
          || os_isInvaild(loc) || !os_hasParentDirectory(oldPath)) {

         commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str newLoc = os_join(this->locationContext->location->value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to move ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);
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

      this->uroboros.contexts.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Move ", getCCName(oldPath), L" to ", getCCName(newLoc), L" as '", fulln, L"'");

         if (this->saveChanges) {
            this->context->loadData(newPath);
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
      const _str& dest = this->locContext->location->value;
      const _str name = os_trim(element->getValue());
      const _str src = os_trim(location->getValue());

      if (os_isInvaild(name) || os_isInvaild(src) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(name));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str srcLoc = os_join(dest, src);
      const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, name);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(name));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str newPath = str(dest, OS_SEPARATOR_STRING, name);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(name));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);
      this->uroboros.contexts.success->value = s;

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
      const _str& dest = this->locContext->location->value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->uroboros.contexts.success->value = true;
         return;
      }

      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(os_trim(names[i])));
         }
         this->uroboros.contexts.success->value = false;
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

      this->uroboros.contexts.success->value = success;
   }
}

void C_DownloadFrom_Definition::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str dest = this->locContext->location->value;
      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(this->locContext->location->value, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         commandLog(this->uroboros, L"Failed to download");
         this->uroboros.contexts.success->value = false;
         return;
      }

      _bool success = true;
      this->locContext->location->value = srcLoc;

      while (elements->hasNext()) {
         if (this->uroboros.state != State::s_Running) {
            elements->reset();
            break;
         }

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

      this->uroboros.contexts.success->value = success;
      this->locContext->location->value = dest;
   }
}

void C_DownloadFrom_String_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str& dest = this->locContext->location->value;
      const _str name = os_trim(element->getValue());
      const _str src = os_trim(location->getValue());

      if (os_isInvaild(name) || os_isInvaild(src) || !os_directoryExists(dest)) {
         commandLog(this->uroboros, L"Failed to download ", getCCNameShort(name));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str srcLoc = os_join(dest, src);
      const _str oldPath = str(srcLoc, OS_SEPARATOR_STRING, name);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
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
      this->uroboros.contexts.success->value = s;

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
      const _str& dest = this->locContext->location->value;
      const _list names = elements->getValue();
      const _size len = names.size();
      if (len == 0) {
         this->uroboros.contexts.success->value = true;
         return;
      }

      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         for (_size i = 0; i < len; i++) {
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(os_trim(names[i])));
         }
         this->uroboros.contexts.success->value = false;
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

      this->uroboros.contexts.success->value = success;
   }
}

void C_DownloadFrom_Definition_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str dest = this->locContext->location->value;
      const _str src = os_trim(location->getValue());
      const _str srcLoc = os_join(dest, src);

      if (os_isInvaild(src) || !os_directoryExists(dest) || !os_directoryExists(srcLoc)) {
         commandLog(this->uroboros, L"Failed to download");
         this->uroboros.contexts.success->value = false;
         return;
      }

      _bool success = true;
      const _str f = os_fullname(srcLoc);
      this->locContext->location->value = srcLoc;

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
                     elements.reset();
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

      this->uroboros.contexts.success->value = success;
      this->locContext->location->value = dest;
   }
}

void C_Download_String::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str oldElement = os_trim(element->getValue());
      const _str& dest = this->locContext->location->value;

      if (os_isInvaild(oldElement) || !os_directoryExists(dest) || !os_isPath(oldElement))
      {
         commandLog(this->uroboros, L"Failed to download ", getCCNameShort(oldElement));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str oldPath = os_join(dest, oldElement);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str newPath = str(dest, OS_SEPARATOR_STRING, os_fullname(oldPath));

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, os_isFile(oldPath), this->uroboros);
      this->uroboros.contexts.success->value = s;

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
         this->uroboros.contexts.success->value = true;
         return;
      }

      const _str& dest = this->locContext->location->value;

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            const _str p = os_trim(oldPaths[i]);
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(p));
         }
         this->uroboros.contexts.success->value = false;
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

      this->uroboros.contexts.success->value = success;
   }
}

void C_Download_String_Stack::run()
{
   if (this->uroboros.state == State::s_Running) {
      const _str oldElement = os_trim(element->getValue());
      const _str& dest = this->locContext->location->value;

      if (os_isInvaild(oldElement) || !os_isPath(oldElement) || !os_directoryExists(dest))
      {
         commandLog(this->uroboros, L"Failed to download ", getCCNameShort(oldElement));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str oldPath = os_join(dest, oldElement);

      if (!os_exists(oldPath)) {
         commandLog(this->uroboros, L"Failed to download ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
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
      this->uroboros.contexts.success->value = s;

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
         this->uroboros.contexts.success->value = true;
         return;
      }

      const _str& dest = this->locContext->location->value;

      if (!os_directoryExists(dest)) {
         for (_size i = 0; i < len; i++) {
            const _str p = os_trim(oldPaths[i]);
            commandLog(this->uroboros, L"Failed to download ", getCCNameShort(p));
         }
         this->uroboros.contexts.success->value = false;
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

      this->uroboros.contexts.success->value = success;
   }
}

void C_CopyTo::run()
{
   if (this->uroboros.state == State::s_Running) {
      _str& oldPath = this->context->v_path->value;
      _str n = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str newLoc = os_join(this->locationContext->location->value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      const _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->uroboros);
      this->uroboros.contexts.success->value = s;

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
      _str& oldPath = this->context->v_path->value;
      _str n = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(n) || !os_hasParentDirectory(oldPath)) {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      _str newLoc = os_join(this->locationContext->location->value, n);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _str fulln = os_fullname(oldPath);
      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);
      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

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

      const _bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->uroboros);
      this->uroboros.contexts.success->value = s;

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
      _str& oldPath = this->context->v_path->value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(fulln) || os_isInvaild(loc) || !os_hasParentDirectory(oldPath))
      {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);

      if (!extensionless && hasExt && !os_hasExtension(fulln)) {
         fulln += str(L".", os_extension(oldPath));
      }

      const _str newLoc = os_join(this->locationContext->location->value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      _str newPath = str(newLoc, OS_SEPARATOR_STRING, fulln);

      if (os_exists(newPath)) {
         if (!(forced && os_drop(newPath, this->uroboros))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->uroboros);
      this->uroboros.contexts.success->value = s;

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
      const _str& oldPath = this->context->v_path->value;
      _str fulln = os_fullname(os_trim(name->getValue()));
      const _str loc = os_trim(location->getValue());

      if (!this->context->v_exists->value || os_isInvaild(fulln) || os_isInvaild(loc) || !os_hasParentDirectory(oldPath))
      {
         commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
         this->uroboros.contexts.success->value = false;
         return;
      }

      const _str newLoc = os_join(this->locationContext->location->value, loc);

      if (!os_directoryExists(newLoc)) {
         if (!(os_hasParentDirectory(newLoc) && os_createDirectory(newLoc))) {
            commandLog(this->uroboros, L"Failed to copy ", getCCName(oldPath));
            this->uroboros.contexts.success->value = false;
            return;
         }
      }

      const _bool hasExt = this->context->v_isfile->value && os_hasExtension(oldPath);
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

      const _bool s = os_copyTo(oldPath, newPath, this->context->v_isfile->value, this->uroboros);
      this->uroboros.contexts.success->value = s;

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
      return STRING_NOTHING;
   }

   return os_hasParentDirectory(path) || path.size() != 2
      ? str(STRING_CHAR_APOSTROPHE, os_fullname(path), STRING_CHAR_APOSTROPHE)
      : str(STRING_CHAR_APOSTROPHE, path, OS_SEPARATOR_APOSTROPHE);
}

_str getCCNameShort(const _str& path)
{
   if (path.empty()) {
      return STRING_NOTHING;
   }

   const _str f = os_fullname(path);
   if (f.empty()) {
      return STRING_NOTHING;
   }

   return f.size() == 2 && f[1] == CHAR_COLON && os_isDriveLetter(f[0])
      ? str(STRING_CHAR_APOSTROPHE, f, OS_SEPARATOR_APOSTROPHE)
      : str(STRING_CHAR_APOSTROPHE, f, STRING_CHAR_APOSTROPHE);
}

}
