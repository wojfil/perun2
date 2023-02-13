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

#define UNICODE
#define _UNICODE
#include <locale>
#include <clocale>
#include <cstdlib>
#include <cwctype>
#include <combaseapi.h>
#include <iostream>
#include <fcntl.h>
#include "uroboros.h"


namespace uro
{
   void consoleInit()
   {
      std::wcin.tie(0);
      std::wcout.tie(0);
      std::setlocale(LC_CTYPE, "");
      _setmode(_fileno(stdout), _O_U8TEXT);
      CoInitializeEx(0, COINIT_MULTITHREADED);
   }
}

uro::_exitint wmain(uro::_int argc, uro::_char* argv[], uro::_char* envp[])
{
   uro::consoleInit();
   uro::Uroboros2 uroboros(argc, argv);
   uroboros.run();
   return uroboros.getExitCode();
}
