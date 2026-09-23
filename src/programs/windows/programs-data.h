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

#include "win-programs.h"


namespace perun2::prog
{


struct WP_7zip : WinProgram
{
public:
   WP_7zip(Perun2Process& p2);
   void actualize() override;

private:
   StartMenuLnk startMenuLink;
   p_riptr r_1;
};


struct WP_Acrobat : WinProgram
{
public:
   WP_Acrobat(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
};


struct WP_Audacity : WinProgram
{
public:
   WP_Audacity(Perun2Process& p2);
   void actualize() override;

private:
   StartMenuLnk startMenuLink;
   p_riptr r_1;
   p_riptr r_2;
};


struct WP_Firefox : WinProgram
{
public:
   WP_Firefox(Perun2Process& p2);
   void actualize() override;

private:
   StartMenuLnk startMenuLink;
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
   p_riptr r_5;
};


struct WP_Gimp : WinProgram
{
public:
   WP_Gimp(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
   p_riptr r_5;
};


struct WP_Inkscape : WinProgram
{
public:
   WP_Inkscape(Perun2Process& p2);
   void actualize() override;

private:
   StartMenuLnk startMenuLink;
   p_riptr r_1;
};


struct WP_Notepad : WinProgram
{
public:
   WP_Notepad(Perun2Process& p2);
   void actualize() override;
};


struct WP_NotepadPlusPlus : WinProgram
{
public:
   WP_NotepadPlusPlus(Perun2Process& p2);
   void actualize() override;

private:
   StartMenuLnk startMenuLink;
   p_riptr r_1;
};


struct WP_OpenOffice : WinProgram
{
public:
   WP_OpenOffice(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
};


struct WP_Paint : WinProgram
{
public:
   WP_Paint(Perun2Process& p2);
   void actualize() override;
};


struct WP_Photoshop : WinProgram
{
public:
   WP_Photoshop(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
};


struct WP_Sumatra : WinProgram
{
public:
   WP_Sumatra(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
};


struct WP_Vlc : WinProgram
{
public:
   WP_Vlc(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
};


struct WP_WinRAR : WinProgram
{
public:
   WP_WinRAR(Perun2Process& p2);
   void actualize() override;

private:
   StartMenuLnk startMenuLink;
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
};


struct WP_Word : WinProgram
{
public:
   WP_Word(Perun2Process& p2);
   void actualize() override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
};


}
