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

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "gen-os.h"
#include "gen-string.h"
#include "../../uroboros.h"
#include "../../os.h"


namespace uro::gen
{

#define P_GEN_OS_ARGS_DEFAULT loc, this->uroboros, gen::os::IS_RELATIVE_PATH, gen::os::NO_PREFIX
#define P_GEN_OS_ARGS_DEFAULT_EXT loc, this->uroboros, gen::os::DEFAULT_PATTERN, gen::os::IS_RELATIVE_PATH, gen::os::NO_PREFIX

#define P_OS_GEN_VALUE_ALTERATION if (this->isAbsolute) { \
      this->value = this->hasPrefix \
         ? str(this->prefix, this->baseLocation, OS_SEPARATOR_STRING, this->value) \
         : str(this->baseLocation, OS_SEPARATOR_STRING, this->value); \
      } \
   else if (this->hasPrefix) { \
      this->value = str(this->prefix, this->value); \
   } \


DefinitionGenerator::DefinitionGenerator(const OsElement& el, _uro& uro)
   : element_(el), uroboros(uro) { };

_bool DefinitionGenerator::generate(_defptr& result) const
{
   _genptr<_str> loc(new LocationReference(this->uroboros));

   switch (this->element_) {
      case OsElement::oe_All: {
         result = std::make_unique<All>(P_GEN_OS_ARGS_DEFAULT_EXT);
         break;
      }
      case OsElement::oe_Directories: {
         result = std::make_unique<Directories>(P_GEN_OS_ARGS_DEFAULT_EXT);
         break;
      }
      case OsElement::oe_Files: {
         result = std::make_unique<Files>(P_GEN_OS_ARGS_DEFAULT_EXT);
         break;
      }
      case OsElement::oe_RecursiveFiles: {
         result = std::make_unique<RecursiveFiles>(P_GEN_OS_ARGS_DEFAULT);
         break;
      }
      case OsElement::oe_RecursiveDirectories: {
         result = std::make_unique<RecursiveDirectories>(P_GEN_OS_ARGS_DEFAULT);
         break;
      }
      case OsElement::oe_RecursiveAll: {
         result = std::make_unique<RecursiveAll>(P_GEN_OS_ARGS_DEFAULT);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

OsDefinition::OsDefinition(P_GEN_OS_ARGS)
   : location(std::move(loc)), uroboros(uro), inner(uro.vars.inner), 
     flags(uro.flags), isAbsolute(abs),
     hasPrefix(!pref.empty()), prefix(pref) { };


_fdata* OsDefinition::getDataPtr()
{
   return &data;
}

void OsDefinitionPlain::reset()
{
   if (!first) {
      first = true;
      FindClose(handle);
      P_MEMORY_RESTORE;
   }
}

void OsDefinitionRecursive::reset()
{
   if (!first) {
      first = true;
      paths.clear();
      bases.clear();
      const _size len = handles.size();
      if (len != 0) {
         for (_size i = 0; i < len; i++) {
            FindClose(handles[i]);
         }
         handles.clear();
      }
      P_MEMORY_RESTORE;
   }
}

_bool All::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      if (os_directoryExists(this->baseLocation)) {
         const _str path = str(this->baseLocation, pattern);
         handle = FindFirstFile(path.c_str(), &data);
         if (handle == INVALID_HANDLE_VALUE) {
            return false;
         }

         first = false;
         value = data.cFileName;
         P_MEMORY_LOAD;
         this->inner.depth.value.setToZero();
         index.setToZero();
         this->inner.index.value = index;

         if (!os_isBrowsePath(value)) {
            const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            if ((this->flags & FLAG_NOOMIT) || (isDir && value != OS_GIT_DIRECTORY)
               || (!isDir && os_extension(value) != OS_UROEXT))
            {
               this->inner.index.value = index;
               index++;
               this->inner.depth.value.setToZero();

               P_OS_GEN_VALUE_ALTERATION;

               this->inner.this_s.value = value;
               return true;
            }
         }
      }
      else {
         return false;
      }
   }

   while (FindNextFile(handle, &data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         if ((this->flags & FLAG_NOOMIT) || (isDir && value != OS_GIT_DIRECTORY)
            || (!isDir && os_extension(value) != OS_UROEXT))
         {
            this->inner.index.value = index;
            index++;
            this->inner.depth.value.setToZero();

            P_OS_GEN_VALUE_ALTERATION;

            this->inner.this_s.value = value;
            return true;
         }
      }
   }

   first = true;
   FindClose(handle);
   P_MEMORY_RESTORE;
   return false;
}

_bool Files::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      if (os_directoryExists(this->baseLocation)) {
         const _str path = str(this->baseLocation, pattern);
         handle = FindFirstFile(path.c_str(), &data);
         if (handle == INVALID_HANDLE_VALUE) {
            return false;
         }

         first = false;
         value = data.cFileName;
         P_MEMORY_LOAD;
         this->inner.depth.value.setToZero();
         index.setToZero();
         this->inner.index.value = index;

         if (!os_isBrowsePath(value)) {
            const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            if (!isDir && ((this->flags & FLAG_NOOMIT) || os_extension(value) != OS_UROEXT)) {
               this->inner.index.value = index;
               index++;
               this->inner.depth.value.setToZero();

               P_OS_GEN_VALUE_ALTERATION;

               this->inner.this_s.value = value;
               return true;
            }
         }
      }
      else {
         return false;
      }
   }

   while (FindNextFile(handle, &data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         if (!isDir && ((this->flags & FLAG_NOOMIT) || os_extension(value) != OS_UROEXT)) {
            this->inner.index.value = index;
            index++;
            this->inner.depth.value.setToZero();

            P_OS_GEN_VALUE_ALTERATION;

            this->inner.this_s.value = value;
            return true;
         }
      }
   }

   first = true;
   FindClose(handle);
   P_MEMORY_RESTORE;
   return false;
}

_bool Directories::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      if (os_directoryExists(this->baseLocation)) {
         const _str path = str(this->baseLocation, pattern);
         handle = FindFirstFile(path.c_str(), &data);
         if (handle == INVALID_HANDLE_VALUE) {
            return false;
         }

         first = false;
         value = data.cFileName;
         P_MEMORY_LOAD;
         this->inner.depth.value.setToZero();
         index.setToZero();
         this->inner.index.value = index;

         if (!os_isBrowsePath(value)) {
            const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

            if (isDir && ((this->flags & FLAG_NOOMIT) || value != OS_GIT_DIRECTORY)) {
               this->inner.index.value = index;
               index++;
               this->inner.depth.value.setToZero();

               P_OS_GEN_VALUE_ALTERATION;

               this->inner.this_s.value = value;
               return true;
            }
         }
      }
      else {
         return false;
      }
   }

   while (FindNextFile(handle, &data)) {
      value = data.cFileName;

      if (!os_isBrowsePath(value)) {
         const _bool isDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

         if (isDir && ((this->flags & FLAG_NOOMIT) || value != OS_GIT_DIRECTORY)) {
            this->inner.index.value = index;
            index++;
            this->inner.depth.value.setToZero();

            P_OS_GEN_VALUE_ALTERATION;

            this->inner.this_s.value = value;
            return true;
         }
      }
   }

   first = true;
   FindClose(handle);
   P_MEMORY_RESTORE;
   return false;
}


void OsDefinitionRecursive::setDepth()
{
   this->inner.depth.value = this->depth;
}

_bool RecursiveFiles::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      P_MEMORY_LOAD;
      this->depth.setToZero();
      this->setDepth();
      goDeeper = true;
      first = false;
      index.setToZero();
      this->inner.index.value = index;
   }

   while (this->uroboros.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), gen::os::DEFAULT_PATTERN);
            handles.emplace_back(FindFirstFile(p.c_str(), &data));

            if (handles.back() == INVALID_HANDLE_VALUE)
            {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
                  this->depth--;
                  this->setDepth();
               }
            }
            else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
               const _str& v = data.cFileName;

               if ((this->flags & FLAG_NOOMIT) || os_extension(v) != OS_UROEXT) {
                  value = v;
                  this->inner.index.value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->inner.this_s.value = value;
                  return true;
               }
            }
         }
         else {
            paths.pop_back();
            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               this->depth--;
               this->setDepth();
            }
         }
      }
      else {
         if (FindNextFile(handles.back(), &data)) {
            const _str& v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  if ((this->flags & FLAG_NOOMIT) || v != OS_GIT_DIRECTORY) {
                     paths.emplace_back(str(paths.back(), OS_SEPARATOR_STRING, v));

                     if (this->depth.isZero()) {
                        bases.emplace_back(str(v, OS_SEPARATOR_STRING));
                     }
                     else {
                        bases.emplace_back(str(bases.back(), v, OS_SEPARATOR_STRING));
                     }

                     goDeeper = true;
                     this->depth++;
                     this->setDepth();
                  }
               }
               else  if ((this->flags & FLAG_NOOMIT) || os_extension(v) != OS_UROEXT) {
                  value = this->depth.isZero() ? v : str(bases.back(), v);
                  this->inner.index.value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->inner.this_s.value = value;
                  return true;
               }
            }
         }
         else {
            FindClose(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               this->depth--;
               this->setDepth();
            }
         }
      }
   }

   reset();
   return false;
}

_bool RecursiveDirectories::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      P_MEMORY_LOAD;
      this->depth.setToMinusOne();
      this->setDepth();
      goDeeper = true;
      first = false;
      index.setToZero();
      this->inner.index.value = index;
   }

   while (this->uroboros.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), gen::os::DEFAULT_PATTERN);
            handles.emplace_back(FindFirstFile(p.c_str(), &data));

            if (handles.back() == INVALID_HANDLE_VALUE)
            {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
                  this->depth--;
                  this->setDepth();
               }
            }
         }
         else {
            paths.pop_back();
            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               this->depth--;
               this->setDepth();
            }
         }
      }
      else {
         if (FindNextFile(handles.back(), &data)) {
            const _str& v = data.cFileName;

            if (!os_isBrowsePath(v) && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                && ((this->flags & FLAG_NOOMIT) || v != OS_GIT_DIRECTORY))
            {
               const _bool isBase = this->depth.isMinusOne();
               value = isBase ? v : str(bases.back(), v);
               paths.emplace_back(str(paths.back(), OS_SEPARATOR_STRING, v));

               if (isBase) {
                  bases.emplace_back(str(v, OS_SEPARATOR_STRING));
               }
               else {
                  bases.emplace_back(str(bases.back(), v, OS_SEPARATOR_STRING));
               }

               goDeeper = true;
               this->depth++;
               this->setDepth();
               this->inner.index.value = index;
               index++;

               P_OS_GEN_VALUE_ALTERATION;

               this->inner.this_s.value = value;
               return true;
            }
         }
         else {
            FindClose(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               this->depth--;
               this->setDepth();
            }
         }
      }
   }

   reset();
   return false;
}

_bool RecursiveAll::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      P_MEMORY_LOAD;
      this->depth.setToMinusOne();
      this->setDepth();
      goDeeper = true;
      first = false;
      index.setToZero();
      this->inner.index.value = index;
   }

   while (this->uroboros.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), gen::os::DEFAULT_PATTERN);
            handles.emplace_back(FindFirstFile(p.c_str(), &data));

            if (handles.back() == INVALID_HANDLE_VALUE)
            {
               handles.pop_back();
               paths.pop_back();
               if (paths.empty()) {
                  break;
               }
               else {
                  bases.pop_back();
                  this->depth--;
                  this->setDepth();
               }
            }
         }
         else {
            paths.pop_back();
            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               this->depth--;
               this->setDepth();
            }
         }
      }
      else {
         if (FindNextFile(handles.back(), &data)) {
            const _str& v = data.cFileName;

            if (!os_isBrowsePath(v)) {
               if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                  && ((this->flags & FLAG_NOOMIT) || v != OS_GIT_DIRECTORY))
               {
                  if (this->prevFile) {
                     this->prevFile = false;
                     this->depth--;
                     this->setDepth();
                  }

                  const _bool isBase = this->depth.isMinusOne();

                  value = isBase ? v : str(bases.back(), v);
                  paths.emplace_back(str(paths.back(), OS_SEPARATOR_STRING, v));

                  if (isBase) {
                     bases.emplace_back(str(v, OS_SEPARATOR_STRING));
                  }
                  else {
                     bases.emplace_back(str(bases.back(), v, OS_SEPARATOR_STRING));
                  }

                  goDeeper = true;
                  this->depth++;
                  this->setDepth();
                  this->inner.index.value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->inner.this_s.value = value;
                  return true;
               }
               else if ((!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                  && (this->flags & FLAG_NOOMIT) || os_extension(v) != OS_UROEXT)
               {
                  if (!this->prevFile) {
                     this->prevFile = true;
                     this->depth++;
                     this->setDepth();
                  }

                  const _bool isBase = this->depth.isZero();
                  value = isBase ? v : str(bases.back(), v);
                  this->inner.index.value = index;
                  index++;

                  P_OS_GEN_VALUE_ALTERATION;

                  this->inner.this_s.value = value;
                  return true;
               }
            }
         }
         else {
            FindClose(handles.back());
            handles.pop_back();
            paths.pop_back();

            if (paths.empty()) {
               break;
            }
            else {
               bases.pop_back();
               this->depth--;
               this->setDepth();
            }
         }
      }
   }

   reset();
   return false;
}

}
