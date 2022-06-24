/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#include "print.h"
#include <iostream>
#include "uroboros.h"
#include "os.h"
#define GUIMES_LIMIT 219
#define GUIMES_TIME 40

_int g_guiMes;

void commandLog(const _str& value)
{
   if (!(g_flags & FLAG_SILENT)) {
      print(value);
   }
}

void print(const _str& value)
{
   std::wcout << value << std::endl;
   if (g_flags & FLAG_GUI) {
      // this is ugly solution for a GUI unsolvable bug in Windows
      // (user interaction sometimes gets frozen while runtime)
      // delete this if the bug is solved properly
      g_guiMes++;
      if (g_guiMes == GUIMES_LIMIT) {
         g_guiMes = 0;
         Sleep(GUIMES_TIME);
      }
   }
}
