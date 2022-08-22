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
#include "literals.h"
#include "command/com.h"
#include "command/com-parse-unit.h"
#include "datatype/generator/gen-pattern.h"

#define VERSION_STR L"1.0.6"

inline constexpr  _int EXITCODE_OK =             0;
inline constexpr  _int EXITCODE_RUNTIME_ERROR =  1;
inline constexpr  _int EXITCODE_SYNTAX_ERROR =   2;
inline constexpr  _int EXITCODE_CLI_ERROR =      3;

inline constexpr  _uint32 FLAG_NULL =      0b000;
inline constexpr  _uint32 FLAG_NOOMIT =    0b001;
inline constexpr  _uint32 FLAG_SILENT =    0b010;
inline constexpr  _uint32 FLAG_GUI =       0b100;


struct Uroboros
{
public:
   Uroboros(const Arguments& args);
   _boo run();

   const Arguments arguments;
   Hashes hashes;
   Math math;
   Variables vars;
   VariablesContext vc;
   Terminator terminator;
   const KeywordsData keywordsData;
   SideProcess sideProcess;
   Literals literals;
   const _uint32 flags;
   IfContext ifContext;
   const PatternParser patternParser;

   _boo running = true;
   _boo break_ = false;
   _boo continue_ = false;
   _int exitCode = EXITCODE_OK;

private:
   Command* commands = nullptr;
   std::vector<Token> tokens;

   _boo uro_tokenize();
   _boo uro_parse();
   _boo uro_postParse();
   _boo uro_runCommands();

};

#endif /* UROBOROS_H */
