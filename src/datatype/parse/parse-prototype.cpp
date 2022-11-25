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

#include "parse-prototype.h"
#include "../generator/gen-definition.h"


namespace uro::parse
{

void makeWhereFilter(_genptr<_bool>& boo, _attrptr& attr, const _bool& hasMemory, _defptr& result, Uroboros& uro)
{
   result = std::make_unique<gen::Filter_WhereDef>(result, boo, attr, hasMemory, uro);
}

void makeWhereFilter(_genptr<_bool>& boo, _defptr& result, Uroboros& uro)
{
   result = std::make_unique<gen::Filter_WhereDef>(result, boo, uro);
}

void makeNumericFilter(const Keyword& kw, _genptr<_num>& num, _defptr& result, Uroboros& uro)
{
   _defptr prev = std::move(result);

   switch (kw) {
      case Keyword::kw_Every: {
         result = std::make_unique<gen::Filter_EveryDef>(prev, num, uro);
         break;
      }
      case Keyword::kw_Final: {
         result = std::make_unique<gen::Filter_FinalDef>(prev, num, uro);
         break;
      }
      case Keyword::kw_Limit: {
         result = std::make_unique<gen::Filter_LimitDef>(prev, num, uro);
         break;
      }
      case Keyword::kw_Skip: {
         result = std::make_unique<gen::Filter_SkipDef>(prev, num, uro);
         break;
      }
   }
}

}
