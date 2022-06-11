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

#include "print.h"
#include "var/var-runtime.h"
#include <random>
#include <Windows.h>

#define EXITCODE_OK             0
#define EXITCODE_RUNTIME_ERROR  1
#define EXITCODE_SYNTAX_ERROR   2
#define EXITCODE_CLI_ERROR      3

extern _boo g_running;
extern _boo g_process;
extern _boo g_break;
extern _boo g_continue;
extern int g_exitCode;
extern _boo g_noomit;
extern _boo g_silent;
extern _boo g_guiMode;
extern std::mt19937 g_generator;
extern std::uniform_real_distribution<> g_ddist;
extern PROCESS_INFORMATION g_processInfo;


void run(const _str& location, const _str& code, const _boo& noomit,
   const _boo& silent, const _boo& guimode, const _list& args);

void runCode(const _str& code);

void init(const _str& location, const _list& args);
void finish();

#endif /* UROBOROS_H */

