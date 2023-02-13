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

#ifndef UROBOROS_H
#define UROBOROS_H

#include "var/var-runtime.h"
#include "var/var-context.h"
#include "arguments.h"
#include "hash.h"
#include "datatype/math.h"
#include "terminator.h"
#include "keyword.h"
#include "side-process.h"
#include "command/com.h"
#include "command/com-parse-unit.h"
#include "context.h"


namespace uro
{

struct Contextes;

static const _str VERSION_STR = L"2.0.0";

inline constexpr _exitint EXITCODE_OK =             0;
inline constexpr _exitint EXITCODE_RUNTIME_ERROR =  1;
inline constexpr _exitint EXITCODE_SYNTAX_ERROR =   2;
inline constexpr _exitint EXITCODE_CLI_ERROR =      3;


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


struct _uro
{
public:
   _uro() = delete;
   _uro(const Arguments& args);
   _bool run();

   const Arguments& arguments;
   Hashes hashes;
   Math math;
   Contextes contextes;
   Terminator terminator;
   const KeywordsData keywordsData;
   SideProcess sideProcess;
   const _uint32 flags;
   comm::ConditionContext conditionContext;
   State state = State::s_Running;
   _exitint exitCode = EXITCODE_OK;

private:
   ParseState parseState = ParseState::ps_NotParsed;
   _comptr commands;
   std::vector<Token> tokens;

   _bool preParse();
   _bool parse();
   _bool postParse();
   _bool runCommands();
};


// this is the facade for Uroboros2
// create an object once
// and run() it multiple times
// the structure is cached, so the syntax analysis is performed only once
struct Uroboros2
{
public:
   Uroboros2(const _int& argc, _char* const argv[]);
   Uroboros2(const _str& location, const _str& code);
   Uroboros2(const _str& location, const _str& code, const _uint32& flags);

   Uroboros2() = delete;
   Uroboros2(Uroboros2 const&) = delete;
   Uroboros2& operator= (Uroboros2 const&) = delete;
   
   _bool run();
   _exitint getExitCode() const;

private:
   Arguments arguments;
   _uro process;
};


}

#endif /* UROBOROS_H */
