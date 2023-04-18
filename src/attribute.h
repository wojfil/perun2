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

#ifndef ATTRIB_H
#define ATTRIB_H

#include "hash.h"
#include "token.h"
#include <memory>


namespace perun2
{

typedef _uint64 _aunit;

_constexpr _aunit ATTR_NULL =         0b00000000000000000000000000000000;
_constexpr _aunit ATTR_ACCESS =       0b00000000000000000000000000000001;
_constexpr _aunit ATTR_ARCHIVE =      0b00000000000000000000000000000010;
_constexpr _aunit ATTR_CHANGE =       0b00000000000000000000000000000100;
_constexpr _aunit ATTR_COMPRESSED =   0b00000000000000000000000000001000;
_constexpr _aunit ATTR_CREATION =     0b00000000000000000000000000010000;
_constexpr _aunit ATTR_DEPTH =        0b00000000000000000000000000100000;
_constexpr _aunit ATTR_DRIVE =        0b00000000000000000000000001000000;
_constexpr _aunit ATTR_EMPTY =        0b00000000000000000000000010000000;
_constexpr _aunit ATTR_ENCRYPTED =    0b00000000000000000000000100000000;
_constexpr _aunit ATTR_EXISTS =       0b00000000000000000000001000000000;
_constexpr _aunit ATTR_EXTENSION =    0b00000000000000000000010000000000;
_constexpr _aunit ATTR_FULLNAME =     0b00000000000000000000100000000000;
_constexpr _aunit ATTR_HIDDEN =       0b00000000000000000001000000000000;
_constexpr _aunit ATTR_LIFETIME =     0b00000000000000000010000000000000;
_constexpr _aunit ATTR_MODIFICATION = 0b00000000000000000100000000000000;
_constexpr _aunit ATTR_NAME =         0b00000000000000001000000000000000;
_constexpr _aunit ATTR_PARENT =       0b00000000000000010000000000000000;
_constexpr _aunit ATTR_PATH =         0b00000000000000100000000000000000;
_constexpr _aunit ATTR_READONLY =     0b00000000000001000000000000000000;
_constexpr _aunit ATTR_SIZE =         0b00000000000010000000000000000000;

// certain expression or syntax structure may require multiple file attributes:
// for example - creation time, modification time, size and extension
// they also may repeat
// instead of reading them one by one
// all attributes from an expression are joined together and read at once at the beginning of expression evaluation

struct _p2;

struct Attribute
{
public:
   Attribute() = delete;
   Attribute(_p2& p2);
   Attribute(const _aunit val, _p2& p2);

   void add(const _hash val);
   inline void set(const _aunit v);
   void setCoreCommandBase();
   void setTimeCommandBase();
   _bool has(const _aunit v) const;
   _bool hasAny() const;
   _aunit getValue() const;
   
   _p2& perun2;

protected:
   _aunit value = ATTR_NULL;
};

typedef std::unique_ptr<Attribute> _attrptr;

}

#endif /* ATTRIB_H */
