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
   logger.print(str(metadata::NAME, U" version ", metadata::VERSION));
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
   logger.print(U"In order to run a script, pass a file name or its path as an argument. Extension is not mandatory.");
   logger.print(U"By default, working location is the directory where the script is located.");
   logger.emptyLine();
   logger.print(U"Options:");
   logger.print(U"  --help       Display this information again.");
   logger.print(U"  --version    Display interpreter version information.");
   logger.print(str(U"  --website    Enter the official ", metadata::NAME, U" website."));
   logger.print(str(U"  --docs       Enter the official ", metadata::NAME, U" documentation."));
   logger.print(str(U"  -c <value>   Pass ", metadata::NAME, U" code to run."));
   logger.print(U"  -d <value>   Set working location to certain value.");
   logger.print(U"  -h           Set working location to the place where this command was called from.");
   logger.print(U"  -n           Run in noomit mode (iterate all filesystem elements with no exceptions).");
   logger.print(U"  -s           Run in silent mode (no command log messages).");
   logger.print(U"  -o           Maximum performance mode. The terminal is completely disabled.");
   logger.print(U"  -m           Static analysis. Check code correctness without running it. Prints \"good\" if no error detected.");
}

namespace error
{
   void argumentsNotAccessed()
   {
      Logger logger;
      logger.print(str(U"Command-line error: the arguments could not be accessed."));
   }

   void noArguments()
   {
      Logger logger;
      logger.print(str(U"Command-line error: the arguments are missing. Run \"", 
         metadata::EXECUTABLE_NAME, U" --help\" for command-line tips."));
   }

   void unknownOption(const p_str& option)
   {
      Logger logger;
      logger.print(str(U"Command-line error: unknown option \"", option, U"\"."));
   }

   void noDestination()
   {
      Logger logger;
      logger.print(U"Command-line error: the destination directory has not been defined.");
   }

   void noCode()
   {
      Logger logger;
      logger.print(U"Command-line error: the argument with source code is missing.");
   }

   void noMainArgument()
   {
      Logger logger;
      logger.print(U"Command-line error: the main argument is missing.");
   }

   void noInput()
   {
      Logger logger;
      logger.print(U"Command-line error: the input file is missing.");
   }

   void noLocation()
   {
      Logger logger;
      logger.print(U"Command-line error: current working location could not be read.");
   }

   void fileNotFound(const p_str& fileName)
   {
      Logger logger;
      logger.print(str(U"Command-line error: the input file \"", fileName, U"\" does not exist."));
   }

   void wrongFileExtension()
   {
      Logger logger;
      logger.print(str(U"Command-line error: wrong input file extension. Only \"", metadata::EXTENSION, U"\" is allowed."));
   }

   void fileReadFailure(const p_str& fileName)
   {
      Logger logger;
      logger.print(str(U"Command-line error: the input file \"", fileName, U"\" could not be read."));
   }
}

}
