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

#include "../../datatype/generator.hpp"
#include "../../logic.hpp"


namespace perun2::prog
{

struct StartMenuLnk : Generator<p_str>
{
public:
   StartMenuLnk() = delete;
   StartMenuLnk(const p_str& path);
   
   p_bool hasValue();
   p_str getValue() override;

private:
   p_str getPathRoot(const p_bool allUsers) const;
   p_bool getFilePath(p_str& path) const;

   const p_str relativePath;
   Logic state = Logic::Unknown;
   p_str result;
};


}
