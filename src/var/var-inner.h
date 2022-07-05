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

#ifndef VAR_INNER_H_INCLUDED
#define VAR_INNER_H_INCLUDED

#include "var.h"
#include "../datatype/generator.h"
#include "../datatype/datatype.h"
#include "../datatype/primitives.h"



// parsing context of the variable "this"
enum ThisState
{
   ts_None = 0,
   ts_String,
   ts_Time,
   ts_Number
};

struct InnerVariables
{
public:

   InnerVariables();
   _list getAlphabet();
   _list getAscii();

   void createThisReference(Variable<_str>*& result);
   void createThisReference(Variable<_num>*& result);
   void createThisReference(Variable<_tim>*& result);

   ThisState thisState;
   Variable<_tim> access;
   Variable<_boo> archive;
   Variable<_tim> change;
   Variable<_boo> compressed;
   Variable<_tim> creation;
   Variable<_num> depth;
   Variable<_str> drive;
   Variable<_boo> empty;
   Variable<_boo> encrypted;
   Variable<_boo> exists;
   Variable<_str> extension;
   Variable<_str> fullname;
   Variable<_boo> hidden;
   Variable<_num> index;
   Variable<_boo> isdirectory;
   Variable<_boo> isfile;
   Variable<_per> lifetime;
   Variable<_str> location;
   Variable<_tim> modification;
   Variable<_str> name;
   Variable<_str> parent;
   Variable<_str> path;
   Variable<_boo> readonly;
   Variable<_num> size;

   Variable<_num> this_n;
   Variable<_str> this_s;
   Variable<_tim> this_t;

   Variable<_boo> success;

   _str trimmed;
   _str urocom;

};

#endif // VAR_INNER_H_INCLUDED
