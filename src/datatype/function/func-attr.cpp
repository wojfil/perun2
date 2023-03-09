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

_str F_Parent::getValue()
{
   const _str v = os_trim(arg1->getValue());
   if (os_isInvaild(v)) {
      return EMPTY_STRING;
   }

   // there is no separator, so simply return location
   if (v.find(OS_SEPARATOR) == _str::npos) {
      return this->context->location->value;
   }

   return os_parent(os_join(this->context->location->value, v));
}


_str F_Path_1::getValue()
{
   const _str v = os_trim(arg1->getValue());
   if (os_isInvaild(v)) {
      return EMPTY_STRING;
   }

   return os_join(this->context->location->value, v);
}


_str F_Path_2::getValue()
{
   const _str v2 = os_trim(this->value_2->getValue());

   if (os_isAbsolute(v2)) {
      return v2;
   }
   else if (v2.empty()) {
      return EMPTY_STRING;
   }
   else {
      const _str v1 = os_trim(this->value_1->getValue());
      return v1.empty()
         ? EMPTY_STRING
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
      return EMPTY_STRING;
   }
   else {
      const _str v2 = os_trim(this->value_2->getValue());

      if (os_isAbsolute(v2)) {
         return os_join(v2, v3);
      }
      else if (v2.empty()) {
         return EMPTY_STRING;
      }
      else {
         const _str v1 = os_trim(this->value_1->getValue());
         return v1.empty()
            ? EMPTY_STRING
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
      return EMPTY_STRING;
   }
   else {
      const _str v3 = os_trim(this->value_3->getValue());

      if (os_isAbsolute(v3)) {
         return os_join(v3, v4);
      }
      else if (v3.empty()) {
         return EMPTY_STRING;
      }
      else {
         const _str v2 = os_trim(this->value_2->getValue());

         if (os_isAbsolute(v2)) {
            return os_join(os_join(v2, v3), v4);
         }
         else if (v2.empty()) {
            return EMPTY_STRING;
         }
         else {
            const _str v1 = os_trim(this->value_1->getValue());
            return v1.empty()
               ? EMPTY_STRING
               : os_join(os_join(os_join(v1, v2), v3), v4);
         }
      }
   }
}

_str F_Path_Multi::getValue()
{
   _str path = os_trim((values[length - 1])->getValue());
   if (path.empty()) {
      return EMPTY_STRING;
   }

   _int index = length - 2;

   while (index >= 0 && !os_isAbsolute(path)) {
      const _str p = os_trim(values[index]->getValue());
      if (p.empty()) {
         return EMPTY_STRING;
      }

      path = os_join(p, path);
      index--;
   }

   return path;
}


_num F_SizeDefinition::getValue()
{
   _nint total = NINT_ZERO;

   while (definition->hasNext()) {
      if (!this->perun2.state == State::s_Running) {
         definition->reset();
         return _num(NINT_MINUS_ONE);
      }

      const _str v = definition->getValue();
      const _nint s = os_size(os_join(this->context->location->value, v), this->perun2);
      if (s != NINT_MINUS_ONE) {
         total += s;
      }
   }

   return _num(total);
}


_num F_SizeList::getValue()
{
   _bool any = false;
   _nint total = NINT_ZERO;
   const _list vs = values->getValue();
   const _size len = vs.size();
   if (len == 0) {
      return _num();
   }

   for (_size i = 0; i < len; i++) {
      if (!this->perun2.state == State::s_Running) {
         return _num(NINT_MINUS_ONE);
      }

      const _str v = os_trim(vs[i]);
      if (!v.empty() && !os_isInvaild(v)) {
         const _nint s = os_size(os_join(this->context->location->value, v), this->perun2);
         if (s != NINT_MINUS_ONE) {
            total += s;
            any = true;
         }
      }
   }

   return any ? _num(total) : _num(NINT_MINUS_ONE);
}

}
