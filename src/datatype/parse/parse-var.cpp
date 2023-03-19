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

   _bool makeVarRefAsFunction(const Token& tk, _genptr<_bool>& result, _p2& p2)
   {
      const _hash h = tk.value.word.h;

      if (h == p2.hashes.HASH_VAR_ARCHIVE) {
         result = std::make_unique<func::F_Attr_Archive>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_COMPRESSED) {
         result = std::make_unique<func::F_Attr_Compressed>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_EMPTY) {
         result = std::make_unique<func::F_Attr_Empty>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_EXISTS) {
         result = std::make_unique<func::F_Attr_Exists>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_ENCRYPTED) {
         result = std::make_unique<func::F_Attr_Encrypted>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_HIDDEN) {
         result = std::make_unique<func::F_Attr_Hidden>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_ISDIRECTORY) {
         result = std::make_unique<func::F_Attr_IsDirectory>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_ISFILE) {
         result = std::make_unique<func::F_Attr_IsFile>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_READONLY) {
         result = std::make_unique<func::F_Attr_Readonly>(p2);
         return true;
      }

      return false;
   }

   _bool makeVarRefAsFunction(const Token& tk, _genptr<_num>& result, _p2& p2)
   {
      const _hash h = tk.value.word.h;
      if (h == p2.hashes.HASH_VAR_SIZE) {
         result = std::make_unique<func::F_Attr_Size>(p2);
         return true;
      }

      return false;
   }

   _bool makeVarRefAsFunction(const Token& tk, _genptr<_per>& result, _p2& p2)
   {
      const _hash h = tk.value.word.h;
      if (h == p2.hashes.HASH_VAR_LIFETIME) {
         result = std::make_unique<func::F_Attr_Lifetime>(p2);
         return true;
      }

      return false;
   }

   _bool makeVarRefAsFunction(const Token& tk, _genptr<_tim>& result, _p2& p2)
   {
      const _hash h = tk.value.word.h;

      if (h == p2.hashes.HASH_VAR_ACCESS) {
         result = std::make_unique<func::F_Attr_Access>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_CHANGE) {
         result = std::make_unique<func::F_Attr_Change>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_CREATION) {
         result = std::make_unique<func::F_Attr_Creation>(p2);
         return true;
      }
      else if (h == p2.hashes.HASH_VAR_MODIFICATION) {
         result = std::make_unique<func::F_Attr_Modification>(p2);
         return true;
      }

      return false;
   }

   _bool makeVarRef(const Token& tk, _defptr& result, _p2& p2)
   {
      const _hash var = tk.value.word.h;
      auto v = p2.contexts.osGenerators.find(var);

      if (v != p2.contexts.osGenerators.end()) {
         return v->second.generate(result);
      }

      return false;
   }
}
