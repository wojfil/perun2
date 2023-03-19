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


namespace perun2::parse
{
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
