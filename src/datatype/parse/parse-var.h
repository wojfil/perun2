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

#ifndef PARSE_VAR_H_INCLUDED
#define PARSE_VAR_H_INCLUDED

#include "../../context.h"


namespace perun2::parse
{

   _bool makeVarRefAsFunction(const Token& tk, _genptr<_bool>& result, _p2& p2);
   _bool makeVarRefAsFunction(const Token& tk, _genptr<_num>& result, _p2& p2);
   _bool makeVarRefAsFunction(const Token& tk, _genptr<_per>& result, _p2& p2);
   _bool makeVarRefAsFunction(const Token& tk, _genptr<_tim>& result, _p2& p2);

   template <typename T>
   _bool makeVarRefAsFunction(const Token& tk, _genptr<T>& result, _p2& p2)
   {
      return false;
   }

   _bool makeVarRef(const Token& tk, _defptr& result, _p2& p2);

   template <typename T>
   _bool makeVarRef(const Token& tk, _genptr<T>& result, _p2& p2)
   {
      Variable<T>* var;
      if (!p2.contexts.getVar(tk, var, p2)) {
         return false;
      }

      if (var->type == VarType::vt_Attribute
         && !p2.contexts.getFileContext()->attributeScope
         && tk.isVariable(STRINGS_ALTERABLE_ATTR, p2))
      {
         return makeVarRefAsFunction(tk, result, p2);
      }

      result = std::make_unique<VariableReference<T>>(var);
      return true;
   };
}

#endif // PARSE_VAR_H_INCLUDED
