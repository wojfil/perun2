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

#include "com-core-aggr.h"
#include "../uroboros.h"
#include "../os.h"


void C_AggrCopy_String::run()
{
   const _str n = os_trim(value->getValue());
   if (n.empty()) {
      aggregate->failedCopy++;
   }
   else if (os_isInvaild(n)) {
      aggregate->invalidCopy.insert(os_fullname(n));
   }
   else {
      aggregate->copyPaths.insert(os_join(g_location.value, n));
   }
}

void C_AggrCopy_List::run()
{
   const _list list = value->getValue();
   const _size len = list.size();

   for (_size i = 0; i < len; i++) {
      const _str n = os_trim(list[i]);
      if (n.empty()) {
         aggregate->failedCopy++;
      }
      else if (os_isInvaild(n)) {
         aggregate->invalidCopy.insert(os_fullname(n));
      }
      else {
         aggregate->copyPaths.insert(os_join(g_location.value, n));
      }
   }
}

void C_AggrSelect_String::run()
{
   const _str n = os_trim(value->getValue());

   if (n.empty()) {
      aggregate->failedSelect++;
   }
   else if (os_isInvaild(n)) {
      aggregate->invalidSelect.insert(os_fullname(n));
   }
   else {
      const _str path = os_join(g_location.value, n);

      if (os_hasParentDirectory(path)) {
         const _str parent = os_parent(path);
         auto it = aggregate->selectPaths.find(parent);

         if (it == aggregate->selectPaths.end()) {
            std::set<_str> newSet;
            newSet.insert(path);
            aggregate->selectPaths.insert(
               std::pair<_str,std::set<_str>>(parent, newSet));
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
   const _list elements = value->getValue();
   const _size length = elements.size();
   _str prevParent;
   std::set<_str>* prevSet;

   for (_size i = 0; i < length; i++) {
      const _str n = os_trim(elements[i]);
      if (n.empty()) {
         aggregate->failedSelect++;
      }
      else if (os_isInvaild(n)) {
         aggregate->invalidSelect.insert(os_fullname(n));
      }
      else {
         const _str path = os_join(g_location.value, n);

         if (os_hasParentDirectory(path)) {
            const _str parent = os_parent(path);

            if (i > 0 && parent == prevParent) {
               prevSet->insert(path);
            }
            else {
               auto it = aggregate->selectPaths.find(parent);
               if (it == aggregate->selectPaths.end()) {
                  std::set<_str> newSet;
                  newSet.insert(path);
                  aggregate->selectPaths.insert(
                     std::pair<_str, std::set<_str>>(parent, newSet));

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



void logCopyError(const _str& name)
{
   print(L"Failed to copy " + getCCNameShort(name));
}

void logCopySuccess(const _str& name)
{
   print(L"Copy " + getCCNameShort(name));
}

void logSelectError(const _str& name)
{
   print(L"Failed to select " + getCCNameShort(name));
}

void logSelectSuccess(const _str& name)
{
   print(L"Select " + getCCNameShort(name));
}



void C_Copy_String::run()
{
   const _str n = os_trim(value->getValue());
   if (os_isInvaild(n)) {
      logCopyError(n);
      g_success.value = false;
      return;
   }

   const _str path = os_join(g_location.value, n);
   if (os_exists(path)) {
      std::set<_str> set;
      set.insert(path);
      const _boo s = os_copy(set);
      if (s) {
         logCopySuccess(path);
      }
      else {
         logCopyError(path);
      }
      g_success.value = s;

   }
   else {
      logCopyError(path);
      g_success.value = false;
   }
}

void C_Copy_List::run()
{
   const _list elements = value->getValue();
   const _size length = elements.size();

   if (length == 0) {
      g_success.value = true;
      return;
   }

   std::set<_str> set;
   _boo anyFailure = false;

   for (_size i = 0; i < length; i++) {
      const _str n = os_trim(elements[i]);
      if (os_isInvaild(n)) {
         logCopyError(n);
         anyFailure = true;
      }
      else {
         const _str path = os_join(g_location.value, n);
         if (os_exists(path)) {
            set.insert(path);
         }
         else {
            logCopyError(path);
            anyFailure = true;
         }
      }
   }

   if (!set.empty()) {
      const _boo s = os_copy(set);
      const auto end = set.end();

      for (auto it = set.begin(); it != end; ++it) {
         if (s) {
            logCopySuccess(*it);
         }
         else {
            logCopyError(*it);
         }
      }
   }

   g_success.value = !anyFailure;
}

void C_Select_String::run()
{
   const _str n = os_trim(value->getValue());
   if (os_isInvaild(n)) {
      logSelectError(n);
      g_success.value = false;
      return;
   }

   const _str path = os_join(g_location.value, n);
   _boo success = false;

   if (os_exists(path) && os_hasParentDirectory(path)) {
      const _str parent = os_parent(path);

      if (os_directoryExists(parent)) {
         std::set<_str> set;
         set.insert(path);
         success = os_select(parent, set);

         if (success) {
            logSelectSuccess(path);
         }
         else {
            logSelectError(path);
         }
      }
      else {
         logSelectError(path);
      }
   }
   else {
      logSelectError(path);
   }

   g_success.value = success;
}

void C_Select_List::run()
{
   const _list elements = value->getValue();
   const _size length = elements.size();

   if (length == 0) {
      g_success.value = true;
      return;
   }

   std::map<_str, std::set<_str>> selectPaths;
   _str prevParent;
   std::set<_str>* prevSet;
   bool isFirst = true;

   for (_size i = 0; i < length; i++) {
      const _str n = os_trim(elements[i]);

      if (os_isInvaild(n)) {
         logSelectError(n);
         continue;
      }

      const _str path = os_join(g_location.value, n);

      if (!os_exists(path) || !os_hasParentDirectory(path)) {
         logSelectError(path);
         continue;
      }

      const _str parent = os_parent(path);
      if (!os_directoryExists(parent)) {
         logSelectError(path);
         continue;
      }

      if (!isFirst && parent == prevParent) {
         prevSet->insert(path);
      }
      else {
         auto it = selectPaths.find(parent);
         if (it == selectPaths.end())
         {
            std::set<_str> newSet;
            newSet.insert(path);
            selectPaths.insert(std::pair<_str, std::set<_str>>(parent, newSet));
            prevSet = &(selectPaths.find(parent)->second);
         }
         else
         {
            it->second.insert(path);
            prevSet = &(it->second);
         }
         prevParent = parent;
      }

      isFirst = false;
   }

   if (selectPaths.empty()) {
      g_success.value = false;
   }
   else {
      _boo anyFailed = false;

      for (auto it = selectPaths.begin(); it != selectPaths.end(); it++)
      {
         if (!g_running) {
            break;
         }

         const _boo success = os_select(it->first, it->second);
         if (!anyFailed && !success) {
            anyFailed = true;
         }

         const auto end = it->second.end();
         for (auto it2 = it->second.begin(); it2 != end; ++it2) {
            if (success) {
               logSelectSuccess(*it2);
            }
            else {
               logSelectError(*it2);
            }
         }
      }

      g_success.value = !anyFailed;
   }
}
