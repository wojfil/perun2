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
#include "unicode/convert.h"
#include "cmd.h"


static int mainRun(const perun2::p_list& args)
{
   perun2::Perun2 instance(args);

   if (instance.hasArgFlag(perun2::FLAG_STATIC_ANALYSIS)) {
      instance.staticallyAnalyze();
   }
   else {
      instance.run();
   }

   return instance.getExitCode();
}


#if defined(_WIN32)

   int main(void)
   {
      int argc;
      LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

      if (argv == NULL) {
         perun2::cmd::error::argumentsNotAccessed();
         return perun2::EXITCODE_CLI_ERROR;
      }

      perun2::p_list args;
      args.reserve(argc);

      for (int i = 0; i < argc; ++i) {
         std::wstring arg = argv[i];
         args.emplace_back(perun2::utf16_to_utf32(arg));
      }

      LocalFree(argv);
      return mainRun(args);
   }

#elif defined(__APPLE__) || defined(__linux__)

   int main(int argc, char* argv[])
   {
      perun2::p_list args;
      args.reserve(argc);

      for (int i = 0; i < argc; ++i) {
         std::string arg = argv[i];
         args.emplace_back(perun2::utf8_to_utf32(arg));
      }

      return mainRun(args);
   }

#else
   #error "Unsupported platform"
#endif
