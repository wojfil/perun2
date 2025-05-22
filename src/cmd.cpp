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

#include "cmd.h"
#include "os/os.h"
#include "perun2.h"
#include "logger.h"
#include "metadata.h"


namespace perun2::cmd
{

void version()
{
   Logger logger;
   logger.print(str(metadata::NAME, L" version ", metadata::VERSION));
}

void docs()
{
   os_showWebsite(metadata::WEBSITE_DOCS);
}

void website()
{
   os_showWebsite(metadata::WEBSITE_FRONT);
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
   logger.print(str(L"  --website    Enter the official ", metadata::NAME, L" website."));
   logger.print(str(L"  --docs       Enter the official ", metadata::NAME, L" documentation."));
   logger.print(str(L"  -c <value>   Pass ", metadata::NAME, L" code to run."));
   logger.print(L"  -d <value>   Set working location to certain value.");
   logger.print(L"  -h           Set working location to the place where this command was called from.");
   logger.print(L"  -n           Run in noomit mode (iterate all filesystem elements with no exceptions).");
   logger.print(L"  -s           Run in silent mode (no command log messages).");
   logger.print(L"  -m           Static analysis. Check code correctness without running it. Print \"good\" if no error detected.");
}

namespace error
{
   void argumentsNotAccessed()
   {
      Logger logger;
      logger.print(str(L"Command-line error: arguments could not be accessed."));
   }

   void noArguments()
   {
      Logger logger;
      logger.print(str(L"Command-line error: missing arguments. Run \"", 
         metadata::EXECUTABLE_NAME, L" --help\" for command-line tips."));
   }

   void unknownOption(const p_str& option)
   {
      Logger logger;
      logger.print(str(L"Command-line error: unknown option \"", option, L"\"."));
   }

   void noDestination()
   {
      Logger logger;
      logger.print(L"Command-line error: a destination directory has not been defined.");
   }

   void noCode()
   {
      Logger logger;
      logger.print(L"Command-line error: an argument with source code is missing.");
   }

   void noMainArgument()
   {
      Logger logger;
      logger.print(L"Command-line error: the main argument is missing.");
   }

   void noInput()
   {
      Logger logger;
      logger.print(L"Command-line error: no input file.");
   }

   void fileNotFound(const p_str& fileName)
   {
      Logger logger;
      logger.print(str(L"Command-line error: an input file \"", fileName, L"\" does not exist."));
   }

   void wrongFileExtension()
   {
      Logger logger;
      logger.print(str(L"Command-line error: wrong input file extension. Only \"", metadata::EXTENSION, L"\" is allowed."));
   }

   void fileReadFailure(const p_str& fileName)
   {
      Logger logger;
      logger.print(str(L"Command-line error: the input file \"", fileName, L"\" could not be read."));
   }
}

}
