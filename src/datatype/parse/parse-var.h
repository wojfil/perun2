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

#include "../../perun2.h"


namespace perun2::parse
{
   p_bool isAlterableAttribute(const Token& tk, Perun2Process& p2);

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_bool>& result, Perun2Process& p2);
   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_num>& result, Perun2Process& p2);
   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_per>& result, Perun2Process& p2);
   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_tim>& result, Perun2Process& p2);

   template <typename T>
   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<T>& result, Perun2Process& p2)
   {
      return false;
   }

   p_bool makeVarRef(const Token& tk, p_defptr& result, Perun2Process& p2);

   template <typename T>
   p_bool makeVarRef(const Token& tk, p_genptr<T>& result, Perun2Process& p2)
   {
      Variable<T>* var;
      if (!p2.contexts.getVar(tk, var, p2)) {
         return false;
      }

      if (var->type == VarType::vt_Attribute && isAlterableAttribute(tk, p2))
      {
         return makeVarRefAsFunction(tk, result, p2);
      }

      result = std::make_unique<VariableReference<T>>(var);
      return true;
   };
}
