#include "cmd.h"
#include "os.h"
#include "print.h"
#include "uroboros.h"


namespace uro
{

void cmd_version()
{
   rawPrint(L"Uroboros " VERSION_STR);
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
   rawPrint(L"");
   rawPrint(L"In order to run a script, pass a file name or its path as an argument. Extension is not mandatory.");
   rawPrint(L"By default, working location is the directory where the script is located.");
   rawPrint(L"");
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
