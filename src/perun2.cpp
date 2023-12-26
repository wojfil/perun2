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
#include "os.h"
#include "logger.h"
#include "datatype/math.h"


namespace perun2
{

p_perun2::p_perun2(const Arguments& args) : arguments(args), contexts(*this),
   flags(args.getFlags()), logger(*this), cache(*this)
{
   p_perun2::init();
   Terminator::addPtr(this);
};

p_perun2::~p_perun2() noexcept
{
   Terminator::removePtr(this);
}

p_bool p_perun2::run()
{
   if (! arguments.areGood()) {
      return false;
   }

   return this->preParse() 
       && this->parse() 
       && this->postParse()
       && this->runCommands();
};

void p_perun2::terminate()
{
   this->state = State::s_Exit;
   if (this->sideProcess.running) {
      this->sideProcess.running = false;
      os_terminate(this->sideProcess);
   }
}

p_bool p_perun2::isRunning() const
{
   return this->state == State::s_Running;
};

p_bool p_perun2::isNotRunning() const
{
   return this->state != State::s_Running;
};

p_bool p_perun2::preParse()
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


p_bool p_perun2::parse()
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

p_bool p_perun2::postParse()
{
   this->conditionContext.deleteClosedUnits();
   this->math.init();

   // this is a potential direction of optimizations
   // next iteration of syntax analysis after successful parsing of commands

   return true;
};

p_bool p_perun2::runCommands()
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

p_bool p_perun2::initialized = false;

void p_perun2::init()
{
   if (!initialized) {
      initialized = true;
      os_init();
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

int Perun2::getExitCode() const
{
   return this->process.exitCode;
}

}
