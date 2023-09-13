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
#include "../logger.h"


namespace perun2::prog
{


RegistryIterator::RegistryIterator(const RegistryRootType type, p_genptr<p_str>& rt)
   : rootType(type), root(std::move(rt)) { };


p_str RegistryIterator::getValue()
{
   return this->value;
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


MultiRegistry::MultiRegistry(const RegistryRootType type, p_genptr<p_str>& rt, const p_str& pattern)
   : RegistryIterator(type, rt), comparer(pattern) { };


void MultiRegistry::reset()
{
   if (! this->first) {
      this->first = true;
      RegCloseKey(this->key);
   }
}


p_bool MultiRegistry::hasNext()
{
   if (this->first) {
      this->result = RegOpenKeyExW(this->getRootKey(), this->root->getValue().c_str(), 0, KEY_READ, &this->key);

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







}
