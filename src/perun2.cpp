/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include <vector>
#include <cstdlib>
#include "perun2.h"
#include "exception.h"
#include "command/com-parse.h"
#include "tokens.h"
#include "util.h"
#include "brackets.h"
#include "lexer.h"
#include "os.h"
#include "hash.h"
#include "logger.h"
#include "datatype/math.h"


namespace perun2
{

_p2::_p2(const Arguments& args) : arguments(args), contexts(*this),
   flags(args.getFlags()), keywordsData(*this), logger(*this), cache(*this)
{
   _p2::init();
   Terminator::addPtr(this);
   if (arguments.getParseState() == ArgsParseState::aps_Failed) {
      this->parseState = ParseState::ps_ParsingFailure;
   }
};

_p2::~_p2() noexcept
{
   Terminator::removePtr(this);
}

_bool _p2::run()
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

void _p2::terminate()
{
   this->state = State::s_Exit;
   if (this->sideProcess.running) {
      this->sideProcess.running = false;
      TerminateProcess(this->sideProcess.info.hProcess, 0);
   }
}

_bool _p2::isRunning() const
{
   return this->state == State::s_Running;
};

_bool _p2::isNotRunning() const
{
   return this->state != State::s_Running;
};

_bool _p2::preParse()
{
   try {
      this->tokens = tokenize(this->arguments.getCode(), *this);
   }
   catch (const SyntaxError& ex) {
      this->logger.print(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }
   catch (...) {
      SyntaxError ex = SyntaxError::wrongSyntax(1);
      this->logger.print(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }

   return true;
};


_bool _p2::parse()
{
   try {
      const Tokens tks(this->tokens);
      if (!comm::parseCommands(this->commands, tks, *this)) {
         return false;
      }
      this->conditionContext.deleteClosedUnits();
   }
   catch (const SyntaxError& ex) {
      this->logger.print(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }
   catch (...) {
      SyntaxError ex = SyntaxError::wrongSyntax(1);
      this->logger.print(ex.getMessage());
      this->exitCode = EXITCODE_SYNTAX_ERROR;
      return false;
   }

   return true;
};

_bool _p2::postParse()
{
   this->math.init();

   // this is a potential direction of optimizations
   // next iteration of syntax analysis after successful parsing of commands

   return true;
};

_bool _p2::runCommands()
{
   try {
      this->commands->run();
   }
   catch (const RuntimeError& re) {
      this->logger.print(re.getMessage());
      this->exitCode = EXITCODE_RUNTIME_ERROR;
      return false;
   }

   return true;
};

_bool _p2::initialized = false;

void _p2::init()
{
   if (!initialized) {
      initialized = true;
      os_init();
   }
}

Perun2::Perun2(const _int argc, _char* const argv[])
   : arguments(argc, argv), process(this->arguments)
{
   if (this->process.exitCode == EXITCODE_CLI_ERROR) {
      this->process.parseState = ParseState::ps_ParsingFailure;
   }
};

Perun2::Perun2(const _str& location, const _str& code)
   : arguments(location, code), process(this->arguments) { };

Perun2::Perun2(const _str& location, const _str& code, const _flags flags)
   : arguments(location, code, flags), process(this->arguments) { };

_bool Perun2::run()
{
   return this->process.run();
}

_exitint Perun2::getExitCode() const
{
   return this->process.exitCode;
}

}
