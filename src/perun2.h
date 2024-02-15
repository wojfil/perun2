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
#include "datatype/generator/gen-os.h"
#include "context/ctx-main.h"
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


// this struct is used only internally within the namespace
// for external facade of the entire language, use struct 'Perun2' from below
struct Perun2Process
{
public:
   Perun2Process() = delete;
   Perun2Process(const Arguments& args);
   ~Perun2Process() noexcept;
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

// count how many Perun2 processes are there globally
   static p_int globalCount;
   static void init();
   static void deinit();
};


// external facade for Perun2
// create an object once
// and run() it multiple times
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
   Perun2Process process;
};


}

#endif /* PERUN2_H */
