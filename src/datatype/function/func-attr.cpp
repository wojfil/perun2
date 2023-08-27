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

#include "func-attr.h"
#include "..\..\os.h"
#include "..\..\util.h"


namespace perun2::func
{

p_str F_Parent::getValue()
{
   const p_str v = os_trim(arg1->getValue());
   if (os_isInvalid(v)) {
      return p_str();
   }

   // there is no separator, so simply return location
   if (v.find(OS_SEPARATOR) == p_str::npos) {
      return this->context->location->value;
   }

   return os_parent(os_leftJoin(this->context->location->value, v));
}


p_str F_Path_1::getValue()
{
   const p_str v = os_trim(arg1->getValue());
   if (os_isInvalid(v)) {
      return p_str();
   }

   return os_leftJoin(this->context->location->value, v);
}


p_str F_Path_2::getValue()
{
   const p_str v1 = os_trim(this->value_1->getValue());
   const p_str v2 = os_trim(this->value_2->getValue());
   return os_join(v1, v2);
}


p_str F_Path_3::getValue()
{
   const p_str v1 = os_trim(this->value_1->getValue());
   const p_str v2 = os_trim(this->value_2->getValue());
   const p_str v3 = os_trim(this->value_3->getValue());
   return os_join(os_join(v1, v2), v3);
}


p_str F_Path_4::getValue()
{
   const p_str v1 = os_trim(this->value_1->getValue());
   const p_str v2 = os_trim(this->value_2->getValue());
   const p_str v3 = os_trim(this->value_3->getValue());
   const p_str v4 = os_trim(this->value_4->getValue());
   return os_join(os_join(os_join(v1, v2), v3), v4);
}

p_str F_Path_Multi::getValue()
{
   p_str path = os_trim((values[length - 1])->getValue());
   if (path.empty()) {
      return p_str();
   }

   p_int index = length - 2;

   while (index >= 0 && !os_isAbsolute(path)) {
      const p_str p = os_trim(values[index]->getValue());
      path = os_join(p, path);

      if (path.empty()) {
         return p_str();
      }

      index--;
   }

   return path;
}


p_num F_SizeDefinition::getValue()
{
   p_nint total = NINT_ZERO;

   while (definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         definition->reset();
         return p_num(NINT_MINUS_ONE);
      }

      const p_str v = definition->getValue();
      const p_nint s = os_size(os_leftJoin(this->context->location->value, v), this->perun2);
      if (s != NINT_MINUS_ONE) {
         total += s;
      }
   }

   return p_num(total);
}


p_num F_SizeList::getValue()
{
   p_nint total = NINT_ZERO;
   const p_list vs = values->getValue();
   const p_size len = vs.size();
   p_bool any = false;

   if (len == 0) {
      return p_num();
   }

   for (p_size i = 0; i < len; i++) {
      if (this->perun2.isNotRunning()) {
         return p_num(NINT_MINUS_ONE);
      }

      const p_str v = os_trim(vs[i]);
      if (!v.empty() && !os_isInvalid(v)) {
         const p_nint s = os_size(os_leftJoin(this->context->location->value, v), this->perun2);
         if (s != NINT_MINUS_ONE) {
            total += s;
            any = true;
         }
      }
   }

   return p_num(any? total : NINT_MINUS_ONE);
}


void F_Attribute::checkExistence()
{
   this->context.v_exists->value = this->context.invalid
      ? false
      : os_exists(this->context.v_path->value);
}


p_num F_Attr_Size::getValue()
{
   return this->context.invalid
      ? NINT_MINUS_ONE
      : os_size(this->context.v_path->value, this->perun2);
}


p_per F_Attr_Lifetime::getValue()
{
   return this->context.invalid
      ? p_per()
      : os_lifetime(this->context.v_path->value);
}


p_tim F_Attr_Access::getValue()
{
   return this->context.invalid
      ? p_tim()
      : os_access(this->context.v_path->value);
}


p_tim F_Attr_Change::getValue()
{
   return this->context.invalid
      ? p_tim()
      : os_change(this->context.v_path->value);
}


p_tim F_Attr_Creation::getValue()
{
   return this->context.invalid
      ? p_tim()
      : os_creation(this->context.v_path->value);
}


p_tim F_Attr_Modification::getValue()
{
   return this->context.invalid
      ? p_tim()
      : os_modification(this->context.v_path->value);
}


p_bool F_Attr_Archive::getValue()
{
   return this->context.invalid
      ? false
      : os_archive(this->context.v_path->value);
}


p_bool F_Attr_Compressed::getValue()
{
   return this->context.invalid
      ? false
      : os_compressed(this->context.v_path->value);
}


p_bool F_Attr_Empty::getValue()
{
   return this->context.invalid
      ? false
      : os_empty(this->context.v_path->value);
}


p_bool F_Attr_Exists::getValue()
{
   return this->context.invalid
      ? false
      : os_exists(this->context.v_path->value);
}


p_bool F_Attr_Encrypted::getValue()
{
   return this->context.invalid
      ? false
      : os_encrypted(this->context.v_path->value);
}


p_bool F_Attr_Hidden::getValue()
{
   return this->context.invalid
      ? false
      : os_hidden(this->context.v_path->value);
}


p_bool F_Attr_IsDirectory::getValue()
{
   return this->context.invalid
      ? false
      : os_isDirectory(this->context.v_path->value);
}


p_bool F_Attr_IsFile::getValue()
{
   return this->context.invalid
      ? false
      : os_isFile(this->context.v_path->value);
}


p_bool F_Attr_Readonly::getValue()
{
   return this->context.invalid
      ? false
      : os_readonly(this->context.v_path->value);
}

}
