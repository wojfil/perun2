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

#include "attr-memory.h"
#include "var\var-runtime.h"
#include "os.h"


void AttributeMemory::load()
{
   if (!hasAttribute || attribute->markToRun) {
      return;
   }

   prev_trimmed = inner->trimmed;
   prev_depth = inner->depth.value;

   if (attribute->has(ATTR_PATH)) {
      prev_path = inner->path.value;
   }

   if (attribute->has(ATTR_FULLNAME)) {
      prev_fullname = inner->fullname.value;
   }

   if (attribute->has(ATTR_PARENT)) {
      prev_parent = inner->parent.value;
   }

   if (attribute->has(ATTR_DRIVE)) {
      prev_drive = inner->drive.value;
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   prev_exists = inner->exists.value;
   prev_isfile = inner->isfile.value;
   prev_isdirectory = inner->isdirectory.value;

   if (attribute->has(ATTR_ACCESS)) {
      prev_access = inner->access.value;
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      prev_archive = inner->archive.value;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      prev_compressed = inner->compressed.value;
   }

   if (attribute->has(ATTR_CHANGE)) {
      prev_change = inner->change.value;
   }

   if (attribute->has(ATTR_CREATION)) {
      prev_creation = inner->creation.value;
   }

   if (attribute->has(ATTR_EMPTY)) {
      prev_empty = inner->empty.value;
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      prev_encrypted = inner->encrypted.value;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      prev_extension = inner->extension.value;
   }

   if (attribute->has(ATTR_HIDDEN)) {
      prev_hidden = inner->hidden.value;
   }

   if (attribute->has(ATTR_LIFETIME)) {
      prev_lifetime = inner->lifetime.value;
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      prev_modification = inner->modification.value;
   }

   if (attribute->has(ATTR_NAME)) {
      prev_name = inner->name.value;
   }

   if (attribute->has(ATTR_READONLY)) {
      prev_readonly = inner->readonly.value;
   }

   if (attribute->has(ATTR_SIZE)) {
      prev_size = inner->size.value;
   }
}


void AttributeMemory::restore() const
{
   if (!hasAttribute) {
      return;
   }

   if (attribute->markToRun) {
      this->attribute->run();
      return;
   }

   inner->trimmed = prev_trimmed;
   inner->depth.value = prev_depth;

   if (attribute->has(ATTR_PATH)) {
      inner->path.value = prev_path;
   }

   if (attribute->has(ATTR_FULLNAME)) {
      inner->fullname.value = prev_fullname;
   }

   if (attribute->has(ATTR_PARENT)) {
      inner->parent.value = prev_parent;
   }

   if (attribute->has(ATTR_DRIVE)) {
      inner->drive.value = prev_drive;
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   inner->exists.value = prev_exists;
   inner->isfile.value = prev_isfile;
   inner->isdirectory.value = prev_isdirectory;

   if (attribute->has(ATTR_ACCESS)) {
      inner->access.value = prev_access;
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      inner->archive.value = prev_archive;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      inner->compressed.value = prev_compressed;
   }

   if (attribute->has(ATTR_CHANGE)) {
      inner->change.value = prev_change;
   }

   if (attribute->has(ATTR_CREATION)) {
      inner->creation.value = prev_creation;
   }

   if (attribute->has(ATTR_EMPTY)) {
      inner->empty.value - prev_empty;
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      inner->encrypted.value = prev_encrypted;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      inner->extension.value = prev_extension;
   }

   if (attribute->has(ATTR_HIDDEN)) {
      inner->hidden.value = prev_hidden;
   }

   if (attribute->has(ATTR_LIFETIME)) {
      inner->lifetime.value = prev_lifetime;
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      inner->modification.value = prev_modification;
   }

   if (attribute->has(ATTR_NAME)) {
      inner->name.value = prev_name;
   }

   if (attribute->has(ATTR_READONLY)) {
      inner->readonly.value = prev_readonly;
   }

   if (attribute->has(ATTR_SIZE)) {
      inner->size.value = prev_size;
   }
}
