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

#ifndef PERUN2_H
#define PERUN2_H

#include "console.h"
#include "arguments.h"
#include "hash.h"
#include "datatype/math.h"
#include "terminator.h"
#include "keyword.h"
#include "side-process.h"
#include "command/com.h"
#include "command/com-parse-unit.h"
#include "context.h"
#include "logger.h"
#include "cache.h"


namespace perun2
{

struct Contexts;

_constexpr _exitint EXITCODE_OK =             0;
_constexpr _exitint EXITCODE_RUNTIME_ERROR =  1;
_constexpr _exitint EXITCODE_SYNTAX_ERROR =   2;
_constexpr _exitint EXITCODE_CLI_ERROR =      3;


enum State
{
   s_Running = 0,
   s_Break,
   s_Continue,
   s_Exit
};


enum ParseState
{
   ps_NotParsed,
   ps_ParsingSuccess,
   ps_ParsingFailure
};


// this is an instance of Perun2
// used only internally within the namespace
// for external facade, use struct 'Perun2' instead
struct _p2
{
public:
   _p2() = delete;
   _p2(const Arguments& args);
   ~_p2() noexcept;
   _bool run();
   void terminate();

   const Arguments& arguments;
   Hashes hashes;
   Math math;
   Contexts contexts;
   const KeywordsData keywordsData;
   SideProcess sideProcess;
   const _flags flags;
   comm::ConditionContext conditionContext;
   State state = State::s_Running;
   _exitint exitCode = EXITCODE_OK;
   ParseState parseState = ParseState::ps_NotParsed;
   Logger logger;
   Cache cache;

   _bool isRunning() const;
   _bool isNotRunning() const;

private:
   _comptr commands;
   std::vector<Token> tokens;

   _bool preParse();
   _bool parse();
   _bool postParse();
   _bool runCommands();

// this initialization is global and performed only once for all struct instances
   static _bool initialized;
   static void init();
};


// facade for Perun2
// create an object once
// and run() it multiple times
// the structure is cached, so the syntax analysis is performed only once at creation
struct Perun2
{
public:
   Perun2(const _int argc, _char* const argv[]);
   Perun2(const _str& location, const _str& code);
   Perun2(const _str& location, const _str& code, const _flags flags);

   Perun2() = delete;
   Perun2(Perun2 const&) = delete;
   Perun2& operator= (Perun2 const&) = delete;

   _bool run();
   _exitint getExitCode() const;

private:
   Arguments arguments;
   _p2 process;
};


}

#endif /* PERUN2_H */
