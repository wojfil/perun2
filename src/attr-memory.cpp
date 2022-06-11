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

   prev_trimmed = g_trimmed;
   prev_depth = g_depth.value;

   if (attribute->has(ATTR_PATH)) {
      prev_path = g_path.value;
   }

   if (attribute->has(ATTR_FULLNAME)) {
      prev_fullname = g_fullname.value;
   }

   if (attribute->has(ATTR_PARENT)) {
      prev_parent = g_parent.value;
   }

   if (attribute->has(ATTR_DRIVE)) {
      prev_drive = g_drive.value;
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   prev_exists = g_exists.value;
   prev_isfile = g_isfile.value;
   prev_isdirectory = g_isdirectory.value;

   if (attribute->has(ATTR_ACCESS)) {
      prev_access = g_access.value;
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      prev_archive = g_archive.value;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      prev_compressed = g_compressed.value;
   }

   if (attribute->has(ATTR_CHANGE)) {
      prev_change = g_change.value;
   }

   if (attribute->has(ATTR_CREATION)) {
      prev_creation = g_creation.value;
   }

   if (attribute->has(ATTR_EMPTY)) {
      prev_empty = g_empty.value;
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      prev_encrypted = g_encrypted.value;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      prev_extension = g_extension.value;
   }

   if (attribute->has(ATTR_HIDDEN)) {
      prev_hidden = g_hidden.value;
   }

   if (attribute->has(ATTR_LIFETIME)) {
      prev_lifetime = g_lifetime.value;
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      prev_modification = g_modification.value;
   }

   if (attribute->has(ATTR_NAME)) {
      prev_name = g_name.value;
   }

   if (attribute->has(ATTR_READONLY)) {
      prev_readonly = g_readonly.value;
   }

   if (attribute->has(ATTR_SIZE)) {
      prev_size = g_size.value;
   }
}


void AttributeMemory::restore() const
{
   if (!hasAttribute) {
      return;
   }

   if (attribute->markToRun) {
      os_loadAttributes(*attribute);
      return;
   }

   g_trimmed = prev_trimmed;
   g_depth.value = prev_depth;

   if (attribute->has(ATTR_PATH)) {
      g_path.value = prev_path;
   }

   if (attribute->has(ATTR_FULLNAME)) {
      g_fullname.value = prev_fullname;
   }

   if (attribute->has(ATTR_PARENT)) {
      g_parent.value = prev_parent;
   }

   if (attribute->has(ATTR_DRIVE)) {
      g_drive.value = prev_drive;
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   g_exists.value = prev_exists;
   g_isfile.value = prev_isfile;
   g_isdirectory.value = prev_isdirectory;

   if (attribute->has(ATTR_ACCESS)) {
      g_access.value = prev_access;
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      g_archive.value = prev_archive;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      g_compressed.value = prev_compressed;
   }

   if (attribute->has(ATTR_CHANGE)) {
      g_change.value = prev_change;
   }

   if (attribute->has(ATTR_CREATION)) {
      g_creation.value = prev_creation;
   }

   if (attribute->has(ATTR_EMPTY)) {
      g_empty.value - prev_empty;
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      g_encrypted.value = prev_encrypted;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      g_extension.value = prev_extension;
   }

   if (attribute->has(ATTR_HIDDEN)) {
      g_hidden.value = prev_hidden;
   }

   if (attribute->has(ATTR_LIFETIME)) {
      g_lifetime.value = prev_lifetime;
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      g_modification.value = prev_modification;
   }

   if (attribute->has(ATTR_NAME)) {
      g_name.value = prev_name;
   }

   if (attribute->has(ATTR_READONLY)) {
      g_readonly.value = prev_readonly;
   }

   if (attribute->has(ATTR_SIZE)) {
      g_size.value = prev_size;
   }
}
