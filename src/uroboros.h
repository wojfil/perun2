/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
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


namespace uro
{

static const _str VERSION_STR = L"1.0.6";

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

struct Uroboros
{
public:
   Uroboros(const Arguments& args);
   _bool run();

   const Arguments& arguments;
   Hashes hashes;
   Math math;
   vars::Variables vars;
   vars::VariablesContext vc;
   Terminator terminator;
   const KeywordsData keywordsData;
   SideProcess sideProcess;
   const _uint32 flags;
   comm::ConditionContext conditionContext;
   State state = State::s_Running;
   _exitint exitCode = EXITCODE_OK;

private:
   _comptr commands;
   std::vector<Token> tokens;

   _bool uro_tokenize();
   _bool uro_parse();
   _bool uro_postParse();
   _bool uro_runCommands();

};

}

#endif /* UROBOROS_H */
