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

#pragma once

#include "../datatype.h"
#include "gen-string.h"


namespace perun2
{
  struct Perun2Process;
}

namespace perun2::gen
{

enum OsElement
{
   oe_None = 0,
   oe_All,
   oe_Directories,
   oe_Files,
   oe_Images,
   oe_Videos,
   oe_RecursiveAll,
   oe_RecursiveDirectories,
   oe_RecursiveFiles,
   oe_RecursiveImages,
   oe_RecursiveVideos
};


struct DefinitionGenerator
{
public:
   DefinitionGenerator() = delete;
   DefinitionGenerator(const OsElement el, Perun2Process& p2)
      : element_(el), perun2(p2) { };
   p_bool generate(p_defptr& result) const;

private:
   Perun2Process& perun2;
   const OsElement element_;
};

}
