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

#include "parse-var.hpp"
#include "../../perun2.hpp"
#include "../function/func-attr.hpp"


namespace perun2::parse
{
   p_bool isAlterableAttribute(const Token& tk, Perun2Process& p2)
   {
      return !p2.contexts.getFileContext()->attributeScope
         && tk.isVariable(STRINGS_ALTERABLE_ATTR);
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_bool>& result, Perun2Process& p2)
   {
      if (tk.isVariable(STRING_ARCHIVE)) {
         result = std::make_unique<func::F_Attr_Archive>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_COMPRESSED)) {
         result = std::make_unique<func::F_Attr_Compressed>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_EMPTY)) {
         result = std::make_unique<func::F_Attr_Empty>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_EXISTS)) {
         result = std::make_unique<func::F_Attr_Exists>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ENCRYPTED)) {
         result = std::make_unique<func::F_Attr_Encrypted>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_HIDDEN)) {
         result = std::make_unique<func::F_Attr_Hidden>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ISDIRECTORY)) {
         result = std::make_unique<func::F_Attr_IsDirectory>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ISFILE)) {
         result = std::make_unique<func::F_Attr_IsFile>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_READONLY)) {
         result = std::make_unique<func::F_Attr_Readonly>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ISIMAGE)) {
         result = std::make_unique<func::F_Attr_IsImage>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_ISVIDEO)) {
         result = std::make_unique<func::F_Attr_IsVideo>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_num>& result, Perun2Process& p2)
   {
      if (tk.isVariable(STRING_SIZE)) {
         result = std::make_unique<func::F_Attr_Size>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_WIDTH)) {
         result = std::make_unique<func::F_Attr_Width>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_HEIGHT)) {
         result = std::make_unique<func::F_Attr_Height>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_per>& result, Perun2Process& p2)
   {
      if (tk.isVariable(STRING_LIFETIME)) {
         result = std::make_unique<func::F_Attr_Lifetime>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_DURATION)) {
         result = std::make_unique<func::F_Attr_Duration>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRefAsFunction(const Token& tk, p_genptr<p_tim>& result, Perun2Process& p2)
   {
      if (tk.isVariable(STRING_ACCESS)) {
         result = std::make_unique<func::F_Attr_Access>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_CHANGE)) {
         result = std::make_unique<func::F_Attr_Change>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_CREATION)) {
         result = std::make_unique<func::F_Attr_Creation>(p2);
         return true;
      }
      else if (tk.isVariable(STRING_MODIFICATION)) {
         result = std::make_unique<func::F_Attr_Modification>(p2);
         return true;
      }

      return false;
   }

   p_bool makeVarRef(const Token& tk, p_defptr& result, Perun2Process& p2)
   {
      p_str var = tk.toLowerString();
      auto v = p2.contexts.osGenerators.find(var);

      if (v != p2.contexts.osGenerators.end()) {
         return v->second.generate(result);
      }

      return false;
   }
}
