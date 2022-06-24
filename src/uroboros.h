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

const int EXITCODE_OK =             0;
const int EXITCODE_RUNTIME_ERROR =  1;
const int EXITCODE_SYNTAX_ERROR =   2;
const int EXITCODE_CLI_ERROR =      3;

const _uint32 FLAG_NOOMIT =    0b001;
const _uint32 FLAG_SILENT =    0b010;
const _uint32 FLAG_GUI =       0b100;

extern _boo g_running; // application exits this way
extern _boo g_process; // another process has been created and we are waiting for its exit
extern _boo g_break;
extern _boo g_continue;
extern int g_exitCode;
extern _uint32 g_flags;

extern PROCESS_INFORMATION g_processInfo;


void run(const _str& location, const _str& code, const _uint32& flags, const _list& args);


#endif /* UROBOROS_H */
