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

#include "gen-os-gen.h"
#include "gen-os.h"
#include "gen-string.h"


namespace perun2::gen
{

#define P_GEN_OS_ARGS_DEFAULT loc, this->perun2, PathType::Relative, p_str()
#define P_GEN_OS_ARGS_DEFAULT_EXT loc, this->perun2, gen::os::DEFAULT_PATTERN, PathType::Relative, p_str()


p_bool DefinitionGenerator::generate(p_defptr& result) const
{
   p_genptr<p_str> loc = std::make_unique<LocationReference>(this->perun2);

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
};


}
