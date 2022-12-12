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

#include "cmd.h"
#include "os.h"
#include "print.h"
#include "uroboros.h"


namespace uro::cmd
{

void version()
{
   rawPrint(str(L"Uroboros ", VERSION_STR));
}

void docs()
{
   os_showWebsite(L"https://uroboros-lang.org/docs");
}

void website()
{
   os_showWebsite(L"https://uroboros-lang.org");
}

void help()
{
   rawPrint(EMPTY_STRING);
   rawPrint(L"In order to run a script, pass a file name or its path as an argument. Extension is not mandatory.");
   rawPrint(L"By default, working location is the directory where the script is located.");
   rawPrint(EMPTY_STRING);
   rawPrint(L"Options:");
   rawPrint(L"  --help       Display this information again.");
   rawPrint(L"  --version    Display interpreter version information.");
   rawPrint(L"  --website    Enter the official Uroboros website.");
   rawPrint(L"  --docs       Enter the official Uroboros documentation.");
   rawPrint(L"  -c           Pass Uroboros code as an argument instead of file name.");
   rawPrint(L"  -d <name>    Set working location to certain value.");
   rawPrint(L"  -h           Set working location to the place where this command was called from.");
   rawPrint(L"  -n           Run in noomit mode (iterate over all file system elements with no exceptions).");
   rawPrint(L"  -s           Run in silent mode (no command log messages).");
   rawPrint(L"  -g           For internal use only. Set up log message communication with GUI.");
}

namespace error
{
   void noArguments()
   {
      rawPrint(L"Command-line error: missing arguments. Run 'uro --help' for command-line tips.");
   }

   void unknownOption(const _str& option)
   {
      rawPrint(str(L"Command-line error: unknown option '", option, L"'."));
   }

   void noDestination()
   {
      rawPrint(L"Command-line error: destination directory has not been defined.");
   }

   void noMainArgument()
   {
      rawPrint(L"Command-line error: main argument is missing.");
   }

   void noInput()
   {
      rawPrint(L"Command-line error: no input file.");
   }

   void fileNotFound(const _str& fileName)
   {
      rawPrint(str(L"Command-line error: input file '", fileName, L"' does not exist."));
   }

   void wrongFileExtension(const _str& expected)
   {
      rawPrint(str(L"Command-line error: wrong input file extension. Only '", expected, L"' is allowed."));
   }

   void fileReadFailure(const _str& fileName)
   {
      rawPrint(str(L"Command-line error: input file '", fileName, L"' could not be read."));
   }
}

}
