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

#include "hash.h"
#include "var.h"


namespace perun2
{

typedef uint64_t _cunit;

_constexpr _cunit CACHE_NULL =                 0b00000000000000000000000000000000;
_constexpr _cunit CACHE_DESKTOP_PATH =         0b00000000000000000000000000000001;
_constexpr _cunit CACHE_EXE_PATH =             0b00000000000000000000000000000010;
_constexpr _cunit CACHE_CMD_PROCESS =          0b00000000000000000000000000000100;
_constexpr _cunit CACHE_ALPHABET =             0b00000000000000000000000000001000;
_constexpr _cunit CACHE_ASCII =                0b00000000000000000000000000010000;
_constexpr _cunit CACHE_ORIGIN =               0b00000000000000000000000000100000;
_constexpr _cunit CACHE_ARGUMENTS =            0b00000000000000000000000001000000;

// Perun2 offers access to some special constant values
// like path to the desktop
// they are rarely used, but when used, it is enough to load them only once
// they also may be expensive to get
// struct Cache solves all mentioned problems in an efficient way
// it only loads the values we need and ignores the rest

struct _p2;
struct VarsContext;


struct Cache
{
public:
   Cache() = delete;
   Cache(_p2& p2);

   void actualize(const _hash nameHash);
   void loadCmdPath();

   _str cmdProcessStartingArgs;

private:
   _bool isNotLoaded(const _cunit v);
   _str getCmdProcessStartingArgs() const;
   _list getAlphabet() const;

   _cunit value = CACHE_NULL;
   _p2& perun2;
   VarsContext& context;
};

}

#endif // CACHE_H_INCLUDED
