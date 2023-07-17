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

#include "cast.h"
#include "../perun2.h"


namespace perun2::gen
{

_num Cast_B_N::getValue()
{
   return _num ( this->base->getValue() ? NINT_ONE : NINT_ZERO ) ;
};

_nlist Cast_B_NL::getValue()
{
   return _nlist { this->base->getValue() ? NINT_ONE : NINT_ZERO };
};

p_str Cast_B_S::getValue()
{
   return this->base->getValue() ? toStr(CHAR_1) : toStr(CHAR_0);
};

p_list Cast_B_L::getValue()
{
   return p_list { this->base->getValue() ? toStr(CHAR_1) : toStr(CHAR_0) };
};

_nlist Cast_N_NL::getValue(){
   return _nlist { this->base->getValue() };
};

p_str Cast_N_S::getValue()
{
   return this->base->getValue().toString();
};

p_list Cast_N_L::getValue()
{
   return p_list { this->base->getValue().toString() };
};

p_str Cast_T_S::getValue()
{
   return this->base->getValue().toString();
};

p_list Cast_T_L::getValue()
{
   return p_list { this->base->getValue().toString() };
};

p_str Cast_P_S::getValue()
{
   return this->base->getValue().toString();
};

p_list Cast_P_L::getValue()
{
   return p_list { this->base->getValue().toString() };
};

p_list Cast_NL_L::getValue()
{
   const _nlist nums = this->base->getValue();
   const p_size len = nums.size();
   p_list strings(len);

   for (p_size i = 0; i < len; i++) {
      strings[i] = nums[i].toString();
   }

   return strings;
};

p_list Cast_S_L::getValue()
{
   return p_list { this->base->getValue() };
};

_tlist Cast_T_TL::getValue()
{
   return _tlist { this->base->getValue() };
}

p_list Cast_TL_L::getValue()
{
   const _tlist times = this->base->getValue();
   const p_size len = times.size();
   p_list strings(len);

   for (p_size i = 0; i < len; i++) {
      strings[i] = times[i].toString();
   }

   return strings;
};

p_list Cast_D_L::getValue()
{
   p_list strings;

   while (this->base->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->base->reset();
         break;
      }

      strings.emplace_back(this->base->getValue());
   }

   return strings;
};

}
