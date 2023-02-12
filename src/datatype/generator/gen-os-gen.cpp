/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "gen-os-gen.h"
#include "gen-string.h"


namespace uro::gen
{

#define P_GEN_OS_ARGS_DEFAULT loc, this->uroboros, gen::os::IS_RELATIVE_PATH, gen::os::NO_PREFIX
#define P_GEN_OS_ARGS_DEFAULT_EXT loc, this->uroboros, gen::os::DEFAULT_PATTERN, gen::os::IS_RELATIVE_PATH, gen::os::NO_PREFIX
   

_bool DefinitionGenerator::generate(_defptr& result) const
{
   _genptr<_str> loc(new LocationReference(this->uroboros));

   switch (this->element_) {
      case OsElement::oe_All: {
         result = std::make_unique<All>(P_GEN_OS_ARGS_DEFAULT_EXT);
         break;
      }
      case OsElement::oe_Directories: {
         result = std::make_unique<Directories>(P_GEN_OS_ARGS_DEFAULT_EXT);
         break;
      }
      case OsElement::oe_Files: {
         result = std::make_unique<Files>(P_GEN_OS_ARGS_DEFAULT_EXT);
         break;
      }
      case OsElement::oe_RecursiveFiles: {
         result = std::make_unique<RecursiveFiles>(P_GEN_OS_ARGS_DEFAULT);
         break;
      }
      case OsElement::oe_RecursiveDirectories: {
         result = std::make_unique<RecursiveDirectories>(P_GEN_OS_ARGS_DEFAULT);
         break;
      }
      case OsElement::oe_RecursiveAll: {
         result = std::make_unique<RecursiveAll>(P_GEN_OS_ARGS_DEFAULT);
         break;
      }
      default: {
         return false;
      }
   }

   return true;
}


}
