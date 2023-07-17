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

#ifndef GEN_OS_GEN_H_INCLUDED
#define GEN_OS_GEN_H_INCLUDED

#include "../datatype.h"
#include "gen-string.h"


namespace perun2
{
  struct p_perun2;
}

namespace perun2::gen
{

enum OsElement
{
   oe_None = 0,
   oe_All,
   oe_Directories,
   oe_Files,
   oe_RecursiveAll,
   oe_RecursiveDirectories,
   oe_RecursiveFiles
};


struct DefinitionGenerator
{
public:
   DefinitionGenerator() = delete;
   DefinitionGenerator(const OsElement el, p_perun2& p2)
      : element_(el), perun2(p2) { };
   p_bool generate(_defptr& result) const;

private:
   p_perun2& perun2;
   const OsElement element_;
};

}

#endif // GEN_OS_GEN_H_INCLUDED
