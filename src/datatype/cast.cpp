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

#include "cast.h"
#include "../uroboros.h"


_num Cast_B_N::getValue()
{
   return _num ( this->base->getValue() ? 1LL : 0LL ) ;
};

_nlist Cast_B_NL::getValue()
{
   return _nlist { this->base->getValue() ? 1LL : 0LL };
};

_str Cast_B_S::getValue()
{
   return this->base->getValue() ? L"1" : L"0";
};

_list Cast_B_L::getValue()
{
   return _list { this->base->getValue() ? L"1" : L"0" };
};

_nlist Cast_N_NL::getValue(){
   return _nlist { this->base->getValue() };
};

_str Cast_N_S::getValue()
{
   return this->base->getValue().toString();
};

_list Cast_N_L::getValue()
{
   return _list { this->base->getValue().toString() };
};

_str Cast_T_S::getValue()
{
   return this->base->getValue().toString();
};

_list Cast_T_L::getValue()
{
   return _list { this->base->getValue().toString() };
};

_str Cast_P_S::getValue()
{
   return this->base->getValue().toString();
};

_list Cast_P_L::getValue()
{
   return _list { this->base->getValue().toString() };
};

_list Cast_NL_L::getValue()
{
   const _nlist nums = this->base->getValue();
   const _size len = nums.size();
   _list strings(len);

   for (_size i = 0; i < len; i++) {
      strings[i] = nums[i].toString();
   }

   return strings;
};

_list Cast_S_L::getValue()
{
   return _list { this->base->getValue() };
};

_tlist Cast_T_TL::getValue()
{
   return _tlist { this->base->getValue() };
}

_list Cast_TL_L::getValue()
{
   const _tlist times = this->base->getValue();
   const _size len = times.size();
   _list strings(len);

   for (_size i = 0; i < len; i++) {
      strings[i] = times[i].toString();
   }

   return strings;
};

_list Cast_D_L::getValue()
{
   _list strings;

   while (this->base->hasNext()) {
      if (!this->uroboros->running) {
         this->base->reset();
         break;
      }

      strings.push_back(this->base->getValue());
   }

   return strings;
};
