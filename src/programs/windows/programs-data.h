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

#include "programs.h"


namespace perun2::prog
{


struct WP_7zip : WinProgram
{
public:
   WP_7zip(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
};


struct WP_Acrobat : WinProgram
{
public:
   WP_Acrobat(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
};


struct WP_Audacity : WinProgram
{
public:
   WP_Audacity(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
};


struct WP_Firefox : WinProgram
{
public:
   WP_Firefox(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
   p_riptr r_5;
};


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


struct WP_Notepad : WinProgram
{
public:
   WP_Notepad(p_perun2& p2);
   void actualize(const Token& tk) override;
};


struct WP_NotepadPlusPlus : WinProgram
{
public:
   WP_NotepadPlusPlus(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
};


struct WP_OpenOffice : WinProgram
{
public:
   WP_OpenOffice(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
};


struct WP_Paint : WinProgram
{
public:
   WP_Paint(p_perun2& p2);
   void actualize(const Token& tk) override;
};


struct WP_Vlc : WinProgram
{
public:
   WP_Vlc(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
};


struct WP_WinRAR : WinProgram
{
public:
   WP_WinRAR(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
   p_riptr r_4;
};


struct WP_Word : WinProgram
{
public:
   WP_Word(p_perun2& p2);
   void actualize(const Token& tk) override;

private:
   p_riptr r_1;
   p_riptr r_2;
   p_riptr r_3;
};


}

#endif // WIN_PROGRAMS_H_INCLUDED
