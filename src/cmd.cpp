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

#include "cmd.h"
#include "os.h"
#include "uroboros.h"
#include "logger.h"


namespace uro::cmd
{

void version()
{
   Logger logger;
   logger.print(str(L"Uroboros2 version ", VERSION_STR));
}

void docs()
{
   os_showWebsite(L"https://uroboros2.org/docs");
}

void website()
{
   os_showWebsite(L"https://uroboros2.org");
}

void help()
{
   Logger logger;
   logger.emptyLine();
   logger.print(L"In order to run a script, pass a file name or its path as an argument. Extension is not mandatory.");
   logger.print(L"By default, working location is the directory where the script is located.");
   logger.emptyLine();
   logger.print(L"Options:");
   logger.print(L"  --help       Display this information again.");
   logger.print(L"  --version    Display interpreter version information.");
   logger.print(L"  --website    Enter the official Uroboros2 website.");
   logger.print(L"  --docs       Enter the official Uroboros2 documentation.");
   logger.print(L"  -c           Pass Uroboros2 code as an argument instead of file name.");
   logger.print(L"  -d <name>    Set working location to certain value.");
   logger.print(L"  -h           Set working location to the place where this command was called from.");
   logger.print(L"  -n           Run in noomit mode (iterate over all file system elements with no exceptions).");
   logger.print(L"  -s           Run in silent mode (no command log messages).");
}

namespace error
{
   void noArguments()
   {
      Logger logger;
      logger.print(L"Command-line error: missing arguments. Run 'uro --help' for command-line tips.");
   }

   void unknownOption(const _str& option)
   {
      Logger logger;
      logger.print(str(L"Command-line error: unknown option '", option, L"'."));
   }

   void noDestination()
   {
      Logger logger;
      logger.print(L"Command-line error: destination directory has not been defined.");
   }

   void noMainArgument()
   {
      Logger logger;
      logger.print(L"Command-line error: main argument is missing.");
   }

   void noInput()
   {
      Logger logger;
      logger.print(L"Command-line error: no input file.");
   }

   void fileNotFound(const _str& fileName)
   {
      Logger logger;
      logger.print(str(L"Command-line error: input file '", fileName, L"' does not exist."));
   }

   void wrongFileExtension(const _str& expected)
   {
      Logger logger;
      logger.print(str(L"Command-line error: wrong input file extension. Only '", expected, L"' is allowed."));
   }

   void fileReadFailure(const _str& fileName)
   {
      Logger logger;
      logger.print(str(L"Command-line error: input file '", fileName, L"' could not be read."));
   }
}

}
