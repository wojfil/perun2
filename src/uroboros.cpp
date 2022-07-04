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
#include "uroboros.h"
#include "exception.h"
#include "command/com-parse.h"
#include "tokens.h"
#include "util.h"
#include "brackets.h"
#include "lexer.h"
#include "os.h"
#include "hash.h"
#include "print.h"
#include "datatype/math.h"
#include "var/var-runtime.h"


Uroboros::Uroboros(const Arguments& args) : arguments(args), hashes(Hashes()), vars(Variables(this)),
   vc(VariablesContext(&this->hashes)), math(Math()), flags(args.getFlags()), running(true),
   break_(false), continue_(false), exitCode(EXITCODE_OK), code(args.getCode()), sideProcess(SideProcess()),
   terminator(Terminator(this)), keywordsData(KeywordsData()) { };


void Uroboros::run()
{
   Command* commands;

   // parse code into commands
   try {
      const std::vector<Token> vec = tokenize(this->code, this);
      Tokens tks(&vec);
      checkBrackets(tks);
      commands = parseCommands(tks, this);
   }
   catch (SyntaxException ex) {
      rawPrint(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return;
   }
   catch (...) {
      SyntaxException ex2(L"wrong syntax. No command can be formed of this code", 1);
      rawPrint(ex2.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return;
   }

   this->math.init();

   // run commands
   try {
      commands->run();
   }
   catch (UroRuntimeException re) {
      rawPrint(re.getMessage());
      delete commands;
      this->exitCode = EXITCODE_RUNTIME_ERROR;
      return;
   }

   delete commands;
}
