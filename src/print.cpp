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
#include "os.h"


namespace uro
{

_int g_guiMes;

void rawPrint(const _str& value)
{
   std::wcout << value << L"\n";
}

void print(const Uroboros& uro,const _str& value)
{
   std::wcout << value << L"\n";
   if (uro.flags & FLAG_GUI) {
      g_guiMes++;
      if (g_guiMes == GUIMES_LIMIT) {
         g_guiMes = 0;
         Sleep(GUIMES_TIME);
      }
   }
}

void commandLog(const Uroboros& uro,const _str& value)
{
   if (!(uro.flags & FLAG_SILENT)) {
      std::wcout << value << L"\n";
      if (uro.flags & FLAG_GUI) {
         g_guiMes++;
         if (g_guiMes == GUIMES_LIMIT) {
            g_guiMes = 0;
            Sleep(GUIMES_TIME);
         }
      }
   }
}

}
