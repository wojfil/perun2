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

typedef uint32_t     p_flags;

p_constexpr p_flags FLAG_NULL =                 0;
p_constexpr p_flags FLAG_NOOMIT =               1 << 0;
p_constexpr p_flags FLAG_SILENT =               1 << 1;
p_constexpr p_flags FLAG_GUI =                  1 << 2;

p_constexpr p_char CHAR_FLAG_GUI =              CHAR_g;
p_constexpr p_char CHAR_FLAG_NOOMIT =           CHAR_n;
p_constexpr p_char CHAR_FLAG_SILENT =           CHAR_s;
p_constexpr p_char CHAR_FLAG_DIRECTORY =        CHAR_d;
p_constexpr p_char CHAR_FLAG_HERE =             CHAR_h;
p_constexpr p_char CHAR_FLAG_CODE =             CHAR_c;

p_constexpr p_char CHAR_FLAG_GUI_UPPER =        CHAR_G;
p_constexpr p_char CHAR_FLAG_NOOMIT_UPPER =     CHAR_N;
p_constexpr p_char CHAR_FLAG_SILENT_UPPER =     CHAR_S;
p_constexpr p_char CHAR_FLAG_DIRECTORY_UPPER =  CHAR_D;
p_constexpr p_char CHAR_FLAG_HERE_UPPER =       CHAR_H;
p_constexpr p_char CHAR_FLAG_CODE_UPPER =       CHAR_C;


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
   Arguments(const p_int argc, p_char* const argv[]);

   // Perun2 arguments made from location and code
   Arguments(const p_str& loc, const p_str& cod);

   // Perun2 arguments made from location, code and flags
   Arguments(const p_str& loc, const p_str& cod, const p_flags fls);

   p_bool areGood() const;
   p_flags getFlags() const;
   p_list getArgs() const;
   p_str getLocation() const;
   const p_str& getCode() const;
   ArgsParseState getParseState() const;
   p_bool hasFlag(const p_flags flag) const;

private:
   p_str code;
   p_flags flags = FLAG_NULL;
   p_list args;
   p_str location;
   ArgsParseState parseState = ArgsParseState::aps_Failed;
};

}

#endif // ARGUMENTS_H_INCLUDED
