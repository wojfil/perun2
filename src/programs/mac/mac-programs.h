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

#include "../../token.h"


namespace perun2
{
   struct GlobalContext;
   struct Perun2Process;
}

namespace perun2::prog
{


struct MacPrograms
{
public:
   MacPrograms() = delete;
   MacPrograms(Perun2Process& p2);

   void actualize(const Token& tk);
   p_bool haveName(const Token& tk) const;
   void insertVars(GlobalContext& ctx) const;

private:
   Perun2Process& perun2;
};


}
