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


namespace uro
{

_uro::_uro(const Arguments& args) : arguments(args), contexts(*this),
   flags(args.getFlags()), terminator(Terminator(this)) { };


_bool _uro::run()
{
   switch (this->parseState) {
      case ParseState::ps_NotParsed: {
         const _bool parsed = this->preParse() && this->parse() && this->postParse();
         if (parsed) {
            this->parseState = ParseState::ps_ParsingSuccess;
            return this->runCommands();
         }
         else {
            this->parseState = ParseState::ps_ParsingFailure;
            return false;
         }
         break;
      }
      case ParseState::ps_ParsingSuccess: {
         this->exitCode = EXITCODE_OK;
         return this->runCommands();
         break;
      }
      case ParseState::ps_ParsingFailure: {
         return false;
         break;
      }
   }

   return false;
};

_bool _uro::preParse()
{
   try {
      this->tokens = tokenize(this->arguments.getCode(), *this);
   }
   catch (const SyntaxError& ex) {
      rawPrint(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }
   catch (...) {
      SyntaxError ex = SyntaxError::wrongSyntax(1);
      rawPrint(ex.getMessage());
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
      SyntaxError ex = SyntaxError::wrongSyntax(1);
      rawPrint(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }

   return true;
};

_bool _uro::postParse()
{
   this->math.init();

   // this is a potential direction of optimizations
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

Uroboros2::Uroboros2(const _int& argc, _char* const argv[])
   : arguments(argc, argv), process(this->arguments)
{
   if (this->process.exitCode == EXITCODE_CLI_ERROR) {
      this->process.parseState = ParseState::ps_ParsingFailure;
   }
};

Uroboros2::Uroboros2(const _str& location, const _str& code)
   : arguments(location, code), process(this->arguments) { };

Uroboros2::Uroboros2(const _str& location, const _str& code, const _uint32& flags)
   : arguments(location, code, flags), process(this->arguments) { };

_bool Uroboros2::run()
{
   return this->process.run();
}

_exitint Uroboros2::getExitCode() const
{
   return this->process.exitCode;
}

}
