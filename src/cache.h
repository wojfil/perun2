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

#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include "var.h"
#include "programs/windows/programs.h"


namespace perun2
{

typedef uint64_t p_cunit;

p_constexpr p_cunit CACHE_NULL =                 0b00000000000000000000000000000000;
p_constexpr p_cunit CACHE_DESKTOP_PATH =         0b00000000000000000000000000000001;
p_constexpr p_cunit CACHE_EXE_PATH =             0b00000000000000000000000000000010;
p_constexpr p_cunit CACHE_CMD_PROCESS =          0b00000000000000000000000000000100;
p_constexpr p_cunit CACHE_ALPHABET =             0b00000000000000000000000000001000;
p_constexpr p_cunit CACHE_ASCII =                0b00000000000000000000000000010000;
p_constexpr p_cunit CACHE_ORIGIN =               0b00000000000000000000000000100000;
p_constexpr p_cunit CACHE_ARGUMENTS =            0b00000000000000000000000001000000;
p_constexpr p_cunit CACHE_PENDRIVES =            0b00000000000000000000000010000000;

// Perun2 offers access to some special constant values
// like path to the desktop
// they are rarely used, but when used, it is enough to load them only once
// struct Cache solves all mentioned problems
// it loads the values we need only once and ignores the rest

struct p_perun2;
struct VarsContext;
struct Token;


struct Cache
{
public:
   Cache() = delete;
   Cache(p_perun2& p2);

   void actualize(const Token& tk);
   void loadCmdPath();

   p_str cmdProcessStartingArgs;
   prog::WinPrograms programs; 

private:
   p_bool isNotLoaded(const p_cunit v);
   p_str getCmdProcessStartingArgs() const;
   p_list getAlphabet() const;

   p_cunit value = CACHE_NULL;
   p_perun2& perun2;
   VarsContext& context;
};

}

#endif // CACHE_H_INCLUDED
