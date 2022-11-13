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

#ifndef ATTRIB_H
#define ATTRIB_H

#include "datatype/primitives.h"
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
inline constexpr _aunit ATTR_DRIVE =        0b00000000000000000000000000100000;
inline constexpr _aunit ATTR_EMPTY =        0b00000000000000000000000001000000;
inline constexpr _aunit ATTR_ENCRYPTED =    0b00000000000000000000000010000000;
inline constexpr _aunit ATTR_EXISTS =       0b00000000000000000000000100000000;
inline constexpr _aunit ATTR_EXTENSION =    0b00000000000000000000001000000000;
inline constexpr _aunit ATTR_FULLNAME =     0b00000000000000000000010000000000;
inline constexpr _aunit ATTR_HIDDEN =       0b00000000000000000000100000000000;
inline constexpr _aunit ATTR_LIFETIME =     0b00000000000000000001000000000000;
inline constexpr _aunit ATTR_MODIFICATION = 0b00000000000000000010000000000000;
inline constexpr _aunit ATTR_NAME =         0b00000000000000000100000000000000;
inline constexpr _aunit ATTR_PARENT =       0b00000000000000001000000000000000;
inline constexpr _aunit ATTR_PATH =         0b00000000000000010000000000000000;
inline constexpr _aunit ATTR_READONLY =     0b00000000000000100000000000000000;
inline constexpr _aunit ATTR_SIZE =         0b00000000000001000000000000000000;

// certain expression or syntax structure may require multiple attributes of file:
// for example - creation time, modification time, size and extension
// instead of reading them one by one
// all attributes from an expression are joined together and read at once at the beginning of expression evaluation

struct Uroboros;

struct Attribute
{
public:
   Attribute(Uroboros& uro);
   Attribute(const _aunit& val, Uroboros& uro);

   void add(const Token& tk);
   void set(const _aunit& v);
   void setCoreCommandBase();
   void setTimeCommandBase();
   _bool has(const _aunit& v) const;
   _bool hasAny() const;
   _aunit getValue() const;
   _bool isMarkedToEvaluate() const;
   void markToEvaluate();
   _bool isMarkedToRun() const;
   void markToRun();

   virtual void run() const;

protected:
   _bool markedToEvaluate = false;
   _bool markedToRun = false;
   _aunit value;
   Uroboros& uroboros;
};

typedef std::unique_ptr<Attribute> _attrptr;

// sometimes files and directories are taken from the file system
// by Win32API functions 'FindFirstFile()' and 'FindNextFile()'
// the result of their call is a WIN32_FIND_DATA struct filled with data
// this struct already contains all the data we will ever need when operating with files
// BridgeAttribute contains a dangling pointer to this data
// as a result, we do not have to load the same data multiple times

struct BridgeAttribute : Attribute
{
public:
   BridgeAttribute(Uroboros& uro, _fdata* data);
   BridgeAttribute(const _aunit& val, Uroboros& uro, _fdata* data);
   void run() const;

private:
   _fdata* dataPnt;
};

}

#endif /* ATTRIB_H */
