/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_PRINT_H_INCLUDED
#define COM_PRINT_H_INCLUDED

#include "../datatype/primitives.h"
#include "../uroboros.h"
#include "../os.h"
#include <iostream>


namespace uro::comm
{

inline constexpr _int GUI_WAIT_LIMIT = 219;
inline constexpr _int GUI_WAIT_TIME = 40;

extern _int g_guiMes;

void rawPrint(const _str& value);
void print(const _uro& uro, const _str& value);
void commandLog(const _uro& uro, const _str& value);

template<typename... Args>
void commandLog(const _uro& uro, Args const&... args)
{
   if (!(uro.flags & FLAG_SILENT)) {
      using value_type = std::common_type_t<Args const&...>;
      for (auto const &arg : {static_cast<value_type>(args)...}) {
         std::wcout << arg;
      }

      std::wcout << CHAR_NEW_LINE;

      if (uro.flags & FLAG_GUI) {
      // if this program was started by the GUI application
      // all logs are sent to GUI's TextField and printed there
      // sometimes, the flow of messages may slow GUI down
      // solution? from time to time, sleep for few milliseconds
      // as a result, GUI app never lags nor freezes
      // a bit of efficiency is sacrificed for the sake of user convenience
      // very ugly solution indeed
      // delete this if a better solution is found
         g_guiMes++;
         if (g_guiMes == GUI_WAIT_LIMIT) {
            g_guiMes = 0;
            os_rawSleepForMs(GUI_WAIT_TIME);
         }
      }
   }
}

}

#endif // COM_PRINT_H_INCLUDED
