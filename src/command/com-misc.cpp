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


namespace uro::comm
{

void C_PrintSingle::run()
{
   print(this->uroboros, this->value->getValue());
}

void C_PrintList::run()
{
   const _list list = this->value->getValue();
   const _size length = list.size();
   for (_size i = 0; this->uroboros.state == State::s_Running && i < length; i++) {
      print(this->uroboros, list[i]);
   }
}

void C_PrintDefinition::run()
{
   while (this->value->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->value->reset();
         break;
      }
      print(this->uroboros, this->value->getValue());
   }
}

void C_PrintThis_Str::run()
{
   print(this->uroboros, this->variable.value);
}

void C_PrintThis_Num::run()
{
   print(this->uroboros, this->variable.value.toString());
}

void C_PrintThis_Tim::run()
{
   print(this->uroboros, this->variable.value.toString());
}

void C_SleepPeriod::run()
{
   os_sleepForMs(1000LL * this->value->getValue().toSeconds(), this->uroboros);
}

void C_SleepMs::run()
{
   os_sleepForMs(this->value->getValue().toInt(), this->uroboros);
}

void C_Break::run()
{
   this->uroboros.state = State::s_Break;
}

void C_Continue::run()
{
   this->uroboros.state = State::s_Continue;
}

void C_Exit::run()
{
   this->uroboros.state = State::s_Exit;
}

void C_Error::run()
{
   this->uroboros.state = State::s_Exit;
   this->uroboros.exitCode = EXITCODE_RUNTIME_ERROR;
}

void C_ErrorWithExitCode::run()
{
   this->uroboros.state = State::s_Exit;
   const _int code = static_cast<_int>(this->exitCode->getValue().toInt());
   this->uroboros.exitCode = (code == EXITCODE_OK)
      ? EXITCODE_RUNTIME_ERROR
      : code;
}

void C_Run::run()
{
   _str command = this->value->getValue();
   os_rawTrim(command);

   if (command.empty()) {
      commandLog(this->uroboros, L"Failed to run an empty command");
      this->inner.success.value = false;
      return;
   }

   const _bool s = os_run(command, this->uroboros);
   this->inner.success.value = s;

   if (s) {
      commandLog(this->uroboros, L"Run '", command, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run '", command, L"'");
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

void C_RunWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->inner.exists.value || base.empty()) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
      this->inner.success.value = false;
      return;
   }

   const _str com = str(base, L" ", os_quoteEmbraced(this->inner.trimmed));
   const _bool s = os_run(com, this->uroboros);
   this->inner.success.value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

void C_RunWithWithString::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->inner.exists.value || base.empty()) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
      this->inner.success.value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(base, L" ", os_quoteEmbraced(this->inner.trimmed), L" ", arg);

   const _bool s = os_run(com, this->uroboros);
   this->inner.success.value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->inner.trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

void C_RunWithWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->inner.exists.value || base.empty()) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
      this->inner.success.value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(base, L" ", os_quoteEmbraced(this->inner.trimmed));
      const _bool s = os_run(com, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with '", base, L"'");
      }
   }
   else {
      _stream comStream;
      _stream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(this->inner.trimmed), L" with '", base, L"' with '", first, L"'");
      comStream << str(base, L" ", os_quoteEmbraced(this->inner.trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _bool s = os_run(com, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", logStream.str());
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", logStream.str());
      }
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

void C_RunWithUroboros::run()
{
   if (!this->inner.exists.value) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with Uroboros");
      this->inner.success.value = false;
      return;
   }

   const _str com = str(this->inner.urocom, os_quoteEmbraced(this->inner.trimmed));
   const _bool s = os_run(com, this->uroboros);
   this->inner.success.value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->inner.trimmed), L" with Uroboros");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with Uroboros");
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

void C_RunWithUroborosWithString::run()
{
   if (!this->inner.exists.value) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with Uroboros");
      this->inner.success.value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(this->inner.urocom, os_quoteEmbraced(this->inner.trimmed), L" ", arg);

   const _bool s = os_run(com, this->uroboros);
   this->inner.success.value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->inner.trimmed), L" with Uroboros with '", rawArg, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with Uroboros with '", rawArg, L"'");
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

void C_RunWithUroborosWith::run()
{
   if (!this->inner.exists.value) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with Uroboros");
      this->inner.success.value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(this->inner.urocom, os_quoteEmbraced(this->inner.trimmed));
      const _bool s = os_run(com, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", getCCName(this->inner.trimmed), L" with Uroboros");
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", getCCName(this->inner.trimmed), L" with Uroboros");
      }
   }
   else {
      _stream comStream;
      _stream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(this->inner.trimmed), L" with Uroboros with '", first, L"'");
      comStream << str(this->inner.urocom, os_quoteEmbraced(this->inner.trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _bool s = os_run(com, this->uroboros);
      this->inner.success.value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", logStream.str());
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", logStream.str());
      }
   }

   if (hasAttribute) {
      this->attribute->run();
   }
}

}
