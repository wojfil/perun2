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

#include "parse-var.h"
#include "../../perun2.h"
#include "../function/func-attr.h"


namespace perun2::parse
{

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_bool>& result, p_perun2& p2)
   {
      if (tk.isVariable(STRING_ARCHIVE, p2)) {
         result = std::make_unique<func::F_Attr_Archive>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_COMPRESSED, p2)) {
         result = std::make_unique<func::F_Attr_Compressed>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_EMPTY, p2)) {
         result = std::make_unique<func::F_Attr_Empty>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_EXISTS, p2)) {
         result = std::make_unique<func::F_Attr_Exists>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ENCRYPTED, p2)) {
         result = std::make_unique<func::F_Attr_Encrypted>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_HIDDEN, p2)) {
         result = std::make_unique<func::F_Attr_Hidden>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ISDIRECTORY, p2)) {
         result = std::make_unique<func::F_Attr_IsDirectory>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ISFILE, p2)) {
         result = std::make_unique<func::F_Attr_IsFile>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_READONLY, p2)) {
         result = std::make_unique<func::F_Attr_Readonly>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_num>& result, p_perun2& p2)
   {
      if (tk.isVariable(STRING_SIZE, p2)) {
         result = std::make_unique<func::F_Attr_Size>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_per>& result, p_perun2& p2)
   {
      if (tk.isVariable(STRING_LIFETIME, p2)) {
         result = std::make_unique<func::F_Attr_Lifetime>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_tim>& result, p_perun2& p2)
   {
      if (tk.isVariable(STRING_ACCESS, p2)) {
         result = std::make_unique<func::F_Attr_Access>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_CHANGE, p2)) {
         result = std::make_unique<func::F_Attr_Change>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_CREATION, p2)) {
         result = std::make_unique<func::F_Attr_Creation>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_MODIFICATION, p2)) {
         result = std::make_unique<func::F_Attr_Modification>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRef(const Token& tk, p_defptr& result, p_perun2& p2)
   {
      p_str var = tk.toLowerString(p2);
      auto v = p2.contexts.osGenerators.find(var);

      if (v != p2.contexts.osGenerators.end()) {
         return v->second.generate(result);
      }

      return false;
   }
}
