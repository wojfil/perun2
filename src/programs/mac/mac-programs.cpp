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

#include "../../../include/perun2/programs/mac/mac-programs.hpp"
#include "../../../include/perun2/os/os.hpp"
#include "../../../include/perun2/perun2.hpp"


namespace perun2::prog
{
   

MacPrograms::MacPrograms(Perun2Process& p2) : perun2(p2) { };


void MacPrograms::actualize(const Token& tk)
{
   // todo
}


p_bool MacPrograms::haveName(const Token& tk) const
{
   // todo
   return false;
}


void MacPrograms::insertVars(GlobalContext& ctx) const
{
   // todo
}


}
