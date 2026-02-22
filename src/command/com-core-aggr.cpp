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

#include "com-core-aggr.hpp"
#include "../perun2.hpp"
#include "../os/os.hpp"


namespace perun2::comm
{


void C_AggrCopy_String::run()
{
   const p_str n = os_trim(value->getValue());

   if (n.empty()) {
      aggregate->failedCopy++;
   }
   else if (os_isInvalid(n)) {
      aggregate->invalidCopy.insert(os_fullname(n));
   }
   else {
      aggregate->copyPaths.insert(os_leftJoin(this->locationContext->location->value, n));
   }
}

void C_AggrCopy_List::run()
{
   for (const p_str& str : value->getValue()) {
      const p_str n = os_trim(str);

      if (n.empty()) {
         aggregate->failedCopy++;
      }
      else if (os_isInvalid(n)) {
         aggregate->invalidCopy.insert(os_fullname(n));
      }
      else {
         aggregate->copyPaths.insert(os_leftJoin(this->locationContext->location->value, n));
      }
   }
}


void C_AggrSelect_String::run()
{
   const p_str n = os_trim(value->getValue());

   if (n.empty()) {
      aggregate->failedSelect++;
   }
   else if (os_isInvalid(n)) {
      aggregate->invalidSelect.insert(os_fullname(n));
   }
   else {
      const p_str path = os_leftJoin(this->locationContext->location->value, n);

      if (os_hasParentDirectory(path)) {
         const p_str parent = os_parent(path);
         auto it = aggregate->selectPaths.find(parent);

         if (it == aggregate->selectPaths.end()) {
            p_set newSet;
            newSet.insert(path);
            aggregate->selectPaths.insert(
               std::pair<p_str, p_set>(parent, newSet));
         }
         else {
            it->second.insert(path);
         }
      }
      else {
         aggregate->invalidSelect.insert(os_fullname(n));
      }
   }
}

void C_AggrSelect_List::run()
{
   p_str prevParent;
   p_set* prevSet = nullptr;

   for (const p_str& str : value->getValue()) {
      const p_str n = os_trim(str);

      if (n.empty()) {
         aggregate->failedSelect++;
      }
      else if (os_isInvalid(n)) {
         aggregate->invalidSelect.insert(os_fullname(n));
      }
      else {
         const p_str path = os_leftJoin(this->locationContext->location->value, n);

         if (os_hasParentDirectory(path)) {
            const p_str parent = os_parent(path);

            if (prevSet != nullptr && parent == prevParent) {
               prevSet->insert(path);
            }
            else {
               auto it = aggregate->selectPaths.find(parent);
               if (it == aggregate->selectPaths.end()) {
                  p_set newSet;
                  newSet.insert(path);
                  aggregate->selectPaths.insert(
                     std::pair<p_str, p_set>(parent, newSet));

                  prevSet = &(aggregate->selectPaths.find(parent)->second);
               }
               else {
                  it->second.insert(path);
                  prevSet = &(it->second);
               }
               prevParent = parent;
            }
         }
         else {
            aggregate->invalidSelect.insert(os_fullname(n));
         }
      }
   }
}



void logCopyError(Perun2Process& p2, const p_str& name)
{
   p2.logger.log(L"Failed to copy ", getCCNameShort(name));
}

void logCopySuccess(Perun2Process& p2, const p_str& name)
{
   p2.logger.log(L"Copy ", getCCNameShort(name));
}

void logSelectError(Perun2Process& p2, const p_str& name)
{
   p2.logger.log(L"Failed to select ", getCCNameShort(name));
}

void logSelectSuccess(Perun2Process& p2, const p_str& name)
{
   p2.logger.log(L"Select ", getCCNameShort(name));
}



void C_Copy_String::run()
{
   const p_str n = os_trim(value->getValue());
   if (os_isInvalid(n)) {
      logCopyError(this->perun2, n);
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str path = os_leftJoin(this->locationContext->location->value, n);
   if (os_exists(path)) {
      p_set set;
      set.insert(path);
      const p_bool s = os_copy(set);
      if (s) {
         logCopySuccess(this->perun2, path);
      }
      else {
         logCopyError(this->perun2, path);
      }
      this->perun2.contexts.success->value = s;

   }
   else {
      logCopyError(this->perun2, path);
      this->perun2.contexts.success->value = false;
   }
}

void C_Copy_List::run()
{
   const p_list elements = value->getValue();

   if (elements.empty()) {
      this->perun2.contexts.success->value = true;
      return;
   }

   p_set set;
   p_bool anyFailure = false;

   for (const p_str& str : elements) {
      const p_str n = os_trim(str);
      if (os_isInvalid(n)) {
         logCopyError(this->perun2, n);
         anyFailure = true;
      }
      else {
         const p_str path = os_leftJoin(this->locationContext->location->value, n);
         if (os_exists(path)) {
            set.insert(path);
         }
         else {
            logCopyError(this->perun2, path);
            anyFailure = true;
         }
      }
   }

   if (!set.empty()) {
      const p_bool s = os_copy(set);
      const auto end = set.end();

      for (auto it = set.begin(); it != end; ++it) {
         if (s) {
            logCopySuccess(this->perun2, *it);
         }
         else {
            logCopyError(this->perun2, *it);
         }
      }
   }

   this->perun2.contexts.success->value = !anyFailure;
}

void Selector::reset()
{
   this->selectPaths.clear();
   this->prevParent.clear();
   this->isFirst = true;
}

void Selector::insertValue()
{
   if (!isFirst && this->parent == prevParent) {
      prevSet->insert(this->path);
   }
   else {
      auto it = selectPaths.find(this->parent);
      if (it == selectPaths.end())
      {
         p_set newSet;
         newSet.insert(this->path);
         selectPaths.insert(std::pair<p_str, p_set>(this->parent, newSet));
         prevSet = &(selectPaths.find(this->parent)->second);
      }
      else
      {
         it->second.insert(this->path);
         prevSet = &(it->second);
      }
      prevParent = this->parent;
   }

   isFirst = false;
}

void Selector::run()
{
   if (this->selectPaths.empty()) {
      this->perun2.contexts.success->value = false;
   }
   else {
      p_bool anyFailed = false;

      for (auto it = this->selectPaths.begin(); it != this->selectPaths.end(); it++)
      {
         if (this->perun2.isNotRunning()) {
            break;
         }

         const p_bool success = os_select(it->first, it->second);
         if (!anyFailed && !success) {
            anyFailed = true;
         }

         const auto end = it->second.end();
         for (auto it2 = it->second.begin(); it2 != end; ++it2) {
            if (success) {
               logSelectSuccess(this->perun2, *it2);
            }
            else {
               logSelectError(this->perun2, *it2);
            }
         }
      }

      this->perun2.contexts.success->value = !anyFailed;
   }
}

void C_Select_String::run()
{
   const p_str n = os_trim(value->getValue());
   if (os_isInvalid(n)) {
      logSelectError(this->perun2, n);
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str path = os_leftJoin(this->locationContext->location->value, n);
   p_bool success = false;

   if (os_exists(path) && os_hasParentDirectory(path)) {
      const p_str parent = os_parent(path);

      if (os_directoryExists(parent)) {
         p_set set;
         set.insert(path);
         success = os_select(parent, set);

         if (success) {
            logSelectSuccess(this->perun2, path);
         }
         else {
            logSelectError(this->perun2, path);
         }
      }
      else {
         logSelectError(this->perun2, path);
      }
   }
   else {
      logSelectError(this->perun2, path);
   }

   this->perun2.contexts.success->value = success;
}

void C_Select_List::run()
{
   const p_list elements = value->getValue();

   if (elements.empty()) {
      this->perun2.contexts.success->value = true;
      return;
   }

   this->selector.reset();

   for (const p_str& str : elements) {
      const p_str n = os_trim(str);

      if (os_isInvalid(n)) {
         logSelectError(this->perun2, n);
         continue;
      }

      this->path = os_leftJoin(this->locationContext->location->value, n);

      if (!os_exists(path) || !os_hasParentDirectory(path)) {
         logSelectError(this->perun2, path);
         continue;
      }

      this->parent = os_parent(path);
      if (!os_directoryExists(parent)) {
         logSelectError(this->perun2, path);
         continue;
      }

      this->selector.insertValue();
   }

   this->selector.run();
}


void C_Select_Definition::run()
{
   this->selector.reset();

   while (this->value->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->value->reset();
         break;
      }

      this->path = os_leftJoin(this->locationContext->location->value, this->value->getValue());

      if (!os_exists(path) || !os_hasParentDirectory(path)) {
         logSelectError(this->perun2, path);
         continue;
      }

      this->parent = os_parent(path);
      if (!os_directoryExists(parent)) {
         logSelectError(this->perun2, path);
         continue;
      }

      this->selector.insertValue();
   }

   this->selector.run();
}


void C_Select_ContextDefinition::run()
{
   this->selector.reset();

   while (this->value->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->value->reset();
         break;
      }

      if (!this->context->v_exists->value || this->context->v_parent->value.empty()) {
         logSelectError(this->perun2, this->context->v_path->value);
         continue;
      }

      this->selector.insertValue();
   }

   this->selector.run();
}

}
