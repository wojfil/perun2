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

#pragma once

#include "token.hpp"
#include <memory>


namespace perun2
{

typedef uint64_t     p_aunit;

p_constexpr p_aunit ATTR_NULL =           0;
p_constexpr p_aunit ATTR_ACCESS =         1 << 0;
p_constexpr p_aunit ATTR_ARCHIVE =        1 << 1;
p_constexpr p_aunit ATTR_CHANGE =         1 << 2;
p_constexpr p_aunit ATTR_COMPRESSED =     1 << 3;
p_constexpr p_aunit ATTR_CREATION =       1 << 4;
p_constexpr p_aunit ATTR_DEPTH =          1 << 5;
p_constexpr p_aunit ATTR_DRIVE =          1 << 6;
p_constexpr p_aunit ATTR_EMPTY =          1 << 7;
p_constexpr p_aunit ATTR_ENCRYPTED =      1 << 8;
p_constexpr p_aunit ATTR_EXISTS =         1 << 9;
p_constexpr p_aunit ATTR_EXTENSION =      1 << 10;
p_constexpr p_aunit ATTR_FULLNAME =       1 << 11;
p_constexpr p_aunit ATTR_HIDDEN =         1 << 12;
p_constexpr p_aunit ATTR_LIFETIME =       1 << 13;
p_constexpr p_aunit ATTR_MODIFICATION =   1 << 14;
p_constexpr p_aunit ATTR_NAME =           1 << 15;
p_constexpr p_aunit ATTR_PARENT =         1 << 16;
p_constexpr p_aunit ATTR_PATH =           1 << 17;
p_constexpr p_aunit ATTR_READONLY =       1 << 18;
p_constexpr p_aunit ATTR_SIZE =           1 << 19;
p_constexpr p_aunit ATTR_SIZE_FILE_ONLY=  1 << 20;
p_constexpr p_aunit ATTR_IMAGE_OR_VIDEO = 1 << 21;

// certain expression or syntax structure may require multiple file attributes:
// for example - creation time, modification time, size and extension
// they also may repeat
// instead of reading them one by one
// all attributes from an expression are joined together and read at once at the beginning of expression evaluation


struct Attribute
{
public:
   Attribute();
   Attribute(const p_aunit val);

   void add(const Token& tk);
   void set(const p_aunit v);
   void setCoreCommandBase();
   void setTimeCommandBase();
   p_bool has(const p_aunit v) const;
   p_bool hasAny() const;
   p_aunit getValue() const;

private:
   p_aunit value;
};

typedef std::unique_ptr<Attribute> p_attrptr;

}
