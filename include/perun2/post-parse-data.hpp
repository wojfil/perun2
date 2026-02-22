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

#include "var.hpp"
#include "logic.hpp"
#include "side-process.hpp"
#include "programs/programs.hpp"


namespace perun2
{

typedef uint64_t     p_cunit;

p_constexpr p_cunit CONST_CACHE_NULL =                 0;
p_constexpr p_cunit CONST_CACHE_DESKTOP_PATH =         1 << 0;
p_constexpr p_cunit CONST_CACHE_EXE_PATH =             1 << 1;
p_constexpr p_cunit CONST_CACHE_CMD_PROCESS =          1 << 2;
p_constexpr p_cunit CONST_CACHE_ALPHABET =             1 << 3;
p_constexpr p_cunit CONST_CACHE_ASCII =                1 << 4;
p_constexpr p_cunit CONST_CACHE_ORIGIN =               1 << 5;
p_constexpr p_cunit CONST_CACHE_ARGUMENTS =            1 << 6;
p_constexpr p_cunit CONST_CACHE_PENDRIVES =            1 << 7;
p_constexpr p_cunit CONST_CACHE_DOWNLOADS_PATH =       1 << 8;
p_constexpr p_cunit CONST_CACHE_P3_ASKER_PATH =        1 << 9;
p_constexpr p_cunit CONST_CACHE_P3_ANALYZER_PATH =     1 << 10;

// Perun2 offers access to some special constant values
// like path to the desktop
// they are rarely used, but when used, it is enough to load them only once
// the struct PostParseData loads the values we need for fast access

struct Perun2Process;
struct VarsContext;
struct Token;


struct PostParseData
{
public:
   PostParseData() = delete;
   PostParseData(Perun2Process& p2);

   void actualize(const Token& tk);
   void loadCmdPath();
   Python3State getPython3State(p_str& cmdPath);
   p_str getPython3AskerPath();
   p_str getPython3AnalyzerPath();

   p_str cmdProcessStartingArgs;
   prog::Programs programs; 

private:
   p_bool isNotLoaded(const p_cunit v);
   p_str getCmdProcessStartingArgs() const;
   p_list getAlphabet() const;

   p_cunit value = CONST_CACHE_NULL;
   Perun2Process& perun2;
   VarsContext& context;
   p_str python3CmdPath;
   Python3State python3State = Python3State::P3_Unknown;
};

}
