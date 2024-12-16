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

#include <vector>
#include <cstdlib>
#include "perun2.h"
#include "exception.h"
#include "command/com-parse.h"
#include "tokens.h"
#include "util.h"
#include "brackets.h"
#include "lexer.h"
#include "os/os.h"
#include "logger.h"
#include "datatype/math.h"


namespace perun2
{

Perun2Process::Perun2Process(const Arguments& args) : arguments(args), contexts(*this),
   flags(args.getFlags()), logger(*this), constCache(*this)
{
   Perun2Process::tryInit();
   Terminator::addPtr(this);
};

Perun2Process::~Perun2Process() noexcept
{
   Perun2Process::tryDeinit();
   Terminator::removePtr(this);
}

p_bool Perun2Process::run()
{
   if (! this->arguments.areGood()) {
      this->exitCode = EXITCODE_CLI_ERROR;
      return false;
   }

   if (! os_directoryExists(this->arguments.getLocation())) {
      this->exitCode = EXITCODE_NO_LOCATION;
      return false;
   }

   this->exitCode = EXITCODE_OK;

   return this->preParse() 
       && this->parse() 
       && this->postParse()
       && this->runCommands();
};

p_bool Perun2Process::staticallyAnalyze()
{
   if (! this->arguments.areGood()) {
      this->exitCode = EXITCODE_CLI_ERROR;
      return false;
   }

   this->exitCode = EXITCODE_OK;

   if (this->preParse() 
       && this->parse() 
       && this->postParse())
   {
      this->logger.log(STRING_GOOD);
      return true;
   }

   return false;
}

void Perun2Process::terminate()
{
   this->state = State::s_Exit;
   if (this->sideProcess.running) {
      this->sideProcess.running = false;
      os_terminate(this->sideProcess);
   }
}

p_bool Perun2Process::isRunning() const
{
   return this->state == State::s_Running;
};

p_bool Perun2Process::isNotRunning() const
{
   return this->state != State::s_Running;
};

p_bool Perun2Process::preParse()
{
   try {
      this->tokens = tokenize(this->arguments.getCodeRef(), *this);
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


p_bool Perun2Process::parse()
{
   try {
      const Tokens tks(this->tokens);
      if (!comm::parseCommands(this->commands, tks, *this)) {
         return false;
      }
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

p_bool Perun2Process::postParse()
{
   this->conditionContext.deleteClosedUnits();
   this->math.init();

   // this is a potential direction of optimizations
   // next iteration of syntax analysis after successful parsing of commands

   return true;
};

p_bool Perun2Process::runCommands()
{
   try {
      this->commands->run();
   }
   catch (...) {
      this->exitCode = EXITCODE_RUNTIME_ERROR;
      return false;
   }

   return true;
};

p_int Perun2Process::globalCount = 0;

void Perun2Process::tryInit()
{
   if (globalCount == 0) {
      os_init();
   }

   globalCount++;
}

void Perun2Process::tryDeinit()
{
   globalCount--;
   if (globalCount == 0) {
      os_deinit();
   }
}


Perun2::Perun2(const p_int argc, p_char* const argv[])
   : arguments(argc, argv), process(this->arguments) { };

Perun2::Perun2(const p_str& location, const p_str& code)
   : arguments(location, code), process(this->arguments) { };

Perun2::Perun2(const p_str& location, const p_str& code, const p_flags flags)
   : arguments(location, code, flags), process(this->arguments) { };

p_bool Perun2::run()
{
   return this->process.run();
}

p_bool Perun2::staticallyAnalyze()
{
   return this->process.staticallyAnalyze();
}

int Perun2::getExitCode() const
{
   return this->process.exitCode;
}

p_bool Perun2::hasArgFlag(const p_flags flag) const
{
   return this->arguments.hasFlag(flag);
}

}
