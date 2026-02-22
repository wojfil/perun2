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

#pragma once

#include "console.hpp"
#include "arguments.hpp"
#include "datatype/math.hpp"
#include "terminator.hpp"
#include "keyword.hpp"
#include "side-process.hpp"
#include "command/com.hpp"
#include "python3/python3-processes.hpp"
#include "command/com-parse-unit.hpp"
#include "datatype/generator/gen-os.hpp"
#include "context/ctx-main.hpp"
#include "logger.hpp"
#include "post-parse-data.hpp"


namespace perun2
{

// Warning:
// Perun2 modifies the global settings of the program it runs in.
// These changes include, among others, character encoding, output console behavior, system locale.
// It is recommended to compile Perun2 as a separate program and run it via the CLI.

struct Contexts;

p_constexpr int EXITCODE_OK =             0;
p_constexpr int EXITCODE_RUNTIME_ERROR =  1;
p_constexpr int EXITCODE_SYNTAX_ERROR =   2;
p_constexpr int EXITCODE_CLI_ERROR =      3;
p_constexpr int EXITCODE_NO_LOCATION =    4;


enum State
{
   s_Running = 0,
   s_Break,
   s_Continue,
   s_Exit
};

enum ExecutionType
{
   et_None = 0,
   et_Run,
   et_StaticAnalysis
};


// this struct is used only internally within the namespace
// for the external facade of the entire language, use the struct 'Perun2' from below
struct Perun2Process
{
public:
   Perun2Process() = delete;
   Perun2Process(const Arguments& args);
   ~Perun2Process() noexcept;

   // perform all parsing and then run all parsed commands if parsing succeeded
   p_bool run();
   
   // perform all parsing, but do not run any command
   p_bool staticallyAnalyze();

   // stop running process
   void terminate();

   p_bool isRunning() const;
   p_bool isNotRunning() const;
   ExecutionType getExecutionType() const;

   const Arguments& arguments;
   ConsoleSettings consoleSettings;
   Math math;
   Contexts contexts;
   const KeywordsData keywordsData;
   SideProcess sideProcess;
   const p_flags flags;
   comm::ConditionContext conditionContext;
   State state = State::s_Running;
   int exitCode = EXITCODE_OK;
   Logger logger;
   PostParseData postParseData;
   comm::Python3Processes python3Processes;

private:
   p_bool preParse();
   p_bool parse();
   p_bool postParse();
   p_bool runCommands();

// count how many Perun2 processes are there globally
   static p_int globalCount;
   static void tryInit();
   static void tryDeinit();

   p_comptr commands;
   std::vector<Token> tokens;
   ExecutionType executionType = ExecutionType::et_None;
   Terminator terminator;
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

   // perform all parsing and then run all parsed commands if parsing succeeded
   p_bool run();

   // perform all parsing, but do not run any command
   p_bool staticallyAnalyze();

   // return exit code of last run or last static analysis
   // if we never run or analyzed anything, return EXITCODE_OK
   int getExitCode() const;

   p_bool hasArgFlag(const p_flags flag) const;

private:
   Arguments arguments;
   Perun2Process process;
};


}
