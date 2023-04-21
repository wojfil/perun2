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

typedef uint32_t _flags;

_constexpr _flags FLAG_NULL =                 0b0000;
_constexpr _flags FLAG_NOOMIT =               0b0001;
_constexpr _flags FLAG_SILENT =               0b0010;
_constexpr _flags FLAG_GUI =                  0b0100;

_constexpr _char CHAR_FLAG_GUI =              CHAR_g;
_constexpr _char CHAR_FLAG_NOOMIT =           CHAR_n;
_constexpr _char CHAR_FLAG_SILENT =           CHAR_s;
_constexpr _char CHAR_FLAG_DIRECTORY =        CHAR_d;
_constexpr _char CHAR_FLAG_HERE =             CHAR_h;
_constexpr _char CHAR_FLAG_CODE =             CHAR_c;

_constexpr _char CHAR_FLAG_GUI_UPPER =        CHAR_G;
_constexpr _char CHAR_FLAG_NOOMIT_UPPER =     CHAR_N;
_constexpr _char CHAR_FLAG_SILENT_UPPER =     CHAR_S;
_constexpr _char CHAR_FLAG_DIRECTORY_UPPER =  CHAR_D;
_constexpr _char CHAR_FLAG_HERE_UPPER =       CHAR_H;
_constexpr _char CHAR_FLAG_CODE_UPPER =       CHAR_C;


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
   Arguments(const _str& loc, const _str& cod, const _flags fls);

   _flags getFlags() const;
   _list getArgs() const;
   _str getLocation() const;
   const _str& getCode() const;
   ArgsParseState getParseState() const;
   _bool hasFlag(const _flags flag) const;

private:

   _str code;
   _flags flags = FLAG_NULL;
   _list args;
   _str location;
   ArgsParseState parseState = ArgsParseState::aps_Failed;
};

}

#endif // ARGUMENTS_H_INCLUDED
