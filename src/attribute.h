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

#ifndef ATTRIB_H
#define ATTRIB_H

#include "hash.h"
#include "token.h"
#include <memory>


namespace uro
{

typedef _uint64 _aunit;

inline constexpr _aunit ATTR_NULL =         0b00000000000000000000000000000000;
inline constexpr _aunit ATTR_ACCESS =       0b00000000000000000000000000000001;
inline constexpr _aunit ATTR_ARCHIVE =      0b00000000000000000000000000000010;
inline constexpr _aunit ATTR_CHANGE =       0b00000000000000000000000000000100;
inline constexpr _aunit ATTR_COMPRESSED =   0b00000000000000000000000000001000;
inline constexpr _aunit ATTR_CREATION =     0b00000000000000000000000000010000;
inline constexpr _aunit ATTR_DEPTH =        0b00000000000000000000000000100000;
inline constexpr _aunit ATTR_DRIVE =        0b00000000000000000000000001000000;
inline constexpr _aunit ATTR_EMPTY =        0b00000000000000000000000010000000;
inline constexpr _aunit ATTR_ENCRYPTED =    0b00000000000000000000000100000000;
inline constexpr _aunit ATTR_EXISTS =       0b00000000000000000000001000000000;
inline constexpr _aunit ATTR_EXTENSION =    0b00000000000000000000010000000000;
inline constexpr _aunit ATTR_FULLNAME =     0b00000000000000000000100000000000;
inline constexpr _aunit ATTR_HIDDEN =       0b00000000000000000001000000000000;
inline constexpr _aunit ATTR_LIFETIME =     0b00000000000000000010000000000000;
inline constexpr _aunit ATTR_MODIFICATION = 0b00000000000000000100000000000000;
inline constexpr _aunit ATTR_NAME =         0b00000000000000001000000000000000;
inline constexpr _aunit ATTR_PARENT =       0b00000000000000010000000000000000;
inline constexpr _aunit ATTR_PATH =         0b00000000000000100000000000000000;
inline constexpr _aunit ATTR_READONLY =     0b00000000000001000000000000000000;
inline constexpr _aunit ATTR_SIZE =         0b00000000000010000000000000000000;

// certain expression or syntax structure may require multiple attributes of file:
// for example - creation time, modification time, size and extension
// instead of reading them one by one
// all attributes from an expression are joined together and read at once at the beginning of expression evaluation

struct _uro;

struct Attribute
{
public:
   Attribute() = delete;
   Attribute(_uro& uro);
   Attribute(const _aunit val, _uro& uro);

   void add(const _hash& val);
   inline void set(const _aunit v);
   void setCoreCommandBase();
   void setTimeCommandBase();
   _bool has(const _aunit v) const;
   _bool hasAny() const;
   _aunit getValue() const;
   _bool isMarkedToEvaluate() const;
   void markToEvaluate();
   _bool isMarkedToRun() const;
   void markToRun();
   
   _uro& uroboros;

protected:
   _bool markedToEvaluate = false;
   _bool markedToRun = false;
   _aunit value = ATTR_NULL;
};

typedef std::unique_ptr<Attribute> _attrptr;

}

#endif /* ATTRIB_H */
