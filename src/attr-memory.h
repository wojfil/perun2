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

#ifndef ATTR_MEMORY_H_INCLUDED
#define ATTR_MEMORY_H_INCLUDED

#include "attribute.h"
#include "datatype/datatype.h"
#include "var/var-inner.h"


namespace uro
{

// this struct is a consequence of using struct Attribute in iteration loops with other nested iteration loops
// sometimes attributes have to be saved in the memory and rolled back later

struct AttributeMemory
{
public:
   AttributeMemory(_attrptr& attr, InnerVariables& iv);
   AttributeMemory(InnerVariables& iv);

   void load();
   void restore() const;

private:
   const _bool hasAttribute;
   Attribute* attribute;
   InnerVariables& inner;

   _str prev_trimmed;
   _str prev_path;
   _str prev_fullname;
   _str prev_parent;
   _str prev_drive;
   _numi prev_depth;
   _bool prev_exists;
   _bool prev_isfile;
   _bool prev_isdirectory;
   _tim prev_access;
   _bool prev_archive;
   _bool prev_compressed;
   _tim prev_change;
   _tim prev_creation;
   _bool prev_empty;
   _bool prev_encrypted;
   _str prev_extension;
   _bool prev_hidden;
   _per prev_lifetime;
   _tim prev_modification;
   _str prev_name;
   _bool prev_readonly;
   _num prev_size;
};

}

#endif // ATTR_MEMORY_H_INCLUDED
