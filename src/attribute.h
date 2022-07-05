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

#define _aunit _uint64

const _aunit ATTR_NULL =         0b00000000000000000000000000000000;
const _aunit ATTR_ACCESS =       0b00000000000000000000000000000001;
const _aunit ATTR_ARCHIVE =      0b00000000000000000000000000000010;
const _aunit ATTR_CHANGE =       0b00000000000000000000000000000100;
const _aunit ATTR_COMPRESSED =   0b00000000000000000000000000001000;
const _aunit ATTR_CREATION =     0b00000000000000000000000000010000;
const _aunit ATTR_DRIVE =        0b00000000000000000000000000100000;
const _aunit ATTR_EMPTY =        0b00000000000000000000000001000000;
const _aunit ATTR_ENCRYPTED =    0b00000000000000000000000010000000;
const _aunit ATTR_EXISTS =       0b00000000000000000000000100000000;
const _aunit ATTR_EXTENSION =    0b00000000000000000000001000000000;
const _aunit ATTR_FULLNAME =     0b00000000000000000000010000000000;
const _aunit ATTR_HIDDEN =       0b00000000000000000000100000000000;
const _aunit ATTR_LIFETIME =     0b00000000000000000001000000000000;
const _aunit ATTR_MODIFICATION = 0b00000000000000000010000000000000;
const _aunit ATTR_NAME =         0b00000000000000000100000000000000;
const _aunit ATTR_PARENT =       0b00000000000000001000000000000000;
const _aunit ATTR_PATH =         0b00000000000000010000000000000000;
const _aunit ATTR_READONLY =     0b00000000000000100000000000000000;
const _aunit ATTR_SIZE =         0b00000000000001000000000000000000;

// certain expression or syntax structure may require multiple file attributes:
// for example - creation time, modification time, size and extension
// instead of reading them one by one
// all attributes from an expression are joined together and read at once
// this approach introduces a lot of optimization - only one file system call is needed

struct Uroboros;
struct InnerVariables;

struct Attribute
{
public:
   Attribute(Uroboros* uro);

   void add(const Token& tk);
   void set(const _aunit& v);
   void setCoreCommandBase();
   _boo has(const _aunit& v) const;
   _boo hasAny() const;

   void run() const;

   _boo markToRun = false;
   _boo markToEvaluate = false;

   Uroboros* uroboros;
   InnerVariables* inner;

private:
   _aunit value;
};

#endif /* ATTRIB_H */
