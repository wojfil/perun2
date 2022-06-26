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

#define UNICODE
#define _UNICODE
#include <locale>
#include <clocale>
#include <cstdlib>
#include <cwctype>
#include <combaseapi.h>
#include <iostream>
#include "datatype/primitives.h"
#include "uroboros.h"
#include "arguments.h"
#include "os.h"
#include <fcntl.h>


BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType)
{
  switch (dwCtrlType) {
    case CTRL_C_EVENT:
      g_running = false;
      if (g_process) {
         g_process = false;
         TerminateProcess(g_processInfo.hProcess, 0);
      }
      return TRUE;
    default:
      return FALSE;
    }
}


void main_init()
{
   std::wcin.tie(0);
   std::wcout.tie(0);
   std::setlocale(LC_CTYPE, "");
   SetConsoleCtrlHandler(HandlerRoutine, TRUE);
   _setmode(_fileno(stdout), _O_U8TEXT);
   CoInitializeEx(0, COINIT_MULTITHREADED);
}


_int wmain(_int argc, _char* argv[], _char *envp[])
{
   main_init();

   Arguments arguments(&argc, &argv);

   switch (arguments.getParseState())
   {
      case ArgsParseState::aps_Failed:
      {
         return EXITCODE_CLI_ERROR;
      }
      case ArgsParseState::aps_PrintInfo:
      {
         return EXITCODE_OK;
      }
   }

   run(arguments);
   return g_exitCode;
}
