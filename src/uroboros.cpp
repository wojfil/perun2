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

#include <vector>
#include <cstdlib>
#include "exception.h"
#include "command/com-parse.h"
#include "tokens.h"
#include "util.h"
#include "brackets.h"
#include "lexer.h"
#include "uroboros.h"
#include "os.h"
#include "hash.h"
#include "datatype/math.h"


_boo g_running;
_boo g_process;
_boo g_break;
_boo g_continue;
int g_exitCode;
_uint32 g_flags;
/*
std::mt19937 g_generator;
std::uniform_real_distribution<> g_ddist;*/
PROCESS_INFORMATION g_processInfo;



void run(const _str& location, const _str& code, const _uint32& flags,  const _list& args)
{
   g_flags = flags;

   initHashes();
   initVars(args);
   g_math = new Math();

   g_running = true;
   g_exitCode = EXITCODE_OK;
   g_location.value = os_trim(location);

   Command* commands;

   // parse code into commands
   try {
      const std::vector<Token> vec = tokenize(code);
      Tokens tks(&vec);
      checkBrackets(tks);
      commands = parseCommands(tks);
   }
   catch (SyntaxException ex) {
      print(ex.getMessage());
      g_exitCode = EXITCODE_SYNTAX_ERROR;
      return;
   }
   catch (...) {
      SyntaxException ex2(L"wrong syntax. No command can be formed of this code", 1);
      print(ex2.getMessage());
      g_exitCode = EXITCODE_SYNTAX_ERROR;
      return;
   }

   g_math->init();

   // run commands
   try {
      commands->run();
   }
   catch (UroRuntimeException re) {
      print(re.getMessage());
      delete commands;
      g_exitCode = EXITCODE_RUNTIME_ERROR;
      return;
   }

   delete commands;
}
