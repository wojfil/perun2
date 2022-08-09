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

#include "attribute.h"
#include "uroboros.h"
#include "os.h"

Attribute::Attribute(Uroboros* uro)
   : value(ATTR_NULL), uroboros(uro) { };

Attribute::Attribute(const _aunit& val, Uroboros* uro)
   : value(val), uroboros(uro) { };

void Attribute::add(const Token& tk)
{
   const _size& h = tk.value.word.h;
   const Hashes& hashes = this->uroboros->hashes;

   if (hashes.HASH_GROUP_ATTR.find(h) == hashes.HASH_GROUP_ATTR.end()) {
      return;
   }

   if (value == ATTR_NULL) {
      set(ATTR_PATH);
   }

   if (h == hashes.HASH_VAR_DRIVE) {
      set(ATTR_DRIVE);
      return;
   }

   if (h != hashes.HASH_VAR_PATH && h != hashes.HASH_VAR_FULLNAME) {
      set(ATTR_EXISTS);
   }

   if (h == hashes.HASH_VAR_ACCESS){
      set(ATTR_ACCESS);
   }
   else if (h == hashes.HASH_VAR_ARCHIVE)
      set(ATTR_ARCHIVE);
   else if (h == hashes.HASH_VAR_COMPRESSED)
      set(ATTR_COMPRESSED);
   else if (h == hashes.HASH_VAR_CREATION)
      set(ATTR_CREATION);
   else if (h == hashes.HASH_VAR_CHANGE)
      set(ATTR_CHANGE);
   else if (h == hashes.HASH_VAR_DRIVE)
      set(ATTR_DRIVE);
   else if (h == hashes.HASH_VAR_EMPTY)
      set(ATTR_EMPTY);
   else if (h == hashes.HASH_VAR_ENCRYPTED)
      set(ATTR_ENCRYPTED);
   else if (h == hashes.HASH_VAR_EXTENSION) {
      set(ATTR_EXTENSION);
   }
   else if (h == hashes.HASH_VAR_FULLNAME)
      set(ATTR_FULLNAME);
   else if (h == hashes.HASH_VAR_HIDDEN)
      set(ATTR_HIDDEN);
   else if (h == hashes.HASH_VAR_LIFETIME) {
      set(ATTR_CREATION);
      set(ATTR_MODIFICATION);
      set(ATTR_LIFETIME);
   }
   else if (h == hashes.HASH_VAR_MODIFICATION)
      set(ATTR_MODIFICATION);
   else if (h == hashes.HASH_VAR_NAME)
      set(ATTR_NAME);
   else if (h == hashes.HASH_VAR_PARENT)
      set(ATTR_PARENT);
   else if (h == hashes.HASH_VAR_PATH)
      set(ATTR_PATH);
   else if (h == hashes.HASH_VAR_READONLY)
      set(ATTR_READONLY);
   else if (h == hashes.HASH_VAR_SIZE)
      set(ATTR_SIZE);
}

void Attribute::set(const _aunit& v)
{
   value |= v;
}

void Attribute::setCoreCommandBase()
{
   this->set(ATTR_EXISTS);
   this->set(ATTR_FULLNAME);
   this->set(ATTR_PATH);
}

void Attribute::setTimeCommandBase()
{
   this->set(ATTR_EXISTS);
   this->set(ATTR_FULLNAME);
   this->set(ATTR_PATH);
   this->set(ATTR_ACCESS);
   this->set(ATTR_CREATION);
   this->set(ATTR_MODIFICATION);
   this->set(ATTR_CHANGE);
}

_boo Attribute::has(const _aunit& v) const
{
   return value & v;
}

_boo Attribute::hasAny() const
{
   return value != ATTR_NULL;
}

_aunit Attribute::getValue() const
{
   return value;
}

_boo Attribute::isMarkedToEvaluate() const
{
   return isMarkedToEvaluate_;
}

void Attribute::markToEvaluate()
{
   isMarkedToEvaluate_ = true;
}

void Attribute::run() const
{
   os_loadAttributes(this, this->uroboros);
}

BridgeAttribute::BridgeAttribute(const _aunit& val, Uroboros* uro, WIN32_FIND_DATAW* data)
   : Attribute(val, uro), dataPnt(data) { };

void BridgeAttribute::run() const
{
   os_loadDataAttributes(this, this->uroboros, this->dataPnt);
}
