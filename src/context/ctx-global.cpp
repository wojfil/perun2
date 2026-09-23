/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ctx-global.h"
#include "../perun2.h"
#include "../datatype/generator/gen-time.h"


namespace perun2
{

   GlobalContext::GlobalContext(Perun2Process& p2)
   {
      this->globalVars.times.insert(std::make_pair(STRING_NOW, std::make_unique<gen::v_Now>()));
      this->globalVars.times.insert(std::make_pair(STRING_TODAY, std::make_unique<gen::v_Today>()));
      this->globalVars.times.insert(std::make_pair(STRING_YESTERDAY, std::make_unique<gen::v_Yesterday>()));
      this->globalVars.times.insert(std::make_pair(STRING_TOMORROW, std::make_unique<gen::v_Tomorrow>()));

      this->insertConstant<p_str>(STRING_DESKTOP);
      this->insertConstant<p_str>(STRING_PERUN2);
      this->insertConstant<p_str>(STRING_ORIGIN);
      this->insertConstant<p_list>(STRING_ALPHABET);
      this->insertConstant<p_list>(STRING_ASCII);
      this->insertConstant<p_list>(STRING_ARGUMENTS);
      this->insertConstant<p_list>(STRING_NOTHING);
      this->insertConstant<p_list>(STRING_PENDRIVES);
      this->insertConstant<p_str>(STRING_PENDRIVE);
      this->insertConstant<p_tim>(STRING_NEVER);
      this->insertConstant<p_str>(STRING_DOWNLOADS);
      
      this->insertConstant<p_num>(STRING_NAN);
      this->globalVars.numbers[STRING_NAN]->value.setToNaN();
   };

}
