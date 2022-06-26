#include "cmd.h"
#include "os.h"
#include "print.h"
#include "uroboros.h"


void cmd_version()
{
   print(L"Uroboros " VERSION_STR);
}

void cmd_docs()
{
   os_showWebsite(L"https://uroboros-lang.org/docs");
}

void cmd_website()
{
   os_showWebsite(L"https://uroboros-lang.org");
}

void cmd_help()
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
