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

}
