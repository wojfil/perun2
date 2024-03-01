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

#include "perun2.h"
#include "cmd.h"


int main(void)
{
   int argc;
   LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

   if (argv == NULL) {
      perun2::cmd::error::argumentsNotAccessed();
      return perun2::EXITCODE_CLI_ERROR;
   }

   // these two inits below affect globally the flow of the program
   // do not call them if Perun2 is used like an imported external library
   perun2::initConsole();
   perun2::initTerminator();

   perun2::Perun2 instance(argc, argv);

   if (instance.hasArgFlag(perun2::FLAG_STATIC_ANALYSIS)) {
      instance.staticallyAnalyze();
   }
   else {
      instance.run();
   }

   LocalFree(argv);
   return instance.getExitCode();
}
