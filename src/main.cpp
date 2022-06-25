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
#include "print.h"
#include "os.h"
#include <fcntl.h>

#define VERSION_STR L"1.0.6"


_boo main_parseArgs(_int* argc, _char** argv[], _uint32& flags, _list& args, _str& location, _str& code);

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


_int wmain(_int argc, _char* argv[], _char *envp[])
{
   std::wcin.tie(0);
   std::wcout.tie(0);
   std::setlocale(LC_CTYPE, "");
   SetConsoleCtrlHandler(HandlerRoutine, TRUE);
   _setmode(_fileno(stdout), _O_U8TEXT);
   CoInitializeEx(0, COINIT_MULTITHREADED);

   if (argc == 1) {
      print(L"Command-line error: missing arguments. Run 'uro --help' for command-line tips.");
      return EXITCODE_CLI_ERROR;
   }

   _uint32 flags;
   _list args;
   _str location;
   _str code;

   if (main_parseArgs(&argc, &argv, flags, args, location, code))
   {
      run(location, code, flags, args);
   }

   return g_exitCode;
}

void main_version()
{
   print(L"Uroboros " VERSION_STR);
}

void main_docs()
{
   os_showWebsite(L"https://uroboros-lang.org/docs");
}

void main_website()
{
   os_showWebsite(L"https://uroboros-lang.org");
}

void main_help()
{
   print(L"");
   print(L"In order to run a script, pass a file name or its path as an argument. Extension is not mandatory.");
   print(L"By default, working location is the directory where the script is located.");
   print(L"");
   print(L"Options:");
   print(L"  --help       Display this information again.");
   print(L"  --version    Display interpreter version information.");
   print(L"  --website    Enter the official Uroboros website.");
   print(L"  --docs       Enter the official Uroboros documentation.");
   print(L"  -c           Pass Uroboros code as an argument instead of file name.");
   print(L"  -d <name>    Set working location to certain value.");
   print(L"  -h           Set working location to the place where this command was called from.");
   print(L"  -n           Run in noomit mode (iterate over all file system elements with no exceptions).");
   print(L"  -s           Run in silent mode (no command log messages).");
   print(L"  -g           For internal use only. Set up log message communication with GUI.");
}

_boo main_parseArgs(_int* argc, _char** argv[], _uint32& flags, _list& args, _str& location, _str& code)
{
   _boo options = true;
   _boo nextParseLocation = false;
   _boo hasCode = false;
   _boo hasValue = false;
   _boo here = false;
   _str value;
   _boo d_has = false;
   _str d_value;

   for (_int i = 1; i < *argc; i++) {
      const _str arg = _str((*argv)[i]);
      const _size len = arg.size();

      if (options && len >= 2 && arg[0] == L'-') {
         if (arg[1] == L'-') {
            if (len == 2) {
               options = false;
               continue;
            }

            _str lowerArg = arg;
            toLower(lowerArg);

            if (lowerArg == L"--version") {
               main_version();
            }
            else if (lowerArg == L"--docs") {
               main_docs();
            }
            else if (lowerArg == L"--website") {
               main_website();
            }
            else if (lowerArg == L"--help") {
               main_help();
            }
            else {
               print(str(L"Command-line error: unknown option '", arg.substr(2), L"'."));
               g_exitCode = EXITCODE_CLI_ERROR;
            }
            return false;
         }
         else {
            for (_size j = 1; j < len; j++) {
               switch (arg[j]) {
                  case L'c': case L'C': {
                     hasCode = true;
                     break;
                  }
                  case L'd': case L'D': {
                     nextParseLocation = true;
                     break;
                  }
                  case L'n': case L'N': {
                     if (!(flags & FLAG_NOOMIT)) {
                        flags |= FLAG_NOOMIT;
                     }
                     break;
                  }
                  case L's': case L'S': {
                     if (!(flags & FLAG_SILENT)) {
                        flags |= FLAG_SILENT;
                     }
                     break;
                  }
                  case L'g': case L'G': {
                     if (!(flags & FLAG_GUI)) {
                        flags |= FLAG_GUI;
                     }
                     break;
                  }
                  case L'h': case L'H': {
                     here = true;
                     break;
                  }
                  default: {
                     print(str(L"Command-line error: unknown option '", charStr(arg[j]), L"'."));
                     g_exitCode = EXITCODE_CLI_ERROR;
                     return false;
                  }
               }
            }
         }
         continue;
      }

      if (nextParseLocation) {
         const _str v = os_trim(arg);

         if (v.empty()) {
            continue;
         }

         d_value = v;
         d_has = true;
         nextParseLocation = false;
         continue;
      }

      if (hasValue) {
         args.push_back(arg);
      }
      else {
         hasValue = true;
         value = arg;
      }
   }

   if (nextParseLocation) {
      print(L"Command-line error: destination directory has not been defined.");
      g_exitCode = EXITCODE_CLI_ERROR;
      return false;
   }

   if (!hasValue) {
      print(L"Command-line error: an argument is missing.");
      g_exitCode = EXITCODE_CLI_ERROR;
      return false;
   }

   const _str cdLocation = os_currentPath();

   if (hasCode) {
      code = value;
      if (d_has) {
         if (os_isAbsolute(d_value)) {
            location = d_value;
         }
         else {
            location = str(cdLocation, OS_SEPARATOR_STRING, d_value);
         }
      }
      else {
         location = cdLocation;
      }
   }
   else {
      _str filePath = os_trim(value);
      if (filePath.empty()) {
         print(L"Command-line error: no input file.");
         g_exitCode = EXITCODE_CLI_ERROR;
         return false;
      }

      if (!os_isAbsolute(filePath)) {
         filePath = str(cdLocation, OS_SEPARATOR_STRING, filePath);
      }

      if (os_hasExtension(filePath)) {
         if (os_extension(filePath) != OS_UROEXT) {
            print(str(L"Command-line error: wrong input file extension. Only '", OS_UROEXT, L"' is allowed."));
            g_exitCode = EXITCODE_CLI_ERROR;
            return false;
         }
      }
      else {
         filePath = str(filePath, OS_DOT_UROEXT);
      }

      if (!os_exists(filePath)) {
         print(str(L"Command-line error: input file '", os_fullname(filePath), L"' does not exist."));
         g_exitCode = EXITCODE_CLI_ERROR;
         return false;
      }

      _boo result = true;
      code = os_readFile(filePath, result);

      if (!result) {
         print(L"Command-line error: input file cannot be opened.");
         g_exitCode = EXITCODE_CLI_ERROR;
         return false;
      }

      const _str basisLocation = here ? cdLocation : os_parent(filePath);

      if (d_has) {
         if (os_isAbsolute(d_value)) {
            location = d_value;
         }
         else {
            location = str(basisLocation, OS_SEPARATOR_STRING, d_value);
         }
      }
      else {
         location = basisLocation;
      }
   }

   return true;
}
