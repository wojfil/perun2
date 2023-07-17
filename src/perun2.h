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

p_constexpr int EXITCODE_OK =             0;
p_constexpr int EXITCODE_RUNTIME_ERROR =  1;
p_constexpr int EXITCODE_SYNTAX_ERROR =   2;
p_constexpr int EXITCODE_CLI_ERROR =      3;


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
struct p_perun2
{
public:
   p_perun2() = delete;
   p_perun2(const Arguments& args);
   ~p_perun2() noexcept;
   p_bool run();
   void terminate();

   const Arguments& arguments;
   Math math;
   Contexts contexts;
   const KeywordsData keywordsData;
   SideProcess sideProcess;
   const p_flags flags;
   comm::ConditionContext conditionContext;
   State state = State::s_Running;
   int exitCode = EXITCODE_OK;
   ParseState parseState = ParseState::ps_NotParsed;
   Logger logger;
   Cache cache;

   p_bool isRunning() const;
   p_bool isNotRunning() const;

private:
   p_comptr commands;
   std::vector<Token> tokens;

   p_bool preParse();
   p_bool parse();
   p_bool postParse();
   p_bool runCommands();

// this initialization is global and performed only once for all struct instances
   static p_bool initialized;
   static void init();
};


// facade for Perun2
// create an object once
// and run() it multiple times
// the structure is cached, so the syntax analysis is performed only once at creation
struct Perun2
{
public:
   Perun2(const p_int argc, p_char* const argv[]);
   Perun2(const p_str& location, const p_str& code);
   Perun2(const p_str& location, const p_str& code, const p_flags flags);

   Perun2() = delete;
   Perun2(Perun2 const&) = delete;
   Perun2& operator= (Perun2 const&) = delete;

   p_bool run();
   int getExitCode() const;

private:
   Arguments arguments;
   p_perun2 process;
};


}

#endif /* PERUN2_H */
