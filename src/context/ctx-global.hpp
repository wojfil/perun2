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

#pragma once

#include "ctx-vars.hpp"


namespace perun2
{
   struct Perun2Process;

   struct GlobalContext
   {
   public:
      GlobalContext() = delete;
      GlobalContext(Perun2Process& p2);

      template <typename T>
      void insertConstant(const p_str& name)
      {
         Variable<T>* v = this->globalVars.insertVar<T>(name, VarType::vt_Special);
         v->isConstant_ = true;
      }

      VarsContext globalVars;

   private:
      template <typename T>
      Variable<T>* insertVar(const p_str& name)
      {
         return this->globalVars.insertVar<T>(name, VarType::vt_Special);
      }
   };

}
