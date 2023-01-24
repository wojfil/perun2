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


namespace uro
{

_uro::_uro(const Arguments& args) : arguments(args), vars(vars::Variables(*this)),
   vc(vars::VariablesContext(this->hashes, this->vars)), flags(args.getFlags()),
   terminator(Terminator(this)) { };


_bool _uro::run()
{
   return this->preParse()
       && this->parse()
       && this->postParse()
       && this->runCommands();
};

_bool _uro::preParse()
{
   this->exitCode = EXITCODE_OK;

   try {
      this->tokens = tokenize(this->arguments.getCode(), *this);
   }
   catch (const SyntaxError& ex) {
      rawPrint(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }
   catch (...) {
      SyntaxError ex2(L"wrong syntax. No command can be formed of this code", 1);
      rawPrint(ex2.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }

   return true;
};


_bool _uro::parse()
{
   try {
      const Tokens tks(this->tokens);
      if (!comm::parseCommands(this->commands, tks, *this)) {
         return false;
      }
      this->conditionContext.deleteClosedUnits();
   }
   catch (const SyntaxError& ex) {
      rawPrint(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }
   catch (...) {
      SyntaxError ex2(L"wrong syntax. No command can be formed of this code", 1);
      rawPrint(ex2.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }

   return true;
};

_bool _uro::postParse()
{
   this->math.init();

   // this is potential direction of optimizations
   // next iteration of syntax analysis after successful parsing of commands

   return true;
};

_bool _uro::runCommands()
{
   try {
      this->commands->run();
   }
   catch (const RuntimeError& re) {
      rawPrint(re.getMessage());
      this->exitCode = EXITCODE_RUNTIME_ERROR;
      return false;
   }

   return true;
};

}
