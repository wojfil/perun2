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

#define P_OS_GEN_CORE_ARGS location, this->uroboros, pattern, isAbsolute, prefix

#define P_OS_GEN_VALUE_ALTERATION if (this->isAbsolute) { \
      this->value = this->hasPrefix \
         ? str(this->prefix, this->baseLocation, OS_SEPARATOR_STRING, this->value) \
         : str(this->baseLocation, OS_SEPARATOR_STRING, this->value); \
      } \
   else if (this->hasPrefix) { \
      this->value = str(this->prefix, this->value); \
   } \


DefinitionGenerator::DefinitionGenerator(const OsElement& el, Uroboros& uro)
   : element_(el), uroboros(uro) { };

_bool DefinitionGenerator::generateDefault(_defptr& result) const
{
   _genptr<_str> loc(new LocationReference(this->uroboros));
   return this->generate(result, loc, this->element_, OS_SEPARATOR_ASTERISK, false, EMPTY_STRING);
}

_bool DefinitionGenerator::generate(_defptr& result, _genptr<_str>& location, const OsElement& element,
   const _str& pattern, const _bool& isAbsolute, const _str& prefix) const
{
   switch (element) {
      case OsElement::oe_All: {
         result = std::make_unique<Uro_All>(P_OS_GEN_CORE_ARGS);
         break;
      }
      case OsElement::oe_Directories: {
         result = std::make_unique<Uro_Directories>(P_OS_GEN_CORE_ARGS);
         break;
      }
      case OsElement::oe_Files: {
         result = std::make_unique<Uro_Files>(P_OS_GEN_CORE_ARGS);
         break;
      }
      case OsElement::oe_RecursiveFiles: {
         result = std::make_unique<Uro_RecursiveFiles>(P_OS_GEN_CORE_ARGS);
         break;
      }
      case OsElement::oe_RecursiveDirectories: {
         result = std::make_unique<Uro_RecursiveDirectories>(P_OS_GEN_CORE_ARGS);
         break;
      }
      case OsElement::oe_RecursiveAll: {
         result = std::make_unique<Uro_RecursiveAll>(P_OS_GEN_CORE_ARGS);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}

OsDefinition::OsDefinition(P_OS_GEN_ARGS)
   : first(true), location(std::move(loc)), uroboros(uro),
     inner(uro.vars.inner), flags(uro.flags), pattern(patt), isAbsolute(abs),
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

_bool Uro_All::hasNext()
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

_bool Uro_Files::hasNext()
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

_bool Uro_Directories::hasNext()
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

_bool Uro_RecursiveFiles::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      P_MEMORY_LOAD;
      this->inner.depth.value.setToZero();
      goDeeper = true;
      first = false;
      index.setToZero();
      this->inner.index.value = index;
   }

   while (this->uroboros.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), pattern);
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
                  this->inner.depth.value--;
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
               this->inner.depth.value--;
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

                     if (this->inner.depth.value.isZero()) {
                        bases.emplace_back(str(v, OS_SEPARATOR_STRING));
                     }
                     else {
                        bases.emplace_back(str(bases.back(), v, OS_SEPARATOR_STRING));
                     }

                     goDeeper = true;
                     this->inner.depth.value++;
                  }
               }
               else  if ((this->flags & FLAG_NOOMIT) || os_extension(v) != OS_UROEXT) {
                  value = this->inner.depth.value.isZero() ? v : str(bases.back(), v);
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
               this->inner.depth.value--;
            }
         }
      }
   }

   reset();
   return false;
}

_bool Uro_RecursiveDirectories::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      P_MEMORY_LOAD;
      this->inner.depth.value.setToMinusOne();
      goDeeper = true;
      first = false;
      index.setToZero();
      this->inner.index.value = index;
   }

   while (this->uroboros.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), pattern);
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
                  this->inner.depth.value--;
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
               this->inner.depth.value--;
            }
         }
      }
      else {
         if (FindNextFile(handles.back(), &data)) {
            const _str& v = data.cFileName;

            if (!os_isBrowsePath(v) && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                && ((this->flags & FLAG_NOOMIT) || v != OS_GIT_DIRECTORY))
            {
               value = this->inner.depth.value.isMinusOne() ? v : str(bases.back(), v);
               paths.emplace_back(str(paths.back(), OS_SEPARATOR_STRING, v));

               if (this->inner.depth.value.isMinusOne()) {
                  bases.emplace_back(str(v, OS_SEPARATOR_STRING));
               }
               else {
                  bases.emplace_back(str(bases.back(), v, OS_SEPARATOR_STRING));
               }

               goDeeper = true;
               this->inner.depth.value++;
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
               this->inner.depth.value--;
            }
         }
      }
   }

   reset();
   return false;
}

_bool Uro_RecursiveAll::hasNext()
{
   if (first) {
      this->baseLocation = os_trim(location->getValue());
      this->paths.emplace_back(this->baseLocation);
      P_MEMORY_LOAD;
      this->inner.depth.value.setToMinusOne();
      goDeeper = true;
      first = false;
      index.setToZero();
      this->inner.index.value = index;
   }

   while (this->uroboros.state == State::s_Running) {
      if (goDeeper) {
         goDeeper = false;
         if (os_directoryExists(paths.back())) {
            const _str p = str(paths.back(), pattern);
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
                  this->inner.depth.value--;
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
               this->inner.depth.value--;
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
                     this->inner.depth.value--;
                  }

                  value = this->inner.depth.value.isMinusOne() ? v : str(bases.back(), v);
                  paths.emplace_back(str(paths.back(), OS_SEPARATOR_STRING, v));

                  if (this->inner.depth.value.isMinusOne()) {
                     bases.emplace_back(str(v, OS_SEPARATOR_STRING));
                  }
                  else {
                     bases.emplace_back(str(bases.back(), v, OS_SEPARATOR_STRING));
                  }

                  goDeeper = true;
                  this->inner.depth.value++;
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
                     this->inner.depth.value++;
                  }

                  value = this->inner.depth.value.isZero() ? v : str(bases.back(), v);
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
               this->inner.depth.value--;
            }
         }
      }
   }

   reset();
   return false;
}

}
