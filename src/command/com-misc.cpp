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

#include "com-misc.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include "../os.h"
#include "../print.h"
#include "../uroboros.h"
#include "com-core.h"
#include <sstream>


void C_PrintSingle::run()
{
   print(value->getValue());
}

void C_PrintList::run()
{
   const _list list = value->getValue();
   const _size length = list.size();
   for (_size i = 0; i < length; i++) {
      print(list[i]);
   }
}

void C_PrintDefinition::run()
{
   while (value->hasNext()) {
      if (!g_running) {
         value->reset();
         break;
      }
      print(value->getValue());
   }
}

void C_PrintThis_Str::run()
{
   print(g_this_s.value);
}

void C_PrintThis_Num::run()
{
   print(g_this_n.value.toString());
}

void C_PrintThis_Tim::run()
{
   print(g_this_t.value.toString());
}

void C_SleepPeriod::run()
{
   os_sleepForMs(1000LL * value->getValue().toSeconds());
}

void C_SleepMs::run()
{
   os_sleepForMs(value->getValue().toInt());
}

void C_Break::run()
{
   g_running = false;
   g_break = true;
}

void C_Continue::run()
{
   g_running = false;
   g_continue = true;
}

void C_Exit::run()
{
   g_running = false;
}

void C_Error::run()
{
   g_running = false;
   g_exitCode = EXITCODE_RUNTIME_ERROR;
}

void C_ErrorWithExitCode::run()
{
   g_running = false;
   const _int code = (_int)(exitCode->getValue().toInt());
   g_exitCode = code == EXITCODE_OK ? EXITCODE_RUNTIME_ERROR : code;
}

void C_Run::run()
{
   _str command = value->getValue();
   os_rawTrim(command);

   if (command.empty()) {
      commandLog(L"Failed to run an empty command");
      g_success.value = false;
      return;
   }

   const _boo s = os_run(command);
   g_success.value = s;

   if (s) {
      commandLog(L"Run '", command, L"'");
   }
   else {
      commandLog(L"Failed to run '", command, L"'");
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}

void C_RunWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!g_exists.value || base.empty()) {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with '", base, L"'");
      g_success.value = false;
      return;
   }

   const _str com = str(base, L" ", os_quoteEmbraced(g_trimmed));
   const _boo s = os_run(com);
   g_success.value = s;

   if (s) {
      commandLog(L"Run ", getCCName(g_trimmed), L" with '", base, L"'");
   }
   else {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with '", base, L"'");
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}

void C_RunWithWithString::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!g_exists.value || base.empty()) {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with '", base, L"'");
      g_success.value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(base, L" ", os_quoteEmbraced(g_trimmed), L" ", arg);

   const _boo s = os_run(com);
   g_success.value = s;

   if (s) {
      commandLog(L"Run ", getCCName(g_trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }
   else {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}

void C_RunWithWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!g_exists.value || base.empty()) {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with '", base, L"'");
      g_success.value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(base, L" ", os_quoteEmbraced(g_trimmed));
      const _boo s = os_run(com);
      g_success.value = s;

      if (s) {
         commandLog(L"Run ", getCCName(g_trimmed), L" with '", base, L"'");
      }
      else {
         commandLog(L"Failed to run ", getCCName(g_trimmed), L" with '", base, L"'");
      }
   }
   else {
      std::wstringstream comStream;
      std::wstringstream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(g_trimmed), L" with '", base, L"' with '", first, L"'");
      comStream << str(base, L" ", os_quoteEmbraced(g_trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _boo s = os_run(com);
      g_success.value = s;

      if (s) {
         commandLog(L"Run ", logStream.str());
      }
      else {
         commandLog(L"Failed to run ", logStream.str());
      }
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}

void C_RunWithUroboros::run()
{
   if (!g_exists.value) {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with Uroboros");
      g_success.value = false;
      return;
   }

   const _str com = str(g_urocom, os_quoteEmbraced(g_trimmed));
   const _boo s = os_run(com);
   g_success.value = s;

   if (s) {
      commandLog(L"Run ", getCCName(g_trimmed), L" with Uroboros");
   }
   else {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with Uroboros");
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}

void C_RunWithUroborosWithString::run()
{
   if (!g_exists.value) {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with Uroboros");
      g_success.value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(g_urocom, os_quoteEmbraced(g_trimmed), L" ", arg);

   const _boo s = os_run(com);
   g_success.value = s;

   if (s) {
      commandLog(L"Run ", getCCName(g_trimmed), L" with Uroboros with '", rawArg, L"'");
   }
   else {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with Uroboros with '", rawArg, L"'");
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}

void C_RunWithUroborosWith::run()
{
   if (!g_exists.value) {
      commandLog(L"Failed to run ", getCCName(g_trimmed), L" with Uroboros");
      g_success.value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(g_urocom, os_quoteEmbraced(g_trimmed));
      const _boo s = os_run(com);
      g_success.value = s;

      if (s) {
         commandLog(L"Run ", getCCName(g_trimmed), L" with Uroboros");
      }
      else {
         commandLog(L"Failed to run ", getCCName(g_trimmed), L" with Uroboros");
      }
   }
   else {
      std::wstringstream comStream;
      std::wstringstream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(g_trimmed), L" with Uroboros with '", first, L"'");
      comStream << str(g_urocom, os_quoteEmbraced(g_trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _boo s = os_run(com);
      g_success.value = s;

      if (s) {
         commandLog(L"Run ", logStream.str());
      }
      else {
         commandLog(L"Failed to run ", logStream.str());
      }
   }

   if (hasAttribute) {
      os_loadAttributes(*attribute);
   }
}


void C_Process::run()
{
   _str process = value->getValue();
   os_rawTrim(process);

   if (process.empty()) {
      commandLog(L"Failed to start an empty process");
      g_success.value = false;
      return;
   }

   const _boo s = os_process(process);
   g_success.value = s;

   if (s) {
      commandLog(L"Start process '", process, L"'");
   }
   else {
      commandLog(L"Failed to start process '", process, L"'");
   }
}
