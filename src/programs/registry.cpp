/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"
#include "../datatype/concat.h"
#include "../datatype/strings.h"


namespace perun2::prog
{


RegistryIterator::RegistryIterator(const RegistryRootType type)
   : rootType(type) { };


p_str RegistryIterator::getRegistryValue(const p_str& name) const
{
   HKEY hKey;
   LONG result = RegOpenKeyExW(this->getRootKey(), this->value.c_str(), 0, KEY_READ, &hKey);

   if (result == ERROR_SUCCESS) {
      p_char buffer[MAX_PATH];
      DWORD bufferSize = sizeof(buffer);
      result = RegQueryValueExW(hKey, name.c_str(), nullptr, nullptr, reinterpret_cast<BYTE*>(buffer), &bufferSize);

      if (result == ERROR_SUCCESS) {
         return buffer;
      } 
      else {
         return EMPTY_STRING;
      }

      RegCloseKey(hKey);
   } 
    
   return EMPTY_STRING;
}


p_str RegistryIterator::getValue()
{
   return this->value;
}


p_bool RegistryIterator::hasEmptyValue() const
{
   return this->value.empty();
}


HKEY RegistryIterator::getRootKey() const
{
   switch (this->rootType) {
      case RegistryRootType::ClassesRoot: {
         return HKEY_CLASSES_ROOT;
      }
      case RegistryRootType::CurrentUser: {
         return HKEY_CURRENT_USER;
      }
      default: { // case RegistryRootType::LocalMachine
         return HKEY_LOCAL_MACHINE;
      }
   }
}


MultiRegistryRoot::MultiRegistryRoot(const RegistryRootType type, const p_str& pattern)
   : RegistryIterator(type), comparer(pattern) { };


void MultiRegistryRoot::reset()
{
   if (! this->first) {
      this->first = true;
      RegCloseKey(this->key);
   }
}


p_bool MultiRegistryRoot::hasNext()
{
   if (this->first) {
      this->result = RegOpenKeyExW(this->getRootKey(), NULL, 0, KEY_READ, &this->key);

      if (this->result != ERROR_SUCCESS) {
         return false;
      }

      this->first = false;
   }

   while (true) {
      this->result = RegEnumKeyExW(this->key, this->index, this->subkeyName, &this->subkeyNameSize, NULL, NULL, NULL, NULL);

      if (this->result != ERROR_SUCCESS) {
         this->reset();
         return false;
      }

      this->value = this->subkeyName;
      this->subkeyNameSize = MAX_PATH;
      this->index++;

      if (this->comparer.matches(this->value)) {
         return true;
      }
   }

   this->reset();
   return false;
}


SingleRegistryRoot::SingleRegistryRoot(const RegistryRootType type, const p_str& rt)
   : RegistryIterator(type), root(rt) { };


void SingleRegistryRoot::reset()
{
   this->taken = false;
}


p_bool SingleRegistryRoot::hasNext()
{
   if (this->taken) {
      this->reset();
      return false;
   }

   this->result = RegOpenKeyExW(this->getRootKey(), this->root.empty() ? NULL : this->root.c_str(), 0, KEY_READ, &this->key);

   if (this->result != ERROR_SUCCESS) {
      return false;
   }

   this->taken = true;
   this->value = this->root;
   RegCloseKey(this->key);
   return true;
}


MultiRegistry::MultiRegistry(const RegistryRootType type, p_riptr& prev, const p_str& pattern)
   : RegistryIterator(type), previous(std::move(prev)), comparer(pattern) { };


void MultiRegistry::reset()
{
   // do nothin
}


p_bool MultiRegistry::hasNext()
{
   while (true) {
      if (this->exploreRoot) {
         if (!this->previous->hasNext()) {
            this->reset();
            return false;
         }

         this->result = RegOpenKeyExW(this->getRootKey(), this->previous->getValue().c_str(), 0, KEY_READ, &this->key);

         if (this->result == ERROR_SUCCESS) {
            this->exploreRoot = false;
         }
         else {
            continue;
         }
      }

      this->result = RegEnumKeyExW(this->key, this->index, this->subkeyName, &this->subkeyNameSize, NULL, NULL, NULL, NULL);

      if (this->result == ERROR_SUCCESS) {
         this->value = this->subkeyName;
         this->subkeyNameSize = MAX_PATH;
         this->index++;

         if (this->comparer.matches(this->value)) {
            const p_str v = this->previous->hasEmptyValue() 
               ? this->subkeyName 
               : str(this->previous->getValue(), CHAR_BACKSLASH, this->subkeyName);

            this->value = v;
            return true;
         }
      }
      else {
         this->index = 0;
         RegCloseKey(this->key);
         this->exploreRoot = true;
      }
   }
}


SingleRegistry::SingleRegistry(const RegistryRootType type, p_riptr& prev, const p_str& segm)
   : RegistryIterator(type), previous(std::move(prev)), segment(segm) { };


void SingleRegistry::reset()
{
   // do nothin
}


p_bool SingleRegistry::hasNext()
{
   while (this->previous->hasNext()) {
      const p_str v = this->previous->hasEmptyValue() 
         ? this->segment 
         : str(this->previous->getValue(), CHAR_BACKSLASH, this->segment);

      this->result = RegOpenKeyExW(this->getRootKey(), v.c_str(), 0, KEY_READ, &this->key);

      if (this->result == ERROR_SUCCESS) {
         this->value = v;
         RegCloseKey(this->key);
         return true;
      }
   }

   this->reset();
   return false;
}


}
