/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "attribute.hpp"
#include "perun2.hpp"
#include "os/os.hpp"


namespace perun2
{

Attribute::Attribute() 
   : value(ATTR_NULL) { };

Attribute::Attribute(const p_aunit val) 
   : value(val) { };

void Attribute::add(const Token& tk)
{
   if (!tk.isVariable(STRINGS_ATTR)) {
      return;
   }

   if (this->value == ATTR_NULL) {
      this->set(ATTR_PATH);
   }

   if (tk.isVariable(STRING_DRIVE)) {
      this->set(ATTR_DRIVE);
      return;
   }

   if (tk.isVariable(STRING_DEPTH)) {
      this->set(ATTR_DEPTH);
      return;
   }

   if (tk.isVariable(STRING_FULLNAME)) {
      this->set(ATTR_FULLNAME);
      return;
   }

   if (tk.isVariable(STRING_PATH)) {
      return;
   }

   this->set(ATTR_EXISTS);
   
   if (tk.isVariable(STRING_ACCESS)) {
      this->set(ATTR_ACCESS);
   }
   else if (tk.isVariable(STRING_ARCHIVE)) {
      this->set(ATTR_ARCHIVE);
   }
   else if (tk.isVariable(STRING_COMPRESSED)) {
      this->set(ATTR_COMPRESSED);
   }
   else if (tk.isVariable(STRING_CREATION)) {
      this->set(ATTR_CREATION);
   }
   else if (tk.isVariable(STRING_CHANGE)) {
      this->set(ATTR_CHANGE);
   }
   else if (tk.isVariable(STRING_DRIVE)) {
      this->set(ATTR_DRIVE);
   }
   else if (tk.isVariable(STRING_EMPTY)) {
      this->set(ATTR_EMPTY);
   }
   else if (tk.isVariable(STRING_ENCRYPTED)) {
      this->set(ATTR_ENCRYPTED);
   }
   else if (tk.isVariable(STRING_EXTENSION)) {
      this->set(ATTR_EXTENSION);
   }
   else if (tk.isVariable(STRING_HIDDEN)) {
      this->set(ATTR_HIDDEN);
   }
   else if (tk.isVariable(STRING_LIFETIME)) {
      this->set(ATTR_CREATION);
      this->set(ATTR_MODIFICATION);
      this->set(ATTR_LIFETIME);
   }
   else if (tk.isVariable(STRING_MODIFICATION)) {
      this->set(ATTR_MODIFICATION);
   }
   else if (tk.isVariable(STRING_NAME)) {
      this->set(ATTR_NAME);
   }
   else if (tk.isVariable(STRING_PARENT)) {
      this->set(ATTR_PARENT);
   }
   else if (tk.isVariable(STRING_READONLY)) {
      this->set(ATTR_READONLY);
   }
   else if (tk.isVariable(STRING_SIZE)) {
      this->set(ATTR_SIZE);
   }
   else if (tk.isVariable(STRING_ISIMAGE)
         || tk.isVariable(STRING_ISVIDEO)
         || tk.isVariable(STRING_DURATION)
         || tk.isVariable(STRING_WIDTH)
         || tk.isVariable(STRING_HEIGHT))
   {
      this->setCoreCommandBase();
      this->set(ATTR_IMAGE_OR_VIDEO);
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
