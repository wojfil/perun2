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

#ifndef WIN_PROGRAMS_DATA_H_INCLUDED
#define WIN_PROGRAMS_DATA_H_INCLUDED

#include "win-programs.h"


namespace perun2::prog
{


struct WP_Gimp : WinProgram
{
public:
   WP_Gimp(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
   p_riptr r_5;
};


}

#endif // WIN_PROGRAMS_H_INCLUDED
