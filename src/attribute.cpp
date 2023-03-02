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

#include "attribute.h"
#include "uroboros.h"
#include "os.h"


namespace uro
{

Attribute::Attribute(_uro& uro)
   : uroboros(uro) { };

Attribute::Attribute(const _aunit val, _uro& uro)
   : value(val), uroboros(uro) { };

void Attribute::add(const _hash& val)
{
   const Hashes& hashes = this->uroboros.hashes;

   if (hashes.HASH_GROUP_ATTR.find(val) == hashes.HASH_GROUP_ATTR.end()) {
      return;
   }

   if (this->value == ATTR_NULL) {
      this->set(ATTR_PATH);
   }

   if (val == hashes.HASH_VAR_DRIVE) {
      this->set(ATTR_DRIVE);
      return;
   }

   if (val == hashes.HASH_VAR_DEPTH) {
      this->set(ATTR_DEPTH);
      return;
   }

   if (val != hashes.HASH_VAR_PATH && val != hashes.HASH_VAR_FULLNAME) {
      this->set(ATTR_EXISTS);
   }

   if (val == hashes.HASH_VAR_ACCESS){
      this->set(ATTR_ACCESS);
   }
   else if (val == hashes.HASH_VAR_ARCHIVE)
      this->set(ATTR_ARCHIVE);
   else if (val == hashes.HASH_VAR_COMPRESSED)
      this->set(ATTR_COMPRESSED);
   else if (val == hashes.HASH_VAR_CREATION)
      this->set(ATTR_CREATION);
   else if (val == hashes.HASH_VAR_CHANGE)
      this->set(ATTR_CHANGE);
   else if (val == hashes.HASH_VAR_DRIVE)
      this->set(ATTR_DRIVE);
   else if (val == hashes.HASH_VAR_EMPTY)
      this->set(ATTR_EMPTY);
   else if (val == hashes.HASH_VAR_ENCRYPTED)
      this->set(ATTR_ENCRYPTED);
   else if (val == hashes.HASH_VAR_EXTENSION) {
      this->set(ATTR_EXTENSION);
   }
   else if (val == hashes.HASH_VAR_FULLNAME)
      this->set(ATTR_FULLNAME);
   else if (val == hashes.HASH_VAR_HIDDEN)
      this->set(ATTR_HIDDEN);
   else if (val == hashes.HASH_VAR_LIFETIME) {
      this->set(ATTR_CREATION);
      this->set(ATTR_MODIFICATION);
      this->set(ATTR_LIFETIME);
   }
   else if (val == hashes.HASH_VAR_MODIFICATION)
      this->set(ATTR_MODIFICATION);
   else if (val == hashes.HASH_VAR_NAME)
      this->set(ATTR_NAME);
   else if (val == hashes.HASH_VAR_PARENT)
      this->set(ATTR_PARENT);
   else if (val == hashes.HASH_VAR_PATH)
      this->set(ATTR_PATH);
   else if (val == hashes.HASH_VAR_READONLY)
      this->set(ATTR_READONLY);
   else if (val == hashes.HASH_VAR_SIZE)
      this->set(ATTR_SIZE);
}

inline void Attribute::set(const _aunit v)
{
   this->value |= v;
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

_bool Attribute::has(const _aunit v) const
{
   return this->value & v;
}

_bool Attribute::hasAny() const
{
   return this->value != ATTR_NULL;
}

_aunit Attribute::getValue() const
{
   return this->value;
}

_bool Attribute::isMarkedToEvaluate() const
{
   return this->markedToEvaluate;
}

void Attribute::markToEvaluate()
{
   this->markedToEvaluate = true;
}

_bool Attribute::isMarkedToRun() const
{
   return this->markedToRun;
}

void Attribute::markToRun()
{
   this->markedToRun = true;
}

}
