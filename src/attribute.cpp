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

#include "attribute.h"
#include "perun2.h"
#include "os.h"


namespace perun2
{

Attribute::Attribute(p_perun2& p2)
   : perun2(p2) { };

Attribute::Attribute(const p_aunit val, p_perun2& p2)
   : value(val), perun2(p2) { };

void Attribute::add(const Token& tk)
{
   if (!tk.isWord(STRINGS_ATTR, this->perun2)) {
      return;
   }

   if (this->value == ATTR_NULL) {
      this->set(ATTR_PATH);
   }

   if (tk.isWord(STRING_DRIVE, this->perun2)) {
      this->set(ATTR_DRIVE);
      return;
   }

   if (tk.isWord(STRING_DEPTH, this->perun2)) {
      this->set(ATTR_DEPTH);
      return;
   }

   if (!tk.isWord(STRING_PATH, this->perun2) && !tk.isWord(STRING_FULLNAME, this->perun2)) {
      this->set(ATTR_EXISTS);
   }
   if (tk.isWord(STRING_ACCESS, this->perun2)) {
      this->set(ATTR_ACCESS);
   }
   else if (tk.isWord(STRING_ARCHIVE, this->perun2)) {
      this->set(ATTR_ARCHIVE);
   }
   else if (tk.isWord(STRING_COMPRESSED, this->perun2)) {
      this->set(ATTR_COMPRESSED);
   }
   else if (tk.isWord(STRING_CREATION, this->perun2)) {
      this->set(ATTR_CREATION);
   }
   else if (tk.isWord(STRING_CHANGE, this->perun2)) {
      this->set(ATTR_CHANGE);
   }
   else if (tk.isWord(STRING_DRIVE, this->perun2)) {
      this->set(ATTR_DRIVE);
   }
   else if (tk.isWord(STRING_EMPTY, this->perun2)) {
      this->set(ATTR_EMPTY);
   }
   else if (tk.isWord(STRING_ENCRYPTED, this->perun2)) {
      this->set(ATTR_ENCRYPTED);
   }
   else if (tk.isWord(STRING_EXTENSION, this->perun2)) {
      this->set(ATTR_EXTENSION);
   }
   else if (tk.isWord(STRING_FULLNAME, this->perun2)) {
      this->set(ATTR_FULLNAME);
   }
   else if (tk.isWord(STRING_HIDDEN, this->perun2)) {
      this->set(ATTR_HIDDEN);
   }
   else if (tk.isWord(STRING_LIFETIME, this->perun2)) {
      this->set(ATTR_CREATION);
      this->set(ATTR_MODIFICATION);
      this->set(ATTR_LIFETIME);
   }
   else if (tk.isWord(STRING_MODIFICATION, this->perun2)) {
      this->set(ATTR_MODIFICATION);
   }
   else if (tk.isWord(STRING_NAME, this->perun2)) {
      this->set(ATTR_NAME);
   }
   else if (tk.isWord(STRING_PARENT, this->perun2)) {
      this->set(ATTR_PARENT);
   }
   else if (tk.isWord(STRING_PATH, this->perun2)) {
      this->set(ATTR_PATH);
   }
   else if (tk.isWord(STRING_READONLY, this->perun2)) {
      this->set(ATTR_READONLY);
   }
   else if (tk.isWord(STRING_SIZE, this->perun2)) {
      this->set(ATTR_SIZE);
   }
}

void Attribute::set(const p_aunit v)
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

p_bool Attribute::has(const p_aunit v) const
{
   return this->value & v;
}

p_bool Attribute::hasAny() const
{
   return this->value != ATTR_NULL;
}

p_aunit Attribute::getValue() const
{
   return this->value;
}

}
