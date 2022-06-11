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
#include "../datatype/datatype.h"
#include "../datatype/primitives.h"



// parsing state of the variable "this"
enum ThisState
{
   ts_None = 0,
   ts_String,
   ts_Time,
   ts_Number
};



extern ThisState g_thisstate;

// internal variables:
extern Variable<_tim> g_access;
extern Variable<_boo> g_archive;
extern Variable<_tim> g_change;
extern Variable<_boo> g_compressed;
extern Variable<_tim> g_creation;
extern Variable<_num> g_depth;
extern Variable<_str> g_drive;
extern Variable<_boo> g_empty;
extern Variable<_boo> g_encrypted;
extern Variable<_boo> g_exists;
extern Variable<_str> g_extension;
extern Variable<_str> g_fullname;
extern Variable<_boo> g_hidden;
extern Variable<_num> g_index;
extern Variable<_boo> g_isdirectory;
extern Variable<_boo> g_isfile;
extern Variable<_per> g_lifetime;
extern Variable<_str> g_location;
extern Variable<_tim> g_modification;
extern Variable<_str> g_name;
extern Variable<_str> g_parent;
extern Variable<_str> g_path;
extern Variable<_boo> g_readonly;
extern Variable<_num> g_size;
extern Variable<_num> g_this_n;
extern Variable<_str> g_this_s;
extern Variable<_tim> g_this_t;

extern Variable<_boo> g_success;

extern _str g_trimmed;
extern _str g_urocom;

_list vinit_getAlphabet();
_list vinit_getAscii();


#endif // VAR_INNER_H_INCLUDED
