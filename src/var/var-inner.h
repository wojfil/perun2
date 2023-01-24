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

#ifndef VAR_INNER_H_INCLUDED
#define VAR_INNER_H_INCLUDED

#include "var.h"
#include "../datatype/generator.h"
#include "../datatype/datatype.h"
#include "../datatype/primitives.h"


namespace uro
{

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

   InnerVariables(const _str& loc, const _str& ucom);
   _list getAlphabet();

   void createThisRef(_genptr<_str>& result);
   void createThisRef(_genptr<_num>& result);
   void createThisRef(_genptr<_tim>& result);

   void createThisVarRef(vars::Variable<_str>*& result);
   void createThisVarRef(vars::Variable<_num>*& result);
   void createThisVarRef(vars::Variable<_tim>*& result);

   ThisState thisState;
   vars::Variable<_tim> access;
   vars::Variable<_bool> archive;
   vars::Variable<_tim> change;
   vars::Variable<_bool> compressed;
   vars::Variable<_tim> creation;
   vars::Variable<_numi> depth;
   vars::Variable<_str> drive;
   vars::Variable<_bool> empty;
   vars::Variable<_bool> encrypted;
   vars::Variable<_bool> exists;
   vars::Variable<_str> extension;
   vars::Variable<_str> fullname;
   vars::Variable<_bool> hidden;
   vars::Variable<_numi> index;
   vars::Variable<_bool> isdirectory;
   vars::Variable<_bool> isfile;
   vars::Variable<_per> lifetime;
   vars::Variable<_str> location;
   vars::Variable<_tim> modification;
   vars::Variable<_str> name;
   vars::Variable<_str> parent;
   vars::Variable<_str> path;
   vars::Variable<_bool> readonly;
   vars::Variable<_num> size;

   vars::Variable<_num> this_n;
   vars::Variable<_str> this_s;
   vars::Variable<_tim> this_t;

   vars::Variable<_bool> success;

   _str trimmed;
   const _str urocom;
};

}

#endif // VAR_INNER_H_INCLUDED
