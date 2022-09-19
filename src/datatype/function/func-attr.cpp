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

#include "func-attr.h"
#include "..\..\var\var-runtime.h"
#include "..\..\os.h"
#include "..\..\util.h"


namespace uro::func
{

AttrFunction::~AttrFunction()
{
   delete value;
}

_str AttrFunction::getPath()
{
   const _str b = os_trim(this->value->getValue());

   if (os_isInvaild(b)) {
      return L"";
   }

   const _str p = os_join(this->inner->location.value, b);
   return os_isAbsolute(p) ? p : L"";
}

_str F_Path::getValue()
{
   const _str b = os_trim(this->value->getValue());

   return b.empty() || this->inner->location.value.empty()
      ? L""
      : os_join(this->inner->location.value, b);
}

F_Path_2::~F_Path_2()
{
   delete value_1;
   delete value_2;
}

F_Path_3::~F_Path_3()
{
   delete value_1;
   delete value_2;
   delete value_3;
}

F_Path_4::~F_Path_4()
{
   delete value_1;
   delete value_2;
   delete value_3;
   delete value_4;
}

_str F_Path_2::getValue()
{
   const _str v2 = os_trim(this->value_2->getValue());

   if (os_isAbsolute(v2)) {
      return v2;
   }
   else if (v2.empty()) {
      return L"";
   }
   else {
      const _str v1 = os_trim(this->value_1->getValue());
      return v1.empty()
         ? L""
         : os_join(v1, v2);
   }
}

_str F_Path_3::getValue()
{
   const _str v3 = os_trim(this->value_3->getValue());

   if (os_isAbsolute(v3)) {
      return v3;
   }
   else if (v3.empty()) {
      return L"";
   }
   else {
      const _str v2 = os_trim(this->value_2->getValue());

      if (os_isAbsolute(v2)) {
         return os_join(v2, v3);
      }
      else if (v2.empty()) {
         return L"";
      }
      else {
         const _str v1 = os_trim(this->value_1->getValue());
         return v1.empty()
            ? L""
            : os_join(os_join(v1, v2), v3);
      }
   }
}

_str F_Path_4::getValue()
{
   const _str v4 = os_trim(this->value_4->getValue());

   if (os_isAbsolute(v4)) {
      return v4;
   }
   else if (v4.empty()) {
      return L"";
   }
   else {
      const _str v3 = os_trim(this->value_3->getValue());

      if (os_isAbsolute(v3)) {
         return os_join(v3, v4);
      }
      else if (v3.empty()) {
         return L"";
      }
      else {
         const _str v2 = os_trim(this->value_2->getValue());

         if (os_isAbsolute(v2)) {
            return os_join(os_join(v2, v3), v4);
         }
         else if (v2.empty()) {
            return L"";
         }
         else {
            const _str v1 = os_trim(this->value_1->getValue());
            return v1.empty()
               ? L""
               : os_join(os_join(os_join(v1, v2), v3), v4);
         }
      }
   }
}

F_Path_Multi::~F_Path_Multi()
{
   langutil::deleteVectorPtr(this->values);
}

_str F_Path_Multi::getValue()
{
   _str path = os_trim(((*values)[length - 1])->getValue());
   if (path.empty()) {
      return L"";
   }

   _int index = length - 2;

   while (index >= 0 && !os_isAbsolute(path)) {
      const _str p = os_trim(((*values)[index])->getValue());
      if (p.empty()) {
         return L"";
      }

      path = os_join(p, path);
      index--;
   }

   return path;
}

_str F_Drive::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? L"" : os_drive(p);
}

_str F_Extension::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? L"" : os_extension(p);
}

_str F_Fullname::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? L"" : os_fullname(p);
}

_str F_Name::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? L"" : os_name(p);
}

_str F_Parent::getValue()
{
   const _str p = this->getPath();
   return (p.empty() || !os_hasParentDirectory(p))
      ? L""
      : os_parent(p);
}

_num F_Size::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? _num(-1LL) : _num(os_size(p, this->uroboros));
}


_num F_SizeDefinition::getValue()
{
   _nint total = 0LL;

   while (definition->hasNext()) {
      if (!this->uroboros->running) {
         definition->reset();
         return _num(-1LL);
      }

      const _str v = definition->getValue();
      const _nint s = os_size(os_join(this->inner->location.value, v), this->uroboros);
      if (s != -1LL) {
         total += s;
      }
   }

   return _num(total);
}


_num F_SizeList::getValue()
{
   _boo any = false;
   _nint total = 0LL;
   const _list vs = values->getValue();
   const _size len = vs.size();
   if (len == 0) {
      return 0LL;
   }

   for (_size i = 0; i < len; i++) {
      if (!this->uroboros->running) {
         return _num(-1LL);
      }

      const _str v = os_trim(vs[i]);
      if (!v.empty() && !os_isInvaild(v)) {
         const _nint s = os_size(os_join(this->inner->location.value, v), this->uroboros);
         if (s != -1LL) {
            total += s;
            any = true;
         }
      }
   }

   return any ? _num(total) : _num(-1LL);
}


_per F_Lifetime::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? _per() : os_lifetime(p);
}

_tim F_Access::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? _tim() : os_access(p);
}

_tim F_Change::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? _tim() : os_change(p);
}

_tim F_Creation::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? _tim() : os_creation(p);
}

_tim F_Modification::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? _tim() : os_modification(p);
}

_boo F_Archive::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_archive(p);
}

_boo F_Compressed::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_compressed(p);
}

_boo F_Empty::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_empty(p);
}

_boo F_Encrypted::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_encrypted(p);
}

F_Exist::~F_Exist()
{
   delete value;
}

_boo F_Exist::getValue()
{
   if (this->inner->location.value.empty()) {
      return false;
   }

   const _list v = value->getValue();
   const _size len = v.size();

   switch (len) {
      case 0: {
         return true;
      }
      case 1: {
         const _str b = os_trim(v[0]);
         if (b.empty()) {
            return false;
         }

         return os_exists(os_join(this->inner->location.value, b));
      }
      default: {
         for (_size i = 0; i < len; i++) {
            const _str b = os_trim(v[i]);
            if (b.empty()) {
               return false;
            }

            if (!os_exists(os_join(this->inner->location.value, b))) {
               return false;
            }
         }

         return true;
      }
   }
}

_boo F_Exists::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_exists(p);
}

_boo F_Hidden::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_hidden(p);
}

_boo F_IsDirectory::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_isDirectory(p);
}

_boo F_IsFile::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_isFile(p);
}

_boo F_Readonly::getValue()
{
   const _str p = this->getPath();
   return p.empty() ? false : os_readonly(p);
}

}
