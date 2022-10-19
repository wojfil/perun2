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

void makeWhereFilter(Generator<_bool>* boo, Attribute* attr, const _bool& hasMemory, _def*& result, uro::Uroboros* uro)
{
   result = new gen::Filter_WhereDef(result, boo, attr, hasMemory, uro);
}

void makeWhereFilter(Generator<_bool>* boo, _def*& result, uro::Uroboros* uro)
{
   result = new gen::Filter_WhereDef(result, boo, uro);
}

void makeNumericFilter(const Keyword& kw, Generator<_num>* num, _def*& result, uro::Uroboros* uro)
{
   switch (kw) {
      case Keyword::kw_Every: {
         result = new gen::Filter_EveryDef(result, num, uro);
         break;
      }
      case Keyword::kw_Final: {
         result = new gen::Filter_FinalDef(result, num, uro);
         break;
      }
      case Keyword::kw_Limit: {
         result = new gen::Filter_LimitDef(result, num, uro);
         break;
      }
      case Keyword::kw_Skip: {
         result = new gen::Filter_SkipDef(result, num, uro);
         break;
      }
   }
}

}
