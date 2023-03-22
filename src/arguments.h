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

#ifndef ARGUMENTS_H_INCLUDED
#define ARGUMENTS_H_INCLUDED

#include "datatype/datatype.h"

namespace perun2
{

_constexpr _uint32 FLAG_NULL =      0b000;
_constexpr _uint32 FLAG_NOOMIT =    0b001;
_constexpr _uint32 FLAG_SILENT =    0b010;
_constexpr _uint32 FLAG_GUI =       0b100;

_constexpr _char CHAR_FLAG_GUI =              CHAR_g;
_constexpr _char CHAR_FLAG_GUI_UPPER =        CHAR_G;

_constexpr _char CHAR_FLAG_NOOMIT =           CHAR_n;
_constexpr _char CHAR_FLAG_NOOMIT_UPPER =     CHAR_N;

_constexpr _char CHAR_FLAG_SILENT =           CHAR_s;
_constexpr _char CHAR_FLAG_SILENT_UPPER =     CHAR_S;

_constexpr _char CHAR_FLAG_DIRECTORY =        CHAR_d;
_constexpr _char CHAR_FLAG_DIRECTORY_UPPER =  CHAR_D;

_constexpr _char CHAR_FLAG_HERE =             CHAR_h;
_constexpr _char CHAR_FLAG_HERE_UPPER =       CHAR_H;

_constexpr _char CHAR_FLAG_CODE =             CHAR_c;
_constexpr _char CHAR_FLAG_CODE_UPPER =       CHAR_C;

_const _str STRING_ARG_VERSION =              str(STRING_CHAR_MINUS, STRING_CHAR_MINUS, STRING_VERSION);
_const _str STRING_ARG_DOCS =                 str(STRING_CHAR_MINUS, STRING_CHAR_MINUS, STRING_DOCS);
_const _str STRING_ARG_WEBSITE =              str(STRING_CHAR_MINUS, STRING_CHAR_MINUS, STRING_WEBSITE);
_const _str STRING_ARG_HELP =                 str(STRING_CHAR_MINUS, STRING_CHAR_MINUS, STRING_HELP);


enum ArgsParseState 
{
   aps_Ok = 0,
   aps_PrintInfo,
   aps_Failed
};

struct Arguments
{
public:

   Arguments() = delete;

   // Perun2 arguments parsed from the command-line
   Arguments(const _int argc, _char* const argv[]);

   // Perun2 arguments made from code and location
   Arguments(const _str& loc, const _str& cod);

   // Perun2 arguments made from code, location and flags
   Arguments(const _str& loc, const _str& cod, const _uint32 fls);

   _uint32 getFlags() const;
   _list getArgs() const;
   _str getLocation() const;
   const _str& getCode() const;
   ArgsParseState getParseState() const;
   _bool hasFlag(const _uint32 flag) const;

private:

   _str code;
   _uint32 flags = FLAG_NULL;
   _list args;
   _str location;
   ArgsParseState parseState = ArgsParseState::aps_Failed;
};

}

#endif // ARGUMENTS_H_INCLUDED
